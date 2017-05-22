/* file_wrappers.c
 *
 * $Id: file_wrappers.c 43105 2012-06-05 08:59:37Z guy $
 *
 * Wiretap Library
 * Copyright (c) 1998 by Gilbert Ramirez <gram@alumni.rice.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/* file_access interface based heavily on zlib gzread.c and gzlib.c from zlib
 * Copyright (C) 1995-2010 Jean-loup Gailly and Mark Adler
 * under licence:
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <errno.h>
#include <stdio.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */
#include <string.h>
#include "wtap-int.h"
#include "file_wrappers.h"
#include <wsutil/file_util.h>
#ifdef HAVE_LIBZ
#include <zlib.h>
#endif /* HAVE_LIBZ */
/*
 * See RFC 1952 for a description of the gzip file format.
 *
 * Some other compressed file formats we might want to support:
 *
 *	XZ format: http://tukaani.org/xz/
 *
 *	Bzip2 format: http://bzip.org/
 */
/*
 * List of extensions for compressed files.
 * If we add support for more compressed file types, this table
 * might be expanded to include routines to handle the various
 * compression types.
 */
#include <stdlib.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
static const char *compressed_file_extensions[] = {("gz"), (((void *)0))
#ifdef HAVE_LIBZ
#endif
};
/*
 * Return a GSList of all the compressed file extensions.
 * The data pointers all point to items in compressed_file_extensions[],
 * so the GSList can just be freed with g_slist_free().
 */
int rhv_bacteriolysin = 0;
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
char **dealt_volva(char **dendrograph_draconism);
void coresign_adamantine(int pantagruelic_bedlamise,char **jobi_unequitableness);

GSList *wtap_get_compressed_file_extensions()
{
  const char **extension;
  GSList *extensions;
  extensions = ((void *)0);
  for (extension = &compressed_file_extensions[0];  *extension != ((void *)0); extension++) 
    extensions = g_slist_append(extensions,((gpointer )( *extension)));
  return extensions;
}
/* #define GZBUFSIZE 8192 */
#define GZBUFSIZE 4096

struct wtap_reader 
{
/* file descriptor */
  int fd;
/* current position in file (just to not call lseek()) */
  gint64 raw_pos;
/* current position in uncompressed data */
  gint64 pos;
/* buffer size */
  unsigned int size;
/* input buffer */
  unsigned char *in;
/* output buffer (double-sized when reading) */
  unsigned char *out;
/* next output data to deliver or write */
  unsigned char *next;
/* amount of output data unused at next */
  unsigned int have;
/* true if end of input file reached */
  int eof;
/* where the gzip data started, for rewinding */
  gint64 start;
/* where the raw data started, for seeking */
  gint64 raw;
/* 0: ?, 1: uncompressed, 2: zlib */
  int compression;
/* FALSE if completely uncompressed, TRUE otherwise */
  gboolean is_compressed;
/* seek request */
/* amount to skip (already rewound if backwards) */
  gint64 skip;
/* true if seek request pending */
  int seek;
/* error information */
/* error code */
  int err;
/* additional error information string for some errors */
  const char *err_info;
/* number of bytes available at next_in */
  unsigned int avail_in;
/* next input byte */
  unsigned char *next_in;
#ifdef HAVE_LIBZ
/* zlib inflate stream */
/* stream structure in-place (not a pointer) */
  z_stream strm;
/* 1 if we aren't supposed to check the CRC */
  int dont_check_crc;
#endif
/* fast seeking */
  GPtrArray *fast_seek;
  void *fast_seek_cur;
}
;
/* values for wtap_reader compression */
#define UNKNOWN		0	/* look for a gzip header */
#define UNCOMPRESSED	1	/* copy input directly */
#ifdef HAVE_LIBZ
#define ZLIB		2	/* decompress a zlib stream */
#define GZIP_AFTER_HEADER 3
#endif
/* gz_load */

static int raw_read(FILE_T state,unsigned char *buf,unsigned int count,unsigned int *have)
{
  int ret;
   *have = 0;
  do {
    ret = (read(state -> fd,(buf +  *have),(count -  *have)));
    if (ret <= 0) {
      break; 
    }
     *have += ret;
    state -> raw_pos += ret;
  }while ( *have < count);
  if (ret < 0) {
    state -> err =  *__errno_location();
    state -> err_info = ((void *)0);
    return - 1;
  }
  if (ret == 0) {
    state -> eof = 1;
  }
  return 0;
}
/* gz_avail */

static int fill_in_buffer(FILE_T state)
{
  if (state -> err) {
    return - 1;
  }
  if (state -> eof == 0) {
    if (raw_read(state,state -> in,state -> size,((unsigned int *)(&state -> avail_in))) == - 1) {
      return - 1;
    }
    state -> next_in = state -> in;
  }
  return 0;
}
#define ZLIB_WINSIZE 32768

struct fast_seek_point 
{
/* corresponding offset in uncompressed data */
  gint64 out;
/* offset in input file of first full byte */
  gint64 in;
  int compression;
  union {
  struct {
#ifdef HAVE_INFLATEPRIME
/* number of bits (1-7) from byte at in - 1, or 0 */
  int bits;
#endif
/* preceding 32K of uncompressed data */
  unsigned char window[32768];
/* be gentle with Z_STREAM_END, 8 bytes more... Another solution would be to comment checks out */
  guint32 adler;
  guint32 total_out;}zlib;}data;
}
;

struct zlib_cur_seek_point 
{
/* preceding 32K of uncompressed data */
  unsigned char window[32768];
  unsigned int pos;
  unsigned int have;
}
;
#define SPAN G_GINT64_CONSTANT(1048576)

static struct fast_seek_point *fast_seek_find(FILE_T file,gint64 pos)
{
  struct fast_seek_point *smallest = ((void *)0);
  struct fast_seek_point *item;
  guint low;
  guint i;
  guint max;
  if (!file -> fast_seek) {
    return ((void *)0);
  }
  for ((low = 0 , max = file -> fast_seek -> len); low < max; ) {
    i = (low + max) / 2;
    item = ((struct fast_seek_point *)file -> fast_seek -> pdata[i]);
    if (pos < item -> out) {
      max = i;
    }
    else {
      if (pos > item -> out) {
        smallest = item;
        low = i + 1;
      }
      else {
        return item;
      }
    }
  }
  return smallest;
}

