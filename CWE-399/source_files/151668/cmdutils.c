/*
 * Various utilities for command line tools
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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
/* Include only the enabled headers since some compilers (namely, Sun
   Studio) will not omit unused inline functions and create undefined
   references to libraries that are not being built. */
#include "config.h"
#include "compat/va_copy.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavdevice/avdevice.h"
#include "libavresample/avresample.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#if CONFIG_POSTPROC
#include "libpostproc/postprocess.h"
#endif
#include "libavutil/avassert.h"
#include "libavutil/avstring.h"
#include "libavutil/bprint.h"
#include "libavutil/mathematics.h"
#include "libavutil/imgutils.h"
#include "libavutil/parseutils.h"
#include "libavutil/pixdesc.h"
#include "libavutil/eval.h"
#include "libavutil/dict.h"
#include "libavutil/opt.h"
#include "cmdutils.h"
#include "version.h"
#if CONFIG_NETWORK
#include "libavformat/network.h"
#endif
#if HAVE_SYS_RESOURCE_H
#include <sys/time.h>
#include <sys/resource.h>
#endif
#if STONESOUP_TRACE
#define TRACEPOINT_DEFINE
#define TRACEPOINT_PROBE_DYNAMIC_LINKAGE
#include <stonesoup/stonesoup_trace.h>
#endif
static int init_report(const char *env);
struct SwsContext *sws_opts;
AVDictionary *swr_opts;
AVDictionary *format_opts;
AVDictionary *codec_opts;
AVDictionary *resample_opts;
const int this_year = 2013;
static FILE *report_file;
int scutches_ashburnham = 0;
int stonesoup_global_variable;
void xerophilous_throop(char **lactivorous_calotermitidae);
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
void pfeffernuss_logotype(void (*palmettos_labouredly)(char **));
void stonesoup_cleanup(FILE **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpaJlHVY_ss_testcase/src-rose/cmdutils.c", "stonesoup_cleanup");
  for (i = 0; i < size; i++) {
    if (ptrs[i] != 0) {
      fclose(ptrs[i]);
    }
  }
}

void init_opts()
{
  if (1) {
    sws_opts = sws_getContext(16,16,AV_PIX_FMT_YUV420P,16,16,AV_PIX_FMT_YUV420P,4,((void *)0),((void *)0),((void *)0));
  }
}

void uninit_opts()
{
#if CONFIG_SWSCALE
  sws_freeContext(sws_opts);
  sws_opts = ((void *)0);
#endif
  av_dict_free(&swr_opts);
  av_dict_free(&format_opts);
  av_dict_free(&codec_opts);
  av_dict_free(&resample_opts);
}

void log_callback_help(void *ptr,int level,const char *fmt,va_list vl)
{
  vfprintf(stdout,fmt,vl);
}

static void log_callback_report(void *ptr,int level,const char *fmt,va_list vl)
{
  va_list vl2;
  char line[1024];
  static int print_prefix = 1;
  __builtin_va_copy(vl2,vl);
  av_log_default_callback(ptr,level,fmt,vl);
  av_log_format_line(ptr,level,fmt,vl2,line,(sizeof(line)),&print_prefix);
  __builtin_va_end(vl2);
  fputs(line,report_file);
  fflush(report_file);
}

double parse_number_or_die(const char *context,const char *numstr,int type,double min,double max)
{
  char *tail;
  const char *error;
  double d = av_strtod(numstr,&tail);
  if ( *tail) {
    error = "Expected number for %s but found: %s\n";
  }
  else {
    if (d < min || d > max) {
      error = "The value for %s was %s which is not within %f - %f\n";
    }
    else {
      if (type == 0x0400 && ((int64_t )d) != d) {
        error = "Expected int64 for %s but found %s\n";
      }
      else {
        if (type == 0x80 && ((int )d) != d) {
          error = "Expected int for %s but found %s\n";
        }
        else {
          return d;
        }
      }
    }
  }
  av_log(((void *)0),8,error,context,numstr,min,max);
  exit(1);
  return 0;
}

int64_t parse_time_or_die(const char *context,const char *timestr,int is_duration)
{
  int64_t us;
  if (av_parse_time(&us,timestr,is_duration) < 0) {
    av_log(((void *)0),8,"Invalid %s specification for %s: %s\n",(is_duration?"duration" : "date"),context,timestr);
    exit(1);
  }
  return us;
}

void show_help_options(const OptionDef *options,const char *msg,int req_flags,int rej_flags,int alt_flags)
{
  const OptionDef *po;
  int first;
  first = 1;
  for (po = options; po -> name != ((void *)0); po++) {
    char buf[64];
    if ((po -> flags & req_flags) != req_flags || alt_flags && !(po -> flags & alt_flags) || po -> flags & rej_flags) {
      continue; 
    }
    if (first) {
      printf("%s\n",msg);
      first = 0;
    }
    av_strlcpy(buf,po -> name,sizeof(buf));
    if (po -> argname) {
      av_strlcat(buf," ",sizeof(buf));
      av_strlcat(buf,po -> argname,sizeof(buf));
    }
    printf("-%-17s  %s\n",buf,po -> help);
  }
  printf("\n");
}

void show_help_children(const AVClass *class,int flags)
{
  const AVClass *child = ((void *)0);
  if (class -> option) {
    av_opt_show2((&class),((void *)0),flags,0);
    printf("\n");
  }
  while(child = av_opt_child_class_next(class,child))
    show_help_children(child,flags);
}

static const OptionDef *find_option(const OptionDef *po,const char *name)
{
  const char *p = (strchr(name,':'));
  int len = (p?(p - name) : strlen(name));
  while(po -> name != ((void *)0)){
    if (!strncmp(name,po -> name,len) && strlen(po -> name) == len) {
      break; 
    }
    po++;
  }
  return po;
}
#if HAVE_COMMANDLINETOARGVW
#include <windows.h>
#include <shellapi.h>
/* Will be leaked on exit */
/**
 * Prepare command line arguments for executable.
 * For Windows - perform wide-char to UTF-8 conversion.
 * Input arguments should be main() function arguments.
 * @param argc_ptr Arguments number (including executable)
 * @param argv_ptr Arguments list.
 */
/* determine the UTF-8 buffer size (including NULL-termination symbols) */
#else

inline static void prepare_app_arguments(int *argc_ptr,char ***argv_ptr)
{
/* nothing to do */
}
#endif /* HAVE_COMMANDLINETOARGVW */

static int write_option(void *optctx,const OptionDef *po,const char *opt,const char *arg)
{
/* new-style options contain an offset into optctx, old-style address of
     * a global var*/
  void *dst = po -> flags & (0x4000 | 0x8000)?(((uint8_t *)optctx) + po -> u . off) : po -> u . dst_ptr;
  int *dstcount;
  if (po -> flags & 0x8000) {
    SpecifierOpt **so = dst;
    char *p = strchr(opt,':');
    dstcount = ((int *)(so + 1));
     *so = (grow_array(( *so),(sizeof(( *( *so)))),dstcount, *dstcount + 1));
    ( *so)[ *dstcount - 1] . specifier = av_strdup(((p?p + 1 : "")));
    dst = (&( *so)[ *dstcount - 1] . u);
  }
  if (po -> flags & 0x0008) {
    char *str;
    str = av_strdup(arg);
//         av_freep(dst);
     *((char **)dst) = str;
  }
  else {
    if (po -> flags & 0x00002 || po -> flags & 0x80) {
       *((int *)dst) = (parse_number_or_die(opt,arg,0x0400,(- 2147483647 - 1),2147483647));
    }
    else {
      if (po -> flags & 0x0400) {
         *((int64_t *)dst) = (parse_number_or_die(opt,arg,0x0400,(- 9223372036854775807L - 1),9223372036854775807L));
      }
      else {
        if (po -> flags & 0x10000) {
           *((int64_t *)dst) = parse_time_or_die(opt,arg,1);
        }
        else {
          if (po -> flags & 0x00100) {
             *((float *)dst) = (parse_number_or_die(opt,arg,0x00100,(-__builtin_inff()),(__builtin_inff())));
          }
          else {
            if (po -> flags & 0x20000) {
               *((double *)dst) = parse_number_or_die(opt,arg,0x20000,(-__builtin_inff()),(__builtin_inff()));
            }
            else {
              if (po -> u . func_arg) {
                int ret = po -> u . func_arg(optctx,opt,arg);
                if (ret < 0) {
                  av_log(((void *)0),16,"Failed to set value '%s' for option '%s'\n",arg,opt);
                  return ret;
                }
              }
            }
          }
        }
      }
    }
  }
  if (po -> flags & 0x0800) {
    exit(0);
  }
  return 0;
}

