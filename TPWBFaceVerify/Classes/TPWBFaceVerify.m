//
//  TPWBFaceVerify.m
//  TinyBuilder
//
//  Created by 顾卫明的mac on 2017/5/8.
//  Copyright © 2017年 Sunline. All rights reserved.
//

#import "TPWBFaceVerify.h"
#import <WBCloudFaceVerifySDK/WBFaceVerifyCustomerService.h>
#define kRandomLength 32
#define iOS8Later ([UIDevice currentDevice].systemVersion.floatValue >= 8.0f)

@interface TPWBFaceVerify : NSObject <WBFaceVerifyCustomerServiceDelegate,TPWBFaceVerifyProtocol>
@property (nonatomic, strong) UIViewController *viewController;
@end

@implementation TPWBFaceVerify
@synthesize Userid;
@synthesize name;
@synthesize idNo;
@synthesize idType;
@synthesize orderNo;
@synthesize sign;
@synthesize appid;
@synthesize apiVersion;
@synthesize faceverifyType;
@synthesize licence;
@synthesize success;
@synthesize error;

static const NSString *kRandomAlphabet = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static NSString* createUuid(){
    NSMutableString *randomString = [NSMutableString stringWithCapacity:kRandomLength];
    for (int i = 0; i < kRandomLength; i++) {
        [randomString appendFormat: @"%C", [kRandomAlphabet characterAtIndex:arc4random_uniform((u_int32_t)[kRandomAlphabet length])]];
    }
    NSLog(@"randomString = %@", randomString);
    return randomString;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        Class TinyPlus=NSClassFromString(@"TinyPlus");
        if(TinyPlus){
            id tinyPlusInPod  =  [[TinyPlus alloc] init];
            if([tinyPlusInPod respondsToSelector:@selector(getViewController)]){
                self.viewController = [tinyPlusInPod performSelector:@selector(getViewController)];
            }
        }
        [WBFaceVerifyCustomerService sharedInstance].delegate = self;
    }
    return self;
}

- (void)start {
    NSInteger type = 0;
    if ([self.faceverifyType isEqualToString:@"0"]) {
        type = WBFaceVerifyTypeSimple;
    }
    if ([self.faceverifyType isEqualToString:@"1"]) {
        type = WBFaceVerifyTypeMiddle;
    }
    if ([self.faceverifyType isEqualToString:@"2"]) {
        type = WBFaceVerifyTypeAdvanced;
    }
    
    WBFaceUserInfo *userInfo = [[WBFaceUserInfo alloc] init];
    userInfo.name = self.name;
    userInfo.idNo = self.idNo;
    userInfo.idType = self.idType;
    userInfo.orderNo = self.orderNo;
    
    [[WBFaceVerifyCustomerService sharedInstance] startWBFaceServiceWithUserid:self.Userid nonce:createUuid() sign:self.sign appid:self.appid userInfo:userInfo apiVersion:self.apiVersion faceverifyType:type licence:self.licence success:^{
//        [self.success callWithArguments:@[@"shareTitle，shareContent，shareUrl，必须设置。"]];
    } failure:^(WBFaceVerifyLogin errorCode, NSString * _Nonnull faceCode, NSString * _Nonnull message) {
        [self.error callWithArguments:@[faceCode,message]];
    }];
}

#pragma mark - WBFaceVerifyCustomerServiceDelegate
-(void)wbfaceVerifyCustomerServiceDidFinished:(WBFaceVerifySDKErrorCode)errorCode faceCode:(NSString *)faceCode faceMsg:(NSString *)message sign:(NSString *)sign{
//    NSString *msg = [NSString stringWithFormat:@"第三方获取到退出信息: errorCode:%ld faceCode:%@ message:%@ ,sign:%@",(long)errorCode, faceCode, message, sign];
//    NSLog(@"%@",msg);
    if (iOS8Later) {
        if (errorCode == WBFaceVerifySDKErrorCode_SUCESS) {
          [self.success callWithArguments:@[faceCode,message,sign]];

        }else{
           [self.error callWithArguments:@[faceCode,message,sign]];
        }
    }
    
    // 在这里可以自行进行页面跳转
}

- (UIViewController *)wbfaceVerifyServiceGetViewController:(WBFaceVerifyCustomerService *)service {
    return self.viewController;
}
@end
