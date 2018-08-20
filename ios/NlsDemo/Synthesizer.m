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
 * Synthesizer.m
 * NlsDemo
 * Created by Songsong Shao on 2018/6/27.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "Synthesizer.h"

@interface Synthesizer()<NlsSpeechSynthesizerDelegate>{
    IBOutlet UITextView *textViewSynthesizer;
}
@end

@implementation Synthesizer

-(void)viewDidLoad{
    [super viewDidLoad];
    //1. 全局参数初始化操作
    //1.1 初始化语音合成客户端
    _nlsClient = [[NlsClientAdaptor alloc]init];
    
    //1.2 初始化语音播放工具类
    _nlsAudioPlayer = [[NLSPlayAudio alloc]init];
    
    //1.3 初始化合成参数类
    _requestParam = [[SynthesizerRequestParam alloc]init];
    
    //1.4 设置log级别
    [_nlsClient setLog:NULL logLevel:1];
}

-(IBAction)startSynthesizer{
    //2. 创建请求对象和开始语音合成
    if(_synthesizerRequest!= NULL){
        _synthesizerRequest = NULL;
    }
    //2.1 初始化语音播放类
    [_nlsAudioPlayer cleanup];
    _nlsAudioPlayer = [[NLSPlayAudio alloc]init];
    
    //2.2 创建请求对象，设置NlsSpeechSynthesizerRequest回调
    _synthesizerRequest = [_nlsClient createSynthesizerRequest];
    _synthesizerRequest.delegate = self;
    
    //2.3 获取页面合成文本
    NSString *inputText = [textViewSynthesizer text];
    
    //2.4 设置SynthesizerRequestParam请求参数
    [_requestParam setFormat:@"pcm"];
    [_requestParam setText:inputText];
    //请使用https://help.aliyun.com/document_detail/72153.html 动态生成token
    // <AccessKeyId> <AccessKeySecret> 请使用您的阿里云账户生成 https://ak-console.aliyun.com/
    [_requestParam setToken:@""];
    //请使用阿里云语音服务管控台(https://nls-portal.console.aliyun.com/)生成您的appkey
    [_requestParam setAppkey:@""];
    
    //2.5 传入请求参数
    [_synthesizerRequest setSynthesizerParams:_requestParam];
    
    //2.6 开始语音合成
    [_synthesizerRequest start];
    
}

/**
 *3. NlsSpeechSynthesizerDelegate接口回调
 */

//3.1 本次请求失败
- (void)OnTaskFailed:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    NSLog(@"OnTaskFailed, statusCode is: %@ error message ：%@",statusCode,eMsg);
}

//3.2 服务端连接关闭
- (void)OnChannelClosed:(NlsDelegateEvent)event statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
    NSLog(@"OnChannelClosed, statusCode is: %@",statusCode);
}

//3.3 回调合成语音数据，通过NlsAudioPlayer工具播放
- (void)OnBinaryDataReceived:(NlsDelegateEvent)event voiceData:(Byte *)data length:(NSInteger)length{
    NSLog(@"Received voice data length %lu", length);
    [_nlsAudioPlayer process:data length:length];
}

//3.4 合成结束
- (void)OnSynthesizerCompleted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
}

//3.5 合成开始
- (void)OnSynthesizerStarted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString *)statusCode errorMessage:(NSString *)eMsg {
}

@end
