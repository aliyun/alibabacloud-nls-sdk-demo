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
 * Synthesizer.h
 * NlsDemo
 * Created by Songsong Shao on 2018/6/27.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <UIKit/UIKit.h>
#import "NLSPlayAudio.h"
#import "NlsSdk/NlsSpeechSynthesizerRequest.h"
#import "NlsSdk/SynthesizerRequestParam.h"
#import "NlsSdk/NlsClientAdaptor.h"

@interface Synthesizer:UIViewController

@property(nonatomic,strong) NlsClientAdaptor *nlsClient;
@property(nonatomic,strong) NlsSpeechSynthesizerRequest *synthesizerRequest;
@property(nonatomic,strong) SynthesizerRequestParam *requestParam;
@property(nonatomic,strong) NLSPlayAudio *nlsAudioPlayer;

- (IBAction)startSynthesizer;

@end
