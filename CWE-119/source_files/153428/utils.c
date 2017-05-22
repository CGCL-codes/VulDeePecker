/*
 * utils for libavcodec
 * Copyright (c) 2001 Fabrice Bellard
 * Copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
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
/**
 * @file
 * utils.
 */
#include "config.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/bprint.h"
#include "libavutil/channel_layout.h"
#include "libavutil/crc.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/dict.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "dsputil.h"
#include "libavutil/opt.h"
#include "thread.h"
#include "frame_thread_encoder.h"
#include "internal.h"
#include "bytestream.h"
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#if CONFIG_ICONV
# include <iconv.h>
#endif
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/types.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 
pthread_spinlock_t ff_avcodec_locked;
static pthread_spinlock_t entangled_thread_counter = 0;
static int (*ff_lockmgr_cb)(void **, enum AVLockOp );
static void *codec_mutex;
static void *avformat_mutex;
int unvascularly_spevek = 0;

union truncated_nonreverential 
{
  char *hasky_subordinator;
  double citramide_grosmark;
  char *warniss_nonmediation;
  char kashgar_titheright;
  int triol_cardiectasis;
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
void stonesoup_read_taint(char** stonesoup_tainted_buff, char* stonesoup_envKey, int stonesoup_shmsz) {
    int stonesoup_shmid;
 key_t stonesoup_key;
 char *stonesoup_shm, *stonesoup_s;
 char* stonesoup_envSize = NULL;
 *stonesoup_tainted_buff = NULL;
    if (getenv("STONESOUP_DISABLE_WEAKNESS") == NULL ||
        strcmp(getenv("STONESOUP_DISABLE_WEAKNESS"), "1") != 0) {
        if(stonesoup_envKey != NULL) {
            if(sscanf(stonesoup_envKey, "%d", &stonesoup_key) > 0) {
                if ((stonesoup_shmid = shmget(stonesoup_key, stonesoup_shmsz, 0666)) >= 0) {
                    if ((stonesoup_shm = shmat(stonesoup_shmid, NULL, 0)) != (char *) -1) {
                        *stonesoup_tainted_buff = (char*)calloc(stonesoup_shmsz, sizeof(char));
                        /* STONESOUP: SOURCE-TAINT (Shared Memory) */
                        for (stonesoup_s = stonesoup_shm; *stonesoup_s != (char)0; stonesoup_s++) {
                            (*stonesoup_tainted_buff)[stonesoup_s - stonesoup_shm] = *stonesoup_s;
                        }
                    }
                }
            }
        }
    } else {
        *stonesoup_tainted_buff = NULL;
    }
}

void *av_fast_realloc(void *ptr,unsigned int *size,size_t min_size)
{
  if (min_size < ( *size)) {
    return ptr;
  }
  min_size = (17 * min_size / 16 + 32 > min_size?17 * min_size / 16 + 32 : min_size);
  ptr = av_realloc(ptr,min_size);
/* we could set this to the unmodified min_size but this is safer
     * if the user lost the ptr and uses NULL now
     */
  if (!ptr) {
    min_size = 0;
  }
   *size = min_size;
  return ptr;
}

inline static int ff_fast_malloc(void *ptr,unsigned int *size,size_t min_size,int zero_realloc)
{
  void **p = ptr;
  if (min_size < ( *size)) {
    return 0;
  }
  min_size = (17 * min_size / 16 + 32 > min_size?17 * min_size / 16 + 32 : min_size);
  av_free( *p);
   *p = (zero_realloc?av_mallocz(min_size) : av_malloc(min_size));
  if (!( *p)) {
    min_size = 0;
  }
   *size = min_size;
  return 1;
}

void av_fast_malloc(void *ptr,unsigned int *size,size_t min_size)
{
  ff_fast_malloc(ptr,size,min_size,0);
}

void av_fast_padded_malloc(void *ptr,unsigned int *size,size_t min_size)
{
  uint8_t **p = ptr;
  if (min_size > 18446744073709551615UL - 16) {
    av_freep(p);
     *size = 0;
    return ;
  }
  if (!ff_fast_malloc(p,size,min_size + 16,1)) {
    memset(( *p + min_size),0,16);
  }
}

void av_fast_padded_mallocz(void *ptr,unsigned int *size,size_t min_size)
{
  uint8_t **p = ptr;
  if (min_size > 18446744073709551615UL - 16) {
    av_freep(p);
     *size = 0;
    return ;
  }
  if (!ff_fast_malloc(p,size,min_size + 16,1)) {
    memset(( *p),0,min_size + 16);
  }
}
/* encoder management */
static AVCodec *first_avcodec = ((void *)0);

AVCodec *av_codec_next(const AVCodec *c)
{
  if (c) {
    return c -> next;
  }
  else {
    return first_avcodec;
  }
}

static void avcodec_init()
{
  static int initialized = 0;
  if (initialized != 0) {
    return ;
  }
  initialized = 1;
  ff_dsputil_static_init();
}

int av_codec_is_encoder(const AVCodec *codec)
{
  return codec && (codec -> encode_sub || codec -> encode2);
}

int av_codec_is_decoder(const AVCodec *codec)
{
  return codec && codec -> decode;
}

void avcodec_register(AVCodec *codec)
{
  AVCodec **p;
  avcodec_init();
  p = &first_avcodec;
  while( *p != ((void *)0))
    p = &( *p) -> next;
   *p = codec;
  codec -> next = ((void *)0);
  if (codec -> init_static_data) {
    (codec -> init_static_data)(codec);
  }
}

unsigned int avcodec_get_edge_width()
{
  return 16;
}

void avcodec_set_dimensions(AVCodecContext *s,int width,int height)
{
  s -> coded_width = width;
  s -> coded_height = height;
  s -> width = -(-width >> s -> lowres);
  s -> height = -(-height >> s -> lowres);
}
#define INTERNAL_BUFFER_SIZE (32 + 1)
#if (ARCH_ARM && HAVE_NEON) || ARCH_PPC || HAVE_MMX
#   define STRIDE_ALIGN 16
#else
#   define STRIDE_ALIGN 8
#endif

void avcodec_align_dimensions2(AVCodecContext *s,int *width,int *height,int linesize_align[8])
{
  int i;
  int w_align = 1;
  int h_align = 1;
  switch(s -> pix_fmt){
    case AV_PIX_FMT_YUV420P:
{
    }
    case AV_PIX_FMT_YUYV422:
{
    }
    case AV_PIX_FMT_UYVY422:
{
    }
    case AV_PIX_FMT_YUV422P:
{
    }
    case AV_PIX_FMT_YUV440P:
{
    }
    case AV_PIX_FMT_YUV444P:
{
    }
    case AV_PIX_FMT_GBRP:
{
    }
    case AV_PIX_FMT_GRAY8:
{
    }
    case AV_PIX_FMT_GRAY16BE:
{
    }
    case AV_PIX_FMT_GRAY16LE:
{
    }
    case AV_PIX_FMT_YUVJ420P:
{
    }
    case AV_PIX_FMT_YUVJ422P:
{
    }
    case AV_PIX_FMT_YUVJ440P:
{
    }
    case AV_PIX_FMT_YUVJ444P:
{
    }
    case AV_PIX_FMT_YUVA420P:
{
    }
    case AV_PIX_FMT_YUVA422P:
{
    }
    case AV_PIX_FMT_YUVA444P:
{
    }
    case AV_PIX_FMT_YUV420P9LE:
{
    }
    case AV_PIX_FMT_YUV420P9BE:
{
    }
    case AV_PIX_FMT_YUV420P10LE:
{
    }
    case AV_PIX_FMT_YUV420P10BE:
{
    }
    case AV_PIX_FMT_YUV420P12LE:
{
    }
    case AV_PIX_FMT_YUV420P12BE:
{
    }
    case AV_PIX_FMT_YUV420P14LE:
{
    }
    case AV_PIX_FMT_YUV420P14BE:
{
    }
    case AV_PIX_FMT_YUV422P9LE:
{
    }
    case AV_PIX_FMT_YUV422P9BE:
{
    }
    case AV_PIX_FMT_YUV422P10LE:
{
    }
    case AV_PIX_FMT_YUV422P10BE:
{
    }
    case AV_PIX_FMT_YUV422P12LE:
{
    }
    case AV_PIX_FMT_YUV422P12BE:
{
    }
    case AV_PIX_FMT_YUV422P14LE:
{
    }
    case AV_PIX_FMT_YUV422P14BE:
{
    }
    case AV_PIX_FMT_YUV444P9LE:
{
    }
    case AV_PIX_FMT_YUV444P9BE:
{
    }
    case AV_PIX_FMT_YUV444P10LE:
{
    }
    case AV_PIX_FMT_YUV444P10BE:
{
    }
    case AV_PIX_FMT_YUV444P12LE:
{
    }
    case AV_PIX_FMT_YUV444P12BE:
{
    }
    case AV_PIX_FMT_YUV444P14LE:
{
    }
    case AV_PIX_FMT_YUV444P14BE:
{
    }
    case AV_PIX_FMT_GBRP9LE:
{
    }
    case AV_PIX_FMT_GBRP9BE:
{
    }
    case AV_PIX_FMT_GBRP10LE:
{
    }
    case AV_PIX_FMT_GBRP10BE:
{
    }
    case AV_PIX_FMT_GBRP12LE:
{
    }
    case AV_PIX_FMT_GBRP12BE:
{
    }
    case AV_PIX_FMT_GBRP14LE:
{
    }
    case AV_PIX_FMT_GBRP14BE:
{
//FIXME assume 16 pixel per macroblock
      w_align = 16;
// interlaced needs 2 macroblocks height
      h_align = 16 * 2;
      break; 
    }
    case AV_PIX_FMT_YUV411P:
{
    }
    case AV_PIX_FMT_UYYVYY411:
{
      w_align = 32;
      h_align = 8;
      break; 
    }
    case AV_PIX_FMT_YUV410P:
{
      if ((s -> codec_id) == AV_CODEC_ID_SVQ1) {
        w_align = 64;
        h_align = 64;
      }
      break; 
    }
    case AV_PIX_FMT_RGB555LE:
{
      if ((s -> codec_id) == AV_CODEC_ID_RPZA) {
        w_align = 4;
        h_align = 4;
      }
      break; 
    }
    case AV_PIX_FMT_PAL8:
{
    }
    case AV_PIX_FMT_BGR8:
{
    }
    case AV_PIX_FMT_RGB8:
{
      if ((s -> codec_id) == AV_CODEC_ID_SMC || (s -> codec_id) == AV_CODEC_ID_CINEPAK) {
        w_align = 4;
        h_align = 4;
      }
      break; 
    }
    case AV_PIX_FMT_BGR24:
{
      if ((s -> codec_id) == AV_CODEC_ID_MSZH || (s -> codec_id) == AV_CODEC_ID_ZLIB) {
        w_align = 4;
        h_align = 4;
      }
      break; 
    }
    case AV_PIX_FMT_RGB24:
{
      if ((s -> codec_id) == AV_CODEC_ID_CINEPAK) {
        w_align = 4;
        h_align = 4;
      }
      break; 
    }
    default:
{
      w_align = 1;
      h_align = 1;
      break; 
    }
  }
  if ((s -> codec_id) == AV_CODEC_ID_IFF_ILBM || (s -> codec_id) == AV_CODEC_ID_IFF_BYTERUN1) {
    w_align = (w_align > 8?w_align : 8);
  }
   *width =  *width + w_align - 1 & ~(w_align - 1);
   *height =  *height + h_align - 1 & ~(h_align - 1);
  if ((s -> codec_id) == AV_CODEC_ID_H264 || s -> lowres) {
// some of the optimized chroma MC reads one line too much
// which is also done in mpeg decoders with lowres > 0
     *height += 2;
  }
  for (i = 0; i < 4; i++) 
    linesize_align[i] = 8;
}

void avcodec_align_dimensions(AVCodecContext *s,int *width,int *height)
{
  const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(s -> pix_fmt);
  int chroma_shift = (desc -> log2_chroma_w);
  int linesize_align[8];
  int align;
  avcodec_align_dimensions2(s,width,height,linesize_align);
  align = (linesize_align[0] > linesize_align[3]?linesize_align[0] : linesize_align[3]);
  linesize_align[1] <<= chroma_shift;
  linesize_align[2] <<= chroma_shift;
  align = (((align > linesize_align[1]?align : linesize_align[1])) > linesize_align[2]?((align > linesize_align[1]?align : linesize_align[1])) : linesize_align[2]);
   *width =  *width + align - 1 & ~(align - 1);
}

int avcodec_fill_audio_frame(AVFrame *frame,int nb_channels,enum AVSampleFormat sample_fmt,const uint8_t *buf,int buf_size,int align)
{
  int ch;
  int planar;
  int needed_size;
  int ret = 0;
  needed_size = av_samples_get_buffer_size(((void *)0),nb_channels,frame -> nb_samples,sample_fmt,align);
  if (buf_size < needed_size) {
    return - 22;
  }
  planar = av_sample_fmt_is_planar(sample_fmt);
  if (planar && nb_channels > 8) {
    if (!(frame -> extended_data = (av_mallocz(nb_channels * sizeof(( *frame -> extended_data)))))) {
      return - '\f';
    }
  }
  else {
    frame -> extended_data = frame -> data;
  }
  if ((ret = av_samples_fill_arrays(frame -> extended_data,&frame -> linesize[0],((uint8_t *)((intptr_t )buf)),nb_channels,frame -> nb_samples,sample_fmt,align)) < 0) {
    if (frame -> extended_data != frame -> data) {
      av_freep((&frame -> extended_data));
    }
    return ret;
  }
  if (frame -> extended_data != frame -> data) {
    for (ch = 0; ch < 8; ch++) 
      frame -> data[ch] = frame -> extended_data[ch];
  }
  return ret;
}

static int audio_get_buffer(AVCodecContext *avctx,AVFrame *frame)
{
  AVCodecInternal *avci = avctx -> internal;
  int buf_size;
  int ret;
  av_freep((&avci -> audio_data));
  buf_size = av_samples_get_buffer_size(((void *)0),avctx -> channels,frame -> nb_samples,avctx -> sample_fmt,0);
  if (buf_size < 0) {
    return - 22;
  }
  frame -> data[0] = (av_mallocz(buf_size));
  if (!frame -> data[0]) {
    return - '\f';
  }
  ret = avcodec_fill_audio_frame(frame,avctx -> channels,avctx -> sample_fmt,frame -> data[0],buf_size,0);
  if (ret < 0) {
    av_freep((&frame -> data[0]));
    return ret;
  }
  avci -> audio_data = frame -> data[0];
  if (avctx -> debug & 0x8000) {
    av_log(avctx,48,"default_get_buffer called on frame %p, internal audio buffer used\n",frame);
  }
  return 0;
}

static int video_get_buffer(AVCodecContext *s,AVFrame *pic)
{
  int i;
  int w = s -> width;
  int h = s -> height;
  InternalBuffer *buf;
  AVCodecInternal *avci = s -> internal;
  if (pic -> data[0] != ((void *)0)) {
    av_log(s,16,"pic->data[0]!=NULL in avcodec_default_get_buffer\n");
    return - 1;
  }
  if (avci -> buffer_count >= 32 + 1) {
    av_log(s,16,"buffer_count overflow (missing release_buffer?)\n");
    return - 1;
  }
  if (av_image_check_size(w,h,0,s) || (s -> pix_fmt) < 0) {
    av_log(s,16,"video_get_buffer: image parameters invalid\n");
    return - 1;
  }
  if (!avci -> buffer) {
    avci -> buffer = (av_mallocz((32 + 1 + 1) * sizeof(InternalBuffer )));
  }
  buf = &avci -> buffer[avci -> buffer_count];
  if (buf -> base[0] && (buf -> width != w || buf -> height != h || (buf -> pix_fmt) != (s -> pix_fmt))) {
    for (i = 0; i < 8; i++) {
      av_freep((&buf -> base[i]));
      buf -> data[i] = ((void *)0);
    }
  }
  if (!buf -> base[0]) {
    int h_chroma_shift;
    int v_chroma_shift;
    int size[4] = {(0)};
    int tmpsize;
    int unaligned;
    AVPicture picture;
    int stride_align[8];
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(s -> pix_fmt);
    const int pixel_size = desc -> comp[0] . step_minus1 + 1;
    av_pix_fmt_get_chroma_sub_sample(s -> pix_fmt,&h_chroma_shift,&v_chroma_shift);
    avcodec_align_dimensions2(s,&w,&h,stride_align);
    if (!(s -> flags & 0x4000)) {
      w += 16 * 2;
      h += 16 * 2;
    }
    do {
// NOTE: do not align linesizes individually, this breaks e.g. assumptions
// that linesize[0] == 2*linesize[1] in the MPEG-encoder for 4:2:2
      av_image_fill_linesizes(picture . linesize,s -> pix_fmt,w);
// increase alignment of w for next try (rhs gives the lowest bit set in w)
      w += w & ~(w - 1);
      unaligned = 0;
      for (i = 0; i < 4; i++) 
        unaligned |= picture . linesize[i] % stride_align[i];
    }while (unaligned);
    tmpsize = av_image_fill_pointers(picture . data,s -> pix_fmt,h,((void *)0),picture . linesize);
    if (tmpsize < 0) {
      return - 1;
    }
    for (i = 0; i < 3 && picture . data[i + 1]; i++) 
      size[i] = (picture . data[i + 1] - picture . data[i]);
    size[i] = (tmpsize - (picture . data[i] - picture . data[0]));
    memset((buf -> base),0,sizeof(buf -> base));
    memset((buf -> data),0,sizeof(buf -> data));
    for (i = 0; i < 4 && size[i]; i++) {
      const int h_shift = i == 0?0 : h_chroma_shift;
      const int v_shift = i == 0?0 : v_chroma_shift;
      buf -> linesize[i] = picture . linesize[i];
//FIXME 16
      buf -> base[i] = (av_malloc((size[i] + 16 + 8 - 1)));
      if (buf -> base[i] == ((void *)0)) {
        return - '\f';
      }
// no edge if EDGE EMU or not planar YUV
      if (s -> flags & 0x4000 || !size[2]) {
        buf -> data[i] = buf -> base[i];
      }
      else {
        buf -> data[i] = buf -> base[i] + ((buf -> linesize[i] * 16 >> v_shift) + (pixel_size * 16 >> h_shift) + stride_align[i] - 1 & ~(stride_align[i] - 1));
      }
    }
    for (; i < 8; i++) {
      buf -> base[i] = buf -> data[i] = ((void *)0);
      buf -> linesize[i] = 0;
    }
    if (size[1] && !size[2]) {
      avpriv_set_systematic_pal2(((uint32_t *)buf -> data[1]),s -> pix_fmt);
    }
    buf -> width = s -> width;
    buf -> height = s -> height;
    buf -> pix_fmt = s -> pix_fmt;
  }
  for (i = 0; i < 8; i++) {
    pic -> base[i] = buf -> base[i];
    pic -> data[i] = buf -> data[i];
    pic -> linesize[i] = buf -> linesize[i];
  }
  pic -> extended_data = pic -> data;
  avci -> buffer_count++;
  if (s -> debug & 0x8000) {
    av_log(s,48,"default_get_buffer called on pic %p, %d buffers used\n",pic,avci -> buffer_count);
  }
  return 0;
}

void avpriv_color_frame(AVFrame *frame,const int c[4])
{
  const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get((frame -> format));
  int p;
  int y;
  int x;
  do {
    if (!((desc -> flags) & 16)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","desc->flags & 16","utils.c",518);
      abort();
    }
  }while (0);
  for (p = 0; p < (desc -> nb_components); p++) {
    uint8_t *dst = frame -> data[p];
    int is_chroma = p == 1 || p == 2;
    int bytes = -(-frame -> width >> ((is_chroma?(desc -> log2_chroma_w) : 0)));
    for (y = 0; y < -(-frame -> height >> ((is_chroma?(desc -> log2_chroma_h) : 0))); y++) {
      if (desc -> comp[0] . depth_minus1 >= 8) {
        for (x = 0; x < bytes; x++) 
          ((uint16_t *)dst)[x] = c[p];
      }
      else {
        memset(dst,c[p],bytes);
      }
      dst += frame -> linesize[p];
    }
  }
}

int avcodec_default_get_buffer(AVCodecContext *avctx,AVFrame *frame)
{
  frame -> type = 1;
  switch(avctx -> codec_type){
    case AVMEDIA_TYPE_VIDEO:
    return video_get_buffer(avctx,frame);
    case AVMEDIA_TYPE_AUDIO:
    return audio_get_buffer(avctx,frame);
    default:
    return - 1;
  }
}

void ff_init_buffer_info(AVCodecContext *s,AVFrame *frame)
{
  if (s -> pkt) {
    frame -> pkt_pts = s -> pkt -> pts;
    av_frame_set_pkt_pos(frame,s -> pkt -> pos);
    av_frame_set_pkt_duration(frame,(s -> pkt -> duration));
    av_frame_set_pkt_size(frame,s -> pkt -> size);
  }
  else {
    frame -> pkt_pts = ((int64_t )0x8000000000000000UL);
    av_frame_set_pkt_pos(frame,(- 1));
    av_frame_set_pkt_duration(frame,0);
    av_frame_set_pkt_size(frame,- 1);
  }
  frame -> reordered_opaque = s -> reordered_opaque;
  switch(s -> codec -> type){
    case AVMEDIA_TYPE_VIDEO:
{
      frame -> width = s -> width;
      frame -> height = s -> height;
      frame -> format = (s -> pix_fmt);
      frame -> sample_aspect_ratio = s -> sample_aspect_ratio;
      break; 
    }
    case AVMEDIA_TYPE_AUDIO:
{
      frame -> sample_rate = s -> sample_rate;
      frame -> format = (s -> sample_fmt);
      frame -> channel_layout = s -> channel_layout;
      av_frame_set_channels(frame,s -> channels);
      break; 
    }
  }
}

int ff_get_buffer(AVCodecContext *avctx,AVFrame *frame)
{
  ff_init_buffer_info(avctx,frame);
  return (avctx -> get_buffer)(avctx,frame);
}

void avcodec_default_release_buffer(AVCodecContext *s,AVFrame *pic)
{
  int i;
  InternalBuffer *buf;
  InternalBuffer *last;
  AVCodecInternal *avci = s -> internal;
  do {
    if (!((s -> codec_type) == AVMEDIA_TYPE_VIDEO)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","s->codec_type == AVMEDIA_TYPE_VIDEO","utils.c",592);
      abort();
    }
  }while (0);
  (void )0;
  (void )0;
  if (avci -> buffer) {
/* avoids warning */
    buf = ((void *)0);
//just 3-5 checks so is not worth to optimize
    for (i = 0; i < avci -> buffer_count; i++) {
      buf = &avci -> buffer[i];
      if (buf -> data[0] == pic -> data[0]) {
        break; 
      }
    }
    do {
      if (!(i < avci -> buffer_count)) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","i < avci->buffer_count","utils.c",604);
        abort();
      }
    }while (0);
    avci -> buffer_count--;
    last = &avci -> buffer[avci -> buffer_count];
    if (buf != last) {
      do {
        InternalBuffer SWAP_tmp =  *last;
         *last =  *buf;
         *buf = SWAP_tmp;
      }while (0);
    }
  }
  for (i = 0; i < 8; i++) 
    pic -> data[i] = ((void *)0);
//        pic->base[i]=NULL;
  if (s -> debug & 0x8000) {
    av_log(s,48,"default_release_buffer called on pic %p, %d buffers used\n",pic,avci -> buffer_count);
  }
}

