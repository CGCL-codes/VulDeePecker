/*
 * buffered I/O
 * Copyright (c) 2000,2001 Fabrice Bellard
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
#include "libavutil/crc.h"
#include "libavutil/dict.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/log.h"
#include "libavutil/opt.h"
#include "libavutil/avassert.h"
#include "avformat.h"
#include "avio.h"
#include "avio_internal.h"
#include "internal.h"
#include "url.h"
#include <stdarg.h>
#define IO_BUFFER_SIZE 32768
/**
 * Do seeks within this distance ahead of the current buffer by skipping
 * data instead of calling the protocol seek function, for seekable
 * protocols.
 */
#define SHORT_SEEK_THRESHOLD 4096
#include <sys/stat.h> 
#include <stdio.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
int coeymans_tidehead = 0;
int stonesoup_global_variable;
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
void hospitate_limax(int denumerantive_ewald,... );
void reeder_psychosensorial(int bibliopolism_dichlamydeous,char *unintermixed_chignon);
void diureticalness_implacental(int tiddleywink_nonassenting,char *exostoses_idyllists);
void stonesoup_cleanup(FILE **ptrs,int size)
{
  int i = 0;
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpoSxk7b_ss_testcase/src-rose/libavformat/aviobuf.c", "stonesoup_cleanup");
  for (i = 0; i < size; i++) {
    if (ptrs[i] != 0) {
      fclose(ptrs[i]);
    }
  }
}

static void *ffio_url_child_next(void *obj,void *prev)
{
  AVIOContext *s = obj;
  return prev?((void *)0) : s -> opaque;
}

static const AVClass *ffio_url_child_class_next(const AVClass *prev)
{
  return prev?((void *)0) : &ffurl_context_class;
}
static const AVOption ffio_url_options[] = {{(((void *)0))}};
const AVClass ffio_url_class = {.class_name = "AVIOContext", .item_name = av_default_item_name, .version = '4' << 16 | 18 << 8 | 100, .option = ffio_url_options, .child_next = ffio_url_child_next, .child_class_next = ffio_url_child_class_next};
static void fill_buffer(AVIOContext *s);
static int url_resetbuf(AVIOContext *s,int flags);

int ffio_init_context(AVIOContext *s,unsigned char *buffer,int buffer_size,int write_flag,void *opaque,int (*read_packet)(void *, uint8_t *, int ),int (*write_packet)(void *, uint8_t *, int ),int64_t (*seek)(void *, int64_t , int ))
{
  s -> buffer = buffer;
  s -> buffer_size = buffer_size;
  s -> buf_ptr = buffer;
  s -> opaque = opaque;
  s -> direct = 0;
  url_resetbuf(s,(write_flag?2 : 1));
  s -> write_packet = write_packet;
  s -> read_packet = read_packet;
  s -> seek = seek;
  s -> pos = 0;
  s -> must_flush = 0;
  s -> eof_reached = 0;
  s -> error = 0;
  s -> seekable = 0x1;
  s -> max_packet_size = 0;
  s -> update_checksum = ((void *)0);
  if (!read_packet && !write_flag) {
    s -> pos = buffer_size;
    s -> buf_end = s -> buffer + buffer_size;
  }
  s -> read_pause = ((void *)0);
  s -> read_seek = ((void *)0);
  return 0;
}

AVIOContext *avio_alloc_context(unsigned char *buffer,int buffer_size,int write_flag,void *opaque,int (*read_packet)(void *, uint8_t *, int ),int (*write_packet)(void *, uint8_t *, int ),int64_t (*seek)(void *, int64_t , int ))
{
  AVIOContext *s = (av_mallocz(sizeof(AVIOContext )));
  if (!s) {
    return ((void *)0);
  }
  ffio_init_context(s,buffer,buffer_size,write_flag,opaque,read_packet,write_packet,seek);
  return s;
}

static void writeout(AVIOContext *s,const uint8_t *data,int len)
{
  if (s -> write_packet && !s -> error) {
    int ret = (s -> write_packet)(s -> opaque,((uint8_t *)data),len);
    if (ret < 0) {
      s -> error = ret;
    }
  }
  s -> pos += len;
}

static void flush_buffer(AVIOContext *s)
{
  if (s -> buf_ptr > s -> buffer) {
    writeout(s,(s -> buffer),(s -> buf_ptr - s -> buffer));
    if (s -> update_checksum) {
      s -> checksum = ((s -> update_checksum)(s -> checksum,(s -> checksum_ptr),(s -> buf_ptr - s -> checksum_ptr)));
      s -> checksum_ptr = s -> buffer;
    }
  }
  s -> buf_ptr = s -> buffer;
}

