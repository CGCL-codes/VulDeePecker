/* column-utils.c
 * Routines for column utilities.
 *
 * $Id: column-utils.c 49778 2013-06-04 21:43:49Z sake $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <string.h>
#include <time.h>
#include "column-utils.h"
#include "timestamp.h"
#include "sna-utils.h"
#include "atalk-utils.h"
#include "to_str.h"
#include "packet_info.h"
#include "pint.h"
#include "addr_resolv.h"
#include "ipv6-utils.h"
#include "osi-utils.h"
#include "value_string.h"
#include "column_info.h"
#include <epan/strutil.h>
#include <epan/epan.h>
/* Allocate all the data structures for constructing column data, given
   the number of columns. */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <fcntl.h> 
#include <unistd.h> 
int carbonnieux_mccafferty = 0;
typedef char *pacane_charonian;
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

void col_setup(column_info *cinfo,const gint num_cols)
{
  int i;
  cinfo -> num_cols = num_cols;
  cinfo -> col_fmt = ((gint *)(g_malloc_n(num_cols,sizeof(gint ))));
  cinfo -> fmt_matx = ((gboolean **)(g_malloc_n(num_cols,sizeof(gboolean *))));
  cinfo -> col_first = ((int *)(g_malloc_n(NUM_COL_FMTS,sizeof(int ))));
  cinfo -> col_last = ((int *)(g_malloc_n(NUM_COL_FMTS,sizeof(int ))));
  cinfo -> col_title = ((gchar **)(g_malloc_n(num_cols,sizeof(gchar *))));
  cinfo -> col_custom_field = ((gchar **)(g_malloc_n(num_cols,sizeof(gchar *))));
  cinfo -> col_custom_occurrence = ((gint *)(g_malloc_n(num_cols,sizeof(gint ))));
  cinfo -> col_custom_field_id = ((int *)(g_malloc_n(num_cols,sizeof(int ))));
  cinfo -> col_custom_dfilter = ((dfilter_t **)(g_malloc_n(num_cols,sizeof(dfilter_t *))));
  cinfo -> col_data = ((const gchar **)((gchar **)(g_malloc_n(num_cols,sizeof(gchar *)))));
  cinfo -> col_buf = ((gchar **)(g_malloc_n(num_cols,sizeof(gchar *))));
  cinfo -> col_fence = ((int *)(g_malloc_n(num_cols,sizeof(int ))));
  cinfo -> col_expr . col_expr = ((const gchar **)((gchar **)(g_malloc_n((num_cols + 1),sizeof(gchar *)))));
  cinfo -> col_expr . col_expr_val = ((gchar **)(g_malloc_n((num_cols + 1),sizeof(gchar *))));
  for (i = 0; i < NUM_COL_FMTS; i++) {
    cinfo -> col_first[i] = - 1;
    cinfo -> col_last[i] = - 1;
  }
}
/* Initialize the data structures for constructing column data. */

void col_init(column_info *cinfo)
{
  int i;
  if (!cinfo) {
    return ;
  }
  for (i = 0; i < cinfo -> num_cols; i++) {
    cinfo -> col_buf[i][0] = '\0';
    cinfo -> col_data[i] = cinfo -> col_buf[i];
    cinfo -> col_fence[i] = 0;
    cinfo -> col_expr . col_expr[i] = "";
    cinfo -> col_expr . col_expr_val[i][0] = '\0';
  }
  cinfo -> writable = !0;
}
#define COL_GET_WRITABLE(cinfo) (cinfo ? cinfo->writable : FALSE)

gboolean col_get_writable(column_info *cinfo)
{
  return cinfo?cinfo -> writable : 0;
}

void col_set_writable(column_info *cinfo,const gboolean writable)
{
  if (cinfo) {
    cinfo -> writable = writable;
  }
}
/* Checks to see if a particular packet information element is needed for the packet list */
#define CHECK_COL(cinfo, el) \
    /* We are constructing columns, and they're writable */ \
    (COL_GET_WRITABLE(cinfo) && \
      /* There is at least one column in that format */ \
    ((cinfo)->col_first[el] >= 0))

gint check_col(column_info *cinfo,const gint el)
{
  return ((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0;
}
/* Sets the fence for a column to be at the end of the column. */

void col_set_fence(column_info *cinfo,const gint el)
{
  int i;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      cinfo -> col_fence[i] = ((int )(strlen(cinfo -> col_data[i])));
    }
  }
}
/* Use this to clear out a column, especially if you're going to be
   appending to it later; at least on some platforms, it's more
   efficient than using "col_add_str()" with a null string, and
   more efficient than "col_set_str()" with a null string if you
   later append to it, as the later append will cause a string
   copy to be done. */

void col_clear(column_info *cinfo,const gint el)
{
  int i;
  int fence;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
/*
       * At this point, either
       *
       *   1) col_data[i] is equal to col_buf[i], in which case we
       *      don't have to worry about copying col_data[i] to
       *      col_buf[i];
       *
       *   2) col_data[i] isn't equal to col_buf[i], in which case
       *      the only thing that's been done to the column is
       *      "col_set_str()" calls and possibly "col_set_fence()"
       *      calls, in which case the fence is either unset and
       *      at the beginning of the string or set and at the end
       *      of the string - if it's at the beginning, we're just
       *      going to clear the column, and if it's at the end,
       *      we don't do anything.
       */
      fence = cinfo -> col_fence[i];
      if (cinfo -> col_buf[i] == cinfo -> col_data[i] || fence == 0) {
/*
         * The fence isn't at the end of the column, or the column wasn't
         * last set with "col_set_str()", so clear the column out.
         */
        cinfo -> col_buf[i][fence] = '\0';
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      cinfo -> col_expr . col_expr[i] = "";
      cinfo -> col_expr . col_expr_val[i][0] = '\0';
    }
  }
}
#define COL_CHECK_APPEND(cinfo, i, max_len) \
  if (cinfo->col_data[i] != cinfo->col_buf[i]) {        \
    /* This was set with "col_set_str()"; copy the string they  \
       set it to into the buffer, so we can append to it. */    \
    g_strlcpy(cinfo->col_buf[i], cinfo->col_data[i], max_len);  \
    cinfo->col_data[i] = cinfo->col_buf[i];         \
  }
#define COL_CHECK_REF_TIME(fd, buf)         \
  if(fd->flags.ref_time){                   \
    g_strlcpy(buf, "*REF*", COL_MAX_LEN );  \
    return;                                 \
  }
/* The same as CHECK_COL(), but without the check to see if the column is writable. */
#define HAVE_CUSTOM_COLS(cinfo) ((cinfo) && (cinfo)->col_first[COL_CUSTOM] >= 0)

gboolean have_custom_cols(column_info *cinfo)
{
  return cinfo && cinfo -> col_first[COL_CUSTOM] >= 0;
}
/* search in edt tree custom fields */