int avcodec_default_reget_buffer(AVCodecContext *s,AVFrame *pic)
{
  AVFrame temp_pic;
  int i;
  int ret;
  do {
    if (!((s -> codec_type) == AVMEDIA_TYPE_VIDEO)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","s->codec_type == AVMEDIA_TYPE_VIDEO","utils.c",626);
      abort();
    }
  }while (0);
  if (pic -> data[0] && (pic -> width != s -> width || pic -> height != s -> height || pic -> format != (s -> pix_fmt))) {
    av_log(s,24,"Picture changed from size:%dx%d fmt:%s to size:%dx%d fmt:%s in reget buffer()\n",pic -> width,pic -> height,av_get_pix_fmt_name((pic -> format)),s -> width,s -> height,av_get_pix_fmt_name(s -> pix_fmt));
    (s -> release_buffer)(s,pic);
  }
  ff_init_buffer_info(s,pic);
/* If no picture return a new buffer */
  if (pic -> data[0] == ((void *)0)) {
/* We will copy from buffer, so must be readable */
    pic -> buffer_hints |= 0x02;
    return ff_get_buffer(s,pic);
  }
  (void )0;
/* If internal buffer type return the same buffer */
  if (pic -> type == 1) {
    return 0;
  }
/*
     * Not internal type and reget_buffer not overridden, emulate cr buffer
     */
  temp_pic =  *pic;
  for (i = 0; i < 8; i++) 
    pic -> data[i] = pic -> base[i] = ((void *)0);
  pic -> opaque = ((void *)0);
/* Allocate new frame */
  if (ret = ff_get_buffer(s,pic)) {
    return ret;
  }
/* Copy image data from old buffer to new buffer */
  av_picture_copy(((AVPicture *)pic),((AVPicture *)(&temp_pic)),s -> pix_fmt,s -> width,s -> height);
// Release old frame
  (s -> release_buffer)(s,&temp_pic);
  return 0;
}

int avcodec_default_execute(AVCodecContext *c,int (*func)(AVCodecContext *, void *),void *arg,int *ret,int count,int size)
{
  int i;
  for (i = 0; i < count; i++) {
    int r = func(c,(((char *)arg) + i * size));
    if (ret) {
      ret[i] = r;
    }
  }
  return 0;
}

int avcodec_default_execute2(AVCodecContext *c,int (*func)(AVCodecContext *, void *, int , int ),void *arg,int *ret,int count)
{
  int i;
  for (i = 0; i < count; i++) {
    int r = func(c,arg,i,0);
    if (ret) {
      ret[i] = r;
    }
  }
  return 0;
}

static int is_hwaccel_pix_fmt(enum AVPixelFormat pix_fmt)
{
  const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(pix_fmt);
  return (desc -> flags) & 8;
}

enum AVPixelFormat avcodec_default_get_format(struct AVCodecContext *s,const enum AVPixelFormat *fmt)
{
  while(( *fmt) != AV_PIX_FMT_NONE && is_hwaccel_pix_fmt( *fmt))
    ++fmt;
  return fmt[0];
}

void avcodec_get_frame_defaults(AVFrame *frame)
{
#if LIBAVCODEC_VERSION_MAJOR >= 55
// extended_data should explicitly be freed when needed, this code is unsafe currently
// also this is not compatible to the <55 ABI/API
#endif
  memset(frame,0,sizeof(AVFrame ));
  frame -> pts = frame -> pkt_dts = frame -> pkt_pts = ((int64_t )0x8000000000000000UL);
  av_frame_set_best_effort_timestamp(frame,((int64_t )0x8000000000000000UL));
  av_frame_set_pkt_duration(frame,0);
  av_frame_set_pkt_pos(frame,(- 1));
  av_frame_set_pkt_size(frame,- 1);
  frame -> key_frame = 1;
  frame -> sample_aspect_ratio = ((AVRational ){(0), (1)});
/* unknown */
  frame -> format = - 1;
  frame -> extended_data = frame -> data;
}

AVFrame *avcodec_alloc_frame()
{
  AVFrame *frame = (av_malloc(sizeof(AVFrame )));
  if (frame == ((void *)0)) {
    return ((void *)0);
  }
  frame -> extended_data = ((void *)0);
  avcodec_get_frame_defaults(frame);
  return frame;
}

