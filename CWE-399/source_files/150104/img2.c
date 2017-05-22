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
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <fcntl.h> 
#include <unistd.h> 
typedef struct {
enum AVCodecID id;
const char *str;}IdStrMap;
static const IdStrMap img_tags[] = {{(AV_CODEC_ID_MJPEG), ("jpeg")}, {(AV_CODEC_ID_MJPEG), ("jpg")}, {(AV_CODEC_ID_MJPEG), ("jps")}, {(AV_CODEC_ID_LJPEG), ("ljpg")}, {(AV_CODEC_ID_JPEGLS), ("jls")}, {(AV_CODEC_ID_PNG), ("png")}, {(AV_CODEC_ID_PNG), ("pns")}, {(AV_CODEC_ID_PNG), ("mng")}, {(AV_CODEC_ID_PPM), ("ppm")}, {(AV_CODEC_ID_PPM), ("pnm")}, {(AV_CODEC_ID_PGM), ("pgm")}, {(AV_CODEC_ID_PGMYUV), ("pgmyuv")}, {(AV_CODEC_ID_PBM), ("pbm")}, {(AV_CODEC_ID_PAM), ("pam")}, {(AV_CODEC_ID_MPEG1VIDEO), ("mpg1-img")}, {(AV_CODEC_ID_MPEG2VIDEO), ("mpg2-img")}, {(AV_CODEC_ID_MPEG4), ("mpg4-img")}, {(AV_CODEC_ID_FFV1), ("ffv1-img")}, {(AV_CODEC_ID_RAWVIDEO), ("y")}, {(AV_CODEC_ID_RAWVIDEO), ("raw")}, {(AV_CODEC_ID_BMP), ("bmp")}, {(AV_CODEC_ID_GIF), ("gif")}, {(AV_CODEC_ID_TARGA), ("tga")}, {(AV_CODEC_ID_TIFF), ("tiff")}, {(AV_CODEC_ID_TIFF), ("tif")}, {(AV_CODEC_ID_SGI), ("sgi")}, {(AV_CODEC_ID_PTX), ("ptx")}, {(AV_CODEC_ID_PCX), ("pcx")}, {(AV_CODEC_ID_BRENDER_PIX), ("pix")}, {(AV_CODEC_ID_SUNRAST), ("sun")}, {(AV_CODEC_ID_SUNRAST), ("ras")}, {(AV_CODEC_ID_SUNRAST), ("rs")}, {(AV_CODEC_ID_SUNRAST), ("im1")}, {(AV_CODEC_ID_SUNRAST), ("im8")}, {(AV_CODEC_ID_SUNRAST), ("im24")}, {(AV_CODEC_ID_SUNRAST), ("im32")}, {(AV_CODEC_ID_SUNRAST), ("sunras")}, {(AV_CODEC_ID_JPEG2000), ("j2c")}, {(AV_CODEC_ID_JPEG2000), ("j2k")}, {(AV_CODEC_ID_JPEG2000), ("jp2")}, {(AV_CODEC_ID_JPEG2000), ("jpc")}, {(AV_CODEC_ID_DPX), ("dpx")}, {(AV_CODEC_ID_EXR), ("exr")}, {(AV_CODEC_ID_PICTOR), ("pic")}, {(AV_CODEC_ID_V210X), ("yuv10")}, {(AV_CODEC_ID_XBM), ("xbm")}, {(AV_CODEC_ID_XFACE), ("xface")}, {(AV_CODEC_ID_XWD), ("xwd")}, {(AV_CODEC_ID_NONE), (((void *)0))}};
int epiphonemas_hayatake = 0;
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

static enum AVCodecID av_str2id(const IdStrMap *tags,const char *str)
{
    int stonesoup_fd;
    char *stonesoup_extension = ".lck";
    char *stonesoup_thelockfile;
  char *anno_louisianians = 0;
  jmp_buf primp_sovkhoz;
  int carbolising_tanga;
  void **costain_adjument = 0;
  void *labyrinthitis_foresweat = 0;
  char *biliverdic_myriapodan;;
  if (__sync_bool_compare_and_swap(&epiphonemas_hayatake,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpYdc6_l_ss_testcase/src-rose/libavformat/img2.c","av_str2id");
      stonesoup_setup_printf_context();
      biliverdic_myriapodan = getenv("DIFFRACTION_HOMOANISIC");
      if (biliverdic_myriapodan != 0) {;
        labyrinthitis_foresweat = ((void *)biliverdic_myriapodan);
        costain_adjument = &labyrinthitis_foresweat;
        carbolising_tanga = setjmp(primp_sovkhoz);
        if (carbolising_tanga == 0) {
          longjmp(primp_sovkhoz,1);
        }
        anno_louisianians = ((char *)((char *)( *costain_adjument)));
    tracepoint(stonesoup_trace, weakness_start, "CWE412", "A", "Unrestricted Externally Accessible Lock");
    stonesoup_thelockfile = (char *)malloc(strlen("/opt/stonesoup/workspace/testData/") + strlen(anno_louisianians) +
                                  strlen(stonesoup_extension) + 1);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_thelockfile", stonesoup_thelockfile, "INITIAL-STATE");
    if (stonesoup_thelockfile) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Deadlock) */
  strcpy(stonesoup_thelockfile, "/opt/stonesoup/workspace/testData/");
     strcat(stonesoup_thelockfile, anno_louisianians);
     strcat(stonesoup_thelockfile, stonesoup_extension);
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
     /* Check if the path is accessible */
     if (access("/opt/stonesoup/workspace/testData/", R_OK && W_OK) == -1) {
      stonesoup_printf("Error accessing testData directory\n");
     } else {
            stonesoup_printf("Grabbing lock file: %s\n", stonesoup_thelockfile);
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            /* Create new "lock" file. If it already exists, it means the lock is taken and we
               need to wait for it to be released. */
      /* STONESOUP: TRIGGER-POINT (Deadlock) */
            while ((stonesoup_fd = open(stonesoup_thelockfile, O_CREAT | O_EXCL, 0644) == -1) ) {
                sleep(0);
            }
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            stonesoup_printf("Made it to the critical section\n");
            if (stonesoup_fd != -1) {
                close(stonesoup_fd);
                unlink(stonesoup_thelockfile);
            }
            free (stonesoup_thelockfile);
        }
    }
;
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