void col_custom_set_edt(epan_dissect_t *edt,column_info *cinfo)
{
  int i;
  if (!(cinfo && cinfo -> col_first[COL_CUSTOM] >= 0)) {
    return ;
  }
  for (i = cinfo -> col_first[COL_CUSTOM]; i <= cinfo -> col_last[COL_CUSTOM]; i++) {
    if (cinfo -> fmt_matx[i][COL_CUSTOM] && cinfo -> col_custom_field[i] && cinfo -> col_custom_field_id[i] != - 1) {
      cinfo -> col_data[i] = cinfo -> col_buf[i];
      cinfo -> col_expr . col_expr[i] = epan_custom_set(edt,cinfo -> col_custom_field_id[i],cinfo -> col_custom_occurrence[i],cinfo -> col_buf[i],cinfo -> col_expr . col_expr_val[i],256);
    }
  }
}

void col_custom_prime_edt(epan_dissect_t *edt,column_info *cinfo)
{
  int i;
  if (!(cinfo && cinfo -> col_first[COL_CUSTOM] >= 0)) {
    return ;
  }
  for (i = cinfo -> col_first[COL_CUSTOM]; i <= cinfo -> col_last[COL_CUSTOM]; i++) {
    cinfo -> col_custom_field_id[i] = - 1;
    if (cinfo -> fmt_matx[i][COL_CUSTOM] && cinfo -> col_custom_dfilter[i]) {
      epan_dissect_prime_dfilter(edt,cinfo -> col_custom_dfilter[i]);
      if (cinfo -> col_custom_field) {
        header_field_info *hfinfo = proto_registrar_get_byname(cinfo -> col_custom_field[i]);
        cinfo -> col_custom_field_id[i] = (hfinfo?hfinfo -> id : - 1);
      }
    }
  }
}
/*  Appends a vararg list to a packet info string.
 *  This function's code is duplicated in col_append_sep_fstr() below because
 *  the for() loop below requires us to call va_start/va_end so intermediate
 *  functions are a problem.
 */

void col_append_fstr(column_info *cinfo,const gint el,const gchar *format,... )
{
  int i;
  int len;
  int max_len;
  va_list ap;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
/*
       * First arrange that we can append, if necessary.
       */
      if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
        g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      ;
      len = ((int )(strlen(cinfo -> col_buf[i])));
      __builtin_va_start(ap,format);
      g_vsnprintf(&cinfo -> col_buf[i][len],(max_len - len),format,ap);
      __builtin_va_end(ap);
    }
  }
}
/*  Appends a vararg list to a packet info string.
 *  Prefixes it with the given separator if the column is not empty.
 *  Code is duplicated from col_append_fstr above().
 */

void col_append_sep_fstr(column_info *cinfo,const gint el,const gchar *separator,const gchar *format,... )
{
  int i;
  int len;
  int max_len;
  int sep_len;
  va_list ap;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (separator == ((void *)0)) {
/* default */
    separator = ", ";
  }
  sep_len = ((int )(strlen(separator)));
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
/*
       * First arrange that we can append, if necessary.
       */
      if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
        g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      ;
      len = ((int )(strlen(cinfo -> col_buf[i])));
/*
       * If we have a separator, append it if the column isn't empty.
       */
      if (sep_len != 0) {
        if (len != 0) {
          g_strlcat(cinfo -> col_buf[i],separator,max_len);
          len += sep_len;
        }
      }
      __builtin_va_start(ap,format);
      g_vsnprintf(&cinfo -> col_buf[i][len],(max_len - len),format,ap);
      __builtin_va_end(ap);
    }
  }
}
/* Prepends a vararg list to a packet info string. */
#define COL_BUF_MAX_LEN (((COL_MAX_INFO_LEN) > (COL_MAX_LEN)) ? \
    (COL_MAX_INFO_LEN) : (COL_MAX_LEN))

void col_prepend_fstr(column_info *cinfo,const gint el,const gchar *format,... )
{
  va_list ap;
  int i;
  char orig_buf[1?4096 : 256];
  const char *orig;
  int max_len;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
/* This was set with "col_set_str()"; which is effectively const */
        orig = cinfo -> col_data[i];
      }
      else {
        g_strlcpy(orig_buf,cinfo -> col_buf[i],max_len);
        orig = orig_buf;
      }
      __builtin_va_start(ap,format);
      g_vsnprintf(cinfo -> col_buf[i],max_len,format,ap);
      __builtin_va_end(ap);
/*
       * Move the fence, unless it's at the beginning of the string.
       */
      if (cinfo -> col_fence[i] > 0) {
        cinfo -> col_fence[i] += ((int )(strlen(cinfo -> col_buf[i])));
      }
      g_strlcat(cinfo -> col_buf[i],orig,max_len);
      cinfo -> col_data[i] = cinfo -> col_buf[i];
    }
  }
}

void col_prepend_fence_fstr(column_info *cinfo,const gint el,const gchar *format,... )
{
  va_list ap;
  int i;
  char orig_buf[1?4096 : 256];
  const char *orig;
  int max_len;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
/* This was set with "col_set_str()"; which is effectively const */
        orig = cinfo -> col_data[i];
      }
      else {
        g_strlcpy(orig_buf,cinfo -> col_buf[i],max_len);
        orig = orig_buf;
      }
      __builtin_va_start(ap,format);
      g_vsnprintf(cinfo -> col_buf[i],max_len,format,ap);
      __builtin_va_end(ap);
/*
       * Move the fence if it exists, else create a new fence at the
       * end of the prepended data.
       */
      if (cinfo -> col_fence[i] > 0) {
        cinfo -> col_fence[i] += ((int )(strlen(cinfo -> col_buf[i])));
      }
      else {
        cinfo -> col_fence[i] = ((int )(strlen(cinfo -> col_buf[i])));
      }
      g_strlcat(cinfo -> col_buf[i],orig,max_len);
      cinfo -> col_data[i] = cinfo -> col_buf[i];
    }
  }
}
/* Use this if "str" points to something that won't stay around (and
   must thus be copied). */

void col_add_str(column_info *cinfo,const gint el,const gchar *str)
{
  int i;
  int fence;
  size_t max_len;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      fence = cinfo -> col_fence[i];
      if (fence != 0) {
/*
         * We will append the string after the fence.
         * First arrange that we can append, if necessary.
         */
        if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
          g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
          cinfo -> col_data[i] = cinfo -> col_buf[i];
        }
        ;
      }
      else {
/*
         * There's no fence, so we can just write to the string.
         */
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      g_strlcpy(&cinfo -> col_buf[i][fence],str,max_len - fence);
    }
  }
}
/* Use this if "str" points to something that will stay around (and thus
   needn't be copied). */

void col_set_str(column_info *cinfo,const gint el,const gchar *str)
{
  int i;
  int fence;
  size_t max_len;
  (void )(str?((void )0) : ((getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"%s\"","column-utils.c",479,"str"))))));