void avio_w8(AVIOContext *s,int b)
{
  (void )0;
   *(s -> buf_ptr++) = b;
  if (s -> buf_ptr >= s -> buf_end) {
    flush_buffer(s);
  }
}

void ffio_fill(AVIOContext *s,int b,int count)
{
  while(count > 0){
    int len = (s -> buf_end - s -> buf_ptr > count?count : s -> buf_end - s -> buf_ptr);
    memset((s -> buf_ptr),b,len);
    s -> buf_ptr += len;
    if (s -> buf_ptr >= s -> buf_end) {
      flush_buffer(s);
    }
    count -= len;
  }
}

void avio_write(AVIOContext *s,const unsigned char *buf,int size)
{
  if (s -> direct && !s -> update_checksum) {
    avio_flush(s);
    writeout(s,buf,size);
    return ;
  }
  while(size > 0){
    int len = (s -> buf_end - s -> buf_ptr > size?size : s -> buf_end - s -> buf_ptr);
    memcpy((s -> buf_ptr),buf,len);
    s -> buf_ptr += len;
    if (s -> buf_ptr >= s -> buf_end) {
      flush_buffer(s);
    }
    buf += len;
    size -= len;
  }
}

void avio_flush(AVIOContext *s)
{
  flush_buffer(s);
  s -> must_flush = 0;
}

int64_t avio_seek(AVIOContext *s,int64_t offset,int whence)
{
  int64_t offset1;
  int64_t pos;
  int force = whence & 0x20000;
  whence &= ~0x20000;
  if (!s) {
    return (- 22);
  }
  pos = s -> pos - ((s -> write_flag?0 : s -> buf_end - s -> buffer));
  if (whence != 1 && whence != 0) {
    return (- 22);
  }
  if (whence == 1) {
    offset1 = pos + (s -> buf_ptr - s -> buffer);
    if (offset == 0) {
      return offset1;
    }
    offset += offset1;
  }
  offset1 = offset - pos;
  if (!s -> must_flush && (!s -> direct || !s -> seek) && offset1 >= 0 && offset1 <= s -> buf_end - s -> buffer) {
/* can do the seek inside the buffer */
    s -> buf_ptr = s -> buffer + offset1;
  }
  else {
    if ((!s -> seekable || offset1 <= s -> buf_end + 4096 - s -> buffer) && !s -> write_flag && offset1 >= 0 && (!s -> direct || !s -> seek) && (whence != 2 || force)) {
      while(s -> pos < offset && !s -> eof_reached)
        fill_buffer(s);
      if (s -> eof_reached) {
        return (-((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24)));
      }
      s -> buf_ptr = s -> buf_end + offset - s -> pos;
    }
    else {
      int64_t res;
      if (s -> write_flag) {
        flush_buffer(s);
        s -> must_flush = 1;
      }
      if (!s -> seek) {
        return (- 32);
      }
      if ((res = ((s -> seek)(s -> opaque,offset,0))) < 0) {
        return res;
      }
      s -> seek_count++;
      if (!s -> write_flag) {
        s -> buf_end = s -> buffer;
      }
      s -> buf_ptr = s -> buffer;
      s -> pos = offset;
    }
  }
  s -> eof_reached = 0;
  return offset;
}

int64_t avio_skip(AVIOContext *s,int64_t offset)
{
  return avio_seek(s,offset,1);
}

int64_t avio_size(AVIOContext *s)
{
  int64_t size;
  if (!s) {
    return (- 22);
  }
  if (!s -> seek) {
    return (- 38);
  }
  size = ((s -> seek)(s -> opaque,0,0x10000));
  if (size < 0) {
    if ((size = ((s -> seek)(s -> opaque,(- 1),2))) < 0) {
      return size;
    }
    size++;
    (s -> seek)(s -> opaque,s -> pos,0);
  }
  return size;
}

int url_feof(AVIOContext *s)
{
  if (!s) {
    return 0;
  }
  if (s -> eof_reached) {
    s -> eof_reached = 0;
    fill_buffer(s);
  }
  return s -> eof_reached;
}

void avio_wl32(AVIOContext *s,unsigned int val)
{
  avio_w8(s,((uint8_t )val));
  avio_w8(s,((uint8_t )(val >> 8)));
  avio_w8(s,((uint8_t )(val >> 16)));
  avio_w8(s,(val >> 24));
}

void avio_wb32(AVIOContext *s,unsigned int val)
{
  avio_w8(s,(val >> 24));
  avio_w8(s,((uint8_t )(val >> 16)));
  avio_w8(s,((uint8_t )(val >> 8)));
  avio_w8(s,((uint8_t )val));
}