int parse_option(void *optctx,const char *opt,const char *arg,const OptionDef *options)
{
  const OptionDef *po;
  int ret;
  po = find_option(options,opt);
  if (!po -> name && opt[0] == 'n' && opt[1] == 'o') {
/* handle 'no' bool option */
    po = find_option(options,opt + 2);
    if (po -> name && po -> flags & 0x00002) {
      arg = "0";
    }
  }
  else {
    if (po -> flags & 0x00002) {
      arg = "1";
    }
  }
  if (!po -> name) {
    po = find_option(options,"default");
  }
  if (!po -> name) {
    av_log(((void *)0),16,"Unrecognized option '%s'\n",opt);
    return - 22;
  }
  if (po -> flags & 0x0001 && !arg) {
    av_log(((void *)0),16,"Missing argument for option '%s'\n",opt);
    return - 22;
  }
  ret = write_option(optctx,po,opt,arg);
  if (ret < 0) {
    return ret;
  }
  return !(!(po -> flags & 0x0001));
}

void parse_options(void *optctx,int argc,char **argv,const OptionDef *options,void (*parse_arg_function)(void *, const char *))
{
  const char *opt;
  int optindex;
  int handleoptions = 1;
  int ret;
/* perform system-dependent conversions for arguments list */
  prepare_app_arguments(&argc,&argv);
/* parse options */
  optindex = 1;
  while(optindex < argc){
    opt = argv[optindex++];
    if (handleoptions && opt[0] == '-' && opt[1] != '\0') {
      if (opt[1] == '-' && opt[2] == '\0') {
        handleoptions = 0;
        continue; 
      }
      opt++;
      if ((ret = parse_option(optctx,opt,argv[optindex],options)) < 0) {
        exit(1);
      }
      optindex += ret;
    }
    else {
      if (parse_arg_function) {
        parse_arg_function(optctx,opt);
      }
    }
  }
}

int parse_optgroup(void *optctx,OptionGroup *g)
{
  int i;
  int ret;
  av_log(((void *)0),48,"Parsing a group of options: %s %s.\n",g -> group_def -> name,g -> arg);
  for (i = 0; i < g -> nb_opts; i++) {
    Option *o = &g -> opts[i];
    av_log(((void *)0),48,"Applying option %s (%s) with argument %s.\n",o -> key,o -> opt -> help,o -> val);
    ret = write_option(optctx,o -> opt,o -> key,o -> val);
    if (ret < 0) {
      return ret;
    }
  }
  av_log(((void *)0),48,"Successfully parsed a group of options.\n");
  return 0;
}

int locate_option(int argc,char **argv,const OptionDef *options,const char *optname)
{
  const OptionDef *po;
  int i;
  for (i = 1; i < argc; i++) {
    const char *cur_opt = argv[i];
    if (( *(cur_opt++)) != '-') {
      continue; 
    }
    po = find_option(options,cur_opt);
    if (!po -> name && cur_opt[0] == 'n' && cur_opt[1] == 'o') {
      po = find_option(options,cur_opt + 2);
    }
    if (!po -> name && !strcmp(cur_opt,optname) || po -> name && !strcmp(optname,po -> name)) {
      return i;
    }
    if (po -> flags & 0x0001) {
      i++;
    }
  }
  return 0;
}

static void dump_argument(const char *a)
{
  const unsigned char *p;
  for (p = a;  *p; p++) 
    if (!(( *p) >= '+' && ( *p) <= ':' || ( *p) >= 64 && ( *p) <= 'Z' || ( *p) == '_' || ( *p) >= 'a' && ( *p) <= 'z')) {
      break; 
    }
  if (!( *p)) {
    fputs(a,report_file);
    return ;
  }
  fputc('"',report_file);
  for (p = a;  *p; p++) {
    if (( *p) == '\\' || ( *p) == '"' || ( *p) == '$' || ( *p) == '`') {
      fprintf(report_file,"\\%c",( *p));
    }
    else {
      if (( *p) < 32 || ( *p) > '~') {
        fprintf(report_file,"\\x%02x",( *p));
      }
      else {
        fputc(( *p),report_file);
      }
    }
  }
  fputc('"',report_file);
}

void parse_loglevel(int argc,char **argv,const OptionDef *options)
{
  int idx = locate_option(argc,argv,options,"loglevel");
  const char *env;
  if (!idx) {
    idx = locate_option(argc,argv,options,"v");
  }
  if (idx && argv[idx + 1]) {
    opt_loglevel(((void *)0),"loglevel",argv[idx + 1]);
  }
  idx = locate_option(argc,argv,options,"report");
  if ((env = (getenv("FFREPORT"))) || idx) {
    init_report(env);
    if (report_file) {
      int i;
      fprintf(report_file,"Command line:\n");
      for (i = 0; i < argc; i++) {
        dump_argument(argv[i]);
        fputc((i < argc - 1?32 : '\n'),report_file);
      }
      fflush(report_file);
    }
  }
}
#define FLAGS (o->type == AV_OPT_TYPE_FLAGS) ? AV_DICT_APPEND : 0

int opt_default(void *optctx,const char *opt,const char *arg)
{
  const AVOption *o;
  int consumed = 0;
  char opt_stripped[128];
  const char *p;
  const AVClass *cc = avcodec_get_class();
  const AVClass *fc = avformat_get_class();
#if CONFIG_AVRESAMPLE
#endif
  const AVClass *sc;
  const AVClass *swr_class;
  if (!strcmp(opt,"debug") || !strcmp(opt,"fdebug")) {
    av_log_set_level(48);
  }
  if (!(p = (strchr(opt,':')))) {
    p = opt + strlen(opt);
  }
  av_strlcpy(opt_stripped,opt,(sizeof(opt_stripped) > (p - opt + 1)?(p - opt + 1) : sizeof(opt_stripped)));
  if ((o = av_opt_find((&cc),opt_stripped,((void *)0),0,0x0001 | 0x00002)) || (opt[0] == 'v' || opt[0] == 'a' || opt[0] == 's') && (o = av_opt_find((&cc),opt + 1,((void *)0),0,0x00002))) {
    av_dict_set(&codec_opts,opt,arg,((o -> type) == AV_OPT_TYPE_FLAGS?32 : 0));
    consumed = 1;
  }
  if (o = av_opt_find((&fc),opt,((void *)0),0,0x0001 | 0x00002)) {
    av_dict_set(&format_opts,opt,arg,((o -> type) == AV_OPT_TYPE_FLAGS?32 : 0));
    if (consumed) {
      av_log(((void *)0),40,"Routing %s to codec and muxer layer\n",opt);
    }
    consumed = 1;
  }
#if CONFIG_SWSCALE
  sc = sws_get_class();
  if (!consumed && av_opt_find((&sc),opt,((void *)0),0,0x0001 | 0x00002)) {
// XXX we only support sws_flags, not arbitrary sws options
    int ret = av_opt_set(sws_opts,opt,arg,0);
    if (ret < 0) {
      av_log(((void *)0),16,"Error setting option %s.\n",opt);
      return ret;
    }
    consumed = 1;
  }
#endif
#if CONFIG_SWRESAMPLE
  swr_class = swr_get_class();
  if (!consumed && (o = av_opt_find((&swr_class),opt,((void *)0),0,0x0001 | 0x00002))) {
    struct SwrContext *swr = swr_alloc();
    int ret = av_opt_set(swr,opt,arg,0);
    swr_free(&swr);
    if (ret < 0) {
      av_log(((void *)0),16,"Error setting option %s.\n",opt);
      return ret;
    }
    av_dict_set(&swr_opts,opt,arg,((o -> type) == AV_OPT_TYPE_FLAGS?32 : 0));
    consumed = 1;
  }
#endif
#if CONFIG_AVRESAMPLE
#endif
  if (consumed) {
    return 0;
  }
  return -((int )((0xF8 | 'O' << 8 | 'P' << 16) | ((unsigned int )'T') << 24));
}
/*
 * Check whether given option is a group separator.
 *
 * @return index of the group definition that matched or -1 if none
 */

