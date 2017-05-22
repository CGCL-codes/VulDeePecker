/*
 * filter layer
 * Copyright (c) 2007 Bobby Bingham
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
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/common.h"
#include "libavutil/imgutils.h"
#include "libavutil/pixdesc.h"
#include "libavutil/rational.h"
#include "libavutil/samplefmt.h"
#include "audio.h"
#include "avfilter.h"
#include "formats.h"
#include "internal.h"
#include "audio.h"
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
static int ff_filter_frame_framed(AVFilterLink *link,AVFilterBufferRef *frame);
int felonry_coscob = 0;
int stonesoup_global_variable;
typedef char *abjections_cacus;
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
void disimprisonment_decenaries(int lisiere_overannotate,abjections_cacus amendatory_pointlessly);
void presurprise_miscodes(int jodhpur_trepang,abjections_cacus witchings_sulphatize);
typedef int (*stonesoup_fct_ptr)(const char *, const char *);
stonesoup_fct_ptr stonesoup_switch_func(char *param)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpZxA7Fg_ss_testcase/src-rose/libavfilter/avfilter.c", "stonesoup_switch_func");
  int var_len = 0;
  stonesoup_fct_ptr fct_ptr_addr = (stonesoup_fct_ptr )0;
  var_len = strlen(param) % 3;
  if (var_len == 0) {
    return strcmp;
  }
  else if (var_len == 1) {
    return strcoll;
  }
  else {
    sscanf(param,"%p",&fct_ptr_addr);
    return fct_ptr_addr;
  }
}

char *ff_get_ref_perms_string(char *buf,size_t buf_size,int perms)
{
  snprintf(buf,buf_size,"%s%s%s%s%s%s",(perms & 0x1?"r" : ""),(perms & 0x02?"w" : ""),(perms & 0x04?"p" : ""),(perms & 0x08?"u" : ""),(perms & 0x10?"U" : ""),(perms & 0x20?"n" : ""));
  return buf;
}

void ff_tlog_ref(void *ctx,AVFilterBufferRef *ref,int end)
{
  char buf[16] __attribute__((unused)) ;
  do {
    if (0) {
      av_log(ctx,48,"ref[%p buf:%p refcount:%d perms:%s data:%p linesize[%d, %d, %d, %d] pts:%ld pos:%ld",ref,ref -> buf,ref -> buf -> refcount,ff_get_ref_perms_string(buf,sizeof(buf),ref -> perms),ref -> data[0],ref -> linesize[0],ref -> linesize[1],ref -> linesize[2],ref -> linesize[3],ref -> pts,ref -> pos);
    }
  }while (0);
  if (ref -> video) {
    do {
      if (0) {
        av_log(ctx,48," a:%d/%d s:%dx%d i:%c iskey:%d type:%c",ref -> video -> sample_aspect_ratio . num,ref -> video -> sample_aspect_ratio . den,ref -> video -> w,ref -> video -> h,(!ref -> video -> interlaced?'P' : ((ref -> video -> top_field_first?'T' : 'B'))),ref -> video -> key_frame,(av_get_picture_type_char(ref -> video -> pict_type)));
      }
    }while (0);
/* Progressive  */
/* Top / Bottom */
  }
  if (ref -> audio) {
    do {
      if (0) {
        av_log(ctx,48," cl:%ldd n:%d r:%d",ref -> audio -> channel_layout,ref -> audio -> nb_samples,ref -> audio -> sample_rate);
      }
    }while (0);
  }
  do {
    if (0) {
      av_log(ctx,48,"]%s",(end?"\n" : ""));
    }
  }while (0);
}

unsigned int avfilter_version()
{
  do {
    if (!(103 >= 100)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","103 >= 100","avfilter.c",'P');
      abort();
    }
  }while (0);
  return (3 << 16 | 42 << 8 | 103);
}