int avio_put_str(AVIOContext *s,const char *str)
{
  int len = 1;
  if (str) {
    len += strlen(str);
    avio_write(s,((const unsigned char *)str),len);
  }
  else {
    avio_w8(s,0);
  }
  return len;
}

int avio_put_str16le(AVIOContext *s,const char *str)
{
  const uint8_t *q = str;
  int ret = 0;
  while( *q){
    uint32_t ch;
    uint16_t tmp;
    ch = ( *(q++));
{
      uint32_t top = (ch & 128) >> 1;
      if ((ch & 0xc0) == 0x80) {
        break; 
      }
      while(ch & top){
        int tmp = ( *(q++)) - 128;
        if (tmp >> 6) {
          break; 
        }
        ch = (ch << 6) + tmp;
        top <<= 5;
      }
      ch &= (top << 1) - 1;
    }
{
      uint32_t in = ch;
      if (in < 0x10000) {
        tmp = in;
        avio_wl16(s,tmp);
        ret += 2;
      }
      else {
        tmp = (0xD800 | in - 0x10000 >> 10);
        avio_wl16(s,tmp);
        ret += 2;
        tmp = (0xDC00 | in - 0x10000 & 0x3FF);
        avio_wl16(s,tmp);
        ret += 2;
      }
    }
  }
  avio_wl16(s,0);
  ret += 2;
  return ret;
}

int ff_get_v_length(uint64_t val)
{
  int i = 1;
  while(val >>= 7)
    i++;
  return i;
}

void ff_put_v(AVIOContext *bc,uint64_t val)
{
  int i = ff_get_v_length(val);
  while(--i > 0)
    avio_w8(bc,128 | ((uint8_t )(val >> 7 * i)));
  avio_w8(bc,(val & 127));
}

void avio_wl64(AVIOContext *s,uint64_t val)
{
  avio_wl32(s,((uint32_t )(val & 0xffffffff)));
  avio_wl32(s,((uint32_t )(val >> 32)));
}

void avio_wb64(AVIOContext *s,uint64_t val)
{
  avio_wb32(s,((uint32_t )(val >> 32)));
  avio_wb32(s,((uint32_t )(val & 0xffffffff)));
}

void avio_wl16(AVIOContext *s,unsigned int val)
{
  avio_w8(s,((uint8_t )val));
  avio_w8(s,((int )val) >> 8);
}

void avio_wb16(AVIOContext *s,unsigned int val)
{
  avio_w8(s,((int )val) >> 8);
  avio_w8(s,((uint8_t )val));
}

void avio_wl24(AVIOContext *s,unsigned int val)
{
  avio_wl16(s,val & 0xffff);
  avio_w8(s,((int )val) >> 16);
}

void avio_wb24(AVIOContext *s,unsigned int val)
{
  avio_wb16(s,(((int )val) >> 8));
  avio_w8(s,((uint8_t )val));
}
/* Input stream */

static void fill_buffer(AVIOContext *s)
{
  uint8_t *dst = !s -> max_packet_size && s -> buf_end - s -> buffer < (s -> buffer_size)?s -> buf_end : s -> buffer;
  int len = ((s -> buffer_size) - (dst - s -> buffer));
  int max_buffer_size = s -> max_packet_size?s -> max_packet_size : 32768;
/* can't fill the buffer without read_packet, just set EOF if appropriate */
  if (!s -> read_packet && s -> buf_ptr >= s -> buf_end) {
    s -> eof_reached = 1;
  }
/* no need to do anything if EOF already reached */
  if (s -> eof_reached) {
    return ;
  }
  if (s -> update_checksum && dst == s -> buffer) {
    if (s -> buf_end > s -> checksum_ptr) {
      s -> checksum = ((s -> update_checksum)(s -> checksum,(s -> checksum_ptr),(s -> buf_end - s -> checksum_ptr)));
    }
    s -> checksum_ptr = s -> buffer;
  }
/* make buffer smaller in case it ended up large after probing */
  if (s -> read_packet && s -> buffer_size > max_buffer_size) {
    ffio_set_buf_size(s,max_buffer_size);
    s -> checksum_ptr = dst = s -> buffer;
    len = s -> buffer_size;
  }
  if (s -> read_packet) {
    len = ((s -> read_packet)(s -> opaque,dst,len));
  }
  else {
    len = 0;
  }
  if (len <= 0) {
/* do not modify buffer if EOF reached so that a seek back can
           be done without rereading data */
    s -> eof_reached = 1;
    if (len < 0) {
      s -> error = len;
    }
  }
  else {
    s -> pos += len;
    s -> buf_ptr = dst;
    s -> buf_end = dst + len;
    s -> bytes_read += len;
  }
}

