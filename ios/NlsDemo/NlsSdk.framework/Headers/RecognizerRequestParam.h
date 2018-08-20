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
 * NlsRequest.h
 * NlsSdk
 * Created by Songsong Shao on 2018/5/8.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import <Foundation/Foundation.h>

@interface RecognizerRequestParam: NSObject

@property(nonatomic) NSString *serviceUrl;

/**
 * @brief 设置语音识别的appkey
 * @note 用于区分语音识别的模型类型
 */
@property(nonatomic) NSString *appkey;

/**
 * @brief 设置调用阿里云语音识别的accessToken
 * @note 鉴权使用 token不通过返回403错误。
 */
@property(nonatomic) NSString *token;

/**
 * @brief 设置语音格式
 * @note 默认opu，一般无需更改。
 */
@property(nonatomic) NSString *format;

/**
 * @brief 是否开启中间识别结果的输出 
 * @note 默认False
 */
@property(nonatomic) Boolean enableIntermediateResult;

/**
 * @brief 是否在后处理中添加标点，可选
 * @note 默认是False
 */
@property(nonatomic) Boolean enablePunctuationPrediction;

/**
 * @brief 是否在后处理中执行ITN
 * @note ，默认是False
 */
@property(nonatomic) Boolean enableInverseTextNormalization;

/**
 * @brief 用户自定义字段值
 * @note ，可选
 */
@property(nonatomic) NSDictionary *params;

@end
