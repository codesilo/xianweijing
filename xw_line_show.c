#include <stdio.h>
#include <stlib.h>
#include <string.h>

#include "xw_line_show.h"
#include "gk_image_sdk_new.h"
#include "xw_window_id_df.h"
#include "xw_window_xy_df.h"

#define     XW_LINE_NUMS_MAX            16   //H:8 w:8    


#define     XW_LINE_SIZE_MAX            3
#define     XW_LINE_SIZE_DEUFALT        1

#define     XW_LINE_RARR_WINDOW_H       1060
#define     XW_LINE_RARR_WINDOW_W       1800





typedef     struct xw_line_s{
    struct user_set_node_atrr   _attr;
    window_node_line_t          line;
}xw_line_data_t;

static  xw_line_data_t       *xw_lt = NULL;
static  FILE                 *xw_fp = NULL;
#define     XW_LINE_T_SAVEFILE_PATH     NULL

//file save and load
static  void xw_load_line_data(char *path,xw_line_data_t *lt);
static  void xw_save_line_data(char *path,xw_line_data_t *lt);


//mouse active 
static  void mouse_ldown_theline(void *data);
static  void mouse_offset_theline(void *data);


//init data
static  int  xw_line_t_init(void);

static window_node_line_t*  find_line_for_array(char *node_id)
{
    #define LINE_NODE_ID_LENS   3
    
    int i ;
    for(i = 0 ;i < XW_LINE_NUMS_MAX; i++){
        
        if(xw_lt[i]._attr.node_id[LINE_NODE_ID_LENS - 1 ] == node_id[LINE_NODE_ID_LENS - 1])
            return &xw_lt[i].line;
        }
    return NULL;
}

//mouse active 
static  void mouse_ldown_theline(void *data)
{
    window_node_line_t *lt  = (window_node_line_t *)data;
    window_node_line_t *sa = NULL;
    if(lt->this_node->move_arrt == MOUSE_LDOWN_MOVE)
    {   
                
            sa = find_line_for_array(lt->this_node->node_id);
            if(lt->start_x == lt->end_x){
                lt->start_y = lt->this_node->mouse_data.y;
                lt->end_y  = lt->start_y;
            }else{
                lt->start_x = lt->this_node->mouse_data.x;
                lt->end_x = lt->start_x;
            }
            *sa = *lt;
    }
    
    lt->this_node->freshen_arrt = NEED_FRESHEN;
    return;


}
static  void mouse_offset_theline(void *data)
{

    return;
}