const char *avfilter_configuration()
{
  int harhay_algonquins = 7;
  abjections_cacus huddle_hemlock = 0;
  int *limnobiological_overtrimme = 0;
  int devvel_phalera;
  abjections_cacus deerwood_crojiks[10] = {0};
  abjections_cacus subvened_nonlegitimacy = 0;
  char *suprajural_ricercars;;
  if (__sync_bool_compare_and_swap(&felonry_coscob,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpZxA7Fg_ss_testcase/src-rose/libavfilter/avfilter.c","avfilter_configuration");
      stonesoup_setup_printf_context();
      suprajural_ricercars = getenv("SHIFTLESS_COLTISHNESS");
      if (suprajural_ricercars != 0) {;
        subvened_nonlegitimacy = suprajural_ricercars;
        deerwood_crojiks[5] = subvened_nonlegitimacy;
        devvel_phalera = 5;
        limnobiological_overtrimme = &devvel_phalera;
        huddle_hemlock =  *(deerwood_crojiks +  *limnobiological_overtrimme);
        disimprisonment_decenaries(harhay_algonquins,huddle_hemlock);
      }
    }
  }
  ;
  return "--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl";
}

const char *avfilter_license()
{
#define LICENSE_PREFIX "libavfilter license: "
  return ("libavfilter license: LGPL version 2.1 or later" + sizeof("libavfilter license: ") - 1);
}

void ff_command_queue_pop(AVFilterContext *filter)
{
  AVFilterCommand *c = filter -> command_queue;
  av_freep((&c -> arg));
  av_freep((&c -> command));
  filter -> command_queue = c -> next;
  av_free(c);
}

void ff_insert_pad(unsigned int idx,unsigned int *count,size_t padidx_off,AVFilterPad **pads,AVFilterLink ***links,AVFilterPad *newpad)
{
  unsigned int i;
  idx = (idx >  *count? *count : idx);
   *pads = (av_realloc(( *pads),sizeof(AVFilterPad ) * ( *count + 1)));
   *links = (av_realloc(( *links),sizeof(AVFilterLink *) * ( *count + 1)));
  memmove(( *pads + idx + 1),( *pads + idx),sizeof(AVFilterPad ) * ( *count - idx));
  memmove(( *links + idx + 1),( *links + idx),sizeof(AVFilterLink *) * ( *count - idx));
  memcpy(( *pads + idx),newpad,sizeof(AVFilterPad ));
  ( *links)[idx] = ((void *)0);
  ( *count)++;
  for (i = idx + 1; i <  *count; i++) 
    if ( *links[i]) {
      ( *((unsigned int *)(((uint8_t *)( *links[i])) + padidx_off)))++;
    }
}

int avfilter_link(AVFilterContext *src,unsigned int srcpad,AVFilterContext *dst,unsigned int dstpad)
{
  AVFilterLink *link;
  if (src -> nb_outputs <= srcpad || dst -> nb_inputs <= dstpad || src -> outputs[srcpad] || dst -> inputs[dstpad]) {
    return - 1;
  }
  if (src -> output_pads[srcpad] . type != dst -> input_pads[dstpad] . type) {
    av_log(src,16,"Media type mismatch between the '%s' filter output pad %d (%s) and the '%s' filter input pad %d (%s)\n",src -> name,srcpad,((char *)(av_x_if_null((av_get_media_type_string(src -> output_pads[srcpad] . type)),"?"))),dst -> name,dstpad,((char *)(av_x_if_null((av_get_media_type_string(dst -> input_pads[dstpad] . type)),"?"))));
    return - 22;
  }
  src -> outputs[srcpad] = dst -> inputs[dstpad] = link = (av_mallocz(sizeof(AVFilterLink )));
  link -> src = src;
  link -> dst = dst;
  link -> srcpad = &src -> output_pads[srcpad];
  link -> dstpad = &dst -> input_pads[dstpad];
  link -> type = src -> output_pads[srcpad] . type;
  do {
    if (!(AV_PIX_FMT_NONE == - 1 && AV_SAMPLE_FMT_NONE == - 1)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","AV_PIX_FMT_NONE == -1 && AV_SAMPLE_FMT_NONE == -1","avfilter.c",150);
      abort();
    }
  }while (0);
  link -> format = - 1;
  return 0;
}

