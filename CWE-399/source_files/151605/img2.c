/*
 * Image format
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
 * Copyright (c) 2004 Michael Niedermayer
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "libavutil/avstring.h"
#include "internal.h"
#include <sys/stat.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct {
enum AVCodecID id;
const char *str;}IdStrMap;
static const IdStrMap img_tags[] = {{(AV_CODEC_ID_MJPEG), ("jpeg")}, {(AV_CODEC_ID_MJPEG), ("jpg")}, {(AV_CODEC_ID_MJPEG), ("jps")}, {(AV_CODEC_ID_LJPEG), ("ljpg")}, {(AV_CODEC_ID_JPEGLS), ("jls")}, {(AV_CODEC_ID_PNG), ("png")}, {(AV_CODEC_ID_PNG), ("pns")}, {(AV_CODEC_ID_PNG), ("mng")}, {(AV_CODEC_ID_PPM), ("ppm")}, {(AV_CODEC_ID_PPM), ("pnm")}, {(AV_CODEC_ID_PGM), ("pgm")}, {(AV_CODEC_ID_PGMYUV), ("pgmyuv")}, {(AV_CODEC_ID_PBM), ("pbm")}, {(AV_CODEC_ID_PAM), ("pam")}, {(AV_CODEC_ID_MPEG1VIDEO), ("mpg1-img")}, {(AV_CODEC_ID_MPEG2VIDEO), ("mpg2-img")}, {(AV_CODEC_ID_MPEG4), ("mpg4-img")}, {(AV_CODEC_ID_FFV1), ("ffv1-img")}, {(AV_CODEC_ID_RAWVIDEO), ("y")}, {(AV_CODEC_ID_RAWVIDEO), ("raw")}, {(AV_CODEC_ID_BMP), ("bmp")}, {(AV_CODEC_ID_GIF), ("gif")}, {(AV_CODEC_ID_TARGA), ("tga")}, {(AV_CODEC_ID_TIFF), ("tiff")}, {(AV_CODEC_ID_TIFF), ("tif")}, {(AV_CODEC_ID_SGI), ("sgi")}, {(AV_CODEC_ID_PTX), ("ptx")}, {(AV_CODEC_ID_PCX), ("pcx")}, {(AV_CODEC_ID_BRENDER_PIX), ("pix")}, {(AV_CODEC_ID_SUNRAST), ("sun")}, {(AV_CODEC_ID_SUNRAST), ("ras")}, {(AV_CODEC_ID_SUNRAST), ("rs")}, {(AV_CODEC_ID_SUNRAST), ("im1")}, {(AV_CODEC_ID_SUNRAST), ("im8")}, {(AV_CODEC_ID_SUNRAST), ("im24")}, {(AV_CODEC_ID_SUNRAST), ("im32")}, {(AV_CODEC_ID_SUNRAST), ("sunras")}, {(AV_CODEC_ID_JPEG2000), ("j2c")}, {(AV_CODEC_ID_JPEG2000), ("j2k")}, {(AV_CODEC_ID_JPEG2000), ("jp2")}, {(AV_CODEC_ID_JPEG2000), ("jpc")}, {(AV_CODEC_ID_DPX), ("dpx")}, {(AV_CODEC_ID_EXR), ("exr")}, {(AV_CODEC_ID_PICTOR), ("pic")}, {(AV_CODEC_ID_V210X), ("yuv10")}, {(AV_CODEC_ID_XBM), ("xbm")}, {(AV_CODEC_ID_XFACE), ("xface")}, {(AV_CODEC_ID_XWD), ("xwd")}, {(AV_CODEC_ID_NONE), (((void *)0))}};
int depressing_whisperable = 0;
void* stonesoup_printf_context = NULL;
void stonesoup_setup_printf_context() {
    struct stat st = {0};
    char * ss_tc_root = NULL;
    char * dirpath = NULL;
    int size_dirpath = 0;
    char * filepath = NULL;
    int size_filepath = 0;
    int retval = 0;
    ss_tc_root = getenv("SS_TC_ROOT");
    if (ss_tc_root != NULL) {
        size_dirpath = strlen(ss_tc_root) + strlen("testData") + 2;
        dirpath = (char*) malloc (size_dirpath * sizeof(char));
        if (dirpath != NULL) {
            sprintf(dirpath, "%s/%s", ss_tc_root, "testData");
            retval = 0;
            if (stat(dirpath, &st) == -1) {
                retval = mkdir(dirpath, 0700);
            }
            if (retval == 0) {
                size_filepath = strlen(dirpath) + strlen("logfile.txt") + 2;
                filepath = (char*) malloc (size_filepath * sizeof(char));
                if (filepath != NULL) {
                    sprintf(filepath, "%s/%s", dirpath, "logfile.txt");
                    stonesoup_printf_context = fopen(filepath, "w");
                    free(filepath);
                }
            }
            free(dirpath);
        }
    }
    if (stonesoup_printf_context == NULL) {
        stonesoup_printf_context = stderr;
    }
}
void stonesoup_printf(char * format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stonesoup_printf_context, format, argptr);
    va_end(argptr);
    fflush(stonesoup_printf_context);
}
void stonesoup_close_printf_context() {
    if (stonesoup_printf_context != NULL &&
        stonesoup_printf_context != stderr) {
        fclose(stonesoup_printf_context);
    }
}
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_env_var_name) {
  if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
      strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        char* stonesoup_tainted_file_name = 0;
        FILE * stonesoup_tainted_file = 0;
        size_t stonesoup_result = 0;
        long stonesoup_lsize = 0;
        stonesoup_tainted_file_name = getenv(stonesoup_env_var_name);
        stonesoup_tainted_file = fopen(stonesoup_tainted_file_name,"rb");
        if (stonesoup_tainted_file != 0) {
            fseek(stonesoup_tainted_file,0L,2);
            stonesoup_lsize = ftell(stonesoup_tainted_file);
            rewind(stonesoup_tainted_file);
            *stonesoup_tainted_buff = ((char *)(malloc(sizeof(char ) * (stonesoup_lsize + 1))));
            if (*stonesoup_tainted_buff != 0) {
                /* STONESOUP: SOURCE-TAINT (File Contents) */
                stonesoup_result = fread(*stonesoup_tainted_buff,1,stonesoup_lsize,stonesoup_tainted_file);
                (*stonesoup_tainted_buff)[stonesoup_lsize] = '\0';
            }
        }
        if (stonesoup_tainted_file != 0) {
            fclose(stonesoup_tainted_file);
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}