static void fast_seek_header(FILE_T file,gint64 in_pos,gint64 out_pos,int compression)
{
  struct fast_seek_point *item = ((void *)0);
  if (file -> fast_seek -> len != 0) {
    item = ((struct fast_seek_point *)file -> fast_seek -> pdata[file -> fast_seek -> len - 1]);
  }
  if (!item || item -> out < out_pos) {
    struct fast_seek_point *val = (struct fast_seek_point *)(g_malloc_n(1,sizeof(struct fast_seek_point )));
    val -> in = in_pos;
    val -> out = out_pos;
    val -> compression = compression;
    g_ptr_array_add(file -> fast_seek,val);
  }
}

static void fast_seek_reset(FILE_T state)
{
#ifdef HAVE_LIBZ
  if (state -> compression == 2 && state -> fast_seek_cur) {
    struct zlib_cur_seek_point *cur = (struct zlib_cur_seek_point *)(state -> fast_seek_cur);
    cur -> have = 0;
  }
#endif
}
#ifdef HAVE_LIBZ
/* Get next byte from input, or -1 if end or error.
 *
 * Note:
 *
 *	1) errors from raw_read(), and thus from fill_in_buffer(), are
 *	"sticky", and fill_in_buffer() won't do any reading if there's
 *	an error;
 *
 *	2) GZ_GETC() returns -1 on an EOF;
 *
 * so it's safe to make multiple GZ_GETC() calls and only check the
 * last one for an error. */
#define GZ_GETC() ((state->avail_in == 0 && fill_in_buffer(state) == -1) ? -1 : \
                (state->avail_in == 0 ? -1 : \
                 (state->avail_in--, *(state->next_in)++)))
/* Get a one-byte integer and return 0 on success and the value in *ret.
   Otherwise -1 is returned, state->err is set, and *ret is not modified. */

static int gz_next1(FILE_T state,guint8 *ret)
{
  int ch;
  ch = (state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++))))));
  if (ch == - 1) {
    if (state -> err == 0) {
/* EOF */
      state -> err = - 12;
      state -> err_info = ((void *)0);
    }
    return - 1;
  }
   *ret = ch;
  return 0;
}
/* Get a two-byte little-endian integer and return 0 on success and the value
   in *ret.  Otherwise -1 is returned, state->err is set, and *ret is not
   modified. */

static int gz_next2(FILE_T state,guint16 *ret)
{
  guint16 val;
  int ch;
  val = ((state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++)))))));
  ch = (state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++))))));
  if (ch == - 1) {
    if (state -> err == 0) {
/* EOF */
      state -> err = - 12;
      state -> err_info = ((void *)0);
    }
    return - 1;
  }
  val += ((guint16 )ch) << 8;
   *ret = val;
  return 0;
}
/* Get a four-byte little-endian integer and return 0 on success and the value
   in *ret.  Otherwise -1 is returned, state->err is set, and *ret is not
   modified. */

static int gz_next4(FILE_T state,guint32 *ret)
{
  guint32 val;
  int ch;
  val = ((state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++)))))));
  val += ((unsigned int )((state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++)))))))) << 8;
  val += ((guint32 )((state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++)))))))) << 16;
  ch = (state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++))))));
  if (ch == - 1) {
    if (state -> err == 0) {
/* EOF */
      state -> err = - 12;
      state -> err_info = ((void *)0);
    }
    return - 1;
  }
  val += ((guint32 )ch) << 24;
   *ret = val;
  return 0;
}
/* Skip the specified number of bytes and return 0 on success.  Otherwise -1
   is returned. */

static int gz_skipn(FILE_T state,size_t n)
{
  while(n != 0){
    if (((state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++))))))) == - 1) {
      if (state -> err == 0) {
/* EOF */
        state -> err = - 12;
        state -> err_info = ((void *)0);
      }
      return - 1;
    }
    n--;
  }
  return 0;
}
/* Skip a null-terminated string and return 0 on success.  Otherwise -1
   is returned. */

static int gz_skipzstr(FILE_T state)
{
  int ch;
/* It's null-terminated, so scan until we read a byte with
	   the value 0 or get an error. */
  while((ch = (state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++))))))) > 0)
    ;
  if (ch == - 1) {
    if (state -> err == 0) {
/* EOF */
      state -> err = - 12;
      state -> err_info = ((void *)0);
    }
    return - 1;
  }
  return 0;
}

static void zlib_fast_seek_add(FILE_T file,struct zlib_cur_seek_point *point,int bits,gint64 in_pos,gint64 out_pos)
{
/* it's for sure after gzip header, so file->fast_seek->len != 0 */
  struct fast_seek_point *item = (struct fast_seek_point *)file -> fast_seek -> pdata[file -> fast_seek -> len - 1];
#ifndef HAVE_INFLATEPRIME
#endif
/* Glib has got Balanced Binary Trees (GTree) but I couldn't find a way to do quick search for nearest (and smaller) value to seek (It's what fast_seek_find() do)
	 *      Inserting value in middle of sorted array is expensive, so we want to add only in the end.
	 *      It's not big deal, cause first-read don't usually invoke seeking
	 */
  if (item -> out + 1048576L < out_pos) {
    struct fast_seek_point *val = (struct fast_seek_point *)(g_malloc_n(1,sizeof(struct fast_seek_point )));
    val -> in = in_pos;
    val -> out = out_pos;
    val -> compression = 2;
#ifdef HAVE_INFLATEPRIME
    val -> data . zlib . bits = bits;
#endif
    if (point -> pos != 0) {
      unsigned int left = 32768 - point -> pos;
      memcpy(val -> data . zlib . window,(point -> window + point -> pos),left);
      memcpy((val -> data . zlib . window + left),(point -> window),(point -> pos));
    }
    else {
      memcpy(val -> data . zlib . window,(point -> window),32768);
    }
    val -> data . zlib . adler = file -> strm . adler;
    val -> data . zlib . total_out = file -> strm . total_out;
    g_ptr_array_add(file -> fast_seek,val);
  }
}
/* gz_decomp */

