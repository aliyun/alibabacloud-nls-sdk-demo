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

#ifdef _WIN32

#pragma warning( push )
#pragma warning ( disable : 4251 )

#ifndef  ASR_API
#define ASR_API _declspec(dllexport)
#endif
#else
#define ASR_API
#endif

#include "pthread.h"

namespace AlibabaNls {

class SpeechRecognizerCallback;
class SpeechRecognizerRequest;
class SpeechTranscriberCallback;
class SpeechTranscriberRequest;
class SpeechSynthesizerCallback;
class SpeechSynthesizerRequest;

enum LogLevel {
    LogError = 1,
    LogWarning,
    LogInfo,
    LogDebug
};

class ASR_API NlsClient {
public:

/** 
    * @brief 设置日志文件与存储路径
    * @param logOutputFile	日志文件
    * @param logLevel	日志级别，默认1（LogError : 1, LogWarning : 2, LogInfo : 3, LogDebug : 4）
    * @param logFileSize 日志文件的大小，以MB为单位，默认为10MB；
    *                    如果日志文件内容的大小超过这个值，SDK会自动备份当前的日志文件，最多可备份5个文件，超过后会循环覆盖已有文件
    * @return 成功则返回0，失败返回-1
    */	
int setLogConfig(const char* logOutputFile, LogLevel logLevel, unsigned int logFileSize = 10);

/**
    * @brief 创建一句话识别对象
    * @param onResultReceivedEvent	事件回调接口
    * @return 成功返回speechRecognizerRequest对象，否则返回NULL
    */
SpeechRecognizerRequest* createRecognizerRequest(SpeechRecognizerCallback* onResultReceivedEvent);

/**
    * @brief 销毁一句话识别对象
    * @param request  createRecognizerRequest所建立的request对象
    * @return
    */
void releaseRecognizerRequest(SpeechRecognizerRequest* request);

/**
    * @brief 创建实时音频流识别对象
    * @param onResultReceivedEvent	事件回调接口
    * @return 成功返回SpeechTranscriberRequest对象，否则返回NULL
    */
SpeechTranscriberRequest* createTranscriberRequest(SpeechTranscriberCallback* onResultReceivedEvent);

/**
    * @brief 销毁实时音频流识别对象
    * @param request  createTranscriberRequest所建立的request对象
    * @return
    */
void releaseTranscriberRequest(SpeechTranscriberRequest* request);

/**
    * @brief 创建语音合成对象
    * @param onResultReceivedEvent	事件回调接口
    * @return 成功则SpeechSynthesizerRequest对象，否则返回NULL
    */
SpeechSynthesizerRequest* createSynthesizerRequest(SpeechSynthesizerCallback* onResultReceivedEvent);

/**
    * @brief 销毁语音合成对象
    * @param request  createSynthesizerRequest所建立的request对象
    * @return
    */
void releaseSynthesizerRequest(SpeechSynthesizerRequest* request);

/**
    * @brief NlsClient对象实例
    * @param sslInitial	是否初始化openssl 线程安全，默认为true
    * @return NlsClient对象
    */
static NlsClient* getInstance(bool sslInitial = true);

/**
    * @brief 销毁NlsClient对象实例
    * @note releaseInstance()非线程安全.
    * @return
    */
static void releaseInstance();

private:

NlsClient();
~NlsClient();

static pthread_mutex_t _mtx;
static bool _isInitializeSSL;
static NlsClient* _instance;

};

}

#ifdef _WIN32
#pragma warning( pop )
#endif

#endif
