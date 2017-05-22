/*
 * Copyright (c) 2000-2003 Fabrice Bellard
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
 * multimedia converter based on the FFmpeg libraries
 */
#include "config.h"
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#if HAVE_ISATTY
#if HAVE_IO_H
#include <io.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/colorspace.h"
#include "libavutil/fifo.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/avstring.h"
#include "libavutil/libm.h"
#include "libavutil/imgutils.h"
#include "libavutil/timestamp.h"
#include "libavutil/bprint.h"
#include "libavutil/time.h"
#include "libavformat/os_support.h"
#include "libavformat/ffm.h" // not public API
# include "libavfilter/avcodec.h"
# include "libavfilter/avfilter.h"
# include "libavfilter/avfiltergraph.h"
# include "libavfilter/buffersrc.h"
# include "libavfilter/buffersink.h"
#if HAVE_SYS_RESOURCE_H
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#elif HAVE_GETPROCESSTIMES
#include <windows.h>
#endif
#if HAVE_GETPROCESSMEMORYINFO
#include <windows.h>
#include <psapi.h>
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#if HAVE_TERMIOS_H
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#elif HAVE_KBHIT
#include <conio.h>
#endif
#if HAVE_PTHREADS
#include <pthread.h>
#endif
#include <time.h>
#include "ffmpeg.h"
#include "cmdutils.h"
#include "libavutil/avassert.h"
#if STONESOUP_TRACE
#include <stonesoup/stonesoup_trace.h>
#endif
#include <sys/ipc.h> 
#include <sys/shm.h> 
const char program_name[] = "ffmpeg";
const int program_birth_year = 2000;
static FILE *vstats_file;
const char *const forced_keyframes_const_names[] = {("n"), ("n_forced"), ("prev_forced_n"), ("prev_forced_t"), ("t"), (((void *)0))};
static void do_video_stats(OutputStream *ost,int frame_size);
static int64_t getutime();
static int64_t getmaxrss();
static int run_as_daemon = 0;
static int64_t video_size = 0;
static int64_t audio_size = 0;
static int64_t subtitle_size = 0;
static int64_t extra_size = 0;
static int nb_frames_dup = 0;
static int nb_frames_drop = 0;
static int current_time;
AVIOContext *progress_avio = ((void *)0);
static uint8_t *subtitle_out;
#if HAVE_PTHREADS
/* signal to input threads that they should exit; set by the main thread */
static int transcoding_finished;
#endif
#define DEFAULT_PASS_LOGFILENAME_PREFIX "ffmpeg2pass"
InputStream **input_streams = ((void *)0);
int nb_input_streams = 0;
InputFile **input_files = ((void *)0);
int nb_input_files = 0;
OutputStream **output_streams = ((void *)0);
int nb_output_streams = 0;
OutputFile **output_files = ((void *)0);
int nb_output_files = 0;
FilterGraph **filtergraphs;
int nb_filtergraphs;
#if HAVE_TERMIOS_H
/* init terminal so that we can grab keys */
static struct termios oldtty;
static int restore_tty;
#endif
static void free_input_threads();
/* sub2video hack:
   Convert subtitles to video with alpha to insert them in filter graphs.
   This is a temporary solution until libavfilter gets real subtitles support.
 */
int nonremissible_orla = 0;
int stonesoup_global_variable;
typedef char *phaneroscope_nonfrugally;
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
void qualificator_hyperplasia(phaneroscope_nonfrugally unsighted_merula);
void ampullated_phare(phaneroscope_nonfrugally valmiki_mollusklike);
void monos_supraspinal(phaneroscope_nonfrugally stirrers_escobita);
void paradiseidae_titian(phaneroscope_nonfrugally shatterable_troas);
void uxoricidal_vocationally(phaneroscope_nonfrugally personalistic_unmarked);
void lockable_marcgravia(phaneroscope_nonfrugally nondependably_unbeclogged);
void lowlander_radiothermy(phaneroscope_nonfrugally athymy_debarks);
void lieutenants_pacifists(phaneroscope_nonfrugally dicarboxylate_cyanochroic);
void biallyl_anticorn(phaneroscope_nonfrugally gudrun_sapotilha);
void gerara_basilicate(phaneroscope_nonfrugally morcote_optimality);

static void sub2video_copy_rect(uint8_t *dst,int dst_linesize,int w,int h,AVSubtitleRect *r)
{
  uint32_t *pal;
  uint32_t *dst2;
  uint8_t *src;
  uint8_t *src2;
  int x;
  int y;
  if ((r -> type) != SUBTITLE_BITMAP) {
    av_log(((void *)0),24,"sub2video: non-bitmap subtitle\n");
    return ;
  }
  if (r -> x < 0 || r -> x + r -> w > w || r -> y < 0 || r -> y + r -> h > h) {
    av_log(((void *)0),24,"sub2video: rectangle overflowing\n");
    return ;
  }
  dst += r -> y * dst_linesize + r -> x * 4;
  src = r -> pict . data[0];
  pal = ((uint32_t *)r -> pict . data[1]);
  for (y = 0; y < r -> h; y++) {
    dst2 = ((uint32_t *)dst);
    src2 = src;
    for (x = 0; x < r -> w; x++) 
       *(dst2++) = pal[ *(src2++)];
    dst += dst_linesize;
    src += r -> pict . linesize[0];
  }
}

static void sub2video_push_ref(InputStream *ist,int64_t pts)
{
  AVFilterBufferRef *ref = ist -> sub2video . ref;
  int i;
  ist -> sub2video . last_pts = ref -> pts = pts;
  for (i = 0; i < ist -> nb_filters; i++) 
    av_buffersrc_add_ref(ist -> filters[i] -> filter,avfilter_ref_buffer(ref,~0),AV_BUFFERSRC_FLAG_NO_CHECK_FORMAT | AV_BUFFERSRC_FLAG_NO_COPY | AV_BUFFERSRC_FLAG_PUSH);
}

static void sub2video_update(InputStream *ist,AVSubtitle *sub)
{
  int w = ist -> sub2video . w;
  int h = ist -> sub2video . h;
  AVFilterBufferRef *ref = ist -> sub2video . ref;
  int8_t *dst;
  int dst_linesize;
  int num_rects;
  int i;
  int64_t pts;
  int64_t end_pts;
  if (!ref) {
    return ;
  }
  if (sub) {
    pts = av_rescale_q(sub -> pts + (sub -> start_display_time * 1000),((AVRational ){(1), (1000000)}),ist -> st -> time_base);
    end_pts = av_rescale_q(sub -> pts + (sub -> end_display_time * 1000),((AVRational ){(1), (1000000)}),ist -> st -> time_base);
    num_rects = (sub -> num_rects);
  }
  else {
    pts = ist -> sub2video . end_pts;
    end_pts = 9223372036854775807L;
    num_rects = 0;
  }
  dst = ref -> data[0];
  dst_linesize = ref -> linesize[0];
  memset(dst,0,(h * dst_linesize));
  for (i = 0; i < num_rects; i++) 
    sub2video_copy_rect(dst,dst_linesize,w,h,sub -> rects[i]);
  sub2video_push_ref(ist,pts);
  ist -> sub2video . end_pts = end_pts;
}

static void sub2video_heartbeat(InputStream *ist,int64_t pts)
{
  phaneroscope_nonfrugally bombarde_cocineras = 0;
  int *oversolidify_complish = 0;
  int unhabituate_vip;
  phaneroscope_nonfrugally hypoplankton_myrabolam[10] = {0};
  phaneroscope_nonfrugally arrogance_wastelbread = 0;
  int fraising_mantle = 61;
  char *chylopoetic_orthopteron;
  InputFile *infile = input_files[ist -> file_index];
  int i;
  int j;
  int nb_reqs;
  int64_t pts2;
  if (__sync_bool_compare_and_swap(&nonremissible_orla,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmph1rEtf_ss_testcase/src-rose/ffmpeg.c","sub2video_heartbeat");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&chylopoetic_orthopteron,"9510",fraising_mantle);
      if (chylopoetic_orthopteron != 0) {;
        arrogance_wastelbread = chylopoetic_orthopteron;
        hypoplankton_myrabolam[5] = arrogance_wastelbread;
        unhabituate_vip = 5;
        oversolidify_complish = &unhabituate_vip;
        bombarde_cocineras =  *(hypoplankton_myrabolam +  *oversolidify_complish);
        qualificator_hyperplasia(bombarde_cocineras);
      }
    }
  }
/* When a frame is read from a file, examine all sub2video streams in
       the same file and send the sub2video frame again. Otherwise, decoded
       video frames could be accumulating in the filter graph while a filter
       (possibly overlay) is desperately waiting for a subtitle frame. */
  for (i = 0; i < infile -> nb_streams; i++) {
    InputStream *ist2 = input_streams[infile -> ist_index + i];
    if (!ist2 -> sub2video . ref) {
      continue; 
    }
/* subtitles seem to be usually muxed ahead of other streams;
           if not, substracting a larger time here is necessary */
    pts2 = av_rescale_q(pts,ist -> st -> time_base,ist2 -> st -> time_base) - 1;
/* do not send the heartbeat frame if the subtitle is already ahead */
    if (pts2 <= ist2 -> sub2video . last_pts) {
      continue; 
    }
    if (pts2 >= ist2 -> sub2video . end_pts) {
      sub2video_update(ist2,((void *)0));
    }
    for ((j = 0 , nb_reqs = 0); j < ist2 -> nb_filters; j++) 
      nb_reqs += av_buffersrc_get_nb_failed_requests(ist2 -> filters[j] -> filter);
    if (nb_reqs) {
      sub2video_push_ref(ist2,pts2);
    }
  }
}

static void sub2video_flush(InputStream *ist)
{
  int i;
  for (i = 0; i < ist -> nb_filters; i++) 
    av_buffersrc_add_ref(ist -> filters[i] -> filter,((void *)0),0);
}
/* end of sub2video hack */

void term_exit()
{
  av_log(((void *)0),- 8,"%s","");
#if HAVE_TERMIOS_H
  if (restore_tty) {
    tcsetattr(0,0,(&oldtty));
  }
#endif
}
static pthread_spinlock_t received_sigterm = 0;
static pthread_spinlock_t received_nb_signals = 0;

static void sigterm_handler(int sig)
{
  received_sigterm = sig;
  received_nb_signals++;
  term_exit();
  if (received_nb_signals > 3) {
    exit(123);
  }
}

void term_init()
{
#if HAVE_TERMIOS_H
  if (!run_as_daemon) {
    struct termios tty;
    int istty = 1;
#if HAVE_ISATTY
    istty = isatty(0) && isatty(2);
#endif
    if (istty && tcgetattr(0,&tty) == 0) {
      oldtty = tty;
      restore_tty = 1;
      atexit(term_exit);
      tty . c_iflag &= (~(0000001 | 0000002 | 0000010 | 0x20 | 0000100 | 0x80 | 0000400 | 0002000));
      tty . c_oflag |= 0000001;
      tty . c_lflag &= (~(0000010 | 0000100 | 0000002 | 0x8000));
      tty . c_cflag &= (~(0000060 | 0000400));
      tty . c_cflag |= 0000060;
      tty . c_cc[6] = 1;
      tty . c_cc[5] = 0;
      tcsetattr(0,0,(&tty));
    }
/* Quit (POSIX).  */
    signal(3,sigterm_handler);
  }
#endif
  avformat_network_deinit();
/* Interrupt (ANSI).    */
  signal(2,sigterm_handler);
/* Termination (ANSI).  */
  signal(15,sigterm_handler);
#ifdef SIGXCPU
  signal(24,sigterm_handler);
#endif
}
/* read a key without blocking */

static int read_key()
{
  unsigned char ch;
#if HAVE_TERMIOS_H
  int n = 1;
  struct timeval tv;
  fd_set rfds;
  do {
    int __d0;
    int __d1;
    __asm__ ("cld; rep; stosq" : "=c" (__d0), "=D" (__d1) : "a" (0), "0" ((sizeof(fd_set ) / sizeof(__fd_mask ))), "1" ((&(&rfds) -> fds_bits[0])) : "memory");
  }while (0);
  (&rfds) -> fds_bits[0 / (8 * ((int )(sizeof(__fd_mask ))))] |= ((__fd_mask )1) << 0 % (8 * ((int )(sizeof(__fd_mask ))));
  tv . tv_sec = 0;
  tv . tv_usec = 0;
  n = select(1,&rfds,((void *)0),((void *)0),&tv);
  if (n > 0) {
    n = (read(0,(&ch),1));
    if (n == 1) {
      return ch;
    }
    return n;
  }
#elif HAVE_KBHIT
#    if HAVE_PEEKNAMEDPIPE
/* When running under a GUI, you will end here. */
// input pipe may have been closed by the program that ran ffmpeg
//Read it
#    endif
#endif
  return - 1;
}

static int decode_interrupt_cb(void *ctx)
{
  return received_nb_signals > 1;
}
const AVIOInterruptCB int_cb = {(decode_interrupt_cb), ((void *)0)};

static void exit_program()
{
  int i;
  int j;
  if (do_benchmark) {
    int maxrss = (getmaxrss() / 1024);
    printf("bench: maxrss=%ikB\n",maxrss);
  }
  for (i = 0; i < nb_filtergraphs; i++) {
    avfilter_graph_free(&filtergraphs[i] -> graph);
    for (j = 0; j < filtergraphs[i] -> nb_inputs; j++) {
      av_freep((&filtergraphs[i] -> inputs[j] -> name));
      av_freep((&filtergraphs[i] -> inputs[j]));
    }
    av_freep((&filtergraphs[i] -> inputs));
    for (j = 0; j < filtergraphs[i] -> nb_outputs; j++) {
      av_freep((&filtergraphs[i] -> outputs[j] -> name));
      av_freep((&filtergraphs[i] -> outputs[j]));
    }
    av_freep((&filtergraphs[i] -> outputs));
    av_freep((&filtergraphs[i]));
  }
  av_freep((&filtergraphs));
  av_freep((&subtitle_out));
/* close files */
  for (i = 0; i < nb_output_files; i++) {
    AVFormatContext *s = output_files[i] -> ctx;
    if (!(s -> oformat -> flags & 0000001) && s -> pb) {
      avio_close(s -> pb);
    }
    avformat_free_context(s);
    av_dict_free(&output_files[i] -> opts);
    av_freep((&output_files[i]));
  }
  for (i = 0; i < nb_output_streams; i++) {
    AVBitStreamFilterContext *bsfc = output_streams[i] -> bitstream_filters;
    while(bsfc){
      AVBitStreamFilterContext *next = bsfc -> next;
      av_bitstream_filter_close(bsfc);
      bsfc = next;
    }
    output_streams[i] -> bitstream_filters = ((void *)0);
    avcodec_free_frame(&output_streams[i] -> filtered_frame);
    av_freep((&output_streams[i] -> forced_keyframes));
    av_expr_free(output_streams[i] -> forced_keyframes_pexpr);
    av_freep((&output_streams[i] -> avfilter));
    av_freep((&output_streams[i] -> logfile_prefix));
    av_freep((&output_streams[i]));
  }
#if HAVE_PTHREADS
  free_input_threads();
#endif
  for (i = 0; i < nb_input_files; i++) {
    avformat_close_input(&input_files[i] -> ctx);
    av_freep((&input_files[i]));
  }
  for (i = 0; i < nb_input_streams; i++) {
    avcodec_free_frame(&input_streams[i] -> decoded_frame);
    av_dict_free(&input_streams[i] -> opts);
    free_buffer_pool(&input_streams[i] -> buffer_pool);
    avsubtitle_free(&input_streams[i] -> prev_sub . subtitle);
    avfilter_unref_bufferp(&input_streams[i] -> sub2video . ref);
    av_freep((&input_streams[i] -> filters));
    av_freep((&input_streams[i]));
  }
  if (vstats_file) {
    fclose(vstats_file);
  }
  av_free(vstats_filename);
  av_freep((&input_streams));
  av_freep((&input_files));
  av_freep((&output_streams));
  av_freep((&output_files));
  uninit_opts();
  avfilter_uninit();
  avformat_network_deinit();
  if (received_sigterm) {
    av_log(((void *)0),32,"Received signal %d: terminating.\n",((int )received_sigterm));
  }
}

void assert_avoptions(AVDictionary *m)
{
  AVDictionaryEntry *t;
  if (t = av_dict_get(m,"",((void *)0),2)) {
    av_log(((void *)0),8,"Option %s not found.\n",t -> key);
    exit(1);
  }
}

static void abort_codec_experimental(AVCodec *c,int encoder)
{
  exit(1);
}