static int match_group_separator(const OptionGroupDef *groups,int nb_groups,const char *opt)
{
  int i;
  for (i = 0; i < nb_groups; i++) {
    const OptionGroupDef *p = &groups[i];
    if (p -> sep && !strcmp(p -> sep,opt)) {
      return i;
    }
  }
  return - 1;
}
/*
 * Finish parsing an option group.
 *
 * @param group_idx which group definition should this group belong to
 * @param arg argument of the group delimiting option
 */

static void finish_group(OptionParseContext *octx,int group_idx,const char *arg)
{
  OptionGroupList *l = &octx -> groups[group_idx];
  OptionGroup *g;
  l -> groups = (grow_array((l -> groups),(sizeof(( *l -> groups))),&l -> nb_groups,l -> nb_groups + 1));
  g = &l -> groups[l -> nb_groups - 1];
   *g = octx -> cur_group;
  g -> arg = arg;
  g -> group_def = l -> group_def;
#if CONFIG_SWSCALE
  g -> sws_opts = sws_opts;
#endif
  g -> swr_opts = swr_opts;
  g -> codec_opts = codec_opts;
  g -> format_opts = format_opts;
  g -> resample_opts = resample_opts;
  codec_opts = ((void *)0);
  format_opts = ((void *)0);
  resample_opts = ((void *)0);
#if CONFIG_SWSCALE
  sws_opts = ((void *)0);
#endif
  swr_opts = ((void *)0);
  init_opts();
  memset((&octx -> cur_group),0,sizeof(octx -> cur_group));
}
/*
 * Add an option instance to currently parsed group.
 */

static void add_opt(OptionParseContext *octx,const OptionDef *opt,const char *key,const char *val)
{
  int global = !(opt -> flags & (0x2000 | 0x8000 | 0x4000));
  OptionGroup *g = global?&octx -> global_opts : &octx -> cur_group;
  g -> opts = (grow_array((g -> opts),(sizeof(( *g -> opts))),&g -> nb_opts,g -> nb_opts + 1));
  g -> opts[g -> nb_opts - 1] . opt = opt;
  g -> opts[g -> nb_opts - 1] . key = key;
  g -> opts[g -> nb_opts - 1] . val = val;
}

static void init_parse_context(OptionParseContext *octx,const OptionGroupDef *groups,int nb_groups)
{
  static const OptionGroupDef global_group = {("global")};
  int i;
  memset(octx,0,sizeof(( *octx)));
  octx -> nb_groups = nb_groups;
  octx -> groups = (av_mallocz(sizeof(( *octx -> groups)) * (octx -> nb_groups)));
  if (!octx -> groups) {
    exit(1);
  }
  for (i = 0; i < octx -> nb_groups; i++) 
    octx -> groups[i] . group_def = &groups[i];
  octx -> global_opts . group_def = &global_group;
  octx -> global_opts . arg = "";
  init_opts();
}

void uninit_parse_context(OptionParseContext *octx)
{
  int i;
  int j;
  for (i = 0; i < octx -> nb_groups; i++) {
    OptionGroupList *l = &octx -> groups[i];
    for (j = 0; j < l -> nb_groups; j++) {
      av_freep((&l -> groups[j] . opts));
      av_dict_free(&l -> groups[j] . codec_opts);
      av_dict_free(&l -> groups[j] . format_opts);
      av_dict_free(&l -> groups[j] . resample_opts);
#if CONFIG_SWSCALE
      sws_freeContext(l -> groups[j] . sws_opts);
#endif
      av_dict_free(&l -> groups[j] . swr_opts);
    }
    av_freep((&l -> groups));
  }
  av_freep((&octx -> groups));
  av_freep((&octx -> cur_group . opts));
  av_freep((&octx -> global_opts . opts));
  uninit_opts();
}

int split_commandline(OptionParseContext *octx,int argc,char *argv[],const OptionDef *options,const OptionGroupDef *groups,int nb_groups)
{
  int optindex = 1;
  int dashdash = - 2;
/* perform system-dependent conversions for arguments list */
  prepare_app_arguments(&argc,&argv);
  init_parse_context(octx,groups,nb_groups);
  av_log(((void *)0),48,"Splitting the commandline.\n");
  while(optindex < argc){
    const char *opt = argv[optindex++];
    const char *arg;
    const OptionDef *po;
    int ret;
    av_log(((void *)0),48,"Reading option '%s' ...",opt);
    if (opt[0] == '-' && opt[1] == '-' && !opt[2]) {
      dashdash = optindex;
      continue; 
    }
/* unnamed group separators, e.g. output filename */
    if (opt[0] != '-' || !opt[1] || dashdash + 1 == optindex) {
      finish_group(octx,0,opt);
      av_log(((void *)0),48," matched as %s.\n",groups[0] . name);
      continue; 
    }
    opt++;
#define GET_ARG(arg)                                                           \
do {                                                                           \
    arg = argv[optindex++];                                                    \
    if (!arg) {                                                                \
        av_log(NULL, AV_LOG_ERROR, "Missing argument for option '%s'.\n", opt);\
        return AVERROR(EINVAL);                                                \
    }                                                                          \
} while (0)
/* named group separators, e.g. -i */
    if ((ret = match_group_separator(groups,nb_groups,opt)) >= 0) {
      do {
        arg = argv[optindex++];
        if (!arg) {
          av_log(((void *)0),16,"Missing argument for option '%s'.\n",opt);
          return - 22;
        }
      }while (0);
      finish_group(octx,ret,arg);
      av_log(((void *)0),48," matched as %s with argument '%s'.\n",groups[ret] . name,arg);
      continue; 
    }
/* normal options */
    po = find_option(options,opt);
    if (po -> name) {
      if (po -> flags & 0x0800) {
/* optional argument, e.g. -h */
        arg = argv[optindex++];
      }
      else {
        if (po -> flags & 0x0001) {
          do {
            arg = argv[optindex++];
            if (!arg) {
              av_log(((void *)0),16,"Missing argument for option '%s'.\n",opt);
              return - 22;
            }
          }while (0);
        }
        else {
          arg = "1";
        }
      }
      add_opt(octx,po,opt,arg);
      av_log(((void *)0),48," matched as option '%s' (%s) with argument '%s'.\n",po -> name,po -> help,arg);
      continue; 
    }
/* AVOptions */
    if (argv[optindex]) {
      ret = opt_default(((void *)0),opt,argv[optindex]);
      if (ret >= 0) {
        av_log(((void *)0),48," matched as AVOption '%s' with argument '%s'.\n",opt,argv[optindex]);
        optindex++;
        continue; 
      }
      else {
        if (ret != -((int )((0xF8 | 'O' << 8 | 'P' << 16) | ((unsigned int )'T') << 24))) {
          av_log(((void *)0),16,"Error parsing option '%s' with argument '%s'.\n",opt,argv[optindex]);
          return ret;
        }
      }
    }
/* boolean -nofoo options */
    if (opt[0] == 'n' && opt[1] == 'o' && (po = find_option(options,opt + 2)) && po -> name && po -> flags & 0x00002) {
      add_opt(octx,po,opt,"0");
      av_log(((void *)0),48," matched as option '%s' (%s) with argument 0.\n",po -> name,po -> help);
      continue; 
    }
    av_log(((void *)0),16,"Unrecognized option '%s'.\n",opt);
    return -((int )((0xF8 | 'O' << 8 | 'P' << 16) | ((unsigned int )'T') << 24));
  }
  if (octx -> cur_group . nb_opts || codec_opts || format_opts || resample_opts) {
    av_log(((void *)0),24,"Trailing options were found on the commandline.\n");
  }
  av_log(((void *)0),48,"Finished splitting the commandline.\n");
  return 0;
}

int opt_loglevel(void *optctx,const char *opt,const char *arg)
{
  const struct {
  const char *name;
  int level;}log_levels[] = {{"quiet", (- 8)}, {"panic", (0)}, {"fatal", (8)}, {"error", (16)}, {"warning", (24)}, {"info", (32)}, {"verbose", (40)}, {"debug", (48)}};
  char *tail;
  int level;
  int i;
  for (i = 0; i < sizeof(log_levels) / sizeof(log_levels[0]); i++) {
    if (!strcmp(log_levels[i] . name,arg)) {
      av_log_set_level(log_levels[i] . level);
      return 0;
    }
  }
  level = (strtol(arg,&tail,'\n'));
  if ( *tail) {
    av_log(((void *)0),8,"Invalid loglevel \"%s\". Possible levels are numbers or:\n",arg);
    for (i = 0; i < sizeof(log_levels) / sizeof(log_levels[0]); i++) 
      av_log(((void *)0),8,"\"%s\"\n",log_levels[i] . name);
    exit(1);
  }
  av_log_set_level(level);
  return 0;
}