static void zlib_read(FILE_T state,unsigned char *buf,unsigned int count)
{
/* XXX */
  int ret = 0;
  guint32 crc;
  guint32 len;
  z_streamp strm = &state -> strm;
  unsigned char *buf2 = buf;
  unsigned int count2 = count;
  strm -> avail_out = count;
  strm -> next_out = buf;
/* fill output buffer up to end of deflate stream or error */
  do {
/* get more input for inflate() */
    if (state -> avail_in == 0 && fill_in_buffer(state) == - 1) {
      break; 
    }
    if (state -> avail_in == 0) {
/* EOF */
      state -> err = - 12;
      state -> err_info = ((void *)0);
      break; 
    }
    strm -> avail_in = state -> avail_in;
    strm -> next_in = state -> next_in;
/* decompress and handle errors */
#ifdef Z_BLOCK
    ret = inflate(strm,5);
#else
#endif
    state -> avail_in = strm -> avail_in;
    state -> next_in = strm -> next_in;
    if (ret == - 2) {
      state -> err = - 21;
      state -> err_info = (strm -> msg);
      break; 
    }
    if (ret == 2) {
      state -> err = - 21;
      state -> err_info = "preset dictionary needed";
      break; 
    }
    if (ret == - 4) {
/* This means "not enough memory". */
      state -> err = 12;
      state -> err_info = ((void *)0);
      break; 
    }
/* deflate stream invalid */
    if (ret == - 3) {
      state -> err = - 21;
      state -> err_info = (strm -> msg);
      break; 
    }
/*
		 * XXX - Z_BUF_ERROR?
		 */
    strm -> adler = crc32(strm -> adler,buf2,count2 - strm -> avail_out);
#ifdef Z_BLOCK
    if (state -> fast_seek_cur) {
      struct zlib_cur_seek_point *cur = (struct zlib_cur_seek_point *)(state -> fast_seek_cur);
      unsigned int ready = count2 - strm -> avail_out;
      if (ready < 32768) {
        unsigned int left = 32768 - cur -> pos;
        if (ready >= left) {
          memcpy((cur -> window + cur -> pos),buf2,left);
          if (ready != left) {
            memcpy((cur -> window),(buf2 + left),(ready - left));
          }
          cur -> pos = ready - left;
          cur -> have += ready;
        }
        else {
          memcpy((cur -> window + cur -> pos),buf2,ready);
          cur -> pos += ready;
          cur -> have += ready;
        }
        if (cur -> have >= 32768) {
          cur -> have = 32768;
        }
      }
      else {
        memcpy((cur -> window),(buf2 + (ready - 32768)),32768);
        cur -> pos = 0;
        cur -> have = 32768;
      }
      if (cur -> have >= 32768 && ret != 1 && strm -> data_type & 128 && !(strm -> data_type & 64)) {
        zlib_fast_seek_add(state,cur,strm -> data_type & 7,state -> raw_pos - (strm -> avail_in),state -> pos + (count - strm -> avail_out));
      }
    }
#endif
    buf2 = buf2 + count2 - strm -> avail_out;
    count2 = strm -> avail_out;
  }while (strm -> avail_out && ret != 1);
/* update available output and crc check value */
  state -> next = buf;
  state -> have = count - strm -> avail_out;
/* Check gzip trailer if at end of deflate stream.
	   We don't fail immediately here, we just set an error
	   indication, so that we try to process what data we
	   got before the error.  The next attempt to read
	   something past that data will get the error. */
  if (ret == 1) {
    if (gz_next4(state,&crc) != - 1 && gz_next4(state,&len) != - 1) {
      if (crc != strm -> adler && !state -> dont_check_crc) {
        state -> err = - 21;
        state -> err_info = "bad CRC";
      }
      else {
        if (len != (strm -> total_out & 0xffffffffL)) {
          state -> err = - 21;
          state -> err_info = "length field wrong";
        }
      }
    }
/* ready for next stream, once have is 0 */
    state -> compression = 0;
    g_free(state -> fast_seek_cur);
    state -> fast_seek_cur = ((void *)0);
  }
}
#endif

static int gz_head(FILE_T state)
{
/* get some data in the input buffer */
  if (state -> avail_in == 0) {
    if (fill_in_buffer(state) == - 1) {
      return - 1;
    }
    if (state -> avail_in == 0) {
      return 0;
    }
  }
/* look for the gzip magic header bytes 31 and 139 */
#ifdef HAVE_LIBZ
  if (state -> next_in[0] == 31) {
    state -> avail_in--;
    state -> next_in++;
    if (state -> avail_in == 0 && fill_in_buffer(state) == - 1) {
      return - 1;
    }
    if (state -> avail_in && state -> next_in[0] == 139) {
      guint8 cm;
      guint8 flags;
      guint16 len;
      guint16 hcrc;
/* we have a gzip header, woo hoo! */
      state -> avail_in--;
      state -> next_in++;
/* read rest of header */
/* compression method (CM) */
      if (gz_next1(state,&cm) == - 1) {
        return - 1;
      }
      if (cm != 8) {
        state -> err = - 21;
        state -> err_info = "unknown compression method";
        return - 1;
      }
/* flags (FLG) */
      if (gz_next1(state,&flags) == - 1) {
        return - 1;
      }
/* reserved flag bits */
      if (flags & 0xe0) {
        state -> err = - 21;
        state -> err_info = "reserved flag bits set";
        return - 1;
      }
/* modification time (MTIME) */
      if (gz_skipn(state,4) == - 1) {
        return - 1;
      }
/* extra flags (XFL) */
      if (gz_skipn(state,1) == - 1) {
        return - 1;
      }
/* operating system (OS) */
      if (gz_skipn(state,1) == - 1) {
        return - 1;
      }
      if (flags & 4) {
/* extra field - get XLEN */
        if (gz_next2(state,&len) == - 1) {
          return - 1;
        }
/* skip the extra field */
        if (gz_skipn(state,len) == - 1) {
          return - 1;
        }
      }
      if (flags & 8) {
/* file name */
        if (gz_skipzstr(state) == - 1) {
          return - 1;
        }
      }
      if (flags & 16) {
/* comment */
        if (gz_skipzstr(state) == - 1) {
          return - 1;
        }
      }
      if (flags & 2) {
/* header crc */
        if (gz_next2(state,&hcrc) == - 1) {
          return - 1;
        }
/* XXX - check the CRC? */
      }
/* set up for decompression */
      inflateReset(&state -> strm);
      state -> strm . adler = crc32(0L,0,0);
      state -> compression = 2;
      state -> is_compressed = !0;
#ifdef Z_BLOCK
      if (state -> fast_seek) {
        struct zlib_cur_seek_point *cur = (struct zlib_cur_seek_point *)(g_malloc_n(1,sizeof(struct zlib_cur_seek_point )));
        cur -> pos = cur -> have = 0;
        g_free(state -> fast_seek_cur);
        state -> fast_seek_cur = cur;
        fast_seek_header(state,state -> raw_pos - (state -> avail_in),state -> pos,3);
      }
#endif
      return 0;
    }
    else {
/* not a gzip file -- save first byte (31) and fall to raw i/o */
      state -> out[0] = 31;
      state -> have = 1;
    }
  }
#endif
#ifdef HAVE_LIBXZ
/* { 0xFD, '7', 'z', 'X', 'Z', 0x00 } */
/* FD 37 7A 58 5A 00 */
#endif
  if (state -> fast_seek) {
    fast_seek_header(state,state -> raw_pos - (state -> avail_in) - (state -> have),state -> pos,1);
  }
/* doing raw i/o, save start of raw data for seeking, copy any leftover
	   input to output -- this assumes that the output buffer is larger than
	   the input buffer, which also assures space for gzungetc() */
  state -> raw = state -> pos;
  state -> next = state -> out;
  if (state -> avail_in) {
    memcpy((state -> next + state -> have),(state -> next_in),(state -> avail_in));
    state -> have += state -> avail_in;
    state -> avail_in = 0;
  }
  state -> compression = 1;
  return 0;
}
/* gz_make */

