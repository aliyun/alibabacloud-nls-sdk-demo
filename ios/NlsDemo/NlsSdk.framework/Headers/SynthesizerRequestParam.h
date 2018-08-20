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
 * SynthesizerRequestParam.h
 * NlsSdk
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>

@interface SynthesizerRequestParam: NSObject

/**
 * @brief 设置语音识别的服务地址，请使用默认配置，可忽略
 * @note 用于区分语音识别的模型类型
 */
@property(nonatomic) NSString *serviceUrl;

/**
 * @brief 设置语音合成的appkey
 * @note 用于区分语音合成的模型类型
 */
@property(nonatomic) NSString *appkey;

/**
 * @brief 设置调用阿里云语音合成的accessToken
 * @note 鉴权使用 token不通过返回403错误。
 */
@property(nonatomic) NSString *token;

/**
 * @brief 设置语音格式，支持pcm、wav、mp3. pcm可以使用Demo提供的示例工具播放。
 * @note 默认pcm
 */
@property(nonatomic) NSString *format;

/**
 * @brief 需要合成的语音文本内容
 * @note 必须
 */
@property(nonatomic) NSString *text;

/**
 * @brief 选择合成语音发音人，xiaoyun/xiaogang
 * @note 默认是'xiaoyun'
 */
@property(nonatomic) NSString *voice;

/**
 * @brief 合成音量大小
 * @note 范围是0~100，可选，默认50
 */
@property(nonatomic) NSInteger volume;


/**
 * @brief 合成音频的语速
 * @note 可选，范围是-500~500，默认是0
 */
@property(nonatomic) NSInteger speechRate;

/**
 * @brief 合成音频的采样率
 * @note 默认16000
 */
@property(nonatomic) NSInteger sampleRate;

/**
 * @brief 语调，范围是-500~500，可选，默认是0
 * @note 可选 ，范围是-500~500，默认是0
 */
@property(nonatomic) NSInteger pitchRate;

/**
 * @brief 选择使用合成参数类型
 * @note 可选 ，默认是0
 *       0  统计参数合成 基于统计参数的语音合成，优点是能适应的韵律特征的范围较宽，合成器比特率低，资源较小，性能高，音质适中
 *       1  波形拼接合成 基于高质量音库提取学习合成，音质较好，更加贴近真实发音，但没有参数合成稳定
 */
@property(nonatomic) NSInteger method;

/**
 * @brief 用户自定义字段值
 * @note ，可选
 */
@property(nonatomic) NSDictionary *params;
@end

