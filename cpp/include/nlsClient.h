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
 */

#ifndef NLS_SDK_CLIENT_H
#define NLS_SDK_CLIENT_H

#ifdef _WINDOWS_COMPILE_
#ifndef  ASR_API
#define ASR_API _declspec(dllexport)
#endif
#else
#define ASR_API
#endif

#include "pthread.h"

class SpeechRecognizerCallback;
class SpeechRecognizerRequest;

class ASR_API NlsClient {
public:

~NlsClient();

/** 
    * @brief 设置日志文件与存储路径
    * @param logOutputFile	日志文件
    * @param logLevel	日志级别，默认1（Error : 1、Warn : 2、Debug : 3）
    * @return 成功则返回0，失败返回-1
    */	
int setLogConfig(const char* logOutputFile, int logLevel);

/**
    * @brief 创建speechRecognizerRequest对象
    * @param onResultReceivedEvent	事件回调接口
    * @param config	配置文件
    * @return 成功返回speechRecognizerRequest对象，否则返回NULL
    */
SpeechRecognizerRequest* createRecognizerRequest(SpeechRecognizerCallback* onResultReceivedEvent,
                                                 const char* config);

///**
//    * @brief 创建语音识别NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createRecognizerRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);
//
///**
//    * @brief 创建语音合成NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createSynthesisRequestRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);
//
///**
//    * @brief 创建云端唤醒NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createWakeWordVerifierRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);
//
///**
//    * @brief 创建天宫NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createTiangongAssistantRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);
//
///**
//    * @brief 创建声纹识别NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createVoiceprintRecognizerRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);
//
///**
//    * @brief 创建声纹管理NlsRequest对象
//    * @param onResultReceivedEvent	事件回调接口
//    * @param config	配置文件
//    * @return 成功则NlsRequest对象，否则返回NULL
//    */
//NlsRequest* createVoiceprintManagerRequest(NlsSpeechCallback* onResultReceivedEvent, const char* config);

/**
    * @brief NlsClient对象实例
    * @param sslInitial	是否初始化openssl 线程安全，默认为true
    * @return NlsClient对象
    */
static NlsClient* getInstance(bool sslInitial = true);

private:

NlsClient();

static pthread_mutex_t _mtx;
static bool _isInitializeSSL;
static NlsClient* _instance;

};

#endif
