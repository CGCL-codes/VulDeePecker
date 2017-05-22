/* $Copyright: $
 * Copyright (c) 1996 - 2014 by Steve Baker (ice@mama.indstate.edu)
 * All Rights reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "tree.h"
/*
 * Hacked in DIR_COLORS support for linux. ------------------------------
 */
/*
 *  If someone asked me, I'd extend dircolors, to provide more generic
 * color support so that more programs could take advantage of it.  This
 * is really just hacked in support.  The dircolors program should:
 * 1) Put the valid terms in a environment var, like:
 *    COLOR_TERMS=linux:console:xterm:vt100...
 * 2) Put the COLOR and OPTIONS directives in a env var too.
 * 3) Have an option to dircolors to silently ignore directives that it
 *    doesn't understand (directives that other programs would
 *    understand).
 * 4) Perhaps even make those unknown directives environment variables.
 *
 * The environment is the place for cryptic crap no one looks at, but
 * programs.  No one is going to care if it takes 30 variables to do
 * something.
 */
#include <stdarg.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
enum __anonymous_0x21cb810 {CMD_COLOR=0,CMD_OPTIONS=1,CMD_TERM=2,CMD_EIGHTBIT=3,COL_NORMAL=4,COL_FILE=5,COL_DIR=6,COL_LINK=7,COL_FIFO=8,COL_DOOR=9,COL_BLK=10,COL_CHR=11,COL_ORPHAN=12,COL_SOCK=13,COL_SETUID=14,COL_SETGID=15,COL_STICKY_OTHER_WRITABLE=16,COL_OTHER_WRITABLE=17,COL_STICKY=18,COL_EXEC=19,COL_MISSING=20,COL_LEFTCODE=21,COL_RIGHTCODE=22,COL_ENDCODE=23,DOT_EXTENSION=24,ERROR=25} ;
bool colorize = FALSE;
bool ansilines = FALSE;
bool linktargetcolor = FALSE;
char *term;
char termmatch = FALSE;
char istty;
char *leftcode = ((void *)0);
char *rightcode = ((void *)0);
char *endcode = ((void *)0);
char *norm_flgs = ((void *)0);
char *file_flgs = ((void *)0);
char *dir_flgs = ((void *)0);
char *link_flgs = ((void *)0);
char *fifo_flgs = ((void *)0);
char *door_flgs = ((void *)0);
char *block_flgs = ((void *)0);
char *char_flgs = ((void *)0);
char *orphan_flgs = ((void *)0);
char *sock_flgs = ((void *)0);
char *suid_flgs = ((void *)0);
char *sgid_flgs = ((void *)0);
char *stickyow_flgs = ((void *)0);
char *otherwr_flgs = ((void *)0);
char *sticky_flgs = ((void *)0);
char *exec_flgs = ((void *)0);
char *missing_flgs = ((void *)0);
char *vgacolor[] = {("black"), ("red"), ("green"), ("yellow"), ("blue"), ("fuchsia"), ("aqua"), ("white"), (((void *)0)), (((void *)0)), ("transparent"), ("red"), ("green"), ("yellow"), ("blue"), ("fuchsia"), ("aqua"), ("black")};
struct colortable colortable[11];
struct extensions *ext = ((void *)0);
const struct linedraw *linedraw;
char **split(char *str,char *delim,int *nwrds);
int cmd(char *s);
extern FILE *outfile;
extern bool Hflag;
extern bool force_color;
extern bool nocolor;
extern const char *charset;
int talocalcanean_romains = 0;
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
void stonesoup_cleanup(char **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmp2AVge9_ss_testcase/src-rose/color.c", "stonesoup_cleanup");
  for (; i < size; ++i) {
    if (ptrs[i] != 0) {
      free(ptrs[i]);
    }
  }
}
int stonesoup_isalnum(int c)
{
  if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57)) {
    return 1;
  }
  return 0;
}
char *stonesoup_isAlphaNum(char *str,int size_param)
{
  int index = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmp2AVge9_ss_testcase/src-rose/color.c", "stonesoup_isAlphaNum");
  for (index = 0; index < size_param; index++) {
    if (!stonesoup_isalnum(str[index])) {
      tracepoint(stonesoup_trace, trace_point, "Returning 0");
/* STONESOUP: TRIGGER-POINT (Missing Reference to Active Allocated Resource) */
      return 0;
    }
  }
  return str;
}