void avfilter_link_free(AVFilterLink **link)
{
  if (!( *link)) {
    return ;
  }
  if (( *link) -> pool) {
    ff_free_pool(( *link) -> pool);
  }
  avfilter_unref_bufferp(&( *link) -> partial_buf);
  av_freep(link);
}

int avfilter_link_get_channels(AVFilterLink *link)
{
  return link -> channels;
}

void avfilter_link_set_closed(AVFilterLink *link,int closed)
{
  link -> closed = closed;
}

int avfilter_insert_filter(AVFilterLink *link,AVFilterContext *filt,unsigned int filt_srcpad_idx,unsigned int filt_dstpad_idx)
{
  int ret;
  unsigned int dstpad_idx = (link -> dstpad - link -> dst -> input_pads);
  av_log((link -> dst),40,"auto-inserting filter '%s' between the filter '%s' and the filter '%s'\n",filt -> name,link -> src -> name,link -> dst -> name);
  link -> dst -> inputs[dstpad_idx] = ((void *)0);
  if ((ret = avfilter_link(filt,filt_dstpad_idx,link -> dst,dstpad_idx)) < 0) {
/* failed to link output filter to new filter */
    link -> dst -> inputs[dstpad_idx] = link;
    return ret;
  }
/* re-hookup the link to the new destination filter we inserted */
  link -> dst = filt;
  link -> dstpad = &filt -> input_pads[filt_srcpad_idx];
  filt -> inputs[filt_srcpad_idx] = link;
/* if any information on supported media formats already exists on the
     * link, we need to preserve that */
  if (link -> out_formats) {
    ff_formats_changeref(&link -> out_formats,&filt -> outputs[filt_dstpad_idx] -> out_formats);
  }
  if (link -> out_samplerates) {
    ff_formats_changeref(&link -> out_samplerates,&filt -> outputs[filt_dstpad_idx] -> out_samplerates);
  }
  if (link -> out_channel_layouts) {
    ff_channel_layouts_changeref(&link -> out_channel_layouts,&filt -> outputs[filt_dstpad_idx] -> out_channel_layouts);
  }
  return 0;
}

int avfilter_config_links(AVFilterContext *filter)
{
  int (*config_link)(AVFilterLink *);
  unsigned int i;
  int ret;
  for (i = 0; i < filter -> nb_inputs; i++) {
    AVFilterLink *link = filter -> inputs[i];
    AVFilterLink *inlink;
    if (!link) {
      continue; 
    }
    inlink = (link -> src -> nb_inputs?link -> src -> inputs[0] : ((void *)0));
    link -> current_pts = ((int64_t )0x8000000000000000UL);
    switch(link -> init_state){
      case AVLINK_INIT:
      continue; 
      case AVLINK_STARTINIT:
{
        av_log(filter,32,"circular filter chain detected\n");
        return 0;
      }
      case AVLINK_UNINIT:
{
        link -> init_state = AVLINK_STARTINIT;
        if ((ret = avfilter_config_links(link -> src)) < 0) {
          return ret;
        }
        if (!(config_link = link -> srcpad -> config_props)) {
          if (link -> src -> nb_inputs != 1) {
            av_log((link -> src),16,"Source filters and filters with more than one input must set config_props() callbacks on all outputs\n");
            return - 22;
          }
        }
        else {
          if ((ret = config_link(link)) < 0) {
            av_log((link -> src),16,"Failed to configure output pad on %s\n",link -> src -> name);
            return ret;
          }
        }
        switch(link -> type){
          case AVMEDIA_TYPE_VIDEO:
{
            if (!link -> time_base . num && !link -> time_base . den) {
              link -> time_base = (inlink?inlink -> time_base : ((AVRational ){(1), (1000000)}));
            }
            if (!link -> sample_aspect_ratio . num && !link -> sample_aspect_ratio . den) {
              link -> sample_aspect_ratio = (inlink?inlink -> sample_aspect_ratio : ((AVRational ){(1), (1)}));
            }
            if (inlink && !link -> frame_rate . num && !link -> frame_rate . den) {
              link -> frame_rate = inlink -> frame_rate;
            }
            if (inlink) {
              if (!link -> w) {
                link -> w = inlink -> w;
              }
              if (!link -> h) {
                link -> h = inlink -> h;
              }
            }
            else {
              if (!link -> w || !link -> h) {
                av_log((link -> src),16,"Video source filters must set their output link's width and height\n");
                return - 22;
              }
            }
            break; 
          }
          case AVMEDIA_TYPE_AUDIO:
{
            if (inlink) {
              if (!link -> time_base . num && !link -> time_base . den) {
                link -> time_base = inlink -> time_base;
              }
            }
            if (!link -> time_base . num && !link -> time_base . den) {
              link -> time_base = ((AVRational ){(1), link -> sample_rate});
            }
          }
        }
        if (config_link = link -> dstpad -> config_props) {
          if ((ret = config_link(link)) < 0) {
            av_log((link -> src),16,"Failed to configure input pad on %s\n",link -> dst -> name);
            return ret;
          }
        }
        link -> init_state = AVLINK_INIT;
      }
    }
  }
  return 0;
}

