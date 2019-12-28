#include "common.h"
#include "jpeglib.h"
int roffset;
int goffset;
int boffset;

int lcd;
uint8_t *fb_mem;
struct fb_var_screeninfo lcdinfo;

int CAMERA_W, CAMERA_H;
int SCREEN_W, SCREEN_H, SCREEN_BPP;
int SCREEN_SIZE;

int R[256][256];
int G[256][256][256];
int B[256][256];

char *jpgname=NULL;
void rgbtojpg(uint8_t *rgb)
{
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        free(jpgname);

        // 准备图片文件
        jpgname = calloc(1, 20);
        snprintf(jpgname, 20, "pic/%d.jpg", time(NULL));

        FILE *fp = fopen(jpgname, "w");
        if(fp == NULL)
        {
            printf("创建文件[%s]失败:%s\n", jpgname, strerror(errno));
            return;
        }

        jpeg_stdio_dest(&cinfo, fp);

        cinfo.image_width  = CAMERA_W;
        cinfo.image_height = CAMERA_H;
        cinfo.input_components = 3;
        cinfo.in_color_space   = JCS_RGB;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality(&cinfo, 80, TRUE);

        jpeg_start_compress(&cinfo, TRUE);

        int row_stride = CAMERA_W*3;
        JSAMPROW row_pointer[1];

        // 开始编码，并将jpg数据存入指定的图片文件中
        while(cinfo.next_scanline < cinfo.image_height)
        {
            row_pointer[0] = rgb + cinfo.next_scanline * CAMERA_W * 3;
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
        //free(jpgname);
        fclose(fp);
}

void *convert(void *arg)
{
	/*******************************
	 R = Y + 1.042*(V-128);
	 G = Y - 0.344*(U-128)-0.714*(V-128);
	 B = Y + 1.772*(U-128);
	*******************************/
	pthread_detach(pthread_self());

	for(int i=0; i<256; i++)
	{
		for(int j=0; j<256; j++)
		{
			R[i][j] = i + 1.042*(j-128);
			R[i][j] = R[i][j]>255 ? 255 : R[i][j];
			R[i][j] = R[i][j]<0   ? 0   : R[i][j];

			B[i][j] = i + 1.772*(j-128);
			B[i][j] = B[i][j]>255 ? 255 : B[i][j];
			B[i][j] = B[i][j]<0   ? 0   : B[i][j];

			for(int k=0; k<256; k++)
			{
				G[i][j][k] = i + 0.344*(j-128)-0.714*(k-128);
				G[i][j][k] = G[i][j][k]>255 ? 255 : G[i][j][k];
				G[i][j][k] = G[i][j][k]<0   ? 0   : G[i][j][k];
			}
		}
	}

	// 保存YUV-RGB映射表
	FILE *fp = fopen("yuv-rgb", "w");
	fwrite(R, 256*4, 256,     fp);
	fwrite(G, 256*4, 256*256, fp);
	fwrite(B, 256*4, 256,     fp);
	fclose(fp);
}

uint8_t *display(uint8_t *yuv)
{
	static uint32_t shown = 0;


	uint8_t Y0, U;
	uint8_t Y1, V;


    uint8_t *rgbdata = calloc(1, CAMERA_W * CAMERA_H * 3);


    int yuv_offset, rgb_offset;
    for(int i=0; i<CAMERA_H; i++)
    {
        for(int j=0; j<CAMERA_W; j+=2)
        {
            yuv_offset = ( CAMERA_W*i + j ) * 2;
            rgb_offset = ( CAMERA_W*i + j ) * 3;

            Y0 = *(yuv + yuv_offset + 0);
            U  = *(yuv + yuv_offset + 1);
            Y1 = *(yuv + yuv_offset + 2);
            V  = *(yuv + yuv_offset + 3);

            *(rgbdata + rgb_offset + 0) = R[Y0][V];
            *(rgbdata + rgb_offset + 1) = G[Y0][U][V];
            *(rgbdata + rgb_offset + 2) = B[Y0][U];

            *(rgbdata + rgb_offset + 3) = R[Y1][V];
            *(rgbdata + rgb_offset + 4) = G[Y1][U][V];
            *(rgbdata + rgb_offset + 5) = B[Y1][U];
        }
    }
    //shown++;
    return rgbdata;
}

void usage(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("用法: %s </dev/videoX>\n", argv[0]);
		exit(0);
	}
}