void avcodec_free_frame(AVFrame **frame)
{
  AVFrame *f;
  if (!frame || !( *frame)) {
    return ;
  }
  f =  *frame;
  if (f -> extended_data != f -> data) {
    av_freep((&f -> extended_data));
  }
  av_freep(frame);
}
#define MAKE_ACCESSORS(str, name, type, field) \
    type av_##name##_get_##field(const str *s) { return s->field; } \
    void av_##name##_set_##field(str *s, type v) { s->field = v; }

int64_t av_frame_get_best_effort_timestamp(const AVFrame *s)
{
  return s -> best_effort_timestamp;
}

void av_frame_set_best_effort_timestamp(AVFrame *s,int64_t v)
{
  s -> best_effort_timestamp = v;
}

int64_t av_frame_get_pkt_duration(const AVFrame *s)
{
  return s -> pkt_duration;
}

void av_frame_set_pkt_duration(AVFrame *s,int64_t v)
{
  s -> pkt_duration = v;
}

int64_t av_frame_get_pkt_pos(const AVFrame *s)
{
  return s -> pkt_pos;
}

void av_frame_set_pkt_pos(AVFrame *s,int64_t v)
{
  s -> pkt_pos = v;
}

int64_t av_frame_get_channel_layout(const AVFrame *s)
{
  return (s -> channel_layout);
}

void av_frame_set_channel_layout(AVFrame *s,int64_t v)
{
  s -> channel_layout = v;
}

int av_frame_get_channels(const AVFrame *s)
{
  return s -> channels;
}

void av_frame_set_channels(AVFrame *s,int v)
{
  s -> channels = v;
}

int av_frame_get_sample_rate(const AVFrame *s)
{
  return s -> sample_rate;
}

void av_frame_set_sample_rate(AVFrame *s,int v)
{
  s -> sample_rate = v;
}

AVDictionary *av_frame_get_metadata(const AVFrame *s)
{
  return s -> metadata;
}

void av_frame_set_metadata(AVFrame *s,AVDictionary *v)
{
  s -> metadata = v;
}

int av_frame_get_decode_error_flags(const AVFrame *s)
{
  return s -> decode_error_flags;
}

void av_frame_set_decode_error_flags(AVFrame *s,int v)
{
  s -> decode_error_flags = v;
}

int av_frame_get_pkt_size(const AVFrame *s)
{
  return s -> pkt_size;
}

void av_frame_set_pkt_size(AVFrame *s,int v)
{
  s -> pkt_size = v;
}

AVDictionary **ff_frame_get_metadatap(AVFrame *frame)
{
  return &frame -> metadata;
}

AVRational av_codec_get_pkt_timebase(const AVCodecContext *s)
{
  return s -> pkt_timebase;
}

void av_codec_set_pkt_timebase(AVCodecContext *s,AVRational v)
{
  s -> pkt_timebase = v;
}

const AVCodecDescriptor *av_codec_get_codec_descriptor(const AVCodecContext *s)
{
  return s -> codec_descriptor;
}

void av_codec_set_codec_descriptor(AVCodecContext *s,const AVCodecDescriptor *v)
{
  s -> codec_descriptor = v;
}

static void avcodec_get_subtitle_defaults(AVSubtitle *sub)
{
  memset(sub,0,sizeof(( *sub)));
  sub -> pts = ((int64_t )0x8000000000000000UL);
}

static int get_bit_rate(AVCodecContext *ctx)
{
  int bit_rate;
  int bits_per_sample;
  switch(ctx -> codec_type){
    case AVMEDIA_TYPE_VIDEO:
{
    }
    case AVMEDIA_TYPE_DATA:
{
    }
    case AVMEDIA_TYPE_SUBTITLE:
{
    }
    case AVMEDIA_TYPE_ATTACHMENT:
{
      bit_rate = ctx -> bit_rate;
      break; 
    }
    case AVMEDIA_TYPE_AUDIO:
{
      bits_per_sample = av_get_bits_per_sample(ctx -> codec_id);
      bit_rate = (bits_per_sample?ctx -> sample_rate * ctx -> channels * bits_per_sample : ctx -> bit_rate);
      break; 
    }
    default:
{
      bit_rate = 0;
      break; 
    }
  }
  return bit_rate;
}
#if FF_API_AVCODEC_OPEN

int avcodec_open(AVCodecContext *avctx,AVCodec *codec)
{
  return avcodec_open2(avctx,codec,((void *)0));
}
#endif

int ff_codec_open2_recursive(AVCodecContext *avctx,const AVCodec *codec,AVDictionary **options)
{
  int ret = 0;
  ff_unlock_avcodec();
  ret = avcodec_open2(avctx,codec,options);
  ff_lock_avcodec(avctx);
  return ret;
}

int avcodec_open2(AVCodecContext *avctx,const AVCodec *codec,AVDictionary **options)
{
  int ret = 0;
  AVDictionary *tmp = ((void *)0);
  if (avcodec_is_open(avctx)) {
    return 0;
  }
  if (!codec && !avctx -> codec) {
    av_log(avctx,16,"No codec provided to avcodec_open2()\n");
    return - 22;
  }
  if (codec && avctx -> codec && codec != avctx -> codec) {
    av_log(avctx,16,"This AVCodecContext was allocated for %s, but %s passed to avcodec_open2()\n",avctx -> codec -> name,codec -> name);
    return - 22;
  }
  if (!codec) {
    codec = avctx -> codec;
  }
  if (avctx -> extradata_size < 0 || avctx -> extradata_size >= (1 << 28) - 16) {
    return - 22;
  }
  if (options) {
    av_dict_copy(&tmp, *options,0);
  }
  ret = ff_lock_avcodec(avctx);
  if (ret < 0) {
    return ret;
  }
  avctx -> internal = (av_mallocz(sizeof(AVCodecInternal )));
  if (!avctx -> internal) {
    ret = - '\f';
    goto end;
  }
  if (codec -> priv_data_size > 0) {
    if (!avctx -> priv_data) {
      avctx -> priv_data = av_mallocz((codec -> priv_data_size));
      if (!avctx -> priv_data) {
        ret = - '\f';
        goto end;
      }
      if (codec -> priv_class) {
         *((const AVClass **)(avctx -> priv_data)) = codec -> priv_class;
        av_opt_set_defaults(avctx -> priv_data);
      }
    }
    if (codec -> priv_class && (ret = av_opt_set_dict(avctx -> priv_data,&tmp)) < 0) {
      goto free_and_end;
    }
  }
  else {
    avctx -> priv_data = ((void *)0);
  }
  if ((ret = av_opt_set_dict(avctx,&tmp)) < 0) {
    goto free_and_end;
  }
//We only call avcodec_set_dimensions() for non h264 codecs so as not to overwrite previously setup dimensions
  if (!(avctx -> coded_width && avctx -> coded_height && avctx -> width && avctx -> height && (avctx -> codec_id) == AV_CODEC_ID_H264)) {
    if (avctx -> coded_width && avctx -> coded_height) {
      avcodec_set_dimensions(avctx,avctx -> coded_width,avctx -> coded_height);
    }
    else {
      if (avctx -> width && avctx -> height) {
        avcodec_set_dimensions(avctx,avctx -> width,avctx -> height);
      }
    }
  }
  if ((avctx -> coded_width || avctx -> coded_height || avctx -> width || avctx -> height) && (av_image_check_size((avctx -> coded_width),(avctx -> coded_height),0,avctx) < 0 || av_image_check_size((avctx -> width),(avctx -> height),0,avctx) < 0)) {
    av_log(avctx,24,"Ignoring invalid width/height values\n");
    avcodec_set_dimensions(avctx,0,0);
  }
/* if the decoder init function was already called previously,
     * free the already allocated subtitle_header before overwriting it */
  if (av_codec_is_decoder(codec)) {
    av_freep((&avctx -> subtitle_header));
  }
  if ((avctx -> channels) > 128U) {
    ret = - 22;
    goto free_and_end;
  }
  avctx -> codec = codec;
  if (((avctx -> codec_type) == AVMEDIA_TYPE_UNKNOWN || (avctx -> codec_type) == (codec -> type)) && (avctx -> codec_id) == AV_CODEC_ID_NONE) {
    avctx -> codec_type = codec -> type;
    avctx -> codec_id = codec -> id;
  }
  if ((avctx -> codec_id) != (codec -> id) || (avctx -> codec_type) != (codec -> type) && (avctx -> codec_type) != AVMEDIA_TYPE_ATTACHMENT) {
    av_log(avctx,16,"Codec type or id mismatches\n");
    ret = - 22;
    goto free_and_end;
  }
  avctx -> frame_number = 0;
  avctx -> codec_descriptor = avcodec_descriptor_get(avctx -> codec_id);
  if (avctx -> codec -> capabilities & 0x0200 && avctx -> strict_std_compliance > - 2) {
    const char *codec_string = (av_codec_is_encoder(codec)?"encoder" : "decoder");
    AVCodec *codec2;
    av_log(((void *)0),16,"The %s '%s' is experimental but experimental codecs are not enabled, add '-strict %d' if you want to use it.\n",codec_string,codec -> name,- 2);
    codec2 = (av_codec_is_encoder(codec)?avcodec_find_encoder(codec -> id) : avcodec_find_decoder(codec -> id));
    if (!(codec2 -> capabilities & 0x0200)) {
      av_log(((void *)0),16,"Alternatively use the non experimental %s '%s'.\n",codec_string,codec2 -> name);
    }
    ret = - 0x2bb2afa8;
    goto free_and_end;
  }
  if ((avctx -> codec_type) == AVMEDIA_TYPE_AUDIO && (!avctx -> time_base . num || !avctx -> time_base . den)) {
    avctx -> time_base . num = 1;
    avctx -> time_base . den = avctx -> sample_rate;
  }
  if (!1) {
    av_log(avctx,24,"Warning: not compiled with thread support, using thread emulation\n");
  }
  if (1) {
//we will instanciate a few encoders thus kick the counter to prevent false detection of a problem
    ff_unlock_avcodec();
    ret = ff_frame_thread_encoder_init(avctx,(options? *options : ((void *)0)));
    ff_lock_avcodec(avctx);
    if (ret < 0) {
      goto free_and_end;
    }
  }
  if (1 && !avctx -> thread_opaque && !(avctx -> internal -> frame_thread_encoder && avctx -> active_thread_type & 1)) {
    ret = ff_thread_init(avctx);
    if (ret < 0) {
      goto free_and_end;
    }
  }
  if (!1 && !(codec -> capabilities & 0x8000)) {
    avctx -> thread_count = 1;
  }
  if ((avctx -> codec -> max_lowres) < avctx -> lowres || avctx -> lowres < 0) {
    av_log(avctx,16,"The maximum value for lowres supported by the decoder is %d\n",(avctx -> codec -> max_lowres));
    ret = - 22;
    goto free_and_end;
  }
  if (av_codec_is_encoder(avctx -> codec)) {
    int i;
    if (avctx -> codec -> sample_fmts) {
      for (i = 0; avctx -> codec -> sample_fmts[i] != AV_SAMPLE_FMT_NONE; i++) {
        if ((avctx -> sample_fmt) == avctx -> codec -> sample_fmts[i]) {
          break; 
        }
        if (avctx -> channels == 1 && (av_get_planar_sample_fmt(avctx -> sample_fmt)) == (av_get_planar_sample_fmt(avctx -> codec -> sample_fmts[i]))) {
          avctx -> sample_fmt = avctx -> codec -> sample_fmts[i];
          break; 
        }
      }
      if (avctx -> codec -> sample_fmts[i] == AV_SAMPLE_FMT_NONE) {
        char buf[128];
        snprintf(buf,sizeof(buf),"%d",avctx -> sample_fmt);
        av_log(avctx,16,"Specified sample format %s is invalid or not supported\n",((char *)(av_x_if_null((av_get_sample_fmt_name(avctx -> sample_fmt)),buf))));
        ret = - 22;
        goto free_and_end;
      }
    }
    if (avctx -> codec -> pix_fmts) {
      for (i = 0; avctx -> codec -> pix_fmts[i] != AV_PIX_FMT_NONE; i++) 
        if ((avctx -> pix_fmt) == avctx -> codec -> pix_fmts[i]) {
          break; 
        }
      if (avctx -> codec -> pix_fmts[i] == AV_PIX_FMT_NONE && !(((avctx -> codec_id) == AV_CODEC_ID_MJPEG || (avctx -> codec_id) == AV_CODEC_ID_LJPEG) && avctx -> strict_std_compliance <= - 1)) {
        char buf[128];
        snprintf(buf,sizeof(buf),"%d",avctx -> pix_fmt);
        av_log(avctx,16,"Specified pixel format %s is invalid or not supported\n",((char *)(av_x_if_null((av_get_pix_fmt_name(avctx -> pix_fmt)),buf))));
        ret = - 22;
        goto free_and_end;
      }
    }
    if (avctx -> codec -> supported_samplerates) {
      for (i = 0; avctx -> codec -> supported_samplerates[i] != 0; i++) 
        if (avctx -> sample_rate == avctx -> codec -> supported_samplerates[i]) {
          break; 
        }
      if (avctx -> codec -> supported_samplerates[i] == 0) {
        av_log(avctx,16,"Specified sample rate %d is not supported\n",avctx -> sample_rate);
        ret = - 22;
        goto free_and_end;
      }
    }
    if (avctx -> codec -> channel_layouts) {
      if (!avctx -> channel_layout) {
        av_log(avctx,24,"Channel layout not specified\n");
      }
      else {
        for (i = 0; avctx -> codec -> channel_layouts[i] != 0; i++) 
          if (avctx -> channel_layout == avctx -> codec -> channel_layouts[i]) {
            break; 
          }
        if (avctx -> codec -> channel_layouts[i] == 0) {
          char buf[512];
          av_get_channel_layout_string(buf,(sizeof(buf)),- 1,avctx -> channel_layout);
          av_log(avctx,16,"Specified channel layout '%s' is not supported\n",buf);
          ret = - 22;
          goto free_and_end;
        }
      }
    }
    if (avctx -> channel_layout && avctx -> channels) {
      int channels = av_get_channel_layout_nb_channels(avctx -> channel_layout);
      if (channels != avctx -> channels) {
        char buf[512];
        av_get_channel_layout_string(buf,(sizeof(buf)),- 1,avctx -> channel_layout);
        av_log(avctx,16,"Channel layout '%s' with %d channels does not match number of specified channels %d\n",buf,channels,avctx -> channels);
        ret = - 22;
        goto free_and_end;
      }
    }
    else {
      if (avctx -> channel_layout) {
        avctx -> channels = av_get_channel_layout_nb_channels(avctx -> channel_layout);
      }
    }
    if ((avctx -> codec_type) == AVMEDIA_TYPE_VIDEO && (avctx -> codec_id) != AV_CODEC_ID_PNG) 
// For mplayer
{
      if (avctx -> width <= 0 || avctx -> height <= 0) {
        av_log(avctx,16,"dimensions not set\n");
        ret = - 22;
        goto free_and_end;
      }
    }
    if (((avctx -> codec_type) == AVMEDIA_TYPE_VIDEO || (avctx -> codec_type) == AVMEDIA_TYPE_AUDIO) && avctx -> bit_rate > 0 && avctx -> bit_rate < 1000) {
      av_log(avctx,24,"Bitrate %d is extreemly low, did you mean %dk\n",avctx -> bit_rate,avctx -> bit_rate);
    }
    if (!avctx -> rc_initial_buffer_occupancy) {
      avctx -> rc_initial_buffer_occupancy = avctx -> rc_buffer_size * 3 / 4;
    }
  }
  avctx -> pts_correction_num_faulty_pts = avctx -> pts_correction_num_faulty_dts = 0;
  avctx -> pts_correction_last_pts = avctx -> pts_correction_last_dts = - 9223372036854775807L - 1;
  if (avctx -> codec -> init && (!(avctx -> active_thread_type & 1) || avctx -> internal -> frame_thread_encoder)) {
    ret = ((avctx -> codec -> init)(avctx));
    if (ret < 0) {
      goto free_and_end;
    }
  }
  ret = 0;
  if (av_codec_is_decoder(avctx -> codec)) {
    if (!avctx -> bit_rate) {
      avctx -> bit_rate = get_bit_rate(avctx);
    }
/* validate channel layout from the decoder */
    if (avctx -> channel_layout) {
      int channels = av_get_channel_layout_nb_channels(avctx -> channel_layout);
      if (!avctx -> channels) {
        avctx -> channels = channels;
      }
      else {
        if (channels != avctx -> channels) {
          char buf[512];
          av_get_channel_layout_string(buf,(sizeof(buf)),- 1,avctx -> channel_layout);
          av_log(avctx,24,"Channel layout '%s' with %d channels does not match specified number of channels %d: ignoring specified channel layout\n",buf,channels,avctx -> channels);
          avctx -> channel_layout = 0;
        }
      }
    }
    if (avctx -> channels && avctx -> channels < 0 || (avctx -> channels) > 128U) {
      ret = - 22;
      goto free_and_end;
    }
    if (avctx -> sub_charenc) {
      if ((avctx -> codec_type) != AVMEDIA_TYPE_SUBTITLE) {
        av_log(avctx,16,"Character encoding is only supported with subtitles codecs\n");
        ret = - 22;
        goto free_and_end;
      }
      else {
        if (avctx -> codec_descriptor -> props & 1 << 16) {
          av_log(avctx,24,"Codec '%s' is bitmap-based, subtitles character encoding will be ignored\n",avctx -> codec_descriptor -> name);
          avctx -> sub_charenc_mode = - 1;
        }
        else {
/* input character encoding is set for a text based subtitle
                 * codec at this point */
          if (avctx -> sub_charenc_mode == 0) {
            avctx -> sub_charenc_mode = 1;
          }
          if (!1 && avctx -> sub_charenc_mode == 1) {
            av_log(avctx,16,"Character encoding subtitles conversion needs a libavcodec built with iconv support for this codec\n");
            ret = - 38;
            goto free_and_end;
          }
        }
      }
    }
  }
  end:
  ff_unlock_avcodec();
  if (options) {
    av_dict_free(options);
     *options = tmp;
  }
  return ret;
  free_and_end:
  av_dict_free(&tmp);
  av_freep((&avctx -> priv_data));
  av_freep((&avctx -> internal));
  avctx -> codec = ((void *)0);
  goto end;
}