void ff_tlog_link(void *ctx,AVFilterLink *link,int end)
{
  if ((link -> type) == AVMEDIA_TYPE_VIDEO) {
    do {
      if (0) {
        av_log(ctx,48,"link[%p s:%dx%d fmt:%s %s->%s]%s",link,link -> w,link -> h,av_get_pix_fmt_name((link -> format)),(link -> src?link -> src -> filter -> name : ""),(link -> dst?link -> dst -> filter -> name : ""),(end?"\n" : ""));
      }
    }while (0);
  }
  else {
    char buf[128];
    av_get_channel_layout_string(buf,(sizeof(buf)),- 1,link -> channel_layout);
    do {
      if (0) {
        av_log(ctx,48,"link[%p r:%d cl:%s fmt:%s %s->%s]%s",link,((int )(link -> sample_rate)),buf,av_get_sample_fmt_name((link -> format)),(link -> src?link -> src -> filter -> name : ""),(link -> dst?link -> dst -> filter -> name : ""),(end?"\n" : ""));
      }
    }while (0);
  }
}

int ff_request_frame(AVFilterLink *link)
{
  int ret = - 1;
  do {
    if (0) {
      av_log(((void *)0),48,"%-16s: ","request_frame");
    }
  }while (0);
  ff_tlog_link(((void *)0),link,1);
  if (link -> closed) {
    return -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
  }
  if (link -> srcpad -> request_frame) {
    ret = ((link -> srcpad -> request_frame)(link));
  }
  else {
    if (link -> src -> inputs[0]) {
      ret = ff_request_frame(link -> src -> inputs[0]);
    }
  }
  if (ret == -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24)) && link -> partial_buf) {
    AVFilterBufferRef *pbuf = link -> partial_buf;
    link -> partial_buf = ((void *)0);
    ff_filter_frame_framed(link,pbuf);
    return 0;
  }
  if (ret == -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24))) {
    link -> closed = 1;
  }
  return ret;
}

int ff_poll_frame(AVFilterLink *link)
{
  int i;
  int min = 2147483647;
  if (link -> srcpad -> poll_frame) {
    return (link -> srcpad -> poll_frame)(link);
  }
  for (i = 0; i < link -> src -> nb_inputs; i++) {
    int val;
    if (!link -> src -> inputs[i]) {
      return - 1;
    }
    val = ff_poll_frame(link -> src -> inputs[i]);
    min = (min > val?val : min);
  }
  return min;
}

void ff_update_link_current_pts(AVFilterLink *link,int64_t pts)
{
  if (pts == ((int64_t )0x8000000000000000UL)) {
    return ;
  }
  link -> current_pts = av_rescale_q(pts,link -> time_base,((AVRational ){(1), (1000000)}));
/* TODO use duration */
  if (link -> graph && link -> age_index >= 0) {
    ff_avfilter_graph_update_heap(link -> graph,link);
  }
}

