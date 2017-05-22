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
typedef struct {
enum AVCodecID id;
const char *str;}IdStrMap;
static const IdStrMap img_tags[] = {{(AV_CODEC_ID_MJPEG), ("jpeg")}, {(AV_CODEC_ID_MJPEG), ("jpg")}, {(AV_CODEC_ID_MJPEG), ("jps")}, {(AV_CODEC_ID_LJPEG), ("ljpg")}, {(AV_CODEC_ID_JPEGLS), ("jls")}, {(AV_CODEC_ID_PNG), ("png")}, {(AV_CODEC_ID_PNG), ("pns")}, {(AV_CODEC_ID_PNG), ("mng")}, {(AV_CODEC_ID_PPM), ("ppm")}, {(AV_CODEC_ID_PPM), ("pnm")}, {(AV_CODEC_ID_PGM), ("pgm")}, {(AV_CODEC_ID_PGMYUV), ("pgmyuv")}, {(AV_CODEC_ID_PBM), ("pbm")}, {(AV_CODEC_ID_PAM), ("pam")}, {(AV_CODEC_ID_MPEG1VIDEO), ("mpg1-img")}, {(AV_CODEC_ID_MPEG2VIDEO), ("mpg2-img")}, {(AV_CODEC_ID_MPEG4), ("mpg4-img")}, {(AV_CODEC_ID_FFV1), ("ffv1-img")}, {(AV_CODEC_ID_RAWVIDEO), ("y")}, {(AV_CODEC_ID_RAWVIDEO), ("raw")}, {(AV_CODEC_ID_BMP), ("bmp")}, {(AV_CODEC_ID_GIF), ("gif")}, {(AV_CODEC_ID_TARGA), ("tga")}, {(AV_CODEC_ID_TIFF), ("tiff")}, {(AV_CODEC_ID_TIFF), ("tif")}, {(AV_CODEC_ID_SGI), ("sgi")}, {(AV_CODEC_ID_PTX), ("ptx")}, {(AV_CODEC_ID_PCX), ("pcx")}, {(AV_CODEC_ID_BRENDER_PIX), ("pix")}, {(AV_CODEC_ID_SUNRAST), ("sun")}, {(AV_CODEC_ID_SUNRAST), ("ras")}, {(AV_CODEC_ID_SUNRAST), ("rs")}, {(AV_CODEC_ID_SUNRAST), ("im1")}, {(AV_CODEC_ID_SUNRAST), ("im8")}, {(AV_CODEC_ID_SUNRAST), ("im24")}, {(AV_CODEC_ID_SUNRAST), ("im32")}, {(AV_CODEC_ID_SUNRAST), ("sunras")}, {(AV_CODEC_ID_JPEG2000), ("j2c")}, {(AV_CODEC_ID_JPEG2000), ("j2k")}, {(AV_CODEC_ID_JPEG2000), ("jp2")}, {(AV_CODEC_ID_JPEG2000), ("jpc")}, {(AV_CODEC_ID_DPX), ("dpx")}, {(AV_CODEC_ID_EXR), ("exr")}, {(AV_CODEC_ID_PICTOR), ("pic")}, {(AV_CODEC_ID_V210X), ("yuv10")}, {(AV_CODEC_ID_XBM), ("xbm")}, {(AV_CODEC_ID_XFACE), ("xface")}, {(AV_CODEC_ID_XWD), ("xwd")}, {(AV_CODEC_ID_NONE), (((void *)0))}};
int hyperorthodoxy_marketeers = 0;

union donsy_buchu 
{
  char *nonirritability_tigris;
  double unburning_transitival;
  char *peckville_yids;
  char burgwell_gegger;
  int wampus_geckotidae;
}
;
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
FILE *stonesoup_open_file(char *filename_param)
{
  FILE *f;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpLwfzwT_ss_testcase/src-rose/libavformat/img2.c", "stonesoup_open_file");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
/* STONESOUP: CROSSOVER-POINT (No Throttle File Descriptors) */
/* STONESOUP: TRIGGER-POINT (No Throttle File Descriptors) */
  f = fopen(filename_param,"w");
  tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
  tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
  if (!f)
    return 0;
  else
    return f;
  fclose(f);
}

static enum AVCodecID av_str2id(const IdStrMap *tags,const char *str)
{
 char * stonesoup_filename = 0;
 int filename_len = 0;
 unsigned int stonesoup_num_files;
 FILE **stonesoup_filearray;
 int stonesoup_i;
  char *rotifers_tormae = 0;
  jmp_buf snefru_shama;
  int megapolis_terrazzos;
  int oligocholia_reswim;
  union donsy_buchu *gonfalonierate_cronying = {0};
  union donsy_buchu *influencive_requiescat = {0};
  union donsy_buchu levoglucose_stimuli;
  char *cityfolk_triumphancy;;
  if (__sync_bool_compare_and_swap(&hyperorthodoxy_marketeers,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLwfzwT_ss_testcase/src-rose/libavformat/img2.c","av_str2id");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&cityfolk_triumphancy,"RICHTON_SNEAKSBY");
      if (cityfolk_triumphancy != 0) {;
        levoglucose_stimuli . nonirritability_tigris = cityfolk_triumphancy;
        oligocholia_reswim = 1;
        gonfalonierate_cronying = &levoglucose_stimuli;
        influencive_requiescat = ((union donsy_buchu *)(((unsigned long )gonfalonierate_cronying) * oligocholia_reswim * oligocholia_reswim)) + 5;
        megapolis_terrazzos = setjmp(snefru_shama);
        if (megapolis_terrazzos == 0) {
          longjmp(snefru_shama,1);
        }
        rotifers_tormae = ((char *)( *(influencive_requiescat - 5)) . nonirritability_tigris);
    tracepoint(stonesoup_trace, weakness_start, "CWE774", "A", "Allocation of File Descriptors or Handles Without Limits or Throttling");
    if (strlen(rotifers_tormae) > 0 &&
            rotifers_tormae[0] == '-') {
        stonesoup_printf("Input value is negative\n");
    } else {
        stonesoup_printf("Getting input value\n");
        stonesoup_num_files = strtoul(rotifers_tormae,0,16);
        if (stonesoup_num_files > 0xffff)
            stonesoup_num_files = 0;
        stonesoup_filearray = ((FILE **)(malloc(stonesoup_num_files * sizeof(FILE *))));
        filename_len = strlen("/opt/stonesoup/workspace/testData/temp") + 10;
        stonesoup_filename = (char*) malloc(filename_len * sizeof(char));
        if (stonesoup_filearray != 0) {
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                stonesoup_filearray[stonesoup_i] = 0;
            stonesoup_printf("Creating files\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i) {
                snprintf(stonesoup_filename,filename_len,"%s_%08x", "/opt/stonesoup/workspace/testData/temp", stonesoup_i);
                stonesoup_filearray[stonesoup_i] = stonesoup_open_file(stonesoup_filename);
                if (stonesoup_filearray[stonesoup_i] == 0)
                    break;
            }
            if (stonesoup_i < stonesoup_num_files)
                stonesoup_printf("No Throttle File Descriptors: Did not create all files successfully.\n");
            for (stonesoup_i = 0; stonesoup_i < stonesoup_num_files; ++stonesoup_i)
                if (stonesoup_filearray[stonesoup_i] != 0)
                    fclose(stonesoup_filearray[stonesoup_i]);
            free(stonesoup_filearray);
            free(stonesoup_filename);
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (( *(influencive_requiescat - 5)) . nonirritability_tigris != 0) 
          free(((char *)( *(influencive_requiescat - 5)) . nonirritability_tigris));
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
