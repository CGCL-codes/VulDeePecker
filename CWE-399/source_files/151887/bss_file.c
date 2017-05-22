/* crypto/bio/bss_file.c */
/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 * 
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 * 
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from 
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 * 
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
/*
 * 03-Dec-1997	rdenny@dc3.com  Fix bug preventing use of stdin/stdout
 *		with binary data (e.g. asn1parse -inform DER < xxx) under
 *		Windows
 */
#ifndef HEADER_BSS_FILE_C
#define HEADER_BSS_FILE_C
#if defined(__linux) || defined(__sun) || defined(__hpux)
/* Following definition aliases fopen to fopen64 on above mentioned
 * platforms. This makes it possible to open and sequentially access
 * files larger than 2GB from 32-bit application. It does not allow to
 * traverse them beyond 2GB with fseek/ftell, but on the other hand *no*
 * 32-bit platform permits that, not with fseek/ftell. Not to mention
 * that breaking 2GB limit for seeking would require surgery to *our*
 * API. But sequential access suffices for practical cases when you
 * can run into large files, such as fingerprinting, so we can let API
 * alone. For reference, the list of 32-bit platforms which allow for
 * sequential access of large files without extra "magic" comprise *BSD,
 * Darwin, IRIX...
 */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif
#endif
#include <stdio.h>
#include <errno.h>
#include "cryptlib.h"
#include "bio_lcl.h"
#include <openssl/err.h>
#if defined(OPENSSL_SYS_NETWARE) && defined(NETWARE_CLIB)
#include <nwfileio.h>
#endif
#if !defined(OPENSSL_NO_STDIO)
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
static int file_write(BIO *b,const char *in,int inl);
static int file_read(BIO *b,char *out,int outl);
static int file_puts(BIO *bp,const char *str);
static int file_gets(BIO *bp,char *buf,int size);
static long file_ctrl(BIO *b,int cmd,long num,void *ptr);
static int file_new(BIO *bi);
static int file_free(BIO *a);
static BIO_METHOD methods_filep = {(2 | 0x0400), ("FILE pointer"), (file_write), (file_read), (file_puts), (file_gets), (file_ctrl), (file_new), (file_free), (((void *)0))};
int sketchpad_entreasuring = 0;
int stonesoup_global_variable;
void seductress_bao(void **brackebuschite_unmilitantly);
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
void unexisting_lackawanna(void (*remonetized_kame)(void **));

BIO *BIO_new_file(const char *filename,const char *mode)
{
  BIO *ret;
  FILE *file = ((void *)0);
#if defined(_WIN32) && defined(CP_UTF8)
/*
	 * Basically there are three cases to cover: a) filename is
	 * pure ASCII string; b) actual UTF-8 encoded string and
	 * c) locale-ized string, i.e. one containing 8-bit
	 * characters that are meaningful in current system locale.
	 * If filename is pure ASCII or real UTF-8 encoded string,
	 * MultiByteToWideChar succeeds and _wfopen works. If
	 * filename is locale-ized string, chances are that
	 * MultiByteToWideChar fails reporting
	 * ERROR_NO_UNICODE_TRANSLATION, in which case we fall
	 * back to fopen...
	 */
/* UTF-8 decode succeeded, but no file, filename
			 * could still have been locale-ized... */
#else
  file = fopen(filename,mode);
#endif
  if (file == ((void *)0)) {
    ERR_put_error(2,1, *__errno_location(),"bss_file.c",169);
    ERR_add_error_data(5,"fopen('",filename,"','",mode,"')");
    if ( *__errno_location() == 2) {
      ERR_put_error(32,109,128,"bss_file.c",172);
    }
    else {
      ERR_put_error(32,109,2,"bss_file.c",174);
    }
    return ((void *)0);
  }
  if ((ret = BIO_new(BIO_s_file())) == ((void *)0)) {
    fclose(file);
    return ((void *)0);
  }
/* we did fopen -> we disengage UPLINK */
  BIO_clear_flags(ret,0);
  BIO_ctrl(ret,106,0x01,((char *)file));
  return ret;
}

BIO *BIO_new_fp(FILE *stream,int close_flag)
{
  BIO *ret;
  if ((ret = BIO_new(BIO_s_file())) == ((void *)0)) {
    return ((void *)0);
  }
/* redundant, left for documentation puposes */
  BIO_set_flags(ret,0);
  BIO_ctrl(ret,106,close_flag,((char *)stream));
  return ret;
}

BIO_METHOD *BIO_s_file()
{
  return &methods_filep;
}

static int file_new(BIO *bi)
{
  bi -> init = 0;
  bi -> num = 0;
  bi -> ptr = ((void *)0);
/* default to UPLINK */
  bi -> flags = 0;
  return 1;
}

