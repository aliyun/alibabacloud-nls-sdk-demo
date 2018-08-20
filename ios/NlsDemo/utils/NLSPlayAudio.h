//
//  NLSPlayAudio.h
//  AliyunNlsClientSample
//
//  Created by 刘方 on 2/25/16.
//  Copyright © 2016 阿里巴巴. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

#define EVERY_READ_LENGTH 200
#define NUM_BUFFERS 3
#define QUEUE_BUFFER_SIZE 640



@interface NLSPlayAudio : NSObject {
    //音频参数
    AudioStreamBasicDescription audioDescription;
    // 音频播放队列
    AudioQueueRef audioQueue;
    // 音频缓存
    AudioQueueBufferRef audioQueueBuffers[QUEUE_BUFFER_SIZE];
}

-(void)process:(Byte *)audioByte length:(long)len;
-(void)play;
-(void)stop;
-(void)cleanup;
@end
