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
 * Transcriber.m
 * NlsDemo
 * Created by Songsong Shao on 2018/6/27.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "Transcriber.h"

@interface Transcriber()<NlsSpeechTranscriberDelegate,NlsVoiceRecorderDelegate>{
    IBOutlet UITextView *textViewTranscriber;
    IBOutlet UISwitch *switchTranscriber;
    Boolean transcriberStarted;
}
@end

@implementation Transcriber

-(void)viewDidLoad {
    [super viewDidLoad];
    //1. 全局参数初始化操作
    //1.1 初始化识别客户端,将transcriberStarted状态置为false
    _nlsClient = [[NlsClientAdaptor alloc]init];
    transcriberStarted = false;
    
    //1.2 初始化录音recorder工具
    _voiceRecorder = [[NlsVoiceRecorder alloc]init];
    _voiceRecorder.delegate = self;
    
    //1.3 初始化识别参数类
    _transRequestParam = [[TranscriberRequestParam alloc]init];
    
    //1.4 设置log级别
    [_nlsClient setLog:NULL logLevel:1];
}

- (IBAction)startTranscriber {
    //2. 创建请求对象和开始识别
    if(_transcriberRequest!= NULL){
        [_transcriberRequest releaseRequest];
        _transcriberRequest = NULL;
    }
    //2.1 创建请求对象，设置NlsSpeechTranscriberDelegate回调
    _transcriberRequest = [_nlsClient createTranscriberRequest];
    _transcriberRequest.delegate = self;
    
    //2.2 设置TranscriberRequestParam请求参数
    [_transRequestParam setFormat:@"opu"];
    [_transRequestParam setEnableIntermediateResult:true];
    //请使用https://help.aliyun.com/document_detail/72153.html 动态生成token
    // <AccessKeyId> <AccessKeySecret> 请使用您的阿里云账户生成 https://ak-console.aliyun.com/
    [_transRequestParam setToken:@""];
    //请使用阿里云语音服务管控台(https://nls-portal.console.aliyun.com/)生成您的appkey
    [_transRequestParam setAppkey:@""];
    
    //2.3 传入请求参数
    [_transcriberRequest setTranscriberParams:_transRequestParam];
    
    //2.4 启动录音和识别，将transcriberStarted置为true
    [_voiceRecorder start];
    [_transcriberRequest start];
    transcriberStarted = true;
    
    //2.5 更新UI
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        [self->switchTranscriber setOn:true];
        self->textViewTranscriber.text = @"start Recognize！";
    });
}

- (IBAction)stopTranscriber {
    //3 结束识别 停止录音，停止识别请求
    [_voiceRecorder stop:true];
    [_transcriberRequest stop];
    transcriberStarted= false;
    _transcriberRequest = NULL;
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        [self->switchTranscriber setOn:false];
    });
}

/**
 *4. NlsSpeechTranscriberDelegate接口回调方法
 */
//4.1 识别回调，本次请求失败
- (void)OnTaskFailed:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    [_voiceRecorder stop:true];
    NSLog(@"OnTaskFailed, statusCode is: %@ error message ：%@",statusCode,eMsg);
}

//4.2 识别回调，服务端连接关闭
- (void)OnChannelClosed:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    NSLog(@"OnChannelClosed, statusCode is: %@",statusCode);
}

//4.3 实时音频流识别开始
- (void)OnTranscriptionStarted:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
}

//4.4 识别回调，一句话的开始
- (void)OnSentenceBegin:(NlsDelegateEvent)event result:(NSString*)result statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        self->textViewTranscriber.text = result;
        NSLog(@"%@", result);
    });
}

//4.5 识别回调，一句话的结束
- (void)OnSentenceEnd:(NlsDelegateEvent)event result:(NSString*)result statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        self->textViewTranscriber.text = result;
        NSLog(@"%@", result);
    });
}

//4.5 识别回调，一句话识别的中间结果
- (void)OnTranscriptionResultChanged:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        self->textViewTranscriber.text = result;
        NSLog(@"%@", result);
    });
}

//4.6 识别回调，实时音频流识别完全结束
- (void)OnTranscriptionCompleted:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    
}

/**
 *5. 录音相关回调
 */
- (void)recorderDidStart {
    NSLog(@"Did start recorder!");
}

- (void)recorderDidStop {
    NSLog(@"Did stop recorder!");
}

- (void)voiceDidFail:(NSError *)error {
    NSLog(@"Did recorder error!");
}

//5.1 录音数据回调
- (void)voiceRecorded:(NSData *)frame {
    if (_transcriberRequest != nil && transcriberStarted) {
        //录音线程回调的数据传给识别服务
        [_transcriberRequest sendAudio:frame length:(short)frame.length];
    }
}

- (void)voiceVolume:(NSInteger)volume {
    // onVoiceVolume if you need.
}

@end