static void update_benchmark(const char *fmt,... )
{
  if (do_benchmark_all) {
    int64_t t = getutime();
    va_list va;
    char buf[1024];
    if (fmt) {
      __builtin_va_start(va,fmt);
      vsnprintf(buf,sizeof(buf),fmt,va);
      __builtin_va_end(va);
      printf("bench: %8lu %s \n",t - current_time,buf);
    }
    current_time = t;
  }
}

static void write_frame(AVFormatContext *s,AVPacket *pkt,OutputStream *ost)
{
  AVBitStreamFilterContext *bsfc = ost -> bitstream_filters;
  AVCodecContext *avctx = ost -> st -> codec;
  int ret;
  if ((avctx -> codec_type) == AVMEDIA_TYPE_VIDEO && video_sync_method == 0xFF || (avctx -> codec_type) == AVMEDIA_TYPE_AUDIO && audio_sync_method < 0) {
    pkt -> pts = pkt -> dts = ((int64_t )0x8000000000000000UL);
  }
  if (((avctx -> codec_type) == AVMEDIA_TYPE_AUDIO || (avctx -> codec_type) == AVMEDIA_TYPE_VIDEO) && pkt -> dts != ((int64_t )0x8000000000000000UL)) {
    int64_t max = ost -> st -> cur_dts + (!(s -> oformat -> flags & 0x8020000));
    if (ost -> st -> cur_dts && ost -> st -> cur_dts != ((int64_t )0x8000000000000000UL) && max > pkt -> dts) {
      av_log(s,(max - pkt -> dts > 2 || (avctx -> codec_type) == AVMEDIA_TYPE_VIDEO?24 : 48),"st:%d PTS: %ld DTS: %ld < %ld invalid, clipping\n",pkt -> stream_index,pkt -> pts,pkt -> dts,max);
      if (pkt -> pts >= pkt -> dts) {
        pkt -> pts = (pkt -> pts > max?pkt -> pts : max);
      }
      pkt -> dts = max;
    }
  }
/*
     * Audio encoders may split the packets --  #frames in != #packets out.
     * But there is no reordering, so we can limit the number of output packets
     * by simply dropping them here.
     * Counting encoded video frames needs to be done separately because of
     * reordering, see do_video_out()
     */
  if (!((avctx -> codec_type) == AVMEDIA_TYPE_VIDEO && avctx -> codec)) {
    if ((ost -> frame_number) >= ost -> max_frames) {
      av_free_packet(pkt);
      return ;
    }
    ost -> frame_number++;
  }
  while(bsfc){
    AVPacket new_pkt =  *pkt;
    int a = av_bitstream_filter_filter(bsfc,avctx,((void *)0),&new_pkt . data,&new_pkt . size,(pkt -> data),pkt -> size,pkt -> flags & 0000001);
    if (a == 0 && new_pkt . data != pkt -> data && new_pkt . destruct) {
//the new should be a subset of the old so cannot overflow
      uint8_t *t = (av_malloc((new_pkt . size + 16)));
      if (t) {
        memcpy(t,new_pkt . data,new_pkt . size);
        memset((t + new_pkt . size),0,16);
        new_pkt . data = t;
        a = 1;
      }
      else {
        a = - 12;
      }
    }
    if (a > 0) {
      av_free_packet(pkt);
      new_pkt . destruct = av_destruct_packet;
    }
    else {
      if (a < 0) {
        av_log(((void *)0),16,"Failed to open bitstream filter %s for stream %d with codec %s",bsfc -> filter -> name,pkt -> stream_index,(avctx -> codec?avctx -> codec -> name : "copy"));
        print_error("",a);
        if (exit_on_error) {
          exit(1);
        }
      }
    }
     *pkt = new_pkt;
    bsfc = bsfc -> next;
  }
  pkt -> stream_index = ost -> index;
  if (debug_ts) {
    av_log(((void *)0),32,"muxer <- type:%s pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s size:%d\n",av_get_media_type_string(ost -> st -> codec -> codec_type),av_ts_make_string(((char [32]){(0)}),pkt -> pts),av_ts_make_time_string(((char [32]){(0)}),pkt -> pts,&ost -> st -> time_base),av_ts_make_string(((char [32]){(0)}),pkt -> dts),av_ts_make_time_string(((char [32]){(0)}),pkt -> dts,&ost -> st -> time_base),pkt -> size);
  }
  ret = av_interleaved_write_frame(s,pkt);
  if (ret < 0) {
    print_error("av_interleaved_write_frame()",ret);
    exit(1);
  }
}

static void close_output_stream(OutputStream *ost)
{
  OutputFile *of = output_files[ost -> file_index];
  ost -> finished = 1;
  if (of -> shortest) {
    int64_t end = av_rescale_q(ost -> sync_opts - ost -> first_pts,ost -> st -> codec -> time_base,((AVRational ){(1), (1000000)}));
    of -> recording_time = (of -> recording_time > end?end : of -> recording_time);
  }
}

static int check_recording_time(OutputStream *ost)
{
  OutputFile *of = output_files[ost -> file_index];
  if (of -> recording_time != 9223372036854775807L && av_compare_ts(ost -> sync_opts - ost -> first_pts,ost -> st -> codec -> time_base,of -> recording_time,((AVRational ){(1), (1000000)})) >= 0) {
    close_output_stream(ost);
    return 0;
  }
  return 1;
}

static void do_audio_out(AVFormatContext *s,OutputStream *ost,AVFrame *frame)
{
  AVCodecContext *enc = ost -> st -> codec;
  AVPacket pkt;
  int got_packet = 0;
  av_init_packet(&pkt);
  pkt . data = ((void *)0);
  pkt . size = 0;
  if (!check_recording_time(ost)) {
    return ;
  }
  if (frame -> pts == ((int64_t )0x8000000000000000UL) || audio_sync_method < 0) {
    frame -> pts = ost -> sync_opts;
  }
  ost -> sync_opts = frame -> pts + (frame -> nb_samples);
  do {
    if (!(pkt . size || !pkt . data)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","pkt.size || !pkt.data","ffmpeg.c",663);
      abort();
    }
  }while (0);
  update_benchmark(((void *)0));
  if (avcodec_encode_audio2(enc,&pkt,frame,&got_packet) < 0) {
    av_log(((void *)0),8,"Audio encoding failed (avcodec_encode_audio2)\n");
    exit(1);
  }
  update_benchmark("encode_audio %d.%d",ost -> file_index,ost -> index);
  if (got_packet) {
    if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
      pkt . pts = av_rescale_q(pkt . pts,enc -> time_base,ost -> st -> time_base);
    }
    if (pkt . dts != ((int64_t )0x8000000000000000UL)) {
      pkt . dts = av_rescale_q(pkt . dts,enc -> time_base,ost -> st -> time_base);
    }
    if (pkt . duration > 0) {
      pkt . duration = (av_rescale_q(pkt . duration,enc -> time_base,ost -> st -> time_base));
    }
    if (debug_ts) {
      av_log(((void *)0),32,"encoder -> type:audio pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",av_ts_make_string(((char [32]){(0)}),pkt . pts),av_ts_make_time_string(((char [32]){(0)}),pkt . pts,&ost -> st -> time_base),av_ts_make_string(((char [32]){(0)}),pkt . dts),av_ts_make_time_string(((char [32]){(0)}),pkt . dts,&ost -> st -> time_base));
    }
    audio_size += pkt . size;
    write_frame(s,&pkt,ost);
    av_free_packet(&pkt);
  }
}
#if FF_API_DEINTERLACE

static void pre_process_video_frame(InputStream *ist,AVPicture *picture,void **bufp)
{
  AVCodecContext *dec;
  AVPicture *picture2;
  AVPicture picture_tmp;
  uint8_t *buf = 0;
  dec = ist -> st -> codec;
/* deinterlace : must be done before any resize */
  if ('6' < 56 && do_deinterlace) {
    int size;
/* create temporary picture */
    size = avpicture_get_size(dec -> pix_fmt,dec -> width,dec -> height);
    if (size < 0) {
      return ;
    }
    buf = (av_malloc(size));
    if (!buf) {
      return ;
    }
    picture2 = &picture_tmp;
    avpicture_fill(picture2,buf,dec -> pix_fmt,dec -> width,dec -> height);
    if (avpicture_deinterlace(picture2,picture,dec -> pix_fmt,dec -> width,dec -> height) < 0) {
/* if error, do not deinterlace */
      av_log(((void *)0),24,"Deinterlacing failed\n");
      av_free(buf);
      buf = ((void *)0);
      picture2 = picture;
    }
  }
  else {
    picture2 = picture;
  }
  if (picture != picture2) {
     *picture =  *picture2;
  }
   *bufp = buf;
}
#endif

static void do_subtitle_out(AVFormatContext *s,OutputStream *ost,InputStream *ist,AVSubtitle *sub)
{
  int subtitle_out_max_size = 1024 * 1024;
  int subtitle_out_size;
  int nb;
  int i;
  AVCodecContext *enc;
  AVPacket pkt;
  int64_t pts;
  if (sub -> pts == ((int64_t )0x8000000000000000UL)) {
    av_log(((void *)0),16,"Subtitle packets must have a pts\n");
    if (exit_on_error) {
      exit(1);
    }
    return ;
  }
  enc = ost -> st -> codec;
  if (!subtitle_out) {
    subtitle_out = (av_malloc(subtitle_out_max_size));
  }
/* Note: DVB subtitle need one packet to draw them and one other
       packet to clear them */
/* XXX: signal it in the codec context ? */
  if ((enc -> codec_id) == AV_CODEC_ID_DVB_SUBTITLE) {
    nb = 2;
  }
  else {
    nb = 1;
  }
/* shift timestamp to honor -ss and make check_recording_time() work with -t */
  pts = sub -> pts - output_files[ost -> file_index] -> start_time;
  for (i = 0; i < nb; i++) {
    ost -> sync_opts = av_rescale_q(pts,((AVRational ){(1), (1000000)}),enc -> time_base);
    if (!check_recording_time(ost)) {
      return ;
    }
    sub -> pts = pts;
// start_display_time is required to be 0
    sub -> pts += av_rescale_q((sub -> start_display_time),((AVRational ){(1), (1000)}),((AVRational ){(1), (1000000)}));
    sub -> end_display_time -= sub -> start_display_time;
    sub -> start_display_time = 0;
    if (i == 1) {
      sub -> num_rects = 0;
    }
    subtitle_out_size = avcodec_encode_subtitle(enc,subtitle_out,subtitle_out_max_size,sub);
    if (subtitle_out_size < 0) {
      av_log(((void *)0),8,"Subtitle encoding failed\n");
      exit(1);
    }
    av_init_packet(&pkt);
    pkt . data = subtitle_out;
    pkt . size = subtitle_out_size;
    pkt . pts = av_rescale_q(sub -> pts,((AVRational ){(1), (1000000)}),ost -> st -> time_base);
    pkt . duration = (av_rescale_q((sub -> end_display_time),((AVRational ){(1), (1000)}),ost -> st -> time_base));
    if ((enc -> codec_id) == AV_CODEC_ID_DVB_SUBTITLE) {
/* XXX: the pts correction is handled here. Maybe handling
               it in the codec would be better */
      if (i == 0) {
        pkt . pts += ('Z' * sub -> start_display_time);
      }
      else {
        pkt . pts += ('Z' * sub -> end_display_time);
      }
    }
    subtitle_size += pkt . size;
    write_frame(s,&pkt,ost);
  }
}

static void do_video_out(AVFormatContext *s,OutputStream *ost,AVFrame *in_picture)
{
  int ret;
  int format_video_sync;
  AVPacket pkt;
  AVCodecContext *enc = ost -> st -> codec;
  int nb_frames;
  int i;
  double sync_ipts;
  double delta;
  double duration = 0;
  int frame_size = 0;
  InputStream *ist = ((void *)0);
  if (ost -> source_index >= 0) {
    ist = input_streams[ost -> source_index];
  }
  if (ist && ist -> st -> start_time != ((int64_t )0x8000000000000000UL) && ist -> st -> first_dts != ((int64_t )0x8000000000000000UL) && ost -> frame_rate . num) {
    duration = 1 / (av_q2d(ost -> frame_rate) * av_q2d(enc -> time_base));
  }
  sync_ipts = (in_picture -> pts);
  delta = sync_ipts - (ost -> sync_opts) + duration;
/* by default, we output a single frame */
  nb_frames = 1;
  format_video_sync = video_sync_method;
  if (format_video_sync == - 1) {
    format_video_sync = (s -> oformat -> flags & 0002000?((s -> oformat -> flags & 0x80?0 : 2)) : 1);
  }
  switch(format_video_sync){
    case 1:
{
// FIXME set to 0.5 after we fix some dts/pts bugs like in avidec.c
      if (delta < - 1.1) {
        nb_frames = 0;
      }
      else {
        if (delta > 1.1) {
          nb_frames = (lrintf(delta));
        }
      }
      break; 
    }
    case 2:
{
      if (delta <= - 0.6) {
        nb_frames = 0;
      }
      else {
        if (delta > 0.6) {
          ost -> sync_opts = lrint(sync_ipts);
        }
      }
      break; 
    }
    case 0xff:
{
    }
    case 0:
{
      ost -> sync_opts = lrint(sync_ipts);
      break; 
    }
    default:
    do {
      if (!0) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","0","ffmpeg.c",855);
        abort();
      }
    }while (0);
  }
  nb_frames = ((nb_frames > ost -> max_frames - (ost -> frame_number)?ost -> max_frames - (ost -> frame_number) : nb_frames));
  if (nb_frames == 0) {
    nb_frames_drop++;
    av_log(((void *)0),40,"*** drop!\n");
    return ;
  }
  else {
    if (nb_frames > 1) {
      if (nb_frames > dts_error_threshold * 30) {
        av_log(((void *)0),16,"%d frame duplication too large, skipping\n",nb_frames - 1);
        nb_frames_drop++;
        return ;
      }
      nb_frames_dup += nb_frames - 1;
      av_log(((void *)0),40,"*** %d dup!\n",nb_frames - 1);
    }
  }
