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
 * ViewController.m
 * NlsDemo
 * Created by xuebin on 2018/1/30.
 * Copyright © 2018年 Alibaba idst. All rights reserved.
 */

#import "ViewController.h"
#import "Recognizer.h"

@interface ViewController () {
    
    IBOutlet UIButton *asrButton;

}


- (IBAction)asrClick;

@end



@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)asrClick {
    NSLog(@"asrClick");
    Recognizer *recognizer = [[Recognizer alloc]init];
    [self.navigationController pushViewController: recognizer animated:true];
}

- (IBAction)ttsClick {
    
}

- (IBAction)RecognizerClick{
    

}


- (IBAction)unwindSegueToViewController:(UIStoryboardSegue *)segue {
    
}
@end

