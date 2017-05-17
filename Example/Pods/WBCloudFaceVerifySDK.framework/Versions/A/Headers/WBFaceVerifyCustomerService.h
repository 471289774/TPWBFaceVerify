//
//  WBFaceVerifyCustomerService.h
//  WBFaceV2
//
//  Created by tank on 25/10/2016.
//  Copyright © 2016 webank. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN
#define WBFaceV2SDKVersion @"2.1.1"

// SDK界面风格等配置字段key值
UIKIT_EXTERN NSString *const WBCloudFaceVerifySDKVersion;
UIKIT_EXTERN NSString *const WBFaceExternalIsShowSuccessPageKey;
UIKIT_EXTERN NSString *const WBFaceExternalIsShowFailurePageKey;
UIKIT_EXTERN NSString *const WBFaceExternalIsLightnessKey;
UIKIT_EXTERN NSString *const WBFaceExternalIsShowGuidePageKey;

//登录的错误码
typedef NS_ENUM(NSInteger, WBFaceVerifyLogin) {
    WBFaceVerifyLogin_ERROR = -10000, // 登录请求返回报错
    WBFaceVerifyLogin_PARAMS_ERROR = -20000,// 登录请求传入参数有误
    WBFaceVerifyLogin_NORESPONSE_ERROR = -30000 // 登录请求网络错误
};

typedef NS_ENUM(NSInteger, WBFaceVerifySDKErrorCode) {
    WBFaceVerifySDKErrorCode_SUCESS = 0,// 身份认证成功
    WBFaceVerifySDKErrorCode_FAILURE = 1,// 身份认证出错
    WBFaceVerifySDKErrorCode_CANCELLED = 2,// 用户取消认证
};

typedef NS_ENUM(NSInteger,WBFaceVerifyType){
    WBFaceVerifyTypeSimple,
    WBFaceVerifyTypeMiddle,
    WBFaceVerifyTypeAdvanced,
};

typedef void (^WBFaceLoginSuccessBlock)(void);
//登录过程中 loginCode  细分错误码 ---  -2 表示登录时网络错误, -1 表示登录时webank后台返回出错, 0 表示登录通用默认码, 其他数字透传webank后台错误码(请参考后台文档)
typedef void (^WBFaceLoginFailureBlock)(WBFaceVerifyLogin errorCode, NSString *loginCode, NSString *message);

@interface WBFaceUserInfo : NSObject
@property (nonatomic,copy) NSString *orderNo; // 订单号（合作方上送，每次唯一）
@property (nonatomic,copy) NSString *name;    // 姓名
@property (nonatomic,copy) NSString *idType;  // 证件类型
@property (nonatomic,copy) NSString *idNo;    // 证件号码


/**
 判断UserInfo信息是否满足要求,内部只判断属性是否有nil

 */
-(BOOL)isPropertyRight;
@end

/**
   处理刷脸回调
 */
@class WBFaceVerifyCustomerService;
@protocol WBFaceVerifyCustomerServiceDelegate  <NSObject>
@required
/**
 *  向WBFaceVerifyCustomerService传入一个ViewController,在sdk登录成功以后会在该viewController通过presentViewControllerxxxx方法拉起人脸认证页面.
 
    如果不知道该参数传递什么内容.那么请传递:
    [UIApplication sharedApplication].keyWindow.rootViewController
 *
 */
- (UIViewController *)wbfaceVerifyServiceGetViewController:(WBFaceVerifyCustomerService *)service;

/**
 *  刷脸身份认证的回调方法,带结果签名sign的回调
 *
 *  @param errorCode iOS SDK定义的概要错误码 -- 重要信息: 身份认证成功与失败都在这里显示
 *  @param faceCode  细分错误码 ---  -2 表示网络错误, -1 表示webank后台返回出错, 0 表示通用默认码, 其他数字透传webank后台错误码(请参考后台文档)
 *  @param faceMsg   身份认证错误的相关提示信息
 *  @param sign      当前身份认证结果的签名信息
 */
-(void)wbfaceVerifyCustomerServiceDidFinished:(WBFaceVerifySDKErrorCode)errorCode faceCode:(NSString *)faceCode faceMsg:(NSString *)faceMsg sign:(NSString *)sign;