unsigned long ff_crc04C11DB7_update(unsigned long checksum,const uint8_t *buf,unsigned int len)
{
  return (av_crc(av_crc_get_table(AV_CRC_32_IEEE),checksum,buf,len));
}

unsigned long ffio_get_checksum(AVIOContext *s)
{
  s -> checksum = ((s -> update_checksum)(s -> checksum,(s -> checksum_ptr),(s -> buf_ptr - s -> checksum_ptr)));
  s -> update_checksum = ((void *)0);
  return s -> checksum;
}

void ffio_init_checksum(AVIOContext *s,unsigned long (*update_checksum)(unsigned long , const uint8_t *, unsigned int ),unsigned long checksum)
{
  s -> update_checksum = update_checksum;
  if (s -> update_checksum) {
    s -> checksum = checksum;
    s -> checksum_ptr = s -> buf_ptr;
  }
}
/* XXX: put an inline version */

int avio_r8(AVIOContext *s)
{
  if (s -> buf_ptr >= s -> buf_end) {
    fill_buffer(s);
  }
  if (s -> buf_ptr < s -> buf_end) {
    return ( *(s -> buf_ptr++));
  }
  return 0;
}

int avio_read(AVIOContext *s,unsigned char *buf,int size)
{
  int len;
  int size1;
  size1 = size;
  while(size > 0){
    len = (s -> buf_end - s -> buf_ptr);
    if (len > size) {
      len = size;
    }
    if (len == 0 || s -> write_flag) {
      if ((s -> direct || size > s -> buffer_size) && !s -> update_checksum) {
        if (s -> read_packet) {
          len = ((s -> read_packet)(s -> opaque,buf,size));
        }
        if (len <= 0) {
/* do not modify buffer if EOF reached so that a seek back can
                    be done without rereading data */
          s -> eof_reached = 1;
          if (len < 0) {
            s -> error = len;
          }
          break; 
        }
        else {
          s -> pos += len;
          s -> bytes_read += len;
          size -= len;
          buf += len;
          s -> buf_ptr = s -> buffer;
/* + len*/
          s -> buf_end = s -> buffer;
        }
      }
      else {
        fill_buffer(s);
        len = (s -> buf_end - s -> buf_ptr);
        if (len == 0) {
          break; 
        }
      }
    }
    else {
      memcpy(buf,(s -> buf_ptr),len);
      buf += len;
      s -> buf_ptr += len;
      size -= len;
    }
  }
  if (size1 == size) {
    if (s -> error) {
      return s -> error;
    }
    if (url_feof(s)) {
      return -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
    }
  }
  return size1 - size;
}

int ffio_read_partial(AVIOContext *s,unsigned char *buf,int size)
{
  int len;
  if (size < 0) {
    return - 1;
  }
  if (s -> read_packet && s -> write_flag) {
    len = ((s -> read_packet)(s -> opaque,buf,size));
    if (len > 0) {
      s -> pos += len;
    }
    return len;
  }
  len = (s -> buf_end - s -> buf_ptr);
  if (len == 0) {
/* Reset the buf_end pointer to the start of the buffer, to make sure
         * the fill_buffer call tries to read as much data as fits into the
         * full buffer, instead of just what space is left after buf_end.
         * This avoids returning partial packets at the end of the buffer,
         * for packet based inputs.
         */
    s -> buf_end = s -> buf_ptr = s -> buffer;
    fill_buffer(s);
    len = (s -> buf_end - s -> buf_ptr);
  }
  if (len > size) {
    len = size;
  }
  memcpy(buf,(s -> buf_ptr),len);
  s -> buf_ptr += len;
  if (!len) {
    if (s -> error) {
      return s -> error;
    }
    if (url_feof(s)) {
      return -((int )(('E' | 'O' << 8 | 'F' << 16) | ((unsigned int )32) << 24));
    }
  }
  return len;
}

unsigned int avio_rl16(AVIOContext *s)
{
  unsigned int val;
  val = (avio_r8(s));
  val |= (avio_r8(s) << 8);
  return val;
}

unsigned int avio_rl24(AVIOContext *s)
{
  unsigned int val;
  val = avio_rl16(s);
  val |= (avio_r8(s) << 16);
  return val;
}

unsigned int avio_rl32(AVIOContext *s)
{
  unsigned int val;
  val = avio_rl16(s);
  val |= avio_rl16(s) << 16;
  return val;
}