/* The caller is expected to pass in something that 'will stay around' and
   * something from the ephemeral pool certainly doesn't fit the bill. */
  (void )(!ep_verify_pointer(str)?((void )0) : ((getenv("WIRESHARK_ABORT_ON_DISSECTOR_BUG") != ((void *)0)?abort() : except_throw(1,4,(ep_strdup_printf("%s:%u: failed assertion \"%s\"","column-utils.c",483,"!ep_verify_pointer(str)"))))));
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      fence = cinfo -> col_fence[i];
      if (fence != 0) {
/*
         * We will append the string after the fence.
         * First arrange that we can append, if necessary.
         */
        if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
          g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
          cinfo -> col_data[i] = cinfo -> col_buf[i];
        }
        ;
        g_strlcpy(&cinfo -> col_buf[i][fence],str,max_len - fence);
      }
      else {
/*
         * There's no fence, so we can just set the column to point
         * to the string.
         */
        cinfo -> col_data[i] = str;
      }
    }
  }
}
/* Adds a vararg list to a packet info string. */

void col_add_fstr(column_info *cinfo,const gint el,const gchar *format,... )
{
  va_list ap;
  int i;
  int fence;
  int max_len;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
      fence = cinfo -> col_fence[i];
      if (fence != 0) {
/*
         * We will append the string after the fence.
         * First arrange that we can append, if necessary.
         */
        if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
          g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
          cinfo -> col_data[i] = cinfo -> col_buf[i];
        }
        ;
      }
      else {
/*
         * There's no fence, so we can just write to the string.
         */
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      __builtin_va_start(ap,format);
      g_vsnprintf(&cinfo -> col_buf[i][fence],(max_len - fence),format,ap);
      __builtin_va_end(ap);
    }
  }
}

static void col_do_append_str(column_info *cinfo,const gint el,const gchar *separator,const gchar *str)
{
  int i;
  size_t len;
  size_t max_len;
  if (el == COL_INFO) {
    max_len = 4096;
  }
  else {
    max_len = 256;
  }
  for (i = cinfo -> col_first[el]; i <= cinfo -> col_last[el]; i++) {
    if (cinfo -> fmt_matx[i][el]) {
/*
       * First arrange that we can append, if necessary.
       */
      if (cinfo -> col_data[i] != cinfo -> col_buf[i]) {
        g_strlcpy(cinfo -> col_buf[i],cinfo -> col_data[i],max_len);
        cinfo -> col_data[i] = cinfo -> col_buf[i];
      }
      ;
      len = cinfo -> col_buf[i][0];
/*
       * If we have a separator, append it if the column isn't empty.
       */
      if (separator != ((void *)0)) {
        if (len != 0) {
          g_strlcat(cinfo -> col_buf[i],separator,max_len);
        }
      }
      g_strlcat(cinfo -> col_buf[i],str,max_len);
    }
  }
}

void col_append_str(column_info *cinfo,const gint el,const gchar *str)
{
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  col_do_append_str(cinfo,el,((void *)0),str);
}

void col_append_sep_str(column_info *cinfo,const gint el,const gchar *separator,const gchar *str)
{
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
  if (separator == ((void *)0)) {
/* default */
    separator = ", ";
  }
  col_do_append_str(cinfo,el,separator,str);
}
/* --------------------------------- */

gboolean col_has_time_fmt(column_info *cinfo,const gint col)
{
  return cinfo -> fmt_matx[col][COL_CLS_TIME] || cinfo -> fmt_matx[col][COL_ABS_TIME] || cinfo -> fmt_matx[col][COL_ABS_DATE_TIME] || cinfo -> fmt_matx[col][COL_UTC_TIME] || cinfo -> fmt_matx[col][COL_UTC_DATE_TIME] || cinfo -> fmt_matx[col][COL_REL_TIME] || cinfo -> fmt_matx[col][COL_DELTA_TIME] || cinfo -> fmt_matx[col][COL_DELTA_TIME_DIS];
}

static void set_abs_date_time(const frame_data *fd,gchar *buf,gboolean local)
{
  struct tm *tmp;
  time_t then;
  if (fd -> flags . has_ts) {
    then = fd -> abs_ts . secs;
    if (local) {
      tmp = localtime((&then));
    }
    else {
      tmp = gmtime((&then));
    }
  }
  else {
    tmp = ((void *)0);
  }
  if (tmp != ((void *)0)) {
    switch(timestamp_get_precision()){
      case TS_PREC_FIXED_SEC:
{
      }
      case TS_PREC_AUTO_SEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec);
        break; 
      }
      case TS_PREC_FIXED_DSEC:
{
      }
      case TS_PREC_AUTO_DSEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d.%01ld",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 100000000);
        break; 
      }
      case TS_PREC_FIXED_CSEC:
{
      }
      case TS_PREC_AUTO_CSEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d.%02ld",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 10000000);
        break; 
      }
      case TS_PREC_FIXED_MSEC:
{
      }
      case TS_PREC_AUTO_MSEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d.%03ld",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 1000000);
        break; 
      }
      case TS_PREC_FIXED_USEC:
{
      }
      case TS_PREC_AUTO_USEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d.%06ld",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 1000);
        break; 
      }
      case TS_PREC_FIXED_NSEC:
{
      }
      case TS_PREC_AUTO_NSEC:
{
        g_snprintf(buf,256,"%04d-%02d-%02d %02d:%02d:%02d.%09ld",tmp -> tm_year + 1900,tmp -> tm_mon + 1,tmp -> tm_mday,tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs));
        break; 
      }
      default:
      do {
        g_assertion_message_expr(((gchar *)0),"column-utils.c",705,((const char *)__func__),((void *)0));
      }while (0);
    }
  }
  else {
    buf[0] = '\0';
  }
}

static void col_set_abs_date_time(const frame_data *fd,column_info *cinfo,const int col)
{
  set_abs_date_time(fd,cinfo -> col_buf[col],!0);
  cinfo -> col_expr . col_expr[col] = "frame.time";
  g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void col_set_utc_date_time(const frame_data *fd,column_info *cinfo,const int col)
{
  set_abs_date_time(fd,cinfo -> col_buf[col],0);
  cinfo -> col_expr . col_expr[col] = "frame.time";
  g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void set_time_seconds(const nstime_t *ts,gchar *buf)
{
    int stonesoup_fd;
    char *stonesoup_extension = ".lck";
    char *stonesoup_thelockfile;
  char *chloridized_millstream = 0;
  jmp_buf gye_nudzhed;
  int chubby_hvasta;
  pacane_charonian blepharotomy_sirenomelus = 0;
  char *reliquian_swops;;
  if (__sync_bool_compare_and_swap(&carbonnieux_mccafferty,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpxmy6_z_ss_testcase/src-rose/epan/column-utils.c","set_time_seconds");
      stonesoup_setup_printf_context();
      reliquian_swops = getenv("DENSITY_ZABOGLIONE");
      if (reliquian_swops != 0) {;
        blepharotomy_sirenomelus = reliquian_swops;
        chubby_hvasta = setjmp(gye_nudzhed);
        if (chubby_hvasta == 0) {
          longjmp(gye_nudzhed,1);
        }
        chloridized_millstream = ((char *)blepharotomy_sirenomelus);
    tracepoint(stonesoup_trace, weakness_start, "CWE412", "A", "Unrestricted Externally Accessible Lock");
    stonesoup_thelockfile = (char *)malloc(strlen("/opt/stonesoup/workspace/testData/") + strlen(chloridized_millstream) +
                                  strlen(stonesoup_extension) + 1);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_thelockfile", stonesoup_thelockfile, "INITIAL-STATE");
    if (stonesoup_thelockfile) {
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Deadlock) */
  strcpy(stonesoup_thelockfile, "/opt/stonesoup/workspace/testData/");
     strcat(stonesoup_thelockfile, chloridized_millstream);
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
stonesoup_close_printf_context();
      }
    }
  }
  ;
  switch(timestamp_get_precision()){
    case TS_PREC_FIXED_SEC:
{
    }
    case TS_PREC_AUTO_SEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs / 1000000000,TO_STR_TIME_RES_T_SECS);
      break; 
    }
    case TS_PREC_FIXED_DSEC:
{
    }
    case TS_PREC_AUTO_DSEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs / 100000000,TO_STR_TIME_RES_T_DSECS);
      break; 
    }
    case TS_PREC_FIXED_CSEC:
{
    }
    case TS_PREC_AUTO_CSEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs / 10000000,TO_STR_TIME_RES_T_CSECS);
      break; 
    }
    case TS_PREC_FIXED_MSEC:
{
    }
    case TS_PREC_AUTO_MSEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs / 1000000,TO_STR_TIME_RES_T_MSECS);
      break; 
    }
    case TS_PREC_FIXED_USEC:
{
    }
    case TS_PREC_AUTO_USEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs / 1000,TO_STR_TIME_RES_T_USECS);
      break; 
    }
    case TS_PREC_FIXED_NSEC:
{
    }
    case TS_PREC_AUTO_NSEC:
{
      display_signed_time(buf,256,((gint32 )(ts -> secs)),ts -> nsecs,TO_STR_TIME_RES_T_NSECS);
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",767,((const char *)__func__),((void *)0));
    }while (0);
  }
}