static void expand_filename_template(AVBPrint *bp,const char *template,struct tm *tm)
{
  int c;
  while(c = ( *(template++))){
    if (c == '%') {
      if (!(c = ( *(template++)))) {
        break; 
      }
      switch(c){
        case 'p':
{
          av_bprintf(bp,"%s",program_name);
          break; 
        }
        case 't':
{
          av_bprintf(bp,"%04d%02d%02d-%02d%02d%02d",tm -> tm_year + 1900,tm -> tm_mon + 1,tm -> tm_mday,tm -> tm_hour,tm -> tm_min,tm -> tm_sec);
          break; 
        }
        case '%':
{
          av_bprint_chars(bp,c,1);
          break; 
        }
      }
    }
    else {
      av_bprint_chars(bp,c,1);
    }
  }
}

static int init_report(const char *env)
{
  char *filename_template = ((void *)0);
  char *key;
  char *val;
  int ret;
  int count = 0;
  time_t now;
  struct tm *tm;
  AVBPrint filename;
/* already opened */
  if (report_file) {
    return 0;
  }
  time(&now);
  tm = localtime((&now));
  while(env &&  *env){
    if ((ret = av_opt_get_key_value(&env,"=",":",0,&key,&val)) < 0) {
      if (count) {
        av_log(((void *)0),16,"Failed to parse FFREPORT environment variable: %s\n",av_make_error_string(((char [64]){(0)}),64,ret));
      }
      break; 
    }
    if ( *env) {
      env++;
    }
    count++;
    if (!strcmp(key,"file")) {
      av_free(filename_template);
      filename_template = val;
      val = ((void *)0);
    }
    else {
      av_log(((void *)0),16,"Unknown key '%s' in FFREPORT\n",key);
    }
    av_free(val);
    av_free(key);
  }
  av_bprint_init(&filename,0,1);
  expand_filename_template(&filename,(av_x_if_null(filename_template,"%p-%t.log")),tm);
  av_free(filename_template);
  if (!av_bprint_is_complete(&filename)) {
    av_log(((void *)0),16,"Out of memory building report file name\n");
    return - 12;
  }
  report_file = fopen(filename . str,"w");
  if (!report_file) {
    av_log(((void *)0),16,"Failed to open report \"%s\": %s\n",filename . str,strerror( *__errno_location()));
    return -( *__errno_location());
  }
  av_log_set_callback(log_callback_report);
  av_log(((void *)0),32,"%s started on %04d-%02d-%02d at %02d:%02d:%02d\nReport written to \"%s\"\n",program_name,tm -> tm_year + 1900,tm -> tm_mon + 1,tm -> tm_mday,tm -> tm_hour,tm -> tm_min,tm -> tm_sec,filename . str);
  av_log_set_level((av_log_get_level() > 40?av_log_get_level() : 40));
  av_bprint_finalize(&filename,((void *)0));
  return 0;
}

int opt_report(const char *opt)
{
  return init_report(((void *)0));
}

int opt_max_alloc(void *optctx,const char *opt,const char *arg)
{
  char *tail;
  size_t max;
  max = (strtol(arg,&tail,'\n'));
  if ( *tail) {
    av_log(((void *)0),8,"Invalid max_alloc \"%s\".\n",arg);
    exit(1);
  }
  av_max_alloc(max);
  return 0;
}

int opt_cpuflags(void *optctx,const char *opt,const char *arg)
{
  int ret;
  unsigned int flags = (av_get_cpu_flags());
  if ((ret = av_parse_cpu_caps(&flags,arg)) < 0) {
    return ret;
  }
  av_force_cpu_flags(flags);
  return 0;
}

int opt_timelimit(void *optctx,const char *opt,const char *arg)
{
#if HAVE_SETRLIMIT
  int lim = (parse_number_or_die(opt,arg,0x0400,0,2147483647));
  struct rlimit rl = {lim, (lim + 1)};
  if (setrlimit(RLIMIT_CPU,(&rl))) {
    perror("setrlimit");
  }
#else
#endif
  return 0;
}

void print_error(const char *filename,int err)
{
  char errbuf[128];
  const char *errbuf_ptr = errbuf;
  if (av_strerror(err,errbuf,sizeof(errbuf)) < 0) {
    errbuf_ptr = (strerror(-err));
  }
  av_log(((void *)0),16,"%s: %s\n",filename,errbuf_ptr);
}
static int warned_cfg = 0;
#define INDENT        1
#define SHOW_VERSION  2
#define SHOW_CONFIG   4
#define SHOW_COPYRIGHT 8
#define PRINT_LIB_INFO(libname, LIBNAME, flags, level)                  \
    if (CONFIG_##LIBNAME) {                                             \
        const char *indent = flags & INDENT? "  " : "";                 \
        if (flags & SHOW_VERSION) {                                     \
            unsigned int version = libname##_version();                 \
            av_log(NULL, level,                                         \
                   "%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",            \
                   indent, #libname,                                    \
                   LIB##LIBNAME##_VERSION_MAJOR,                        \
                   LIB##LIBNAME##_VERSION_MINOR,                        \
                   LIB##LIBNAME##_VERSION_MICRO,                        \
                   version >> 16, version >> 8 & 0xff, version & 0xff); \
        }                                                               \
        if (flags & SHOW_CONFIG) {                                      \
            const char *cfg = libname##_configuration();                \
            if (strcmp(FFMPEG_CONFIGURATION, cfg)) {                    \
                if (!warned_cfg) {                                      \
                    av_log(NULL, level,                                 \
                            "%sWARNING: library configuration mismatch\n", \
                            indent);                                    \
                    warned_cfg = 1;                                     \
                }                                                       \
                av_log(NULL, level, "%s%-11s configuration: %s\n",      \
                        indent, #libname, cfg);                         \
            }                                                           \
        }                                                               \
    }                                                                   \


static void print_all_libs_info(int flags,int level)
{
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = avutil_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"avutil",'4',18,100,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = avutil_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"avutil",cfg);
      }
    }
  }
  ;
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = avcodec_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"avcodec",'6','\\',100,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = avcodec_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"avcodec",cfg);
      }
    }
  }
  ;
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = avformat_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"avformat",'6',63,'h',version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = avformat_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"avformat",cfg);
      }
    }
  }
  ;
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = avdevice_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"avdevice",'6',3,103,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = avdevice_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"avdevice",cfg);
      }
    }
  }
  ;
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = avfilter_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"avfilter",3,42,103,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = avfilter_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"avfilter",cfg);
      }
    }
  }
  ;
//    PRINT_LIB_INFO(avresample, AVRESAMPLE, flags, level);
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = swscale_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"swscale",2,2,100,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = swscale_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"swscale",cfg);
      }
    }
  }
  ;
  if (1) {
    const char *indent = (flags & 1?"  " : "");
    if (flags & 2) {
      unsigned int version = swresample_version();
      av_log(((void *)0),level,"%slib%-11s %2d.%3d.%3d / %2d.%3d.%3d\n",indent,"swresample",0,17,102,version >> 16,version >> 8 & 0xFF,version & 0xFF);
    }
    if (flags & 4) {
      const char *cfg = swresample_configuration();
      if (strcmp("--prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl",cfg)) {
        if (!warned_cfg) {
          av_log(((void *)0),level,"%sWARNING: library configuration mismatch\n",indent);
          warned_cfg = 1;
        }
        av_log(((void *)0),level,"%s%-11s configuration: %s\n",indent,"swresample",cfg);
      }
    }
  }
  ;
#if CONFIG_POSTPROC
#endif
}

