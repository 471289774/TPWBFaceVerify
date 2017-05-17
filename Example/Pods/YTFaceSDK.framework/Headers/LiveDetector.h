//
//  FaceDetector.h
//  liveness
//
//  Created by barretlzhao on 16/6/26.
//  Copyright © 2016年 tc. All rights reserved.
//

#ifndef FaceDetector_h
#define FaceDetector_h

#import <UIKit/UIKit.h>
#import <CoreMedia/CoreMedia.h>
#import "LiveDetectorDelegate.h"
#include "CommonDefine.h"

//检测结果图片
@interface FaceImage : NSObject

-(UIImage*)getBestImage;

/**
 *  最佳照片检测时的返回码:
 *  -2: 当前帧光照质量较差
 *  -3: 当前帧的人脸所占屏幕区域较小
 *  -4: 当前帧的人脸姿态角度过大
 *  -5: 当前帧的人脸嘴部姿态异常
 *   0: 当前帧其他异常情况
 *   1: 检测正常
 *  
 */
-(int)getImageCode;

@end

@interface LiveDetector : NSObject

- (id)init;

/**
 *  设置委托
 *
 *  在LiveDetector中，delegate为若引用
 */
- (void)setDelegate:(id<LiveDetectorDelegate>)delegate;

/**
 *  设置配置选项
 *
 *  @param options key-value形式的配置. e.g: @{@"enableLightDetection":@1, @"lightDetectionFrequence":@3}
 *  support value: @"enableLightDetection" (是否打开光照检测功能, @1:打开光照检测；@0:关闭光照检测)
 *  support value: @"lightDetectionFrequence" (打开光照检测功能时，该值若<=0，则每帧进行光照检测；如果大于0，则每隔lightDetectionFrequence帧进行一次光照检测。关闭光照检测功能时，忽略lightDetectionFrequence。)
 *
 *
 */
- (void)setOptions:(NSDictionary *)options;

/**
 *  将视频流获取的原始数据传入检测器进行异步活体检测，检测结果将以异步的形式通知delegate
 *
 *  @param imgBuffer AVCaptureOutput获取的原始数据流，RGB格式
 *
 *  @return 请求是否成功
 */
- (BOOL)detectWithBuffer:(CMSampleBufferRef)imgBuffer;

/*
 *  重置Detector类的状态，当用户需要重新开始活体检测流程时，调用此函数。默认为进入校正状态
 */
-(void)reset;

/**
 *  设置下一个检测类型
 *
 *  @param t : DETECTION_TYPE_POS_YAW,DETECTION_TYPE_BLINK...
 */
- (void)setNextType:(LivenessDetectionType)t;

/**
 *  设置启动检测的标志位,如启动则并开始计时
 *
 *  @param b true为启动检测；false为禁用检测;
 */
- (void)setDetectFlag:(BOOL)b;

/**
 *  设置活体检测准备阶段的标志位（准备阶段时，可在delegate中获取人脸框和姿态；非准备阶段时则不能获取）
 *
 *  @param b true为准备阶段；false为非准备阶段（非准备阶段即活体检测阶段）,此时需要调用setNextType设置当前活体检测的类别(摇头/眨眼/唇动);
 */
- (void)setPrepareStageFlag:(BOOL)b;

/**
 *  获得当前所属阶段的标志位（准备阶段返回true，活体检测阶段返回false）
 *
 *  @return 返回当前所属阶段
 */
- (BOOL)getPrepareStageFlag;

/**
 *  活体检测成功结果,包含最优图片和最优图片的签名
 *
 *  @return 检测成功结果
 */
- (FaceImage *)faceImage;

/**
 *  获取光照检测分数，范围0~1，越接近1，光照条件越好。
 *
 *  @return 返回光照检测分数。NSNumber存储为double类型。
 */
- (NSNumber*)getIlluminationScore;

/**
 *   判断是否检测到人脸
 *
 *  @return true:检测到人脸； false:没有检测到人脸
 */
- (BOOL)faceDetected;

/**
 *  在准备阶段时，用于获取当前人脸框；仅在setPrepareStageFlag中传入true时有效，传入false后，返回值无定义（前置摄像头，左上角为坐标原点）。
 *
 *  @return 返回人脸框
 */
- (CGRect)getFaceRect;

/**
 *  在准备阶段时，用于获取当前人脸姿态；仅在setPrepareStageFlag中传入true时有效，传入false后，返回值无定义。
 *
 *  @return 返回人脸姿态：NSMutableArray中存储了3个NSNumber(int)的角度值，分别为绕x/y/z轴的旋转角度。
 */
- (NSMutableArray*)getPoses;

/**
 *  用于获取当前人脸瞳孔位置
 *
 *  @return 返回人脸瞳孔位置：NSMutableArray中存储了4个NSNumber(float)的角度值，分别为左瞳孔x值、左瞳孔y值、右瞳孔x值、右瞳孔y值（前置摄像头，左上角为坐标原点）。
 */
- (NSMutableArray*)getPupils;


// set state
-(void)startLiveCheck;
-(void)endLiveCheck;
// return liveness result during livecheck
-(UIImage*)getResultLiveCheckImage;



@end

#endif /* FaceDetector_h */
