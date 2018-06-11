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

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "pthread.h"
#include "nlsClient.h"
#include "speechRecognizerRequest.h"
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <functional>
#include <exception>

#ifdef _WINDOWS_COMPILE_
#include <windows.h>
#else
#include <unistd.h>
#endif

#define FRAME_SIZE 6400

using namespace std;

SpeechRecognizerCallback* callback = NULL;
NlsClient *gNlc;

struct ParamStruct {
	std::string filename;
	std::string config;
	int threadcount;
	int count;
	std::string token;
	bool isSendData;
};

void* func(void* arg) {

	try {
		ParamStruct* tst = (ParamStruct*)arg;
		if (tst == NULL) {
			cout << "filename is not valid" << endl;
			return NULL;
		}

		ifstream fs;
		fs.open(tst->filename.c_str(), ios::binary | ios::in);
		if (!fs) {
			throw ("file isn't exist");
		}

		//createTranscriberRequest创建的request在一个会话周期内可以重复使用.
                //会话周期是一个逻辑概念. 比如Demo中，指读取, 发送完整个音频文件数据的时间.
                //音频文件数据发送结束时, 可以delete request.
                //request方法调用，比如create, start, sendAudio, stop, delete必须在
                //同一线程内完成，跨线程使用可能会引起异常错误.
                //下面是以读取config.txt方式创建request
		SpeechRecognizerRequest* request = gNlc->createRecognizerRequest(callback, tst->config.c_str());
		if (request == NULL) {
			cout << "createNlsRequest fail" << endl;
			return NULL;
		}

		/*
                //以参数设置方式创建request
		SpeechRecognizerRequest* request = gNlc->createRecognizerRequest(callback, NULL);
		if (request == NULL) {
			std::cout << "createNlsRequest fail" << endl;
			return NULL;
		}
		request->setUrl("wss://nls-gateway.cn-shanghai.aliyuncs.com/ws/v1");
		request->setAppKey("your-appkey");
		request->setFormat("pcm");
		request->setSampleRate("16000");
		request->setIntermediateResult("true");
		request->setPunctuationPrediction("true");
		request->setInverseTextNormalization("false");
		*/

        	request->setToken(tst->token.c_str());
		//start()为非异步操作，发送start指令之后，会等待服务端响应、或超时之后才返回
		if (request->start() < 0) {
			cout << "start fail" << endl;
			delete request;
			request = NULL;
			return NULL;
		}

		//获取待发送语音数据
		if (tst->isSendData) {
		    while (!fs.eof()) {
			vector<char> data(FRAME_SIZE, 0);
			fs.read(&data[0], sizeof(char) * FRAME_SIZE);
			int nlen = fs.gcount();
			nlen = request->sendAudio((char*)&data[0], nlen);
			if (nlen <= 0) {
			    std::cout << "send data fail" << endl;
			    break;
			}

//服务端处理的音频数据需等同于数据原始时长, 上传速度不能过快, 否则服务端可能会有异常.
//如果是实时语音数据, 此处不需要sleep操作.
//如果是文件数据或压缩数据, 此处需要sleep操作. 
//16K采样率, 16位, 单通道, 每ms数据量32byte.Demo中FRAME_SIZE为6400，所以需sleep 200ms.
#ifdef _WINDOWS_COMPILE_
			Sleep(200);
#else
			usleep(200 * 1000);
#endif
		    }
	        }
	        fs.close();

                //音频文件数据发送结束(一个会话周期结束), 此时调用stop().
                //stop()非异步，在接受到服务端响应之后，才会返回.
                //返回之后，可以调用delete request
		request->stop();

		delete request;
		request = NULL;
	} catch (const char* e) {
		cout << e << endl;
	}

	return NULL;
}

void OnStarted(NlsEvent* str, void* para = NULL) {
	ParamStruct* tst = (ParamStruct*)para;
	cout << "OnStarted: " << str->getResponse() << endl;
}

void OnResultChanged(NlsEvent* str, void* para = NULL) {
	ParamStruct* tst = (ParamStruct*)para;
	cout << "OnResultChanged: " << str->getResponse() << endl;
}

void OnCompleted(NlsEvent* str, void* para = NULL) {
	ParamStruct* tst = (ParamStruct*)para;
	cout << "OnCompleted: " << str->getResponse() << endl;
}

void OnOperationFailed(NlsEvent* str, void* para = NULL) {
	ParamStruct* tst = (ParamStruct*)para;
	cout << "OnOperationFailed: " << str->getErrorMessage() << endl;
}

void OnChannelCloseed(NlsEvent* str, void* para = NULL) {
	ParamStruct* tst = (ParamStruct*)para;
	cout << "OnChannelCloseed: " << str->getResponse() << endl;
}

int main(int arc, char* argv[]) {
	try {
		if (arc < 6) {
			cout << "param is not valid. Usage: demo test.wav config.txt 1 1 [your-token] 1" << endl;
			return -1;
		}

		gNlc = NlsClient::getInstance();
		int ret = gNlc->setLogConfig("log-realtime.txt", 3);

		ParamStruct pa;
		pa.filename = argv[1];
		pa.config = argv[2];
		pa.threadcount = atoi(argv[3]);
		pa.count = atoi(argv[4]);
		pa.token = argv[5];
		pa.isSendData = strcmp(argv[6], "0") == 0 ? false : true;

		const int ThreadCount = pa.threadcount;

		callback = new SpeechRecognizerCallback();
		callback->setOnRecognitionStarted(OnStarted);
		callback->setOnTaskFailed(OnOperationFailed);
		callback->setOnChannelClosed(OnChannelCloseed);
		callback->setOnRecognitionResultChanged(OnResultChanged);
		callback->setOnRecognitionCompleted(OnCompleted);

		vector<pthread_t> tarr(ThreadCount);
		for (int i = 0; i < pa.count; i++) {
			for (int j = 0; j < ThreadCount; j++) {
				pthread_create(&tarr[j], NULL, &func, (void *)&pa);
			}
			for (int j = 0; j < ThreadCount; j++) {
				pthread_join(tarr[j], NULL);
			}
		}

		if (gNlc != NULL) {
			delete gNlc;
			gNlc = NULL;
		}

		delete callback;
		callback = NULL;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	return 0;

}



