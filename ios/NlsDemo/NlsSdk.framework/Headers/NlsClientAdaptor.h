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
#import "NlsSpeechRecognizerRequest.h"
#import "NlsSpeechSynthesizerRequest.h"
#import "NlsSpeechTranscriberRequest.h"
#import "NlsSpeechRecognizerRequestwithRecorder.h"
#import "NlsSpeechTranscriberRequestwithRecorder.h"

@interface NlsClientAdaptor: NSObject

/**
 * @brief 构造函数
 * @note 全局只能创建一个。
 * @param ifSSL 是否初始化openssl为线程安全，默认为true
 */
-(id)init:(Boolean)ifSSL;



/**
 * @brief 创建一句话语音识别RecognizeRequest对象
 * @return 成功则RecognizeRequest对象，否则返回NULL
 */
-(NlsSpeechRecognizerRequest*)createRecognizerRequest;

/**
 * @brief 创建一句话语音识别RecognizeRequest对象
 * @note  包含录音功能
 * @return 成功则RecognizeRequest对象，否则返回NULL
 */
-(NlsSpeechRecognizerRequestwithRecorder*)createRecognizerRequestwithRecorder;

/**
 * @brief 创建语音合成SynthesizerRequest对象
 * @return 成功则SynthesizerRequest对象，否则返回NULL
 */
-(NlsSpeechSynthesizerRequest*)createSynthesizerRequest;

/**
 * @brief 创建实时语音识别TranscriberRequest对象
 * @return 成功则TranscriberRequest对象，否则返回NULL
 */
-(NlsSpeechTranscriberRequest*)createTranscriberRequest;

/**
 * @brief 创建实时语音识别TranscriberRequest对象
 * @return 成功则TranscriberRequest对象，否则返回NULL
 */
-(NlsSpeechTranscriberRequestwithRecorder*)createTranscriberRequestwithRecorder;

/**
 * @brief 设置日志文件与存储路径
 * @param path    日志文件保存路径
 * @param level    日志级别，默认1（Error : 1、Warn : 2、Debug : 3）
 */
-(void)setLog:(NSString*)path logLevel:(NSInteger)level;

//类方法
+(id)getInstance;
@end