uint64_t avio_rl64(AVIOContext *s)
{
  uint64_t val;
  val = ((uint64_t )(avio_rl32(s)));
  val |= ((uint64_t )(avio_rl32(s))) << 32;
  return val;
}

unsigned int avio_rb16(AVIOContext *s)
{
  unsigned int val;
  val = (avio_r8(s) << 8);
  val |= (avio_r8(s));
  return val;
}

unsigned int avio_rb24(AVIOContext *s)
{
  unsigned int val;
  val = avio_rb16(s) << 8;
  val |= (avio_r8(s));
  return val;
}

unsigned int avio_rb32(AVIOContext *s)
{
  unsigned int val;
  val = avio_rb16(s) << 16;
  val |= avio_rb16(s);
  return val;
}

int ff_get_line(AVIOContext *s,char *buf,int maxlen)
{
  int i = 0;
  char c;
  do {
    c = (avio_r8(s));
    if (c && i < maxlen - 1) {
      buf[i++] = c;
    }
  }while (c != 10 && c);
  buf[i] = 0;
  return i;
}

int avio_get_str(AVIOContext *s,int maxlen,char *buf,int buflen)
{
  int i;
  if (buflen <= 0) {
    return - 22;
  }
// reserve 1 byte for terminating 0
  buflen = (buflen - 1 > maxlen?maxlen : buflen - 1);
  for (i = 0; i < buflen; i++) 
    if (!(buf[i] = (avio_r8(s)))) {
      return i + 1;
    }
  buf[i] = 0;
  for (; i < maxlen; i++) 
    if (!avio_r8(s)) {
      return i + 1;
    }
  return maxlen;
}
#define GET_STR16(type, read) \
    int avio_get_str16 ##type(AVIOContext *pb, int maxlen, char *buf, int buflen)\
{\
    char* q = buf;\
    int ret = 0;\
    if (buflen <= 0) \
        return AVERROR(EINVAL); \
    while (ret + 1 < maxlen) {\
        uint8_t tmp;\
        uint32_t ch;\
        GET_UTF16(ch, (ret += 2) <= maxlen ? read(pb) : 0, break;)\
        if (!ch)\
            break;\
        PUT_UTF8(ch, tmp, if (q - buf < buflen - 1) *q++ = tmp;)\
    }\
    *q = 0;\
    return ret;\
}\


int avio_get_str16le(AVIOContext *pb,int maxlen,char *buf,int buflen)
{
  char *q = buf;
  int ret = 0;
  if (buflen <= 0) {
    return - 22;
  }
  while(ret + 1 < maxlen){
    uint8_t tmp;
    uint32_t ch;
    ch = ((ret += 2) <= maxlen?avio_rl16(pb) : 0);
{
      unsigned int hi = ch - 0xD800;
      if (hi < 0x800) {
        ch = ((ret += 2) <= maxlen?avio_rl16(pb) : (0 - 0xDC00));
        if (ch > 0x3FFU || hi > 0x3FFU) {
          break; 
        }
        ch += (hi << 10) + 0x10000;
      }
    }
    if (!ch) {
      break; 
    }
{
      int bytes;
      int shift;
      uint32_t in = ch;
      if (in < 0x80) {
        tmp = in;
        if (q - buf < (buflen - 1)) {
           *(q++) = tmp;
        }
      }
      else {
        bytes = (ff_log2_c(in) + 4) / 5;
        shift = (bytes - 1) * 6;
        tmp = ((256 - (256 >> bytes)) | in >> shift);
        if (q - buf < (buflen - 1)) {
           *(q++) = tmp;
        }
        while(shift >= 6){
          shift -= 6;
          tmp = (0x80 | in >> shift & 0x3F);
          if (q - buf < (buflen - 1)) {
             *(q++) = tmp;
          }
        }
      }
    }
  }
   *q = 0;
  return ret;
}