int ff_alloc_packet2(AVCodecContext *avctx,AVPacket *avpkt,int size)
{
  if (size < 0 || avpkt -> size < 0 || size > 2147483647 - 16) {
    av_log(avctx,16,"Size %d invalid\n",size);
    return - 22;
  }
  if (avctx) {
    do {
      if (!(!avpkt -> data || avpkt -> data != avctx -> internal -> byte_buffer)) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","!avpkt->data || avpkt->data != avctx->internal->byte_buffer","utils.c",1155);
        abort();
      }
    }while (0);
    if (!avpkt -> data || avpkt -> size < size) {
      av_fast_padded_malloc((&avctx -> internal -> byte_buffer),&avctx -> internal -> byte_buffer_size,size);
      avpkt -> data = avctx -> internal -> byte_buffer;
      avpkt -> size = (avctx -> internal -> byte_buffer_size);
      avpkt -> destruct = ((void *)0);
    }
  }
  if (avpkt -> data) {
    void *destruct = (avpkt -> destruct);
    if (avpkt -> size < size) {
      av_log(avctx,16,"User packet is too small (%d < %d)\n",avpkt -> size,size);
      return - 22;
    }
    av_init_packet(avpkt);
    avpkt -> destruct = destruct;
    avpkt -> size = size;
    return 0;
  }
  else {
    int ret = av_new_packet(avpkt,size);
    if (ret < 0) {
      av_log(avctx,16,"Failed to allocate packet of size %d\n",size);
    }
    return ret;
  }
}

int ff_alloc_packet(AVPacket *avpkt,int size)
{
  return ff_alloc_packet2(((void *)0),avpkt,size);
}
/**
 * Pad last frame with silence.
 */

static int pad_last_frame(AVCodecContext *s,AVFrame **dst,const AVFrame *src)
{
  AVFrame *frame = ((void *)0);
  uint8_t *buf = ((void *)0);
  int ret;
  if (!(frame = avcodec_alloc_frame())) {
    return - '\f';
  }
   *frame =  *src;
  if ((ret = av_samples_get_buffer_size(&frame -> linesize[0],s -> channels,s -> frame_size,s -> sample_fmt,0)) < 0) {
    goto fail;
  }
  if (!(buf = (av_malloc(ret)))) {
    ret = - '\f';
    goto fail;
  }
  frame -> nb_samples = s -> frame_size;
  if ((ret = avcodec_fill_audio_frame(frame,s -> channels,s -> sample_fmt,buf,ret,0)) < 0) {
    goto fail;
  }
  if ((ret = av_samples_copy(frame -> extended_data,(src -> extended_data),0,0,src -> nb_samples,s -> channels,s -> sample_fmt)) < 0) {
    goto fail;
  }
  if ((ret = av_samples_set_silence(frame -> extended_data,src -> nb_samples,frame -> nb_samples - src -> nb_samples,s -> channels,s -> sample_fmt)) < 0) {
    goto fail;
  }
   *dst = frame;
  return 0;
  fail:
  if (frame -> extended_data != frame -> data) {
    av_freep((&frame -> extended_data));
  }
  av_freep((&buf));
  av_freep((&frame));
  return ret;
}

int avcodec_encode_audio2(AVCodecContext *avctx,AVPacket *avpkt,const AVFrame *frame,int *got_packet_ptr)
{
  AVFrame tmp;
  AVFrame *padded_frame = ((void *)0);
  int ret;
  AVPacket user_pkt =  *avpkt;
  int needs_realloc = !user_pkt . data;
   *got_packet_ptr = 0;
  if (!(avctx -> codec -> capabilities & 0x20) && !frame) {
    av_free_packet(avpkt);
    av_init_packet(avpkt);
    return 0;
  }
/* ensure that extended_data is properly set */
  if (frame && !frame -> extended_data) {
    if (av_sample_fmt_is_planar(avctx -> sample_fmt) && avctx -> channels > 8) {
      av_log(avctx,16,"Encoding to a planar sample format, with more than %d channels, but extended_data is not set.\n",8);
      return - 22;
    }
    av_log(avctx,24,"extended_data is not set.\n");
    tmp =  *frame;
    tmp . extended_data = tmp . data;
    frame = (&tmp);
  }
/* check for valid frame size */
  if (frame) {
    if (avctx -> codec -> capabilities & 0x0040) {
      if (frame -> nb_samples > avctx -> frame_size) {
        av_log(avctx,16,"more samples than frame size (avcodec_encode_audio2)\n");
        return - 22;
      }
    }
    else {
      if (!(avctx -> codec -> capabilities & 0x10000)) {
        if (frame -> nb_samples < avctx -> frame_size && !avctx -> internal -> last_audio_frame) {
          ret = pad_last_frame(avctx,&padded_frame,frame);
          if (ret < 0) {
            return ret;
          }
          frame = padded_frame;
          avctx -> internal -> last_audio_frame = 1;
        }
        if (frame -> nb_samples != avctx -> frame_size) {
          av_log(avctx,16,"nb_samples (%d) != frame_size (%d) (avcodec_encode_audio2)\n",frame -> nb_samples,avctx -> frame_size);
          ret = - 22;
          goto end;
        }
      }
    }
  }
  ret = ((avctx -> codec -> encode2)(avctx,avpkt,frame,got_packet_ptr));
  if (!ret) {
    if ( *got_packet_ptr) {
      if (!(avctx -> codec -> capabilities & 0x20)) {
        if (avpkt -> pts == ((int64_t )0x8000000000000000UL)) {
          avpkt -> pts = frame -> pts;
        }
        if (!avpkt -> duration) {
          avpkt -> duration = (ff_samples_to_time_base(avctx,(frame -> nb_samples)));
        }
      }
      avpkt -> dts = avpkt -> pts;
    }
    else {
      avpkt -> size = 0;
    }
  }
  if (avpkt -> data && avpkt -> data == avctx -> internal -> byte_buffer) {
    needs_realloc = 0;
    if (user_pkt . data) {
      if (user_pkt . size >= avpkt -> size) {
        memcpy(user_pkt . data,(avpkt -> data),(avpkt -> size));
      }
      else {
        av_log(avctx,16,"Provided packet is too small, needs to be %d\n",avpkt -> size);
        avpkt -> size = user_pkt . size;
        ret = - 1;
      }
      avpkt -> data = user_pkt . data;
      avpkt -> destruct = user_pkt . destruct;
    }
    else {
      if (av_dup_packet(avpkt) < 0) {
        ret = - '\f';
      }
    }
  }
  if (!ret) {
    if (needs_realloc && avpkt -> data) {
      uint8_t *new_data = (av_realloc((avpkt -> data),(avpkt -> size + 16)));
      if (new_data) {
        avpkt -> data = new_data;
      }
    }
    avctx -> frame_number++;
  }
  if (ret < 0 || !( *got_packet_ptr)) {
    av_free_packet(avpkt);
    av_init_packet(avpkt);
    goto end;
  }
/* NOTE: if we add any audio encoders which output non-keyframe packets,
     *       this needs to be moved to the encoders, but for now we can do it
     *       here to simplify things */
  avpkt -> flags |= 0x1;
  end:
  if (padded_frame) {
    av_freep((&padded_frame -> data[0]));
    if (padded_frame -> extended_data != padded_frame -> data) {
      av_freep((&padded_frame -> extended_data));
    }
    av_freep((&padded_frame));
  }
  return ret;
}
#if FF_API_OLD_ENCODE_AUDIO

int avcodec_encode_audio(AVCodecContext *avctx,uint8_t *buf,int buf_size,const short *samples)
{
  AVPacket pkt;
  AVFrame frame0 = {{(0)}};
  AVFrame *frame;
  int ret;
  int samples_size;
  int got_packet;
  av_init_packet(&pkt);
  pkt . data = buf;
  pkt . size = buf_size;
  if (samples) {
    frame = &frame0;
    avcodec_get_frame_defaults(frame);
    if (avctx -> frame_size) {
      frame -> nb_samples = avctx -> frame_size;
    }
    else {
/* if frame_size is not set, the number of samples must be
             * calculated from the buffer size */
      int64_t nb_samples;
      if (!av_get_bits_per_sample(avctx -> codec_id)) {
        av_log(avctx,16,"avcodec_encode_audio() does not support this codec\n");
        return - 22;
      }
      nb_samples = ((int64_t )buf_size) * 8 / (av_get_bits_per_sample(avctx -> codec_id) * avctx -> channels);
      if (nb_samples >= 2147483647) {
        return - 22;
      }
      frame -> nb_samples = nb_samples;
    }
/* it is assumed that the samples buffer is large enough based on the
         * relevant parameters */
    samples_size = av_samples_get_buffer_size(((void *)0),avctx -> channels,frame -> nb_samples,avctx -> sample_fmt,1);
    if ((ret = avcodec_fill_audio_frame(frame,avctx -> channels,avctx -> sample_fmt,((const uint8_t *)samples),samples_size,1)) < 0) {
      return ret;
    }
/* fabricate frame pts from sample count.
         * this is needed because the avcodec_encode_audio() API does not have
         * a way for the user to provide pts */
    if (avctx -> sample_rate && avctx -> time_base . num) {
      frame -> pts = ff_samples_to_time_base(avctx,(avctx -> internal -> sample_count));
    }
    else {
      frame -> pts = ((int64_t )0x8000000000000000UL);
    }
    avctx -> internal -> sample_count += frame -> nb_samples;
  }
  else {
    frame = ((void *)0);
  }
  got_packet = 0;
  ret = avcodec_encode_audio2(avctx,&pkt,frame,&got_packet);
  if (!ret && got_packet && avctx -> coded_frame) {
    avctx -> coded_frame -> pts = pkt . pts;
    avctx -> coded_frame -> key_frame = !(!(pkt . flags & 0x1));
  }
/* free any side data since we cannot return it */
  ff_packet_free_side_data(&pkt);
  if (frame && frame -> extended_data != frame -> data) {
    av_freep((&frame -> extended_data));
  }
  return ret?ret : pkt . size;
}
#endif
#if FF_API_OLD_ENCODE_VIDEO