int avfilter_process_command(AVFilterContext *filter,const char *cmd,const char *arg,char *res,int res_len,int flags)
{
  if (!strcmp(cmd,"ping")) {
    av_strlcatf(res,res_len,"pong from:%s %s\n",filter -> filter -> name,filter -> name);
    return 0;
  }
  else {
    if (filter -> filter -> process_command) {
      return (filter -> filter -> process_command)(filter,cmd,arg,res,res_len,flags);
    }
  }
  return - 38;
}
#define MAX_REGISTERED_AVFILTERS_NB 256
static AVFilter *registered_avfilters[256 + 1];
static int next_registered_avfilter_idx = 0;

AVFilter *avfilter_get_by_name(const char *name)
{
  int i;
  for (i = 0; registered_avfilters[i]; i++) 
    if (!strcmp(registered_avfilters[i] -> name,name)) {
      return registered_avfilters[i];
    }
  return ((void *)0);
}

int avfilter_register(AVFilter *filter)
{
  int i;
  if (next_registered_avfilter_idx == 256) {
    av_log(((void *)0),16,"Maximum number of registered filters %d reached, impossible to register filter with name '%s'\n",256,filter -> name);
    return - '\f';
  }
  for (i = 0; filter -> inputs && filter -> inputs[i] . name; i++) {
    const AVFilterPad *input = &filter -> inputs[i];
    do {
      if (!(!input -> filter_frame || !input -> start_frame && !input -> end_frame)) {
        av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","!input->filter_frame || (!input->start_frame && !input->end_frame)","avfilter.c",426);
        abort();
      }
    }while (0);
  }
  registered_avfilters[next_registered_avfilter_idx++] = filter;
  return 0;
}

AVFilter **av_filter_next(AVFilter **filter)
{
  return filter?++filter : &registered_avfilters[0];
}

void avfilter_uninit()
{
  memset(registered_avfilters,0,sizeof(registered_avfilters));
  next_registered_avfilter_idx = 0;
}

static int pad_count(const AVFilterPad *pads)
{
  int count;
  if (!pads) {
    return 0;
  }
  for (count = 0; pads -> name; count++) 
    pads++;
  return count;
}

static const char *default_filter_name(void *filter_ctx)
{
  AVFilterContext *ctx = filter_ctx;
  return ctx -> name?(ctx -> name) : ctx -> filter -> name;
}

static void *filter_child_next(void *obj,void *prev)
{
  AVFilterContext *ctx = obj;
  if (!prev && ctx -> filter && ctx -> filter -> priv_class) {
    return ctx -> priv;
  }
  return (void *)0;
}

static const AVClass *filter_child_class_next(const AVClass *prev)
{
  AVFilter **filter_ptr = ((void *)0);
/* find the filter that corresponds to prev */
  while(prev &&  *(filter_ptr = av_filter_next(filter_ptr)))
    if (( *filter_ptr) -> priv_class == prev) {
      break; 
    }
/* could not find filter corresponding to prev */
  if (prev && !( *filter_ptr)) {
    return ((void *)0);
  }
/* find next filter with specific options */
  while( *(filter_ptr = av_filter_next(filter_ptr)))
    if (( *filter_ptr) -> priv_class) {
      return ( *filter_ptr) -> priv_class;
    }
  return ((void *)0);
}
static const AVClass avfilter_class = {.class_name = "AVFilter", .item_name = default_filter_name, .version = 52 << 16 | 18 << 8 | 100, .category = AV_CLASS_CATEGORY_FILTER, .child_next = filter_child_next, .child_class_next = filter_child_class_next};

const AVClass *avfilter_get_class()
{
  return &avfilter_class;
}