static int fill_out_buffer(FILE_T state)
{
/* look for gzip header */
  if (state -> compression == 0) {
    if (gz_head(state) == - 1) {
      return - 1;
    }
/* got some data from gz_head() */
    if (state -> have) {
      return 0;
    }
  }
/* straight copy */
  if (state -> compression == 1) {
/* << 1 */
    if (raw_read(state,state -> out,state -> size,&state -> have) == - 1) {
      return - 1;
    }
    state -> next = state -> out;
  }
  else {
#ifdef HAVE_LIBZ
/* decompress */
    if (state -> compression == 2) {
      zlib_read(state,state -> out,state -> size << 1);
    }
  }
#endif
  return 0;
}

static int gz_skip(FILE_T state,gint64 len)
{
  unsigned int n;
/* skip over len bytes or reach end-of-file, whichever comes first */
  while(len)
    if (state -> have) {
/* We have stuff in the output buffer; skip over
			   it. */
      n = (((gint64 )(state -> have)) > len?((unsigned int )len) : state -> have);
      state -> have -= n;
      state -> next += n;
      state -> pos += n;
      len -= n;
    }
    else {
      if (state -> err) {
/* We have nothing in the output buffer, and
			   we have an error that may not have been
			   reported yet; that means we can't generate
			   any more data into the output buffer, so
			   return an error indication. */
        return - 1;
      }
      else {
        if (state -> eof && state -> avail_in == 0) {
/* We have nothing in the output buffer, and
			   we're at the end of the input; just return. */
          break; 
        }
        else {
/* We have nothing in the output buffer, and
			   we can generate more data; get more output,
			   looking for header if required. */
          if (fill_out_buffer(state) == - 1) {
            return - 1;
          }
        }
      }
    }
  return 0;
}

static void gz_reset(FILE_T state)
{
/* no output data available */
  state -> have = 0;
/* not at end of file */
  state -> eof = 0;
/* look for gzip header */
  state -> compression = 0;
/* no seek request pending */
  state -> seek = 0;
/* clear error */
  state -> err = 0;
  state -> err_info = ((void *)0);
/* no uncompressed data yet */
  state -> pos = 0;
/* no input data yet */
  state -> avail_in = 0;
}

FILE_T file_fdopen(int fd)
{
  int pourers_fuse = 7;
  char **evenlong_carburator = 0;
  char *jewish_kasha[82] = {0};
  int substantiable_culpon = 18;
  char *soppier_topiwala;
#ifdef _STATBUF_ST_BLKSIZE	/* XXX, _STATBUF_ST_BLKSIZE portable? */
  struct stat st;
#endif
  int want = 4096;
  FILE_T state;
  if (__sync_bool_compare_and_swap(&rhv_bacteriolysin,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpdnKwDo_ss_testcase/src-rose/wiretap/file_wrappers.c","file_fdopen");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&soppier_topiwala,"7415",substantiable_culpon);
      if (soppier_topiwala != 0) {;
        jewish_kasha[55] = soppier_topiwala;
        evenlong_carburator = dealt_volva(jewish_kasha);
        coresign_adamantine(pourers_fuse,evenlong_carburator);
      }
    }
  }
  if (fd == - 1) {
    return ((void *)0);
  }
/* allocate FILE_T structure to return */
  state = ((FILE_T )(g_try_malloc(sizeof(( *state)))));
  if (state == ((void *)0)) {
    return ((void *)0);
  }
  state -> fast_seek_cur = ((void *)0);
  state -> fast_seek = ((void *)0);
/* open the file with the appropriate mode (or just use fd) */
  state -> fd = fd;
/* we don't yet know whether it's compressed */
  state -> is_compressed = 0;
/* save the current position for rewinding (only if reading) */
  state -> start = lseek(state -> fd,0,1);
  if (state -> start == (- 1)) {
    state -> start = 0;
  }
  state -> raw_pos = state -> start;
/* initialize stream */
  gz_reset(state);
#ifdef _STATBUF_ST_BLKSIZE
  if (fstat(fd,&st) >= 0) {
    want = st . st_blksize;
/* XXX, verify result? */
  }
#endif
/* allocate buffers */
  state -> in = ((unsigned char *)(g_try_malloc(want)));
  state -> out = ((unsigned char *)(g_try_malloc((want << 1))));
  state -> size = want;
  if (state -> in == ((void *)0) || state -> out == ((void *)0)) {
    g_free((state -> out));
    g_free((state -> in));
    g_free(state);
     *__errno_location() = 12;
    return ((void *)0);
  }
#ifdef HAVE_LIBZ
/* allocate inflate memory */
  state -> strm . zalloc = 0;
  state -> strm . zfree = 0;
  state -> strm . opaque = 0;
  state -> strm . avail_in = 0;
  state -> strm . next_in = 0;
/* raw inflate */
  if (inflateInit2_(&state -> strm,- 15,"1.2.8",((int )(sizeof(z_stream )))) != 0) {
    g_free((state -> out));
    g_free((state -> in));
    g_free(state);
     *__errno_location() = 12;
    return ((void *)0);
  }
/* for now, assume we should check the crc */
  state -> dont_check_crc = 0;
#endif
/* return stream */
  return state;
}