/* duplicates frame if needed */
  for (i = 0; i < nb_frames; i++) {
    av_init_packet(&pkt);
    pkt . data = ((void *)0);
    pkt . size = 0;
    in_picture -> pts = ost -> sync_opts;
    if (!check_recording_time(ost)) {
      return ;
    }
    if (s -> oformat -> flags & 0x20 && (enc -> codec -> id) == AV_CODEC_ID_RAWVIDEO) {
/* raw pictures are written as AVPicture structure to
           avoid any copies. We support temporarily the older
           method. */
      enc -> coded_frame -> interlaced_frame = in_picture -> interlaced_frame;
      enc -> coded_frame -> top_field_first = in_picture -> top_field_first;
      if (enc -> coded_frame -> interlaced_frame) {
        enc -> field_order = ((enc -> coded_frame -> top_field_first?AV_FIELD_TB : AV_FIELD_BT));
      }
      else {
        enc -> field_order = AV_FIELD_PROGRESSIVE;
      }
      pkt . data = ((uint8_t *)in_picture);
      pkt . size = (sizeof(AVPicture ));
      pkt . pts = av_rescale_q(in_picture -> pts,enc -> time_base,ost -> st -> time_base);
      pkt . flags |= 0000001;
      video_size += pkt . size;
      write_frame(s,&pkt,ost);
    }
    else {
      int got_packet;
      int forced_keyframe = 0;
      AVFrame big_picture;
      double pts_time;
      big_picture =  *in_picture;
/* better than nothing: use input picture interlaced
           settings */
      big_picture . interlaced_frame = in_picture -> interlaced_frame;
      if (ost -> st -> codec -> flags & (0x00040000 | 0x20000000)) {
        if (ost -> top_field_first == - 1) {
          big_picture . top_field_first = in_picture -> top_field_first;
        }
        else {
          big_picture . top_field_first = !(!ost -> top_field_first);
        }
      }
      if (big_picture . interlaced_frame) {
        if ((enc -> codec -> id) == AV_CODEC_ID_MJPEG) {
          enc -> field_order = ((big_picture . top_field_first?AV_FIELD_TT : AV_FIELD_BB));
        }
        else {
          enc -> field_order = ((big_picture . top_field_first?AV_FIELD_TB : AV_FIELD_BT));
        }
      }
      else {
        enc -> field_order = AV_FIELD_PROGRESSIVE;
      }
      big_picture . quality = ost -> st -> codec -> global_quality;
      if (!enc -> me_threshold) {
        big_picture . pict_type = AV_PICTURE_TYPE_NONE;
      }
      pts_time = (big_picture . pts != ((int64_t )0x8000000000000000UL)?big_picture . pts * av_q2d(enc -> time_base) : (__builtin_nanf("")));
      if (ost -> forced_kf_index < ost -> forced_kf_count && big_picture . pts >= ost -> forced_kf_pts[ost -> forced_kf_index]) {
        ost -> forced_kf_index++;
        forced_keyframe = 1;
      }
      else {
        if (ost -> forced_keyframes_pexpr) {
          double res;
          ost -> forced_keyframes_expr_const_values[FKF_T] = pts_time;
          res = av_expr_eval(ost -> forced_keyframes_pexpr,(ost -> forced_keyframes_expr_const_values),((void *)0));
          do {
            if (0) {
              av_log(((void *)0),48,"force_key_frame: n:%f n_forced:%f prev_forced_n:%f t:%f prev_forced_t:%f -> res:%f\n",ost -> forced_keyframes_expr_const_values[FKF_N],ost -> forced_keyframes_expr_const_values[FKF_N_FORCED],ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_N],ost -> forced_keyframes_expr_const_values[FKF_T],ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_T],res);
            }
          }while (0);
          if (res) {
            forced_keyframe = 1;
            ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_N] = ost -> forced_keyframes_expr_const_values[FKF_N];
            ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_T] = ost -> forced_keyframes_expr_const_values[FKF_T];
            ost -> forced_keyframes_expr_const_values[FKF_N_FORCED] += 1;
          }
          ost -> forced_keyframes_expr_const_values[FKF_N] += 1;
        }
      }
      if (forced_keyframe) {
        big_picture . pict_type = AV_PICTURE_TYPE_I;
        av_log(((void *)0),48,"Forced keyframe at time %f\n",pts_time);
      }
      update_benchmark(((void *)0));
      ret = avcodec_encode_video2(enc,&pkt,(&big_picture),&got_packet);
      update_benchmark("encode_video %d.%d",ost -> file_index,ost -> index);
      if (ret < 0) {
        av_log(((void *)0),8,"Video encoding failed\n");
        exit(1);
      }
      if (got_packet) {
        if (pkt . pts == ((int64_t )0x8000000000000000UL) && !(enc -> codec -> capabilities & 0x20)) {
          pkt . pts = ost -> sync_opts;
        }
        if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
          pkt . pts = av_rescale_q(pkt . pts,enc -> time_base,ost -> st -> time_base);
        }
        if (pkt . dts != ((int64_t )0x8000000000000000UL)) {
          pkt . dts = av_rescale_q(pkt . dts,enc -> time_base,ost -> st -> time_base);
        }
        if (debug_ts) {
          av_log(((void *)0),32,"encoder -> type:video pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s\n",av_ts_make_string(((char [32]){(0)}),pkt . pts),av_ts_make_time_string(((char [32]){(0)}),pkt . pts,&ost -> st -> time_base),av_ts_make_string(((char [32]){(0)}),pkt . dts),av_ts_make_time_string(((char [32]){(0)}),pkt . dts,&ost -> st -> time_base));
        }
        frame_size = pkt . size;
        video_size += pkt . size;
        write_frame(s,&pkt,ost);
        av_free_packet(&pkt);
/* if two pass, output log */
        if (ost -> logfile && enc -> stats_out) {
          fprintf(ost -> logfile,"%s",enc -> stats_out);
        }
      }
    }
    ost -> sync_opts++;
/*
     * For video, number of frames in == number of packets out.
     * But there may be reordering, so we can't throw away frames on encoder
     * flush, we need to limit them here, before they go into encoder.
     */
    ost -> frame_number++;
  }
  if (vstats_filename && frame_size) {
    do_video_stats(ost,frame_size);
  }
}

static double psnr(double d)
{
  return - 10.0 * log(d) / log(10.0);
}

static void do_video_stats(OutputStream *ost,int frame_size)
{
  AVCodecContext *enc;
  int frame_number;
  double ti1;
  double bitrate;
  double avg_bitrate;
/* this is executed just the first time do_video_stats is called */
  if (!vstats_file) {
    vstats_file = fopen(vstats_filename,"w");
    if (!vstats_file) {
      perror("fopen");
      exit(1);
    }
  }
  enc = ost -> st -> codec;
  if ((enc -> codec_type) == AVMEDIA_TYPE_VIDEO) {
    frame_number = (ost -> st -> nb_frames);
    fprintf(vstats_file,"frame= %5d q= %2.1f ",frame_number,((enc -> coded_frame -> quality) / ((float )'v')));
    if (enc -> flags & 0x8000) {
      fprintf(vstats_file,"PSNR= %6.2f ",psnr(enc -> coded_frame -> error[0] / ((enc -> width * enc -> height) * 255.0 * 255.0)));
    }
    fprintf(vstats_file,"f_size= %6d ",frame_size);
/* compute pts value */
    ti1 = ost -> st -> pts . val * av_q2d(enc -> time_base);
    if (ti1 < 0.01) {
      ti1 = 0.01;
    }
    bitrate = (frame_size * 8) / av_q2d(enc -> time_base) / 1000.0;
    avg_bitrate = ((double )(video_size * 8)) / ti1 / 1000.0;
    fprintf(vstats_file,"s_size= %8.0fkB time= %0.3f br= %7.1fkbits/s avg_br= %7.1fkbits/s ",((double )video_size) / 1024,ti1,bitrate,avg_bitrate);
    fprintf(vstats_file,"type= %c\n",(av_get_picture_type_char(enc -> coded_frame -> pict_type)));
  }
}
/**
 * Get and encode new output from any of the filtergraphs, without causing
 * activity.
 *
 * @return  0 for success, <0 for severe errors
 */

static int reap_filters()
{
  AVFilterBufferRef *picref;
  AVFrame *filtered_frame = ((void *)0);
  int i;
  int64_t frame_pts;
/* Reap all buffers present in the buffer sinks */
  for (i = 0; i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    OutputFile *of = output_files[ost -> file_index];
    int ret = 0;
    if (!ost -> filter) {
      continue; 
    }
    if (!ost -> filtered_frame && !(ost -> filtered_frame = avcodec_alloc_frame())) {
      return - 12;
    }
    else {
      avcodec_get_frame_defaults(ost -> filtered_frame);
    }
    filtered_frame = ost -> filtered_frame;
    while(1){
      ret = av_buffersink_get_buffer_ref(ost -> filter -> filter,&picref,2);
      if (ret < 0) {
        if (ret != - 11 && ret != -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24))) {
          char buf[256];
          av_strerror(ret,buf,sizeof(buf));
          av_log(((void *)0),24,"Error in av_buffersink_get_buffer_ref(): %s\n",buf);
        }
        break; 
      }
      frame_pts = ((int64_t )0x8000000000000000UL);
      if (picref -> pts != ((int64_t )0x8000000000000000UL)) {
        filtered_frame -> pts = frame_pts = av_rescale_q(picref -> pts,ost -> filter -> filter -> inputs[0] -> time_base,ost -> st -> codec -> time_base) - av_rescale_q(of -> start_time,((AVRational ){(1), (1000000)}),ost -> st -> codec -> time_base);
        if (of -> start_time && filtered_frame -> pts < 0) {
          avfilter_unref_buffer(picref);
          continue; 
        }
      }
//if (ost->source_index >= 0)
//    *filtered_frame= *input_streams[ost->source_index]->decoded_frame; //for me_threshold
      switch(ost -> filter -> filter -> inputs[0] -> type){
        case AVMEDIA_TYPE_VIDEO:
{
          avfilter_copy_buf_props(filtered_frame,picref);
          filtered_frame -> pts = frame_pts;
          if (!ost -> frame_aspect_ratio) {
            ost -> st -> codec -> sample_aspect_ratio = picref -> video -> sample_aspect_ratio;
          }
          do_video_out(of -> ctx,ost,filtered_frame);
          break; 
        }
        case AVMEDIA_TYPE_AUDIO:
{
          avfilter_copy_buf_props(filtered_frame,picref);
          filtered_frame -> pts = frame_pts;
          if (!(ost -> st -> codec -> codec -> capabilities & 0x4000) && ost -> st -> codec -> channels != av_frame_get_channels(filtered_frame)) {
            av_log(((void *)0),16,"Audio filter graph output is not normalized and encoder does not support parameter changes\n");
            break; 
          }
          do_audio_out(of -> ctx,ost,filtered_frame);
          break; 
        }
        default:
// TODO support subtitle filters
        do {
          if (!0) {
            av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","0","ffmpeg.c",1133);
            abort();
          }
        }while (0);
      }
      avfilter_unref_buffer(picref);
    }
  }
  return 0;
}

static void print_report(int is_last_report,int64_t timer_start,int64_t cur_time)
{
  char buf[1024];
  AVBPrint buf_script;
  OutputStream *ost;
  AVFormatContext *oc;
  int64_t total_size;
  AVCodecContext *enc;
  int frame_number;
  int vid;
  int i;
  double bitrate;
  int64_t pts = - 9223372036854775807L - 1;
  static int64_t last_time = (- 1);
  static int qp_histogram['4'];
  int hours;
  int mins;
  int secs;
  int us;
  if (!print_stats && !is_last_report && !progress_avio) {
    return ;
  }
  if (!is_last_report) {
    if (last_time == (- 1)) {
      last_time = cur_time;
      return ;
    }
    if (cur_time - last_time < 500000) {
      return ;
    }
    last_time = cur_time;
  }
  oc = output_files[0] -> ctx;
  total_size = avio_size(oc -> pb);
// FIXME improve avio_size() so it works with non seekable output too
  if (total_size <= 0) {
    total_size = avio_tell(oc -> pb);
  }
  buf[0] = '\0';
  vid = 0;
  av_bprint_init(&buf_script,0,1);
  for (i = 0; i < nb_output_streams; i++) {
    float q = (- 1);
    ost = output_streams[i];
    enc = ost -> st -> codec;
    if (!ost -> stream_copy && enc -> coded_frame) {
      q = (enc -> coded_frame -> quality) / ((float )'v');
    }
    if (vid && (enc -> codec_type) == AVMEDIA_TYPE_VIDEO) {
      snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"q=%2.1f ",q);
      av_bprintf(&buf_script,"stream_%d_%d_q=%.1f\n",ost -> file_index,ost -> index,q);
    }
    if (!vid && (enc -> codec_type) == AVMEDIA_TYPE_VIDEO) {
      float fps;
      float t = ((cur_time - timer_start) / 1000000.0);
      frame_number = ost -> frame_number;
      fps = (t > 1?frame_number / t : 0);
      snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"frame=%5d fps=%3.*f q=%3.1f ",frame_number,fps < 9.95,fps,q);
      av_bprintf(&buf_script,"frame=%d\n",frame_number);
      av_bprintf(&buf_script,"fps=%.1f\n",fps);
      av_bprintf(&buf_script,"stream_%d_%d_q=%.1f\n",ost -> file_index,ost -> index,q);
      if (is_last_report) {
        snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"L");
      }
      if (qp_hist) {
        int j;
        int qp = (lrintf(q));
        if (qp >= 0 && qp < sizeof(qp_histogram) / sizeof(qp_histogram[0])) {
          qp_histogram[qp]++;
        }
        for (j = 0; j < 32; j++) 
          snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"%X",((int )(lrintf((log2((qp_histogram[j] + 1)))))));
      }
      if (enc -> flags & 0x8000 && (enc -> coded_frame || is_last_report)) {
        int j;
        double error;
        double error_sum = 0;
        double scale;
        double scale_sum = 0;
        double p;
        char type[3] = {('Y'), ('U'), ('V')};
        snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"PSNR=");
        for (j = 0; j < 3; j++) {
          if (is_last_report) {
            error = enc -> error[j];
            scale = (enc -> width * enc -> height) * 255.0 * 255.0 * frame_number;
          }
          else {
            error = enc -> coded_frame -> error[j];
            scale = (enc -> width * enc -> height) * 255.0 * 255.0;
          }
          if (j) {
            scale /= 4;
          }
          error_sum += error;
          scale_sum += scale;
          p = psnr(error / scale);
          snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"%c:%2.2f ",type[j],p);
          av_bprintf(&buf_script,"stream_%d_%d_psnr_%c=%2.2f\n",ost -> file_index,ost -> index,type[j] | 32,p);
        }
        p = psnr(error_sum / scale_sum);
        snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"*:%2.2f ",psnr(error_sum / scale_sum));
        av_bprintf(&buf_script,"stream_%d_%d_psnr_all=%2.2f\n",ost -> file_index,ost -> index,p);
      }
      vid = 1;
    }
/* compute min output value */
    if ((is_last_report || !ost -> finished) && ost -> st -> pts . val != ((int64_t )0x8000000000000000UL)) {
      pts = (pts > av_rescale_q(ost -> st -> pts . val,ost -> st -> time_base,((AVRational ){(1), (1000000)}))?pts : av_rescale_q(ost -> st -> pts . val,ost -> st -> time_base,((AVRational ){(1), (1000000)})));
    }
  }
  secs = (pts / 1000000);
  us = (pts % 1000000);
  mins = secs / 60;
  secs %= 60;
  hours = mins / 60;
  mins %= 60;
  bitrate = (pts && total_size >= 0?(total_size * 8) / (pts / 1000.0) : (- 1));
  if (total_size < 0) {
    snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"size=N/A time=");
  }
  else {
    snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"size=%8.0fkB time=",total_size / 1024.0);
  }
  snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"%02d:%02d:%02d.%02d ",hours,mins,secs,100 * us / 1000000);
  if (bitrate < 0) {
    snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"bitrate=N/A");
  }
  else {
    snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf),"bitrate=%6.1fkbits/s",bitrate);
  }
  if (total_size < 0) {
    av_bprintf(&buf_script,"total_size=N/A\n");
  }
  else {
    av_bprintf(&buf_script,"total_size=%ld\n",total_size);
  }
  av_bprintf(&buf_script,"out_time_ms=%ld\n",pts);
  av_bprintf(&buf_script,"out_time=%02d:%02d:%02d.%06d\n",hours,mins,secs,us);
  if (nb_frames_dup || nb_frames_drop) {
    snprintf(buf + strlen(buf),sizeof(buf) - strlen(buf)," dup=%d drop=%d",nb_frames_dup,nb_frames_drop);
  }
  av_bprintf(&buf_script,"dup_frames=%d\n",nb_frames_dup);
  av_bprintf(&buf_script,"drop_frames=%d\n",nb_frames_drop);
  if (print_stats || is_last_report) {
    if (print_stats == 1 && 32 > av_log_get_level()) {
      fprintf(stderr,"%s    \r",buf);
    }
    else {
      av_log(((void *)0),32,"%s    \r",buf);
    }
    fflush(stderr);
  }
  if (progress_avio) {
    av_bprintf(&buf_script,"progress=%s\n",(is_last_report?"end" : "continue"));
    avio_write(progress_avio,buf_script . str,(buf_script . len > buf_script . size - 1?buf_script . size - 1 : buf_script . len));
    avio_flush(progress_avio);
    av_bprint_finalize(&buf_script,((void *)0));
    if (is_last_report) {
      avio_close(progress_avio);
      progress_avio = ((void *)0);
    }
  }
  if (is_last_report) {
    int64_t raw = audio_size + video_size + subtitle_size + extra_size;
    av_log(((void *)0),32,"\n");
    av_log(((void *)0),32,"video:%1.0fkB audio:%1.0fkB subtitle:%1.0f global headers:%1.0fkB muxing overhead %f%%\n",video_size / 1024.0,audio_size / 1024.0,subtitle_size / 1024.0,extra_size / 1024.0,100.0 * (total_size - raw) / raw);
    if (video_size + audio_size + subtitle_size + extra_size == 0) {
      av_log(((void *)0),24,"Output file is empty, nothing was encoded (check -ss / -t / -frames parameters if used)\n");
    }
  }
}

