
#ifndef __GK_MOUSE_EVENT_H__
#define __GK_MOUSE_EVENT_H__
#include <stdint.h>

typedef     enum{
    GK_MOUSE_OFFSET=0,
    GK_MOUSE_LEFT_DOWN,
    GK_MOUSE_LEFT_UP,
    GK_MOUSE_RIGHT_DOWN,
    GK_MOUSE_RIGHT_UP,
    GK_MOUSE_LEAVE,
    GK_MOUSE_NOTHOR,
    GK_MOUSE_DEL,
}GK_MOUSE_EVENT;


typedef  struct  gk_mouse_data{
    int  x;
    int  y;
    int  z;
    GK_MOUSE_EVENT  event;
}GK_MOUSE_DATA;

typedef struct gk_mouse_s{
    unsigned int    scree_w;
    unsigned int    scree_h;
    unsigned int    mouse_step_h;
    unsigned int    mouse_step_w;
    unsigned int    speed;
    unsigned int    mouse_size;
    int             fd;

}GK_MOUSE_T;

//return fd
int  gk_mouse_open(char *path,unsigned int scree_w,unsigned int scree_h,unsigned int speed,
        unsigned int mouse_size);
void gk_mouse_close(int fd);
int  gk_mouse_read_data(int fd,GK_MOUSE_DATA *event);



#endif

