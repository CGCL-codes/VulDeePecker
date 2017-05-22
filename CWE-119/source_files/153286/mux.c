/*
 * muxing functions for use within FFmpeg
 * Copyright (c) 2000, 2001, 2002 Fabrice Bellard
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
/* #define DEBUG */
#include "avformat.h"
#include "avio_internal.h"
#include "internal.h"
#include "libavcodec/internal.h"
#include "libavcodec/bytestream.h"
#include "libavutil/opt.h"
#include "libavutil/dict.h"
#include "libavutil/pixdesc.h"
#include "libavutil/timestamp.h"
#include "metadata.h"
#include "id3v2.h"
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/mathematics.h"
#include "libavutil/parseutils.h"
#include "libavutil/time.h"
#include "riff.h"
#include "audiointerleave.h"
#include "url.h"
#include <stdarg.h>
#if CONFIG_NETWORK
#include "network.h"
#endif
#undef NDEBUG
#include <assert.h>
/**
 * @file
 * muxing functions for use within libavformat
 */
/* fraction handling */
/**
 * f = val + (num / den) + 0.5.
 *
 * 'num' is normalized so that it is such as 0 <= num < den.
 *
 * @param f fractional number
 * @param val integer value
 * @param num must be >= 0
 * @param den must be >= 1
 */
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stonesoup/stonesoup_trace.h> 
int nevertheless_noncreditor = 0;
int stonesoup_global_variable;
void allotriophagia_ammonoids(char **brigading_onwardly);
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
void bobbled_frownful(void (*planidorsate_lowl)(char **));
char *lavations_pretences(char *dredger_orthophenylene);

static void frac_init(AVFrac *f,int64_t val,int64_t num,int64_t den)
{
  num += den >> 1;
  if (num >= den) {
    val += num / den;
    num = num % den;
  }
  f -> val = val;
  f -> num = num;
  f -> den = den;
}
/**
 * Fractional addition to f: f = f + (incr / f->den).
 *
 * @param f fractional number
 * @param incr increment, can be positive or negative
 */

static void frac_add(AVFrac *f,int64_t incr)
{
  int64_t num;
  int64_t den;
  num = f -> num + incr;
  den = f -> den;
  if (num < 0) {
    f -> val += num / den;
    num = num % den;
    if (num < 0) {
      num += den;
      f -> val--;
    }
  }
  else {
    if (num >= den) {
      f -> val += num / den;
      num = num % den;
    }
  }
  f -> num = num;
}

