/*
 * Copyright 2015 Alibaba Group Holding Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NlsVoiceRecorder.m
 * NlsDemo
 * Created by Shawn Chain on 13-11-22.
 * Copyright (c) 2018年 Alibaba iDST. All rights reserved.
 */

#import "NlsVoiceRecorder.h"

#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIApplication.h>
#import <AVFoundation/AVFoundation.h>

#define QUEUE_BUFFER_COUNT 3
#define QUEUE_BUFFER_SIZE 640
#define PCM_FRAME_BYTE_SIZE 640

typedef enum {
    STATE_INIT = 0,
    STATE_START,
    STATE_STOP
}NlsVoiceRecorderState;

NSString * const kNlsVoiceRecorderErrorDomain = @"NlsVoiceRecorderErrorDomain";

#pragma mark - NlsVoiceRecorder Implementation

@interface NlsVoiceRecorder(){
    AudioQueueRef mQueue;
    BOOL _inBackground;
}
@property(atomic,assign) NlsVoiceRecorderState state;
@property(nonatomic,strong) NSMutableData *bufferedVoiceData;
@property(nonatomic,assign,readwrite) NSUInteger currentVoiceVolume;
@property(nonatomic,copy) NSString *originCategory;

@end


@implementation NlsVoiceRecorder

-(id)init{
    self = [super init];
    if(self){
        
        static BOOL _audioSessionInited = NO;
        if(!_audioSessionInited){
            // Force to initialize the audio session once, but deprecated in iOS 7. See apple doc for more
            _audioSessionInited = YES;
            AudioSessionInitialize(NULL, NULL, NULL, NULL);
        }
        self.bufferedVoiceData = [NSMutableData data];
        // register for app resign/active notifications for recorder state
        [self _registerForBackgroundNotifications];
    }
    return self;
}

-(void)dealloc{
    [self _unregisterForBackgroundNotifications];
    
    
    [self stop:NO];
    
    [self _disposeAudioQueue];
    
    self.originCategory=nil;
    
    //    LOG(@"dealloc %@",self);
    //    [super dealloc];
}

-(void)start{
    // perform the permission check
    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    self.originCategory = audioSession.category;
    //    DBG(@"session before recognize: %@", audioSession.category);
    [audioSession setCategory:AVAudioSessionCategoryRecord error:nil];
//    
//    NSArray<AVAudioSessionDataSourceDescription*> *descriptions = audioSession.inputDataSources;
//    for (AVAudioSessionDataSourceDescription *description in descriptions) {
//        NSLog(@"cccccccccccccc %@",description.description);
//    }
//    
//    
//    [audioSession setInputDataSource:AVAudioSessionOrientationTop error:nil];
    if ([audioSession respondsToSelector:@selector(requestRecordPermission:)]) {
        [audioSession performSelector:@selector(requestRecordPermission:) withObject:^(BOOL allow){
            if(allow){
                [self _start];
                
            }else{
                // no permission
                //                [((NSObject*)_delegate) performSelectorOnMainThread:@selector(voiceDidFail:) withObject:[NSError errorMicNotAvailable] waitUntilDone:NO];
            }
        }];
    }else{
        [self _start];
    }
}

-(void)_start{
    if(self.state == STATE_START){
        return;
    }
    
    if([self _createAudioQueue] && [self _startAudioQueue]){
        
        //        [self _startAudioQueue];
        
        self.bufferedVoiceData = [NSMutableData data];
//        [_recordedVoiceData setLength:0 ];
        self.state = STATE_START;
        // we're started, notify the delegate
        if([_delegate respondsToSelector:@selector(recorderDidStart)]){
            dispatch_async(dispatch_get_main_queue(), ^{
                [_delegate recorderDidStart];
            });
            //
            //            if(true)
            //                [((NSObject*)_delegate) performSelectorOnMainThread:@selector(recorderDidStart) withObject:nil waitUntilDone:NO];
            //            else
            //                [_delegate recorderDidStart];
        }
    }else{
        
        //** when _startAudioQueue failed ,return [NSError errorVoiceNotRecognized]
        //        NSError *error = [NSError errorWithDomain:kNlsVoiceRecorderErrorDomain code:0 userInfo:@{NSLocalizedDescriptionKey:@"Failed to create audio queue"}];
        //        [((NSObject*)_delegate) performSelectorOnMainThread:@selector(voiceDidFail:) withObject:error waitUntilDone:NO];
        //        [((NSObject*)_delegate) performSelectorOnMainThread:@selector(voiceDidFail:) withObject:[NSError errorVoiceNotRecognized] waitUntilDone:NO];
    }
}

