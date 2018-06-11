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
 * Recgnizer.h
 * NlsDemo
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */


#import <UIKit/UIKit.h>
#import "NlsVoiceRecorder.h"
#import "NlsSdk/RecognizeRequest.h"
#import "NlsSdk/RecognizeRequestParam.h"
#import "NlsSdk/NlsClientAdaptor.h"

@interface Recognizer:UIViewController

@property(nonatomic,strong) NlsClientAdaptor *nlsClient;
@property(nonatomic,strong) RecognizeRequest *recognizeRequest;
@property(nonatomic,strong) NlsVoiceRecorder *voiceRecorder;
@property(nonatomic,strong) RecognizeRequestParam *recognizeRequestParam;

@property(atomic) Boolean record;

- (IBAction)startRecognize;
- (IBAction)stopRecognize;
@end

