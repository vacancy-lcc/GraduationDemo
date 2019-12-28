
#ifndef CAMINFO_H
#define CAMINFO_H



#ifdef __cplusplus
extern "C" {
#endif

// 获取摄像头格式信息（固定）
void get_caminfo(int camfd);

// 获取/设置摄像头格式信息（可调）
void get_camfmt(int camfd);
void set_camfmt(int camfd, char *pixfmt);

// 获取摄像头设备的基本参数
void get_camcap(int camfd);

#ifdef __cplusplus
}
#endif


#endif // RFID_H