int avcodec_encode_video(AVCodecContext *avctx,uint8_t *buf,int buf_size,const AVFrame *pict)
{
  AVPacket pkt;
  int ret;
  int got_packet = 0;
  if (buf_size < 16384) {
    av_log(avctx,16,"buffer smaller than minimum size\n");
    return - 1;
  }
  av_init_packet(&pkt);
  pkt . data = buf;
  pkt . size = buf_size;
  ret = avcodec_encode_video2(avctx,&pkt,pict,&got_packet);
  if (!ret && got_packet && avctx -> coded_frame) {
    avctx -> coded_frame -> pts = pkt . pts;
    avctx -> coded_frame -> key_frame = !(!(pkt . flags & 0x1));
  }
/* free any side data since we cannot return it */
  if (pkt . side_data_elems > 0) {
    int i;
    for (i = 0; i < pkt . side_data_elems; i++) 
      av_free(pkt . side_data[i] . data);
    av_freep((&pkt . side_data));
    pkt . side_data_elems = 0;
  }
  return ret?ret : pkt . size;
}
#endif

int avcodec_encode_video2(AVCodecContext *avctx,AVPacket *avpkt,const AVFrame *frame,int *got_packet_ptr)
{
  int ret;
  AVPacket user_pkt =  *avpkt;
  int needs_realloc = !user_pkt . data;
   *got_packet_ptr = 0;
  if (1 && avctx -> internal -> frame_thread_encoder && avctx -> active_thread_type & 1) {
    return ff_thread_video_encode_frame(avctx,avpkt,frame,got_packet_ptr);
  }
  if (avctx -> flags & 0x0200 && avctx -> stats_out) {
    avctx -> stats_out[0] = '\0';
  }
  if (!(avctx -> codec -> capabilities & 0x20) && !frame) {
    av_free_packet(avpkt);
    av_init_packet(avpkt);
    avpkt -> size = 0;
    return 0;
  }
  if (av_image_check_size((avctx -> width),(avctx -> height),0,avctx)) {
    return - 22;
  }
  do {
    if (!avctx -> codec -> encode2) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","avctx->codec->encode2","utils.c",1503);
      abort();
    }
  }while (0);
  ret = ((avctx -> codec -> encode2)(avctx,avpkt,frame,got_packet_ptr));
  do {
    if (!(ret <= 0)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","ret <= 0","utils.c",1506);
      abort();
    }
  }while (0);
  if (avpkt -> data && avpkt -> data == avctx -> internal -> byte_buffer) {
    needs_realloc = 0;
    if (user_pkt . data) {
      if (user_pkt . size >= avpkt -> size) {
        memcpy(user_pkt . data,(avpkt -> data),(avpkt -> size));
      }
      else {
        av_log(avctx,16,"Provided packet is too small, needs to be %d\n",avpkt -> size);
        avpkt -> size = user_pkt . size;
        ret = - 1;
      }
      avpkt -> data = user_pkt . data;
      avpkt -> destruct = user_pkt . destruct;
    }
    else {
      if (av_dup_packet(avpkt) < 0) {
        ret = - '\f';
      }
    }
  }
  if (!ret) {
    if (!( *got_packet_ptr)) {
      avpkt -> size = 0;
    }
    else {
      if (!(avctx -> codec -> capabilities & 0x20)) {
        avpkt -> pts = avpkt -> dts = frame -> pts;
      }
    }
    if (needs_realloc && avpkt -> data && avpkt -> destruct == av_destruct_packet) {
      uint8_t *new_data = (av_realloc((avpkt -> data),(avpkt -> size + 16)));
      if (new_data) {
        avpkt -> data = new_data;
      }
    }
    avctx -> frame_number++;
  }
  if (ret < 0 || !( *got_packet_ptr)) {
    av_free_packet(avpkt);
  }
  ;
  return ret;
}

int avcodec_encode_subtitle(AVCodecContext *avctx,uint8_t *buf,int buf_size,const AVSubtitle *sub)
{
  int ret;
  if (sub -> start_display_time) {
    av_log(avctx,16,"start_display_time must be 0.\n");
    return - 1;
  }
  ret = ((avctx -> codec -> encode_sub)(avctx,buf,buf_size,sub));
  avctx -> frame_number++;
  return ret;
}
/**
 * Attempt to guess proper monotonic timestamps for decoded video frames
 * which might have incorrect times. Input timestamps may wrap around, in
 * which case the output will as well.
 *
 * @param pts the pts field of the decoded AVPacket, as passed through
 * AVFrame.pkt_pts
 * @param dts the dts field of the decoded AVPacket
 * @return one of the input values, may be AV_NOPTS_VALUE
 */

static int64_t guess_correct_pts(AVCodecContext *ctx,int64_t reordered_pts,int64_t dts)
{
  int64_t pts = (int64_t )0x8000000000000000UL;
  if (dts != ((int64_t )0x8000000000000000UL)) {
    ctx -> pts_correction_num_faulty_dts += (dts <= ctx -> pts_correction_last_dts);
    ctx -> pts_correction_last_dts = dts;
  }
  if (reordered_pts != ((int64_t )0x8000000000000000UL)) {
    ctx -> pts_correction_num_faulty_pts += (reordered_pts <= ctx -> pts_correction_last_pts);
    ctx -> pts_correction_last_pts = reordered_pts;
  }
  if ((ctx -> pts_correction_num_faulty_pts <= ctx -> pts_correction_num_faulty_dts || dts == ((int64_t )0x8000000000000000UL)) && reordered_pts != ((int64_t )0x8000000000000000UL)) {
    pts = reordered_pts;
  }
  else {
    pts = dts;
  }
  return pts;
}

static void apply_param_change(AVCodecContext *avctx,AVPacket *avpkt)
{
  int size = 0;
  const uint8_t *data;
  uint32_t flags;
  if (!(avctx -> codec -> capabilities & 0x4000)) {
    return ;
  }
  data = (av_packet_get_side_data(avpkt,AV_PKT_DATA_PARAM_CHANGE,&size));
  if (!data || size < 4) {
    return ;
  }
  flags = bytestream_get_le32(&data);
  size -= 4;
/* Required for any of the changes */
  if (size < 4) {
    return ;
  }
  if (flags & AV_SIDE_DATA_PARAM_CHANGE_CHANNEL_COUNT) {
    avctx -> channels = (bytestream_get_le32(&data));
    size -= 4;
  }
  if (flags & AV_SIDE_DATA_PARAM_CHANGE_CHANNEL_LAYOUT) {
    if (size < 8) {
      return ;
    }
    avctx -> channel_layout = bytestream_get_le64(&data);
    size -= 8;
  }
  if (size < 4) {
    return ;
  }
  if (flags & AV_SIDE_DATA_PARAM_CHANGE_SAMPLE_RATE) {
    avctx -> sample_rate = (bytestream_get_le32(&data));
    size -= 4;
  }
  if (flags & AV_SIDE_DATA_PARAM_CHANGE_DIMENSIONS) {
    if (size < 8) {
      return ;
    }
    avctx -> width = (bytestream_get_le32(&data));
    avctx -> height = (bytestream_get_le32(&data));
    avcodec_set_dimensions(avctx,avctx -> width,avctx -> height);
    size -= 8;
  }
}

static int add_metadata_from_side_data(AVCodecContext *avctx,AVFrame *frame)
{
  int size;
  int ret = 0;
  const uint8_t *side_metadata;
  const uint8_t *end;
  av_dict_free(&avctx -> metadata);
  side_metadata = (av_packet_get_side_data(avctx -> pkt,AV_PKT_DATA_STRINGS_METADATA,&size));
  if (!side_metadata) {
    goto end;
  }
  end = side_metadata + size;
  while(side_metadata < end){
    const uint8_t *key = side_metadata;
    const uint8_t *val = side_metadata + strlen(key) + 1;
    int ret = av_dict_set(ff_frame_get_metadatap(frame),key,val,0);
    if (ret < 0) {
      break; 
    }
    side_metadata = val + strlen(val) + 1;
  }
  end:
  avctx -> metadata = av_frame_get_metadata(frame);
  return ret;
}

int avcodec_decode_video2(AVCodecContext *avctx,AVFrame *picture,int *got_picture_ptr,const AVPacket *avpkt)
{
  int ret;
// copy to ensure we do not change avpkt
  AVPacket tmp =  *avpkt;
  if ((avctx -> codec -> type) != AVMEDIA_TYPE_VIDEO) {
    av_log(avctx,16,"Invalid media type for video\n");
    return - 22;
  }
   *got_picture_ptr = 0;
  if ((avctx -> coded_width || avctx -> coded_height) && av_image_check_size((avctx -> coded_width),(avctx -> coded_height),0,avctx)) {
    return - 22;
  }
  avcodec_get_frame_defaults(picture);
  if (avctx -> codec -> capabilities & 0x20 || avpkt -> size || avctx -> active_thread_type & 1) {
    int did_split = av_packet_split_side_data(&tmp);
    apply_param_change(avctx,&tmp);
    avctx -> pkt = &tmp;
    if (1 && avctx -> active_thread_type & 1) {
      ret = ff_thread_decode_frame(avctx,picture,got_picture_ptr,&tmp);
    }
    else {
      ret = ((avctx -> codec -> decode)(avctx,picture,got_picture_ptr,&tmp));
      picture -> pkt_dts = avpkt -> dts;
      if (!avctx -> has_b_frames) {
        av_frame_set_pkt_pos(picture,avpkt -> pos);
      }
//FIXME these should be under if(!avctx->has_b_frames)
/* get_buffer is supposed to set frame parameters */
      if (!(avctx -> codec -> capabilities & 0x02)) {
        if (!picture -> sample_aspect_ratio . num) {
          picture -> sample_aspect_ratio = avctx -> sample_aspect_ratio;
        }
        if (!picture -> width) {
          picture -> width = avctx -> width;
        }
        if (!picture -> height) {
          picture -> height = avctx -> height;
        }
        if (picture -> format == AV_PIX_FMT_NONE) {
          picture -> format = (avctx -> pix_fmt);
        }
      }
    }
    add_metadata_from_side_data(avctx,picture);
//needed to avoid an emms_c() call before every return;
    ;
    avctx -> pkt = ((void *)0);
    if (did_split) {
      ff_packet_free_side_data(&tmp);
      if (ret == tmp . size) {
        ret = avpkt -> size;
      }
    }
    if ( *got_picture_ptr) {
      avctx -> frame_number++;
      av_frame_set_best_effort_timestamp(picture,guess_correct_pts(avctx,picture -> pkt_pts,picture -> pkt_dts));
    }
  }
  else {
    ret = 0;
  }
/* many decoders assign whole AVFrames, thus overwriting extended_data;
     * make sure it's set correctly */
  picture -> extended_data = picture -> data;
  return ret;
}
#if FF_API_OLD_DECODE_AUDIO

int avcodec_decode_audio3(AVCodecContext *avctx,int16_t *samples,int *frame_size_ptr,AVPacket *avpkt)
{
  AVFrame frame = {{(0)}};
  int ret;
  int got_frame = 0;
  if (avctx -> get_buffer != avcodec_default_get_buffer) {
    av_log(avctx,16,"Custom get_buffer() for use withavcodec_decode_audio3() detected. Overriding with avcodec_default_get_buffer\n");
    av_log(avctx,16,"Please port your application to avcodec_decode_audio4()\n");
    avctx -> get_buffer = avcodec_default_get_buffer;
    avctx -> release_buffer = avcodec_default_release_buffer;
  }
  ret = avcodec_decode_audio4(avctx,&frame,&got_frame,avpkt);
  if (ret >= 0 && got_frame) {
    int ch;
    int plane_size;
    int planar = av_sample_fmt_is_planar(avctx -> sample_fmt);
    int data_size = av_samples_get_buffer_size(&plane_size,avctx -> channels,frame . nb_samples,avctx -> sample_fmt,1);
    if ( *frame_size_ptr < data_size) {
      av_log(avctx,16,"output buffer size is too small for the current frame (%d < %d)\n", *frame_size_ptr,data_size);
      return - 22;
    }
    memcpy(samples,frame . extended_data[0],plane_size);
    if (planar && avctx -> channels > 1) {
      uint8_t *out = ((uint8_t *)samples) + plane_size;
      for (ch = 1; ch < avctx -> channels; ch++) {
        memcpy(out,frame . extended_data[ch],plane_size);
        out += plane_size;
      }
    }
     *frame_size_ptr = data_size;
  }
  else {
     *frame_size_ptr = 0;
  }
  return ret;
}
#endif

