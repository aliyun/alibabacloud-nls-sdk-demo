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

package com.alibaba.nls.client.example;

import com.alibaba.nls.client.protocol.NlsClient;
import com.alibaba.nls.client.protocol.asr.ResponseModeEnum;
import com.alibaba.nls.client.protocol.asr.SampleRateEnum;
import com.alibaba.nls.client.protocol.asr.FormatEnum;
import com.alibaba.nls.client.protocol.asr.SpeechRecognizer;
import com.alibaba.nls.client.protocol.asr.SpeechRecognizerResponse;
import com.alibaba.nls.client.protocol.asr.SpeechRecognizerListener;

import java.io.InputStream;

/**
 * SpeechRecognizerDemo class
 *
 * 语音识别Demo
 * @author siwei
 * @date 2018/5/29
 */
public class SpeechRecognizerDemo {
    private String appkey;
    private String accessToken;
    NlsClient client;

    /**
     * @param appkey
     * @param token
     */
    public SpeechRecognizerDemo(String appkey, String token) {
        this.appkey = appkey;
        this.accessToken = token;
        client = new NlsClient(accessToken);
    }

    private static SpeechRecognizerListener getSpeechRecognizerListener() {
        SpeechRecognizerListener listener = new SpeechRecognizerListener() {
            @Override
            public void onSpeechRecResult(SpeechRecognizerResponse response) {
                System.out.println("name: " + response.getName() +
                        ", status: " + response.getStatus() +
                        ", text: " + response.getRecognizedText());
            }

            @Override
            public void onSpeechRecComplete(SpeechRecognizerResponse response) {
                System.out.println("name: " + response.getName() +
                ", status: " + response.getStatus() +
                ", text: " + response.getRecognizedText());
            }

        };
        return listener;
    }

    public void process(InputStream ins) {
        SpeechRecognizer recognizer = client.buildSpeechRecognizer(getSpeechRecognizerListener());
        recognizer.setAppKey(appkey);
        recognizer.setFormat(FormatEnum.PCM);
        recognizer.setSampleRate(SampleRateEnum.SAMPLE_RATE_16K);
        recognizer.setEnableIntermediateResult(true);

        try {
            recognizer.start();
            recognizer.send(ins, 8000, 250);
            recognizer.stop();
        } catch (Exception e) {
            System.err.println(e.getMessage());
        } finally {
            recognizer.close();
        }
    }

    public void shutdown() {
        client.shutdown();
    }

    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("SpeechRecognizerDemo need params: <app-key> <token>");
            System.exit(-1);
        }
        String appKey = args[0];
        String token = args[1];
        SpeechRecognizerDemo demo = new SpeechRecognizerDemo(appKey, token);
        InputStream ins = SpeechRecognizerDemo.class.getResourceAsStream("/sample.pcm");
        if (ins == null) {
            System.err.println("open the audio file failed!");
            System.exit(-1);
        }
        try {
            demo.process(ins);
            demo.shutdown();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