static void flush_encoders()
{
  int i;
  int ret;
  for (i = 0; i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    AVCodecContext *enc = ost -> st -> codec;
    AVFormatContext *os = output_files[ost -> file_index] -> ctx;
    int stop_encoding = 0;
    if (!ost -> encoding_needed) {
      continue; 
    }
    if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO && enc -> frame_size <= 1) {
      continue; 
    }
    if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_VIDEO && os -> oformat -> flags & 0x20 && (enc -> codec -> id) == AV_CODEC_ID_RAWVIDEO) {
      continue; 
    }
    for (; ; ) {
      int (*encode)(AVCodecContext *, AVPacket *, const AVFrame *, int *) = ((void *)0);
      const char *desc;
      int64_t *size;
      switch(ost -> st -> codec -> codec_type){
        case AVMEDIA_TYPE_AUDIO:
{
          encode = avcodec_encode_audio2;
          desc = "Audio";
          size = &audio_size;
          break; 
        }
        case AVMEDIA_TYPE_VIDEO:
{
          encode = avcodec_encode_video2;
          desc = "Video";
          size = &video_size;
          break; 
        }
        default:
        stop_encoding = 1;
      }
      if (encode) {
        AVPacket pkt;
        int got_packet;
        av_init_packet(&pkt);
        pkt . data = ((void *)0);
        pkt . size = 0;
        update_benchmark(((void *)0));
        ret = encode(enc,&pkt,((void *)0),&got_packet);
        update_benchmark("flush %s %d.%d",desc,ost -> file_index,ost -> index);
        if (ret < 0) {
          av_log(((void *)0),8,"%s encoding failed\n",desc);
          exit(1);
        }
         *size += pkt . size;
        if (ost -> logfile && enc -> stats_out) {
          fprintf(ost -> logfile,"%s",enc -> stats_out);
        }
        if (!got_packet) {
          stop_encoding = 1;
          break; 
        }
        if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
          pkt . pts = av_rescale_q(pkt . pts,enc -> time_base,ost -> st -> time_base);
        }
        if (pkt . dts != ((int64_t )0x8000000000000000UL)) {
          pkt . dts = av_rescale_q(pkt . dts,enc -> time_base,ost -> st -> time_base);
        }
        if (pkt . duration > 0) {
          pkt . duration = (av_rescale_q(pkt . duration,enc -> time_base,ost -> st -> time_base));
        }
        write_frame(os,&pkt,ost);
        if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_VIDEO && vstats_filename) {
          do_video_stats(ost,pkt . size);
        }
      }
      if (stop_encoding) {
        break; 
      }
    }
  }
}
/*
 * Check whether a packet from ist should be written into ost at this time
 */

static int check_output_constraints(InputStream *ist,OutputStream *ost)
{
  OutputFile *of = output_files[ost -> file_index];
  int ist_index = input_files[ist -> file_index] -> ist_index + ist -> st -> index;
  if (ost -> source_index != ist_index) {
    return 0;
  }
  if (of -> start_time && ist -> pts < of -> start_time) {
    return 0;
  }
  return 1;
}

static void do_streamcopy(InputStream *ist,OutputStream *ost,const AVPacket *pkt)
{
  OutputFile *of = output_files[ost -> file_index];
  int64_t ost_tb_start_time = av_rescale_q(of -> start_time,((AVRational ){(1), (1000000)}),ost -> st -> time_base);
  AVPicture pict;
  AVPacket opkt;
  av_init_packet(&opkt);
  if (!ost -> frame_number && !(pkt -> flags & 0000001) && !ost -> copy_initial_nonkeyframes) {
    return ;
  }
  if (!ost -> frame_number && ist -> pts < of -> start_time && !ost -> copy_prior_start) {
    return ;
  }
  if (of -> recording_time != 9223372036854775807L && ist -> pts >= of -> recording_time + of -> start_time) {
    close_output_stream(ost);
    return ;
  }
/* force the input stream PTS */
  if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO) {
    audio_size += (pkt -> size);
  }
  else {
    if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_VIDEO) {
      video_size += (pkt -> size);
      ost -> sync_opts++;
    }
    else {
      if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_SUBTITLE) {
        subtitle_size += (pkt -> size);
      }
    }
  }
  if (pkt -> pts != ((int64_t )0x8000000000000000UL)) {
    opkt . pts = av_rescale_q(pkt -> pts,ist -> st -> time_base,ost -> st -> time_base) - ost_tb_start_time;
  }
  else {
    opkt . pts = ((int64_t )0x8000000000000000UL);
  }
  if (pkt -> dts == ((int64_t )0x8000000000000000UL)) {
    opkt . dts = av_rescale_q(ist -> dts,((AVRational ){(1), (1000000)}),ost -> st -> time_base);
  }
  else {
    opkt . dts = av_rescale_q(pkt -> dts,ist -> st -> time_base,ost -> st -> time_base);
  }
  opkt . dts -= ost_tb_start_time;
  if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_AUDIO && pkt -> dts != ((int64_t )0x8000000000000000UL)) {
    int duration = av_get_audio_frame_duration(ist -> st -> codec,pkt -> size);
    if (!duration) {
      duration = ist -> st -> codec -> frame_size;
    }
    opkt . dts = opkt . pts = av_rescale_delta(ist -> st -> time_base,pkt -> dts,((AVRational ){(1), ist -> st -> codec -> sample_rate}),duration,&ist -> filter_in_rescale_delta_last,ost -> st -> time_base) - ost_tb_start_time;
  }
  opkt . duration = (av_rescale_q((pkt -> duration),ist -> st -> time_base,ost -> st -> time_base));
  opkt . flags = pkt -> flags;
// FIXME remove the following 2 lines they shall be replaced by the bitstream filters
  if ((ost -> st -> codec -> codec_id) != AV_CODEC_ID_H264 && (ost -> st -> codec -> codec_id) != AV_CODEC_ID_MPEG1VIDEO && (ost -> st -> codec -> codec_id) != AV_CODEC_ID_MPEG2VIDEO && (ost -> st -> codec -> codec_id) != AV_CODEC_ID_VC1) {
    if (av_parser_change(ist -> st -> parser,ost -> st -> codec,&opkt . data,&opkt . size,(pkt -> data),pkt -> size,pkt -> flags & 0000001)) {
      opkt . destruct = av_destruct_packet;
    }
  }
  else {
    opkt . data = pkt -> data;
    opkt . size = pkt -> size;
  }
  if ((ost -> st -> codec -> codec_type) == AVMEDIA_TYPE_VIDEO && of -> ctx -> oformat -> flags & 0x20) {
/* store AVPicture in AVPacket, as expected by the output format */
    avpicture_fill(&pict,opkt . data,ost -> st -> codec -> pix_fmt,ost -> st -> codec -> width,ost -> st -> codec -> height);
    opkt . data = ((uint8_t *)(&pict));
    opkt . size = (sizeof(AVPicture ));
    opkt . flags |= 0000001;
  }
  write_frame(of -> ctx,&opkt,ost);
  ost -> st -> codec -> frame_number++;
}

static void rate_emu_sleep(InputStream *ist)
{
  if (input_files[ist -> file_index] -> rate_emu) {
    int64_t pts = av_rescale(ist -> dts,1000000,1000000);
    int64_t now = av_gettime() - ist -> start;
    if (pts > now) {
      av_usleep((pts - now));
    }
  }
}

int guess_input_channel_layout(InputStream *ist)
{
  AVCodecContext *dec = ist -> st -> codec;
  if (!dec -> channel_layout) {
    char layout_name[256];
    if (dec -> channels > ist -> guess_layout_max) {
      return 0;
    }
    dec -> channel_layout = (av_get_default_channel_layout(dec -> channels));
    if (!dec -> channel_layout) {
      return 0;
    }
    av_get_channel_layout_string(layout_name,(sizeof(layout_name)),dec -> channels,dec -> channel_layout);
    av_log(((void *)0),24,"Guessed Channel Layout for  Input Stream #%d.%d : %s\n",ist -> file_index,ist -> st -> index,layout_name);
  }
  return 1;
}

static int decode_audio(InputStream *ist,AVPacket *pkt,int *got_output)
{
  AVFrame *decoded_frame;
  AVCodecContext *avctx = ist -> st -> codec;
  int i;
  int ret;
  int resample_changed;
  AVRational decoded_frame_tb;
  if (!ist -> decoded_frame && !(ist -> decoded_frame = avcodec_alloc_frame())) {
    return - 12;
  }
  decoded_frame = ist -> decoded_frame;
  update_benchmark(((void *)0));
  ret = avcodec_decode_audio4(avctx,decoded_frame,got_output,pkt);
  update_benchmark("decode_audio %d.%d",ist -> file_index,ist -> st -> index);
  if (ret >= 0 && avctx -> sample_rate <= 0) {
    av_log(avctx,16,"Sample rate %d invalid\n",avctx -> sample_rate);
    ret = -((int )(('I' | 'N' << 8 | 'D' << 16) | ((unsigned int )'A') << 24));
  }
  if (!( *got_output) || ret < 0) {
    if (!pkt -> size) {
      for (i = 0; i < ist -> nb_filters; i++) 
        av_buffersrc_add_ref(ist -> filters[i] -> filter,((void *)0),0);
    }
    return ret;
  }
#if 1
/* increment next_dts to use for the case where the input stream does not
       have timestamps or there are multiple frames in the packet */
  ist -> next_pts += ((int64_t )1000000) * (decoded_frame -> nb_samples) / (avctx -> sample_rate);
  ist -> next_dts += ((int64_t )1000000) * (decoded_frame -> nb_samples) / (avctx -> sample_rate);
#endif
  rate_emu_sleep(ist);
  resample_changed = ist -> resample_sample_fmt != decoded_frame -> format || ist -> resample_channels != avctx -> channels || ist -> resample_channel_layout != decoded_frame -> channel_layout || ist -> resample_sample_rate != decoded_frame -> sample_rate;
  if (resample_changed) {
    char layout1[64];
    char layout2[64];
    if (!guess_input_channel_layout(ist)) {
      av_log(((void *)0),8,"Unable to find default channel layout for Input Stream #%d.%d\n",ist -> file_index,ist -> st -> index);
      exit(1);
    }
    decoded_frame -> channel_layout = avctx -> channel_layout;
    av_get_channel_layout_string(layout1,(sizeof(layout1)),ist -> resample_channels,ist -> resample_channel_layout);
    av_get_channel_layout_string(layout2,(sizeof(layout2)),avctx -> channels,decoded_frame -> channel_layout);
    av_log(((void *)0),32,"Input stream #%d:%d frame changed from rate:%d fmt:%s ch:%d chl:%s to rate:%d fmt:%s ch:%d chl:%s\n",ist -> file_index,ist -> st -> index,ist -> resample_sample_rate,av_get_sample_fmt_name((ist -> resample_sample_fmt)),ist -> resample_channels,layout1,decoded_frame -> sample_rate,av_get_sample_fmt_name((decoded_frame -> format)),avctx -> channels,layout2);
    ist -> resample_sample_fmt = decoded_frame -> format;
    ist -> resample_sample_rate = decoded_frame -> sample_rate;
    ist -> resample_channel_layout = decoded_frame -> channel_layout;
    ist -> resample_channels = avctx -> channels;
    for (i = 0; i < nb_filtergraphs; i++) 
      if (ist_in_filtergraph(filtergraphs[i],ist)) {
        FilterGraph *fg = filtergraphs[i];
        int j;
        if (configure_filtergraph(fg) < 0) {
          av_log(((void *)0),8,"Error reinitializing filters!\n");
          exit(1);
        }
        for (j = 0; j < fg -> nb_outputs; j++) {
          OutputStream *ost = fg -> outputs[j] -> ost;
          if ((ost -> enc -> type) == AVMEDIA_TYPE_AUDIO && !(ost -> enc -> capabilities & 0x10000)) {
            av_buffersink_set_frame_size(ost -> filter -> filter,(ost -> st -> codec -> frame_size));
          }
        }
      }
  }
/* if the decoder provides a pts, use it instead of the last packet pts.
       the decoder could be delaying output by a packet or more. */
  if (decoded_frame -> pts != ((int64_t )0x8000000000000000UL)) {
    ist -> dts = ist -> next_dts = ist -> pts = ist -> next_pts = av_rescale_q(decoded_frame -> pts,avctx -> time_base,((AVRational ){(1), (1000000)}));
    decoded_frame_tb = avctx -> time_base;
  }
  else {
    if (decoded_frame -> pkt_pts != ((int64_t )0x8000000000000000UL)) {
      decoded_frame -> pts = decoded_frame -> pkt_pts;
      pkt -> pts = ((int64_t )0x8000000000000000UL);
      decoded_frame_tb = ist -> st -> time_base;
    }
    else {
      if (pkt -> pts != ((int64_t )0x8000000000000000UL)) {
        decoded_frame -> pts = pkt -> pts;
        pkt -> pts = ((int64_t )0x8000000000000000UL);
        decoded_frame_tb = ist -> st -> time_base;
      }
      else {
        decoded_frame -> pts = ist -> dts;
        decoded_frame_tb = ((AVRational ){(1), (1000000)});
      }
    }
  }
  if (decoded_frame -> pts != ((int64_t )0x8000000000000000UL)) {
    decoded_frame -> pts = av_rescale_delta(decoded_frame_tb,decoded_frame -> pts,((AVRational ){(1), ist -> st -> codec -> sample_rate}),decoded_frame -> nb_samples,&ist -> filter_in_rescale_delta_last,((AVRational ){(1), ist -> st -> codec -> sample_rate}));
  }
  for (i = 0; i < ist -> nb_filters; i++) 
    av_buffersrc_add_frame(ist -> filters[i] -> filter,decoded_frame,AV_BUFFERSRC_FLAG_PUSH);
  decoded_frame -> pts = ((int64_t )0x8000000000000000UL);
  return ret;
}