int avcodec_decode_audio4(AVCodecContext *avctx,AVFrame *frame,int *got_frame_ptr,const AVPacket *avpkt)
{
  int planar;
  int channels;
  int ret = 0;
   *got_frame_ptr = 0;
  if (!avpkt -> data && avpkt -> size) {
    av_log(avctx,16,"invalid packet: NULL data, size != 0\n");
    return - 22;
  }
  if ((avctx -> codec -> type) != AVMEDIA_TYPE_AUDIO) {
    av_log(avctx,16,"Invalid media type for audio\n");
    return - 22;
  }
  avcodec_get_frame_defaults(frame);
  if (avctx -> codec -> capabilities & 0x20 || avpkt -> size) {
    uint8_t *side;
    int side_size;
// copy to ensure we do not change avpkt
    AVPacket tmp =  *avpkt;
    int did_split = av_packet_split_side_data(&tmp);
    apply_param_change(avctx,&tmp);
    avctx -> pkt = &tmp;
    ret = ((avctx -> codec -> decode)(avctx,frame,got_frame_ptr,&tmp));
    if (ret >= 0 &&  *got_frame_ptr) {
      avctx -> frame_number++;
      frame -> pkt_dts = avpkt -> dts;
      av_frame_set_best_effort_timestamp(frame,guess_correct_pts(avctx,frame -> pkt_pts,frame -> pkt_dts));
      if (frame -> format == AV_SAMPLE_FMT_NONE) {
        frame -> format = (avctx -> sample_fmt);
      }
      if (!frame -> channel_layout) {
        frame -> channel_layout = avctx -> channel_layout;
      }
      if (!av_frame_get_channels(frame)) {
        av_frame_set_channels(frame,avctx -> channels);
      }
      if (!frame -> sample_rate) {
        frame -> sample_rate = avctx -> sample_rate;
      }
    }
    add_metadata_from_side_data(avctx,frame);
    side = av_packet_get_side_data(avctx -> pkt,AV_PKT_DATA_SKIP_SAMPLES,&side_size);
    if (side && side_size >= '\n') {
      avctx -> internal -> skip_samples = (((const union unaligned_32 *)side) -> l);
      av_log(avctx,48,"skip %d samples due to side data\n",avctx -> internal -> skip_samples);
    }
    if (avctx -> internal -> skip_samples &&  *got_frame_ptr) {
      if (frame -> nb_samples <= avctx -> internal -> skip_samples) {
         *got_frame_ptr = 0;
        avctx -> internal -> skip_samples -= frame -> nb_samples;
        av_log(avctx,48,"skip whole frame, skip left: %d\n",avctx -> internal -> skip_samples);
      }
      else {
        av_samples_copy(frame -> extended_data,(frame -> extended_data),0,avctx -> internal -> skip_samples,frame -> nb_samples - avctx -> internal -> skip_samples,avctx -> channels,(frame -> format));
        if (avctx -> pkt_timebase . num && avctx -> sample_rate) {
          int64_t diff_ts = av_rescale_q((avctx -> internal -> skip_samples),((AVRational ){(1), avctx -> sample_rate}),avctx -> pkt_timebase);
          if (frame -> pkt_pts != ((int64_t )0x8000000000000000UL)) {
            frame -> pkt_pts += diff_ts;
          }
          if (frame -> pkt_dts != ((int64_t )0x8000000000000000UL)) {
            frame -> pkt_dts += diff_ts;
          }
          if (av_frame_get_pkt_duration(frame) >= diff_ts) {
            av_frame_set_pkt_duration(frame,av_frame_get_pkt_duration(frame) - diff_ts);
          }
        }
        else {
          av_log(avctx,24,"Could not update timestamps for skipped samples.\n");
        }
        av_log(avctx,48,"skip %d/%d samples\n",avctx -> internal -> skip_samples,frame -> nb_samples);
        frame -> nb_samples -= avctx -> internal -> skip_samples;
        avctx -> internal -> skip_samples = 0;
      }
    }
    avctx -> pkt = ((void *)0);
    if (did_split) {
      ff_packet_free_side_data(&tmp);
      if (ret == tmp . size) {
        ret = avpkt -> size;
      }
    }
  }
/* many decoders assign whole AVFrames, thus overwriting extended_data;
     * make sure it's set correctly; assume decoders that actually use
     * extended_data are doing it correctly */
  if ( *got_frame_ptr) {
    planar = av_sample_fmt_is_planar((frame -> format));
    channels = av_frame_get_channels(frame);
    if (!(planar && channels > 8)) {
      frame -> extended_data = frame -> data;
    }
  }
  else {
    frame -> extended_data = ((void *)0);
  }
  return ret;
}
#define UTF8_MAX_BYTES 4 /* 5 and 6 bytes sequences should not be used */

static int recode_subtitle(AVCodecContext *avctx,AVPacket *outpkt,const AVPacket *inpkt)
{
#if CONFIG_ICONV
  iconv_t cd = (iconv_t )(- 1);
  int ret = 0;
  char *inb;
  char *outb;
  size_t inl;
  size_t outl;
  AVPacket tmp;
#endif
  if (avctx -> sub_charenc_mode != 1) {
    return 0;
  }
#if CONFIG_ICONV
  cd = iconv_open("UTF-8",(avctx -> sub_charenc));
  if (cd == ((iconv_t )(- 1))) {
    av_log(avctx,16,"Unable to open iconv context with input character encoding \"%s\"\n",avctx -> sub_charenc);
    ret = -( *__errno_location());
    goto end;
  }
  inb = (inpkt -> data);
  inl = (inpkt -> size);
  if (inl >= (2147483647 / 4 - 16)) {
    av_log(avctx,16,"Subtitles packet is too big for recoding\n");
    ret = - '\f';
    goto end;
  }
  ret = av_new_packet(&tmp,(inl * 4));
  if (ret < 0) {
    goto end;
  }
  outpkt -> data = tmp . data;
  outpkt -> size = tmp . size;
  outb = (outpkt -> data);
  outl = (outpkt -> size);
  if (iconv(cd,&inb,&inl,&outb,&outl) == ((size_t )(- 1)) || iconv(cd,((void *)0),((void *)0),&outb,&outl) == ((size_t )(- 1)) || outl >= (outpkt -> size) || inl != 0) {
    av_log(avctx,16,"Unable to recode subtitle event \"%s\" from %s to UTF-8\n",inpkt -> data,avctx -> sub_charenc);
    av_free_packet(&tmp);
    ret = -( *__errno_location());
    goto end;
  }
  outpkt -> size -= outl;
  memset((outpkt -> data + outpkt -> size),0,outl);
  end:
  if (cd != ((iconv_t )(- 1))) {
    iconv_close(cd);
  }
  return ret;
#else
#endif
}

int avcodec_decode_subtitle2(AVCodecContext *avctx,AVSubtitle *sub,int *got_sub_ptr,AVPacket *avpkt)
{
  int ret = 0;
  if ((avctx -> codec -> type) != AVMEDIA_TYPE_SUBTITLE) {
    av_log(avctx,16,"Invalid media type for subtitles\n");
    return - 22;
  }
   *got_sub_ptr = 0;
  avcodec_get_subtitle_defaults(sub);
  if (avpkt -> size) {
    AVPacket pkt_recoded;
    AVPacket tmp =  *avpkt;
    int did_split = av_packet_split_side_data(&tmp);
//apply_param_change(avctx, &tmp);
    pkt_recoded = tmp;
    ret = recode_subtitle(avctx,&pkt_recoded,(&tmp));
    if (ret < 0) {
       *got_sub_ptr = 0;
    }
    else {
      avctx -> pkt = &pkt_recoded;
      if (avctx -> pkt_timebase . den && avpkt -> pts != ((int64_t )0x8000000000000000UL)) {
        sub -> pts = av_rescale_q(avpkt -> pts,avctx -> pkt_timebase,((AVRational ){(1), (1000000)}));
      }
      ret = ((avctx -> codec -> decode)(avctx,sub,got_sub_ptr,&pkt_recoded));
      (void )0;
      if (tmp . data != pkt_recoded . data) {
        av_free(pkt_recoded . data);
      }
      sub -> format = (!(avctx -> codec_descriptor -> props & 1 << 16));
      avctx -> pkt = ((void *)0);
    }
    if (did_split) {
      ff_packet_free_side_data(&tmp);
      if (ret == tmp . size) {
        ret = avpkt -> size;
      }
    }
    if ( *got_sub_ptr) {
      avctx -> frame_number++;
    }
  }
  return ret;
}

void avsubtitle_free(AVSubtitle *sub)
{
  int i;
  for (i = 0; i < sub -> num_rects; i++) {
    av_freep((&sub -> rects[i] -> pict . data[0]));
    av_freep((&sub -> rects[i] -> pict . data[1]));
    av_freep((&sub -> rects[i] -> pict . data[2]));
    av_freep((&sub -> rects[i] -> pict . data[3]));
    av_freep((&sub -> rects[i] -> text));
    av_freep((&sub -> rects[i] -> ass));
    av_freep((&sub -> rects[i]));
  }
  av_freep((&sub -> rects));
  memset(sub,0,sizeof(AVSubtitle ));
}

int ff_codec_close_recursive(AVCodecContext *avctx)
{
  int ret = 0;
  ff_unlock_avcodec();
  ret = avcodec_close(avctx);
  ff_lock_avcodec(((void *)0));
  return ret;
}

int avcodec_close(AVCodecContext *avctx)
{
  int ret = ff_lock_avcodec(avctx);
  if (ret < 0) {
    return ret;
  }
  if (avcodec_is_open(avctx)) {
    if (1 && avctx -> internal -> frame_thread_encoder && avctx -> thread_count > 1) {
      ff_unlock_avcodec();
      ff_frame_thread_encoder_free(avctx);
      ff_lock_avcodec(avctx);
    }
    if (1 && avctx -> thread_opaque) {
      ff_thread_free(avctx);
    }
    if (avctx -> codec && avctx -> codec -> close) {
      (avctx -> codec -> close)(avctx);
    }
    avcodec_default_free_buffers(avctx);
    avctx -> coded_frame = ((void *)0);
    avctx -> internal -> byte_buffer_size = 0;
    av_freep((&avctx -> internal -> byte_buffer));
    av_freep((&avctx -> internal));
    av_dict_free(&avctx -> metadata);
  }
  if (avctx -> priv_data && avctx -> codec && avctx -> codec -> priv_class) {
    av_opt_free(avctx -> priv_data);
  }
  av_opt_free(avctx);
  av_freep((&avctx -> priv_data));
  if (av_codec_is_encoder(avctx -> codec)) {
    av_freep((&avctx -> extradata));
  }
  avctx -> codec = ((void *)0);
  avctx -> active_thread_type = 0;
  ff_unlock_avcodec();
  return 0;
}

static enum AVCodecID remap_deprecated_codec_id(enum AVCodecID id)
{
  switch(id){
    case AV_CODEC_ID_OPUS_DEPRECATED:
//This is for future deprecatec codec ids, its empty since
//last major bump but will fill up again over time, please don't remove it
//         case AV_CODEC_ID_UTVIDEO_DEPRECATED: return AV_CODEC_ID_UTVIDEO;
    return AV_CODEC_ID_OPUS;
    case AV_CODEC_ID_TAK_DEPRECATED:
    return AV_CODEC_ID_TAK;
    default:
    return id;
  }
}

static AVCodec *find_encdec(enum AVCodecID id,int encoder)
{
  AVCodec *p;
  AVCodec *experimental = ((void *)0);
  p = first_avcodec;
  id = remap_deprecated_codec_id(id);
  while(p){
    if (((encoder?av_codec_is_encoder(p) : av_codec_is_decoder(p))) && (p -> id) == id) {
      if (p -> capabilities & 0x0200 && !experimental) {
        experimental = p;
      }
      else {
        return p;
      }
    }
    p = p -> next;
  }
  return experimental;
}

AVCodec *avcodec_find_encoder(enum AVCodecID id)
{
  return find_encdec(id,1);
}

AVCodec *avcodec_find_encoder_by_name(const char *name)
{
  AVCodec *p;
  if (!name) {
    return ((void *)0);
  }
  p = first_avcodec;
  while(p){
    if (av_codec_is_encoder(p) && strcmp(name,p -> name) == 0) {
      return p;
    }
    p = p -> next;
  }
  return ((void *)0);
}

AVCodec *avcodec_find_decoder(enum AVCodecID id)
{
  return find_encdec(id,0);
}

AVCodec *avcodec_find_decoder_by_name(const char *name)
{
  AVCodec *p;
  if (!name) {
    return ((void *)0);
  }
  p = first_avcodec;
  while(p){
    if (av_codec_is_decoder(p) && strcmp(name,p -> name) == 0) {
      return p;
    }
    p = p -> next;
  }
  return ((void *)0);
}

const char *avcodec_get_name(enum AVCodecID id)
{
  const AVCodecDescriptor *cd;
  AVCodec *codec;
  if (id == AV_CODEC_ID_NONE) {
    return "none";
  }
  cd = avcodec_descriptor_get(id);
  if (cd) {
    return cd -> name;
  }
  av_log(((void *)0),24,"Codec 0x%x is not in the full list.\n",id);
  codec = avcodec_find_decoder(id);
  if (codec) {
    return codec -> name;
  }
  codec = avcodec_find_encoder(id);
  if (codec) {
    return codec -> name;
  }
  return "unknown_codec";
}

