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
 * NlsVoiceRecorder.h
 * NlsDemo
 * Created by Shawn Chain on 13-11-22.
 * Copyright (c) 2018年 Alibaba iDST. All rights reserved.
 */


#import <Foundation/Foundation.h>

/**
 *@discuss NlsVoiceRecorder 各种回调接口
 */
@protocol NlsVoiceRecorderDelegate <NSObject>

/**
 * @discuss Recorder启动回调，在主线程中调用
 */
-(void) recorderDidStart;

/**
 * @discuss Recorde停止回调，在主线程中调用
 */
-(void) recorderDidStop;

/**
 * @discuss Recorder收录到数据，通常涉及VAD及压缩等操作，为了避免阻塞主线，因此将在在AudioQueue的线程中调用，注意线程安全！！！
 */
-(void) voiceRecorded:(NSData*) frame;


/**
 * @discuss Recorder录制录音时返回音量大小
 */
-(void) voiceVolume:(NSInteger)volume;

/**
 *@discussion 录音机无法打开或其他错误的时候会回调
 */
-(void) voiceDidFail:(NSError*)error;
@end



/**
 *@discuss 封装了AudioQueue C API的录音机程序
 */
@interface NlsVoiceRecorder : NSObject

@property(nonatomic,assign) id<NlsVoiceRecorderDelegate> delegate;

@property(nonatomic,readonly) NSUInteger currentVoiceVolume;

/**
 * 开始录音
 */
-(void)start;

/**
 * 停止录音
 */
-(void)stop:(BOOL)shouldNotify;

/**
 * 是否在录音
 */
-(BOOL)isStarted;
@end