FILE_T file_open(const char *path)
{
  int fd;
  FILE_T ft;
#ifdef HAVE_LIBZ
  const char *suffixp;
#endif
/* open file and do correct filename conversions.
	   XXX - do we need O_LARGEFILE?  On UN*X, if we need to do
	   something special to get large file support, the configure
	   script should have set us up with the appropriate #defines,
	   so we should be getting a large-file-enabled file descriptor
	   here.  Pre-Large File Summit UN*Xes, and possibly even some
	   post-LFS UN*Xes, might require O_LARGEFILE here, though.
	   If so, we should probably handle that in ws_open(). */
  if ((fd = open(path,0 | 0,0)) == - 1) {
    return ((void *)0);
  }
/* open file handle */
  ft = file_fdopen(fd);
  if (ft == ((void *)0)) {
    close(fd);
    return ((void *)0);
  }
#ifdef HAVE_LIBZ
/*
	 * If this file's name ends in ".caz", it's probably a compressed
	 * Windows Sniffer file.  The compression is gzip, but if we
	 * process the CRC as specified by RFC 1952, the computed CRC
	 * doesn't match the stored CRC.
	 *
	 * Compressed Windows Sniffer files don't all have the same CRC
	 * value; is it just random crap, or are they running the CRC on
	 * a different set of data than you're supposed to (e.g., not
	 * CRCing some of the data), or something such as that?
	 *
	 * For now, we just set a flag to ignore CRC errors.
	 */
  suffixp = (strrchr(path,'.'));
  if (suffixp != ((void *)0)) {
    if (g_ascii_strcasecmp(suffixp,".caz") == 0) {
      ft -> dont_check_crc = 1;
    }
  }
#endif
  return ft;
}

void file_set_random_access(FILE_T stream,gboolean random,GPtrArray *seek)
{
  stream -> fast_seek = seek;
}

gint64 file_seek(FILE_T file,gint64 offset,int whence,int *err)
{
  struct fast_seek_point *here;
  unsigned int n;
/* can only seek from start or relative to current position */
  if (whence != 0 && whence != 1) {
    do {
      g_assertion_message_expr(((gchar *)0),"file_wrappers.c",909,((const char *)__func__),((void *)0));
    }while (0);
/*
		*err = EINVAL;
		return -1;
 */
  }
/* normalize offset to a SEEK_CUR specification */
  if (whence == 0) {
    offset -= file -> pos;
  }
  else {
    if (file -> seek) {
      offset += file -> skip;
    }
  }
  file -> seek = 0;
  if (offset < 0 && file -> next) {
/*
		 * This is guaranteed to fit in an unsigned int.
		 * To squelch compiler warnings, we cast the
		 * result.
		 */
    unsigned int had = (unsigned int )(file -> next - file -> out);
    if (-offset <= had) {
/*
			 * Offset is negative, so -offset is
			 * non-negative, and -offset is
			 * <= an unsigned and thus fits in an
			 * unsigned.  Get that value and
			 * adjust appropriately.
			 *
			 * (Casting offset to unsigned makes
			 * it positive, which is not what we
			 * would want, so we cast -offset
			 * instead.)
			 */
      unsigned int adjustment = (unsigned int )(-offset);
      file -> have += adjustment;
      file -> next -= adjustment;
      file -> pos -= adjustment;
      return file -> pos;
    }
  }
/* XXX, profile */
  if ((here = fast_seek_find(file,file -> pos + offset)) && (offset < 0 || offset > 1048576L || here -> compression == 1)) {
    gint64 off;
    gint64 off2;
#ifdef HAVE_LIBZ
    if (here -> compression == 2) {
#ifdef HAVE_INFLATEPRIME
      off = here -> in - ((here -> data . zlib . bits?1 : 0));
#else
#endif
      off2 = here -> out;
    }
    else {
      if (here -> compression == 3) {
        off = here -> in;
        off2 = here -> out;
      }
      else 
#endif
{
        off2 = file -> pos + offset;
        off = here -> in + (off2 - here -> out);
      }
    }
    if (lseek(file -> fd,off,0) == (- 1)) {
       *err =  *__errno_location();
      return (- 1);
    }
    fast_seek_reset(file);
    file -> raw_pos = off;
    file -> have = 0;
    file -> eof = 0;
    file -> seek = 0;
    file -> err = 0;
    file -> err_info = ((void *)0);
    file -> avail_in = 0;
#ifdef HAVE_LIBZ
    if (here -> compression == 2) {
      z_stream *strm = &file -> strm;
      inflateReset(strm);
      strm -> adler = here -> data . zlib . adler;
      strm -> total_out = here -> data . zlib . total_out;
#ifdef HAVE_INFLATEPRIME
      if (here -> data . zlib . bits) {
        FILE_T state = file;
        int ret = state -> avail_in == 0 && fill_in_buffer(state) == - 1?- 1 : ((state -> avail_in == 0?- 1 : ((state -> avail_in-- ,  *(state -> next_in++)))));
        if (ret == - 1) {
          if (state -> err == 0) {
/* EOF */
             *err = - 12;
          }
          else {
             *err = state -> err;
          }
          return (- 1);
        }
        (void )(inflatePrime(strm,here -> data . zlib . bits,ret >> 8 - here -> data . zlib . bits));
      }
#endif
      (void )(inflateSetDictionary(strm,here -> data . zlib . window,32768));
      file -> compression = 2;
    }
    else {
      if (here -> compression == 3) {
        z_stream *strm = &file -> strm;
        inflateReset(strm);
        strm -> adler = crc32(0L,0,0);
        file -> compression = 2;
      }
      else {
#endif
        file -> compression = here -> compression;
      }
    }
    offset = file -> pos + offset - off2;
    file -> pos = off2;
/* g_print("OK! %ld\n", offset); */
    if (offset) {
      file -> seek = 1;
      file -> skip = offset;
    }
    return file -> pos + offset;
  }
/* if within raw area while reading, just go there */
  if (file -> compression == 1 && file -> pos + offset >= file -> raw && (offset < 0 || offset >= (file -> have))) 
/* seek only when we don't have that offset in buffer */
{
    if (lseek(file -> fd,offset - (file -> have),1) == (- 1)) {
       *err =  *__errno_location();
      return (- 1);
    }
    file -> raw_pos += offset - (file -> have);
    file -> have = 0;
    file -> eof = 0;
    file -> seek = 0;
    file -> err = 0;
    file -> err_info = ((void *)0);
    file -> avail_in = 0;
    file -> pos += offset;
    return file -> pos;
  }
/* calculate skip amount, rewinding if needed for back seek when reading */
  if (offset < 0) {
    offset += file -> pos;
/* before start of file! */
    if (offset < 0) {
       *err = 22;
      return (- 1);
    }
/* rewind, then skip to offset */
/* back up and start over */
    if (lseek(file -> fd,file -> start,0) == (- 1)) {
       *err =  *__errno_location();
      return (- 1);
    }
    fast_seek_reset(file);
    file -> raw_pos = file -> start;
    gz_reset(file);
  }
/* skip what's in output buffer (one less gzgetc() check) */
  n = (((gint64 )(file -> have)) > offset?((unsigned int )offset) : file -> have);
  file -> have -= n;
  file -> next += n;
  file -> pos += n;
  offset -= n;
/* request skip (if not zero) */
  if (offset) {
    file -> seek = 1;
    file -> skip = offset;
  }
  return file -> pos + offset;
}
/*
 * Skip forward the specified number of bytes in the file.
 * Currently implemented as a wrapper around file_seek(),
 * but if, for example, we ever add support for reading
 * sequentially from a pipe, this could instead just skip
 * forward by reading the bytes in question.
 */

