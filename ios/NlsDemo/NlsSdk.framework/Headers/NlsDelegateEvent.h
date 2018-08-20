//
//  NlsDelegateEvent.h
//  NlsSdk
//
//  Created by Songsong Shao on 2018/6/26.
//  Copyright © 2018年 Songsong Shao. All rights reserved.
//

typedef enum{
    VoiceData,
    VoiceVolume,
    
    TaskFailed,
    RecognitionStarted,
    RecognitionCompleted ,
    RecognitionResultChanged,
    
    SynthesizerStarted,
    SynthesizerCompleted,
    Binary,
    
    TranscriberStarted,
    TranscriberCompleted,
    TranscriberSentenceBegin,
    TranscriberSentenceEnd,
    TranscriberResultChanged,
    
    Close
}NlsDelegateEvent;