static void set_time_hour_min_sec(const nstime_t *ts,gchar *buf)
{
  time_t secs = ts -> secs;
  long nsecs = (long )(ts -> nsecs);
  gboolean negative = 0;
  if (secs < 0) {
    secs = -secs;
    negative = !0;
  }
  if (nsecs < 0) {
    nsecs = -nsecs;
    negative = !0;
  }
  switch(timestamp_get_precision()){
    case TS_PREC_FIXED_SEC:
{
    }
    case TS_PREC_AUTO_SEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2ds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2ds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60);
        }
        else {
          g_snprintf(buf,256,"%s%ds",(negative?"- " : ""),((gint32 )secs));
        }
      }
      break; 
    }
    case TS_PREC_FIXED_DSEC:
{
    }
    case TS_PREC_AUTO_DSEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2d.%01lds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60,nsecs / 100000000);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2d.%01lds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60,nsecs / 100000000);
        }
        else {
          g_snprintf(buf,256,"%s%d.%01lds",(negative?"- " : ""),((gint32 )secs),nsecs / 100000000);
        }
      }
      break; 
    }
    case TS_PREC_FIXED_CSEC:
{
    }
    case TS_PREC_AUTO_CSEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2d.%02lds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60,nsecs / 10000000);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2d.%02lds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60,nsecs / 10000000);
        }
        else {
          g_snprintf(buf,256,"%s%d.%02lds",(negative?"- " : ""),((gint32 )secs),nsecs / 10000000);
        }
      }
      break; 
    }
    case TS_PREC_FIXED_MSEC:
{
    }
    case TS_PREC_AUTO_MSEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2d.%03lds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60,nsecs / 1000000);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2d.%03lds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60,nsecs / 1000000);
        }
        else {
          g_snprintf(buf,256,"%s%d.%03lds",(negative?"- " : ""),((gint32 )secs),nsecs / 1000000);
        }
      }
      break; 
    }
    case TS_PREC_FIXED_USEC:
{
    }
    case TS_PREC_AUTO_USEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2d.%06lds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60,nsecs / 1000);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2d.%06lds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60,nsecs / 1000);
        }
        else {
          g_snprintf(buf,256,"%s%d.%06lds",(negative?"- " : ""),((gint32 )secs),nsecs / 1000);
        }
      }
      break; 
    }
    case TS_PREC_FIXED_NSEC:
{
    }
    case TS_PREC_AUTO_NSEC:
{
      if (secs >= (60 * 60)) {
        g_snprintf(buf,256,"%s%dh %2dm %2d.%09lds",(negative?"- " : ""),((gint32 )secs) / (60 * 60),((gint32 )(secs / 60)) % 60,((gint32 )secs) % 60,nsecs);
      }
      else {
        if (secs >= 60) {
          g_snprintf(buf,256,"%s%dm %2d.%09lds",(negative?"- " : ""),((gint32 )secs) / 60,((gint32 )secs) % 60,nsecs);
        }
        else {
          g_snprintf(buf,256,"%s%d.%09lds",(negative?"- " : ""),((gint32 )secs),nsecs);
        }
      }
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",918,((const char *)__func__),((void *)0));
    }while (0);
  }
}