-(void)stop:(BOOL)shouldNotify{
    if(self.state == STATE_STOP){
        return;
    }
    
    self.state = STATE_STOP;
    
    [self _stopAudioQueue];
    [self _disposeAudioQueue];
    
    self.bufferedVoiceData = nil;
    [[AVAudioSession sharedInstance] setCategory:self.originCategory error:nil];
    AVAudioSession *audioSession = [AVAudioSession sharedInstance];
    //    DBG(@"session after recognize: %@", audioSession.category);
    
    if(shouldNotify && _delegate){
        dispatch_async(dispatch_get_main_queue(), ^{
            [_delegate recorderDidStop];
        });
        //        if(true)
        //            [((NSObject*)_delegate) performSelectorOnMainThread:@selector(recorderDidStop) withObject:nil waitUntilDone:NO];
        //        else
        //            [_delegate recorderDidStop];
    }
}

-(BOOL)isStarted{
    return self.state == STATE_START;
}

#pragma mark - Internal implementations

-(void)_updateCurrentVoiceVolume{
    if (mQueue) {
        //FIXME - delay calculate the volume
        static int skipFrame = 0;
        if(skipFrame++ == 3){
            skipFrame = 0;
            // 如果要获得多个通道数据，需要用数组
            // 这里没有去处理多个通道的数据显示,直接就显示最后一个通道的结果了
            UInt32 data_sz = sizeof(AudioQueueLevelMeterState);
            AudioQueueLevelMeterState levelMeter;
            OSErr status = AudioQueueGetProperty(mQueue, kAudioQueueProperty_CurrentLevelMeterDB, &levelMeter, &data_sz);
            if (status == noErr) {
                _currentVoiceVolume = (levelMeter.mAveragePower+50)*2;
            }
        }
    }
}

static void inputBufferHandler(void *                          inUserData,
                               AudioQueueRef                   inAQ,
                               AudioQueueBufferRef             inBuffer,
                               const AudioTimeStamp *          inStartTime,
                               UInt32                          inNumberPacketDescriptions,
                               const AudioStreamPacketDescription *inPacketDescs){
    @autoreleasepool {
        
        NlsVoiceRecorder *recorder = (__bridge NlsVoiceRecorder*) inUserData;
        if(recorder.isStarted){
            // 有时候AuduioQueueBuffer大小并非是预设的640，需要缓冲
            NSData *frame = [recorder _bufferPCMFrame:inBuffer];
            if(frame){
                [recorder _handleVoiceFrame:frame];
            }
            AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, NULL);
        }else{
            NSLog(@"WARN: - recorder is stopped, ignoring the callback data %d bytes",(int)inBuffer->mAudioDataByteSize);
        }
    }
}

/*
 * Allocate audio queue and buffers
 */
-(BOOL) _createAudioQueue{
    @synchronized(self){
        if(mQueue != NULL){
            return YES;
        }
        // parameters 设置AudioQueue相关参数
        AudioStreamBasicDescription format;
        memset(&format, 0, sizeof(format));
        format.mFormatID = kAudioFormatLinearPCM;
        format.mSampleRate = 16000;
        format.mChannelsPerFrame = 1;
        format.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
        format.mBitsPerChannel = 16;
        format.mBytesPerPacket =  (format.mBitsPerChannel >> 3) * format.mChannelsPerFrame;
        format.mBytesPerFrame = format.mBytesPerPacket;
        format.mFramesPerPacket = 1;
        
        // queue
        OSStatus result = AudioQueueNewInput(&format, inputBufferHandler, (__bridge void * _Nullable)(self), NULL, NULL, 0, &mQueue);
        if (result != noErr) {
            mQueue = NULL;
            return NO;
        }
        AudioQueueSetParameter(mQueue, kAudioQueueParam_Volume, 1.0f);

        //        DBG(@"AudioQueue created suceesss!");
        return YES;
    }
}

-(void) _disposeAudioQueue{
    if(mQueue == NULL){
        return;
    }
    
    AudioQueueDispose(mQueue, true);
    mQueue = NULL;
    //    DBG(@"AudioQueue disposed");
}

