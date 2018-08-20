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
 * NlsSpeechSynthesizerRequest.h
 * NlsSdk
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import "SynthesizerRequestParam.h"
#import "NlsDelegateEvent.h"

//回调方法
@protocol NlsSpeechSynthesizerDelegate <NSObject>
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
 * @brief 语音合成开始回调
 * @note  当语音合成开始时，回调该方法
 * @param event 回调方法
 * @param statusCode 状态码
 * @param result 服务端返回消息
 * @param eMsg 错误消息
 */
-(void)OnSynthesizerStarted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;
/**
 * @brief 语音合成结束回调
 * @note  当语音合成结束时，回调该方法
 * @param event 回调方法
 * @param statusCode 状态码
 * @param result 服务端返回消息
 * @param eMsg 错误消息
 */
-(void)OnSynthesizerCompleted:(NlsDelegateEvent)event result:(NSString *)result statusCode:(NSString*)statusCode errorMessage:(NSString*)eMsg;

/**
 * @brief 语音合成二进制语音流数据回调接口
 * @note  通过本接口回调语音合成二进制语音流数据
 * @param event  回调方法
 * @param data   Byte*类型数据
 * @param length 数据长度
 */
-(void)OnBinaryDataReceived:(NlsDelegateEvent)event voiceData:(Byte *)data length:(NSInteger)length;
@end

@interface NlsSpeechSynthesizerRequest: NSObject
@property(nonatomic,weak) id<NlsSpeechSynthesizerDelegate> delegate;

/**
 * @brief 设置SynthesizerRequestParam对象参数
 */
-(void)setSynthesizerParams:(SynthesizerRequestParam *)requestParam;

/**
 * @brief 启动SynthesizerRequest
 * @return 成功则返回0，否则返回-1
 */
-(int)start;
@end