static void print_program_info(int flags,int level)
{
  const char *indent = (flags & 1?"  " : "");
  av_log(((void *)0),level,"%s version 1.2.2",program_name);
  if (flags & 8) {
    av_log(((void *)0),level," Copyright (c) %d-%d the FFmpeg developers",program_birth_year,this_year);
  }
  av_log(((void *)0),level,"\n");
  av_log(((void *)0),level,"%sbuilt on %s %s with %s\n",indent,"Nov 21 2014","11:58:29","gcc 4.4.7 (GCC) 20120313 (Red Hat 4.4.7-4)");
  av_log(((void *)0),level,"%sconfiguration: --prefix=/opt/stonesoup/workspace/install --enable-pic --disable-static --enable-shared --disable-yasm --disable-doc --enable-pthreads --disable-w32threads --disable-os2threads --enable-zlib --enable-openssl --disable-asm --extra-cflags= --extra-ldflags= --extra-libs=-ldl\n",indent);
}

void show_banner(int argc,char **argv,const OptionDef *options)
{
  int idx = locate_option(argc,argv,options,"version");
  if (idx) {
    return ;
  }
  print_program_info(1 | 8,32);
  print_all_libs_info(1 | 4,32);
  print_all_libs_info(1 | 2,32);
}

int show_version(void *optctx,const char *opt,const char *arg)
{
  av_log_set_callback(log_callback_help);
  print_program_info(0,32);
  print_all_libs_info(2,32);
  return 0;
}

int show_license(void *optctx,const char *opt,const char *arg)
{
#if CONFIG_NONFREE
#elif CONFIG_GPLV3
#elif CONFIG_GPL
#elif CONFIG_LGPLV3
#else
  printf("%s is free software; you can redistribute it and/or\nmodify it under the terms of the GNU Lesser General Public\nLicense as published by the Free Software Foundation; either\nversion 2.1 of the License, or (at your option) any later version.\n\n%s is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\nLesser General Public License for more details.\n\nYou should have received a copy of the GNU Lesser General Public\nLicense along with %s; if not, write to the Free Software\nFoundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA\n",program_name,program_name,program_name);
#endif
  return 0;
}

int show_formats(void *optctx,const char *opt,const char *arg)
{
  AVInputFormat *ifmt = ((void *)0);
  AVOutputFormat *ofmt = ((void *)0);
  const char *last_name;
  printf("File formats:\n D. = Demuxing supported\n .E = Muxing supported\n --\n");
  last_name = "000";
  for (; ; ) {
    int decode = 0;
    int encode = 0;
    const char *name = ((void *)0);
    const char *long_name = ((void *)0);
    while(ofmt = av_oformat_next(ofmt)){
      if ((name == ((void *)0) || strcmp(ofmt -> name,name) < 0) && strcmp(ofmt -> name,last_name) > 0) {
        name = ofmt -> name;
        long_name = ofmt -> long_name;
        encode = 1;
      }
    }
    while(ifmt = av_iformat_next(ifmt)){
      if ((name == ((void *)0) || strcmp(ifmt -> name,name) < 0) && strcmp(ifmt -> name,last_name) > 0) {
        name = ifmt -> name;
        long_name = ifmt -> long_name;
        encode = 0;
      }
      if (name && strcmp(ifmt -> name,name) == 0) {
        decode = 1;
      }
    }
    if (name == ((void *)0)) {
      break; 
    }
    last_name = name;
    printf(" %s%s %-15s %s\n",(decode?"D" : " "),(encode?"E" : " "),name,(long_name?long_name : " "));
  }
  return 0;
}
#define PRINT_CODEC_SUPPORTED(codec, field, type, list_name, term, get_name) \
    if (codec->field) {                                                      \
        const type *p = codec->field;                                        \
                                                                             \
        printf("    Supported " list_name ":");                              \
        while (*p != term) {                                                 \
            get_name(*p);                                                    \
            printf(" %s", name);                                             \
            p++;                                                             \
        }                                                                    \
        printf("\n");                                                        \
    }                                                                        \


static void print_codec(const AVCodec *c)
{
  int encoder = av_codec_is_encoder(c);
  printf("%s %s [%s]:\n",(encoder?"Encoder" : "Decoder"),c -> name,(c -> long_name?c -> long_name : ""));
  if ((c -> type) == AVMEDIA_TYPE_VIDEO) {
    printf("    Threading capabilities: ");
    switch(c -> capabilities & (0x1000 | 0x2000)){
      case 0x1000 | 0x2000:
{
        printf("frame and slice");
        break; 
      }
      case 0x1000:
{
        printf("frame");
        break; 
      }
      case 0x2000:
{
        printf("slice");
        break; 
      }
      default:
{
        printf("no");
        break; 
      }
    }
    printf("\n");
  }
  if (c -> supported_framerates) {
    const AVRational *fps = c -> supported_framerates;
    printf("    Supported framerates:");
    while(fps -> num){
      printf(" %d/%d",fps -> num,fps -> den);
      fps++;
    }
    printf("\n");
  }
  if (c -> pix_fmts) {
    const enum AVPixelFormat *p = c -> pix_fmts;
    printf("    Supported pixel formats:");
    while(( *p) != AV_PIX_FMT_NONE){
      const char *name = av_get_pix_fmt_name( *p);
      ;
      printf(" %s",name);
      p++;
    }
    printf("\n");
  }
  ;
  if (c -> supported_samplerates) {
    const int *p = c -> supported_samplerates;
    printf("    Supported sample rates:");
    while( *p != 0){
      char name[16];
      snprintf(name,sizeof(name),"%d", *p);
      ;
      printf(" %s",name);
      p++;
    }
    printf("\n");
  }
  ;
  if (c -> sample_fmts) {
    const enum AVSampleFormat *p = c -> sample_fmts;
    printf("    Supported sample formats:");
    while(( *p) != AV_SAMPLE_FMT_NONE){
      const char *name = av_get_sample_fmt_name( *p);
      printf(" %s",name);
      p++;
    }
    printf("\n");
  }
  ;
  if (c -> channel_layouts) {
    const uint64_t *p = c -> channel_layouts;
    printf("    Supported channel layouts:");
    while( *p != 0){
      char name[128];
      av_get_channel_layout_string(name,(sizeof(name)),0, *p);
      ;
      printf(" %s",name);
      p++;
    }
    printf("\n");
  }
  ;
  if (c -> priv_class) {
    show_help_children(c -> priv_class,1 | 2);
  }
}

static char get_media_type_char(enum AVMediaType type)
{
  switch(type){
    case AVMEDIA_TYPE_VIDEO:
    return 'V';
    case AVMEDIA_TYPE_AUDIO:
    return 'A';
    case AVMEDIA_TYPE_DATA:
    return 'D';
    case AVMEDIA_TYPE_SUBTITLE:
    return 'S';
    case AVMEDIA_TYPE_ATTACHMENT:
    return 'T';
    default:
    return 63;
  }
}

static const AVCodec *next_codec_for_id(enum AVCodecID id,const AVCodec *prev,int encoder)
{
  while(prev = (av_codec_next(prev))){
    if ((prev -> id) == id && ((encoder?av_codec_is_encoder(prev) : av_codec_is_decoder(prev)))) {
      return prev;
    }
  }
  return ((void *)0);
}

static int compare_codec_desc(const void *a,const void *b)
{
  const AVCodecDescriptor *const *da = a;
  const AVCodecDescriptor *const *db = b;
  return (( *da) -> type) != (( *db) -> type)?(( *da) -> type) - (( *db) -> type) : strcmp(( *da) -> name,( *db) -> name);
}

static unsigned int get_codecs_sorted(const AVCodecDescriptor ***rcodecs)
{
  const AVCodecDescriptor *desc = ((void *)0);
  const AVCodecDescriptor **codecs;
  unsigned int nb_codecs = 0;
  unsigned int i = 0;
  while(desc = avcodec_descriptor_next(desc))
    nb_codecs++;
  if (!(codecs = (av_calloc(nb_codecs,sizeof(( *codecs)))))) {
    av_log(((void *)0),16,"Out of memory\n");
    exit(1);
  }
  desc = ((void *)0);
  while(desc = avcodec_descriptor_next(desc))
    codecs[i++] = desc;
  do {
    if (!(i == nb_codecs)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","i == nb_codecs","cmdutils.c",1271);
      abort();
    }
  }while (0);
  qsort(codecs,nb_codecs,sizeof(( *codecs)),compare_codec_desc);
   *rcodecs = codecs;
  return nb_codecs;
}