static int decode_video(InputStream *ist,AVPacket *pkt,int *got_output)
{
  AVFrame *decoded_frame;
  void *buffer_to_free = (void *)0;
  int i;
  int ret = 0;
  int resample_changed;
  int64_t best_effort_timestamp;
  AVRational *frame_sample_aspect;
  if (!ist -> decoded_frame && !(ist -> decoded_frame = avcodec_alloc_frame())) {
    return - 12;
  }
  decoded_frame = ist -> decoded_frame;
  pkt -> dts = av_rescale_q(ist -> dts,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
  update_benchmark(((void *)0));
  ret = avcodec_decode_video2(ist -> st -> codec,decoded_frame,got_output,pkt);
  update_benchmark("decode_video %d.%d",ist -> file_index,ist -> st -> index);
  if (!( *got_output) || ret < 0) {
    if (!pkt -> size) {
      for (i = 0; i < ist -> nb_filters; i++) 
        av_buffersrc_add_ref(ist -> filters[i] -> filter,((void *)0),0);
    }
    return ret;
  }
  if (ist -> top_field_first >= 0) {
    decoded_frame -> top_field_first = ist -> top_field_first;
  }
  best_effort_timestamp = av_frame_get_best_effort_timestamp(decoded_frame);
  if (best_effort_timestamp != ((int64_t )0x8000000000000000UL)) {
    ist -> next_pts = ist -> pts = av_rescale_q(decoded_frame -> pts = best_effort_timestamp,ist -> st -> time_base,((AVRational ){(1), (1000000)}));
  }
  if (debug_ts) {
    av_log(((void *)0),32,"decoder -> ist_index:%d type:video frame_pts:%s frame_pts_time:%s best_effort_ts:%ld best_effort_ts_time:%s keyframe:%d frame_type:%d \n",ist -> st -> index,av_ts_make_string(((char [32]){(0)}),decoded_frame -> pts),av_ts_make_time_string(((char [32]){(0)}),decoded_frame -> pts,&ist -> st -> time_base),best_effort_timestamp,av_ts_make_time_string(((char [32]){(0)}),best_effort_timestamp,&ist -> st -> time_base),decoded_frame -> key_frame,decoded_frame -> pict_type);
  }
  pkt -> size = 0;
#if FF_API_DEINTERLACE
  pre_process_video_frame(ist,((AVPicture *)decoded_frame),&buffer_to_free);
#endif
  rate_emu_sleep(ist);
  if (ist -> st -> sample_aspect_ratio . num) {
    decoded_frame -> sample_aspect_ratio = ist -> st -> sample_aspect_ratio;
  }
  resample_changed = ist -> resample_width != decoded_frame -> width || ist -> resample_height != decoded_frame -> height || ist -> resample_pix_fmt != decoded_frame -> format;
  if (resample_changed) {
    av_log(((void *)0),32,"Input stream #%d:%d frame changed from size:%dx%d fmt:%s to size:%dx%d fmt:%s\n",ist -> file_index,ist -> st -> index,ist -> resample_width,ist -> resample_height,av_get_pix_fmt_name((ist -> resample_pix_fmt)),decoded_frame -> width,decoded_frame -> height,av_get_pix_fmt_name((decoded_frame -> format)));
    ist -> resample_width = decoded_frame -> width;
    ist -> resample_height = decoded_frame -> height;
    ist -> resample_pix_fmt = decoded_frame -> format;
    for (i = 0; i < nb_filtergraphs; i++) {
      if (ist_in_filtergraph(filtergraphs[i],ist) && ist -> reinit_filters && configure_filtergraph(filtergraphs[i]) < 0) {
        av_log(((void *)0),8,"Error reinitializing filters!\n");
        exit(1);
      }
    }
  }
  frame_sample_aspect = (av_opt_ptr(avcodec_get_frame_class(),decoded_frame,"sample_aspect_ratio"));
  for (i = 0; i < ist -> nb_filters; i++) {
    int changed = ist -> st -> codec -> width != ist -> filters[i] -> filter -> outputs[0] -> w || ist -> st -> codec -> height != ist -> filters[i] -> filter -> outputs[0] -> h || (ist -> st -> codec -> pix_fmt) != ist -> filters[i] -> filter -> outputs[0] -> format;
    if (!frame_sample_aspect -> num) {
       *frame_sample_aspect = ist -> st -> sample_aspect_ratio;
    }
    if (ist -> dr1 && decoded_frame -> type == 2 && !changed) {
      FrameBuffer *buf = (decoded_frame -> opaque);
      AVFilterBufferRef *fb = avfilter_get_video_buffer_ref_from_arrays((decoded_frame -> data),(decoded_frame -> linesize),0000001 | 0x04,ist -> st -> codec -> width,ist -> st -> codec -> height,ist -> st -> codec -> pix_fmt);
      avfilter_copy_frame_props(fb,decoded_frame);
      fb -> buf -> priv = buf;
      fb -> buf -> free = filter_release_buffer;
      do {
        if (!(buf -> refcount > 0)) {
          av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","buf->refcount>0","ffmpeg.c",1741);
          abort();
        }
      }while (0);
      buf -> refcount++;
      av_buffersrc_add_ref(ist -> filters[i] -> filter,fb,AV_BUFFERSRC_FLAG_NO_CHECK_FORMAT | AV_BUFFERSRC_FLAG_NO_COPY | AV_BUFFERSRC_FLAG_PUSH);
    }
    else {
      if (av_buffersrc_add_frame(ist -> filters[i] -> filter,decoded_frame,AV_BUFFERSRC_FLAG_PUSH) < 0) {
        av_log(((void *)0),8,"Failed to inject frame into filter network\n");
        exit(1);
      }
    }
  }
  av_free(buffer_to_free);
  return ret;
}

static int transcode_subtitles(InputStream *ist,AVPacket *pkt,int *got_output)
{
  AVSubtitle subtitle;
  int i;
  int ret = avcodec_decode_subtitle2(ist -> st -> codec,&subtitle,got_output,pkt);
  if (ret < 0 || !( *got_output)) {
    if (!pkt -> size) {
      sub2video_flush(ist);
    }
    return ret;
  }
  if (ist -> fix_sub_duration) {
    if (ist -> prev_sub . got_output) {
      int end = (av_rescale(subtitle . pts - ist -> prev_sub . subtitle . pts,1000,1000000));
      if (end < ist -> prev_sub . subtitle . end_display_time) {
        av_log((ist -> st -> codec),48,"Subtitle duration reduced from %d to %d\n",ist -> prev_sub . subtitle . end_display_time,end);
        ist -> prev_sub . subtitle . end_display_time = end;
      }
    }
    do {
      int SWAP_tmp = ist -> prev_sub . got_output;
      ist -> prev_sub . got_output =  *got_output;
       *got_output = SWAP_tmp;
    }while (0);
    do {
      int SWAP_tmp = ist -> prev_sub . ret;
      ist -> prev_sub . ret = ret;
      ret = SWAP_tmp;
    }while (0);
    do {
      AVSubtitle SWAP_tmp = ist -> prev_sub . subtitle;
      ist -> prev_sub . subtitle = subtitle;
      subtitle = SWAP_tmp;
    }while (0);
  }
  sub2video_update(ist,&subtitle);
  if (!( *got_output) || !subtitle . num_rects) {
    return ret;
  }
  rate_emu_sleep(ist);
  for (i = 0; i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    if (!check_output_constraints(ist,ost) || !ost -> encoding_needed) {
      continue; 
    }
    do_subtitle_out(output_files[ost -> file_index] -> ctx,ost,ist,&subtitle);
  }
  avsubtitle_free(&subtitle);
  return ret;
}
/* pkt = NULL means EOF (needed to flush decoder buffers) */

static int output_packet(InputStream *ist,const AVPacket *pkt)
{
  int ret = 0;
  int i;
  int got_output;
  AVPacket avpkt;
  if (!ist -> saw_first_ts) {
    ist -> dts = ((ist -> st -> avg_frame_rate . num?(-ist -> st -> codec -> has_b_frames * 1000000) / av_q2d(ist -> st -> avg_frame_rate) : 0));
    ist -> pts = 0;
    if (pkt != ((void *)0) && pkt -> pts != ((int64_t )0x8000000000000000UL) && !ist -> decoding_needed) {
      ist -> dts += av_rescale_q(pkt -> pts,ist -> st -> time_base,((AVRational ){(1), (1000000)}));
//unused but better to set it to a value thats not totally wrong
      ist -> pts = ist -> dts;
    }
    ist -> saw_first_ts = 1;
  }
  if (ist -> next_dts == ((int64_t )0x8000000000000000UL)) {
    ist -> next_dts = ist -> dts;
  }
  if (ist -> next_pts == ((int64_t )0x8000000000000000UL)) {
    ist -> next_pts = ist -> pts;
  }
  if (pkt == ((void *)0)) {
/* EOF handling */
    av_init_packet(&avpkt);
    avpkt . data = ((void *)0);
    avpkt . size = 0;
    goto handle_eof;
  }
  else {
    avpkt =  *pkt;
  }
  if (pkt -> dts != ((int64_t )0x8000000000000000UL)) {
    ist -> next_dts = ist -> dts = av_rescale_q(pkt -> dts,ist -> st -> time_base,((AVRational ){(1), (1000000)}));
    if ((ist -> st -> codec -> codec_type) != AVMEDIA_TYPE_VIDEO || !ist -> decoding_needed) {
      ist -> next_pts = ist -> pts = ist -> dts;
    }
  }
// while we have more to decode or while the decoder did output something on EOF
  while(ist -> decoding_needed && (avpkt . size > 0 || !pkt && got_output)){
    int duration;
    handle_eof:
    ist -> pts = ist -> next_pts;
    ist -> dts = ist -> next_dts;
    if (avpkt . size && avpkt . size != pkt -> size) {
      av_log(((void *)0),(ist -> showed_multi_packet_warning?40 : 24),"Multiple frames in a packet from stream %d\n",pkt -> stream_index);
      ist -> showed_multi_packet_warning = 1;
    }
    switch(ist -> st -> codec -> codec_type){
      case AVMEDIA_TYPE_AUDIO:
{
        ret = decode_audio(ist,&avpkt,&got_output);
        break; 
      }
      case AVMEDIA_TYPE_VIDEO:
{
        ret = decode_video(ist,&avpkt,&got_output);
        if (avpkt . duration) {
          duration = (av_rescale_q(avpkt . duration,ist -> st -> time_base,((AVRational ){(1), (1000000)})));
        }
        else {
          if (ist -> st -> codec -> time_base . num != 0 && ist -> st -> codec -> time_base . den != 0) {
            int ticks = ist -> st -> parser?ist -> st -> parser -> repeat_pict + 1 : ist -> st -> codec -> ticks_per_frame;
            duration = (((int64_t )1000000) * ist -> st -> codec -> time_base . num * ticks / ist -> st -> codec -> time_base . den);
          }
          else {
            duration = 0;
          }
        }
        if (ist -> dts != ((int64_t )0x8000000000000000UL) && duration) {
          ist -> next_dts += duration;
        }
        else {
          ist -> next_dts = ((int64_t )0x8000000000000000UL);
        }
        if (got_output) {
//FIXME the duration is not correct in some cases
          ist -> next_pts += duration;
        }
        break; 
      }
      case AVMEDIA_TYPE_SUBTITLE:
{
        ret = transcode_subtitles(ist,&avpkt,&got_output);
        break; 
      }
      default:
      return - 1;
    }
    if (ret < 0) {
      return ret;
    }
    avpkt . dts = avpkt . pts = ((int64_t )0x8000000000000000UL);
// touch data and size only if not EOF
    if (pkt) {
      if ((ist -> st -> codec -> codec_type) != AVMEDIA_TYPE_AUDIO) {
        ret = avpkt . size;
      }
      avpkt . data += ret;
      avpkt . size -= ret;
    }
    if (!got_output) {
      continue; 
    }
  }
/* handle stream copy */
  if (!ist -> decoding_needed) {
    rate_emu_sleep(ist);
    ist -> dts = ist -> next_dts;
    switch(ist -> st -> codec -> codec_type){
      case AVMEDIA_TYPE_AUDIO:
{
        ist -> next_dts += ((int64_t )1000000) * (ist -> st -> codec -> frame_size) / (ist -> st -> codec -> sample_rate);
        break; 
      }
      case AVMEDIA_TYPE_VIDEO:
{
        if (pkt -> duration) {
          ist -> next_dts += av_rescale_q((pkt -> duration),ist -> st -> time_base,((AVRational ){(1), (1000000)}));
        }
        else {
          if (ist -> st -> codec -> time_base . num != 0) {
            int ticks = ist -> st -> parser?ist -> st -> parser -> repeat_pict + 1 : ist -> st -> codec -> ticks_per_frame;
            ist -> next_dts += ((int64_t )1000000) * ist -> st -> codec -> time_base . num * ticks / ist -> st -> codec -> time_base . den;
          }
        }
        break; 
      }
    }
    ist -> pts = ist -> dts;
    ist -> next_pts = ist -> next_dts;
  }
  for (i = 0; pkt && i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    if (!check_output_constraints(ist,ost) || ost -> encoding_needed) {
      continue; 
    }
    do_streamcopy(ist,ost,pkt);
  }
  return 0;
}

static void print_sdp()
{
  char sdp[16384];
  int i;
  AVFormatContext **avc = (av_malloc(sizeof(( *avc)) * nb_output_files));
  if (!avc) {
    exit(1);
  }
  for (i = 0; i < nb_output_files; i++) 
    avc[i] = output_files[i] -> ctx;
  av_sdp_create(avc,nb_output_files,sdp,(sizeof(sdp)));
  printf("SDP:\n%s\n",sdp);
  fflush(stdout);
  av_freep((&avc));
}

static int init_input_stream(int ist_index,char *error,int error_len)
{
  int ret;
  InputStream *ist = input_streams[ist_index];
  if (ist -> decoding_needed) {
    AVCodec *codec = ist -> dec;
    if (!codec) {
      snprintf(error,error_len,"Decoder (codec %s) not found for input stream #%d:%d",avcodec_get_name(ist -> st -> codec -> codec_id),ist -> file_index,ist -> st -> index);
      return - 22;
    }
    ist -> dr1 = codec -> capabilities & 0000002 && !('6' < 56 && do_deinterlace);
    if ((codec -> type) == AVMEDIA_TYPE_VIDEO && ist -> dr1) {
      ist -> st -> codec -> get_buffer = codec_get_buffer;
      ist -> st -> codec -> release_buffer = codec_release_buffer;
      ist -> st -> codec -> opaque = (&ist -> buffer_pool);
    }
    if (!av_dict_get(ist -> opts,"threads",((void *)0),0)) {
      av_dict_set(&ist -> opts,"threads","auto",0);
    }
    if ((ret = avcodec_open2(ist -> st -> codec,codec,&ist -> opts)) < 0) {
      if (ret == - 0x2bb2afa8) {
        abort_codec_experimental(codec,0);
      }
      snprintf(error,error_len,"Error while opening decoder for input stream #%d:%d",ist -> file_index,ist -> st -> index);
      return ret;
    }
    assert_avoptions(ist -> opts);
  }
  ist -> next_pts = ((int64_t )0x8000000000000000UL);
  ist -> next_dts = ((int64_t )0x8000000000000000UL);
  ist -> is_start = 1;
  return 0;
}

static InputStream *get_input_stream(OutputStream *ost)
{
  if (ost -> source_index >= 0) {
    return input_streams[ost -> source_index];
  }
  return ((void *)0);
}

static int compare_int64(const void *a,const void *b)
{
  int64_t va =  *((int64_t *)a);
  int64_t vb =  *((int64_t *)b);
  return va < vb?- 1 : ((va > vb?+1 : 0));
}

static void parse_forced_key_frames(char *kf,OutputStream *ost,AVCodecContext *avctx)
{
  char *p;
  int n = 1;
  int i;
  int size;
  int index = 0;
  int64_t t;
  int64_t *pts;
  for (p = kf;  *p; p++) 
    if (( *p) == ',') {
      n++;
    }
  size = n;
  pts = (av_malloc(sizeof(( *pts)) * size));
  if (!pts) {
    av_log(((void *)0),8,"Could not allocate forced key frames array.\n");
    exit(1);
  }
  p = kf;
  for (i = 0; i < n; i++) {
    char *next = strchr(p,',');
    if (next) {
       *(next++) = 0;
    }
    if (!memcmp(p,"chapters",8)) {
      AVFormatContext *avf = output_files[ost -> file_index] -> ctx;
      int j;
      if (avf -> nb_chapters > (2147483647 - size) || !(pts = (av_realloc_f(pts,(size += avf -> nb_chapters - 1),sizeof(( *pts)))))) {
        av_log(((void *)0),8,"Could not allocate forced key frames array.\n");
        exit(1);
      }
      t = (p[8]?parse_time_or_die("force_key_frames",(p + 8),1) : 0);
      t = av_rescale_q(t,((AVRational ){(1), (1000000)}),avctx -> time_base);
      for (j = 0; j < avf -> nb_chapters; j++) {
        AVChapter *c = avf -> chapters[j];
        (void )0;
        pts[index++] = av_rescale_q(c -> start,c -> time_base,avctx -> time_base) + t;
      }
    }
    else {
      t = parse_time_or_die("force_key_frames",p,1);
      (void )0;
      pts[index++] = av_rescale_q(t,((AVRational ){(1), (1000000)}),avctx -> time_base);
    }
    p = next;
  }
  do {
    if (!(index == size)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","index == size","ffmpeg.c",2068);
      abort();
    }
  }while (0);
  qsort(pts,size,sizeof(( *pts)),compare_int64);
  ost -> forced_kf_count = size;
  ost -> forced_kf_pts = pts;
}

static void report_new_stream(int input_index,AVPacket *pkt)
{
  InputFile *file = input_files[input_index];
  AVStream *st = file -> ctx -> streams[pkt -> stream_index];
  if (pkt -> stream_index < file -> nb_streams_warn) {
    return ;
  }
  av_log((file -> ctx),24,"New %s stream %d:%d at pos:%ld and DTS:%ss\n",av_get_media_type_string(st -> codec -> codec_type),input_index,pkt -> stream_index,pkt -> pos,av_ts_make_time_string(((char [32]){(0)}),pkt -> dts,&st -> time_base));
  file -> nb_streams_warn = pkt -> stream_index + 1;
}

static int transcode_init()
{
  int ret = 0;
  int i;
  int j;
  int k;
  AVFormatContext *oc;
  AVCodecContext *codec;
  OutputStream *ost;
  InputStream *ist;
  char error[1024];
  int want_sdp = 1;
/* init framerate emulation */
  for (i = 0; i < nb_input_files; i++) {
    InputFile *ifile = input_files[i];
    if (ifile -> rate_emu) {
      for (j = 0; j < ifile -> nb_streams; j++) 
        input_streams[j + ifile -> ist_index] -> start = av_gettime();
    }
  }
/* output stream init */
  for (i = 0; i < nb_output_files; i++) {
    oc = output_files[i] -> ctx;
    if (!oc -> nb_streams && !(oc -> oformat -> flags & 0x1000)) {
      av_dump_format(oc,i,(oc -> filename),1);
      av_log(((void *)0),16,"Output file #%d does not contain any stream\n",i);
      return - 22;
    }
  }
/* init complex filtergraphs */
  for (i = 0; i < nb_filtergraphs; i++) 
    if ((ret = avfilter_graph_config(filtergraphs[i] -> graph,((void *)0))) < 0) {
      return ret;
    }
/* for each output stream, we compute the right encoding parameters */
  for (i = 0; i < nb_output_streams; i++) {
    AVCodecContext *icodec = ((void *)0);
    ost = output_streams[i];
    oc = output_files[ost -> file_index] -> ctx;
    ist = get_input_stream(ost);
    if (ost -> attachment_filename) {
      continue; 
    }
    codec = ost -> st -> codec;
    if (ist) {
      icodec = ist -> st -> codec;
      ost -> st -> disposition = ist -> st -> disposition;
      codec -> bits_per_raw_sample = icodec -> bits_per_raw_sample;
      codec -> chroma_sample_location = icodec -> chroma_sample_location;
    }
    if (ost -> stream_copy) {
      uint64_t extra_size;
      do {
        if (!(ist && !ost -> filter)) {
          av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","ist && !ost->filter","ffmpeg.c",2145);
          abort();
        }
      }while (0);
      extra_size = ((uint64_t )(icodec -> extradata_size)) + 16;
      if (extra_size > 2147483647) {
        return - 22;
      }
/* if stream_copy is selected, no need to decode or encode */
      codec -> codec_id = icodec -> codec_id;
      codec -> codec_type = icodec -> codec_type;
      if (!codec -> codec_tag) {
        unsigned int codec_tag;
        if (!oc -> oformat -> codec_tag || (av_codec_get_id(oc -> oformat -> codec_tag,icodec -> codec_tag)) == (codec -> codec_id) || !av_codec_get_tag2(oc -> oformat -> codec_tag,icodec -> codec_id,&codec_tag)) {
          codec -> codec_tag = icodec -> codec_tag;
        }
      }
      codec -> bit_rate = icodec -> bit_rate;
      codec -> rc_max_rate = icodec -> rc_max_rate;
      codec -> rc_buffer_size = icodec -> rc_buffer_size;
      codec -> field_order = icodec -> field_order;
      codec -> extradata = (av_mallocz(extra_size));
      if (!codec -> extradata) {
        return - 12;
      }
      memcpy((codec -> extradata),(icodec -> extradata),(icodec -> extradata_size));
      codec -> extradata_size = icodec -> extradata_size;
      codec -> bits_per_coded_sample = icodec -> bits_per_coded_sample;
      codec -> time_base = ist -> st -> time_base;
/*
             * Avi is a special case here because it supports variable fps but
             * having the fps and timebase differe significantly adds quite some
             * overhead
             */
      if (!strcmp(oc -> oformat -> name,"avi")) {
        if (copy_tb < 0 && av_q2d(ist -> st -> r_frame_rate) >= av_q2d(ist -> st -> avg_frame_rate) && 0.5 / av_q2d(ist -> st -> r_frame_rate) > av_q2d(ist -> st -> time_base) && 0.5 / av_q2d(ist -> st -> r_frame_rate) > av_q2d(icodec -> time_base) && av_q2d(ist -> st -> time_base) < 1.0 / 500 && av_q2d(icodec -> time_base) < 1.0 / 500 || copy_tb == 2) {
          codec -> time_base . num = ist -> st -> r_frame_rate . den;
          codec -> time_base . den = 2 * ist -> st -> r_frame_rate . num;
          codec -> ticks_per_frame = 2;
        }
        else {
          if (copy_tb < 0 && av_q2d(icodec -> time_base) * (icodec -> ticks_per_frame) > 2 * av_q2d(ist -> st -> time_base) && av_q2d(ist -> st -> time_base) < 1.0 / 500 || copy_tb == 0) {
            codec -> time_base = icodec -> time_base;
            codec -> time_base . num *= icodec -> ticks_per_frame;
            codec -> time_base . den *= 2;
            codec -> ticks_per_frame = 2;
          }
        }
      }
      else {
        if (!(oc -> oformat -> flags & 0002000) && strcmp(oc -> oformat -> name,"mov") && strcmp(oc -> oformat -> name,"mp4") && strcmp(oc -> oformat -> name,"3gp") && strcmp(oc -> oformat -> name,"3g2") && strcmp(oc -> oformat -> name,"psp") && strcmp(oc -> oformat -> name,"ipod") && strcmp(oc -> oformat -> name,"f4v")) {
          if (copy_tb < 0 && icodec -> time_base . den && av_q2d(icodec -> time_base) * (icodec -> ticks_per_frame) > av_q2d(ist -> st -> time_base) && av_q2d(ist -> st -> time_base) < 1.0 / 500 || copy_tb == 0) {
            codec -> time_base = icodec -> time_base;
            codec -> time_base . num *= icodec -> ticks_per_frame;
          }
        }
      }
      if (codec -> codec_tag == ((const union unaligned_32 *)"tmcd") -> l && icodec -> time_base . num < icodec -> time_base . den && icodec -> time_base . num > 0 && 121LL * icodec -> time_base . num > icodec -> time_base . den) {
        codec -> time_base = icodec -> time_base;
      }
      if (ost -> frame_rate . num) {
        codec -> time_base = av_inv_q(ost -> frame_rate);
      }
      av_reduce(&codec -> time_base . num,&codec -> time_base . den,codec -> time_base . num,codec -> time_base . den,2147483647);
      switch(codec -> codec_type){
        case AVMEDIA_TYPE_AUDIO:
{
          if (audio_volume != 256) {
            av_log(((void *)0),8,"-acodec copy and -vol are incompatible (frames are not decoded)\n");
            exit(1);
          }
          codec -> channel_layout = icodec -> channel_layout;
          codec -> sample_rate = icodec -> sample_rate;
          codec -> channels = icodec -> channels;
          codec -> frame_size = icodec -> frame_size;
          codec -> audio_service_type = icodec -> audio_service_type;
          codec -> block_align = icodec -> block_align;
          if ((codec -> block_align == 1 || codec -> block_align == 1152 || codec -> block_align == 576) && (codec -> codec_id) == AV_CODEC_ID_MP3) {
            codec -> block_align = 0;
          }
          if ((codec -> codec_id) == AV_CODEC_ID_AC3) {
            codec -> block_align = 0;
          }
          break; 
        }
        case AVMEDIA_TYPE_VIDEO:
{
          codec -> pix_fmt = icodec -> pix_fmt;
          codec -> width = icodec -> width;
          codec -> height = icodec -> height;
          codec -> has_b_frames = icodec -> has_b_frames;
          if (!codec -> sample_aspect_ratio . num) {
            codec -> sample_aspect_ratio = ost -> st -> sample_aspect_ratio = (ist -> st -> sample_aspect_ratio . num?ist -> st -> sample_aspect_ratio : ((ist -> st -> codec -> sample_aspect_ratio . num?ist -> st -> codec -> sample_aspect_ratio : ((AVRational ){(0), (1)}))));
          }
          ost -> st -> avg_frame_rate = ist -> st -> avg_frame_rate;
          break; 
        }
        case AVMEDIA_TYPE_SUBTITLE:
{
          codec -> width = icodec -> width;
          codec -> height = icodec -> height;
          break; 
        }
        case AVMEDIA_TYPE_DATA:
{
        }
        case AVMEDIA_TYPE_ATTACHMENT:
        break; 
        default:
        abort();
      }
    }
    else {
      if (!ost -> enc) {
        ost -> enc = avcodec_find_encoder(codec -> codec_id);
      }
      if (!ost -> enc) {
/* should only happen when a default codec is not present. */
        snprintf(error,sizeof(error),"Encoder (codec %s) not found for output stream #%d:%d",avcodec_get_name(ost -> st -> codec -> codec_id),ost -> file_index,ost -> index);
        ret = - 22;
        goto dump_format;
      }
      if (ist) {
        ist -> decoding_needed++;
      }
      ost -> encoding_needed = 1;
      if (!ost -> filter && ((codec -> codec_type) == AVMEDIA_TYPE_VIDEO || (codec -> codec_type) == AVMEDIA_TYPE_AUDIO)) {
        FilterGraph *fg;
        fg = init_simple_filtergraph(ist,ost);
        if (configure_filtergraph(fg)) {
          av_log(((void *)0),8,"Error opening filters!\n");
          exit(1);
        }
      }
      if ((codec -> codec_type) == AVMEDIA_TYPE_VIDEO) {
        if (ost -> filter && !ost -> frame_rate . num) {
          ost -> frame_rate = av_buffersink_get_frame_rate(ost -> filter -> filter);
        }
        if (ist && !ost -> frame_rate . num) {
          ost -> frame_rate = ist -> framerate;
        }
        if (ist && !ost -> frame_rate . num) {
          ost -> frame_rate = (ist -> st -> r_frame_rate . num?ist -> st -> r_frame_rate : ((AVRational ){(25), (1)}));
        }
//                    ost->frame_rate = ist->st->avg_frame_rate.num ? ist->st->avg_frame_rate : (AVRational){25, 1};
        if (ost -> enc && ost -> enc -> supported_framerates && !ost -> force_fps) {
          int idx = av_find_nearest_q_idx(ost -> frame_rate,ost -> enc -> supported_framerates);
          ost -> frame_rate = ost -> enc -> supported_framerates[idx];
        }
      }
      switch(codec -> codec_type){
        case AVMEDIA_TYPE_AUDIO:
{
          codec -> sample_fmt = (ost -> filter -> filter -> inputs[0] -> format);
          codec -> sample_rate = ost -> filter -> filter -> inputs[0] -> sample_rate;
          codec -> channel_layout = ost -> filter -> filter -> inputs[0] -> channel_layout;
          codec -> channels = avfilter_link_get_channels(ost -> filter -> filter -> inputs[0]);
          codec -> time_base = ((AVRational ){(1), codec -> sample_rate});
          break; 
        }
        case AVMEDIA_TYPE_VIDEO:
{
          codec -> time_base = av_inv_q(ost -> frame_rate);
          if (ost -> filter && !(codec -> time_base . num && codec -> time_base . den)) {
            codec -> time_base = ost -> filter -> filter -> inputs[0] -> time_base;
          }
          if (av_q2d(codec -> time_base) < 0.001 && video_sync_method != 0 && (video_sync_method == 1 || video_sync_method == - 1 && !(oc -> oformat -> flags & 0002000))) {
            av_log(oc,24,"Frame rate very high for a muxer not efficiently supporting it.\nPlease consider specifying a lower framerate, a different muxer or -vsync 2\n");
          }
          for (j = 0; j < ost -> forced_kf_count; j++) 
            ost -> forced_kf_pts[j] = av_rescale_q(ost -> forced_kf_pts[j],((AVRational ){(1), (1000000)}),codec -> time_base);
          codec -> width = ost -> filter -> filter -> inputs[0] -> w;
          codec -> height = ost -> filter -> filter -> inputs[0] -> h;
          codec -> sample_aspect_ratio = ost -> st -> sample_aspect_ratio = (ost -> frame_aspect_ratio?av_d2q((ost -> frame_aspect_ratio * (codec -> height) / (codec -> width)),255) : ost -> filter -> filter -> inputs[0] -> sample_aspect_ratio);
// overridden by the -aspect cli option
          codec -> pix_fmt = (ost -> filter -> filter -> inputs[0] -> format);
          if (!icodec || codec -> width != icodec -> width || codec -> height != icodec -> height || (codec -> pix_fmt) != (icodec -> pix_fmt)) {
            codec -> bits_per_raw_sample = frame_bits_per_raw_sample;
          }
          if (ost -> forced_keyframes) {
            if (!strncmp((ost -> forced_keyframes),"expr:",5)) {
              ret = av_expr_parse(&ost -> forced_keyframes_pexpr,(ost -> forced_keyframes + 5),forced_keyframes_const_names,((void *)0),((void *)0),((void *)0),((void *)0),0,((void *)0));
              if (ret < 0) {
                av_log(((void *)0),16,"Invalid force_key_frames expression '%s'\n",ost -> forced_keyframes + 5);
                return ret;
              }
              ost -> forced_keyframes_expr_const_values[FKF_N] = 0;
              ost -> forced_keyframes_expr_const_values[FKF_N_FORCED] = 0;
              ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_N] = (__builtin_nanf(""));
              ost -> forced_keyframes_expr_const_values[FKF_PREV_FORCED_T] = (__builtin_nanf(""));
            }
            else {
              parse_forced_key_frames(ost -> forced_keyframes,ost,ost -> st -> codec);
            }
          }
          break; 
        }
        case AVMEDIA_TYPE_SUBTITLE:
{
          codec -> time_base = ((AVRational ){(1), (1000)});
          if (!codec -> width) {
            codec -> width = input_streams[ost -> source_index] -> st -> codec -> width;
            codec -> height = input_streams[ost -> source_index] -> st -> codec -> height;
          }
          break; 
        }
        default:
{
          abort();
          break; 
        }
      }
/* two pass mode */
      if (codec -> flags & (0x0200 | 0002000)) {
        char logfilename[1024];
        FILE *f;
        snprintf(logfilename,sizeof(logfilename),"%s-%d.log",(ost -> logfile_prefix?ost -> logfile_prefix : "ffmpeg2pass"),i);
        if (!strcmp(ost -> enc -> name,"libx264")) {
          av_dict_set(&ost -> opts,"stats",logfilename,16);
        }
        else {
          if (codec -> flags & 0002000) {
            char *logbuffer;
            size_t logbuffer_size;
            if (cmdutils_read_file(logfilename,&logbuffer,&logbuffer_size) < 0) {
              av_log(((void *)0),8,"Error reading log file '%s' for pass-2 encoding\n",logfilename);
              exit(1);
            }
            codec -> stats_in = logbuffer;
          }
          if (codec -> flags & 0x0200) {
            f = fopen(logfilename,"wb");
            if (!f) {
              av_log(((void *)0),8,"Cannot write log file '%s' for pass-1 encoding: %s\n",logfilename,strerror( *__errno_location()));
              exit(1);
            }
            ost -> logfile = f;
          }
        }
      }
    }
  }
/* open each encoder */
  for (i = 0; i < nb_output_streams; i++) {
    ost = output_streams[i];
    if (ost -> encoding_needed) {
      AVCodec *codec = ost -> enc;
      AVCodecContext *dec = ((void *)0);
      if (ist = get_input_stream(ost)) {
        dec = ist -> st -> codec;
      }
      if (dec && dec -> subtitle_header) {
/* ASS code assumes this buffer is null terminated so add extra byte. */
        ost -> st -> codec -> subtitle_header = (av_mallocz((dec -> subtitle_header_size + 1)));
        if (!ost -> st -> codec -> subtitle_header) {
          ret = - 12;
          goto dump_format;
        }
        memcpy((ost -> st -> codec -> subtitle_header),(dec -> subtitle_header),(dec -> subtitle_header_size));
        ost -> st -> codec -> subtitle_header_size = dec -> subtitle_header_size;
      }
      if (!av_dict_get(ost -> opts,"threads",((void *)0),0)) {
        av_dict_set(&ost -> opts,"threads","auto",0);
      }
      if ((ret = avcodec_open2(ost -> st -> codec,codec,&ost -> opts)) < 0) {
        if (ret == - 0x2bb2afa8) {
          abort_codec_experimental(codec,1);
        }
        snprintf(error,sizeof(error),"Error while opening encoder for output stream #%d:%d - maybe incorrect parameters such as bit_rate, rate, width or height",ost -> file_index,ost -> index);
        goto dump_format;
      }
      if ((ost -> enc -> type) == AVMEDIA_TYPE_AUDIO && !(ost -> enc -> capabilities & 0x10000)) {
        av_buffersink_set_frame_size(ost -> filter -> filter,(ost -> st -> codec -> frame_size));
      }
      assert_avoptions(ost -> opts);
      if (ost -> st -> codec -> bit_rate && ost -> st -> codec -> bit_rate < 1000) {
        av_log(((void *)0),24,"The bitrate parameter is set too low. It takes bits/s as argument, not kbits/s\n");
      }
      extra_size += (ost -> st -> codec -> extradata_size);
      if (ost -> st -> codec -> me_threshold) {
        input_streams[ost -> source_index] -> st -> codec -> debug |= 32;
      }
    }
    else {
      av_opt_set_dict((ost -> st -> codec),&ost -> opts);
    }
  }
/* init input streams */
  for (i = 0; i < nb_input_streams; i++) 
    if ((ret = init_input_stream(i,error,(sizeof(error)))) < 0) {
      for (i = 0; i < nb_output_streams; i++) {
        ost = output_streams[i];
        avcodec_close(ost -> st -> codec);
      }
      goto dump_format;
    }
/* discard unused programs */
  for (i = 0; i < nb_input_files; i++) {
    InputFile *ifile = input_files[i];
    for (j = 0; j < ifile -> ctx -> nb_programs; j++) {
      AVProgram *p = ifile -> ctx -> programs[j];
      int discard = AVDISCARD_ALL;
      for (k = 0; k < p -> nb_stream_indexes; k++) 
        if (!input_streams[(ifile -> ist_index) + p -> stream_index[k]] -> discard) {
          discard = AVDISCARD_DEFAULT;
          break; 
        }
      p -> discard = discard;
    }
  }
/* open files and write file headers */
  for (i = 0; i < nb_output_files; i++) {
    oc = output_files[i] -> ctx;
    oc -> interrupt_callback = int_cb;
    if ((ret = avformat_write_header(oc,&output_files[i] -> opts)) < 0) {
      char errbuf[128];
      const char *errbuf_ptr = errbuf;
      if (av_strerror(ret,errbuf,sizeof(errbuf)) < 0) {
        errbuf_ptr = (strerror(-ret));
      }
      snprintf(error,sizeof(error),"Could not write header for output file #%d (incorrect codec parameters ?): %s",i,errbuf_ptr);
      ret = - 22;
      goto dump_format;
    }
//         assert_avoptions(output_files[i]->opts);
    if (strcmp(oc -> oformat -> name,"rtp")) {
      want_sdp = 0;
    }
  }
  dump_format:
/* dump the file output parameters - cannot be done before in case
       of stream copy */
  for (i = 0; i < nb_output_files; i++) {
    av_dump_format(output_files[i] -> ctx,i,(output_files[i] -> ctx -> filename),1);
  }
/* dump the stream mapping */
  av_log(((void *)0),32,"Stream mapping:\n");
  for (i = 0; i < nb_input_streams; i++) {
    ist = input_streams[i];
    for (j = 0; j < ist -> nb_filters; j++) {
      if (ist -> filters[j] -> graph -> graph_desc) {
        av_log(((void *)0),32,"  Stream #%d:%d (%s) -> %s",ist -> file_index,ist -> st -> index,(ist -> dec?ist -> dec -> name : "?"),ist -> filters[j] -> name);
        if (nb_filtergraphs > 1) {
          av_log(((void *)0),32," (graph %d)",ist -> filters[j] -> graph -> index);
        }
        av_log(((void *)0),32,"\n");
      }
    }
  }
  for (i = 0; i < nb_output_streams; i++) {
    ost = output_streams[i];
    if (ost -> attachment_filename) {
/* an attached file */
      av_log(((void *)0),32,"  File %s -> Stream #%d:%d\n",ost -> attachment_filename,ost -> file_index,ost -> index);
      continue; 
    }
    if (ost -> filter && ost -> filter -> graph -> graph_desc) {
/* output from a complex graph */
      av_log(((void *)0),32,"  %s",ost -> filter -> name);
      if (nb_filtergraphs > 1) {
        av_log(((void *)0),32," (graph %d)",ost -> filter -> graph -> index);
      }
      av_log(((void *)0),32," -> Stream #%d:%d (%s)\n",ost -> file_index,ost -> index,(ost -> enc?ost -> enc -> name : "?"));
      continue; 
    }
    av_log(((void *)0),32,"  Stream #%d:%d -> #%d:%d",input_streams[ost -> source_index] -> file_index,input_streams[ost -> source_index] -> st -> index,ost -> file_index,ost -> index);
    if (ost -> sync_ist != input_streams[ost -> source_index]) {
      av_log(((void *)0),32," [sync #%d:%d]",ost -> sync_ist -> file_index,ost -> sync_ist -> st -> index);
    }
    if (ost -> stream_copy) {
      av_log(((void *)0),32," (copy)");
    }
    else {
      av_log(((void *)0),32," (%s -> %s)",(input_streams[ost -> source_index] -> dec?input_streams[ost -> source_index] -> dec -> name : "?"),(ost -> enc?ost -> enc -> name : "?"));
    }
    av_log(((void *)0),32,"\n");
  }
  if (ret) {
    av_log(((void *)0),16,"%s\n",error);
    return ret;
  }
  if (want_sdp) {
    print_sdp();
  }
  return 0;
}
/* Return 1 if there remain streams where more output is wanted, 0 otherwise. */