static int  xw_line_t_init(void){

    xw_lt = (xw_line_data_t *)malloc(sizeof(xw_line_data_t)*XW_LINE_NUMS_MAX);
    if(xw_lt == NULL)
        return -1;
    memset(xw_lt,0x0,sizeof(xw_line_data_t)*XW_LINE_NUMS_MAX);

    if(XW_LINE_T_SAVEFILE_PATH != NULL)
    {
        ret = xw_load_line_data(XW_LINE_T_SAVEFILE_PATH,xw_lt);
    }
    if(ret == 0)
        return ret;

    //init node
    int i = 0 ;

    for(i = 0; i < XW_LINE_NUMS_MAX; i++){
        xw_lt[i]._attr.en_node              = 1;
        xw_lt[i]._attr.en_freshen           =  NEED_FRESHEN,
;
        xw_lt[i]._attr.mouse_garp_reflect   = OFFSET_REFLECT | LDOWN_REFLECT|LUP_REFLECT;
        xw_lt[i]._attr.move_arrt            =  MOUSE_LDOWN_MOVE;
    }
    //init h line node id
    memcpy(xw_lt[0]._attr.node_id,XW_LINE_H1_WINDOW_ID,strlen(XW_LINE_H1_WINDOW_ID ));
    memcpy(xw_lt[1]._attr.node_id,XW_LINE_H2_WINDOW_ID,strlen(XW_LINE_H2_WINDOW_ID ));
    memcpy(xw_lt[2]._attr.node_id,XW_LINE_H3_WINDOW_ID,strlen(XW_LINE_H3_WINDOW_ID ));
    memcpy(xw_lt[3]._attr.node_id,XW_LINE_H4_WINDOW_ID,strlen(XW_LINE_H4_WINDOW_ID ));
    memcpy(xw_lt[4]._attr.node_id,XW_LINE_H5_WINDOW_ID,strlen(XW_LINE_H5_WINDOW_ID ));
    memcpy(xw_lt[5]._attr.node_id,XW_LINE_H6_WINDOW_ID,strlen(XW_LINE_H6_WINDOW_ID ));
    memcpy(xw_lt[6]._attr.node_id,XW_LINE_H7_WINDOW_ID,strlen(XW_LINE_H7_WINDOW_ID ));
    memcpy(xw_lt[7]._attr.node_id,XW_LINE_H8_WINDOW_ID,strlen(XW_LINE_H8_WINDOW_ID ));
    //init w line node id
    memcpy(xw_lt[8]._attr.node_id,XW_LINE_W1_WINDOW_ID,strlen(XW_LINE_W1_WINDOW_ID ));
    memcpy(xw_lt[9]._attr.node_id,XW_LINE_W2_WINDOW_ID,strlen(XW_LINE_W2_WINDOW_ID ));
    memcpy(xw_lt[10]._attr.node_id,XW_LINE_W3_WINDOW_ID,strlen(XW_LINE_W3_WINDOW_ID ));
    memcpy(xw_lt[11]._attr.node_id,XW_LINE_W4_WINDOW_ID,strlen(XW_LINE_W4_WINDOW_ID ));
    memcpy(xw_lt[12]._attr.node_id,XW_LINE_W5_WINDOW_ID,strlen(XW_LINE_W5_WINDOW_ID ));
    memcpy(xw_lt[13]._attr.node_id,XW_LINE_W6_WINDOW_ID,strlen(XW_LINE_W6_WINDOW_ID ));
    memcpy(xw_lt[14]._attr.node_id,XW_LINE_W7_WINDOW_ID,strlen(XW_LINE_W7_WINDOW_ID ));
    memcpy(xw_lt[15]._attr.node_id,XW_LINE_W8_WINDOW_ID,strlen(XW_LINE_W8_WINDOW_ID ));

    //init h line param
    int k = 0;
    for(i = 0 ,k = 100; i < XW_LINE_NUMS_MAX/2;i++,k += 200){

        xw_lt[i].line.start_x   = k;
        xw_lt[i].line.start_y   = 5;
        xw_lt[i].line.end_x     = k;
        xw_lt[i].line.end_y     = 1070;
        xw_lt[i].line.color     = 0xf00f;
        xw_lt[i].line.size      =  XW_LINE_SIZE_DEUFALT ;
        xw_lt[i].line.video_set.mouse_left_down = mouse_ldown_theline;
    }

    //init w line param
    for(i = XW_LINE_NUMS_MAX/2 - 1,k = 50  ; i < XW_LINE_NUMS_MAX;i++,k += 100){

        xw_lt[i].line.start_x   = 5 ;
        xw_lt[i].line.start_y   = k ;
        xw_lt[i].line.end_x     = 1910 ;
        xw_lt[i].line.end_y     = k;
        xw_lt[i].line.color     = 0xf0f0;
        xw_lt[i].line.size      =  XW_LINE_SIZE_DEUFALT;
        xw_lt[i].line.video_set.mouse_left_down = mouse_ldown_theline;

    }


    return 0;


}

void xw_line_rarr_create(void *data){

    //init xw line data

    int ret = 0;
    ret = xw_line_t_init();
    if(ret < 0)
        return;


    struct user_set_node_atrr _attr;
    memset(&_attr,0x0,sizeof(struct user_set_node_atrr));
    memcpy(_attr.node_id,XW_LINE_RARR_WINDOW_ID,strlen(XW_LINE_RARR_WINDOW_ID ));
    _attr.en_node = 0;
    
    //ceater line father menu
    window_node_menu_t mt;
    memset(&mt,0x0,sizeof(window_node_menu_t));
    mt.x =   XW_LINE_RARR_WINDOW_X;
    mt.y =   XW_LINE_RARR_WINDOW_Y;
    mt.h =   XW_LINE_RARR_WINDOW_H;
    mt.w =   XW_LINE_RARR_WINDOW_W;
    ret  = Image_SDK_Create_Menu(_attr,mt);


    //ceate all line 
    int  i = 0;
    for(i = 0 ; i < XW_LINE_NUMS_MAX ; i++ ){
        ret = Image_SDK_Create_Line(xw_lt[i]._attr,xw_lt[i].line);
    }

    return;

}