-(BOOL) _startAudioQueue{
    
    //    [self _createAudioQueue];
    
    NSAssert(mQueue,@"mQueue is null");
    
    OSStatus result = nil;
    
    // buffers
    AudioQueueBufferRef queueBuffer;
    for (int i = 0; i < QUEUE_BUFFER_COUNT; ++i) {
        queueBuffer = NULL;
        if((result = AudioQueueAllocateBuffer(mQueue, QUEUE_BUFFER_SIZE, &queueBuffer) != noErr)){
            NSLog(@"AudioQueueAllocateBuffer error %d", (int)result);
            [self _disposeAudioQueue];
            return NO;
        }
        if((result = AudioQueueEnqueueBuffer(mQueue, queueBuffer, 0, NULL)) != noErr) {
            NSLog(@"AudioQueueEnqueueBuffer error %d", (int)result);
            [self _disposeAudioQueue];
            return NO;
        }
    }
    
    if ((result = AudioQueueStart(mQueue, NULL)) != noErr) {
        NSLog(@"AudioQueueStart error %d",(int)result);
        [self _disposeAudioQueue];
        return NO;
    }
    
    //TODO - do we need level metering?
    UInt32 val = 1;
    AudioQueueSetProperty(mQueue, kAudioQueueProperty_EnableLevelMetering, &val, sizeof(UInt32));
    
    //    AudioSessionSetActive(YES);
    //    DBG(@"AudioQueue started");
    
    return YES;
}

-(void) _stopAudioQueue{
    if(mQueue == NULL){
        return;
    }
    AudioQueueStop(mQueue, true);
    AudioSessionSetActive(NO);
    //    DBG(@"AudioQueue stopped");
}


/*
 * AudioQueue 返回的 frame长度不确定，这里做一个缓冲，确保满了640bytes以后，返回。
 * 640 bytes = 320 frames/16bit = 20ms
 */

- (NSData*) _bufferPCMFrame:(AudioQueueBufferRef)aqBuffer{
    NSAssert(_bufferedVoiceData != nil,@"_bufferVoiceData is nil" );
    
    NSInteger nBufferSpaceLeft = PCM_FRAME_BYTE_SIZE - _bufferedVoiceData.length;
    
    NSInteger nBytesReceived = aqBuffer->mAudioDataByteSize;
    NSInteger nBytesToCopy = nBufferSpaceLeft >= nBytesReceived ?nBytesReceived:nBufferSpaceLeft;
    NSInteger nBytesLeft = nBytesReceived - nBytesToCopy;
    
    [_bufferedVoiceData appendBytes:aqBuffer->mAudioData length:nBytesToCopy];
    
    //    DBG(@"+ r:%ud/c:%ud/l:%ud",nBytesReceived,nBytesToCopy,nBytesLeft);
    
    if(_bufferedVoiceData.length == PCM_FRAME_BYTE_SIZE){
        // buffer is full
        NSData *frame = [NSData dataWithData:_bufferedVoiceData];
        // reset the buffer
        _bufferedVoiceData.length = 0;
        
        // save the left partial data
        if(nBytesLeft > 0){
            //DBG(@"full: received %u bytes, copied %u bytes, left %u bytes",nBytesReceived, nBytesToCopy,nBytesLeft);
            [_bufferedVoiceData appendBytes:(aqBuffer->mAudioData + nBytesToCopy) length:nBytesLeft];
        }
        return frame;
    }else{
        //        DBG(@"Buffering, %@ of %u received",@(nBytesReceived),PCM_FRAME_BYTE_SIZE);
    }
    
    return nil;
}

/*
 * FIXME - 在回调中，可能涉及了耗时的压缩操作，是否要在主线程中执行？
 */
-(void) _handleVoiceFrame:(NSData*)voiceFrame {
    [self _updateCurrentVoiceVolume];
    if(_delegate){
        if(true){
            [_delegate voiceRecorded:voiceFrame];
        }else{
            [((NSObject*)_delegate) performSelectorOnMainThread:@selector(voiceRecorded:) withObject:voiceFrame waitUntilDone:NO];
        }
    }
}

#pragma mark - Background Notifications
- (void)_registerForBackgroundNotifications {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_appResignActive)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(_appEnterForeground)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
}

- (void)_unregisterForBackgroundNotifications {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}


- (void)_appResignActive {
    // TODO - stop recorder if any
    _inBackground = true;
    AudioSessionSetActive(NO);
}

- (void)_appEnterForeground {
    // TODO - resume recorder if any
    _inBackground = false;
    //    AudioSessionSetActive(YES);
}

@end