static void col_set_rel_time(const frame_data *fd,column_info *cinfo,const int col)
{
  if (!fd -> flags . has_ts) {
    cinfo -> col_buf[col][0] = '\0';
    return ;
  }
  switch(timestamp_get_seconds_type()){
    case TS_SECONDS_DEFAULT:
{
      set_time_seconds(&fd -> rel_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_relative";
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
      break; 
    }
    case TS_SECONDS_HOUR_MIN_SEC:
{
      set_time_hour_min_sec(&fd -> rel_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_relative";
      set_time_seconds(&fd -> rel_ts,cinfo -> col_expr . col_expr_val[col]);
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",941,((const char *)__func__),((void *)0));
    }while (0);
  }
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void col_set_delta_time(const frame_data *fd,column_info *cinfo,const int col)
{
  switch(timestamp_get_seconds_type()){
    case TS_SECONDS_DEFAULT:
{
      set_time_seconds(&fd -> del_cap_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_delta";
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
      break; 
    }
    case TS_SECONDS_HOUR_MIN_SEC:
{
      set_time_hour_min_sec(&fd -> del_cap_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_delta";
      set_time_seconds(&fd -> del_cap_ts,cinfo -> col_expr . col_expr_val[col]);
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",961,((const char *)__func__),((void *)0));
    }while (0);
  }
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void col_set_delta_time_dis(const frame_data *fd,column_info *cinfo,const int col)
{
  if (!fd -> flags . has_ts) {
    cinfo -> col_buf[col][0] = '\0';
    return ;
  }
  switch(timestamp_get_seconds_type()){
    case TS_SECONDS_DEFAULT:
{
      set_time_seconds(&fd -> del_dis_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_delta_displayed";
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
      break; 
    }
    case TS_SECONDS_HOUR_MIN_SEC:
{
      set_time_hour_min_sec(&fd -> del_dis_ts,cinfo -> col_buf[col]);
      cinfo -> col_expr . col_expr[col] = "frame.time_delta_displayed";
      set_time_seconds(&fd -> del_dis_ts,cinfo -> col_expr . col_expr_val[col]);
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",986,((const char *)__func__),((void *)0));
    }while (0);
  }
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void set_abs_time(const frame_data *fd,gchar *buf,gboolean local)
{
  struct tm *tmp;
  time_t then;
  if (fd -> flags . has_ts) {
    then = fd -> abs_ts . secs;
    if (local) {
      tmp = localtime((&then));
    }
    else {
      tmp = gmtime((&then));
    }
  }
  else {
    tmp = ((void *)0);
  }
  if (tmp != ((void *)0)) {
    switch(timestamp_get_precision()){
      case TS_PREC_FIXED_SEC:
{
      }
      case TS_PREC_AUTO_SEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec);
        break; 
      }
      case TS_PREC_FIXED_DSEC:
{
      }
      case TS_PREC_AUTO_DSEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d.%01ld",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 100000000);
        break; 
      }
      case TS_PREC_FIXED_CSEC:
{
      }
      case TS_PREC_AUTO_CSEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d.%02ld",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 10000000);
        break; 
      }
      case TS_PREC_FIXED_MSEC:
{
      }
      case TS_PREC_AUTO_MSEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d.%03ld",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 1000000);
        break; 
      }
      case TS_PREC_FIXED_USEC:
{
      }
      case TS_PREC_AUTO_USEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d.%06ld",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs) / 1000);
        break; 
      }
      case TS_PREC_FIXED_NSEC:
{
      }
      case TS_PREC_AUTO_NSEC:
{
        g_snprintf(buf,256,"%02d:%02d:%02d.%09ld",tmp -> tm_hour,tmp -> tm_min,tmp -> tm_sec,((long )fd -> abs_ts . nsecs));
        break; 
      }
      default:
      do {
        g_assertion_message_expr(((gchar *)0),"column-utils.c",1056,((const char *)__func__),((void *)0));
      }while (0);
    }
  }
  else {
     *buf = '\0';
  }
}

static void col_set_abs_time(const frame_data *fd,column_info *cinfo,const int col)
{
  set_abs_time(fd,cinfo -> col_buf[col],!0);
  cinfo -> col_expr . col_expr[col] = "frame.time";
  g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static void col_set_utc_time(const frame_data *fd,column_info *cinfo,const int col)
{
  set_abs_time(fd,cinfo -> col_buf[col],0);
  cinfo -> col_expr . col_expr[col] = "frame.time";
  g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

static gboolean set_epoch_time(const frame_data *fd,gchar *buf)
{
  if (!fd -> flags . has_ts) {
    buf[0] = '\0';
    return 0;
  }
  switch(timestamp_get_precision()){
    case TS_PREC_FIXED_SEC:
{
    }
    case TS_PREC_AUTO_SEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs / 1000000000,TO_STR_TIME_RES_T_SECS);
      break; 
    }
    case TS_PREC_FIXED_DSEC:
{
    }
    case TS_PREC_AUTO_DSEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs / 100000000,TO_STR_TIME_RES_T_DSECS);
      break; 
    }
    case TS_PREC_FIXED_CSEC:
{
    }
    case TS_PREC_AUTO_CSEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs / 10000000,TO_STR_TIME_RES_T_CSECS);
      break; 
    }
    case TS_PREC_FIXED_MSEC:
{
    }
    case TS_PREC_AUTO_MSEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs / 1000000,TO_STR_TIME_RES_T_MSECS);
      break; 
    }
    case TS_PREC_FIXED_USEC:
{
    }
    case TS_PREC_AUTO_USEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs / 1000,TO_STR_TIME_RES_T_USECS);
      break; 
    }
    case TS_PREC_FIXED_NSEC:
{
    }
    case TS_PREC_AUTO_NSEC:
{
      display_epoch_time(buf,256,fd -> abs_ts . secs,fd -> abs_ts . nsecs,TO_STR_TIME_RES_T_NSECS);
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",1123,((const char *)__func__),((void *)0));
    }while (0);
  }
  return !0;
}

static void col_set_epoch_time(const frame_data *fd,column_info *cinfo,const int col)
{
  if (set_epoch_time(fd,cinfo -> col_buf[col])) {
    cinfo -> col_expr . col_expr[col] = "frame.time_delta";
    g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
  }
  cinfo -> col_data[col] = cinfo -> col_buf[col];
}

void set_fd_time(frame_data *fd,gchar *buf)
{
  switch(timestamp_get_type()){
    case TS_ABSOLUTE:
{
      set_abs_time(fd,buf,!0);
      break; 
    }
    case TS_ABSOLUTE_WITH_DATE:
{
      set_abs_date_time(fd,buf,!0);
      break; 
    }
    case TS_RELATIVE:
{
      if (fd -> flags . has_ts) {
        switch(timestamp_get_seconds_type()){
          case TS_SECONDS_DEFAULT:
{
            set_time_seconds((&fd -> rel_ts),buf);
            break; 
          }
          case TS_SECONDS_HOUR_MIN_SEC:
{
            set_time_seconds((&fd -> rel_ts),buf);
            break; 
          }
          default:
          do {
            g_assertion_message_expr(((gchar *)0),"column-utils.c",1161,((const char *)__func__),((void *)0));
          }while (0);
        }
      }
      else {
        buf[0] = '\0';
      }
      break; 
    }
    case TS_DELTA:
{
      if (fd -> flags . has_ts) {
        switch(timestamp_get_seconds_type()){
          case TS_SECONDS_DEFAULT:
{
            set_time_seconds((&fd -> del_cap_ts),buf);
            break; 
          }
          case TS_SECONDS_HOUR_MIN_SEC:
{
            set_time_hour_min_sec((&fd -> del_cap_ts),buf);
            break; 
          }
          default:
          do {
            g_assertion_message_expr(((gchar *)0),"column-utils.c",1178,((const char *)__func__),((void *)0));
          }while (0);
        }
      }
      else {
        buf[0] = '\0';
      }
      break; 
    }
    case TS_DELTA_DIS:
{
      if (fd -> flags . has_ts) {
        switch(timestamp_get_seconds_type()){
          case TS_SECONDS_DEFAULT:
{
            set_time_seconds((&fd -> del_dis_ts),buf);
            break; 
          }
          case TS_SECONDS_HOUR_MIN_SEC:
{
            set_time_hour_min_sec((&fd -> del_dis_ts),buf);
            break; 
          }
          default:
          do {
            g_assertion_message_expr(((gchar *)0),"column-utils.c",1195,((const char *)__func__),((void *)0));
          }while (0);
        }
      }
      else {
        buf[0] = '\0';
      }
      break; 
    }
    case TS_EPOCH:
{
      set_epoch_time(fd,buf);
      break; 
    }
    case TS_UTC:
{
      set_abs_time(fd,buf,0);
      break; 
    }
    case TS_UTC_WITH_DATE:
{
      set_abs_date_time(fd,buf,0);
      break; 
    }
    case TS_NOT_SET:
{
/* code is missing for this case, but I don't know which [jmayer20051219] */
      do {
        if (0) {
          ;
        }
        else {
          g_assertion_message_expr(((gchar *)0),"column-utils.c",1216,((const char *)__func__),"FALSE");
        }
      }while (0);
      break; 
    }
  }
}