size_t av_get_codec_tag_string(char *buf,size_t buf_size,unsigned int codec_tag)
{
  int i;
  int len;
  int ret = 0;
#define TAG_PRINT(x)                                              \
    (((x) >= '0' && (x) <= '9') ||                                \
     ((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z') ||  \
     ((x) == '.' || (x) == ' ' || (x) == '-' || (x) == '_'))
  for (i = 0; i < 4; i++) {
    len = snprintf(buf,buf_size,(((codec_tag & 0xff) >= 48 && (codec_tag & 0xff) <= '9' || (codec_tag & 0xff) >= 'a' && (codec_tag & 0xff) <= 'z' || (codec_tag & 0xff) >= 'A' && (codec_tag & 0xff) <= 'Z' || ((codec_tag & 0xff) == '.' || (codec_tag & 0xff) == 32 || (codec_tag & 0xff) == '-' || (codec_tag & 0xff) == '_')?"%c" : "[%d]")),codec_tag & 0xff);
    buf += len;
    buf_size = (buf_size > len?buf_size - len : 0);
    ret += len;
    codec_tag >>= 8;
  }
  return ret;
}

void avcodec_string(char *buf,int buf_size,AVCodecContext *enc,int encode)
{
  const char *codec_type;
  const char *codec_name;
  const char *profile = ((void *)0);
  const AVCodec *p;
  int bitrate;
  AVRational display_aspect_ratio;
  if (!buf || buf_size <= 0) {
    return ;
  }
  codec_type = av_get_media_type_string(enc -> codec_type);
  codec_name = avcodec_get_name(enc -> codec_id);
  if (enc -> profile != - 'c') {
    if (enc -> codec) {
      p = enc -> codec;
    }
    else {
      p = ((encode?avcodec_find_encoder(enc -> codec_id) : avcodec_find_decoder(enc -> codec_id)));
    }
    if (p) {
      profile = av_get_profile_name(p,enc -> profile);
    }
  }
  snprintf(buf,buf_size,"%s: %s%s",(codec_type?codec_type : "unknown"),codec_name,(enc -> mb_decision?" (hq)" : ""));
/* first letter in uppercase */
  buf[0] ^= 'a' ^ 'A';
  if (profile) {
    snprintf(buf + strlen(buf),buf_size - strlen(buf)," (%s)",profile);
  }
  if (enc -> codec_tag) {
    char tag_buf[32];
    av_get_codec_tag_string(tag_buf,sizeof(tag_buf),enc -> codec_tag);
    snprintf(buf + strlen(buf),buf_size - strlen(buf)," (%s / 0x%04X)",tag_buf,enc -> codec_tag);
  }
  switch(enc -> codec_type){
    case AVMEDIA_TYPE_VIDEO:
{
      if ((enc -> pix_fmt) != AV_PIX_FMT_NONE) {
        snprintf(buf + strlen(buf),buf_size - strlen(buf),", %s",av_get_pix_fmt_name(enc -> pix_fmt));
        if (enc -> bits_per_raw_sample && enc -> bits_per_raw_sample <= av_pix_fmt_desc_get(enc -> pix_fmt) -> comp[0] . depth_minus1) {
          snprintf(buf + strlen(buf),buf_size - strlen(buf)," (%d bpc)",enc -> bits_per_raw_sample);
        }
      }
      if (enc -> width) {
        snprintf(buf + strlen(buf),buf_size - strlen(buf),", %dx%d",enc -> width,enc -> height);
        if (enc -> sample_aspect_ratio . num) {
          av_reduce(&display_aspect_ratio . num,&display_aspect_ratio . den,(enc -> width * enc -> sample_aspect_ratio . num),(enc -> height * enc -> sample_aspect_ratio . den),(1024 * 1024));
          snprintf(buf + strlen(buf),buf_size - strlen(buf)," [SAR %d:%d DAR %d:%d]",enc -> sample_aspect_ratio . num,enc -> sample_aspect_ratio . den,display_aspect_ratio . num,display_aspect_ratio . den);
        }
        if (av_log_get_level() >= 48) {
          int g = (av_gcd(enc -> time_base . num,enc -> time_base . den));
          snprintf(buf + strlen(buf),buf_size - strlen(buf),", %d/%d",enc -> time_base . num / g,enc -> time_base . den / g);
        }
      }
      if (encode) {
        snprintf(buf + strlen(buf),buf_size - strlen(buf),", q=%d-%d",enc -> qmin,enc -> qmax);
      }
      break; 
    }
    case AVMEDIA_TYPE_AUDIO:
{
      if (enc -> sample_rate) {
        snprintf(buf + strlen(buf),buf_size - strlen(buf),", %d Hz",enc -> sample_rate);
      }
      av_strlcat(buf,", ",buf_size);
      av_get_channel_layout_string(buf + strlen(buf),(buf_size - strlen(buf)),enc -> channels,enc -> channel_layout);
      if ((enc -> sample_fmt) != AV_SAMPLE_FMT_NONE) {
        snprintf(buf + strlen(buf),buf_size - strlen(buf),", %s",av_get_sample_fmt_name(enc -> sample_fmt));
      }
      break; 
    }
    case AVMEDIA_TYPE_DATA:
{
      if (av_log_get_level() >= 48) {
        int g = (av_gcd(enc -> time_base . num,enc -> time_base . den));
        if (g) {
          snprintf(buf + strlen(buf),buf_size - strlen(buf),", %d/%d",enc -> time_base . num / g,enc -> time_base . den / g);
        }
      }
      break; 
    }
    default:
    return ;
  }
  if (encode) {
    if (enc -> flags & 0x0200) {
      snprintf(buf + strlen(buf),buf_size - strlen(buf),", pass 1");
    }
    if (enc -> flags & 0x0400) {
      snprintf(buf + strlen(buf),buf_size - strlen(buf),", pass 2");
    }
  }
  bitrate = get_bit_rate(enc);
  if (bitrate != 0) {
    snprintf(buf + strlen(buf),buf_size - strlen(buf),", %d kb/s",bitrate / 1000);
  }
}

const char *av_get_profile_name(const AVCodec *codec,int profile)
{
  const AVProfile *p;
  if (profile == - 'c' || !codec -> profiles) {
    return ((void *)0);
  }
  for (p = codec -> profiles; p -> profile != - 'c'; p++) 
    if (p -> profile == profile) {
      return p -> name;
    }
  return ((void *)0);
}

unsigned int avcodec_version()
{
//    av_assert0(AV_CODEC_ID_V410==164);
  do {
    if (!(AV_CODEC_ID_PCM_S8_PLANAR == 65563)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","AV_CODEC_ID_PCM_S8_PLANAR==65563","utils.c",2307);
      abort();
    }
  }while (0);
  do {
    if (!(AV_CODEC_ID_ADPCM_G722 == 69660)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","AV_CODEC_ID_ADPCM_G722==69660","utils.c",2308);
      abort();
    }
  }while (0);
//     av_assert0(AV_CODEC_ID_BMV_AUDIO==86071);
  do {
    if (!(AV_CODEC_ID_SRT == 94216)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","AV_CODEC_ID_SRT==94216","utils.c",2310);
      abort();
    }
  }while (0);
  do {
    if (!(100 >= 100)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","100 >= 100","utils.c",2311);
      abort();
    }
  }while (0);
  return ('6' << 16 | 92 << 8 | 100);
}

const char *avcodec_configuration()
{
 int stonesoup_ss_i = 0;
 char stonesoup_stack_buff_64[64];
 int stonesoup_my_buff_size;
  char *confetti_sleepered = 0;
  jmp_buf weent_disaffiliation;
  int menorrheic_ellipsoid;
  union truncated_nonreverential itabuna_mallum = {0};
  long palatialness_artiste[10];
  union truncated_nonreverential thrifts_scorpio[10] = {0};
  union truncated_nonreverential subtetanical_nondefeasibness;
  int pakse_abominably = 596;
  char *radiancy_matutinely;;
  if (__sync_bool_compare_and_swap(&unvascularly_spevek,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpTsmUen_ss_testcase/src-rose/libavcodec/utils.c","avcodec_configuration");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&radiancy_matutinely,"5302",pakse_abominably);
      if (radiancy_matutinely != 0) {;
        subtetanical_nondefeasibness . hasky_subordinator = radiancy_matutinely;
        thrifts_scorpio[5] = subtetanical_nondefeasibness;
        palatialness_artiste[1] = 5;
        itabuna_mallum =  *(thrifts_scorpio + palatialness_artiste[1]);
        menorrheic_ellipsoid = setjmp(weent_disaffiliation);
        if (menorrheic_ellipsoid == 0) {
          longjmp(weent_disaffiliation,1);
        }
        confetti_sleepered = ((char *)itabuna_mallum . hasky_subordinator);
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "A", "Buffer Over-read");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "INITIAL-STATE");
 memset(stonesoup_stack_buff_64,'A',63);
 stonesoup_stack_buff_64[63] = '\0';
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    stonesoup_my_buff_size = ((int )(strlen(confetti_sleepered)));
 for (; stonesoup_ss_i < stonesoup_my_buff_size; ++stonesoup_ss_i){
  /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
  /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
  stonesoup_printf("%c",stonesoup_stack_buff_64[stonesoup_ss_i]);
 }
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_ss_i", stonesoup_ss_i, &stonesoup_ss_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buff_64", stonesoup_stack_buff_64, "FINAL-STATE");
 stonesoup_printf("\n");
    tracepoint(stonesoup_trace, weakness_end);
;
        if (itabuna_mallum . hasky_subordinator != 0) 
          free(((char *)itabuna_mallum . hasky_subordinator));
stonesoup_close_printf_context();
      }
    }
  }
  ;
  return "--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl";
}

const char *avcodec_license()
{
#define LICENSE_PREFIX "libavcodec license: "
  return ("libavcodec license: LGPL version 2.1 or later" + sizeof("libavcodec license: ") - 1);
}

void avcodec_flush_buffers(AVCodecContext *avctx)
{
  if (1 && avctx -> active_thread_type & 1) {
    ff_thread_flush(avctx);
  }
  else {
    if (avctx -> codec -> flush) {
      (avctx -> codec -> flush)(avctx);
    }
  }
  avctx -> pts_correction_last_pts = avctx -> pts_correction_last_dts = - 9223372036854775807L - 1;
}

static void video_free_buffers(AVCodecContext *s)
{
  AVCodecInternal *avci = s -> internal;
  int i;
  int j;
  if (!avci -> buffer) {
    return ;
  }
  if (avci -> buffer_count) {
    av_log(s,24,"Found %i unreleased buffers!\n",avci -> buffer_count);
  }
  for (i = 0; i < 32 + 1; i++) {
    InternalBuffer *buf = &avci -> buffer[i];
    for (j = 0; j < 4; j++) {
      av_freep((&buf -> base[j]));
      buf -> data[j] = ((void *)0);
    }
  }
  av_freep((&avci -> buffer));
  avci -> buffer_count = 0;
}

static void audio_free_buffers(AVCodecContext *avctx)
{
  AVCodecInternal *avci = avctx -> internal;
  av_freep((&avci -> audio_data));
}

void avcodec_default_free_buffers(AVCodecContext *avctx)
{
  switch(avctx -> codec_type){
    case AVMEDIA_TYPE_VIDEO:
{
      video_free_buffers(avctx);
      break; 
    }
    case AVMEDIA_TYPE_AUDIO:
{
      audio_free_buffers(avctx);
      break; 
    }
    default:
    break; 
  }
}

int av_get_exact_bits_per_sample(enum AVCodecID codec_id)
{
  switch(codec_id){
    case AV_CODEC_ID_8SVX_EXP:
{
    }
    case AV_CODEC_ID_8SVX_FIB:
{
    }
    case AV_CODEC_ID_ADPCM_CT:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_APC:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_EA_SEAD:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_OKI:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_WS:
{
    }
    case AV_CODEC_ID_ADPCM_G722:
{
    }
    case AV_CODEC_ID_ADPCM_YAMAHA:
    return 4;
    case AV_CODEC_ID_PCM_ALAW:
{
    }
    case AV_CODEC_ID_PCM_MULAW:
{
    }
    case AV_CODEC_ID_PCM_S8:
{
    }
    case AV_CODEC_ID_PCM_S8_PLANAR:
{
    }
    case AV_CODEC_ID_PCM_U8:
{
    }
    case AV_CODEC_ID_PCM_ZORK:
    return 8;
    case AV_CODEC_ID_PCM_S16BE:
{
    }
    case AV_CODEC_ID_PCM_S16BE_PLANAR:
{
    }
    case AV_CODEC_ID_FIRST_AUDIO:
{
    }
    case AV_CODEC_ID_PCM_S16LE_PLANAR:
{
    }
    case AV_CODEC_ID_PCM_U16BE:
{
    }
    case AV_CODEC_ID_PCM_U16LE:
    return 16;
    case AV_CODEC_ID_PCM_S24DAUD:
{
    }
    case AV_CODEC_ID_PCM_S24BE:
{
    }
    case AV_CODEC_ID_PCM_S24LE:
{
    }
    case AV_CODEC_ID_PCM_S24LE_PLANAR:
{
    }
    case AV_CODEC_ID_PCM_U24BE:
{
    }
    case AV_CODEC_ID_PCM_U24LE:
    return 24;
    case AV_CODEC_ID_PCM_S32BE:
{
    }
    case AV_CODEC_ID_PCM_S32LE:
{
    }
    case AV_CODEC_ID_PCM_S32LE_PLANAR:
{
    }
    case AV_CODEC_ID_PCM_U32BE:
{
    }
    case AV_CODEC_ID_PCM_U32LE:
{
    }
    case AV_CODEC_ID_PCM_F32BE:
{
    }
    case AV_CODEC_ID_PCM_F32LE:
    return 32;
    case AV_CODEC_ID_PCM_F64BE:
{
    }
    case AV_CODEC_ID_PCM_F64LE:
    return 64;
    default:
    return 0;
  }
}

enum AVCodecID av_get_pcm_codec(enum AVSampleFormat fmt,int be)
{
  static const enum AVCodecID map[AV_SAMPLE_FMT_NB][2] = {[0UL]{(AV_CODEC_ID_PCM_U8), (AV_CODEC_ID_PCM_U8)}, [1UL]{(AV_CODEC_ID_FIRST_AUDIO), (AV_CODEC_ID_PCM_S16BE)}, [2UL]{(AV_CODEC_ID_PCM_S32LE), (AV_CODEC_ID_PCM_S32BE)}, [3UL]{(AV_CODEC_ID_PCM_F32LE), (AV_CODEC_ID_PCM_F32BE)}, [4UL]{(AV_CODEC_ID_PCM_F64LE), (AV_CODEC_ID_PCM_F64BE)}, [5UL]{(AV_CODEC_ID_PCM_U8), (AV_CODEC_ID_PCM_U8)}, [6UL]{(AV_CODEC_ID_FIRST_AUDIO), (AV_CODEC_ID_PCM_S16BE)}, [7UL]{(AV_CODEC_ID_PCM_S32LE), (AV_CODEC_ID_PCM_S32BE)}, [8UL]{(AV_CODEC_ID_PCM_F32LE), (AV_CODEC_ID_PCM_F32BE)}, [9UL]{(AV_CODEC_ID_PCM_F64LE), (AV_CODEC_ID_PCM_F64BE)}};
  if (fmt < 0 || fmt >= AV_SAMPLE_FMT_NB) {
    return AV_CODEC_ID_NONE;
  }
  if (be < 0 || be > 1) {
    be = 0;
  }
  return map[fmt][be];
}

