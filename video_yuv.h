#ifndef VIDEO_YUV_H
#define VIDEO_YUV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
uint8_t *display(uint8_t *yuv);
void *convert(void *arg);
void usage(int argc, char *argv[]);

void rgbtojpg(uint8_t *rgb);
#ifdef __cplusplus
}
#endif



#endif // VIDEO_YUV_H