int avio_get_str16be(AVIOContext *pb,int maxlen,char *buf,int buflen)
{
  char *q = buf;
  int ret = 0;
  if (buflen <= 0) {
    return - 22;
  }
  while(ret + 1 < maxlen){
    uint8_t tmp;
    uint32_t ch;
    ch = ((ret += 2) <= maxlen?avio_rb16(pb) : 0);
{
      unsigned int hi = ch - 0xD800;
      if (hi < 0x800) {
        ch = ((ret += 2) <= maxlen?avio_rb16(pb) : (0 - 0xDC00));
        if (ch > 0x3FFU || hi > 0x3FFU) {
          break; 
        }
        ch += (hi << 10) + 0x10000;
      }
    }
    if (!ch) {
      break; 
    }
{
      int bytes;
      int shift;
      uint32_t in = ch;
      if (in < 0x80) {
        tmp = in;
        if (q - buf < (buflen - 1)) {
           *(q++) = tmp;
        }
      }
      else {
        bytes = (ff_log2_c(in) + 4) / 5;
        shift = (bytes - 1) * 6;
        tmp = ((256 - (256 >> bytes)) | in >> shift);
        if (q - buf < (buflen - 1)) {
           *(q++) = tmp;
        }
        while(shift >= 6){
          shift -= 6;
          tmp = (0x80 | in >> shift & 0x3F);
          if (q - buf < (buflen - 1)) {
             *(q++) = tmp;
          }
        }
      }
    }
  }
   *q = 0;
  return ret;
}
#undef GET_STR16

uint64_t avio_rb64(AVIOContext *s)
{
  uint64_t val;
  val = ((uint64_t )(avio_rb32(s))) << 32;
  val |= ((uint64_t )(avio_rb32(s)));
  return val;
}

uint64_t ffio_read_varlen(AVIOContext *bc)
{
  uint64_t val = 0;
  int tmp;
  do {
    tmp = avio_r8(bc);
    val = (val << 7) + (tmp & 127);
  }while (tmp & 128);
  return val;
}

int ffio_fdopen(AVIOContext **s,URLContext *h)
{
  uint8_t *buffer;
  int buffer_size;
  int max_packet_size;
  max_packet_size = h -> max_packet_size;
  if (max_packet_size) {
/* no need to bufferize more than one packet */
    buffer_size = max_packet_size;
  }
  else {
    buffer_size = 32768;
  }
  buffer = (av_malloc(buffer_size));
  if (!buffer) {
    return - 12;
  }
   *s = avio_alloc_context(buffer,buffer_size,h -> flags & 2,h,((void *)ffurl_read),((void *)ffurl_write),((void *)ffurl_seek));
  if (!( *s)) {
    av_free(buffer);
    return - 12;
  }
  ( *s) -> direct = h -> flags & 0x8000;
  ( *s) -> seekable = (h -> is_streamed?0 : 0x1);
  ( *s) -> max_packet_size = max_packet_size;
  if (h -> prot) {
    ( *s) -> read_pause = ((int (*)(void *, int ))(h -> prot -> url_read_pause));
    ( *s) -> read_seek = ((int64_t (*)(void *, int , int64_t , int ))(h -> prot -> url_read_seek));
  }
  ( *s) -> av_class = &ffio_url_class;
  return 0;
}

int ffio_set_buf_size(AVIOContext *s,int buf_size)
{
  uint8_t *buffer;
  buffer = (av_malloc(buf_size));
  if (!buffer) {
    return - 12;
  }
  av_free((s -> buffer));
  s -> buffer = buffer;
  s -> buffer_size = buf_size;
  s -> buf_ptr = buffer;
  url_resetbuf(s,(s -> write_flag?2 : 1));
  return 0;
}

static int url_resetbuf(AVIOContext *s,int flags)
{
  (void )0;
  if (flags & 2) {
    s -> buf_end = s -> buffer + s -> buffer_size;
    s -> write_flag = 1;
  }
  else {
    s -> buf_end = s -> buffer;
    s -> write_flag = 0;
  }
  return 0;
}

int ffio_rewind_with_probe_data(AVIOContext *s,unsigned char **bufp,int buf_size)
{
  int64_t buffer_start;
  int buffer_size;
  int overlap;
  int new_size;
  int alloc_size;
  uint8_t *buf =  *bufp;
  if (s -> write_flag) {
    av_freep(bufp);
    return - 22;
  }
  buffer_size = (s -> buf_end - s -> buffer);
/* the buffers must touch or overlap */
  if ((buffer_start = s -> pos - buffer_size) > buf_size) {
    av_freep(bufp);
    return - 22;
  }
  overlap = (buf_size - buffer_start);
  new_size = buf_size + buffer_size - overlap;
  alloc_size = (s -> buffer_size > new_size?s -> buffer_size : new_size);
  if (alloc_size > buf_size) {
    if (!(buf =  *bufp = (av_realloc_f(buf,1,alloc_size)))) {
      return - 12;
    }
  }
  if (new_size > buf_size) {
    memcpy((buf + buf_size),(s -> buffer + overlap),(buffer_size - overlap));
    buf_size = new_size;
  }
  av_free((s -> buffer));
  s -> buf_ptr = s -> buffer = buf;
  s -> buffer_size = alloc_size;
  s -> pos = buf_size;
  s -> buf_end = s -> buf_ptr + buf_size;
  s -> eof_reached = 0;
  s -> must_flush = 0;
  return 0;
}

