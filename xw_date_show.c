
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "xw_window_id_df.h"
#include "xw_window_xy_df.h"
#include "xw_date_show.h"
#include "gk_image_sdk_new.h"

//
#define FONT_SIZE                   16
#define FONT_WIN_COLOR              0xf00f
#define FONT_COLOR                  0x3A39

//
#define DATE_SET_UP_WINDOW_H        20
#define DATE_SET_UP_WINDOW_W        200
#define DATE_SET_DOWN_WINDOW_H      20
#define DATE_SET_DOWN_WINDOW_W      200
#define DATE_SET_WINDOW_W             80
#define DATE_SET_WINDOW_H             80





typedef enum{
   DATE_WINDOW_RUNING = 0,
   DATE_WINDOW_SET,
} DATE_WINDOW_RUN_STATE;

static void  mouse_ldown_botton_setdown(void *data);
static void  mouse_ldown_button_setup(void *data);
static void  mouse_ldown_button_settime(void *data);



static  DATE_WINDOW_RUN_STATE   _state = DATE_WINDOW_RUNING;    
static  struct  tm  set_time;

static  void  mouse_ldown_button_settime(void *data)
{
    if(_state == DATE_WINDOW_RUNING ){
        _state = DATE_WINDOW_SET;
        return;
    }
    time_t  tmp = 0 ;

    tmp = mktime(&set_time);
    struct timeval  setval;
    struct timezone zval;

    gettimeofday(&setval,&zval);
    setval.tv_sec = tmp;
    settimeofday(&setval,&zval);
    _state = DATE_WINDOW_RUNING;
    return ;
}


#define YEAR_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 2
#define YEAR_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 2

#define MON_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 98
#define MON_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 98

#define DAY_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 162
#define DAY_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 162

#define HOUR_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 194
#define HOUR_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 194

#define MIN_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 258
#define MIN_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 258

#define SEC_SET_UP_X      XW_DATE_SET_UP_WINDOW_X + 322
#define SEC_SET_DOWN_X    XW_DATE_SET_DOWN_WINDOW_X + 322

static  void  mouse_ldown_button_setup(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    
    GK_MOUSE_DATA   mdata = bt->this_node->mouse_data;
    
    if(mdata.x >= YEAR_SET_UP_X &&  mdata.x < MON_SET_UP_X){
        set_time.tm_year++;
    }else if( mdata.x >= MON_SET_UP_X &&  mdata.x < DAY_SET_UP_X){
        set_time.tm_mon = (set_time.tm_mon + 1) % 12;
    }else if(mdata.x >= DAY_SET_UP_X &&  mdata.x < HOUR_SET_UP_X){
        set_time.tm_mday = (set_time.tm_mday + 1) % 31;
    }else if(mdata.x >= HOUR_SET_UP_X &&  mdata.x < MIN_SET_UP_X){
        set_time.tm_hour = (set_time.tm_hour + 1) % 24; 
    }else if(mdata.x >= MIN_SET_UP_X &&  mdata.x < SEC_SET_UP_X){
        set_time.tm_min = (set_time.tm_min + 1) % 60;
    }else if(mdata.x >= SEC_SET_UP_X ){
        set_time.tm_sec = (set_time.tm_sec + 1) % 60;
    }else{
        ;
    }   

    return;
    
    
}



static  void  mouse_ldown_button_setdown(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    
    GK_MOUSE_DATA   mdata = bt->this_node->mouse_data;
    
    if(mdata.x >= YEAR_SET_DOWN_X &&  mdata.x < MON_SET_DOWN_X){
        set_time.tm_year--;
    }else if( mdata.x >= MON_SET_DOWN_X &&  mdata.x < DAY_SET_DOWN_X){
        if(set_time.tm_mon > 0)
            set_time.tm_mon--;
    }else if(mdata.x >= DAY_SET_DOWN_X &&  mdata.x < HOUR_SET_DOWN_X){
        if(set_time.tm_mday > 0)
            set_time.tm_mday--;
    }else if(mdata.x >= HOUR_SET_DOWN_X &&  mdata.x < MIN_SET_DOWN_X){
        if(set_time.tm_hour > 0)
            set_time.tm_hour--; 
    }else if(mdata.x >= MIN_SET_DOWN_X &&  mdata.x < SEC_SET_DOWN_X){
        if(set_time.tm_min > 0) 
            set_time.tm_min--;
    }else if(mdata.x >= SEC_SET_DOWN_X ){
        if(set_time.tm_sec > 0)
            set_time.tm_sec--;
    }else{
        ;
    }   

    return;
    
}