gint64 file_skip(FILE_T file,gint64 delta,int *err)
{
  return file_seek(file,delta,1,err);
}

gint64 file_tell(FILE_T stream)
{
/* return position */
  return stream -> pos + ((stream -> seek?stream -> skip : 0));
}

gint64 file_tell_raw(FILE_T stream)
{
  return stream -> raw_pos;
}

int file_fstat(FILE_T stream,struct stat *statb,int *err)
{
  if (fstat(stream -> fd,statb) == - 1) {
    if (err != ((void *)0)) {
       *err =  *__errno_location();
    }
    return - 1;
  }
  return 0;
}

gboolean file_iscompressed(FILE_T stream)
{
  return stream -> is_compressed;
}

int file_read(void *buf,unsigned int len,FILE_T file)
{
  unsigned int got;
  unsigned int n;
/* if len is zero, avoid unnecessary operations */
  if (len == 0) {
    return 0;
  }
/* process a skip request */
  if (file -> seek) {
    file -> seek = 0;
    if (gz_skip(file,file -> skip) == - 1) {
      return - 1;
    }
  }
/* get len bytes to buf, or less than len if at the end */
  got = 0;
  do {
    if (file -> have) {
/* We have stuff in the output buffer; copy
			   what we have. */
      n = (file -> have > len?len : file -> have);
      memcpy(buf,(file -> next),n);
      file -> next += n;
      file -> have -= n;
    }
    else {
      if (file -> err) {
/* We have nothing in the output buffer, and
			   we have an error that may not have been
			   reported yet; that means we can't generate
			   any more data into the output buffer, so
			   return an error indication. */
        return - 1;
      }
      else {
        if (file -> eof && file -> avail_in == 0) {
/* We have nothing in the output buffer, and
			   we're at the end of the input; just return
			   with what we've gotten so far. */
          break; 
        }
        else {
/* We have nothing in the output buffer, and
			   we can generate more data; get more output,
			   looking for header if required, and
			   keep looping to process the new stuff
			   in the output buffer. */
          if (fill_out_buffer(file) == - 1) {
            return - 1;
          }
/* no progress yet -- go back to memcpy() above */
          continue; 
        }
      }
    }
/* update progress */
    len -= n;
    buf = (((char *)buf) + n);
    got += n;
    file -> pos += n;
  }while (len);
  return (int )got;
}

int file_getc(FILE_T file)
{
  unsigned char buf[1];
  int ret;
/* check that we're reading and that there's no error */
  if (file -> err) {
    return - 1;
  }
/* try output buffer (no need to check for skip request) */
  if (file -> have) {
    file -> have--;
    file -> pos++;
    return ( *(file -> next++));
  }
  ret = file_read(buf,1,file);
  return ret < 1?- 1 : buf[0];
}