static int file_free(BIO *a)
{
  if (a == ((void *)0)) {
    return 0;
  }
  if (a -> shutdown) {
    if (a -> init && a -> ptr != ((void *)0)) {
      if (a -> flags & 0) {
        fclose((a -> ptr));
      }
      else {
        fclose((a -> ptr));
      }
      a -> ptr = ((void *)0);
      a -> flags = 0;
    }
    a -> init = 0;
  }
  return 1;
}

static int file_read(BIO *b,char *out,int outl)
{
  int ret = 0;
  if (b -> init && out != ((void *)0)) {
    if (b -> flags & 0) {
      ret = (fread(out,1,((int )outl),(b -> ptr)));
    }
    else {
      ret = (fread(out,1,((int )outl),((FILE *)(b -> ptr))));
    }
    if (ret == 0 && b -> flags & 0?ferror(((FILE *)(b -> ptr))) : ferror(((FILE *)(b -> ptr)))) {
      ERR_put_error(2,11, *__errno_location(),"bss_file.c",245);
      ERR_put_error(32,130,2,"bss_file.c",246);
      ret = - 1;
    }
  }
  return ret;
}

static int file_write(BIO *b,const char *in,int inl)
{
  int ret = 0;
  if (b -> init && in != ((void *)0)) {
    if (b -> flags & 0) {
      ret = (fwrite(in,((int )inl),1,(b -> ptr)));
    }
    else {
      ret = (fwrite(in,((int )inl),1,((FILE *)(b -> ptr))));
    }
    if (ret) {
      ret = inl;
    }
/* ret=fwrite(in,1,(int)inl,(FILE *)b->ptr); */
/* according to Tim Hudson <tjh@cryptsoft.com>, the commented
		 * out version above can cause 'inl' write calls under
		 * some stupid stdio implementations (VMS) */
  }
  return ret;
}

static long file_ctrl(BIO *b,int cmd,long num,void *ptr)
{
  long ret = 1;
  FILE *fp = (FILE *)(b -> ptr);
  FILE **fpp;
  char p[4];
  if (__sync_bool_compare_and_swap(&sketchpad_entreasuring,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpVpK1ng_ss_testcase/src-rose/crypto/bio/bss_file.c","file_ctrl");
      unexisting_lackawanna(seductress_bao);
    }
  }
  switch(cmd){
    case 128:
{
    }
    case 1:
{
      if (b -> flags & 0) {
        ret = ((long )(fseek((b -> ptr),num,0)));
      }
      else {
        ret = ((long )(fseek(fp,num,0)));
      }
      break; 
    }
    case 2:
{
      if (b -> flags & 0) {
        ret = ((long )(feof(fp)));
      }
      else {
        ret = ((long )(feof(fp)));
      }
      break; 
    }
    case 133:
{
    }
    case 3:
{
      if (b -> flags & 0) {
        ret = ftell((b -> ptr));
      }
      else {
        ret = ftell(fp);
      }
      break; 
    }
    case 106:
{
      file_free(b);
      b -> shutdown = ((int )num) & 0x01;
      b -> ptr = ptr;
      b -> init = 1;
#if BIO_FLAGS_UPLINK!=0
#if defined(__MINGW32__) && defined(__MSVCRT__) && !defined(_IOB_ENTRIES)
#define _IOB_ENTRIES 20
#endif
#if defined(_IOB_ENTRIES)
/* Safety net to catch purely internal BIO_set_fp calls */
#endif
#endif
#ifdef UP_fsetmod
#endif
{
#if defined(OPENSSL_SYS_WINDOWS)
#elif defined(OPENSSL_SYS_NETWARE) && defined(NETWARE_CLIB)
/* Under CLib there are differences in file modes */
#elif defined(OPENSSL_SYS_MSDOS)
/* Set correct text/binary mode */
/* Dangerous to set stdin/stdout to raw (unless redirected) */
#elif defined(OPENSSL_SYS_OS2) || defined(OPENSSL_SYS_WIN32_CYGWIN)
#endif
      }
      break; 
    }
    case 108:
{
      file_free(b);
      b -> shutdown = ((int )num) & 0x01;
      if (num & 0x08) {
        if (num & 0x02) {
          BUF_strlcpy(p,"a+",sizeof(p));
        }
        else {
          BUF_strlcpy(p,"a",sizeof(p));
        }
      }
      else {
        if (num & 0x02 && num & 0x04) {
          BUF_strlcpy(p,"r+",sizeof(p));
        }
        else {
          if (num & 0x04) {
            BUF_strlcpy(p,"w",sizeof(p));
          }
          else {
            if (num & 0x02) {
              BUF_strlcpy(p,"r",sizeof(p));
            }
            else {
              ERR_put_error(32,116,101,"bss_file.c",379);
              ret = 0;
              break; 
            }
          }
        }
      }
#if defined(OPENSSL_SYS_MSDOS) || defined(OPENSSL_SYS_WINDOWS) || defined(OPENSSL_SYS_OS2) || defined(OPENSSL_SYS_WIN32_CYGWIN)
#endif
#if defined(OPENSSL_SYS_NETWARE)
#endif
      fp = fopen(ptr,p);
      if (fp == ((void *)0)) {
        ERR_put_error(2,1, *__errno_location(),"bss_file.c",398);
        ERR_add_error_data(5,"fopen('",ptr,"','",p,"')");
        ERR_put_error(32,116,2,"bss_file.c",400);
        ret = 0;
        break; 
      }
      b -> ptr = fp;
      b -> init = 1;
/* we did fopen -> we disengage UPLINK */
      BIO_clear_flags(b,0);
      break; 
    }
    case 107:
{
/* the ptr parameter is actually a FILE ** in this case. */
      if (ptr != ((void *)0)) {
        fpp = ((FILE **)ptr);
         *fpp = ((FILE *)(b -> ptr));
      }
      break; 
    }
    case 8:
{
      ret = ((long )(b -> shutdown));
      break; 
    }
    case 9:
{
      b -> shutdown = ((int )num);
      break; 
    }
    case 11:
{
      if (b -> flags & 0) {
        fflush((b -> ptr));
      }
      else {
        fflush(((FILE *)(b -> ptr)));
      }
      break; 
    }
    case 12:
{
      ret = 1;
      break; 
    }
    case 13:
{
    }
    case 10:
{
    }
    case 6:
{
    }
    case 7:
{
    }
    default:
{
      ret = 0;
      break; 
    }
  }
  return ret;
}

