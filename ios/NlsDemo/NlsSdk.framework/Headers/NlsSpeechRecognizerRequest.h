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
 * NlsClientAdaptor.h
 * NlsSdk
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "RecognizerRequestParam.h"
#import "NlsDelegateEvent.h"


//回调方法
@protocol NlsSpeechRecognizerDelegate <NSObject>
/**
 * @brief 错误回调函数
 * @note 在请求过程中出现错误时，触发该回调。用户可以在事件的消息头中检查状态码和状态消息，以确认失败的具体原因。
 * @param event 回调方法
 * @param statusCode 状态码
 * @param eMsg 错误消息
 */
-(void)OnTaskFailed:(NlsDelegateEvent)event statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;
/**
 * @brief 与服务端连接关闭的回调
 * @note 在与服务端连接关闭时，触发该回调
 * @param event 回调方法
 * @param statusCode 状态码
 * @param eMsg 错误消息
 */
-(void)OnChannelClosed:(NlsDelegateEvent)event statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;
/**
 * @brief 语音识别结束回调函数
 * @note 在语音识别完成时，触发该回调
 * @param event 回调方法
 * @param statusCode 状态码
 * @param result 识别最终结果
 * @param eMsg 错误消息
 */
-(void)OnRecognizedCompleted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;
/**
 * @brief 语音识别中间结果回调函数
 * @note 只有response_mode为STREAMING，才会有中间结果
 * @param event 回调方法
 * @param statusCode 状态码
 * @param result 识别中间结果
 * @param eMsg 错误消息
 */
-(void)OnRecognizedResultChanged:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;
@end

@interface NlsSpeechRecognizerRequest: NSObject
@property(nonatomic,weak) id<NlsSpeechRecognizerDelegate> delegate;

/**
 * @brief 设置RecognizeRequestParam对象参数
 */
-(void)setRecognizeParams:(RecognizerRequestParam *)requestParam;

/**
 * @brief 启动RecognizeRequest
 * @return 成功则返回0，否则返回-1
 */
-(int)start;

/**
 * @brief 发送语音数据
 * @param audioData 语音数据
 * @param len 语音数据长度
 */
-(void)sendAudio:(NSData *)audioData length:(int)len;

/**
 * @brief 停止RecognizeRequest
 * @return  成功则返回0，否则返回-1
 */
-(int)stop;

-(int)releaseRequest;
@end
