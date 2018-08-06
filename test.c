#include "move_detect.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void dumpFile(unsigned char *pImage, int width, int height, char *filename)
{
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "255\n");
    unsigned char *p = pImage;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            fprintf(fp, "%d ", (int)(*p));
            p++;
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
int count = 0;
//fail on non zero
int kick(char *file)
{
    static unsigned char *jpgData=0;
    static size_t jpgSize=0;
    //set jpeg buffer to pjpeg_need_bytes_callback
    //set g_nInFileSize,jpgFile
    {
        FILE *jpgFile;
        jpgFile = fopen(file, "rb");
        if (!jpgFile)return -1;
        fseek(jpgFile, 0, SEEK_END);
        jpgSize = ftell(jpgFile);
        fseek(jpgFile, 0, SEEK_SET);
        if(jpgData)free(jpgData);
        jpgData = malloc(jpgSize);
        fread(jpgData, 1, jpgSize, jpgFile);
        fclose(jpgFile);
    }
    unsigned short max_s;
    if(100<(max_s=detect_move(jpgData,jpgSize))){
        char f[100];
        //printf("%s\n",file);
        sprintf(f, "out/%d__%d.pgm", count,max_s);
        dumpFile(g_img, F_WIDTH, F_HEIGHT, f);
    }
    printf("%d\n",max_s);
    fflush(stdout);
    return 0;
}

int main()
{
    char file[1024];
    char buf[1024];
    detect_move_init();
    FILE *list = fopen("data/list.txt", "r");
    while (fgets(buf, 1024, list))
    {
         char *p;
        if(0!= (p=strchr(buf,'\r')))*p=0;
        if(0!= (p=strchr(buf,'\n')))*p=0;
        sprintf(file, "data/%s", buf);
        kick(file);
        count++;
    }

    return 0;
}