void parse_dir_colors()
{
  char buf[1025];
  char **arg;
  char **c;
  char *colors;
  char *s;
  int i;
  int n;
  struct extensions *e;
  if (Hflag) {
    return ;
  }
  if (getenv("TERM") == ((void *)0)) {
    colorize = FALSE;
    return ;
  }
  s = getenv("TREE_COLORS");
  if (s == ((void *)0)) {
    s = getenv("LS_COLORS");
  }
  if ((s == ((void *)0) || strlen(s) == 0) && force_color) {
    s = ":no=00:fi=00:di=01;34:ln=01;36:pi=40;33:so=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:ex=01;32:*.bat=01;32:*.BAT=01;32:*.btm=01;32:*.BTM=01;32:*.cmd=01;32:*.CMD=01;32:*.com=01;32:*.COM=01;32:*.dll=01;32:*.DLL=01;32:*.exe=01;32:*.EXE=01;32:*.arj=01;31:*.bz2=01;31:*.deb=01;31:*.gz=01;31:*.lzh=01;31:*.rpm=01;31:*.tar=01;31:*.taz=01;31:*.tb2=01;31:*.tbz2=01;31:*.tbz=01;31:*.tgz=01;31:*.tz2=01;31:*.z=01;31:*.Z=01;31:*.zip=01;31:*.ZIP=01;31:*.zoo=01;31:*.asf=01;35:*.ASF=01;35:*.avi=01;35:*.AVI=01;35:*.bmp=01;35:*.BMP=01;35:*.flac=01;35:*.FLAC=01;35:*.gif=01;35:*.GIF=01;35:*.jpg=01;35:*.JPG=01;35:*.jpeg=01;35:*.JPEG=01;35:*.m2a=01;35:*.M2a=01;35:*.m2v=01;35:*.M2V=01;35:*.mov=01;35:*.MOV=01;35:*.mp3=01;35:*.MP3=01;35:*.mpeg=01;35:*.MPEG=01;35:*.mpg=01;35:*.MPG=01;35:*.ogg=01;35:*.OGG=01;35:*.ppm=01;35:*.rm=01;35:*.RM=01;35:*.tga=01;35:*.TGA=01;35:*.tif=01;35:*.TIF=01;35:*.wav=01;35:*.WAV=01;35:*.wmv=01;35:*.WMV=01;35:*.xbm=01;35:*.xpm=01;35:";
  }
  if (s == ((void *)0) || !force_color && (nocolor || !isatty(1))) {
    colorize = FALSE;
    return ;
  }
  else {
    colorize = TRUE;
/* You can uncomment the below line and tree will always try to ANSI-fy the indentation lines */
/*    ansilines = TRUE; */
  }
  colors = strcpy((xmalloc(strlen(s) + 1)),s);
  arg = split(colors,":",&n);
  for (i = 0; arg[i]; i++) {
    c = split(arg[i],"=",&n);
    switch(cmd(c[0])){
      case COL_NORMAL:
{
        if (c[1]) {
          norm_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_FILE:
{
        if (c[1]) {
          file_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_DIR:
{
        if (c[1]) {
          dir_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_LINK:
{
        if (c[1]) {
          if (strcasecmp("target",c[1]) == 0) {
            linktargetcolor = TRUE;
/* Should never actually be used */
            link_flgs = "01;36";
          }
          else {
            link_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
          }
        }
        break; 
      }
      case COL_FIFO:
{
        if (c[1]) {
          fifo_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_DOOR:
{
        if (c[1]) {
          door_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_BLK:
{
        if (c[1]) {
          block_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_CHR:
{
        if (c[1]) {
          char_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_ORPHAN:
{
        if (c[1]) {
          orphan_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_SOCK:
{
        if (c[1]) {
          sock_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_SETUID:
{
        if (c[1]) {
          suid_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_SETGID:
{
        if (c[1]) {
          sgid_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_STICKY_OTHER_WRITABLE:
{
        if (c[1]) {
          stickyow_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_OTHER_WRITABLE:
{
        if (c[1]) {
          otherwr_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_STICKY:
{
        if (c[1]) {
          sticky_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_EXEC:
{
        if (c[1]) {
          exec_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_MISSING:
{
        if (c[1]) {
          missing_flgs = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_LEFTCODE:
{
        if (c[1]) {
          leftcode = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_RIGHTCODE:
{
        if (c[1]) {
          rightcode = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case COL_ENDCODE:
{
        if (c[1]) {
          endcode = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        }
        break; 
      }
      case DOT_EXTENSION:
      if (c[1]) {
        e = (xmalloc(sizeof(struct extensions )));
        e -> ext = strcpy((xmalloc(strlen((c[0] + 1)) + 1)),(c[0] + 1));
        e -> term_flg = strcpy((xmalloc(strlen(c[1]) + 1)),c[1]);
        e -> nxt = ext;
        ext = e;
      }
    }
    free(c);
  }
  free(arg);
/* make sure at least norm_flgs is defined.  We're going to assume vt100 support */
  if (!leftcode) {
    leftcode = strcpy((xmalloc(strlen("\033[") + 1)),"\033[");
  }
  if (!rightcode) {
    rightcode = strcpy((xmalloc(strlen("m") + 1)),"m");
  }
  if (!norm_flgs) {
    norm_flgs = strcpy((xmalloc(strlen("00") + 1)),"00");
  }
  if (!endcode) {
    sprintf(buf,"%s%s%s",leftcode,norm_flgs,rightcode);
    endcode = strcpy((xmalloc(strlen(buf) + 1)),buf);
  }
  free(colors);
/*  if (!termmatch) colorize = FALSE; */
}
/*
 * You must free the pointer that is allocated by split() after you
 * are done using the array.
 */

char **split(char *str,char *delim,int *nwrds)
{
  int n = 128;
  char **w = (xmalloc(sizeof(char *) * n));
  w[ *nwrds = 0] = strtok(str,delim);
  while(w[ *nwrds]){
    if ( *nwrds == n - 2) {
      w = (xrealloc(w,sizeof(char *) * (n += 256)));
    }
    w[++ *nwrds] = strtok(((void *)0),delim);
  }
  w[ *nwrds] = ((void *)0);
  return w;
}

int cmd(char *s)
{
  static struct {
  char *cmd;
  char cmdnum;}cmds[] = {{("no"), (COL_NORMAL)}, {("fi"), (COL_FILE)}, {("di"), (COL_DIR)}, {("ln"), (COL_LINK)}, {("pi"), (COL_FIFO)}, {("do"), (COL_DOOR)}, {("bd"), (COL_BLK)}, {("cd"), (COL_CHR)}, {("or"), (COL_ORPHAN)}, {("so"), (COL_SOCK)}, {("su"), (COL_SETUID)}, {("sg"), (COL_SETGID)}, {("tw"), (COL_STICKY_OTHER_WRITABLE)}, {("ow"), (COL_OTHER_WRITABLE)}, {("st"), (COL_STICKY)}, {("ex"), (COL_EXEC)}, {("mi"), (COL_MISSING)}, {("lc"), (COL_LEFTCODE)}, {("rc"), (COL_RIGHTCODE)}, {("ec"), (COL_ENDCODE)}, {(((void *)0)), (0)}};
  int i;
  for (i = 0; cmds[i] . cmdnum; i++) 
    if (!strcmp(cmds[i] . cmd,s)) {
      return cmds[i] . cmdnum;
    }
  if (s[0] == '*') {
    return DOT_EXTENSION;
  }
  return ERROR;
}

int color(u_short mode,char *name,bool orphan,bool islink)
{
  struct extensions *e;
  int l;
  int xl;
  if (orphan) {
    if (islink) {
      if (missing_flgs) {
        fprintf(outfile,"%s%s%s",leftcode,missing_flgs,rightcode);
        return TRUE;
      }
    }
    else {
      if (orphan_flgs) {
        fprintf(outfile,"%s%s%s",leftcode,orphan_flgs,rightcode);
        return TRUE;
      }
    }
  }
  switch(mode & 0170000){
    case 0010000:
{
      if (!fifo_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,fifo_flgs,rightcode);
      return TRUE;
    }
    case 0020000:
{
      if (!char_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,char_flgs,rightcode);
      return TRUE;
    }
    case 0040000:
{
      if (mode & 01000) {
        if (mode & 0200 >> 3 >> 3 && stickyow_flgs) {
          fprintf(outfile,"%s%s%s",leftcode,stickyow_flgs,rightcode);
          return TRUE;
        }
        if (!(mode & 0200 >> 3 >> 3) && sticky_flgs) {
          fprintf(outfile,"%s%s%s",leftcode,sticky_flgs,rightcode);
          return TRUE;
        }
      }
      if (mode & 0200 >> 3 >> 3 && otherwr_flgs) {
        fprintf(outfile,"%s%s%s",leftcode,otherwr_flgs,rightcode);
        return TRUE;
      }
      if (!dir_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,dir_flgs,rightcode);
      return TRUE;
    }
    case 0060000:
{
#ifndef __EMX__
      if (!block_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,block_flgs,rightcode);
      return TRUE;
    }
    case 0120000:
{
      if (!link_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,link_flgs,rightcode);
      return TRUE;
    }
    case 0140000:
{
  #ifdef S_IFDOOR
  #endif
#endif
      if (!sock_flgs) {
        return FALSE;
      }
      fprintf(outfile,"%s%s%s",leftcode,sock_flgs,rightcode);
      return TRUE;
    }
    case 0100000:
{
      if (mode & 04000 && suid_flgs) {
        fprintf(outfile,"%s%s%s",leftcode,suid_flgs,rightcode);
        return TRUE;
      }
      if (mode & 02000 && sgid_flgs) {
        fprintf(outfile,"%s%s%s",leftcode,sgid_flgs,rightcode);
        return TRUE;
      }
      if (!exec_flgs) {
        return FALSE;
      }
      if (mode & (0100 | 0100 >> 3 | 0100 >> 3 >> 3)) {
        fprintf(outfile,"%s%s%s",leftcode,exec_flgs,rightcode);
        return TRUE;
      }
/* not a directory, link, special device, etc, so check for extension match */
      l = (strlen(name));
      for (e = ext; e; e = e -> nxt) {
        xl = (strlen((e -> ext)));
        if (!strcmp(((l > xl?name + (l - xl) : name)),(e -> ext))) {
          fprintf(outfile,"%s%s%s",leftcode,e -> term_flg,rightcode);
          return TRUE;
        }
      }
      return FALSE;
    }
  }
  return FALSE;
}
/*
 * Charsets provided by Kyosuke Tokoro (NBG01720@nifty.ne.jp)
 */

const char *getcharset()
{
  #ifndef __EMX__
  return (getenv("TREE_CHARSET"));
  #else
      #endif
}

void initlinedraw(int flag)
{
    char *stonesoup_contents;
    char stonesoup_filename[80];
    FILE *stonesoup_file;
    FILE **stonesoup_file_list;
    FILE *stonesoup_files;
    int stonesoup_str_list_index;
    char **stonesoup_str_list;
    int stonesoup_num_files = 10;
    int stonesoup_size;
    int stonesoup_ssi = 0;
  char *holder_saprolegniales = 0;
  int favellidium_apodeictical = 45;
  char *zeuxian_skinful;
  static const char *latin1_3[] = {("ISO-8859-1"), ("ISO-8859-1:1987"), ("ISO_8859-1"), ("latin1"), ("l1"), ("IBM819"), ("CP819"), ("csISOLatin1"), ("ISO-8859-3"), ("ISO_8859-3:1988"), ("ISO_8859-3"), ("latin3"), ("ls"), ("csISOLatin3"), (((void *)0))};
  static const char *iso8859_789[] = {("ISO-8859-7"), ("ISO_8859-7:1987"), ("ISO_8859-7"), ("ELOT_928"), ("ECMA-118"), ("greek"), ("greek8"), ("csISOLatinGreek"), ("ISO-8859-8"), ("ISO_8859-8:1988"), ("iso-ir-138"), ("ISO_8859-8"), ("hebrew"), ("csISOLatinHebrew"), ("ISO-8859-9"), ("ISO_8859-9:1989"), ("iso-ir-148"), ("ISO_8859-9"), ("latin5"), ("l5"), ("csISOLatin5"), (((void *)0))};
  static const char *shift_jis[] = {("Shift_JIS"), ("MS_Kanji"), ("csShiftJIS"), (((void *)0))};
  static const char *euc_jp[] = {("EUC-JP"), ("Extended_UNIX_Code_Packed_Format_for_Japanese"), ("csEUCPkdFmtJapanese"), (((void *)0))};
  static const char *euc_kr[] = {("EUC-KR"), ("csEUCKR"), (((void *)0))};
  static const char *iso2022jp[] = {("ISO-2022-JP"), ("csISO2022JP"), ("ISO-2022-JP-2"), ("csISO2022JP2"), (((void *)0))};
  static const char *ibm_pc[] = {("IBM437"), ("cp437"), ("437"), ("csPC8CodePage437"), ("IBM852"), ("cp852"), ("852"), ("csPCp852"), ("IBM863"), ("cp863"), ("863"), ("csIBM863"), ("IBM855"), ("cp855"), ("855"), ("csIBM855"), ("IBM865"), ("cp865"), ("865"), ("csIBM865"), ("IBM866"), ("cp866"), ("866"), ("csIBM866"), (((void *)0))};
  static const char *ibm_ps2[] = {("IBM850"), ("cp850"), ("850"), ("csPC850Multilingual"), ("IBM00858"), ("CCSID00858"), ("CP00858"), ("PC-Multilingual-850+euro"), (((void *)0))};
  static const char *ibm_gr[] = {("IBM869"), ("cp869"), ("869"), ("cp-gr"), ("csIBM869"), (((void *)0))};
  static const char *gb[] = {("GB2312"), ("csGB2312"), (((void *)0))};
  static const char *utf8[] = {("UTF-8"), ("utf8"), (((void *)0))};
  static const char *big5[] = {("Big5"), ("csBig5"), (((void *)0))};
  static const char *viscii[] = {("VISCII"), ("csVISCII"), (((void *)0))};
  static const char *koi8ru[] = {("KOI8-R"), ("csKOI8R"), ("KOI8-U"), (((void *)0))};
  static const char *windows[] = {("ISO-8859-1-Windows-3.1-Latin-1"), ("csWindows31Latin1"), ("ISO-8859-2-Windows-Latin-2"), ("csWindows31Latin2"), ("windows-1250"), ("windows-1251"), ("windows-1253"), ("windows-1254"), ("windows-1255"), ("windows-1256"), ("windows-1256"), ("windows-1257"), (((void *)0))};
  static const struct linedraw cstable[] = {{(latin1_3), ("|  "), ("|--"), ("&middot;--"), ("&copy;")}, {(iso8859_789), ("|  "), ("|--"), ("&middot;--"), ("(c)")}, {(shift_jis), ("\204\240 "), ("\204\245"), ("\204\244"), ("(c)")}, {(euc_jp), ("\250\242 "), ("\250\247"), ("\250\246"), ("(c)")}, {(euc_kr), ("\246\242 "), ("\246\247"), ("\246\246"), ("(c)")}, {(iso2022jp), ("\033$B(\"\033(B "), ("\033$B('\033(B"), ("\033$B(&\033(B"), ("(c)")}, {(ibm_pc), ("\263  "), ("\303\304\304"), ("\300\304\304"), ("(c)")}, {(ibm_ps2), ("\263  "), ("\303\304\304"), ("\300\304\304"), ("\227")}, {(ibm_gr), ("\263  "), ("\303\304\304"), ("\300\304\304"), ("\270")}, {(gb), ("\251\246 "), ("\251\300"), ("\251\270"), ("(c)")}, {(utf8), ("\342\224\202\302\240\302\240"), ("\342\224\234\342\224\200\342\224\200"), ("\342\224\224\342\224\200\342\224\200"), ("\302\251")}, {(big5), ("\242x "), ("\242u"), ("\242|"), ("(c)")}, {(viscii), ("|  "), ("|--"), ("`--"), ("\371")}, {(koi8ru), ("\201  "), ("\206\200\200"), ("\204\200\200"), ("\277")}, {(windows), ("|  "), ("|--"), ("`--"), ("\251")}, {(((void *)0)), ("|  "), ("|--"), ("`--"), ("(c)")}};
  const char **s;
  if (__sync_bool_compare_and_swap(&talocalcanean_romains,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp2AVge9_ss_testcase/src-rose/color.c","initlinedraw");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&zeuxian_skinful,"5074",favellidium_apodeictical);
      if (zeuxian_skinful != 0) {;
        holder_saprolegniales = ((char *)zeuxian_skinful);
    tracepoint(stonesoup_trace, weakness_start, "CWE771", "A", "Missing Reference to Active Allocated Resource");
    stonesoup_str_list = malloc(sizeof(char *) * stonesoup_num_files);
    if (stonesoup_str_list != 0) {
        for (stonesoup_str_list_index = 0; stonesoup_str_list_index < stonesoup_num_files; ++stonesoup_str_list_index)
            stonesoup_str_list[stonesoup_str_list_index] = 0;
        stonesoup_files = fopen(holder_saprolegniales,"rb");
        if (stonesoup_files != 0) {
            stonesoup_file_list = malloc(stonesoup_num_files * sizeof(FILE *));
            if (stonesoup_file_list == 0) {
                stonesoup_printf("Error: Failed to allocate memory\n");
                exit(1);
            }
            for (stonesoup_ssi = 0; stonesoup_ssi < stonesoup_num_files; ++stonesoup_ssi) {
                if (fscanf(stonesoup_files,"%79s",stonesoup_filename) == 1) {
                    stonesoup_file_list[stonesoup_ssi] = fopen(stonesoup_filename,"rb");
                }
            }
            stonesoup_ssi = 0;
            while(stonesoup_ssi < stonesoup_num_files){
                stonesoup_file = stonesoup_file_list[stonesoup_ssi];
                if (stonesoup_file == 0) {
                    ++stonesoup_ssi;
                    continue;
                }
                fseek(stonesoup_file,0,2);
                stonesoup_size = ftell(stonesoup_file);
                rewind(stonesoup_file);
                stonesoup_contents = malloc((stonesoup_size + 1) * sizeof(char ));
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
                /* STONESOUP: CROSSOVER-POINT (Missing Reference to Active Allocated Resource */
                if (stonesoup_contents == 0 && errno == 12) {
                    tracepoint(stonesoup_trace, trace_error, "Malloc error due to ulimit.");
                    stonesoup_printf("Malloc error due to ulimit\n");
                }
                if (stonesoup_contents == 0) {
                    fclose(stonesoup_file);
                    break;
                }
                tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
                memset(stonesoup_contents,0,(stonesoup_size + 1) * sizeof(char ));
                fread(stonesoup_contents,1,stonesoup_size,stonesoup_file);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
                stonesoup_contents = stonesoup_isAlphaNum(stonesoup_contents,stonesoup_size);
                tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
                stonesoup_str_list[stonesoup_ssi] = stonesoup_contents;
                fclose(stonesoup_file);
                stonesoup_ssi++;
            }
            fclose(stonesoup_files);
            if (stonesoup_file_list != 0) {
                free(stonesoup_file_list);
            }
        }
        stonesoup_cleanup(stonesoup_str_list,stonesoup_num_files);
        free(stonesoup_str_list);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (zeuxian_skinful != 0) 
          free(((char *)zeuxian_skinful));
stonesoup_close_printf_context();
      }
    }
  }
  if (flag) {
    fprintf(stderr,"tree: missing argument to --charset, valid charsets include:\n");
    for (linedraw = cstable; linedraw -> name; ++linedraw) 
      for (s = linedraw -> name;  *s; ++s) 
        fprintf(stderr,"  %s\n", *s);
    return ;
  }
  if (charset) {
    for (linedraw = cstable; linedraw -> name; ++linedraw) 
      for (s = linedraw -> name;  *s; ++s) 
        if (!strcasecmp(charset, *s)) {
          return ;
        }
  }
  linedraw = cstable + sizeof(cstable) / sizeof(( *cstable)) - 1;
}