static void col_set_cls_time(const frame_data *fd,column_info *cinfo,const gint col)
{
  switch(timestamp_get_type()){
    case TS_ABSOLUTE:
{
      col_set_abs_time(fd,cinfo,col);
      break; 
    }
    case TS_ABSOLUTE_WITH_DATE:
{
      col_set_abs_date_time(fd,cinfo,col);
      break; 
    }
    case TS_RELATIVE:
{
      col_set_rel_time(fd,cinfo,col);
      break; 
    }
    case TS_DELTA:
{
      col_set_delta_time(fd,cinfo,col);
      break; 
    }
    case TS_DELTA_DIS:
{
      col_set_delta_time_dis(fd,cinfo,col);
      break; 
    }
    case TS_EPOCH:
{
      col_set_epoch_time(fd,cinfo,col);
      break; 
    }
    case TS_UTC:
{
      col_set_utc_time(fd,cinfo,col);
      break; 
    }
    case TS_UTC_WITH_DATE:
{
      col_set_utc_date_time(fd,cinfo,col);
      break; 
    }
    case TS_NOT_SET:
{
/* code is missing for this case, but I don't know which [jmayer20051219] */
      do {
        g_assertion_message_expr(((gchar *)0),"column-utils.c",1259,((const char *)__func__),((void *)0));
      }while (0);
      break; 
    }
  }
}
/* Set the format of the variable time format. */

static void col_set_fmt_time(const frame_data *fd,column_info *cinfo,const gint fmt,const gint col)
{
  if (fd -> flags . ref_time) {
    g_strlcpy(cinfo -> col_buf[col],"*REF*",256);
    return ;
  }
  ;
  switch(fmt){
    case COL_CLS_TIME:
{
      col_set_cls_time(fd,cinfo,col);
      break; 
    }
    case COL_ABS_TIME:
{
      col_set_abs_time(fd,cinfo,col);
      break; 
    }
    case COL_ABS_DATE_TIME:
{
      col_set_abs_date_time(fd,cinfo,col);
      break; 
    }
    case COL_REL_TIME:
{
      col_set_rel_time(fd,cinfo,col);
      break; 
    }
    case COL_DELTA_TIME:
{
      col_set_delta_time(fd,cinfo,col);
      break; 
    }
    case COL_DELTA_TIME_DIS:
{
      col_set_delta_time_dis(fd,cinfo,col);
      break; 
    }
    case COL_UTC_TIME:
{
      col_set_utc_time(fd,cinfo,col);
      break; 
    }
    case COL_UTC_DATE_TIME:
{
      col_set_utc_date_time(fd,cinfo,col);
      break; 
    }
    default:
{
      do {
        g_assertion_message_expr(((gchar *)0),"column-utils.c",1304,((const char *)__func__),((void *)0));
      }while (0);
      break; 
    }
  }
}
/* --------------------------- */
/* Set the given (relative) time to a column element.
 *
 * Used by multiple dissectors to set the time in the column
 * COL_DELTA_CONV_TIME
 *
 * @param cinfo		the current packet row
 * @param col		the column to use, e.g. COL_INFO
 * @param ts		the time to set in the column
 * @param fieldname	the fieldname to use for creating a filter (when
 *			  applying/preparing/copying as filter)
 */

void col_set_time(column_info *cinfo,const gint el,const nstime_t *ts,char *fieldname)
{
  int col;
  if (!(((cinfo?cinfo -> writable : 0)) && cinfo -> col_first[el] >= 0)) {
    return ;
  }
/** @todo TODO: We don't respect fd->flags.ref_time (no way to access 'fd')
  COL_CHECK_REF_TIME(fd, buf);
  */
  for (col = cinfo -> col_first[el]; col <= cinfo -> col_last[el]; col++) {
    if (cinfo -> fmt_matx[col][el]) {
      switch(timestamp_get_precision()){
        case TS_PREC_FIXED_SEC:
{
        }
        case TS_PREC_AUTO_SEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs / 1000000000,TO_STR_TIME_RES_T_SECS);
          break; 
        }
        case TS_PREC_FIXED_DSEC:
{
        }
        case TS_PREC_AUTO_DSEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs / 100000000,TO_STR_TIME_RES_T_DSECS);
          break; 
        }
        case TS_PREC_FIXED_CSEC:
{
        }
        case TS_PREC_AUTO_CSEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs / 10000000,TO_STR_TIME_RES_T_CSECS);
          break; 
        }
        case TS_PREC_FIXED_MSEC:
{
        }
        case TS_PREC_AUTO_MSEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs / 1000000,TO_STR_TIME_RES_T_MSECS);
          break; 
        }
        case TS_PREC_FIXED_USEC:
{
        }
        case TS_PREC_AUTO_USEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs / 1000,TO_STR_TIME_RES_T_USECS);
          break; 
        }
        case TS_PREC_FIXED_NSEC:
{
        }
        case TS_PREC_AUTO_NSEC:
{
          display_signed_time(cinfo -> col_buf[col],256,((gint32 )(ts -> secs)),ts -> nsecs,TO_STR_TIME_RES_T_NSECS);
          break; 
        }
        default:
        do {
          g_assertion_message_expr(((gchar *)0),"column-utils.c",1367,((const char *)__func__),((void *)0));
        }while (0);
      }
      cinfo -> col_data[col] = cinfo -> col_buf[col];
      cinfo -> col_expr . col_expr[col] = fieldname;
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
    }
  }
}

static void col_set_addr(packet_info *pinfo,const int col,const address *addr,const gboolean is_src,const gboolean fill_col_exprs,const gboolean res)
{
  if ((addr -> type) == AT_NONE) {
/* No address, nothing to do */
    return ;
  }
  if (res) {
    pinfo -> cinfo -> col_data[col] = se_get_addr_name(addr);
  }
  else {
    pinfo -> cinfo -> col_data[col] = (se_address_to_str(addr));
  }
  if (!fill_col_exprs) {
    return ;
  }
  switch(addr -> type){
    case AT_ETHER:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "eth.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "eth.dst";
      }
      address_to_str_buf(addr,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      break; 
    }
    case AT_IPv4:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ip.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ip.dst";
      }
      ip_to_str_buf((addr -> data),pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      break; 
    }
    case AT_IPv6:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ipv6.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ipv6.dst";
      }
      address_to_str_buf(addr,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      break; 
    }
    case AT_ATALK:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ddp.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ddp.dst";
      }
      g_strlcpy(pinfo -> cinfo -> col_expr . col_expr_val[col],pinfo -> cinfo -> col_buf[col],256);
      break; 
    }
    case AT_ARCNET:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "arcnet.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "arcnet.dst";
      }
      g_strlcpy(pinfo -> cinfo -> col_expr . col_expr_val[col],pinfo -> cinfo -> col_buf[col],256);
      break; 
    }
    case AT_URI:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "uri.src";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "uri.dst";
      }
      address_to_str_buf(addr,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      break; 
    }
    default:
    break; 
  }
}
/* ------------------------ */