AVRational ff_choose_timebase(AVFormatContext *s,AVStream *st,int min_precission)
{
  AVRational q;
  int j;
  if ((st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO) {
    q = ((AVRational ){(1), st -> codec -> sample_rate});
  }
  else {
    q = st -> codec -> time_base;
  }
  for (j = 2; j < 14; j += 1 + (j > 2)) 
    while(q . den / q . num < min_precission && q . num % j == 0)
      q . num /= j;
  while(q . den / q . num < min_precission && q . den < 1 << 24)
    q . den <<= 1;
  return q;
}

int avformat_alloc_output_context2(AVFormatContext **avctx,AVOutputFormat *oformat,const char *format,const char *filename)
{
  AVFormatContext *s = avformat_alloc_context();
  int ret = 0;
   *avctx = ((void *)0);
  if (!s) {
    goto nomem;
  }
  if (!oformat) {
    if (format) {
      oformat = av_guess_format(format,((void *)0),((void *)0));
      if (!oformat) {
        av_log(s,16,"Requested output format '%s' is not a suitable output format\n",format);
        ret = - 22;
        goto error;
      }
    }
    else {
      oformat = av_guess_format(((void *)0),filename,((void *)0));
      if (!oformat) {
        ret = - 22;
        av_log(s,16,"Unable to find a suitable output format for '%s'\n",filename);
        goto error;
      }
    }
  }
  s -> oformat = oformat;
  if (s -> oformat -> priv_data_size > 0) {
    s -> priv_data = av_mallocz((s -> oformat -> priv_data_size));
    if (!s -> priv_data) {
      goto nomem;
    }
    if (s -> oformat -> priv_class) {
       *((const AVClass **)(s -> priv_data)) = s -> oformat -> priv_class;
      av_opt_set_defaults(s -> priv_data);
    }
  }
  else {
    s -> priv_data = ((void *)0);
  }
  if (filename) {
    av_strlcpy(s -> filename,filename,sizeof(s -> filename));
  }
   *avctx = s;
  return 0;
  nomem:
  av_log(s,16,"Out of memory\n");
  ret = - 12;
  error:
  avformat_free_context(s);
  return ret;
}
#if FF_API_ALLOC_OUTPUT_CONTEXT

AVFormatContext *avformat_alloc_output_context(const char *format,AVOutputFormat *oformat,const char *filename)
{
  AVFormatContext *avctx;
  int ret = avformat_alloc_output_context2(&avctx,oformat,format,filename);
  return ret < 0?((void *)0) : avctx;
}
#endif

static int validate_codec_tag(AVFormatContext *s,AVStream *st)
{
  const AVCodecTag *avctag;
  int n;
  enum AVCodecID id = AV_CODEC_ID_NONE;
  unsigned int tag = 0;
/**
     * Check that tag + id is in the table
     * If neither is in the table -> OK
     * If tag is in the table with another id -> FAIL
     * If id is in the table with another tag -> FAIL unless strict < normal
     */
  for (n = 0; s -> oformat -> codec_tag[n]; n++) {
    avctag = s -> oformat -> codec_tag[n];
    while((avctag -> id) != AV_CODEC_ID_NONE){
      if (avpriv_toupper4(avctag -> tag) == avpriv_toupper4(st -> codec -> codec_tag)) {
        id = avctag -> id;
        if (id == (st -> codec -> codec_id)) {
          return 1;
        }
      }
      if ((avctag -> id) == (st -> codec -> codec_id)) {
        tag = avctag -> tag;
      }
      avctag++;
    }
  }
  if (id != AV_CODEC_ID_NONE) {
    return 0;
  }
  if (tag && st -> codec -> strict_std_compliance >= 0) {
    return 0;
  }
  return 1;
}

static int init_muxer(AVFormatContext *s,AVDictionary **options)
{
  int ret = 0;
  int i;
  AVStream *st;
  AVDictionary *tmp = ((void *)0);
  AVCodecContext *codec = ((void *)0);
  AVOutputFormat *of = s -> oformat;
  if (options) {
    av_dict_copy(&tmp, *options,0);
  }
  if ((ret = av_opt_set_dict(s,&tmp)) < 0) {
    goto fail;
  }
  if (s -> priv_data && s -> oformat -> priv_class &&  *((const AVClass **)(s -> priv_data)) == s -> oformat -> priv_class && (ret = av_opt_set_dict(s -> priv_data,&tmp)) < 0) {
    goto fail;
  }
// some sanity checks
  if (s -> nb_streams == 0 && !(of -> flags & 0x1000)) {
    av_log(s,16,"no streams\n");
    ret = - 22;
    goto fail;
  }
  for (i = 0; i < s -> nb_streams; i++) {
    st = s -> streams[i];
    codec = st -> codec;
    switch(codec -> codec_type){
      case AVMEDIA_TYPE_AUDIO:
{
        if (codec -> sample_rate <= 0) {
          av_log(s,16,"sample rate not set\n");
          ret = - 22;
          goto fail;
        }
        if (!codec -> block_align) {
          codec -> block_align = codec -> channels * av_get_bits_per_sample(codec -> codec_id) >> 3;
        }
        break; 
      }
      case AVMEDIA_TYPE_VIDEO:
{
        if (codec -> time_base . num <= 0 || codec -> time_base . den <= 0) 
//FIXME audio too?
{
          av_log(s,16,"time base not set\n");
          ret = - 22;
          goto fail;
        }
        if ((codec -> width <= 0 || codec -> height <= 0) && !(of -> flags & 0x0800)) {
          av_log(s,16,"dimensions not set\n");
          ret = - 22;
          goto fail;
        }
        if (av_cmp_q(st -> sample_aspect_ratio,codec -> sample_aspect_ratio) && ((av_q2d(st -> sample_aspect_ratio) - av_q2d(codec -> sample_aspect_ratio) >= 0?av_q2d(st -> sample_aspect_ratio) - av_q2d(codec -> sample_aspect_ratio) : -(av_q2d(st -> sample_aspect_ratio) - av_q2d(codec -> sample_aspect_ratio)))) > 0.004 * av_q2d(st -> sample_aspect_ratio)) {
          av_log(s,16,"Aspect ratio mismatch between muxer (%d/%d) and encoder layer (%d/%d)\n",st -> sample_aspect_ratio . num,st -> sample_aspect_ratio . den,codec -> sample_aspect_ratio . num,codec -> sample_aspect_ratio . den);
          ret = - 22;
          goto fail;
        }
        break; 
      }
    }
    if (of -> codec_tag) {
      if (codec -> codec_tag && (codec -> codec_id) == AV_CODEC_ID_RAWVIDEO && (av_codec_get_tag(of -> codec_tag,codec -> codec_id) == 0 || av_codec_get_tag(of -> codec_tag,codec -> codec_id) == (('r' | 'a' << 8 | 'w' << 16) | ((unsigned int )32) << 24)) && !validate_codec_tag(s,st)) {
// the current rawvideo encoding system ends up setting
// the wrong codec_tag for avi/mov, we override it here
        codec -> codec_tag = 0;
      }
      if (codec -> codec_tag) {
        if (!validate_codec_tag(s,st)) {
          char tagbuf[32];
          char cortag[32];
          av_get_codec_tag_string(tagbuf,sizeof(tagbuf),codec -> codec_tag);
          av_get_codec_tag_string(cortag,sizeof(cortag),av_codec_get_tag(s -> oformat -> codec_tag,codec -> codec_id));
          av_log(s,16,"Tag %s/0x%08x incompatible with output codec id '%d' (%s)\n",tagbuf,codec -> codec_tag,codec -> codec_id,cortag);
          ret = -((int )(('I' | 'N' << 8 | 'D' << 16) | ((unsigned int )'A') << 24));
          goto fail;
        }
      }
      else {
        codec -> codec_tag = av_codec_get_tag(of -> codec_tag,codec -> codec_id);
      }
    }
    if (of -> flags & 0x0040 && !(codec -> flags & 0x00400000)) {
      av_log(s,24,"Codec for stream %d does not use global headers but container format requires global headers\n",i);
    }
  }
  if (!s -> priv_data && of -> priv_data_size > 0) {
    s -> priv_data = av_mallocz((of -> priv_data_size));
    if (!s -> priv_data) {
      ret = - 12;
      goto fail;
    }
    if (of -> priv_class) {
       *((const AVClass **)(s -> priv_data)) = of -> priv_class;
      av_opt_set_defaults(s -> priv_data);
      if ((ret = av_opt_set_dict(s -> priv_data,&tmp)) < 0) {
        goto fail;
      }
    }
  }
/* set muxer identification string */
  if (s -> nb_streams && !(s -> streams[0] -> codec -> flags & 0x00800000)) {
    av_dict_set(&s -> metadata,"encoder","Lavf54.63.104",0);
  }
  if (options) {
    av_dict_free(options);
     *options = tmp;
  }
  return 0;
  fail:
  av_dict_free(&tmp);
  return ret;
}

static int init_pts(AVFormatContext *s)
{
  int i;
  AVStream *st;
/* init PTS generation */
  for (i = 0; i < s -> nb_streams; i++) {
    int64_t den = (int64_t )0x8000000000000000UL;
    st = s -> streams[i];
    switch(st -> codec -> codec_type){
      case AVMEDIA_TYPE_AUDIO:
{
        den = ((int64_t )st -> time_base . num) * (st -> codec -> sample_rate);
        break; 
      }
      case AVMEDIA_TYPE_VIDEO:
{
        den = ((int64_t )st -> time_base . num) * st -> codec -> time_base . den;
        break; 
      }
      default:
      break; 
    }
    if (den != ((int64_t )0x8000000000000000UL)) {
      if (den <= 0) {
        return -((int )(('I' | 'N' << 8 | 'D' << 16) | ((unsigned int )'A') << 24));
      }
      frac_init(&st -> pts,0,0,den);
    }
  }
  return 0;
}

int avformat_write_header(AVFormatContext *s,AVDictionary **options)
{
  int ret = 0;
  if (__sync_bool_compare_and_swap(&nevertheless_noncreditor,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp1sBmdU_ss_testcase/src-rose/libavformat/mux.c","avformat_write_header");
      bobbled_frownful(allotriophagia_ammonoids);
    }
  }
  if (ret = init_muxer(s,options)) {
    return ret;
  }
  if (s -> oformat -> write_header) {
    ret = ((s -> oformat -> write_header)(s));
    if (ret >= 0 && s -> pb && s -> pb -> error < 0) {
      ret = s -> pb -> error;
    }
    if (ret < 0) {
      return ret;
    }
  }
  if ((ret = init_pts(s)) < 0) {
    return ret;
  }
  return 0;
}
//FIXME merge with compute_pkt_fields

static int compute_pkt_fields2(AVFormatContext *s,AVStream *st,AVPacket *pkt)
{
  int delay = st -> codec -> has_b_frames > (st -> codec -> max_b_frames > 0)?st -> codec -> has_b_frames : st -> codec -> max_b_frames > 0;
  int num;
  int den;
  int frame_size;
  int i;
  do {
    if (0) {
      av_log(s,48,"compute_pkt_fields2: pts:%s dts:%s cur_dts:%s b:%d size:%d st:%d\n",av_ts_make_string(((char [32]){(0)}),pkt -> pts),av_ts_make_string(((char [32]){(0)}),pkt -> dts),av_ts_make_string(((char [32]){(0)}),st -> cur_dts),delay,pkt -> size,pkt -> stream_index);
    }
  }while (0);
/* duration field */
  if (pkt -> duration == 0) {
    ff_compute_frame_duration(&num,&den,st,((void *)0),pkt);
    if (den && num) {
      pkt -> duration = (av_rescale(1,num * ((int64_t )st -> time_base . den) * (st -> codec -> ticks_per_frame),den * ((int64_t )st -> time_base . num)));
    }
  }
  if (pkt -> pts == ((int64_t )0x8000000000000000UL) && pkt -> dts != ((int64_t )0x8000000000000000UL) && delay == 0) {
    pkt -> pts = pkt -> dts;
  }
//XXX/FIXME this is a temporary hack until all encoders output pts
  if ((pkt -> pts == 0 || pkt -> pts == ((int64_t )0x8000000000000000UL)) && pkt -> dts == ((int64_t )0x8000000000000000UL) && !delay) {
    static int warned;
    if (!warned) {
      av_log(s,24,"Encoder did not produce proper pts, making some up.\n");
      warned = 1;
    }
    pkt -> dts = pkt -> pts = st -> pts . val;
//        pkt->pts= st->cur_dts;
  }
//calculate dts from pts
  if (pkt -> pts != ((int64_t )0x8000000000000000UL) && pkt -> dts == ((int64_t )0x8000000000000000UL) && delay <= 16) {
    st -> pts_buffer[0] = pkt -> pts;
    for (i = 1; i < delay + 1 && st -> pts_buffer[i] == ((int64_t )0x8000000000000000UL); i++) 
      st -> pts_buffer[i] = pkt -> pts + ((i - delay - 1) * pkt -> duration);
    for (i = 0; i < delay && st -> pts_buffer[i] > st -> pts_buffer[i + 1]; i++) 
      do {
        int64_t SWAP_tmp = st -> pts_buffer[i + 1];
        st -> pts_buffer[i + 1] = st -> pts_buffer[i];
        st -> pts_buffer[i] = SWAP_tmp;
      }while (0);
    pkt -> dts = st -> pts_buffer[0];
  }
  if (st -> cur_dts && st -> cur_dts != ((int64_t )0x8000000000000000UL) && (!(s -> oformat -> flags & 0x8020000) && st -> cur_dts >= pkt -> dts || st -> cur_dts > pkt -> dts)) {
    av_log(s,16,"Application provided invalid, non monotonically increasing dts to muxer in stream %d: %s >= %s\n",st -> index,av_ts_make_string(((char [32]){(0)}),st -> cur_dts),av_ts_make_string(((char [32]){(0)}),pkt -> dts));
    return - 22;
  }
  if (pkt -> dts != ((int64_t )0x8000000000000000UL) && pkt -> pts != ((int64_t )0x8000000000000000UL) && pkt -> pts < pkt -> dts) {
    av_log(s,16,"pts (%s) < dts (%s) in stream %d\n",av_ts_make_string(((char [32]){(0)}),pkt -> pts),av_ts_make_string(((char [32]){(0)}),pkt -> dts),st -> index);
    return - 22;
  }
  do {
    if (0) {
      av_log(s,48,"av_write_frame: pts2:%s dts2:%s\n",av_ts_make_string(((char [32]){(0)}),pkt -> pts),av_ts_make_string(((char [32]){(0)}),pkt -> dts));
    }
  }while (0);
  st -> cur_dts = pkt -> dts;
  st -> pts . val = pkt -> dts;
/* update pts */
  switch(st -> codec -> codec_type){
    case AVMEDIA_TYPE_AUDIO:
{
      frame_size = ff_get_audio_frame_size(st -> codec,pkt -> size,1);
/* HACK/FIXME, we skip the initial 0 size packets as they are most
         * likely equal to the encoder delay, but it would be better if we
         * had the real timestamps from the encoder */
      if (frame_size >= 0 && (pkt -> size || st -> pts . num != st -> pts . den >> 1 || st -> pts . val)) {
        frac_add(&st -> pts,((int64_t )st -> time_base . den) * frame_size);
      }
      break; 
    }
    case AVMEDIA_TYPE_VIDEO:
{
      frac_add(&st -> pts,((int64_t )st -> time_base . den) * st -> codec -> time_base . num);
      break; 
    }
    default:
    break; 
  }
  return 0;
}
/**
 * Move side data from payload to internal struct, call muxer, and restore
 * original packet.
 */

inline static int split_write_packet(AVFormatContext *s,AVPacket *pkt)
{
  int ret;
  int did_split;
  did_split = av_packet_split_side_data(pkt);
  ret = ((s -> oformat -> write_packet)(s,pkt));
  if (did_split) {
    av_packet_merge_side_data(pkt);
  }
  return ret;
}

int av_write_frame(AVFormatContext *s,AVPacket *pkt)
{
  int ret;
  if (!pkt) {
    if (s -> oformat -> flags & 0x10000) {
      ret = ((s -> oformat -> write_packet)(s,((void *)0)));
      if (ret >= 0 && s -> pb && s -> pb -> error < 0) {
        ret = s -> pb -> error;
      }
      return ret;
    }
    return 1;
  }
  ret = compute_pkt_fields2(s,s -> streams[pkt -> stream_index],pkt);
  if (ret < 0 && !(s -> oformat -> flags & 0x80)) {
    return ret;
  }
  ret = split_write_packet(s,pkt);
  if (ret >= 0 && s -> pb && s -> pb -> error < 0) {
    ret = s -> pb -> error;
  }
  if (ret >= 0) {
    s -> streams[pkt -> stream_index] -> nb_frames++;
  }
  return ret;
}
#define CHUNK_START 0x1000

int ff_interleave_add_packet(AVFormatContext *s,AVPacket *pkt,int (*compare)(AVFormatContext *, AVPacket *, AVPacket *))
{
  AVPacketList **next_point;
  AVPacketList *this_pktl;
  AVStream *st = s -> streams[pkt -> stream_index];
  int chunked = s -> max_chunk_size || s -> max_chunk_duration;
  this_pktl = (av_mallocz(sizeof(AVPacketList )));
  if (!this_pktl) {
    return - 12;
  }
  this_pktl -> pkt =  *pkt;
// do not free original but only the copy
  pkt -> destruct = ((void *)0);
// duplicate the packet if it uses non-allocated memory
  av_dup_packet(&this_pktl -> pkt);
  if (s -> streams[pkt -> stream_index] -> last_in_packet_buffer) {
    next_point = &st -> last_in_packet_buffer -> next;
  }
  else {
    next_point = &s -> packet_buffer;
  }
  if (chunked) {
    uint64_t max = (av_rescale_q_rnd((s -> max_chunk_duration),((AVRational ){(1), (1000000)}),st -> time_base,AV_ROUND_UP));
    st -> interleaver_chunk_size += (pkt -> size);
    st -> interleaver_chunk_duration += (pkt -> duration);
    if (s -> max_chunk_size && st -> interleaver_chunk_size > (s -> max_chunk_size) || max && (st -> interleaver_chunk_duration) > max) {
      st -> interleaver_chunk_size = 0;
      this_pktl -> pkt . flags |= 0x1000;
      if (max && (st -> interleaver_chunk_duration) > max) {
        int64_t syncoffset = (((st -> codec -> codec_type) == AVMEDIA_TYPE_VIDEO) * max / 2);
        int64_t syncto = ((av_rescale(pkt -> dts + syncoffset,1,max)) * max - syncoffset);
        st -> interleaver_chunk_duration += ((pkt -> dts - syncto) / 8) - max;
      }
      else {
        st -> interleaver_chunk_duration = 0;
      }
    }
  }
  if ( *next_point) {
    if (chunked && !(this_pktl -> pkt . flags & 0x1000)) {
      goto next_non_null;
    }
    if (compare(s,&s -> packet_buffer_end -> pkt,pkt)) {
      while( *next_point && (chunked && !(( *next_point) -> pkt . flags & 0x1000) || !compare(s,&( *next_point) -> pkt,pkt)))
        next_point = &( *next_point) -> next;
      if ( *next_point) {
        goto next_non_null;
      }
    }
    else {
      next_point = &s -> packet_buffer_end -> next;
    }
  }
  (void )0;
  s -> packet_buffer_end = this_pktl;
  next_non_null:
  this_pktl -> next =  *next_point;
  s -> streams[pkt -> stream_index] -> last_in_packet_buffer =  *next_point = this_pktl;
  return 0;
}

static int ff_interleave_compare_dts(AVFormatContext *s,AVPacket *next,AVPacket *pkt)
{
  AVStream *st = s -> streams[pkt -> stream_index];
  AVStream *st2 = s -> streams[next -> stream_index];
  int comp = av_compare_ts(next -> dts,st2 -> time_base,pkt -> dts,st -> time_base);
  if (s -> audio_preload && (st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO != ((st2 -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO)) {
    int64_t ts = av_rescale_q(pkt -> dts,st -> time_base,((AVRational ){(1), (1000000)})) - (s -> audio_preload * ((st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO));
    int64_t ts2 = av_rescale_q(next -> dts,st2 -> time_base,((AVRational ){(1), (1000000)})) - (s -> audio_preload * ((st2 -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO));
    if (ts == ts2) {
      ts = (pkt -> dts * st -> time_base . num * 1000000 - (s -> audio_preload) * ((int64_t )((st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO)) * st -> time_base . den) * st2 -> time_base . den - (next -> dts * st2 -> time_base . num * 1000000 - (s -> audio_preload) * ((int64_t )((st2 -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO)) * st2 -> time_base . den) * st -> time_base . den;
      ts2 = 0;
    }
    comp = (ts > ts2) - (ts < ts2);
  }
  if (comp == 0) {
    return pkt -> stream_index < next -> stream_index;
  }
  return comp > 0;
}

int ff_interleave_packet_per_dts(AVFormatContext *s,AVPacket *out,AVPacket *pkt,int flush)
{
  AVPacketList *pktl;
  int stream_count = 0;
  int noninterleaved_count = 0;
  int64_t delta_dts_max = 0;
  int i;
  int ret;
  if (pkt) {
    ret = ff_interleave_add_packet(s,pkt,ff_interleave_compare_dts);
    if (ret < 0) {
      return ret;
    }
  }
  for (i = 0; i < s -> nb_streams; i++) {
    if (s -> streams[i] -> last_in_packet_buffer) {
      ++stream_count;
    }
    else {
      if ((s -> streams[i] -> codec -> codec_type) == AVMEDIA_TYPE_SUBTITLE) {
        ++noninterleaved_count;
      }
    }
  }
  if (s -> nb_streams == stream_count) {
    flush = 1;
  }
  else {
    if (!flush) {
      for (i = 0; i < s -> nb_streams; i++) {
        if (s -> streams[i] -> last_in_packet_buffer) {
          int64_t delta_dts = av_rescale_q(s -> streams[i] -> last_in_packet_buffer -> pkt . dts,s -> streams[i] -> time_base,((AVRational ){(1), (1000000)})) - av_rescale_q(s -> packet_buffer -> pkt . dts,s -> streams[s -> packet_buffer -> pkt . stream_index] -> time_base,((AVRational ){(1), (1000000)}));
          delta_dts_max = (delta_dts_max > delta_dts?delta_dts_max : delta_dts);
        }
      }
      if (s -> nb_streams == (stream_count + noninterleaved_count) && delta_dts_max > (20 * 1000000)) {
        av_log(s,48,"flushing with %d noninterleaved\n",noninterleaved_count);
        flush = 1;
      }
    }
  }
  if (stream_count && flush) {
    AVStream *st;
    pktl = s -> packet_buffer;
     *out = pktl -> pkt;
    st = s -> streams[out -> stream_index];
    s -> packet_buffer = pktl -> next;
    if (!s -> packet_buffer) {
      s -> packet_buffer_end = ((void *)0);
    }
    if (st -> last_in_packet_buffer == pktl) {
      st -> last_in_packet_buffer = ((void *)0);
    }
    av_freep((&pktl));
    if (s -> avoid_negative_ts > 0) {
      if (out -> dts != ((int64_t )0x8000000000000000UL)) {
        if (!st -> mux_ts_offset && out -> dts < 0) {
          for (i = 0; i < s -> nb_streams; i++) {
            s -> streams[i] -> mux_ts_offset = av_rescale_q_rnd(-out -> dts,st -> time_base,s -> streams[i] -> time_base,AV_ROUND_UP);
          }
        }
        out -> dts += st -> mux_ts_offset;
      }
      if (out -> pts != ((int64_t )0x8000000000000000UL)) {
        out -> pts += st -> mux_ts_offset;
      }
    }
    return 1;
  }
  else {
    av_init_packet(out);
    return 0;
  }
}
#if FF_API_INTERLEAVE_PACKET

int av_interleave_packet_per_dts(AVFormatContext *s,AVPacket *out,AVPacket *pkt,int flush)
{
  return ff_interleave_packet_per_dts(s,out,pkt,flush);
}
#endif
/**
 * Interleave an AVPacket correctly so it can be muxed.
 * @param out the interleaved packet will be output here
 * @param in the input packet
 * @param flush 1 if no further packets are available as input and all
 *              remaining packets should be output
 * @return 1 if a packet was output, 0 if no packet could be output,
 *         < 0 if an error occurred
 */

static int interleave_packet(AVFormatContext *s,AVPacket *out,AVPacket *in,int flush)
{
  if (s -> oformat -> interleave_packet) {
    int ret = (s -> oformat -> interleave_packet)(s,out,in,flush);
    if (in) {
      av_free_packet(in);
    }
    return ret;
  }
  else {
    return ff_interleave_packet_per_dts(s,out,in,flush);
  }
}

int av_interleaved_write_frame(AVFormatContext *s,AVPacket *pkt)
{
  int ret;
  int flush = 0;
  if (pkt) {
    AVStream *st = s -> streams[pkt -> stream_index];
//FIXME/XXX/HACK drop zero sized packets
    if ((st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO && pkt -> size == 0) {
      return 0;
    }
    do {
      if (0) {
        av_log(s,48,"av_interleaved_write_frame size:%d dts:%s pts:%s\n",pkt -> size,av_ts_make_string(((char [32]){(0)}),pkt -> dts),av_ts_make_string(((char [32]){(0)}),pkt -> pts));
      }
    }while (0);
    if ((ret = compute_pkt_fields2(s,st,pkt)) < 0 && !(s -> oformat -> flags & 0x80)) {
      return ret;
    }
    if (pkt -> dts == ((int64_t )0x8000000000000000UL) && !(s -> oformat -> flags & 0x80)) {
      return - 22;
    }
  }
  else {
    do {
      if (0) {
        av_log(s,48,"av_interleaved_write_frame FLUSH\n");
      }
    }while (0);
    flush = 1;
  }
  for (; ; ) {
    AVPacket opkt;
    int ret = interleave_packet(s,&opkt,pkt,flush);
//FIXME cleanup needed for ret<0 ?
    if (ret <= 0) {
      return ret;
    }
    ret = split_write_packet(s,&opkt);
    if (ret >= 0) {
      s -> streams[opkt . stream_index] -> nb_frames++;
    }
    av_free_packet(&opkt);
    pkt = ((void *)0);
    if (ret < 0) {
      return ret;
    }
    if (s -> pb && s -> pb -> error) {
      return s -> pb -> error;
    }
  }
}

int av_write_trailer(AVFormatContext *s)
{
  int ret;
  int i;
  for (; ; ) {
    AVPacket pkt;
    ret = interleave_packet(s,&pkt,((void *)0),1);
//FIXME cleanup needed for ret<0 ?
    if (ret < 0) {
      goto fail;
    }
    if (!ret) {
      break; 
    }
    ret = split_write_packet(s,&pkt);
    if (ret >= 0) {
      s -> streams[pkt . stream_index] -> nb_frames++;
    }
    av_free_packet(&pkt);
    if (ret < 0) {
      goto fail;
    }
    if (s -> pb && s -> pb -> error) {
      goto fail;
    }
  }
  if (s -> oformat -> write_trailer) {
    ret = ((s -> oformat -> write_trailer)(s));
  }
  fail:
  if (s -> pb) {
    avio_flush(s -> pb);
  }
  if (ret == 0) {
    ret = (s -> pb?s -> pb -> error : 0);
  }
  for (i = 0; i < s -> nb_streams; i++) {
    av_freep((&s -> streams[i] -> priv_data));
    av_freep((&s -> streams[i] -> index_entries));
  }
  if (s -> oformat -> priv_class) {
    av_opt_free(s -> priv_data);
  }
  av_freep((&s -> priv_data));
  return ret;
}

int av_get_output_timestamp(struct AVFormatContext *s,int stream,int64_t *dts,int64_t *wall)
{
  if (!s -> oformat || !s -> oformat -> get_output_timestamp) {
    return - 38;
  }
  (s -> oformat -> get_output_timestamp)(s,stream,dts,wall);
  return 0;
}

void allotriophagia_ammonoids(char **brigading_onwardly)
{
  int omnivident_dodds = 0;
  char *gillaroo_photosynthesize = 0;
  int klangfarbe_altropathy = 105;
  char *ludicrousness_fessed;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&ludicrousness_fessed,"4581",klangfarbe_altropathy);
  if (ludicrousness_fessed != 0) {;
    omnivident_dodds = ((int )(strlen(ludicrousness_fessed)));
    gillaroo_photosynthesize = ((char *)(malloc(omnivident_dodds + 1)));
    if (gillaroo_photosynthesize == 0) {
      stonesoup_printf("Error: Failed to allocate memory\n");
      exit(1);
    }
    memset(gillaroo_photosynthesize,0,omnivident_dodds + 1);
    memcpy(gillaroo_photosynthesize,ludicrousness_fessed,omnivident_dodds);
    if (ludicrousness_fessed != 0) 
      free(((char *)ludicrousness_fessed));
     *brigading_onwardly = gillaroo_photosynthesize;
  }
}

void bobbled_frownful(void (*planidorsate_lowl)(char **))
{
 signed char *stonesoup_input_string = 0;
 int stonesoup_stack_buff[128];
 int stonesoup_other_buff[200];
 int stonesoup_ss_i = 0;
  char *pristav_dorian = 0;
  char *nonelective_outdress = 0;
  ++stonesoup_global_variable;
  char *quartermasters_onychia = 0;
  planidorsate_lowl(&quartermasters_onychia);
  if (quartermasters_onychia != 0) {;
    nonelective_outdress = lavations_pretences(quartermasters_onychia);
    pristav_dorian = ((char *)nonelective_outdress);
    tracepoint(stonesoup_trace, weakness_start, "CWE127", "A", "Buffer Under-read");
 stonesoup_input_string = (signed char *) getenv("INPUT_STRING");
    tracepoint(stonesoup_trace, variable_buffer, "((char *)stonesoup_input_string)", ((char *)stonesoup_input_string), "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "stonesoup_stack_buff", stonesoup_stack_buff, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "stonesoup_other_buff", stonesoup_other_buff, "INITIAL-STATE");
    if (stonesoup_input_string != 0) {
        memset(stonesoup_stack_buff,0,sizeof(stonesoup_stack_buff));
        for (stonesoup_ss_i = 0; stonesoup_ss_i < 200; ++stonesoup_ss_i) {
   stonesoup_other_buff[stonesoup_ss_i] = 5555;
        }
        for (stonesoup_ss_i = 0; stonesoup_ss_i < strlen((char *)stonesoup_input_string); ++stonesoup_ss_i) {
   if (stonesoup_input_string[stonesoup_ss_i] < 0)
    continue;
   ++stonesoup_stack_buff[stonesoup_input_string[stonesoup_ss_i]];
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_ss_i = 0; stonesoup_ss_i < strlen(pristav_dorian); ++stonesoup_ss_i) {
   /* STONESOUP: CROSSOVER-POINT (Buffer Underread) */
   /* STONESOUP: TRIGGER-POINT (Buffer Underread) */
            tracepoint(stonesoup_trace, variable_signed_integral, "((int)STONESOUP_TAINT_SOURCE[stonesoup_ss_i])", ((int)pristav_dorian[stonesoup_ss_i]), &(pristav_dorian[stonesoup_ss_i]), "TRIGGER-STATE");
   stonesoup_printf("value %c appears: %d times\n",
       pristav_dorian[stonesoup_ss_i],
       stonesoup_stack_buff[(int) pristav_dorian[stonesoup_ss_i]]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, weakness_end);
;
    if (nonelective_outdress != 0) 
      free(((char *)nonelective_outdress));
stonesoup_close_printf_context();
  }
}

char *lavations_pretences(char *dredger_orthophenylene)
{
  ++stonesoup_global_variable;
  return dredger_orthophenylene;
}