//need thread mode run
void    xw_date_show(void *data)
{
    
    
    struct user_set_node_atrr  node_attr;
    
    node_attr.en_node       = 1;
    node_attr.en_freshen    = 1;
    node_attr.move_arrt     = 0;

    //set English font size
    int  size_w = FONT_SIZE/2 ,size_h = FONT_SIZE, ret = 0;
    
    //year
    memcpy(node_attr.node_id,XW_DATE_YEAR_WINDOW_ID,strlen(XW_DATE_YEAR_WINDOW_ID));
    window_node_text_t  _text;
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_YEAR_WINDOW_X + size_w;
    _text.y             = XW_DATE_YEAR_WINDOW_Y + size_h;
    _text.lens          = 4;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);
    
    //moon
    memcpy(node_attr.node_id,XW_DATE_MONN_WINDOW_ID,strlen(XW_DATE_MONN_WINDOW_ID));
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_MONN_WINDOW_X + size_w;
    _text.y             = XW_DATE_MONN_WINDOW_Y + size_h;
    _text.lens          = 2;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);

    //day
    memcpy(node_attr.node_id,XW_DATE_DAY_WINDOW_ID,strlen(XW_DATE_DAY_WINDOW_ID));
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_DAY_WINDOW_X + size_w;
    _text.y             = XW_DATE_DAY_WINDOW_Y + size_h;
    _text.lens          = 2;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);
    
    //housr
    memcpy(node_attr.node_id,XW_DATE_HOUR_WINDOW_ID ,strlen(XW_DATE_HOUR_WINDOW_ID));
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_HOUR_WINDOW_X + size_w;
    _text.y             = XW_DATE_HOUR_WINDOW_Y + size_h;
    _text.lens          = 2;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);

    //minues
    memcpy(node_attr.node_id ,XW_DATE_MIN_WINDOW_ID, strlen(XW_DATE_MIN_WINDOW_ID));
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_MIN_WINDOW_X + size_w;
    _text.y             = XW_DATE_MIN_WINDOW_Y + size_h;
    _text.lens          = 2;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);
    //sec
    memcpy(node_attr.node_id,XW_DATE_SEC_WINDOW_ID,strlen(XW_DATE_SEC_WINDOW_ID));
    memset(&_text,0x0 ,sizeof(window_node_text_t));
    _text.x             = XW_DATE_SEC_WINDOW_X + size_w;
    _text.y             = XW_DATE_SEC_WINDOW_Y + size_h;
    _text.lens          = 2;
    _text.win_color     = FONT_WIN_COLOR;
    _text.text_color    = FONT_COLOR; 
    ret = Image_SDK_Create_Text(node_attr,_text);
    
    //date time set  button create
    memcpy(node_attr.node_id,XW_DATE_SET_UP_WINDOW_ID  ,strlen(XW_DATE_SET_UP_WINDOW_ID));
    node_attr.en_freshen = NORTHING;  
    window_node_button_t bt;
    memset(&bt,0x0,sizeof(window_node_button_t));
    bt.x                =  XW_DATE_SET_UP_WINDOW_X ;
    bt.y                =  XW_DATE_SET_UP_WINDOW_Y ;
    bt.w                =  DATE_SET_UP_WINDOW_W;
    bt.h                =  DATE_SET_UP_WINDOW_H;
    bt.color            =  0;
    bt.video_set.mouse_left_down = mouse_ldown_button_setup;
    ret = Image_SDK_Create_Button(node_attr,bt);
    
    
    memcpy(node_attr.node_id,XW_DATE_SET_DOWN_WINDOW_ID  ,strlen(XW_DATE_SET_DOWN_WINDOW_ID));
    node_attr.en_freshen = NORTHING;  
    memset(&bt,0x0,sizeof(window_node_button_t));
    bt.x                =  XW_DATE_SET_DOWN_WINDOW_X ;
    bt.y                =  XW_DATE_SET_DOWN_WINDOW_Y ;
    bt.w                =  DATE_SET_DOWN_WINDOW_W;
    bt.h                =  DATE_SET_DOWN_WINDOW_H;
    bt.color            =  0;
    bt.video_set.mouse_left_down = mouse_ldown_botton_setdown;
    ret = Image_SDK_Create_Button(node_attr,bt);
    
    memcpy(node_attr.node_id,XW_DATE_SET_WINDOW_ID  ,strlen(XW_DATE_SET_WINDOW_ID));
    node_attr.en_freshen = NORTHING;  
    memset(&bt,0x0,sizeof(window_node_button_t));
    bt.x                =  XW_DATE_SEC_WINDOW_X ;
    bt.y                =  XW_DATE_SET_WINDOW_Y ;
    bt.w                =  DATE_SET_WINDOW_W;
    bt.h                =  DATE_SET_WINDOW_H;
    bt.color            =  0;
    bt.video_set.mouse_left_down = mouse_ldown_button_settime;
    ret = Image_SDK_Create_Button(node_attr,bt);
    

    

    //get time and push
    time_t  tp = 0;
    struct  tm  *time_now,last_time;
    memset(&last_time,0x0,sizeof(struct tm));
    
    char    *year       = (char *)malloc(32);
    last_time.tm_mon    = 13;
    last_time.tm_hour   = 25;
    last_time.tm_min    = 61;

    while(1)
    {
        
        // at set timeing ,stop show   
        time(&tp);
        time_now = localtime(&tp);

        if(_state != DATE_WINDOW_SET)
            set_time = *time_now;

        if(last_time.tm_year != set_time.tm_year)
        {
            sprintf(year,"%d",set_time.tm_year+1900);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_YEAR_WINDOW_ID,year,4);
        }
        if(last_time.tm_mon != set_time.tm_mon)
        {
            sprintf(year,"%d",set_time.tm_mon+1);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_MONN_WINDOW_ID,year,2);
        }
        if(last_time.tm_mday != set_time.tm_mday)
        {   
            sprintf(year,"%d",set_time.tm_mday);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_DAY_WINDOW_ID, year,2);
        }
        if(last_time.tm_hour != set_time.tm_hour)
        {
            sprintf(year,"%d",set_time.tm_hour);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_HOUR_WINDOW_ID , year,2);
        }
        if(last_time.tm_min != set_time.tm_min)
        {
            sprintf(year,"%d",set_time.tm_min);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_MIN_WINDOW_ID, year,2);
        }
        if(last_time.tm_sec != set_time.tm_sec)
        {
            sprintf(year,"%d",set_time.tm_sec);
            ret = Image_SDK_Set_Text_Node_Text(XW_DATE_SEC_WINDOW_ID, year,2);
        }
        
        last_time =  set_time;
        usleep(500000);
    }
    
    return;

}