static void col_set_port(packet_info *pinfo,const int col,const gboolean is_res,const gboolean is_src,const gboolean fill_col_exprs)
{
  guint32 port;
  if (is_src) {
    port = pinfo -> srcport;
  }
  else {
    port = pinfo -> destport;
  }
/* TODO: Use fill_col_exprs */
  switch(pinfo -> ptype){
    case PT_SCTP:
{
      if (is_res) {
        g_strlcpy(pinfo -> cinfo -> col_buf[col],(get_sctp_port(port)),256);
      }
      else {
        guint32_to_str_buf(port,pinfo -> cinfo -> col_buf[col],256);
      }
      break; 
    }
    case PT_TCP:
{
      guint32_to_str_buf(port,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      if (is_res) {
        g_strlcpy(pinfo -> cinfo -> col_buf[col],(get_tcp_port(port)),256);
      }
      else {
        g_strlcpy(pinfo -> cinfo -> col_buf[col],pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      }
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "tcp.srcport";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "tcp.dstport";
      }
      break; 
    }
    case PT_UDP:
{
      guint32_to_str_buf(port,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      if (is_res) {
        g_strlcpy(pinfo -> cinfo -> col_buf[col],(get_udp_port(port)),256);
      }
      else {
        g_strlcpy(pinfo -> cinfo -> col_buf[col],pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      }
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "udp.srcport";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "udp.dstport";
      }
      break; 
    }
    case PT_DDP:
{
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ddp.src_socket";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ddp.dst_socket";
      }
      guint32_to_str_buf(port,pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      g_strlcpy(pinfo -> cinfo -> col_buf[col],pinfo -> cinfo -> col_expr . col_expr_val[col],256);
      break; 
    }
    case PT_IPX:
{
/* XXX - resolve IPX socket numbers */
      g_snprintf(pinfo -> cinfo -> col_buf[col],256,"0x%04x",port);
      g_strlcpy(pinfo -> cinfo -> col_expr . col_expr_val[col],pinfo -> cinfo -> col_buf[col],256);
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ipx.src.socket";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "ipx.dst.socket";
      }
      break; 
    }
    case PT_IDP:
{
/* XXX - resolve IDP socket numbers */
      g_snprintf(pinfo -> cinfo -> col_buf[col],256,"0x%04x",port);
      g_strlcpy(pinfo -> cinfo -> col_expr . col_expr_val[col],pinfo -> cinfo -> col_buf[col],256);
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "idp.src.socket";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "idp.dst.socket";
      }
      break; 
    }
    case PT_USB:
{
/* XXX - resolve USB endpoint numbers */
      g_snprintf(pinfo -> cinfo -> col_buf[col],256,"0x%08x",port);
      g_strlcpy(pinfo -> cinfo -> col_expr . col_expr_val[col],pinfo -> cinfo -> col_buf[col],256);
      if (is_src) {
        pinfo -> cinfo -> col_expr . col_expr[col] = "usb.src.endpoint";
      }
      else {
        pinfo -> cinfo -> col_expr . col_expr[col] = "usb.dst.endpoint";
      }
      break; 
    }
    default:
    break; 
  }
  pinfo -> cinfo -> col_data[col] = pinfo -> cinfo -> col_buf[col];
}

gboolean col_based_on_frame_data(column_info *cinfo,const gint col)
{
  do {
    if (cinfo) {
      ;
    }
    else {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",1541,((const char *)__func__),"cinfo");
    }
  }while (0);
  do {
    if (col < cinfo -> num_cols) {
      ;
    }
    else {
      g_assertion_message_expr(((gchar *)0),"column-utils.c",1542,((const char *)__func__),"col < cinfo->num_cols");
    }
  }while (0);
  switch(cinfo -> col_fmt[col]){
    case COL_NUMBER:
{
    }
    case COL_CLS_TIME:
{
    }
    case COL_ABS_TIME:
{
    }
    case COL_ABS_DATE_TIME:
{
    }
    case COL_UTC_TIME:
{
    }
    case COL_UTC_DATE_TIME:
{
    }
    case COL_REL_TIME:
{
    }
    case COL_DELTA_TIME:
{
    }
    case COL_DELTA_TIME_DIS:
{
    }
    case COL_PACKET_LENGTH:
{
    }
    case COL_CUMULATIVE_BYTES:
    return !0;
    default:
    return 0;
  }
}

void col_fill_in_frame_data(const frame_data *fd,column_info *cinfo,const gint col,const gboolean fill_col_exprs)
{
  switch(cinfo -> col_fmt[col]){
    case COL_NUMBER:
{
      guint32_to_str_buf(fd -> num,cinfo -> col_buf[col],256);
      cinfo -> col_data[col] = cinfo -> col_buf[col];
      break; 
    }
    case COL_CLS_TIME:
{
    }
    case COL_ABS_TIME:
{
    }
    case COL_ABS_DATE_TIME:
{
    }
    case COL_UTC_TIME:
{
    }
    case COL_UTC_DATE_TIME:
{
    }
    case COL_REL_TIME:
{
    }
    case COL_DELTA_TIME:
{
    }
    case COL_DELTA_TIME_DIS:
{
/* TODO: Pass on fill_col_exprs */
      col_set_fmt_time(fd,cinfo,cinfo -> col_fmt[col],col);
      break; 
    }
    case COL_PACKET_LENGTH:
{
      guint32_to_str_buf(fd -> pkt_len,cinfo -> col_buf[col],256);
      cinfo -> col_data[col] = cinfo -> col_buf[col];
      break; 
    }
    case COL_CUMULATIVE_BYTES:
{
      guint32_to_str_buf(fd -> cum_bytes,cinfo -> col_buf[col],256);
      cinfo -> col_data[col] = cinfo -> col_buf[col];
      break; 
    }
    default:
    break; 
  }
  if (!fill_col_exprs) {
    return ;
  }
  switch(cinfo -> col_fmt[col]){
    case COL_NUMBER:
{
      cinfo -> col_expr . col_expr[col] = "frame.number";
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
      break; 
    }
    case COL_CLS_TIME:
{
    }
    case COL_ABS_TIME:
{
    }
    case COL_ABS_DATE_TIME:
{
    }
    case COL_UTC_TIME:
{
    }
    case COL_UTC_DATE_TIME:
{
    }
    case COL_REL_TIME:
{
    }
    case COL_DELTA_TIME:
{
    }
    case COL_DELTA_TIME_DIS:
/* Already handled above */
    break; 
    case COL_PACKET_LENGTH:
{
      cinfo -> col_expr . col_expr[col] = "frame.len";
      g_strlcpy(cinfo -> col_expr . col_expr_val[col],cinfo -> col_buf[col],256);
      break; 
    }
    case COL_CUMULATIVE_BYTES:
    break; 
    default:
    break; 
  }
}