@optional
/**
 *  SDK扩展字段
 *  WBFaceExternalIsShowSuccessPageKey BOOL 表示是否显示身份认证成功的结果页(默认情况显示 - YES)
 *  WBFaceExternalIsShowFailurePageKey BOOL 表示是否显示身份认证失败的结果页(默认情况显示 - YES)
 *  WBFaceExternalIsLightnessKey       BOOL 表示是否使用明亮主题的人脸验证界面风格(默认状态是暗黑主题页面风格)
 *  WBFaceExternalIsShowGuidePageKey   BOOL 表示是否显示身份认证前的引导页(默认情况显示 - YES)
 *
 *  在delegate实现中实现该方法
 -(NSDictionary *)wbfaceVerifyServiceGetExternalParams:(WBFaceVerifyCustomerService *)service{
     return @{
         WBFaceExternalIsShowFailurePageKey : @(YES),
         WBFaceExternalIsShowSuccessPageKey : @(YES),
         WBFaceExternalIsLightnessKey: @(YES),
         WBFaceExternalIsShowGuidePageKey, @(YES),
    };
 }
 
 *
 */
-(NSDictionary *)wbfaceVerifyServiceGetExternalParams:(WBFaceVerifyCustomerService *)service;

/**
 *  向WBFaceVerifyCustomerService传入一个ViewController,在sdk登录成功以后会在该viewController通过presentViewControllerxxxx方法拉起人脸认证页面.
 
    如果不知道该参数传递什么内容.那么请传递:
    [UIApplication sharedApplication].keyWindow.rootViewController
 *
 */
- (UIViewController *)getViewController NS_DEPRECATED_IOS(2_0, 2_0, "该回调方法是前向兼容方法,请使用 wbfaceVerifyServiceGetViewController: 方法");

/**
 *  刷脸身份认证的回调方法,不带结果签名sign的回调
 *
 *  @param errorCode iOS SDK定义的概要错误码 -- 重要信息: 身份认证成功与失败都在这里显示
 *  @param faceCode  细分错误码 ---  -2 表示网络错误, -1 表示webank后台返回出错, 0 表示通用默认码, 其他数字透传webank后台错误码(请参考后台文档)
 *  @param faceMsg   身份认证错误的相关提示信息
 */
-(void)wbfaceVerifyCustomerServiceDidFinished:(WBFaceVerifySDKErrorCode)errorCode faceCode:(NSString *)faceCode faceMsg:(NSString *)faceMsg NS_DEPRECATED_IOS(2_0, 2_0, "该回调方法是前向兼容方法方法,请使用 wbfaceVerifyCustomerServiceDidFinished:faceCode:faceMsg:sign: 方法");

@end

@interface WBFaceVerifyCustomerService : NSObject
@property (nullable,nonatomic,weak) id<WBFaceVerifyCustomerServiceDelegate> delegate;

/*
 * SDK全局单例,请使用此单例.不要使用init创建对象
 */
+ (instancetype)sharedInstance;

/**
 *  调用SDK服务核心方法
 *
 *  @param userid     userid 每个用户唯一的标识
 *  @param nonce      每次请求需要的一次性nonce
 *  @param sign       对nonce,userid等重要信息的签名数据
 *  @param appid      每个与webank合作分配的appid
 *  @param userInfo   用户信息对象,请参考前面定义的内容
 *  @param apiVersion 后台api接口版本号(不是SDK的版本号),默认请填写@"1.0.0"
 *  @param type       身份认证的类型- 简单模式,中级模式,高级模式
 *  @param licence    webank给合作方派发的licence
 *  @param success    调用sdk登录成功回调.请在该回调方法中关闭loading,并且在success block执行以后,sdk为拉起人脸认证页面
 *  @param failure    调用sdk登录失败时回调.请在该回调方法中关闭loading,处理错误逻辑.
 */
-(void)startWBFaceServiceWithUserid:(NSString *)userid
                              nonce:(NSString *)nonce
                               sign:(NSString *)sign
                              appid:(NSString *)appid
                           userInfo:(WBFaceUserInfo *)userInfo
                         apiVersion:(NSString *)apiVersion
                     faceverifyType:(WBFaceVerifyType)type
                            licence:(NSString *)licence
                            success:(WBFaceLoginSuccessBlock)success
                            failure:(WBFaceLoginFailureBlock)failure;
@end
NS_ASSUME_NONNULL_END