int avio_open(AVIOContext **s,const char *filename,int flags)
{
  return avio_open2(s,filename,flags,((void *)0),((void *)0));
}

int avio_open2(AVIOContext **s,const char *filename,int flags,const AVIOInterruptCB *int_cb,AVDictionary **options)
{
  int lamarckian_cardiopath = 0;
  char *pseudovolcano_fos = 0;
  char *faldage_exogenae;
  URLContext *h;
  int err;
  if (__sync_bool_compare_and_swap(&coeymans_tidehead,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpoSxk7b_ss_testcase/src-rose/libavformat/aviobuf.c","avio_open2");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&faldage_exogenae,"HIRAI_BARBIERITE");
      if (faldage_exogenae != 0) {;
        lamarckian_cardiopath = ((int )(strlen(faldage_exogenae)));
        pseudovolcano_fos = ((char *)(malloc(lamarckian_cardiopath + 1)));
        if (pseudovolcano_fos == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(pseudovolcano_fos,0,lamarckian_cardiopath + 1);
        memcpy(pseudovolcano_fos,faldage_exogenae,lamarckian_cardiopath);
        if (faldage_exogenae != 0) 
          free(((char *)faldage_exogenae));
        hospitate_limax(1,pseudovolcano_fos);
      }
    }
  }
  err = ffurl_open(&h,filename,flags,int_cb,options);
  if (err < 0) {
    return err;
  }
  err = ffio_fdopen(s,h);
  if (err < 0) {
    ffurl_close(h);
    return err;
  }
  return 0;
}

int avio_close(AVIOContext *s)
{
  URLContext *h;
  if (!s) {
    return 0;
  }
  avio_flush(s);
  h = (s -> opaque);
  av_freep((&s -> buffer));
  if (!s -> write_flag) {
    av_log(s,48,"Statistics: %ld bytes read, %d seeks\n",s -> bytes_read,s -> seek_count);
  }
  av_free(s);
  return ffurl_close(h);
}

int avio_closep(AVIOContext **s)
{
  int ret = avio_close( *s);
   *s = ((void *)0);
  return ret;
}

int avio_printf(AVIOContext *s,const char *fmt,... )
{
  va_list ap;
  char buf[4096];
  int ret;
  __builtin_va_start(ap,fmt);
  ret = vsnprintf(buf,sizeof(buf),fmt,ap);
  __builtin_va_end(ap);
  avio_write(s,buf,(strlen(buf)));
  return ret;
}

int avio_pause(AVIOContext *s,int pause)
{
  if (!s -> read_pause) {
    return - 38;
  }
  return (s -> read_pause)(s -> opaque,pause);
}

int64_t avio_seek_time(AVIOContext *s,int stream_index,int64_t timestamp,int flags)
{
  URLContext *h = (s -> opaque);
  int64_t ret;
  if (!s -> read_seek) {
    return (- 38);
  }
  ret = ((s -> read_seek)(h,stream_index,timestamp,flags));
  if (ret >= 0) {
    int64_t pos;
// Flush buffer
    s -> buf_ptr = s -> buf_end;
    pos = ((s -> seek)(h,0,1));
    if (pos >= 0) {
      s -> pos = pos;
    }
    else {
      if (pos != (- 38)) {
        ret = pos;
      }
    }
  }
  return ret;
}
/* output in a dynamic buffer */
typedef struct DynBuffer {
int pos;
int size;
int allocated_size;
uint8_t *buffer;
int io_buffer_size;
uint8_t io_buffer[1];}DynBuffer;

static int dyn_buf_write(void *opaque,uint8_t *buf,int buf_size)
{
  DynBuffer *d = opaque;
  unsigned int new_size;
  unsigned int new_allocated_size;
/* reallocate buffer if needed */
  new_size = (d -> pos + buf_size);
  new_allocated_size = (d -> allocated_size);
  if (new_size < (d -> pos) || new_size > (2147483647 / 2)) {
    return - 1;
  }
  while(new_size > new_allocated_size){
    if (!new_allocated_size) {
      new_allocated_size = new_size;
    }
    else {
      new_allocated_size += new_allocated_size / 2 + 1;
    }
  }
  if (new_allocated_size > (d -> allocated_size)) {
    d -> buffer = (av_realloc_f((d -> buffer),1,new_allocated_size));
    if (d -> buffer == ((void *)0)) {
      return - 12;
    }
    d -> allocated_size = new_allocated_size;
  }
  memcpy((d -> buffer + d -> pos),buf,buf_size);
  d -> pos = new_size;
  if (d -> pos > d -> size) {
    d -> size = d -> pos;
  }
  return buf_size;
}

static int dyn_packet_buf_write(void *opaque,uint8_t *buf,int buf_size)
{
  unsigned char buf1[4];
  int ret;
/* packetized write: output the header */
  ((union unaligned_32 *)buf1) -> l = av_bswap32(buf_size);
  ret = dyn_buf_write(opaque,buf1,4);
  if (ret < 0) {
    return ret;
  }
/* then the data */
  return dyn_buf_write(opaque,buf,buf_size);
}

static int64_t dyn_buf_seek(void *opaque,int64_t offset,int whence)
{
  DynBuffer *d = opaque;
  if (whence == 1) {
    offset += (d -> pos);
  }
  else {
    if (whence == 2) {
      offset += (d -> size);
    }
  }
  if (offset < 0 || offset > 0x7fffffffLL) {
    return (- 1);
  }
  d -> pos = offset;
  return 0;
}

static int url_open_dyn_buf_internal(AVIOContext **s,int max_packet_size)
{
  DynBuffer *d;
  unsigned int io_buffer_size = (max_packet_size?max_packet_size : 1024);
  if (sizeof(DynBuffer ) + io_buffer_size < io_buffer_size) {
    return - 1;
  }
  d = (av_mallocz(sizeof(DynBuffer ) + io_buffer_size));
  if (!d) {
    return - 12;
  }
  d -> io_buffer_size = io_buffer_size;
   *s = avio_alloc_context(d -> io_buffer,d -> io_buffer_size,1,d,((void *)0),(max_packet_size?dyn_packet_buf_write : dyn_buf_write),(max_packet_size?((void *)0) : dyn_buf_seek));
  if (!( *s)) {
    av_free(d);
    return - 12;
  }
  ( *s) -> max_packet_size = max_packet_size;
  return 0;
}

int avio_open_dyn_buf(AVIOContext **s)
{
  return url_open_dyn_buf_internal(s,0);
}

int ffio_open_dyn_packet_buf(AVIOContext **s,int max_packet_size)
{
  if (max_packet_size <= 0) {
    return - 1;
  }
  return url_open_dyn_buf_internal(s,max_packet_size);
}

int avio_close_dyn_buf(AVIOContext *s,uint8_t **pbuffer)
{
  DynBuffer *d = (s -> opaque);
  int size;
  static const char padbuf[16] = {(0)};
  int padding = 0;
/* don't attempt to pad fixed-size packet buffers */
  if (!s -> max_packet_size) {
    avio_write(s,padbuf,(sizeof(padbuf)));
    padding = 16;
  }
  avio_flush(s);
   *pbuffer = d -> buffer;
  size = d -> size;
  av_free(d);
  av_free(s);
  return size - padding;
}

void hospitate_limax(int denumerantive_ewald,... )
{
  int unpopularized_estevan = 7;
  char *uncivilizedly_paracephalus = 0;
  va_list pullicate_overclothes;
  ++stonesoup_global_variable;;
  if (denumerantive_ewald > 0) {
    __builtin_va_start(pullicate_overclothes,denumerantive_ewald);
    uncivilizedly_paracephalus = (va_arg(pullicate_overclothes,char *));
    __builtin_va_end(pullicate_overclothes);
  }
  reeder_psychosensorial(unpopularized_estevan,uncivilizedly_paracephalus);
}

void reeder_psychosensorial(int bibliopolism_dichlamydeous,char *unintermixed_chignon)
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[10];
 char stonesoup_filename[80];
  char *ringed_crowdle = 0;
  ++stonesoup_global_variable;
  bibliopolism_dichlamydeous--;
  if (bibliopolism_dichlamydeous > 0) {
    diureticalness_implacental(bibliopolism_dichlamydeous,unintermixed_chignon);
    return ;
  }
  ringed_crowdle = ((char *)unintermixed_chignon);
    tracepoint(stonesoup_trace, weakness_start, "CWE773", "A", "Missing Reference to Active File Descriptor or Handle");
 stonesoup_files = fopen(ringed_crowdle,"r");
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
  if (unintermixed_chignon != 0) 
    free(((char *)unintermixed_chignon));
stonesoup_close_printf_context();
}

void diureticalness_implacental(int tiddleywink_nonassenting,char *exostoses_idyllists)
{
  ++stonesoup_global_variable;
  reeder_psychosensorial(tiddleywink_nonassenting,exostoses_idyllists);
}
