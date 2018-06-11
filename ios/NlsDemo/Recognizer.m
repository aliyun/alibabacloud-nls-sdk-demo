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
 * Recgnizer.m
 * NlsDemo
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */


#import <Foundation/Foundation.h>
#import "Recognizer.h"


@interface Recognizer()<NlsDelegate,NlsVoiceRecorderDelegate>{
    IBOutlet UITextView *textViewRecognize;
    IBOutlet UISwitch *switchRecognize;
}
@end

@implementation Recognizer

- (void)viewDidLoad {
    [super viewDidLoad];
    //初始化识别客户端
    _nlsClient = [[NlsClientAdaptor alloc]init];
    //创建请求
    _recognizeRequest = [_nlsClient createRecognizeRequest];
    _recognizeRequest.delegate = self;
    
    //初始化录音recorder
    _voiceRecorder = [[NlsVoiceRecorder alloc]init];
    _voiceRecorder.delegate = self;
    
    _recognizeRequestParam = [[RecognizeRequestParam alloc]init];
    
    [_nlsClient setLog:NULL logLevel:1];
}

- (IBAction)startRecognize {
    
    //设置请求参数
    [_recognizeRequestParam setFormat:@"opu"];
    [_recognizeRequestParam setEnableIntermediateResult:true];
    //请使用https://help.aliyun.com/document_detail/72153.html 动态生成token
    // <AccessKeyId> <AccessKeySecret> 请使用您的阿里云账户生成 https://ak-console.aliyun.com/
    [_recognizeRequestParam setToken:@""];
    //请使用阿里云语音服务管控台(https://nls-portal.console.aliyun.com/)生成您的appkey
    [_recognizeRequestParam setAppkey:@""];
    
    //
    [_recognizeRequest setRecognizeParams:_recognizeRequestParam];
    [_voiceRecorder start];
    [_recognizeRequest start];
    
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        [self->switchRecognize setOn:true];
        self->textViewRecognize.text = @"start Recognize！";
    });
}

- (IBAction)stopRecognize {
    [_voiceRecorder stop:true];
    [_recognizeRequest stop];
}

/**
 *识别回调，本次请求失败
 */
-(void)OnTaskFailed:(NlsDelegateEvent)event statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg{
    NSLog(@"OnTaskFailed, error message is: %@",eMsg);
}

/**
 *识别回调，服务端连接关闭
 */
-(void)OnChannelClosed:(NlsDelegateEvent)event statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg{
    NSLog(@"OnChannelClosed, statusCode is: %@",statusCode);
}

/**
 *识别回调，识别结果结束
 */
-(void)OnRecognizedCompleted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg{
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        self->textViewRecognize.text = result;
        NSLog(@"%@", result);
        [self->switchRecognize setOn:false];
    });
}

/**
 *识别回调，识别中间结果
 */
-(void)OnRecognizedResultChanged:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg{
    dispatch_async(dispatch_get_main_queue(), ^{
        // UI更新代码
        NSLog(@"%@", result);
        self->textViewRecognize.text = result;
    });
}



/**
 *录音相关回调
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

/**
 *录音数据回调
 */
- (void)voiceRecorded:(NSData *)frame {
    if (_recognizeRequest != nil) {
//        NSLog(@"recived pcm recorder data length :%lu",frame.length);
        //录音线程回调的数据传给识别服务
        [_recognizeRequest sendAudio:frame length:frame.length];
    }
}

@end