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

#ifndef NLS_SDK_EVENT_H
#define NLS_SDK_EVENT_H

#ifdef _WIN32

#pragma warning( push )
#pragma warning ( disable : 4251 )

#ifndef  ASR_API
#define ASR_API _declspec(dllexport)
#endif
#else
#define ASR_API
#endif

#include <string>
#include <vector>

namespace AlibabaNls {

class ASR_API NlsEvent {
public:

enum EventType {
    TaskFailed = 0,
	RecognitionStarted,
	RecognitionCompleted,
	RecognitionResultChanged,
	TranscriptionStarted,
	SentenceBegin,
	TranscriptionResultChanged,
	SentenceEnd,
	TranscriptionCompleted,
	SynthesisStarted,
	SynthesisCompleted,
	Binary,
    Close  /*语音功能通道连接关闭*/
};

/**
    * @brief NlsEvent构造函数
    * @param event    NlsEvent对象
    */
NlsEvent(NlsEvent&  event);

/**
    * @brief NlsEvent构造函数
    * @param msg    Event消息字符串
    * @param code   Event状态编码
    * @param type    Event类型
    * @param taskId 任务的task id
    */
NlsEvent(std::string msg, int code, EventType type, std::string taskId);

/**
    * @brief NlsEvent构造函数
    * @param msg    Event消息字符串
    */
NlsEvent(std::string msg);

/**
    * @brief 解析消息字符串
    * @note SDK内部函数
    * @return 成功返回0，失败返回-1, 抛出异常
*/
int parseJsonMsg();

/**
    * @brief NlsEvent构造函数
    * @param data    二进制数据
    * @param code   Event状态编码
    * @param type    Event类型
    * @param taskId 任务的task id
    * @return
    */
NlsEvent(std::vector<unsigned char> data, int code, EventType type, std::string taskId);

/**
    * @brief NlsEvent析构函数
    */
~NlsEvent();

/**
    * @brief 获取状态码
    * @note 正常情况为0或者20000000，失败时对应失败的错误码。错误码参考SDK文档说明。
    * @return int
    */
int getStausCode();

/**
    * @brief 获取云端返回的识别结果
    * @note json格式
    * @return const char*
    */
const char* getAllResponse();

/**
    * @brief 在TaskFailed回调中，获取NlsRequest操作过程中出现失败时的错误信息
    * @note 在Failed回调函数中使用
    * @return const char*
    */
const char* getErrorMessage();

/**
    * @brief 获取任务的task id
    * @return const char*
    */
const char* getTaskId();

/**
    * @brief 获取一句话识别或者实时语音识别的识别结果
    * @return const char*
    */
const char* getResult();

/**
    * @brief 获取实时语音检测的句子编号
    * @note 只有在实时语音检测功能才能获得识别句子的编号
    * @result int
    */
int getSentenceIndex();

/**
    * @brief 获取实时语音检测的句子的音频时长，单位是毫秒
    * @note 只有在实时语音检测功能才能获得识别句子的音频时长
    * @result int
    */
int getSentenceTime();


/**
    * @brief 获取云端返回的二进制数据
    * @note 仅用于语音合成功能
    * @return vector<unsigned char>
    */
std::vector<unsigned char> getBinaryData();

/**
    * @brief 获取当前所发生Event的类型
    * @return EventType
    */
EventType getMsgType();

private:
int parseMsgType(std::string name);

private:

int _statusCode;
std::string _msg;
EventType _msgtype;
std::string _taskId;
std::string _result;
int _sentenceIndex;
int _sentenceTime;
std::vector<unsigned char> _binaryData;

};

typedef void(*NlsCallbackMethod)(NlsEvent*, void*);

}

#ifdef _WIN32
#pragma warning( pop )
#endif

#endif //NLS_SDK_EVENT_H