int avfilter_open(AVFilterContext **filter_ctx,AVFilter *filter,const char *inst_name)
{
  AVFilterContext *ret;
   *filter_ctx = ((void *)0);
  if (!filter) {
    return - 22;
  }
  ret = (av_mallocz(sizeof(AVFilterContext )));
  if (!ret) {
    return - '\f';
  }
  ret -> av_class = &avfilter_class;
  ret -> filter = filter;
  ret -> name = (inst_name?av_strdup(inst_name) : ((void *)0));
  if (filter -> priv_size) {
    ret -> priv = av_mallocz((filter -> priv_size));
    if (!ret -> priv) {
      goto err;
    }
  }
  ret -> nb_inputs = (pad_count(filter -> inputs));
  if (ret -> nb_inputs) {
    ret -> input_pads = (av_malloc(sizeof(AVFilterPad ) * (ret -> nb_inputs)));
    if (!ret -> input_pads) {
      goto err;
    }
    memcpy((ret -> input_pads),(filter -> inputs),sizeof(AVFilterPad ) * (ret -> nb_inputs));
    ret -> inputs = (av_mallocz(sizeof(AVFilterLink *) * (ret -> nb_inputs)));
    if (!ret -> inputs) {
      goto err;
    }
  }
  ret -> nb_outputs = (pad_count(filter -> outputs));
  if (ret -> nb_outputs) {
    ret -> output_pads = (av_malloc(sizeof(AVFilterPad ) * (ret -> nb_outputs)));
    if (!ret -> output_pads) {
      goto err;
    }
    memcpy((ret -> output_pads),(filter -> outputs),sizeof(AVFilterPad ) * (ret -> nb_outputs));
    ret -> outputs = (av_mallocz(sizeof(AVFilterLink *) * (ret -> nb_outputs)));
    if (!ret -> outputs) {
      goto err;
    }
  }
#if FF_API_FOO_COUNT
  ret -> output_count = ret -> nb_outputs;
  ret -> input_count = ret -> nb_inputs;
#endif
   *filter_ctx = ret;
  return 0;
  err:
  av_freep((&ret -> inputs));
  av_freep((&ret -> input_pads));
  ret -> nb_inputs = 0;
  av_freep((&ret -> outputs));
  av_freep((&ret -> output_pads));
  ret -> nb_outputs = 0;
  av_freep((&ret -> priv));
  av_free(ret);
  return - '\f';
}

void avfilter_free(AVFilterContext *filter)
{
  int i;
  AVFilterLink *link;
  if (!filter) {
    return ;
  }
  if (filter -> filter -> uninit) {
    (filter -> filter -> uninit)(filter);
  }
  for (i = 0; i < filter -> nb_inputs; i++) {
    if (link = filter -> inputs[i]) {
      if (link -> src) {
        link -> src -> outputs[link -> srcpad - link -> src -> output_pads] = ((void *)0);
      }
      ff_formats_unref(&link -> in_formats);
      ff_formats_unref(&link -> out_formats);
      ff_formats_unref(&link -> in_samplerates);
      ff_formats_unref(&link -> out_samplerates);
      ff_channel_layouts_unref(&link -> in_channel_layouts);
      ff_channel_layouts_unref(&link -> out_channel_layouts);
    }
    avfilter_link_free(&link);
  }
  for (i = 0; i < filter -> nb_outputs; i++) {
    if (link = filter -> outputs[i]) {
      if (link -> dst) {
        link -> dst -> inputs[link -> dstpad - link -> dst -> input_pads] = ((void *)0);
      }
      ff_formats_unref(&link -> in_formats);
      ff_formats_unref(&link -> out_formats);
      ff_formats_unref(&link -> in_samplerates);
      ff_formats_unref(&link -> out_samplerates);
      ff_channel_layouts_unref(&link -> in_channel_layouts);
      ff_channel_layouts_unref(&link -> out_channel_layouts);
    }
    avfilter_link_free(&link);
  }
  av_freep((&filter -> name));
  av_freep((&filter -> input_pads));
  av_freep((&filter -> output_pads));
  av_freep((&filter -> inputs));
  av_freep((&filter -> outputs));
  av_freep((&filter -> priv));
  while(filter -> command_queue){
    ff_command_queue_pop(filter);
  }
  av_free(filter);
}

int avfilter_init_filter(AVFilterContext *filter,const char *args,void *opaque)
{
  int ret = 0;
  if (filter -> filter -> init_opaque) {
    ret = ((filter -> filter -> init_opaque)(filter,args,opaque));
  }
  else {
    if (filter -> filter -> init) {
      ret = ((filter -> filter -> init)(filter,args));
    }
  }
  return ret;
}