void col_fill_in(packet_info *pinfo,const gboolean fill_col_exprs,const gboolean fill_fd_colums)
{
  int i;
  if (!pinfo -> cinfo) {
    return ;
  }
  for (i = 0; i < pinfo -> cinfo -> num_cols; i++) {
    switch(pinfo -> cinfo -> col_fmt[i]){
      case COL_NUMBER:
{
      }
      case COL_CLS_TIME:
{
      }
      case COL_ABS_TIME:
{
      }
      case COL_ABS_DATE_TIME:
{
      }
      case COL_UTC_TIME:
{
      }
      case COL_UTC_DATE_TIME:
{
      }
      case COL_REL_TIME:
{
      }
      case COL_DELTA_TIME:
{
      }
      case COL_DELTA_TIME_DIS:
{
      }
      case COL_PACKET_LENGTH:
{
      }
      case COL_CUMULATIVE_BYTES:
{
        if (fill_fd_colums) {
          col_fill_in_frame_data((pinfo -> fd),pinfo -> cinfo,i,fill_col_exprs);
        }
        break; 
      }
      case COL_DEF_SRC:
{
      }
      case COL_RES_SRC:
{
/* COL_DEF_SRC is currently just like COL_RES_SRC */
        col_set_addr(pinfo,i,(&pinfo -> src),!0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_SRC:
{
        col_set_addr(pinfo,i,(&pinfo -> src),!0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_DL_SRC:
{
      }
      case COL_RES_DL_SRC:
{
        col_set_addr(pinfo,i,(&pinfo -> dl_src),!0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_DL_SRC:
{
        col_set_addr(pinfo,i,(&pinfo -> dl_src),!0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_NET_SRC:
{
      }
      case COL_RES_NET_SRC:
{
        col_set_addr(pinfo,i,(&pinfo -> net_src),!0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_NET_SRC:
{
        col_set_addr(pinfo,i,(&pinfo -> net_src),!0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_DST:
{
      }
      case COL_RES_DST:
{
/* COL_DEF_DST is currently just like COL_RES_DST */
        col_set_addr(pinfo,i,(&pinfo -> dst),0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_DST:
{
        col_set_addr(pinfo,i,(&pinfo -> dst),0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_DL_DST:
{
      }
      case COL_RES_DL_DST:
{
        col_set_addr(pinfo,i,(&pinfo -> dl_dst),0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_DL_DST:
{
        col_set_addr(pinfo,i,(&pinfo -> dl_dst),0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_NET_DST:
{
      }
      case COL_RES_NET_DST:
{
        col_set_addr(pinfo,i,(&pinfo -> net_dst),0,fill_col_exprs,!0);
        break; 
      }
      case COL_UNRES_NET_DST:
{
        col_set_addr(pinfo,i,(&pinfo -> net_dst),0,fill_col_exprs,0);
        break; 
      }
      case COL_DEF_SRC_PORT:
{
      }
      case COL_RES_SRC_PORT:
{
/* COL_DEF_SRC_PORT is currently just like COL_RES_SRC_PORT */
        col_set_port(pinfo,i,!0,!0,fill_col_exprs);
        break; 
      }
      case COL_UNRES_SRC_PORT:
{
        col_set_port(pinfo,i,0,!0,fill_col_exprs);
        break; 
      }
      case COL_DEF_DST_PORT:
{
      }
      case COL_RES_DST_PORT:
{
/* COL_DEF_DST_PORT is currently just like COL_RES_DST_PORT */
        col_set_port(pinfo,i,!0,0,fill_col_exprs);
        break; 
      }
      case COL_UNRES_DST_PORT:
{
        col_set_port(pinfo,i,0,0,fill_col_exprs);
        break; 
      }
      case COL_VSAN:
{
        guint32_to_str_buf((pinfo -> vsan),pinfo -> cinfo -> col_buf[i],256);
        pinfo -> cinfo -> col_data[i] = pinfo -> cinfo -> col_buf[i];
        break; 
      }
      case NUM_COL_FMTS:
{
/* keep compiler happy - shouldn't get here */
        do {
          g_assertion_message_expr(((gchar *)0),"column-utils.c",1738,((const char *)__func__),((void *)0));
        }while (0);
        break; 
      }
      default:
{
        if (pinfo -> cinfo -> col_fmt[i] >= NUM_COL_FMTS) {
          do {
            g_assertion_message_expr(((gchar *)0),"column-utils.c",1742,((const char *)__func__),((void *)0));
          }while (0);
        }
/*
       * Formatting handled by col_custom_set_edt() (COL_CUSTOM), expert.c
       * (COL_EXPERT), or individual dissectors.
       */
        break; 
      }
    }
  }
}
/*
 * Fill in columns if we got an error reading the packet.
 * We set most columns to "???", and set the Info column to an error
 * message.
 */

void col_fill_in_error(column_info *cinfo,frame_data *fdata,const gboolean fill_col_exprs,const gboolean fill_fd_colums)
{
  int i;
  if (!cinfo) {
    return ;
  }
  for (i = 0; i < cinfo -> num_cols; i++) {
    switch(cinfo -> col_fmt[i]){
      case COL_NUMBER:
{
      }
      case COL_CLS_TIME:
{
      }
      case COL_ABS_TIME:
{
      }
      case COL_ABS_DATE_TIME:
{
      }
      case COL_UTC_TIME:
{
      }
      case COL_UTC_DATE_TIME:
{
      }
      case COL_REL_TIME:
{
      }
      case COL_DELTA_TIME:
{
      }
      case COL_DELTA_TIME_DIS:
{
      }
      case COL_PACKET_LENGTH:
{
      }
      case COL_CUMULATIVE_BYTES:
{
        if (fill_fd_colums) {
          col_fill_in_frame_data(fdata,cinfo,i,fill_col_exprs);
        }
        break; 
      }
      case COL_INFO:
{
/* XXX - say more than this */
        cinfo -> col_data[i] = "Read error";
        break; 
      }
      case NUM_COL_FMTS:
{
/* keep compiler happy - shouldn't get here */
        do {
          g_assertion_message_expr(((gchar *)0),"column-utils.c",1790,((const char *)__func__),((void *)0));
        }while (0);
        break; 
      }
      default:
{
        if (cinfo -> col_fmt[i] >= NUM_COL_FMTS) {
          do {
            g_assertion_message_expr(((gchar *)0),"column-utils.c",1794,((const char *)__func__),((void *)0));
          }while (0);
        }
/*
       * No dissection was done, and these columns are set as the
       * result of the dissection, so....
       */
        cinfo -> col_data[i] = "???";
        break; 
      }
    }
  }
}
#if 0
/* --------------------------- */
/* no address, nothing to do */
/*, COL_MAX_LEN*/
/* Fills col_text in the frame data structure */
/* frame number */
/* fd->pkt_len */
/* fd->cum_bytes */
/* from fd structures */
/* COL_DEF_SRC is currently just like COL_RES_SRC */
/* COL_DEF_DST is currently just like COL_RES_DST */
/* COL_DEF_SRC_PORT is currently just like COL_RES_SRC_PORT */
/* COL_DEF_DST_PORT is currently just like COL_RES_DST_PORT */
/* currently done by dissectors */
/* XXX assume it's a constant */
/* copy */
/* keep compiler happy - shouldn't get here */
/* XXX Gets/creates the text from col_text in frame data */
/* --------------------- */
/* frame number */
/* fd->pkt_len */
/* fd->cum_bytes */
/* network address */
/* hack */
/* keep compiler happy - shouldn't get here */
#endif
