#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gk_device_init.h"
#include "gk_image_sdk_new.h"


static  void  mouse_leave_botton_func_v2(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t*)(data);
    bt->color = 0xFF80;
}

static  void  mouse_leave_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t*)(data);
    bt->color = 0xF00F;
}

static  void  mouse_offset_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t*)(data);
    bt->color = 0xBBCA;
}

static  void  mouse_rdown_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    bt->color = 0xFE41;
}

static  void  mouse_rup_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    bt->color = 0xFBE4;
}

static  void  mouse_lup_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    bt->color = 0x7497;
}

static  void  mouse_lup_botton_func_v2(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);
    bt->color = 0x7497;
}


static  void  mouse_ldown_botton_func(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);                 
    bt->color = 0xFD79;
}


static  void  mouse_ldown_botton_func_v2(void *data)
{
    
    window_node_button_t *bt  =  (window_node_button_t *)(data);                 
    bt->color = 0xFD79;
   }


static  void  mouse_lup_line_func_v2(void *data)
{
    
    window_node_line_t *bt  =  (window_node_line_t *)(data);
    bt->color = 0x7497;
   }


static  void  mouse_ldown_line_func_v2(void *data)
{
    
    //printf("data:%p\n",data);
    window_node_line_t *bt  =  (window_node_line_t *)(data);            
    bt->color = 0xFD79;
}


// chen hua memu


static  void  mouse_offset_func_bian_meun(void *data){

    window_node_menu_t *window = (window_node_menu_t *)data;

    if(window->x != 1500){
        window->this_node->freshen_arrt = NEED_FRESHEN;
        window->x = 1500;
        window->y = 100;
        window->h = 800;
        window->w = 400;
    }
    
    
    return;
}

  
static  void  mouse_leave_func_bian_meun(void *data){

    printf("menu set claer \n"); 
    window_node_menu_t *window = (window_node_menu_t *)data;
    window->x = 1880;
    window->y = 100;
    window->h = 800;
    window->w = 40;
    window->this_node->freshen_arrt = NEED_CLEAR;
    
    return;
}
  




int main(int argc,char **argv)
{
    /*
     *  root  = 'A'
     *  other = 'A.....'
     *
     * */
    
    int ret = 0;
    ret = gk_device_init(NULL); //ok
    start_read_venc_thread(); //ok 
    
    Image_SDK_Init();
    window_node_menu_t    bt;
    struct user_set_node_atrr  node_attr;
    
    memset(&bt,0x0,sizeof(window_node_menu_t));
    
    memset(&node_attr,0x0,sizeof(struct user_set_node_atrr));
    
    memcpy(node_attr.node_id,"Aa",2);
    
    node_attr.en_node = 1;
    node_attr.en_freshen = 0;
    node_attr.move_arrt = 0;


    bt.x = 1800;
    bt.y = 100;
    bt.w = 100;
    bt.h = 800;
    bt.color = 0xf00f;
    
    bt.video_set.mouse_offset =  mouse_offset_func_bian_meun;
    bt.video_set.mouse_leave =   mouse_leave_func_bian_meun;

    bt.video_set.mouse_left_down = NULL;//mouse_ldown_botton_func;
    bt.video_set.mouse_left_up = NULL;//mouse_lup_botton_func;
    
    ret = Image_SDK_Create_Menu( node_attr,bt);

    memcpy(node_attr.node_id,"Ab",2);

#if 0
    bt.video_set.mouse_leave = mouse_leave_botton_func_v2; 
    bt.x = 750;
    bt.y = 450;
    bt.w = 120;
    bt.h = 120;
    bt.color = 0xFF80;
    ret = Image_SDK_Create_Button(node_attr ,bt);




  

       
    memcpy(node_attr.node_id,"Ac",2);

    node_attr.move_arrt = 1;

    window_node_line_t    lt;

    memset(&lt,0x0,sizeof(window_node_line_t));

    lt.start_x = 20;
    lt.start_y = 800;
    lt.end_y = 800;
    lt.end_x = 1900;
    lt.size = 1;
    lt.color = 0xf00f;
    lt.video_set.mouse_left_down = mouse_ldown_line_func_v2;
    lt.video_set.mouse_left_up = mouse_lup_line_func_v2;
    

    ret = Image_SDK_Create_Line(node_attr,lt);
#endif




    Image_SDK_Run();

    //
    Image_SDK_deInit();



    return 0;
}