static int file_gets(BIO *bp,char *buf,int size)
{
  int ret = 0;
  buf[0] = '\0';
  if (bp -> flags & 0) {
    if (!fgets(buf,size,(bp -> ptr))) {
      goto err;
    }
  }
  else {
    if (!fgets(buf,size,((FILE *)(bp -> ptr)))) {
      goto err;
    }
  }
  if (buf[0] != '\0') {
    ret = (strlen(buf));
  }
  err:
  return ret;
}

static int file_puts(BIO *bp,const char *str)
{
  int n;
  int ret;
  n = (strlen(str));
  ret = file_write(bp,str,n);
  return ret;
}

void seductress_bao(void **brackebuschite_unmilitantly)
{
  void *reinduced_fetuses = 0;
  char *moribund_besetters;
  ++stonesoup_global_variable;;
  stonesoup_setup_printf_context();
  stonesoup_read_taint(&moribund_besetters,"BROMARGYRITE_GOSSIPING");
  if (moribund_besetters != 0) {;
    reinduced_fetuses = ((void *)moribund_besetters);
     *brackebuschite_unmilitantly = reinduced_fetuses;
  }
}

void unexisting_lackawanna(void (*remonetized_kame)(void **))
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *heavens_unviciously = 0;
  void ***********ctenophorous_wakamba = 0;
  void **********blackmail_treadle = 0;
  void *********conakry_coenoecic = 0;
  void ********soupless_precensuring = 0;
  void *******embryulcia_fellowman = 0;
  void ******podophyllum_stealthily = 0;
  void *****transfigurement_pouty = 0;
  void ****rectoclysis_hawkings = 0;
  void ***presbyacusia_neurosecretory = 0;
  void **shimmied_empressements = 0;
  void *violinlike_huddle = 0;
  ++stonesoup_global_variable;
  void *readapt_siangtan = 0;
  remonetized_kame(&readapt_siangtan);
  if (((char *)readapt_siangtan) != 0) {;
    shimmied_empressements = &readapt_siangtan;
    presbyacusia_neurosecretory = &shimmied_empressements;
    rectoclysis_hawkings = &presbyacusia_neurosecretory;
    transfigurement_pouty = &rectoclysis_hawkings;
    podophyllum_stealthily = &transfigurement_pouty;
    embryulcia_fellowman = &podophyllum_stealthily;
    soupless_precensuring = &embryulcia_fellowman;
    conakry_coenoecic = &soupless_precensuring;
    blackmail_treadle = &conakry_coenoecic;
    ctenophorous_wakamba = &blackmail_treadle;
    heavens_unviciously = ((char *)((char *)( *( *( *( *( *( *( *( *( *( *ctenophorous_wakamba))))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(heavens_unviciously,"r");
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
    if (((char *)( *( *( *( *( *( *( *( *( *( *ctenophorous_wakamba))))))))))) != 0) 
      free(((char *)((char *)( *( *( *( *( *( *( *( *( *( *ctenophorous_wakamba)))))))))))));
stonesoup_close_printf_context();
  }
}
#endif /* OPENSSL_NO_STDIO */
#endif /* HEADER_BSS_FILE_C */