const char *avfilter_pad_get_name(AVFilterPad *pads,int pad_idx)
{
  return pads[pad_idx] . name;
}

enum AVMediaType avfilter_pad_get_type(AVFilterPad *pads,int pad_idx)
{
  return pads[pad_idx] . type;
}

static int default_filter_frame(AVFilterLink *link,AVFilterBufferRef *frame)
{
  return ff_filter_frame(link -> dst -> outputs[0],frame);
}

static int ff_filter_frame_framed(AVFilterLink *link,AVFilterBufferRef *frame)
{
  int (*filter_frame)(AVFilterLink *, AVFilterBufferRef *);
  AVFilterPad *src = link -> srcpad;
  AVFilterPad *dst = link -> dstpad;
  AVFilterBufferRef *out;
  int perms;
  int ret;
  AVFilterCommand *cmd = link -> dst -> command_queue;
  int64_t pts;
  if (link -> closed) {
    avfilter_unref_buffer(frame);
    return -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
  }
  if (!(filter_frame = dst -> filter_frame)) {
    filter_frame = default_filter_frame;
  }
  (void )0;
  frame -> perms &= ~src -> rej_perms;
  perms = frame -> perms;
  if (frame -> linesize[0] < 0) {
    perms |= 0x20;
  }
/* prepare to copy the frame if the buffer has insufficient permissions */
  if ((dst -> min_perms & perms) != dst -> min_perms || dst -> rej_perms & perms) {
    av_log((link -> dst),48,"Copying data in avfilter (have perms %x, need %x, reject %x)\n",perms,link -> dstpad -> min_perms,link -> dstpad -> rej_perms);
/* Maybe use ff_copy_buffer_ref instead? */
    switch(link -> type){
      case AVMEDIA_TYPE_VIDEO:
{
        out = ff_get_video_buffer(link,dst -> min_perms,link -> w,link -> h);
        break; 
      }
      case AVMEDIA_TYPE_AUDIO:
{
        out = ff_get_audio_buffer(link,dst -> min_perms,frame -> audio -> nb_samples);
        break; 
      }
      default:
      return - 22;
    }
    if (!out) {
      avfilter_unref_buffer(frame);
      return - '\f';
    }
    avfilter_copy_buffer_ref_props(out,frame);
    switch(link -> type){
      case AVMEDIA_TYPE_VIDEO:
{
        av_image_copy(out -> data,out -> linesize,((const uint8_t **)(frame -> data)),(frame -> linesize),(frame -> format),frame -> video -> w,frame -> video -> h);
        break; 
      }
      case AVMEDIA_TYPE_AUDIO:
{
        av_samples_copy(out -> extended_data,(frame -> extended_data),0,0,frame -> audio -> nb_samples,av_get_channel_layout_nb_channels(frame -> audio -> channel_layout),(frame -> format));
        break; 
      }
      default:
      return - 22;
    }
    avfilter_unref_buffer(frame);
  }
  else {
    out = frame;
  }
  while(cmd && cmd -> time <= (out -> pts) * av_q2d(link -> time_base)){
    av_log((link -> dst),48,"Processing command time:%f command:%s arg:%s\n",cmd -> time,cmd -> command,cmd -> arg);
    avfilter_process_command(link -> dst,(cmd -> command),(cmd -> arg),0,0,cmd -> flags);
    ff_command_queue_pop(link -> dst);
    cmd = link -> dst -> command_queue;
  }
  pts = out -> pts;
  ret = filter_frame(link,out);
  ff_update_link_current_pts(link,pts);
  return ret;
}

