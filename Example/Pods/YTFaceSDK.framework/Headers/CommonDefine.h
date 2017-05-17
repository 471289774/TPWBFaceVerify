//
//  CommonDefine.h
//  FaceAlign
//
//  Created by dreamqian on 16/3/1.
//  Copyright © 2016年 dreamqian. All rights reserved.
//

#ifndef CommonDefine_h
#define CommonDefine_h

//检测类型
typedef enum : NSUInteger {
    DETECTION_TYPE_NONE = 0,                ///<持续校正状态,人脸位置,光线,手机是否垂直
    DETECTION_TYPE_BLINK = 1,               ///<眨眼
    DETECTION_TYPE_MOUTH = 2,               ///<唇动
    DETECTION_TYPE_POS_YAW = 3,             ///<左右转头
    DETECTION_TYPE_POS_PITCH = 4,           ///<上下点头
    DETECTION_TYPE_DONE = 9,                ///<结束状态
} LivenessDetectionType;

typedef enum : NSUInteger {
    DETECTION_FAILED_TIMEOUT = 0,                ///<超时
} LivenessDetectionFailedType;

#endif /* CommonDefine_h */