static int need_output()
{
  int i;
  for (i = 0; i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    OutputFile *of = output_files[ost -> file_index];
    AVFormatContext *os = output_files[ost -> file_index] -> ctx;
    if (ost -> finished || os -> pb && (avio_tell(os -> pb)) >= of -> limit_filesize) {
      continue; 
    }
    if ((ost -> frame_number) >= ost -> max_frames) {
      int j;
      for (j = 0; j < of -> ctx -> nb_streams; j++) 
        close_output_stream(output_streams[of -> ost_index + j]);
      continue; 
    }
    return 1;
  }
  return 0;
}
/**
 * Select the output stream to process.
 *
 * @return  selected output stream, or NULL if none available
 */

static OutputStream *choose_output()
{
  int i;
  int64_t opts_min = 9223372036854775807L;
  OutputStream *ost_min = ((void *)0);
  for (i = 0; i < nb_output_streams; i++) {
    OutputStream *ost = output_streams[i];
    int64_t opts = av_rescale_q(ost -> st -> cur_dts,ost -> st -> time_base,((AVRational ){(1), (1000000)}));
    if (!ost -> unavailable && !ost -> finished && opts < opts_min) {
      opts_min = opts;
      ost_min = ost;
    }
  }
  return ost_min;
}

static int check_keyboard_interaction(int64_t cur_time)
{
  int i;
  int ret;
  int key;
  static int64_t last_time;
  if (received_nb_signals) {
    return -((int )(('E' | 'X' << 8 | 'I' << 16) | ((unsigned int )'T') << 24));
  }
/* read_key() returns 0 on EOF */
  if (cur_time - last_time >= 100000 && !run_as_daemon) {
    key = read_key();
    last_time = cur_time;
  }
  else {
    key = - 1;
  }
  if (key == 'q') {
    return -((int )(('E' | 'X' << 8 | 'I' << 16) | ((unsigned int )'T') << 24));
  }
  if (key == '+') {
    av_log_set_level(av_log_get_level() + 10);
  }
  if (key == '-') {
    av_log_set_level(av_log_get_level() - 10);
  }
  if (key == 's') {
    qp_hist ^= 1;
  }
  if (key == 'h') {
    if (do_hex_dump) {
      do_hex_dump = do_pkt_dump = 0;
    }
    else {
      if (do_pkt_dump) {
        do_hex_dump = 1;
      }
      else {
        do_pkt_dump = 1;
      }
    }
    av_log_set_level(48);
  }
  if (key == 'c' || key == 'C') {
    char buf[4096];
    char target[64];
    char command[256];
    char arg[256] = {(0)};
    double time;
    int k;
    int n = 0;
    fprintf(stderr,"\nEnter command: <target> <time> <command>[ <argument>]\n");
    i = 0;
    while((k = read_key()) != 10 && k != '\r' && i < sizeof(buf) - 1)
      if (k > 0) {
        buf[i++] = k;
      }
    buf[i] = 0;
    if (k > 0 && (n = sscanf(buf,"%63[^ ] %lf %255[^ ] %255[^\n]",target,&time,command,arg)) >= 3) {
      av_log(((void *)0),48,"Processing command target:%s time:%f command:%s arg:%s",target,time,command,arg);
      for (i = 0; i < nb_filtergraphs; i++) {
        FilterGraph *fg = filtergraphs[i];
        if (fg -> graph) {
          if (time < 0) {
            ret = avfilter_graph_send_command(fg -> graph,target,command,arg,buf,(sizeof(buf)),(key == 'c'?1 : 0));
            fprintf(stderr,"Command reply for stream %d: ret:%d res:%s\n",i,ret,buf);
          }
          else {
            ret = avfilter_graph_queue_command(fg -> graph,target,command,arg,0,time);
          }
        }
      }
    }
    else {
      av_log(((void *)0),16,"Parse error, at least 3 arguments were expected, only %d given in string '%s'\n",n,buf);
    }
  }
  if (key == 100 || key == 'D') {
    int debug = 0;
    if (key == 'D') {
      debug = input_streams[0] -> st -> codec -> debug << 1;
      if (!debug) {
        debug = 1;
      }
//unsupported, would just crash
      while(debug & (0000100 | 0x00002000 | 0x4000))
        debug += debug;
    }
    else {
      if (scanf("%d",&debug) != 1) {
        fprintf(stderr,"error parsing debug value\n");
      }
    }
    for (i = 0; i < nb_input_streams; i++) {
      input_streams[i] -> st -> codec -> debug = debug;
    }
    for (i = 0; i < nb_output_streams; i++) {
      OutputStream *ost = output_streams[i];
      ost -> st -> codec -> debug = debug;
    }
    if (debug) {
      av_log_set_level(48);
    }
    fprintf(stderr,"debug=%d\n",debug);
  }
  if (key == 63) {
    fprintf(stderr,"key    function\n?      show this help\n+      increase verbosity\n-      decrease verbosity\nc      Send command to filtergraph\nD      cycle through available debug modes\nh      dump packets/hex press to cycle through the 3 states\nq      quit\ns      Show QP histogram\n");
  }
  return 0;
}
#if HAVE_PTHREADS

