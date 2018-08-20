//
//  NLSPlayAudio.m
//  AliyunNlsClientSample
//
//  Created by 刘方 on 2/25/16.
//  Copyright © 2016 阿里巴巴. All rights reserved.
//

#import "NLSPlayAudio.h"

#define AVCODEC_MAX_AUDIO_FRAME_SIZE  4096*2// (0x10000)/4
//static UInt32 gBufferSizeBytes=0x10000;//65536
static UInt32 gBufferSizeBytes=0x50000;//It must be pow(2,x)

@interface NLSPlayAudio() {
    Byte *audioByte;
    long audioDataIndex;
    long audioDataLength;
    UInt32 numPacketsToRead;
    uint8_t *inbuf;
}

@end

@implementation NLSPlayAudio

- (id) init {
    self = [super init];
    [self cleanup];
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayback withOptions:AVAudioSessionCategoryOptionDefaultToSpeaker error:nil];
//    [[AVAudioSession sharedInstance] overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker error:nil];
    
    ///设置音频参数
    audioDescription.mSampleRate  = 16000;//采样率
    audioDescription.mFormatID    = kAudioFormatLinearPCM;
    audioDescription.mFormatFlags =  kAudioFormatFlagIsSignedInteger|kAudioFormatFlagIsNonInterleaved;
    audioDescription.mChannelsPerFrame = 1;
    audioDescription.mFramesPerPacket  = 1;//每一个packet一侦数据
    audioDescription.mBitsPerChannel   = 16;//av_get_bytes_per_sample(AV_SAMPLE_FMT_S16)*8;//每个采样点16bit量化
    audioDescription.mBytesPerPacket   = 2;
    audioDescription.mBytesPerFrame    = 2;
    audioDescription.mReserved = 0;
    
    //使用player的内部线程播 创建AudioQueue
    AudioQueueNewOutput(&audioDescription, bufferCallback, (__bridge void *)(self), nil, nil, 0, &audioQueue);
    
    return self;
}


-(void)process:(Byte *)byte length:(long)len {
    NSLog(@"process Audio byte >>>>> ");
    audioByte = byte;
    audioDataLength = len;
    
    numPacketsToRead= gBufferSizeBytes/audioDescription.mBytesPerPacket;
    audioDataIndex = 0;
    if(audioQueue)
    {
        ////添加buffer区 创建Buffer
        for(int i=0;i < NUM_BUFFERS; i++) {
            int result =  AudioQueueAllocateBuffer(audioQueue, gBufferSizeBytes, &audioQueueBuffers[i]);
            //读取包数据
            if ([self readPacketsIntoBuffer:audioQueueBuffers[i]]==1) {
                break;
            }
        }
    }
    
    [self play];
    
}

- (void)play {
//    NSLog(@"Audio Play Start >>>>> ");
    Float32 gain=1.0;
    //设置音量
    AudioQueueSetParameter(audioQueue, kAudioQueueParam_Volume, gain);
    //开始播放
    AudioQueueStart(audioQueue, NULL);
}

//回调函数(Callback)的实现
static void bufferCallback(void *inUserData,AudioQueueRef inAQ,AudioQueueBufferRef buffer) {
//    NSLog(@"bufferCallback processAudioData :%u", (unsigned int)buffer->mAudioDataByteSize);
    
}


-(UInt32)readPacketsIntoBuffer:(AudioQueueBufferRef)buffer {
    UInt32 numPackets;
    //从文件中接受数据并保存到缓存(buffer)中
    numPackets = numPacketsToRead;
    AudioQueueBufferRef outBufferRef = buffer;
    
    if(audioDataIndex < audioDataLength){
        memcpy(outBufferRef->mAudioData, audioByte+audioDataIndex, audioDataLength);
        outBufferRef->mAudioDataByteSize = (UInt32)audioDataLength;
        AudioQueueEnqueueBuffer(audioQueue, outBufferRef, 0, nil);
        audioDataIndex += numPackets;
    }
    else{
        return 1;//意味着我们没有读到任何的包
    }
    return 0;//0代表正常的退出
}

-(void)cleanup {
    if(audioQueue)
    {
        NSLog(@"Release AudioQueueNewOutput");
        
        [self stop];
        for(int i=0; i < QUEUE_BUFFER_SIZE; i++)
        {
            AudioQueueFreeBuffer(audioQueue, audioQueueBuffers[i]);
            audioQueueBuffers[i] = nil;
        }
        audioQueue = nil;
    }
}

-(void)stop {
    NSLog(@"Audio Player Stop");
    AudioQueueStop(audioQueue,TRUE);
    AudioQueueReset(audioQueue);
    AudioQueueDispose(audioQueue, TRUE);
}

@end