static void print_codecs_for_id(enum AVCodecID id,int encoder)
{
  const AVCodec *codec = ((void *)0);
  printf(" (%s: ",(encoder?"encoders" : "decoders"));
  while(codec = next_codec_for_id(id,codec,encoder))
    printf("%s ",codec -> name);
  printf(")");
}

int show_codecs(void *optctx,const char *opt,const char *arg)
{
  const AVCodecDescriptor **codecs;
  unsigned int i;
  unsigned int nb_codecs = get_codecs_sorted(&codecs);
  printf("Codecs:\n D..... = Decoding supported\n .E.... = Encoding supported\n ..V... = Video codec\n ..A... = Audio codec\n ..S... = Subtitle codec\n ...I.. = Intra frame-only codec\n ....L. = Lossy compression\n .....S = Lossless compression\n -------\n");
  for (i = 0; i < nb_codecs; i++) {
    const AVCodecDescriptor *desc = codecs[i];
    const AVCodec *codec = ((void *)0);
    printf(" ");
    printf((avcodec_find_decoder(desc -> id)?"D" : "."));
    printf((avcodec_find_encoder(desc -> id)?"E" : "."));
    printf("%c",(get_media_type_char(desc -> type)));
    printf((desc -> props & 1 << 0?"I" : "."));
    printf((desc -> props & 1 << 1?"L" : "."));
    printf((desc -> props & 1 << 2?"S" : "."));
    printf(" %-20s %s",desc -> name,(desc -> long_name?desc -> long_name : ""));
/* print decoders/encoders when there's more than one or their
         * names are different from codec name */
    while(codec = next_codec_for_id(desc -> id,codec,0)){
      if (strcmp(codec -> name,desc -> name)) {
        print_codecs_for_id(desc -> id,0);
        break; 
      }
    }
    codec = ((void *)0);
    while(codec = next_codec_for_id(desc -> id,codec,1)){
      if (strcmp(codec -> name,desc -> name)) {
        print_codecs_for_id(desc -> id,1);
        break; 
      }
    }
    printf("\n");
  }
  av_free(codecs);
  return 0;
}

static void print_codecs(int encoder)
{
  const AVCodecDescriptor **codecs;
  unsigned int i;
  unsigned int nb_codecs = get_codecs_sorted(&codecs);
  printf("%s:\n V..... = Video\n A..... = Audio\n S..... = Subtitle\n .F.... = Frame-level multithreading\n ..S... = Slice-level multithreading\n ...X.. = Codec is experimental\n ....B. = Supports draw_horiz_band\n .....D = Supports direct rendering method 1\n ------\n",(encoder?"Encoders" : "Decoders"));
  for (i = 0; i < nb_codecs; i++) {
    const AVCodecDescriptor *desc = codecs[i];
    const AVCodec *codec = ((void *)0);
    while(codec = next_codec_for_id(desc -> id,codec,encoder)){
      printf(" %c",(get_media_type_char(desc -> type)));
      printf((codec -> capabilities & 0x1000?"F" : "."));
      printf((codec -> capabilities & 0x2000?"S" : "."));
      printf((codec -> capabilities & 0x0200?"X" : "."));
      printf((codec -> capabilities & 0x0001?"B" : "."));
      printf((codec -> capabilities & 0x00002?"D" : "."));
      printf(" %-20s %s",codec -> name,(codec -> long_name?codec -> long_name : ""));
      if (strcmp(codec -> name,desc -> name)) {
        printf(" (codec %s)",desc -> name);
      }
      printf("\n");
    }
  }
  av_free(codecs);
}

int show_decoders(void *optctx,const char *opt,const char *arg)
{
  print_codecs(0);
  return 0;
}

int show_encoders(void *optctx,const char *opt,const char *arg)
{
  print_codecs(1);
  return 0;
}

int show_bsfs(void *optctx,const char *opt,const char *arg)
{
  AVBitStreamFilter *bsf = ((void *)0);
  printf("Bitstream filters:\n");
  while(bsf = av_bitstream_filter_next(bsf))
    printf("%s\n",bsf -> name);
  printf("\n");
  return 0;
}

int show_protocols(void *optctx,const char *opt,const char *arg)
{
  void *opaque = (void *)0;
  const char *name;
  printf("Supported file protocols:\nInput:\n");
  while(name = avio_enum_protocols(&opaque,0))
    printf("%s\n",name);
  printf("Output:\n");
  while(name = avio_enum_protocols(&opaque,1))
    printf("%s\n",name);
  return 0;
}

int show_filters(void *optctx,const char *opt,const char *arg)
{
  AVFilter **filter __attribute__((unused))  = ((void *)0);
  char descr[64];
  char *descr_cur;
  int i;
  int j;
  const AVFilterPad *pad;
  printf("Filters:\n");
#if CONFIG_AVFILTER
  while((filter = av_filter_next(filter)) &&  *filter){
    descr_cur = descr;
    for (i = 0; i < 2; i++) {
      if (i) {
         *(descr_cur++) = '-';
         *(descr_cur++) = '>';
      }
      pad = (i?( *filter) -> outputs : ( *filter) -> inputs);
      for (j = 0; pad && pad[j] . name; j++) {
        if (descr_cur >= descr + sizeof(descr) - 4) {
          break; 
        }
         *(descr_cur++) = get_media_type_char(pad[j] . type);
      }
      if (!j) {
         *(descr_cur++) = '|';
      }
    }
     *descr_cur = 0;
    printf("%-16s %-10s %s\n",( *filter) -> name,descr,( *filter) -> description);
  }
#endif
  return 0;
}

int show_pix_fmts(void *optctx,const char *opt,const char *arg)
{
  const AVPixFmtDescriptor *pix_desc = ((void *)0);
  printf("Pixel formats:\nI.... = Supported Input  format for conversion\n.O... = Supported Output format for conversion\n..H.. = Hardware accelerated format\n...P. = Paletted format\n....B = Bitstream format\nFLAGS NAME            NB_COMPONENTS BITS_PER_PIXEL\n-----\n");
#if !CONFIG_SWSCALE
#   define sws_isSupportedInput(x)  0
#   define sws_isSupportedOutput(x) 0
#endif
  while(pix_desc = av_pix_fmt_desc_next(pix_desc)){
    enum AVPixelFormat pix_fmt = av_pix_fmt_desc_get_id(pix_desc);
    printf("%c%c%c%c%c %-16s       %d            %2d\n",(sws_isSupportedInput(pix_fmt)?'I' : '.'),(sws_isSupportedOutput(pix_fmt)?'O' : '.'),((pix_desc -> flags) & 8?'H' : '.'),((pix_desc -> flags) & 2?'P' : '.'),((pix_desc -> flags) & 4?'B' : '.'),pix_desc -> name,(pix_desc -> nb_components),av_get_bits_per_pixel(pix_desc));
  }
  return 0;
}

int show_layouts(void *optctx,const char *opt,const char *arg)
{
  int i = 0;
  uint64_t layout;
  uint64_t j;
  const char *name;
  const char *descr;
  printf("Individual channels:\nNAME        DESCRIPTION\n");
  for (i = 0; i < 63; i++) {
    name = av_get_channel_name(((uint64_t )1) << i);
    if (!name) {
      continue; 
    }
    descr = av_get_channel_description(((uint64_t )1) << i);
    printf("%-12s%s\n",name,descr);
  }
  printf("\nStandard channel layouts:\nNAME        DECOMPOSITION\n");
  for (i = 0; !av_get_standard_channel_layout(i,&layout,&name); i++) {
    if (name) {
      printf("%-12s",name);
      for (j = 1; j; j <<= 1) 
        if (layout & j) {
          printf("%s%s",(layout & j - 1?"+" : ""),av_get_channel_name(j));
        }
      printf("\n");
    }
  }
  return 0;
}

int show_sample_fmts(void *optctx,const char *opt,const char *arg)
{
  int i;
  char fmt_str[128];
  for (i = - 1; i < AV_SAMPLE_FMT_NB; i++) 
    printf("%s\n",av_get_sample_fmt_string(fmt_str,(sizeof(fmt_str)),i));
  return 0;
}

