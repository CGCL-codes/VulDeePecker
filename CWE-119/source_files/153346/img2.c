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
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
typedef struct {
enum AVCodecID id;
const char *str;}IdStrMap;
static const IdStrMap img_tags[] = {{(AV_CODEC_ID_MJPEG), ("jpeg")}, {(AV_CODEC_ID_MJPEG), ("jpg")}, {(AV_CODEC_ID_MJPEG), ("jps")}, {(AV_CODEC_ID_LJPEG), ("ljpg")}, {(AV_CODEC_ID_JPEGLS), ("jls")}, {(AV_CODEC_ID_PNG), ("png")}, {(AV_CODEC_ID_PNG), ("pns")}, {(AV_CODEC_ID_PNG), ("mng")}, {(AV_CODEC_ID_PPM), ("ppm")}, {(AV_CODEC_ID_PPM), ("pnm")}, {(AV_CODEC_ID_PGM), ("pgm")}, {(AV_CODEC_ID_PGMYUV), ("pgmyuv")}, {(AV_CODEC_ID_PBM), ("pbm")}, {(AV_CODEC_ID_PAM), ("pam")}, {(AV_CODEC_ID_MPEG1VIDEO), ("mpg1-img")}, {(AV_CODEC_ID_MPEG2VIDEO), ("mpg2-img")}, {(AV_CODEC_ID_MPEG4), ("mpg4-img")}, {(AV_CODEC_ID_FFV1), ("ffv1-img")}, {(AV_CODEC_ID_RAWVIDEO), ("y")}, {(AV_CODEC_ID_RAWVIDEO), ("raw")}, {(AV_CODEC_ID_BMP), ("bmp")}, {(AV_CODEC_ID_GIF), ("gif")}, {(AV_CODEC_ID_TARGA), ("tga")}, {(AV_CODEC_ID_TIFF), ("tiff")}, {(AV_CODEC_ID_TIFF), ("tif")}, {(AV_CODEC_ID_SGI), ("sgi")}, {(AV_CODEC_ID_PTX), ("ptx")}, {(AV_CODEC_ID_PCX), ("pcx")}, {(AV_CODEC_ID_BRENDER_PIX), ("pix")}, {(AV_CODEC_ID_SUNRAST), ("sun")}, {(AV_CODEC_ID_SUNRAST), ("ras")}, {(AV_CODEC_ID_SUNRAST), ("rs")}, {(AV_CODEC_ID_SUNRAST), ("im1")}, {(AV_CODEC_ID_SUNRAST), ("im8")}, {(AV_CODEC_ID_SUNRAST), ("im24")}, {(AV_CODEC_ID_SUNRAST), ("im32")}, {(AV_CODEC_ID_SUNRAST), ("sunras")}, {(AV_CODEC_ID_JPEG2000), ("j2c")}, {(AV_CODEC_ID_JPEG2000), ("j2k")}, {(AV_CODEC_ID_JPEG2000), ("jp2")}, {(AV_CODEC_ID_JPEG2000), ("jpc")}, {(AV_CODEC_ID_DPX), ("dpx")}, {(AV_CODEC_ID_EXR), ("exr")}, {(AV_CODEC_ID_PICTOR), ("pic")}, {(AV_CODEC_ID_V210X), ("yuv10")}, {(AV_CODEC_ID_XBM), ("xbm")}, {(AV_CODEC_ID_XFACE), ("xface")}, {(AV_CODEC_ID_XWD), ("xwd")}, {(AV_CODEC_ID_NONE), (((void *)0))}};
int dib_aquapuncture = 0;
int stonesoup_global_variable;
typedef char *pupas_superaffiuence;
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
void moet_maudlinize(int ass_addams,... );

static enum AVCodecID av_str2id(const IdStrMap *tags,const char *str)
{
  pupas_superaffiuence gonium_outdress = 0;
  char *doodling_kolhoz;;
  if (__sync_bool_compare_and_swap(&dib_aquapuncture,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpap6yui_ss_testcase/src-rose/libavformat/img2.c","av_str2id");
      stonesoup_setup_printf_context();
      doodling_kolhoz = getenv("DIETETICAL_UNCOGNIZED");
      if (doodling_kolhoz != 0) {;
        gonium_outdress = doodling_kolhoz;
        moet_maudlinize(1,gonium_outdress);
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

void moet_maudlinize(int ass_addams,... )
{
    int stonesoup_ss_i = 0;
    char* stonesoup_heap_buff_64;
    int stonesoup_buff_size;
  char *enhancive_captor = 0;
  int sawbones_bogginess;
  int trouping_chukars;
  pupas_superaffiuence cardioschisis_overproviding = 0;
  va_list millstream_philogynaecic;
  ++stonesoup_global_variable;;
  if (ass_addams > 0) {
    __builtin_va_start(millstream_philogynaecic,ass_addams);
    cardioschisis_overproviding = (va_arg(millstream_philogynaecic,pupas_superaffiuence ));
    __builtin_va_end(millstream_philogynaecic);
  }
  trouping_chukars = 5;
  while(1 == 1){
    trouping_chukars = trouping_chukars * 2;
    trouping_chukars = trouping_chukars + 2;
    if (trouping_chukars > 1000) {
      break; 
    }
  }
  sawbones_bogginess = trouping_chukars;
  enhancive_captor = ((char *)cardioschisis_overproviding);
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "B", "Buffer Over-read");
    stonesoup_heap_buff_64 = (char*) malloc(64 * sizeof(char));
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "INITIAL-STATE");
    if (stonesoup_heap_buff_64 != NULL) {
        memset(stonesoup_heap_buff_64,'A',63);
        stonesoup_heap_buff_64[63] = '\0';
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "INITIAL-STATE");
        stonesoup_buff_size = ((int )(strlen(enhancive_captor)));
        strncpy(stonesoup_heap_buff_64, enhancive_captor, 64);
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "TAINTED");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (; stonesoup_ss_i < stonesoup_buff_size; ++stonesoup_ss_i){
            /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
            stonesoup_printf("%02x",stonesoup_heap_buff_64[stonesoup_ss_i]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free( stonesoup_heap_buff_64);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
stonesoup_close_printf_context();
}