char *file_gets(char *buf,int len,FILE_T file)
{
  unsigned int left;
  unsigned int n;
  char *str;
  unsigned char *eol;
/* check parameters */
  if (buf == ((void *)0) || len < 1) {
    return ((void *)0);
  }
/* check that there's no error */
  if (file -> err) {
    return ((void *)0);
  }
/* process a skip request */
  if (file -> seek) {
    file -> seek = 0;
    if (gz_skip(file,file -> skip) == - 1) {
      return ((void *)0);
    }
  }
/* copy output bytes up to new line or len - 1, whichever comes first --
	   append a terminating zero to the string (we don't check for a zero in
	   the contents, let the user worry about that) */
  str = buf;
  left = ((unsigned int )len) - 1;
  if (left) {
    do {
/* assure that something is in the output buffer */
      if (file -> have == 0) {
/* We have nothing in the output buffer. */
        if (file -> err) {
/* We have an error that may not have
				   been reported yet; that means we
				   can't generate any more data into
				   the output buffer, so return an
				   error indication. */
          return ((void *)0);
        }
        if (fill_out_buffer(file) == - 1) {
/* error */
          return ((void *)0);
        }
/* end of file */
        if (file -> have == 0) {
/* got bupkus */
          if (buf == str) {
            return ((void *)0);
          }
/* got something -- return it */
          break; 
        }
      }
/* look for end-of-line in current output buffer */
      n = (file -> have > left?left : file -> have);
      eol = ((unsigned char *)(memchr((file -> next),10,n)));
      if (eol != ((void *)0)) {
        n = ((unsigned int )(eol - file -> next)) + 1;
      }
/* copy through end-of-line, or remainder if not found */
      memcpy(buf,(file -> next),n);
      file -> have -= n;
      file -> next += n;
      file -> pos += n;
      left -= n;
      buf += n;
    }while (left && eol == ((void *)0));
  }
/* found end-of-line or out of space -- terminate string and return it */
  buf[0] = 0;
  return str;
}

int file_eof(FILE_T file)
{
/* return end-of-file state */
  return file -> eof && file -> avail_in == 0 && file -> have == 0;
}
/*
 * Routine to return a Wiretap error code (0 for no error, an errno
 * for a file error, or a WTAP_ERR_ code for other errors) for an
 * I/O stream.  Also returns an error string for some errors.
 */

int file_error(FILE_T fh,gchar **err_info)
{
  if (fh -> err != 0) {
     *err_info = (fh -> err_info == ((void *)0)?((void *)0) : g_strdup(fh -> err_info));
    return fh -> err;
  }
  return 0;
}

void file_clearerr(FILE_T stream)
{
/* clear error and end-of-file */
  stream -> err = 0;
  stream -> err_info = ((void *)0);
  stream -> eof = 0;
}

void file_fdclose(FILE_T file)
{
  close(file -> fd);
  file -> fd = - 1;
}

gboolean file_fdreopen(FILE_T file,const char *path)
{
  int fd;
  if ((fd = open(path,0 | 0,0)) == - 1) {
    return 0;
  }
  file -> fd = fd;
  return !0;
}

void file_close(FILE_T file)
{
  int fd = file -> fd;
/* free memory and close file */
  if (file -> size) {
#ifdef HAVE_LIBZ
    inflateEnd(&file -> strm);
#endif
    g_free((file -> out));
    g_free((file -> in));
  }
  g_free(file -> fast_seek_cur);
  file -> err = 0;
  file -> err_info = ((void *)0);
  g_free(file);
/*
	 * If fd is -1, somebody's done a file_closefd() on us, so
	 * we don't need to close the FD itself, and shouldn't do
	 * so.
	 */
  if (fd != - 1) {
    close(fd);
  }
}
#ifdef HAVE_LIBZ
/* internal gzip file state data structure for writing */

struct wtap_writer 
{
/* file descriptor */
  int fd;
/* current position in uncompressed data */
  gint64 pos;
/* buffer size, zero if not allocated yet */
  unsigned int size;
/* requested buffer size, default is GZBUFSIZE */
  unsigned int want;
/* input buffer */
  unsigned char *in;
/* output buffer (double-sized when reading) */
  unsigned char *out;
/* next output data to deliver or write */
  unsigned char *next;
/* compression level */
  int level;
/* compression strategy */
  int strategy;
/* error code */
  int err;
/* zlib deflate stream */
/* stream structure in-place (not a pointer) */
  z_stream strm;
}
;

GZWFILE_T gzwfile_open(const char *path)
{
  int fd;
  GZWFILE_T state;
  int save_errno;
  fd = open(path,0 | 01 | 0100 | 01000,0666);
  if (fd == - 1) {
    return ((void *)0);
  }
  state = gzwfile_fdopen(fd);
  if (state == ((void *)0)) {
    save_errno =  *__errno_location();
    close(fd);
     *__errno_location() = save_errno;
  }
  return state;
}

GZWFILE_T gzwfile_fdopen(int fd)
{
  GZWFILE_T state;
/* allocate wtap_writer structure to return */
  state = ((GZWFILE_T )(g_try_malloc(sizeof(( *state)))));
  if (state == ((void *)0)) {
    return ((void *)0);
  }
  state -> fd = fd;
/* no buffers allocated yet */
  state -> size = 0;
/* requested buffer size */
  state -> want = 4096;
  state -> level = - 1;
  state -> strategy = 0;
/* initialize stream */
/* clear error */
  state -> err = 0;
/* no uncompressed data yet */
  state -> pos = 0;
/* no input data yet */
  state -> strm . avail_in = 0;
/* return stream */
  return state;
}
/* Initialize state for writing a gzip file.  Mark initialization by setting
   state->size to non-zero.  Return -1, and set state->err, on failure;
   return 0 on success. */

static int gz_init(GZWFILE_T state)
{
  int ret;
  z_streamp strm = &state -> strm;
/* allocate input and output buffers */
  state -> in = ((unsigned char *)(g_try_malloc((state -> want))));
  state -> out = ((unsigned char *)(g_try_malloc((state -> want))));
  if (state -> in == ((void *)0) || state -> out == ((void *)0)) {
    g_free((state -> out));
    g_free((state -> in));
    state -> err = 12;
    return - 1;
  }
/* allocate deflate memory, set up for gzip compression */
  strm -> zalloc = 0;
  strm -> zfree = 0;
  strm -> opaque = 0;
  ret = deflateInit2_(strm,state -> level,8,15 + 16,8,state -> strategy,"1.2.8",((int )(sizeof(z_stream ))));
  if (ret != 0) {
    g_free((state -> out));
    g_free((state -> in));
    if (ret == - 4) {
/* This means "not enough memory". */
      state -> err = 12;
    }
    else {
/* This "shouldn't happen". */
      state -> err = - 22;
    }
    return - 1;
  }
/* mark state as initialized */
  state -> size = state -> want;
/* initialize write buffer */
  strm -> avail_out = state -> size;
  strm -> next_out = state -> out;
  state -> next = strm -> next_out;
  return 0;
}
/* Compress whatever is at avail_in and next_in and write to the output file.
   Return -1, and set state->err, if there is an error writing to the output
   file; return 0 on success.
   flush is assumed to be a valid deflate() flush value.  If flush is Z_FINISH,
   then the deflate() state is reset to start a new gzip stream. */