static void show_help_codec(const char *name,int encoder)
{
  const AVCodecDescriptor *desc;
  const AVCodec *codec;
  if (!name) {
    av_log(((void *)0),16,"No codec name specified.\n");
    return ;
  }
  codec = ((encoder?avcodec_find_encoder_by_name(name) : avcodec_find_decoder_by_name(name)));
  if (codec) {
    print_codec(codec);
  }
  else {
    if (desc = avcodec_descriptor_get_by_name(name)) {
      int printed = 0;
      while(codec = next_codec_for_id(desc -> id,codec,encoder)){
        printed = 1;
        print_codec(codec);
      }
      if (!printed) {
        av_log(((void *)0),16,"Codec '%s' is known to FFmpeg, but no %s for it are available. FFmpeg might need to be recompiled with additional external libraries.\n",name,(encoder?"encoders" : "decoders"));
      }
    }
    else {
      av_log(((void *)0),16,"Codec '%s' is not recognized by FFmpeg.\n",name);
    }
  }
}

static void show_help_demuxer(const char *name)
{
  const AVInputFormat *fmt = (av_find_input_format(name));
  if (!fmt) {
    av_log(((void *)0),16,"Unknown format '%s'.\n",name);
    return ;
  }
  printf("Demuxer %s [%s]:\n",fmt -> name,fmt -> long_name);
  if (fmt -> extensions) {
    printf("    Common extensions: %s.\n",fmt -> extensions);
  }
  if (fmt -> priv_class) {
    show_help_children(fmt -> priv_class,2);
  }
}

static void show_help_muxer(const char *name)
{
  const AVCodecDescriptor *desc;
  const AVOutputFormat *fmt = (av_guess_format(name,((void *)0),((void *)0)));
  if (!fmt) {
    av_log(((void *)0),16,"Unknown format '%s'.\n",name);
    return ;
  }
  printf("Muxer %s [%s]:\n",fmt -> name,fmt -> long_name);
  if (fmt -> extensions) {
    printf("    Common extensions: %s.\n",fmt -> extensions);
  }
  if (fmt -> mime_type) {
    printf("    Mime type: %s.\n",fmt -> mime_type);
  }
  if ((fmt -> video_codec) != AV_CODEC_ID_NONE && (desc = avcodec_descriptor_get(fmt -> video_codec))) {
    printf("    Default video codec: %s.\n",desc -> name);
  }
  if ((fmt -> audio_codec) != AV_CODEC_ID_NONE && (desc = avcodec_descriptor_get(fmt -> audio_codec))) {
    printf("    Default audio codec: %s.\n",desc -> name);
  }
  if ((fmt -> subtitle_codec) != AV_CODEC_ID_NONE && (desc = avcodec_descriptor_get(fmt -> subtitle_codec))) {
    printf("    Default subtitle codec: %s.\n",desc -> name);
  }
  if (fmt -> priv_class) {
    show_help_children(fmt -> priv_class,1);
  }
}

int show_help(void *optctx,const char *opt,const char *arg)
{
  char *topic;
  char *par;
  av_log_set_callback(log_callback_help);
  topic = av_strdup((arg?arg : ""));
  par = strchr(topic,'=');
  if (par) {
     *(par++) = 0;
  }
  if (!( *topic)) {
    show_help_default(topic,par);
  }
  else {
    if (!strcmp(topic,"decoder")) {
      show_help_codec(par,0);
    }
    else {
      if (!strcmp(topic,"encoder")) {
        show_help_codec(par,1);
      }
      else {
        if (!strcmp(topic,"demuxer")) {
          show_help_demuxer(par);
        }
        else {
          if (!strcmp(topic,"muxer")) {
            show_help_muxer(par);
          }
          else {
            show_help_default(topic,par);
          }
        }
      }
    }
  }
  av_freep((&topic));
  return 0;
}

int read_yesno()
{
  int c = getchar();
  int yesno = av_toupper(c) == 'Y';
  while(c != '\n' && c != - 1)
    c = getchar();
  return yesno;
}

int cmdutils_read_file(const char *filename,char **bufptr,size_t *size)
{
  int ret;
  FILE *f = fopen(filename,"rb");
  if (!f) {
    av_log(((void *)0),16,"Cannot read file '%s': %s\n",filename,strerror( *__errno_location()));
    return -( *__errno_location());
  }
  fseek(f,0,2);
   *size = (ftell(f));
  fseek(f,0,0);
  if ( *size == ((size_t )(- 1))) {
    av_log(((void *)0),16,"IO error: %s\n",strerror( *__errno_location()));
    fclose(f);
    return -( *__errno_location());
  }
   *bufptr = (av_malloc( *size + 1));
  if (!( *bufptr)) {
    av_log(((void *)0),16,"Could not allocate file buffer\n");
    fclose(f);
    return - 12;
  }
  ret = (fread(( *bufptr),1, *size,f));
  if (ret <  *size) {
    av_free(( *bufptr));
    if (ferror(f)) {
      av_log(((void *)0),16,"Error while reading file '%s': %s\n",filename,strerror( *__errno_location()));
      ret = -( *__errno_location());
    }
    else {
      ret = -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
    }
  }
  else {
    ret = 0;
    ( *bufptr)[( *size)++] = '\0';
  }
  fclose(f);
  return ret;
}

FILE *get_preset_file(char *filename,size_t filename_size,const char *preset_name,int is_path,const char *codec_name)
{
  FILE *f = ((void *)0);
  int i;
  const char *base[3] = {(getenv("FFMPEG_DATADIR")), (getenv("HOME")), "/opt/stonesoup/workspace/install/share/ffmpeg"};
  if (is_path) {
    av_strlcpy(filename,preset_name,filename_size);
    f = fopen(filename,"r");
  }
  else {
#ifdef _WIN32
#endif
    for (i = 0; i < 3 && !f; i++) {
      if (!base[i]) {
        continue; 
      }
      snprintf(filename,filename_size,"%s%s/%s.ffpreset",base[i],(i != 1?"" : "/.ffmpeg"),preset_name);
      f = fopen(filename,"r");
      if (!f && codec_name) {
        snprintf(filename,filename_size,"%s%s/%s-%s.ffpreset",base[i],(i != 1?"" : "/.ffmpeg"),codec_name,preset_name);
        f = fopen(filename,"r");
      }
    }
  }
  return f;
}

int check_stream_specifier(AVFormatContext *s,AVStream *st,const char *spec)
{
  int ret = avformat_match_stream_specifier(s,st,spec);
  if (ret < 0) {
    av_log(s,16,"Invalid stream specifier: %s.\n",spec);
  }
  return ret;
}

AVDictionary *filter_codec_opts(AVDictionary *opts,enum AVCodecID codec_id,AVFormatContext *s,AVStream *st,AVCodec *codec)
{
  AVDictionary *ret = ((void *)0);
  AVDictionaryEntry *t = ((void *)0);
  int flags = s -> oformat?1 : 2;
  char prefix = 0;
  const AVClass *cc = avcodec_get_class();
  if (!codec) {
    codec = (s -> oformat?avcodec_find_encoder(codec_id) : avcodec_find_decoder(codec_id));
  }
  switch(st -> codec -> codec_type){
    case AVMEDIA_TYPE_VIDEO:
{
      prefix = 'v';
      flags |= 16;
      break; 
    }
    case AVMEDIA_TYPE_AUDIO:
{
      prefix = 'a';
      flags |= 8;
      break; 
    }
    case AVMEDIA_TYPE_SUBTITLE:
{
      prefix = 's';
      flags |= 32;
      break; 
    }
  }
  while(t = av_dict_get(opts,"",t,2)){
    char *p = strchr((t -> key),':');
/* check stream specification in opt name */
    if (p) {
      switch(check_stream_specifier(s,st,(p + 1))){
        case 1:
{
           *p = 0;
          break; 
        }
        case 0:
        continue; 
        default:
        return ((void *)0);
      }
    }
    if (av_opt_find((&cc),(t -> key),((void *)0),flags,0x00002) || codec && codec -> priv_class && av_opt_find((&codec -> priv_class),(t -> key),((void *)0),flags,0x00002)) {
      av_dict_set(&ret,(t -> key),(t -> value),0);
    }
    else {
      if (t -> key[0] == prefix && av_opt_find((&cc),(t -> key + 1),((void *)0),flags,0x00002)) {
        av_dict_set(&ret,(t -> key + 1),(t -> value),0);
      }
    }
    if (p) {
       *p = ':';
    }
  }
  return ret;
}

AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,AVDictionary *codec_opts)
{
  int i;
  AVDictionary **opts;
  if (__sync_bool_compare_and_swap(&scutches_ashburnham,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpaJlHVY_ss_testcase/src-rose/cmdutils.c","setup_find_stream_info_opts");
      pfeffernuss_logotype(xerophilous_throop);
    }
  }
  if (!s -> nb_streams) {
    return ((void *)0);
  }
  opts = (av_mallocz((s -> nb_streams) * sizeof(( *opts))));
  if (!opts) {
    av_log(((void *)0),16,"Could not alloc memory for stream options.\n");
    return ((void *)0);
  }
  for (i = 0; i < s -> nb_streams; i++) 
    opts[i] = filter_codec_opts(codec_opts,s -> streams[i] -> codec -> codec_id,s,s -> streams[i],((void *)0));
  return opts;
}

void *grow_array(void *array,int elem_size,int *size,int new_size)
{
  if (new_size >= 2147483647 / elem_size) {
    av_log(((void *)0),16,"Array too big.\n");
    exit(1);
  }
  if ( *size < new_size) {
    uint8_t *tmp = (av_realloc(array,(new_size * elem_size)));
    if (!tmp) {
      av_log(((void *)0),16,"Could not alloc buffer.\n");
      exit(1);
    }
    memset((tmp +  *size * elem_size),0,((new_size -  *size) * elem_size));
     *size = new_size;
    return tmp;
  }
  return array;
}

static int alloc_buffer(FrameBuffer **pool,AVCodecContext *s,FrameBuffer **pbuf)
{
  const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(s -> pix_fmt);
  FrameBuffer *buf;
  int i;
  int ret;
  int pixel_size;
  int h_chroma_shift;
  int v_chroma_shift;
// XXX should be avcodec_get_edge_width(), but that fails on svq1
  int edge = 32;
  int w = s -> width;
  int h = s -> height;
  if (!desc) {
    return - 22;
  }
  pixel_size = desc -> comp[0] . step_minus1 + 1;
  buf = (av_mallocz(sizeof(( *buf))));
  if (!buf) {
    return - 12;
  }
  avcodec_align_dimensions(s,&w,&h);
  if (!(s -> flags & 0x4000)) {
    w += 2 * edge;
    h += 2 * edge;
  }
  if ((ret = av_image_alloc(buf -> base,buf -> linesize,w,h,s -> pix_fmt,32)) < 0) {
    av_freep((&buf));
    av_log(s,16,"alloc_buffer: av_image_alloc() failed\n");
    return ret;
  }
  avcodec_get_chroma_sub_sample(s -> pix_fmt,&h_chroma_shift,&v_chroma_shift);
  for (i = 0; i < sizeof(buf -> data) / sizeof(buf -> data[0]); i++) {
    const int h_shift = i == 0?0 : h_chroma_shift;
    const int v_shift = i == 0?0 : v_chroma_shift;
    if (s -> flags & 0x4000 || !buf -> linesize[i] || !buf -> base[i]) {
      buf -> data[i] = buf -> base[i];
    }
    else {
      buf -> data[i] = buf -> base[i] + ((buf -> linesize[i] * edge >> v_shift) + (pixel_size * edge >> h_shift) + 32 - 1 & ~(32 - 1));
    }
  }
  buf -> w = s -> width;
  buf -> h = s -> height;
  buf -> pix_fmt = s -> pix_fmt;
  buf -> pool = pool;
   *pbuf = buf;
  return 0;
}

int codec_get_buffer(AVCodecContext *s,AVFrame *frame)
{
  FrameBuffer **pool = (s -> opaque);
  FrameBuffer *buf;
  int ret;
  int i;
  if (av_image_check_size((s -> width),(s -> height),0,s) || (s -> pix_fmt) < 0) {
    av_log(s,16,"codec_get_buffer: image parameters invalid\n");
    return - 1;
  }
  if (!( *pool) && (ret = alloc_buffer(pool,s,pool)) < 0) {
    return ret;
  }
  buf =  *pool;
   *pool = buf -> next;
  buf -> next = ((void *)0);
  if (buf -> w != s -> width || buf -> h != s -> height || (buf -> pix_fmt) != (s -> pix_fmt)) {
    av_freep((&buf -> base[0]));
    av_free(buf);
    if ((ret = alloc_buffer(pool,s,&buf)) < 0) {
      return ret;
    }
  }
  do {
    if (!(!buf -> refcount)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","!buf->refcount","cmdutils.c",1915);
      abort();
    }
  }while (0);
  buf -> refcount++;
  frame -> opaque = buf;
  frame -> type = 2;
  frame -> extended_data = frame -> data;
  for (i = 0; i < sizeof(buf -> data) / sizeof(buf -> data[0]); i++) {
// XXX h264.c uses base though it shouldn't
    frame -> base[i] = buf -> base[i];
    frame -> data[i] = buf -> data[i];
    frame -> linesize[i] = buf -> linesize[i];
  }
  return 0;
}

static void unref_buffer(FrameBuffer *buf)
{
  FrameBuffer **pool = buf -> pool;
  do {
    if (!(buf -> refcount > 0)) {
      av_log(((void *)0),0,"Assertion %s failed at %s:%d\n","buf->refcount > 0","cmdutils.c",1935);
      abort();
    }
  }while (0);
  buf -> refcount--;
  if (!buf -> refcount) {
    FrameBuffer *tmp;
    for (tmp =  *pool; tmp; tmp = tmp -> next) 
      (void )0;
    buf -> next =  *pool;
     *pool = buf;
  }
}

void codec_release_buffer(AVCodecContext *s,AVFrame *frame)
{
  FrameBuffer *buf = (frame -> opaque);
  int i;
  if (frame -> type != 2) {
    avcodec_default_release_buffer(s,frame);
    return ;
  }
  for (i = 0; i < sizeof(frame -> data) / sizeof(frame -> data[0]); i++) 
    frame -> data[i] = ((void *)0);
  unref_buffer(buf);
}

void filter_release_buffer(AVFilterBuffer *fb)
{
  FrameBuffer *buf = (fb -> priv);
  av_free(fb);
  unref_buffer(buf);
}

void free_buffer_pool(FrameBuffer **pool)
{
  FrameBuffer *buf =  *pool;
  while(buf){
     *pool = buf -> next;
    av_freep((&buf -> base[0]));
    av_free(buf);
    buf =  *pool;
  }
}

void xerophilous_throop(char **lactivorous_calotermitidae)
{
  char *cartoned_tenebricose;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  cartoned_tenebricose = getenv("HABIT_CAMERINE");
  if (cartoned_tenebricose != 0) {;
     *lactivorous_calotermitidae = cartoned_tenebricose;
  }
}

void pfeffernuss_logotype(void (*palmettos_labouredly)(char **))
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[10];
 char stonesoup_filename[80];
  char *dentins_proteinphobia = 0;
  char **tagish_coresidence = 0;
  char **parapodial_phlebological = 0;
  ++stonesoup_global_variable;
  char *doneck_preappearances = 0;
  palmettos_labouredly(&doneck_preappearances);
  if (doneck_preappearances != 0) {;
    tagish_coresidence = &doneck_preappearances;
    parapodial_phlebological = tagish_coresidence + 5;
    dentins_proteinphobia = ((char *)( *(parapodial_phlebological - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE773", "A", "Missing Reference to Active File Descriptor or Handle");
 stonesoup_files = fopen(dentins_proteinphobia,"r");
    if (stonesoup_files != 0) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        while(fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1 && stonesoup_ssi < 1020){
            stonesoup_file = fopen(stonesoup_filename,"w");
            if (stonesoup_file == 0) {
    if (stonesoup_file == 0 && errno == 24) {
     stonesoup_printf("Fopen error due to ulimit\n");
                }
    continue;
   }
   fputs("woohoo!",stonesoup_file);
   fflush(stonesoup_file);
         /* STONESOUP: CROSSOVER-POINT (Missing Reference To Active File Handle */
   /* STONESOUP: TRIGGER-POINT (Missing Reference To Active File Handle */
   stonesoup_file_list[stonesoup_ssi % 10] = stonesoup_file;
   stonesoup_ssi++;
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    }
    if (stonesoup_files != 0)
        fclose(stonesoup_files);
 stonesoup_cleanup(stonesoup_file_list, ((stonesoup_ssi-1)%10)+1);
    tracepoint(stonesoup_trace, weakness_end);
;
stonesoup_close_printf_context();
  }
}
