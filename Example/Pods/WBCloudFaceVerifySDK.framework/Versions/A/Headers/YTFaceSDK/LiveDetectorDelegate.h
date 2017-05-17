//
//  FaceDetectorDelegate.h
//  liveness
//
//  Created by barretlzhao on 16/6/26.
//  Copyright © 2016年 tc. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "CommonDefine.h"

@class LiveDetector;

@protocol LiveDetectorDelegate <NSObject>

/*!
 *  当前活体检测的动作检测失败时，调用此方法。
 *
 *  @param failedType 动作检测失败的原因
 */
-(void)onDetectionFailed:(LiveDetector *)detector detectType:(LivenessDetectionType)detectType type:(LivenessDetectionFailedType)failedType;

/*!
 *  当前活体检测的动作检测成功时，调用此方法。
 *  @param detectType 检测类型(校正,眨眼等)
 */
-(void)onDetectionSuccess:(LiveDetector *)detector detectType:(LivenessDetectionType)detectType;

/**
 *   每检测一帧图像，都会调用一次此函数。
 *
 *  @param dector     检测器
 *  @param detectType 当前检测类型
 *  @param status     当前检测状态
 *  @param errmsg     有错误则返回错误信息(光线,人脸)
 */
-(void)onFrameDetected:(LiveDetector *)detector detectType:(LivenessDetectionType)detectType errmsg:(NSString *)errmsg;

@end