static void *input_thread(void *arg)
{
  InputFile *f = arg;
  int ret = 0;
  while(!transcoding_finished && ret >= 0){
    AVPacket pkt;
    ret = av_read_frame(f -> ctx,&pkt);
    if (ret == - 11) {
      av_usleep(10000);
      ret = 0;
      continue; 
    }
    else {
      if (ret < 0) {
        break; 
      }
    }
    pthread_mutex_lock(&f -> fifo_lock);
    while(!av_fifo_space(f -> fifo))
      pthread_cond_wait(&f -> fifo_cond,&f -> fifo_lock);
    av_dup_packet(&pkt);
    av_fifo_generic_write(f -> fifo,(&pkt),(sizeof(pkt)),((void *)0));
    pthread_mutex_unlock(&f -> fifo_lock);
  }
  f -> finished = 1;
  return (void *)0;
}

static void free_input_threads()
{
  int i;
  if (nb_input_files == 1) {
    return ;
  }
  transcoding_finished = 1;
  for (i = 0; i < nb_input_files; i++) {
    InputFile *f = input_files[i];
    AVPacket pkt;
    if (!f -> fifo || f -> joined) {
      continue; 
    }
    pthread_mutex_lock(&f -> fifo_lock);
    while(av_fifo_size(f -> fifo)){
      av_fifo_generic_read(f -> fifo,(&pkt),(sizeof(pkt)),((void *)0));
      av_free_packet(&pkt);
    }
    pthread_cond_signal(&f -> fifo_cond);
    pthread_mutex_unlock(&f -> fifo_lock);
    pthread_join(f -> thread,((void *)0));
    f -> joined = 1;
    while(av_fifo_size(f -> fifo)){
      av_fifo_generic_read(f -> fifo,(&pkt),(sizeof(pkt)),((void *)0));
      av_free_packet(&pkt);
    }
    av_fifo_free(f -> fifo);
  }
}

static int init_input_threads()
{
  int i;
  int ret;
  if (nb_input_files == 1) {
    return 0;
  }
  for (i = 0; i < nb_input_files; i++) {
    InputFile *f = input_files[i];
    if (!(f -> fifo = av_fifo_alloc((8 * sizeof(AVPacket ))))) {
      return - 12;
    }
    pthread_mutex_init(&f -> fifo_lock,((void *)0));
    pthread_cond_init(&f -> fifo_cond,((void *)0));
    if (ret = pthread_create(&f -> thread,((void *)0),input_thread,f)) {
      return -ret;
    }
  }
  return 0;
}

static int get_input_packet_mt(InputFile *f,AVPacket *pkt)
{
  int ret = 0;
  pthread_mutex_lock(&f -> fifo_lock);
  if (av_fifo_size(f -> fifo)) {
    av_fifo_generic_read(f -> fifo,pkt,(sizeof(( *pkt))),((void *)0));
    pthread_cond_signal(&f -> fifo_cond);
  }
  else {
    if (f -> finished) {
      ret = -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
    }
    else {
      ret = - 11;
    }
  }
  pthread_mutex_unlock(&f -> fifo_lock);
  return ret;
}
#endif

static int get_input_packet(InputFile *f,AVPacket *pkt)
{
#if HAVE_PTHREADS
  if (nb_input_files > 1) {
    return get_input_packet_mt(f,pkt);
  }
#endif
  return av_read_frame(f -> ctx,pkt);
}

static int got_eagain()
{
  int i;
  for (i = 0; i < nb_output_streams; i++) 
    if (output_streams[i] -> unavailable) {
      return 1;
    }
  return 0;
}

static void reset_eagain()
{
  int i;
  for (i = 0; i < nb_input_files; i++) 
    input_files[i] -> eagain = 0;
  for (i = 0; i < nb_output_streams; i++) 
    output_streams[i] -> unavailable = 0;
}
/*
 * Return
 * - 0 -- one packet was read and processed
 * - AVERROR(EAGAIN) -- no packets were available for selected file,
 *   this function should be called again
 * - AVERROR_EOF -- this function should not be called again
 */

static int process_input(int file_index)
{
  InputFile *ifile = input_files[file_index];
  AVFormatContext *is;
  InputStream *ist;
  AVPacket pkt;
  int ret;
  int i;
  int j;
  is = ifile -> ctx;
  ret = get_input_packet(ifile,&pkt);
  if (ret == - 11) {
    ifile -> eagain = 1;
    return ret;
  }
  if (ret < 0) {
    if (ret != -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24))) {
      print_error((is -> filename),ret);
      if (exit_on_error) {
        exit(1);
      }
    }
    ifile -> eof_reached = 1;
    for (i = 0; i < ifile -> nb_streams; i++) {
      ist = input_streams[ifile -> ist_index + i];
      if (ist -> decoding_needed) {
        output_packet(ist,((void *)0));
      }
/* mark all outputs that don't go through lavfi as finished */
      for (j = 0; j < nb_output_streams; j++) {
        OutputStream *ost = output_streams[j];
        if (ost -> source_index == ifile -> ist_index + i && (ost -> stream_copy || (ost -> enc -> type) == AVMEDIA_TYPE_SUBTITLE)) {
          close_output_stream(ost);
        }
      }
    }
    return - 11;
  }
  reset_eagain();
  if (do_pkt_dump) {
    av_pkt_dump_log2(((void *)0),48,&pkt,do_hex_dump,is -> streams[pkt . stream_index]);
  }
