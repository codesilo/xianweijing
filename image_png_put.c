#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <setjmp.h> 
#include <png.h>
//#include "pngconf.h"
/* Return component 'c' of pixel 'x' from the given row. */
static unsigned int
component(png_const_bytep row, png_uint_32 x, unsigned int c,
   unsigned int bit_depth, unsigned int channels)
{
   /* PNG images can be up to 2^31 pixels wide, but this means they can be up to
    * 2^37 bits wide (for a 64-bit pixel - the largest possible) and hence 2^34
    * bytes wide.  Since the row fitted into memory, however, the following must
    * work:
    */
   png_uint_32 bit_offset_hi = bit_depth * ((x >> 6) * channels);
   png_uint_32 bit_offset_lo = bit_depth * ((x & 0x3f) * channels + c);

   row = (png_const_bytep)(((PNG_CONST png_byte (*)[8])row) + bit_offset_hi);
   row += bit_offset_lo >> 3;
   bit_offset_lo &= 0x07;

   /* PNG pixels are packed into bytes to put the first pixel in the highest
    * bits of the byte and into two bytes for 16-bit values with the high 8 bits
    * first, so:
    */
   switch (bit_depth)
   {
      case 1: return (row[0] >> (7-bit_offset_lo)) & 0x01;
      case 2: return (row[0] >> (6-bit_offset_lo)) & 0x03;
      case 4: return (row[0] >> (4-bit_offset_lo)) & 0x0f;
      case 8: return row[0];
      case 16: return (row[0] << 8) + row[1];
      default:
         /* This should never happen; it indicates a bug in this program or in
          * libpng itself:
          */
         fprintf(stderr, "pngpixel: invalid bit depth %u\n", bit_depth);
         exit(1);
   }
}

/* Print a pixel from a row returned by libpng; determine the row format, find
 * the pixel, and print the relevant information to stdout.
 */
static void
print_pixel(png_structp png_ptr, png_infop info_ptr, png_const_bytep row,
   png_uint_32 x)
{
   PNG_CONST unsigned int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

   switch (png_get_color_type(png_ptr, info_ptr))
   {
      case PNG_COLOR_TYPE_GRAY:
         printf("GRAY %u\n", component(row, x, 0, bit_depth, 1));
         return;

      /* The palette case is slightly more difficult - the palette and, if
       * present, the tRNS ('transparency', though the values are really
       * opacity) data must be read to give the full picture:
       */
      case PNG_COLOR_TYPE_PALETTE:
         {
            PNG_CONST int index = component(row, x, 0, bit_depth, 1);
            png_colorp palette = NULL;
            int num_palette = 0;

            if ((png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette) &
               PNG_INFO_PLTE) && num_palette > 0 && palette != NULL)
            {
               png_bytep trans_alpha = NULL;
               int num_trans = 0;
               if ((png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans,
                  NULL) & PNG_INFO_tRNS) && num_trans > 0 &&
                  trans_alpha != NULL)
                  printf("INDEXED %u = %d %d %d %d\n", index,
                     palette[index].red, palette[index].green,
                     palette[index].blue,
                     index < num_trans ? trans_alpha[index] : 255);

               else /* no transparency */
                  printf("INDEXED %u = %d %d %d\n", index,
                     palette[index].red, palette[index].green,
                     palette[index].blue);
            }

            else
               printf("INDEXED %u = invalid index\n", index);
         }
         return;

      case PNG_COLOR_TYPE_RGB:
         printf("RGB %u %u %u\n", component(row, x, 0, bit_depth, 3),
            component(row, x, 1, bit_depth, 3),
            component(row, x, 2, bit_depth, 3));
         return;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
         printf("GRAY+ALPHA %u %u\n", component(row, x, 0, bit_depth, 2),
            component(row, x, 1, bit_depth, 2));
         return;

      case PNG_COLOR_TYPE_RGB_ALPHA:
         printf("RGBA %u %u %u %u\n", component(row, x, 0, bit_depth, 4),
            component(row, x, 1, bit_depth, 4),
            component(row, x, 2, bit_depth, 4),
            component(row, x, 3, bit_depth, 4));
         return;

      default:
         png_error(png_ptr, "pngpixel: invalid color type");
   }
}