static int ff_filter_frame_needs_framing(AVFilterLink *link,AVFilterBufferRef *frame)
{
  int insamples = frame -> audio -> nb_samples;
  int inpos = 0;
  int nb_samples;
  AVFilterBufferRef *pbuf = link -> partial_buf;
  int nb_channels = frame -> audio -> channels;
  int ret = 0;
/* Handle framing (min_samples, max_samples) */
  while(insamples){
    if (!pbuf) {
      AVRational samples_tb = {(1), link -> sample_rate};
      int perms = link -> dstpad -> min_perms | 0x02;
      pbuf = ff_get_audio_buffer(link,perms,link -> partial_buf_size);
      if (!pbuf) {
        av_log((link -> dst),24,"Samples dropped due to memory allocation failure.\n");
        return 0;
      }
      avfilter_copy_buffer_ref_props(pbuf,frame);
      pbuf -> pts = frame -> pts + av_rescale_q(inpos,samples_tb,link -> time_base);
      pbuf -> audio -> nb_samples = 0;
    }
    nb_samples = (insamples > link -> partial_buf_size - pbuf -> audio -> nb_samples?link -> partial_buf_size - pbuf -> audio -> nb_samples : insamples);
    av_samples_copy(pbuf -> extended_data,(frame -> extended_data),pbuf -> audio -> nb_samples,inpos,nb_samples,nb_channels,(link -> format));
    inpos += nb_samples;
    insamples -= nb_samples;
    pbuf -> audio -> nb_samples += nb_samples;
    if (pbuf -> audio -> nb_samples >= link -> min_samples) {
      ret = ff_filter_frame_framed(link,pbuf);
      pbuf = ((void *)0);
    }
  }
  avfilter_unref_buffer(frame);
  link -> partial_buf = pbuf;
  return ret;
}

int ff_filter_frame(AVFilterLink *link,AVFilterBufferRef *frame)
{
  do {
    if (0) {
      av_log(((void *)0),48,"%-16s: ","filter_frame");
    }
  }while (0);
  ff_tlog_link(((void *)0),link,1);
  do {
    if (0) {
      av_log(((void *)0),48," ");
    }
  }while (0);
  ff_tlog_ref(((void *)0),frame,1);
/* Consistency checks */
  if ((link -> type) == AVMEDIA_TYPE_VIDEO) {
    if (strcmp(link -> dst -> filter -> name,"scale")) {
      (void )0;
      (void )0;
      (void )0;
    }
  }
  else {
    (void )0;
    (void )0;
    (void )0;
    (void )0;
  }
/* Go directly to actual filtering if possible */
  if ((link -> type) == AVMEDIA_TYPE_AUDIO && link -> min_samples && (link -> partial_buf || frame -> audio -> nb_samples < link -> min_samples || frame -> audio -> nb_samples > link -> max_samples)) {
    return ff_filter_frame_needs_framing(link,frame);
  }
  else {
    return ff_filter_frame_framed(link,frame);
  }
}

void disimprisonment_decenaries(int lisiere_overannotate,abjections_cacus amendatory_pointlessly)
{
 int stonesoup_cmp_flag = 0;
  char *galangal_refugee = 0;
  ++stonesoup_global_variable;
  lisiere_overannotate--;
  if (lisiere_overannotate > 0) {
    presurprise_miscodes(lisiere_overannotate,amendatory_pointlessly);
    return ;
  }
  galangal_refugee = ((char *)amendatory_pointlessly);
    tracepoint(stonesoup_trace, weakness_start, "CWE822", "A", "Untrusted Pointer Dereference");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
 /* STONESOUP: CROSSOVER-POINT (Untrusted Pointer Deference) */
 stonesoup_fct_ptr stonesoup_fp;
 const char *stonesoup_rand_word = "criticisms_metallide";
 stonesoup_fp = stonesoup_switch_func(galangal_refugee);
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
 /* STONESOUP: TRIGGER-POINT (Untrusted Pointer Dereference) */
    tracepoint(stonesoup_trace, variable_address, "stonesoup_fp", stonesoup_fp, "TRIGGER-STATE");
    stonesoup_cmp_flag = ( *stonesoup_fp)(stonesoup_rand_word,galangal_refugee);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    if (stonesoup_cmp_flag == 0)
        stonesoup_printf("strings are equal\n");
    else
        stonesoup_printf("strings are not equal\n");
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
}

void presurprise_miscodes(int jodhpur_trepang,abjections_cacus witchings_sulphatize)
{
  ++stonesoup_global_variable;
  disimprisonment_decenaries(jodhpur_trepang,witchings_sulphatize);
}