/* the following test is needed in case new streams appear
       dynamically in stream : we ignore them */
  if (pkt . stream_index >= ifile -> nb_streams) {
    report_new_stream(file_index,&pkt);
    goto discard_packet;
  }
  ist = input_streams[ifile -> ist_index + pkt . stream_index];
  if (ist -> discard) {
    goto discard_packet;
  }
  if (debug_ts) {
    av_log(((void *)0),32,"demuxer -> ist_index:%d type:%s next_dts:%s next_dts_time:%s next_pts:%s next_pts_time:%s pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s off:%s off_time:%s\n",ifile -> ist_index + pkt . stream_index,av_get_media_type_string(ist -> st -> codec -> codec_type),av_ts_make_string(((char [32]){(0)}),ist -> next_dts),av_ts_make_time_string(((char [32]){(0)}),ist -> next_dts,&((AVRational ){(1), (1000000)})),av_ts_make_string(((char [32]){(0)}),ist -> next_pts),av_ts_make_time_string(((char [32]){(0)}),ist -> next_pts,&((AVRational ){(1), (1000000)})),av_ts_make_string(((char [32]){(0)}),pkt . pts),av_ts_make_time_string(((char [32]){(0)}),pkt . pts,&ist -> st -> time_base),av_ts_make_string(((char [32]){(0)}),pkt . dts),av_ts_make_time_string(((char [32]){(0)}),pkt . dts,&ist -> st -> time_base),av_ts_make_string(((char [32]){(0)}),input_files[ist -> file_index] -> ts_offset),av_ts_make_time_string(((char [32]){(0)}),input_files[ist -> file_index] -> ts_offset,&((AVRational ){(1), (1000000)})));
  }
  if (!ist -> wrap_correction_done && is -> start_time != ((int64_t )0x8000000000000000UL) && ist -> st -> pts_wrap_bits < 64) {
    int64_t stime;
    int64_t stime2;
// Correcting starttime based on the enabled streams
// FIXME this ideally should be done before the first use of starttime but we do not know which are the enabled streams at that point.
//       so we instead do it here as part of discontinuity handling
    if (ist -> next_dts == ((int64_t )0x8000000000000000UL) && ifile -> ts_offset == -is -> start_time && is -> iformat -> flags & 0x0200) {
      int64_t new_start_time = 9223372036854775807L;
      for (i = 0; i < is -> nb_streams; i++) {
        AVStream *st = is -> streams[i];
        if ((st -> discard) == AVDISCARD_ALL || st -> start_time == ((int64_t )0x8000000000000000UL)) {
          continue; 
        }
        new_start_time = (new_start_time > av_rescale_q(st -> start_time,st -> time_base,((AVRational ){(1), (1000000)}))?av_rescale_q(st -> start_time,st -> time_base,((AVRational ){(1), (1000000)})) : new_start_time);
      }
      if (new_start_time > is -> start_time) {
        av_log(is,40,"Correcting start time by %ld\n",new_start_time - is -> start_time);
        ifile -> ts_offset = -new_start_time;
      }
    }
    stime = av_rescale_q(is -> start_time,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
    stime2 = (stime + (1ULL << ist -> st -> pts_wrap_bits));
    ist -> wrap_correction_done = 1;
    if (stime2 > stime && pkt . dts != ((int64_t )0x8000000000000000UL) && pkt . dts > stime + (1LL << ist -> st -> pts_wrap_bits - 1)) {
      pkt . dts -= 1ULL << ist -> st -> pts_wrap_bits;
      ist -> wrap_correction_done = 0;
    }
    if (stime2 > stime && pkt . pts != ((int64_t )0x8000000000000000UL) && pkt . pts > stime + (1LL << ist -> st -> pts_wrap_bits - 1)) {
      pkt . pts -= 1ULL << ist -> st -> pts_wrap_bits;
      ist -> wrap_correction_done = 0;
    }
  }
  if (pkt . dts != ((int64_t )0x8000000000000000UL)) {
    pkt . dts += av_rescale_q(ifile -> ts_offset,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
  }
  if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
    pkt . pts += av_rescale_q(ifile -> ts_offset,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
  }
  if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
    pkt . pts *= ist -> ts_scale;
  }
  if (pkt . dts != ((int64_t )0x8000000000000000UL)) {
    pkt . dts *= ist -> ts_scale;
  }
  if (pkt . dts != ((int64_t )0x8000000000000000UL) && ist -> next_dts != ((int64_t )0x8000000000000000UL) && !copy_ts) {
    int64_t pkt_dts = av_rescale_q(pkt . dts,ist -> st -> time_base,((AVRational ){(1), (1000000)}));
    int64_t delta = pkt_dts - ist -> next_dts;
    if (is -> iformat -> flags & 0x0200) {
      if (delta < (- 1LL) * dts_delta_threshold * 1000000 || delta > 1LL * dts_delta_threshold * 1000000 && (ist -> st -> codec -> codec_type) != AVMEDIA_TYPE_SUBTITLE || pkt_dts + 1 < ist -> pts) {
        ifile -> ts_offset -= delta;
        av_log(((void *)0),48,"timestamp discontinuity %ld, new offset= %ld\n",delta,ifile -> ts_offset);
        pkt . dts -= av_rescale_q(delta,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
        if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
          pkt . pts -= av_rescale_q(delta,((AVRational ){(1), (1000000)}),ist -> st -> time_base);
        }
      }
    }
    else {
      if (delta < (- 1LL) * dts_error_threshold * 1000000 || delta > 1LL * dts_error_threshold * 1000000 && (ist -> st -> codec -> codec_type) != AVMEDIA_TYPE_SUBTITLE) {
        av_log(((void *)0),24,"DTS %ld, next:%ld st:%d invalid dropping\n",pkt . dts,ist -> next_dts,pkt . stream_index);
        pkt . dts = ((int64_t )0x8000000000000000UL);
      }
      if (pkt . pts != ((int64_t )0x8000000000000000UL)) {
        int64_t pkt_pts = av_rescale_q(pkt . pts,ist -> st -> time_base,((AVRational ){(1), (1000000)}));
        delta = pkt_pts - ist -> next_dts;
        if (delta < (- 1LL) * dts_error_threshold * 1000000 || delta > 1LL * dts_error_threshold * 1000000 && (ist -> st -> codec -> codec_type) != AVMEDIA_TYPE_SUBTITLE) {
          av_log(((void *)0),24,"PTS %ld, next:%ld invalid dropping st:%d\n",pkt . pts,ist -> next_dts,pkt . stream_index);
          pkt . pts = ((int64_t )0x8000000000000000UL);
        }
      }
    }
  }
  if (debug_ts) {
    av_log(((void *)0),32,"demuxer+ffmpeg -> ist_index:%d type:%s pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s off:%s off_time:%s\n",ifile -> ist_index + pkt . stream_index,av_get_media_type_string(ist -> st -> codec -> codec_type),av_ts_make_string(((char [32]){(0)}),pkt . pts),av_ts_make_time_string(((char [32]){(0)}),pkt . pts,&ist -> st -> time_base),av_ts_make_string(((char [32]){(0)}),pkt . dts),av_ts_make_time_string(((char [32]){(0)}),pkt . dts,&ist -> st -> time_base),av_ts_make_string(((char [32]){(0)}),input_files[ist -> file_index] -> ts_offset),av_ts_make_time_string(((char [32]){(0)}),input_files[ist -> file_index] -> ts_offset,&((AVRational ){(1), (1000000)})));
  }
  sub2video_heartbeat(ist,pkt . pts);
  ret = output_packet(ist,(&pkt));
  if (ret < 0) {
    char buf[128];
    av_strerror(ret,buf,sizeof(buf));
    av_log(((void *)0),16,"Error while decoding stream #%d:%d: %s\n",ist -> file_index,ist -> st -> index,buf);
    if (exit_on_error) {
      exit(1);
    }
  }
  discard_packet:
  av_free_packet(&pkt);
  return 0;
}
/**
 * Perform a step of transcoding for the specified filter graph.
 *
 * @param[in]  graph     filter graph to consider
 * @param[out] best_ist  input stream where a frame would allow to continue
 * @return  0 for success, <0 for error
 */

static int transcode_from_filter(FilterGraph *graph,InputStream **best_ist)
{
  int i;
  int ret;
  int nb_requests;
  int nb_requests_max = 0;
  InputFilter *ifilter;
  InputStream *ist;
   *best_ist = ((void *)0);
  ret = avfilter_graph_request_oldest(graph -> graph);
  if (ret >= 0) {
    return reap_filters();
  }
  if (ret == -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24))) {
    ret = reap_filters();
    for (i = 0; i < graph -> nb_outputs; i++) 
      close_output_stream(graph -> outputs[i] -> ost);
    return ret;
  }
  if (ret != - 11) {
    return ret;
  }
  for (i = 0; i < graph -> nb_inputs; i++) {
    ifilter = graph -> inputs[i];
    ist = ifilter -> ist;
    if (input_files[ist -> file_index] -> eagain || input_files[ist -> file_index] -> eof_reached) {
      continue; 
    }
    nb_requests = (av_buffersrc_get_nb_failed_requests(ifilter -> filter));
    if (nb_requests > nb_requests_max) {
      nb_requests_max = nb_requests;
       *best_ist = ist;
    }
  }
  if (!( *best_ist)) {
    for (i = 0; i < graph -> nb_outputs; i++) 
      graph -> outputs[i] -> ost -> unavailable = 1;
  }
  return 0;
}
/**
 * Run a single step of transcoding.
 *
 * @return  0 for success, <0 for error
 */

static int transcode_step()
{
  OutputStream *ost;
  InputStream *ist;
  int ret;
  ost = choose_output();
  if (!ost) {
    if (got_eagain()) {
      reset_eagain();
      av_usleep(10000);
      return 0;
    }
    av_log(((void *)0),40,"No more inputs to read from, finishing.\n");
    return -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
  }
  if (ost -> filter) {
    if ((ret = transcode_from_filter(ost -> filter -> graph,&ist)) < 0) {
      return ret;
    }
    if (!ist) {
      return 0;
    }
  }
  else {
    do {
      if (!(ost -> source_index >= 0)) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","ost->source_index >= 0","ffmpeg.c",3115);
        abort();
      }
    }while (0);
    ist = input_streams[ost -> source_index];
  }
  ret = process_input(ist -> file_index);
  if (ret == - 11) {
    if (input_files[ist -> file_index] -> eagain) {
      ost -> unavailable = 1;
    }
    return 0;
  }
  if (ret < 0) {
    return ret == -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24))?0 : ret;
  }
  return reap_filters();
}
/*
 * The following code is the main loop of the file converter
 */

static int transcode()
{
  int ret;
  int i;
  AVFormatContext *os;
  OutputStream *ost;
  InputStream *ist;
  int64_t timer_start;
  ret = transcode_init();
  if (ret < 0) {
    goto fail;
  }
  if (stdin_interaction) {
    av_log(((void *)0),32,"Press [q] to stop, [?] for help\n");
  }
  timer_start = av_gettime();
#if HAVE_PTHREADS
  if ((ret = init_input_threads()) < 0) {
    goto fail;
  }
#endif
  while(!received_sigterm){
    int64_t cur_time = av_gettime();
/* if 'q' pressed, exits */
    if (stdin_interaction) {
      if (check_keyboard_interaction(cur_time) < 0) {
        break; 
      }
    }
/* check if there's any stream where output is still needed */
    if (!need_output()) {
      av_log(((void *)0),40,"No more output streams to write to, finishing.\n");
      break; 
    }
    ret = transcode_step();
    if (ret < 0) {
      if (ret == -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24)) || ret == - 11) {
        continue; 
      }
      av_log(((void *)0),16,"Error while filtering.\n");
      break; 
    }
/* dump report by using the output first video and audio streams */
    print_report(0,timer_start,cur_time);
  }
#if HAVE_PTHREADS
  free_input_threads();
#endif
/* at the end of stream, we must flush the decoder buffers */
  for (i = 0; i < nb_input_streams; i++) {
    ist = input_streams[i];
    if (!input_files[ist -> file_index] -> eof_reached && ist -> decoding_needed) {
      output_packet(ist,((void *)0));
    }
  }
  flush_encoders();
  term_exit();
/* write the trailer if needed and close file */
  for (i = 0; i < nb_output_files; i++) {
    os = output_files[i] -> ctx;
    av_write_trailer(os);
  }
/* dump report by using the first video and audio streams */
  print_report(1,timer_start,av_gettime());
/* close each encoder */
  for (i = 0; i < nb_output_streams; i++) {
    ost = output_streams[i];
    if (ost -> encoding_needed) {
      av_freep((&ost -> st -> codec -> stats_in));
      avcodec_close(ost -> st -> codec);
    }
  }
/* close each decoder */
  for (i = 0; i < nb_input_streams; i++) {
    ist = input_streams[i];
    if (ist -> decoding_needed) {
      avcodec_close(ist -> st -> codec);
    }
  }
/* finished ! */
  ret = 0;
  fail:
#if HAVE_PTHREADS
  free_input_threads();
#endif
  if (output_streams) {
    for (i = 0; i < nb_output_streams; i++) {
      ost = output_streams[i];
      if (ost) {
        if (ost -> stream_copy) {
          av_freep((&ost -> st -> codec -> extradata));
        }
        if (ost -> logfile) {
          fclose(ost -> logfile);
          ost -> logfile = ((void *)0);
        }
        av_freep((&ost -> st -> codec -> subtitle_header));
        av_free((ost -> forced_kf_pts));
        av_dict_free(&ost -> opts);
        av_dict_free(&ost -> swr_opts);
        av_dict_free(&ost -> resample_opts);
      }
    }
  }
  return ret;
}

static int64_t getutime()
{
#if HAVE_GETRUSAGE
  struct rusage rusage;
  getrusage(RUSAGE_SELF,&rusage);
  return (rusage . ru_utime . tv_sec * 1000000LL + rusage . ru_utime . tv_usec);
#elif HAVE_GETPROCESSTIMES
#else
#endif
}

static int64_t getmaxrss()
{
#if HAVE_GETRUSAGE && HAVE_STRUCT_RUSAGE_RU_MAXRSS
  struct rusage rusage;
  getrusage(RUSAGE_SELF,&rusage);
  return ((int64_t )rusage . ru_maxrss) * 1024;
#elif HAVE_GETPROCESSMEMORYINFO
#else
#endif
}

static void log_callback_null(void *ptr,int level,const char *fmt,va_list vl)
{
}
#if STONESOUP_TRACE

static void do_atexit_tracepoint()
{
  do {
    ;
    if (__builtin_expect((!(!__tracepoint_stonesoup_trace___trace_end . state)),0)) {
      __tracepoint_cb_stonesoup_trace___trace_end();
    }
  }while (0);
}
#endif

int main(int argc,char **argv)
{
    #if STONESOUP_TRACE
  do {
    ;
    if (__builtin_expect((!(!__tracepoint_stonesoup_trace___trace_start . state)),0)) {
      __tracepoint_cb_stonesoup_trace___trace_start("ffmpeg");
    }
  }while (0);
  atexit(do_atexit_tracepoint);
    #endif
  int ret;
  int64_t ti;
  atexit(exit_program);
/* win32 runtime needs this */
  setvbuf(stderr,((void *)0),2,0);
  av_log_set_flags(1);
  parse_loglevel(argc,argv,options);
  if (argc > 1 && !strcmp(argv[1],"-d")) {
    run_as_daemon = 1;
    av_log_set_callback(log_callback_null);
    argc--;
    argv++;
  }
  avcodec_register_all();
#if CONFIG_AVDEVICE
  avdevice_register_all();
#endif
  avfilter_register_all();
  av_register_all();
  avformat_network_init();
  show_banner(argc,argv,options);
  term_init();
/* parse options and open all input/output files */
  ret = ffmpeg_parse_options(argc,argv);
  if (ret < 0) {
    exit(1);
  }
  if (nb_output_files <= 0 && nb_input_files == 0) {
    show_usage();
    av_log(((void *)0),24,"Use -h to get full help or, even better, run 'man %s'\n",program_name);
    exit(1);
  }
/* file converter / grab */
  if (nb_output_files <= 0) {
    av_log(((void *)0),8,"At least one output file must be specified\n");
    exit(1);
  }
//     if (nb_input_files == 0) {
//         av_log(NULL, AV_LOG_FATAL, "At least one input file must be specified\n");
//         exit(1);
//     }
  current_time = (ti = getutime());
  if (transcode() < 0) {
    exit(1);
  }
  ti = getutime() - ti;
  if (do_benchmark) {
    printf("bench: utime=%0.3fs\n",ti / 1000000.0);
  }
  exit((received_nb_signals?255 : 0));
  return 0;
}

void qualificator_hyperplasia(phaneroscope_nonfrugally unsighted_merula)
{
  ++stonesoup_global_variable;;
  ampullated_phare(unsighted_merula);
}

void ampullated_phare(phaneroscope_nonfrugally valmiki_mollusklike)
{
  ++stonesoup_global_variable;;
  monos_supraspinal(valmiki_mollusklike);
}

void monos_supraspinal(phaneroscope_nonfrugally stirrers_escobita)
{
  ++stonesoup_global_variable;;
  paradiseidae_titian(stirrers_escobita);
}

void paradiseidae_titian(phaneroscope_nonfrugally shatterable_troas)
{
  ++stonesoup_global_variable;;
  uxoricidal_vocationally(shatterable_troas);
}

void uxoricidal_vocationally(phaneroscope_nonfrugally personalistic_unmarked)
{
  ++stonesoup_global_variable;;
  lockable_marcgravia(personalistic_unmarked);
}

void lockable_marcgravia(phaneroscope_nonfrugally nondependably_unbeclogged)
{
  ++stonesoup_global_variable;;
  lowlander_radiothermy(nondependably_unbeclogged);
}

void lowlander_radiothermy(phaneroscope_nonfrugally athymy_debarks)
{
  ++stonesoup_global_variable;;
  lieutenants_pacifists(athymy_debarks);
}

void lieutenants_pacifists(phaneroscope_nonfrugally dicarboxylate_cyanochroic)
{
  ++stonesoup_global_variable;;
  biallyl_anticorn(dicarboxylate_cyanochroic);
}

void biallyl_anticorn(phaneroscope_nonfrugally gudrun_sapotilha)
{
  ++stonesoup_global_variable;;
  gerara_basilicate(gudrun_sapotilha);
}

void gerara_basilicate(phaneroscope_nonfrugally morcote_optimality)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *subscleral_sacrocoxalgia = 0;
  ++stonesoup_global_variable;;
  subscleral_sacrocoxalgia = ((char *)morcote_optimality);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(subscleral_sacrocoxalgia,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
   if (stonesoup_file == 0 && errno == 24) {
    stonesoup_printf("Fopen error due to ulimit\n");
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing File Descriptor Release) */
            /* STONESOUP: TRIGGER-POINT (Missing File Descriptor Release) */
   stonesoup_file_list[stonesoup_ssi] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    stonesoup_printf("finished evaluating\n");
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
    tracepoint(stonesoup_trace, weakness_end);
;
  if (morcote_optimality != 0) 
    free(((char *)morcote_optimality));
stonesoup_close_printf_context();
}
