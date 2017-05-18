//
//  TPWBFaceVerify.h
//  TinyBuilder
//
//  Created by 顾卫明的mac on 2017/5/8.
//  Copyright © 2017年 Sunline. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <JavaScriptCore/JSExport.h>

@protocol TPWBFaceVerifyProtocol <JSExport>
@property (nonatomic, copy) NSString *Userid;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *idNo;
@property (nonatomic, copy) NSString *idType;
@property (nonatomic, copy) NSString *orderNo;
@property (nonatomic, copy) NSString *sign;
@property (nonatomic, copy) NSString *appid;
@property (nonatomic, copy) NSString *apiVersion;
@property (nonatomic, copy) NSString *faceverifyType;
@property (nonatomic, copy) NSString *licence;
@property (nonatomic, strong) JSValue *success;
@property (nonatomic, strong) JSValue *error;

- (void)start;

@end