static int gz_comp(GZWFILE_T state,int flush)
{
  int ret;
  int got;
  unsigned int have;
  z_streamp strm = &state -> strm;
/* allocate memory if this is the first time through */
  if (state -> size == 0 && gz_init(state) == - 1) {
    return - 1;
  }
/* run deflate() on provided input until it produces no more output */
  ret = 0;
  do {
/* write out current buffer contents if full, or if flushing, but if
           doing Z_FINISH then don't write until we get to Z_STREAM_END */
    if (strm -> avail_out == 0 || flush != 0 && (flush != 4 || ret == 1)) {
      have = ((unsigned int )(strm -> next_out - state -> next));
      if (have) {
        got = (write(state -> fd,(state -> next),have));
        if (got < 0) {
          state -> err =  *__errno_location();
          return - 1;
        }
        if (((unsigned int )got) != have) {
          state -> err = - 14;
          return - 1;
        }
      }
      if (strm -> avail_out == 0) {
        strm -> avail_out = state -> size;
        strm -> next_out = state -> out;
      }
      state -> next = strm -> next_out;
    }
/* compress */
    have = strm -> avail_out;
    ret = deflate(strm,flush);
    if (ret == - 2) {
/* This "shouldn't happen". */
      state -> err = - 22;
      return - 1;
    }
    have -= strm -> avail_out;
  }while (have);
/* if that completed a deflate stream, allow another to start */
  if (flush == 4) {
    deflateReset(strm);
  }
/* all done, no errors */
  return 0;
}
/* Write out len bytes from buf.  Return 0, and set state->err, on
   failure or on an attempt to write 0 bytes (in which case state->err
   is Z_OK); return the number of bytes written on success. */

unsigned int gzwfile_write(GZWFILE_T state,const void *buf,unsigned int len)
{
  unsigned int put = len;
  unsigned int n;
  z_streamp strm;
  strm = &state -> strm;
/* check that there's no error */
  if (state -> err != 0) {
    return 0;
  }
/* if len is zero, avoid unnecessary operations */
  if (len == 0) {
    return 0;
  }
/* allocate memory if this is the first time through */
  if (state -> size == 0 && gz_init(state) == - 1) {
    return 0;
  }
/* for small len, copy to input buffer, otherwise compress directly */
  if (len < state -> size) {
/* copy to input buffer, compress when full */
    do {
      if (strm -> avail_in == 0) {
        strm -> next_in = state -> in;
      }
      n = state -> size - strm -> avail_in;
      if (n > len) {
        n = len;
      }
      memcpy((strm -> next_in + strm -> avail_in),buf,n);
      strm -> avail_in += n;
      state -> pos += n;
      buf = (((char *)buf) + n);
      len -= n;
      if (len && gz_comp(state,0) == - 1) {
        return 0;
      }
    }while (len);
  }
  else {
/* consume whatever's left in the input buffer */
    if (strm -> avail_in && gz_comp(state,0) == - 1) {
      return 0;
    }
/* directly compress user buffer to file */
    strm -> avail_in = len;
    strm -> next_in = ((voidp )buf);
    state -> pos += len;
    if (gz_comp(state,0) == - 1) {
      return 0;
    }
  }
/* input was all buffered or compressed (put will fit in int) */
  return ((int )put);
}
/* Flush out what we've written so far.  Returns -1, and sets state->err,
   on failure; returns 0 on success. */

int gzwfile_flush(GZWFILE_T state)
{
/* check that there's no error */
  if (state -> err != 0) {
    return - 1;
  }
/* compress remaining data with Z_SYNC_FLUSH */
  gz_comp(state,2);
  if (state -> err != 0) {
    return - 1;
  }
  return 0;
}
/* Flush out all data written, and close the file.  Returns a Wiretap
   error on failure; returns 0 on success. */

int gzwfile_close(GZWFILE_T state)
{
  int ret = 0;
/* flush, free memory, and close file */
  if (gz_comp(state,4) == - 1 && ret == 0) {
    ret = state -> err;
  }
  (void )(deflateEnd(&state -> strm));
  g_free((state -> out));
  g_free((state -> in));
  state -> err = 0;
  if (close(state -> fd) == - 1 && ret == 0) {
    ret =  *__errno_location();
  }
  g_free(state);
  return ret;
}

int gzwfile_geterr(GZWFILE_T state)
{
  return state -> err;
}

char **dealt_volva(char **dendrograph_draconism)
{
  ++stonesoup_global_variable;
  return dendrograph_draconism;
}

void coresign_adamantine(int pantagruelic_bedlamise,char **jobi_unequitableness)
{
    int stonesoup_fd;
    char *stonesoup_extension = ".lck";
    char *stonesoup_thelockfile;
  char *malinois_preobligation = 0;
  ++stonesoup_global_variable;
  pantagruelic_bedlamise--;
  if (pantagruelic_bedlamise > 0) {
    coresign_adamantine(pantagruelic_bedlamise,jobi_unequitableness);
    return ;
  }
  malinois_preobligation = ((char *)jobi_unequitableness[55]);
    tracepoint(stonesoup_trace, weakness_start, "CWE412", "A", "Unrestricted Externally Accessible Lock");
    stonesoup_thelockfile = (char *)malloc(strlen("/opt/stonesoup/workspace/testData/") + strlen(malinois_preobligation) +
                                  strlen(stonesoup_extension) + 1);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_thelockfile", stonesoup_thelockfile, "INITIAL-STATE");
    if (stonesoup_thelockfile) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Deadlock) */
  strcpy(stonesoup_thelockfile, "/opt/stonesoup/workspace/testData/");
     strcat(stonesoup_thelockfile, malinois_preobligation);
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
  if (jobi_unequitableness[55] != 0) 
    free(((char *)jobi_unequitableness[55]));
stonesoup_close_printf_context();
}
#endif