static enum AVCodecID av_str2id(const IdStrMap *tags,const char *str)
{
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *remarque_waxweed = 0;
  int posticus_semistate;
  int rheotropism_gablewindowed;
  char *ataxophemia_intertissue = 0;
  long belding_moundwork[10];
  char *linguister_virelais[10] = {0};
  int actionize_preferrous = 0;
  char *resecured_drinkability = 0;
  char *confabbed_toluate;;
  if (__sync_bool_compare_and_swap(&depressing_whisperable,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpKNcvS9_ss_testcase/src-rose/libavformat/img2.c","av_str2id");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&confabbed_toluate,"RANINAE_RHEUM");
      if (confabbed_toluate != 0) {;
        actionize_preferrous = ((int )(strlen(confabbed_toluate)));
        resecured_drinkability = ((char *)(malloc(actionize_preferrous + 1)));
        if (resecured_drinkability == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(resecured_drinkability,0,actionize_preferrous + 1);
        memcpy(resecured_drinkability,confabbed_toluate,actionize_preferrous);
        if (confabbed_toluate != 0) 
          free(((char *)confabbed_toluate));
        linguister_virelais[5] = resecured_drinkability;
        belding_moundwork[1] = 5;
        ataxophemia_intertissue =  *(linguister_virelais + belding_moundwork[1]);
        rheotropism_gablewindowed = 5;
        while(1 == 1){
          rheotropism_gablewindowed = rheotropism_gablewindowed * 2;
          rheotropism_gablewindowed = rheotropism_gablewindowed + 2;
          if (rheotropism_gablewindowed > 1000) {
            break; 
          }
        }
        posticus_semistate = rheotropism_gablewindowed;
        remarque_waxweed = ((char *)ataxophemia_intertissue);
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(remarque_waxweed);
    tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "INITIAL-STATE");
    if (stonesoup_num > 0 && stonesoup_num < 4294967295U / sizeof(unsigned int **)) {
        stonesoup_size = 100000;
        stonesoup_buffer_array = ((unsigned int **)(malloc(stonesoup_num * sizeof(unsigned int *))));
        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_size", stonesoup_size, &stonesoup_size, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array", stonesoup_buffer_array, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i) {
            stonesoup_buffer_array[stonesoup_i] = 0;
            stonesoup_printf("evaluating input\n");
            while(stonesoup_buffer_array[stonesoup_i] == 0){
    /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
    /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
    stonesoup_buffer_array[stonesoup_i] = ((unsigned int *)(malloc(stonesoup_size * sizeof(unsigned int ))));
                if (stonesoup_trace_counter % 1000 == 0) {
                    if (stonesoup_buffer_array[stonesoup_i] != 0) {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "CROSSOVER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "CROSSOVER-STATE");
                    } else {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "TRIGGER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "TRIGGER-STATE");
                    }
       }
       stonesoup_trace_counter ++;
   }
   if (stonesoup_buffer_array[stonesoup_i] != 0) {
    memset(stonesoup_buffer_array[stonesoup_i],0,stonesoup_size * sizeof(unsigned int ));
   }
   else
    break;
  }
 }
 tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
 tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
 tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
 stonesoup_printf("finished evaluating\n");
    for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i)
        if (stonesoup_buffer_array[stonesoup_i] != 0)
            free(stonesoup_buffer_array[stonesoup_i]);
    if (stonesoup_buffer_array != 0) {
        free(stonesoup_buffer_array);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (ataxophemia_intertissue != 0) 
          free(((char *)ataxophemia_intertissue));
stonesoup_close_printf_context();
      }
    }
  }
  ;
  str = (strrchr(str,'.'));
  if (!str) {
    return AV_CODEC_ID_NONE;
  }
  str++;
  while(tags -> id){
    if (!av_strcasecmp(str,tags -> str)) {
      return tags -> id;
    }
    tags++;
  }
  return AV_CODEC_ID_NONE;
}

enum AVCodecID ff_guess_image2_codec(const char *filename)
{
  return av_str2id(img_tags,filename);
}