struct  rgba8888{
    
    uint8_t     r;
    uint8_t     g;
    uint8_t     b;
    uint8_t     a;

};

struct  rgba4444{
    
    uint8_t     r:4;
    uint8_t     g:4;
    uint8_t     b:4;
    uint8_t     a:4;

};




void inline   rgba8888_to_rgba4444(uint32_t *in, uint16_t *put)
{
    
    struct rgba8888 *x =  (struct rgba8888 *)in;
    struct rgba4444 *y =  (struct rgba4444 *)put;
    
    y->r = x->r;
    y->g = x->g;
    y->b = x->b;
    y->a = x->a;
    
    return ;

}


int image_png_load_rgba_16bit(char *path,uint16_t *mem,uint32_t *h, uint32_t *w)
{

    if(path == NULL ||  mem == NULL)
        return -1;

    FILE *fp = NULL;
    fp = fopen(path,"rb");
    if(fp == NULL)
        return -2;
    
    volatile int result = 1/*fail*/;
    volatile png_bytep row = NULL;
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
    if(png_ptr == NULL)
        return -3;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL){
        return -5;
    }

    if(setjmp(png_jmpbuf(png_ptr)) !=0 ){

        return -6;

    }

    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method,compression_method, filter_method;
    png_bytep  row_tmp;
    //init io
    png_init_io(png_ptr, fp);
    
    //get png info
    png_read_info(png_ptr, info_ptr);

    //malloc row data mem
    row_tmp  = png_malloc(png_ptr, png_get_rowbytes(png_ptr,
                info_ptr));
    //get IHDR  info
    if (png_get_IHDR(png_ptr, info_ptr, &width, &height,
                &bit_depth, &color_type, &interlace_method,
                &compression_method, &filter_method))
    {
        
        *w = width;
        *h = height;


        int passes, pass;
        //is  need interleace disply ?
        switch (interlace_method)
        {
            case PNG_INTERLACE_NONE:
                passes = 1;
                break;

            case PNG_INTERLACE_ADAM7:
                passes = PNG_INTERLACE_ADAM7_PASSES;
                break;

            default:
                png_error(png_ptr, "pngpixel: unknown interlace");
        }

        /* Now read the pixels, pass-by-pass, row-by-row: */
        png_start_read_image(png_ptr);

        for (pass=0; pass < passes; ++pass)
        {

            png_uint_32 ystart, xstart, ystep, xstep;
            png_uint_32 py;

            if (interlace_method == PNG_INTERLACE_ADAM7)
            {
                
                if (PNG_PASS_COLS(width, pass) == 0)
                    continue;

                xstart = PNG_PASS_START_COL(pass);
                ystart = PNG_PASS_START_ROW(pass);
                xstep = PNG_PASS_COL_OFFSET(pass);
                ystep = PNG_PASS_ROW_OFFSET(pass);
            }else{
                ystart = xstart = 0;
                ystep = xstep = 1;
            }
            
            printf("ystep:%d xsetp:%d \n",ystep,xstep);
            for (py = ystart; py < height; py += ystep)
            {

                png_uint_32 px, ppx;
                //get line;
                png_read_row(png_ptr, row_tmp, NULL);
                
                for (px = xstart, ppx = 0; px < width; px += xstep, ++ppx)
                {
                    //rgba8888 to rbga4444
                    
                    rgba8888_to_rgba4444(((uint32_t *)row_tmp)+ppx, mem);
 
                    // print_pixel(png_ptr, info_ptr, row_tmp, ppx);
                } 
            } 
        } 

        row = NULL;
        png_free(png_ptr, row_tmp);
    }else{
        png_error(png_ptr, "pngpixel: png_get_IHDR failed");
    }

    return result;
}