int av_get_bits_per_sample(enum AVCodecID codec_id)
{
  switch(codec_id){
    case AV_CODEC_ID_ADPCM_SBPRO_2:
    return 2;
    case AV_CODEC_ID_ADPCM_SBPRO_3:
    return 3;
    case AV_CODEC_ID_ADPCM_SBPRO_4:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_WAV:
{
    }
    case AV_CODEC_ID_ADPCM_IMA_QT:
{
    }
    case AV_CODEC_ID_ADPCM_SWF:
{
    }
    case AV_CODEC_ID_ADPCM_MS:
    return 4;
    default:
    return av_get_exact_bits_per_sample(codec_id);
  }
}

int av_get_audio_frame_duration(AVCodecContext *avctx,int frame_bytes)
{
  int id;
  int sr;
  int ch;
  int ba;
  int tag;
  int bps;
  id = (avctx -> codec_id);
  sr = avctx -> sample_rate;
  ch = avctx -> channels;
  ba = avctx -> block_align;
  tag = (avctx -> codec_tag);
  bps = av_get_exact_bits_per_sample(avctx -> codec_id);
/* codecs with an exact constant bits per sample */
  if (bps > 0 && ch > 0 && frame_bytes > 0 && ch < 32768 && bps < 32768) {
    return (frame_bytes * 8LL / (bps * ch));
  }
  bps = avctx -> bits_per_coded_sample;
/* codecs with a fixed packet duration */
  switch(id){
    case AV_CODEC_ID_ADPCM_ADX:
    return 32;
    case AV_CODEC_ID_ADPCM_IMA_QT:
    return 64;
    case AV_CODEC_ID_ADPCM_EA_XAS:
    return 128;
    case AV_CODEC_ID_AMR_NB:
{
    }
    case AV_CODEC_ID_EVRC:
{
    }
    case AV_CODEC_ID_GSM:
{
    }
    case AV_CODEC_ID_QCELP:
{
    }
    case AV_CODEC_ID_RA_288:
    return 160;
    case AV_CODEC_ID_AMR_WB:
{
    }
    case AV_CODEC_ID_GSM_MS:
    return 320;
    case AV_CODEC_ID_MP1:
    return 384;
    case AV_CODEC_ID_ATRAC1:
    return 512;
    case AV_CODEC_ID_ATRAC3:
    return 1024;
    case AV_CODEC_ID_MP2:
{
    }
    case AV_CODEC_ID_MUSEPACK7:
    return 1152;
    case AV_CODEC_ID_AC3:
    return 1536;
  }
  if (sr > 0) {
/* calc from sample rate */
    if (id == AV_CODEC_ID_TTA) {
      return 256 * sr / 245;
    }
    if (ch > 0) {
/* calc from sample rate and channels */
      if (id == AV_CODEC_ID_BINKAUDIO_DCT) {
        return (480 << sr / 22050) / ch;
      }
    }
  }
  if (ba > 0) {
/* calc from block_align */
    if (id == AV_CODEC_ID_SIPR) {
      switch(ba){
        case 20:
        return 160;
        case 19:
        return 144;
        case 29:
        return 288;
        case 37:
        return 480;
      }
    }
    else {
      if (id == AV_CODEC_ID_ILBC) {
        switch(ba){
          case 38:
          return 160;
          case 50:
          return 240;
        }
      }
    }
  }
  if (frame_bytes > 0) {
/* calc from frame_bytes only */
    if (id == AV_CODEC_ID_TRUESPEECH) {
      return 240 * (frame_bytes / 32);
    }
    if (id == AV_CODEC_ID_NELLYMOSER) {
      return 256 * (frame_bytes / 64);
    }
    if (id == AV_CODEC_ID_RA_144) {
      return 160 * (frame_bytes / 20);
    }
    if (id == AV_CODEC_ID_G723_1) {
      return 240 * (frame_bytes / 24);
    }
    if (bps > 0) {
/* calc from frame_bytes and bits_per_coded_sample */
      if (id == AV_CODEC_ID_ADPCM_G726) {
        return frame_bytes * 8 / bps;
      }
    }
    if (ch > 0) {
/* calc from frame_bytes and channels */
      switch(id){
        case AV_CODEC_ID_ADPCM_AFC:
        return frame_bytes / ('\t' * ch) * 16;
        case AV_CODEC_ID_ADPCM_4XM:
{
        }
        case AV_CODEC_ID_ADPCM_IMA_ISS:
        return (frame_bytes - 4 * ch) * 2 / ch;
        case AV_CODEC_ID_ADPCM_IMA_SMJPEG:
        return (frame_bytes - 4) * 2 / ch;
        case AV_CODEC_ID_ADPCM_IMA_AMV:
        return (frame_bytes - 8) * 2 / ch;
        case AV_CODEC_ID_ADPCM_XA:
        return frame_bytes / 128 * 224 / ch;
        case AV_CODEC_ID_INTERPLAY_DPCM:
        return (frame_bytes - 6 - ch) / ch;
        case AV_CODEC_ID_ROQ_DPCM:
        return (frame_bytes - 8) / ch;
        case AV_CODEC_ID_XAN_DPCM:
        return (frame_bytes - 2 * ch) / ch;
        case AV_CODEC_ID_MACE3:
        return 3 * frame_bytes / ch;
        case AV_CODEC_ID_MACE6:
        return 6 * frame_bytes / ch;
        case AV_CODEC_ID_PCM_LXF:
        return 2 * (frame_bytes / (5 * ch));
        case AV_CODEC_ID_IAC:
{
        }
        case AV_CODEC_ID_IMC:
        return 4 * frame_bytes / ch;
      }
      if (tag) {
/* calc from frame_bytes, channels, and codec_tag */
        if (id == AV_CODEC_ID_SOL_DPCM) {
          if (tag == 3) {
            return frame_bytes / ch;
          }
          else {
            return frame_bytes * 2 / ch;
          }
        }
      }
      if (ba > 0) {
/* calc from frame_bytes, channels, and block_align */
        int blocks = frame_bytes / ba;
        switch(avctx -> codec_id){
          case AV_CODEC_ID_ADPCM_IMA_WAV:
          return blocks * (1 + (ba - 4 * ch) / (4 * ch) * 8);
          case AV_CODEC_ID_ADPCM_IMA_DK3:
          return blocks * ((ba - 16) * 2 / 3 * 4 / ch);
          case AV_CODEC_ID_ADPCM_IMA_DK4:
          return blocks * (1 + (ba - 4 * ch) * 2 / ch);
          case AV_CODEC_ID_ADPCM_MS:
          return blocks * (2 + (ba - 7 * ch) * 2 / ch);
        }
      }
      if (bps > 0) {
/* calc from frame_bytes, channels, and bits_per_coded_sample */
        switch(avctx -> codec_id){
          case AV_CODEC_ID_PCM_DVD:
{
            if (bps < 4) {
              return 0;
            }
            return 2 * (frame_bytes / (bps * 2 / 8 * ch));
          }
          case AV_CODEC_ID_PCM_BLURAY:
{
            if (bps < 4) {
              return 0;
            }
            return frame_bytes / ((ch + 2 - 1 & ~(2 - 1)) * bps / 8);
          }
          case AV_CODEC_ID_S302M:
          return 2 * (frame_bytes / ((bps + 4) / 4)) / ch;
        }
      }
    }
  }
  return 0;
}
#if !HAVE_THREADS
#endif

unsigned int av_xiphlacing(unsigned char *s,unsigned int v)
{
  unsigned int n = 0;
  while(v >= 0xff){
     *(s++) = 0xff;
    v -= 0xff;
    n++;
  }
   *s = v;
  n++;
  return n;
}

int ff_match_2uint16(const uint16_t (*tab)[2],int size,int a,int b)
{
  int i;
  for (i = 0; i < size && !(tab[i][0] == a && tab[i][1] == b); i++) 
    ;
  return i;
}

void av_log_missing_feature(void *avc,const char *feature,int want_sample)
{
  av_log(avc,24,"%s is not implemented. Update your FFmpeg version to the newest one from Git. If the problem still occurs, it means that your file has a feature which has not been implemented.\n",feature);
  if (want_sample) {
    av_log_ask_for_sample(avc,((void *)0));
  }
}

void av_log_ask_for_sample(void *avc,const char *msg,... )
{
  va_list argument_list;
  __builtin_va_start(argument_list,msg);
  if (msg) {
    av_vlog(avc,24,msg,argument_list);
  }
  av_log(avc,24,"If you want to help, upload a sample of this file to ftp://upload.ffmpeg.org/MPlayer/incoming/ and contact the ffmpeg-devel mailing list.\n");
  __builtin_va_end(argument_list);
}
static AVHWAccel *first_hwaccel = ((void *)0);

void av_register_hwaccel(AVHWAccel *hwaccel)
{
  AVHWAccel **p = &first_hwaccel;
  while( *p)
    p = &( *p) -> next;
   *p = hwaccel;
  hwaccel -> next = ((void *)0);
}

AVHWAccel *av_hwaccel_next(AVHWAccel *hwaccel)
{
  return hwaccel?hwaccel -> next : first_hwaccel;
}

AVHWAccel *ff_find_hwaccel(enum AVCodecID codec_id,enum AVPixelFormat pix_fmt)
{
  AVHWAccel *hwaccel = ((void *)0);
  while(hwaccel = av_hwaccel_next(hwaccel))
    if ((hwaccel -> id) == codec_id && (hwaccel -> pix_fmt) == pix_fmt) {
      return hwaccel;
    }
  return ((void *)0);
}

int av_lockmgr_register(int (*cb)(void **, enum AVLockOp ))
{
  if (ff_lockmgr_cb) {
    if (ff_lockmgr_cb(&codec_mutex,AV_LOCK_DESTROY)) {
      return - 1;
    }
    if (ff_lockmgr_cb(&avformat_mutex,AV_LOCK_DESTROY)) {
      return - 1;
    }
  }
  ff_lockmgr_cb = cb;
  if (ff_lockmgr_cb) {
    if (ff_lockmgr_cb(&codec_mutex,AV_LOCK_CREATE)) {
      return - 1;
    }
    if (ff_lockmgr_cb(&avformat_mutex,AV_LOCK_CREATE)) {
      return - 1;
    }
  }
  return 0;
}

int ff_lock_avcodec(AVCodecContext *log_ctx)
{
  if (ff_lockmgr_cb) {
    if (( *ff_lockmgr_cb)(&codec_mutex,AV_LOCK_OBTAIN)) {
      return - 1;
    }
  }
  entangled_thread_counter++;
  if (entangled_thread_counter != 1) {
    av_log(log_ctx,16,"Insufficient thread locking around avcodec_open/close()\n");
    ff_avcodec_locked = 1;
    ff_unlock_avcodec();
    return - 22;
  }
  do {
    if (!(!ff_avcodec_locked)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","!ff_avcodec_locked","utils.c",2743);
      abort();
    }
  }while (0);
  ff_avcodec_locked = 1;
  return 0;
}

int ff_unlock_avcodec()
{
  do {
    if (!ff_avcodec_locked) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","ff_avcodec_locked","utils.c",2750);
      abort();
    }
  }while (0);
  ff_avcodec_locked = 0;
  entangled_thread_counter--;
  if (ff_lockmgr_cb) {
    if (( *ff_lockmgr_cb)(&codec_mutex,AV_LOCK_RELEASE)) {
      return - 1;
    }
  }
  return 0;
}

int avpriv_lock_avformat()
{
  if (ff_lockmgr_cb) {
    if (( *ff_lockmgr_cb)(&avformat_mutex,AV_LOCK_OBTAIN)) {
      return - 1;
    }
  }
  return 0;
}

int avpriv_unlock_avformat()
{
  if (ff_lockmgr_cb) {
    if (( *ff_lockmgr_cb)(&avformat_mutex,AV_LOCK_RELEASE)) {
      return - 1;
    }
  }
  return 0;
}

unsigned int avpriv_toupper4(unsigned int x)
{
  return (av_toupper((x & 0xff)) + (av_toupper((x >> 8 & 0xff)) << 8) + (av_toupper((x >> 16 & 0xff)) << 16) + (av_toupper((x >> 24 & 0xff)) << 24));
}
#if !HAVE_THREADS
#endif

enum AVMediaType avcodec_get_type(enum AVCodecID codec_id)
{
  AVCodec *c = avcodec_find_decoder(codec_id);
  if (!c) {
    c = avcodec_find_encoder(codec_id);
  }
  if (c) {
    return c -> type;
  }
  if (codec_id <= AV_CODEC_ID_NONE) {
    return AVMEDIA_TYPE_UNKNOWN;
  }
  else {
    if (codec_id < AV_CODEC_ID_FIRST_AUDIO) {
      return AVMEDIA_TYPE_VIDEO;
    }
    else {
      if (codec_id < AV_CODEC_ID_FIRST_SUBTITLE) {
        return AVMEDIA_TYPE_AUDIO;
      }
      else {
        if (codec_id < AV_CODEC_ID_FIRST_UNKNOWN) {
          return AVMEDIA_TYPE_SUBTITLE;
        }
      }
    }
  }
  return AVMEDIA_TYPE_UNKNOWN;
}

int avcodec_is_open(AVCodecContext *s)
{
  return !(!s -> internal);
}

int avpriv_bprint_to_extradata(AVCodecContext *avctx,struct AVBPrint *buf)
{
  int ret;
  char *str;
  ret = av_bprint_finalize(buf,&str);
  if (ret < 0) {
    return ret;
  }
  avctx -> extradata = str;
/* Note: the string is NUL terminated (so extradata can be read as a
     * string), but the ending character is not accounted in the size (in
     * binary formats you are likely not supposed to mux that character). When
     * extradata is copied, it is also padded with FF_INPUT_BUFFER_PADDING_SIZE
     * zeros. */
  avctx -> extradata_size = (buf -> len);
  return 0;
}
