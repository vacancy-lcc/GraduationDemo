/////////////////////////////////////////////////////
//
//  Copyright(C), 2011-2018, GEC Tech. Co., Ltd.
//  File name: common.h
//
//  Description: 摄像头图像处理头文件
//  Author: 林世霖
//  微信公众号：秘籍酷
//
//  GitHub: github.com/vincent040   
//  Bug Report: 2437231462@qq.com
//
//////////////////////////////////////////////////////

#ifndef __COMMON_H
#define __COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <linux/fb.h>
#include <linux/videodev2.h>
#include <linux/input.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <setjmp.h>
#include <pthread.h>
#include <semaphore.h>


//#define MIN(a, b) \
//	({ \
//	 	typeof(a) _a = a; \
//	 	typeof(b) _b = b; \
//		(void)(&_a==&_b); \
//		_a < _b ? _a : _b;\
//	 })

#ifdef __cplusplus
}
#endif

#endif

