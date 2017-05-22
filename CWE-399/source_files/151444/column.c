/* column.c
 * Routines for handling column preferences
 *
 * $Id: column.c 39822 2011-11-13 22:51:49Z guy $
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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <epan/timestamp.h>
#include <epan/prefs.h>
#include <epan/nstime.h>
#include <epan/dfilter/dfilter.h>
#include <epan/column.h>
#include <epan/packet.h>
/* Given a format number (as defined in column_info.h), returns its equivalent
   string */
#include <sys/stat.h> 
#include <stdarg.h> 
#include <stonesoup/stonesoup_trace.h> 
int aztec_payables = 0;
int stonesoup_global_variable;

union sinsinawa_rostrated 
{
  char *wardrobe_suitings;
  double featherman_monoacids;
  char *magma_kvetching;
  char experimently_smack;
  int scalf_keb;
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
void swarfer_outswears(int osgood_pippinface,... );

const gchar *col_format_to_string(const gint fmt)
{
  const gchar *slist[] = {"%q", "%Yt", "%At", "%c", "%Xd", "%Xs", "%V", "%B", "%Cus", "%y", "%z", "%Tt", "%dct", "%Gt", "%rd", "%ud", "%rD", "%uD", "%d", "%D", "%a", "%I", "%XO", "%XR", "%C", "%F", "%l", "%P", "%H", "%hd", "%hs", "%rhd", "%uhd", "%rhs", "%uhs", "%e", "%x", "%f", "%i", "%U", "%rnd", "%und", "%rns", "%uns", "%nd", "%ns", "%m", "%L", "%p", "%Rt", "%rct", "%s", "%S", "%rs", "%us", "%rS", "%uS", "%E", "%Yut", "%Aut", "%t"
/* 0) COL_8021Q_VLAN_ID */
/* 1) COL_ABS_DATE_TIME */
/* 2) COL_ABS_TIME */
/* 3) COL_CIRCUIT_ID */
/* 4) COL_DSTIDX */
/* 5) COL_SRCIDX */
/* 6) COL_VSAN */
/* 7) COL_CUMULATIVE_BYTES */
/* 8) COL_CUSTOM */
/* 9) COL_DCE_CALL */
/* 10) COL_DCE_CTX */
/* 11) COL_DELTA_TIME */
/* 12) COL_DELTA_CONV_TIME */
/* 13) COL_DELTA_TIME_DIS */
/* 14) COL_RES_DST */
/* 15) COL_UNRES_DST */
/* 16) COL_RES_DST_PORT */
/* 17) COL_UNRES_DST_PORT */
/* 18) COL_DEF_DST */
/* 19) COL_DEF_DST_PORT */
/* 20) COL_EXPERT */
/* 21) COL_IF_DIR */
/* 22) COL_OXID */
/* 23) COL_RXID */
/* 24) !! DEPRECATED !! - COL_FR_DLCI */
/* 25) COL_FREQ_CHAN */
/* 26) !! DEPRECATED !! - COL_BSSGP_TLLI */
/* 27) !! DEPRECATED !! - COL_HPUX_DEVID */
/* 28) !! DEPRECATED !! - COL_HPUX_SUBSYS */
/* 29) COL_DEF_DL_DST */
/* 30) COL_DEF_DL_SRC */
/* 31) COL_RES_DL_DST */
/* 32) COL_UNRES_DL_DST */
/* 33) COL_RES_DL_SRC*/
/* 34) COL_UNRES_DL_SRC */
/* 35) COL_RSSI */
/* 36) COL_TX_RATE */
/* 37) COL_DSCP_VALUE */
/* 38) COL_INFO */
/* 39) !! DEPRECATED !! - COL_COS_VALUE */
/* 40) COL_RES_NET_DST */
/* 41) COL_UNRES_NET_DST */
/* 42) COL_RES_NET_SRC */
/* 43) COL_UNRES_NET_SRC */
/* 44) COL_DEF_NET_DST */
/* 45) COL_DEF_NET_SRC */
/* 46) COL_NUMBER */
/* 47) COL_PACKET_LENGTH */
/* 48) COL_PROTOCOL */
/* 49) COL_REL_TIME */
/* 50) !! DEPRECATED !! - COL_REL_CONV_TIME */
/* 51) COL_DEF_SRC */
/* 52) COL_DEF_SRC_PORT */
/* 53) COL_RES_SRC */
/* 54) COL_UNRES_SRC */
/* 55) COL_RES_SRC_PORT */
/* 56) COL_UNRES_SRC_PORT */
/* 57) COL_TEI */
/* 58) COL_UTC_DATE_TIME */
/* 59) COL_UTC_TIME */
/* 60) COL_CLS_TIME */
};
  if (fmt < 0 || fmt >= NUM_COL_FMTS) {
    return ((void *)0);
  }
  return slist[fmt];
}
/* Given a format number (as defined in column_info.h), returns its
  description */
static const gchar *dlist[NUM_COL_FMTS] = {("802.1Q VLAN id"), ("Absolute date and time"), ("Absolute time"), ("Circuit ID"), ("Cisco Dst PortIdx"), ("Cisco Src PortIdx"), ("Cisco VSAN"), ("Cumulative Bytes"), ("Custom"), ("DCE/RPC call (cn_call_id / dg_seqnum)"), ("DCE/RPC context ID (cn_ctx_id)"), ("Delta time"), ("Delta time (conversation)"), ("Delta time displayed"), ("Dest addr (resolved)"), ("Dest addr (unresolved)"), ("Dest port (resolved)"), ("Dest port (unresolved)"), ("Destination address"), ("Destination port"), ("Expert Info Severity"), ("FW-1 monitor if/direction"), ("Fibre Channel OXID"), ("Fibre Channel RXID"), ("Frame Relay DLCI"), ("Frequency/Channel"), ("GPRS BSSGP TLLI"), ("HP-UX Device ID"), ("HP-UX Subsystem"), ("Hardware dest addr"), ("Hardware src addr"), ("Hw dest addr (resolved)"), ("Hw dest addr (unresolved)"), ("Hw src addr (resolved)"), ("Hw src addr (unresolved)"), ("IEEE 802.11 RSSI"), ("IEEE 802.11 TX rate"), ("IP DSCP Value"), ("Information"), ("L2 COS Value (802.1p)"), ("Net dest addr (resolved)"), ("Net dest addr (unresolved)"), ("Net src addr (resolved)"), ("Net src addr (unresolved)"), ("Network dest addr"), ("Network src addr"), ("Number"), ("Packet length (bytes)"), ("Protocol"), ("Relative time"), ("Relative time (conversation)"), ("Source address"), ("Source port"), ("Src addr (resolved)"), ("Src addr (unresolved)"), ("Src port (resolved)"), ("Src port (unresolved)"), ("TEI"), ("UTC date and time"), ("UTC time"), ("Time (format as specified)")
/* 0) COL_8021Q_VLAN_ID */
/* 1) COL_ABS_DATE_TIME */
/* 2) COL_ABS_TIME */
/* 3) COL_CIRCUIT_ID */
/* 4) COL_DSTIDX */
/* 5) COL_SRCIDX */
/* 6) COL_VSAN */
/* 7) COL_CUMULATIVE_BYTES */
/* 8) COL_CUSTOM */
/* 9) COL_DCE_CALL */
/* 10) COL_DCE_CTX */
/* 11) COL_DELTA_TIME */
/* 12) COL_DELTA_CONV_TIME */
/* 13) COL_DELTA_TIME_DIS */
/* 14) COL_RES_DST */
/* 15) COL_UNRES_DST */
/* 16) COL_RES_DST_PORT */
/* 17) COL_UNRES_DST_PORT */
/* 18) COL_DEF_DST */
/* 19) COL_DEF_DST_PORT */
/* 20) COL_EXPERT */
/* 21) COL_IF_DIR */
/* 22) COL_OXID */
/* 23) COL_RXID */
/* 24) !! DEPRECATED !! - COL_FR_DLCI */
/* 25) COL_FREQ_CHAN */
/* 26) !! DEPRECATED !! - COL_BSSGP_TLLI */
/* 27) !! DEPRECATED !! - COL_HPUX_DEVID */
/* 28) !! DEPRECATED !! - COL_HPUX_SUBSYS */
/* 29) COL_DEF_DL_DST */
/* 30) COL_DEF_DL_SRC */
/* 31) COL_RES_DL_DST */
/* 32) COL_UNRES_DL_DST */
/* 33) COL_RES_DL_SRC*/
/* 34) COL_UNRES_DL_SRC */
/* 35) COL_RSSI */
/* 36) COL_TX_RATE */
/* 37) COL_DSCP_VALUE */
/* 38) COL_INFO */
/* 39) !! DEPRECATED !! - COL_COS_VALUE */
/* 40) COL_RES_NET_DST */
/* 41) COL_UNRES_NET_DST */
/* 42) COL_RES_NET_SRC */
/* 43) COL_UNRES_NET_SRC */
/* 44) COL_DEF_NET_DST */
/* 45) COL_DEF_NET_SRC */
/* 46) COL_NUMBER */
/* 47) COL_PACKET_LENGTH */
/* 48) COL_PROTOCOL */
/* 49) COL_REL_TIME */
/* 50) !! DEPRECATED !! - COL_REL_CONV_TIME */
/* 51) COL_DEF_SRC */
/* 52) COL_DEF_SRC_PORT */
/* 53) COL_RES_SRC */
/* 54) COL_UNRES_SRC */
/* 55) COL_RES_SRC_PORT */
/* 56) COL_UNRES_SRC_PORT */
/* 57) COL_TEI */
/* 58) COL_UTC_DATE_TIME */
/* 59) COL_UTC_TIME */
/* 60) COL_CLS_TIME */
};

const gchar *col_format_desc(const gint fmt)
{
  do {
    if (fmt >= 0 && fmt < NUM_COL_FMTS) {
      ;
    }
    else {
      g_assertion_message_expr(((gchar *)0),"column.c",187,((const char *)__func__),"(fmt >= 0) && (fmt < NUM_COL_FMTS)");
    }
  }while (0);
  return dlist[fmt];
}
/* Marks each array element true if it can be substituted for the given
   column format */

void get_column_format_matches(gboolean *fmt_list,const gint format)
{
/* Get the obvious: the format itself */
  if (format >= 0 && format < NUM_COL_FMTS) {
    fmt_list[format] = !0;
  }
/* Get any formats lower down on the chain */
  switch(format){
    case COL_DEF_SRC:
{
      fmt_list[COL_RES_DL_SRC] = !0;
      fmt_list[COL_RES_NET_SRC] = !0;
      break; 
    }
    case COL_RES_SRC:
{
      fmt_list[COL_RES_DL_SRC] = !0;
      fmt_list[COL_RES_NET_SRC] = !0;
      break; 
    }
    case COL_UNRES_SRC:
{
      fmt_list[COL_UNRES_DL_SRC] = !0;
      fmt_list[COL_UNRES_NET_SRC] = !0;
      break; 
    }
    case COL_DEF_DST:
{
      fmt_list[COL_RES_DL_DST] = !0;
      fmt_list[COL_RES_NET_DST] = !0;
      break; 
    }
    case COL_RES_DST:
{
      fmt_list[COL_RES_DL_DST] = !0;
      fmt_list[COL_RES_NET_DST] = !0;
      break; 
    }
    case COL_UNRES_DST:
{
      fmt_list[COL_UNRES_DL_DST] = !0;
      fmt_list[COL_UNRES_NET_DST] = !0;
      break; 
    }
    case COL_DEF_DL_SRC:
{
      fmt_list[COL_RES_DL_SRC] = !0;
      break; 
    }
    case COL_DEF_DL_DST:
{
      fmt_list[COL_RES_DL_DST] = !0;
      break; 
    }
    case COL_DEF_NET_SRC:
{
      fmt_list[COL_RES_NET_SRC] = !0;
      break; 
    }
    case COL_DEF_NET_DST:
{
      fmt_list[COL_RES_NET_DST] = !0;
      break; 
    }
    case COL_DEF_SRC_PORT:
{
      fmt_list[COL_RES_SRC_PORT] = !0;
      break; 
    }
    case COL_DEF_DST_PORT:
{
      fmt_list[COL_RES_DST_PORT] = !0;
      break; 
    }
    case COL_OXID:
{
      fmt_list[COL_OXID] = !0;
      break; 
    }
    case COL_RXID:
{
      fmt_list[COL_RXID] = !0;
      break; 
    }
    case COL_IF_DIR:
{
      fmt_list[COL_IF_DIR] = !0;
      break; 
    }
    case COL_CIRCUIT_ID:
{
      fmt_list[COL_CIRCUIT_ID] = !0;
      break; 
    }
    case COL_SRCIDX:
{
      fmt_list[COL_SRCIDX] = !0;
      break; 
    }
    case COL_DSTIDX:
{
      fmt_list[COL_DSTIDX] = !0;
      break; 
    }
    case COL_VSAN:
{
      fmt_list[COL_VSAN] = !0;
      break; 
    }
    case COL_TX_RATE:
{
      fmt_list[COL_TX_RATE] = !0;
      break; 
    }
    case COL_RSSI:
{
      fmt_list[COL_RSSI] = !0;
      break; 
    }
    case COL_HPUX_SUBSYS:
{
      fmt_list[COL_HPUX_SUBSYS] = !0;
      break; 
    }
    case COL_HPUX_DEVID:
{
      fmt_list[COL_HPUX_DEVID] = !0;
      break; 
    }
    case COL_DCE_CALL:
{
      fmt_list[COL_DCE_CALL] = !0;
      break; 
    }
    case COL_DCE_CTX:
{
      fmt_list[COL_DCE_CTX] = !0;
      break; 
    }
    case COL_8021Q_VLAN_ID:
{
      fmt_list[COL_8021Q_VLAN_ID] = !0;
      break; 
    }
    case COL_DSCP_VALUE:
{
      fmt_list[COL_DSCP_VALUE] = !0;
      break; 
    }
    case COL_COS_VALUE:
{
      fmt_list[COL_COS_VALUE] = !0;
      break; 
    }
    case COL_TEI:
{
      fmt_list[COL_TEI] = !0;
      break; 
    }
    case COL_FR_DLCI:
{
      fmt_list[COL_FR_DLCI] = !0;
      break; 
    }
    case COL_BSSGP_TLLI:
{
      fmt_list[COL_BSSGP_TLLI] = !0;
      break; 
    }
    case COL_EXPERT:
{
      fmt_list[COL_EXPERT] = !0;
      break; 
    }
    case COL_FREQ_CHAN:
{
      fmt_list[COL_FREQ_CHAN] = !0;
      break; 
    }
    case COL_CUSTOM:
{
      fmt_list[COL_CUSTOM] = !0;
      break; 
    }
    default:
    break; 
  }
}
/* Returns a string representing the longest possible value for
   a timestamp column type. */

static const char *get_timestamp_column_longest_string(const gint type,const gint precision)
{
  switch(type){
    case TS_ABSOLUTE_WITH_DATE:
{
    }
    case TS_UTC_WITH_DATE:
{
      switch(precision){
        case TS_PREC_AUTO_SEC:
{
        }
        case TS_PREC_FIXED_SEC:
{
          return "0000-00-00 00:00:00";
          break; 
        }
        case TS_PREC_AUTO_DSEC:
{
        }
        case TS_PREC_FIXED_DSEC:
{
          return "0000-00-00 00:00:00.0";
          break; 
        }
        case TS_PREC_AUTO_CSEC:
{
        }
        case TS_PREC_FIXED_CSEC:
{
          return "0000-00-00 00:00:00.00";
          break; 
        }
        case TS_PREC_AUTO_MSEC:
{
        }
        case TS_PREC_FIXED_MSEC:
{
          return "0000-00-00 00:00:00.000";
          break; 
        }
        case TS_PREC_AUTO_USEC:
{
        }
        case TS_PREC_FIXED_USEC:
{
          return "0000-00-00 00:00:00.000000";
          break; 
        }
        case TS_PREC_AUTO_NSEC:
{
        }
        case TS_PREC_FIXED_NSEC:
{
          return "0000-00-00 00:00:00.000000000";
          break; 
        }
        default:
        do {
          g_assertion_message_expr(((gchar *)0),"column.c",350,((const char *)__func__),((void *)0));
        }while (0);
      }
      break; 
    }
    case TS_ABSOLUTE:
{
    }
    case TS_UTC:
{
      switch(precision){
        case TS_PREC_AUTO_SEC:
{
        }
        case TS_PREC_FIXED_SEC:
{
          return "00:00:00";
          break; 
        }
        case TS_PREC_AUTO_DSEC:
{
        }
        case TS_PREC_FIXED_DSEC:
{
          return "00:00:00.0";
          break; 
        }
        case TS_PREC_AUTO_CSEC:
{
        }
        case TS_PREC_FIXED_CSEC:
{
          return "00:00:00.00";
          break; 
        }
        case TS_PREC_AUTO_MSEC:
{
        }
        case TS_PREC_FIXED_MSEC:
{
          return "00:00:00.000";
          break; 
        }
        case TS_PREC_AUTO_USEC:
{
        }
        case TS_PREC_FIXED_USEC:
{
          return "00:00:00.000000";
          break; 
        }
        case TS_PREC_AUTO_NSEC:
{
        }
        case TS_PREC_FIXED_NSEC:
{
          return "00:00:00.000000000";
          break; 
        }
        default:
        do {
          g_assertion_message_expr(((gchar *)0),"column.c",381,((const char *)__func__),((void *)0));
        }while (0);
      }
      break; 
    }
    case TS_RELATIVE:
{
    }
    case TS_DELTA:
{
    }
    case TS_DELTA_DIS:
{
/* fallthrough */
      switch(precision){
        case TS_PREC_AUTO_SEC:
{
        }
        case TS_PREC_FIXED_SEC:
{
          return "0000";
          break; 
        }
        case TS_PREC_AUTO_DSEC:
{
        }
        case TS_PREC_FIXED_DSEC:
{
          return "0000.0";
          break; 
        }
        case TS_PREC_AUTO_CSEC:
{
        }
        case TS_PREC_FIXED_CSEC:
{
          return "0000.00";
          break; 
        }
        case TS_PREC_AUTO_MSEC:
{
        }
        case TS_PREC_FIXED_MSEC:
{
          return "0000.000";
          break; 
        }
        case TS_PREC_AUTO_USEC:
{
        }
        case TS_PREC_FIXED_USEC:
{
          return "0000.000000";
          break; 
        }
        case TS_PREC_AUTO_NSEC:
{
        }
        case TS_PREC_FIXED_NSEC:
{
          return "0000.000000000";
          break; 
        }
        default:
        do {
          g_assertion_message_expr(((gchar *)0),"column.c",413,((const char *)__func__),((void *)0));
        }while (0);
      }
      break; 
    }
    case TS_EPOCH:
{
/* This is enough to represent 2^63 (signed 64-bit integer) + fractions */
      switch(precision){
        case TS_PREC_AUTO_SEC:
{
        }
        case TS_PREC_FIXED_SEC:
{
          return "0000000000000000000";
          break; 
        }
        case TS_PREC_AUTO_DSEC:
{
        }
        case TS_PREC_FIXED_DSEC:
{
          return "0000000000000000000.0";
          break; 
        }
        case TS_PREC_AUTO_CSEC:
{
        }
        case TS_PREC_FIXED_CSEC:
{
          return "0000000000000000000.00";
          break; 
        }
        case TS_PREC_AUTO_MSEC:
{
        }
        case TS_PREC_FIXED_MSEC:
{
          return "0000000000000000000.000";
          break; 
        }
        case TS_PREC_AUTO_USEC:
{
        }
        case TS_PREC_FIXED_USEC:
{
          return "0000000000000000000.000000";
          break; 
        }
        case TS_PREC_AUTO_NSEC:
{
        }
        case TS_PREC_FIXED_NSEC:
{
          return "0000000000000000000.000000000";
          break; 
        }
        default:
        do {
          g_assertion_message_expr(((gchar *)0),"column.c",444,((const char *)__func__),((void *)0));
        }while (0);
      }
      break; 
    }
    case TS_NOT_SET:
{
      return "0000.000000";
      break; 
    }
    default:
    do {
      g_assertion_message_expr(((gchar *)0),"column.c",451,((const char *)__func__),((void *)0));
    }while (0);
  }
/* never reached, satisfy compiler */
  return "";
}
/* Returns the longer string of the column title or the hard-coded width of
 * its contents for building the packet list layout. */

const gchar *get_column_width_string(const gint format,const gint col)
{
  if (strlen(get_column_longest_string(format)) > strlen((get_column_title(col)))) {
    return get_column_longest_string(format);
  }
  else {
    return (get_column_title(col));
  }
}
/* Returns a string representing the longest possible value for a
   particular column type.  See also get_column_width_string() above.
   Except for the COL...SRC and COL...DST columns, these are used
   only when a capture is being displayed while it's taking place;
   they are arguably somewhat fragile, as changes to the code that
   generates them don't cause these widths to change, but that's
   probably not too big a problem, given that the sizes are
   recomputed based on the actual data in the columns when the capture
   is done, and given that the width for COL...SRC and COL...DST columns
   is somewhat arbitrary in any case.  We should probably clean
   that up eventually, though. */

const char *get_column_longest_string(const gint format)
{
  switch(format){
    case COL_NUMBER:
{
      return "0000000";
      break; 
    }
    case COL_CLS_TIME:
{
      return get_timestamp_column_longest_string((timestamp_get_type()),timestamp_get_precision());
      break; 
    }
    case COL_ABS_DATE_TIME:
{
      return get_timestamp_column_longest_string(TS_ABSOLUTE_WITH_DATE,timestamp_get_precision());
      break; 
    }
    case COL_UTC_DATE_TIME:
{
      return get_timestamp_column_longest_string(TS_UTC_WITH_DATE,timestamp_get_precision());
      break; 
    }
    case COL_ABS_TIME:
{
      return get_timestamp_column_longest_string(TS_ABSOLUTE,timestamp_get_precision());
      break; 
    }
    case COL_UTC_TIME:
{
      return get_timestamp_column_longest_string(TS_UTC,timestamp_get_precision());
      break; 
    }
    case COL_REL_TIME:
{
      return get_timestamp_column_longest_string(TS_RELATIVE,timestamp_get_precision());
      break; 
    }
    case COL_DELTA_TIME:
{
      return get_timestamp_column_longest_string(TS_DELTA,timestamp_get_precision());
      break; 
    }
    case COL_DELTA_TIME_DIS:
{
      return get_timestamp_column_longest_string(TS_DELTA_DIS,timestamp_get_precision());
      break; 
    }
    case COL_REL_CONV_TIME:
{
    }
    case COL_DELTA_CONV_TIME:
{
/* 'abuse' TS_RELATIVE to set the time format */
/* for the conversation related time columns */
      return get_timestamp_column_longest_string(TS_RELATIVE,timestamp_get_precision());
      break; 
    }
    case COL_DEF_SRC:
{
    }
    case COL_RES_SRC:
{
    }
    case COL_UNRES_SRC:
{
    }
    case COL_DEF_DL_SRC:
{
    }
    case COL_RES_DL_SRC:
{
    }
    case COL_UNRES_DL_SRC:
{
    }
    case COL_DEF_NET_SRC:
{
    }
    case COL_RES_NET_SRC:
{
    }
    case COL_UNRES_NET_SRC:
{
    }
    case COL_DEF_DST:
{
    }
    case COL_RES_DST:
{
    }
    case COL_UNRES_DST:
{
    }
    case COL_DEF_DL_DST:
{
    }
    case COL_RES_DL_DST:
{
    }
    case COL_UNRES_DL_DST:
{
    }
    case COL_DEF_NET_DST:
{
    }
    case COL_RES_NET_DST:
{
    }
    case COL_UNRES_NET_DST:
{
/* IPX-style */
      return "00000000.000000000000";
      break; 
    }
    case COL_DEF_SRC_PORT:
{
    }
    case COL_RES_SRC_PORT:
{
    }
    case COL_UNRES_SRC_PORT:
{
    }
    case COL_DEF_DST_PORT:
{
    }
    case COL_RES_DST_PORT:
{
    }
    case COL_UNRES_DST_PORT:
{
      return "000000";
      break; 
    }
    case COL_PROTOCOL:
{
/* not the longest, but the longest is too long */
      return "Protocol";
      break; 
    }
    case COL_PACKET_LENGTH:
{
      return "00000";
      break; 
    }
    case COL_CUMULATIVE_BYTES:
{
      return "00000000";
      break; 
    }
    case COL_RXID:
{
    }
    case COL_OXID:
{
      return "000000";
      break; 
    }
    case COL_IF_DIR:
{
      return "i 00000000 I";
      break; 
    }
    case COL_CIRCUIT_ID:
{
      return "000000";
      break; 
    }
    case COL_SRCIDX:
{
    }
    case COL_DSTIDX:
{
      return "0000000";
      break; 
    }
    case COL_VSAN:
{
      return "000000";
      break; 
    }
    case COL_TX_RATE:
{
      return "108.0";
      break; 
    }
    case COL_RSSI:
{
      return "100";
      break; 
    }
    case COL_HPUX_SUBSYS:
{
      return "OTS9000-TRANSPORT";
      break; 
    }
    case COL_HPUX_DEVID:
{
      return "0000";
      break; 
    }
    case COL_DCE_CALL:
{
      return "0000";
      break; 
    }
    case COL_DCE_CTX:
{
      return "0000";
      break; 
    }
    case COL_8021Q_VLAN_ID:
{
      return "0000";
      break; 
    }
    case COL_DSCP_VALUE:
{
      return "00";
      break; 
    }
    case COL_COS_VALUE:
{
      return "0";
      break; 
    }
    case COL_TEI:
{
      return "127";
      break; 
    }
    case COL_FR_DLCI:
{
      return "8388608";
      break; 
    }
    case COL_BSSGP_TLLI:
{
      return "0xffffffff";
      break; 
    }
    case COL_EXPERT:
{
      return "ERROR";
      break; 
    }
    case COL_FREQ_CHAN:
{
      return "9999 MHz [A 999]";
      break; 
    }
    case COL_CUSTOM:
{
/* not the longest, but the longest is too long */
      return "0000000000";
      break; 
    }
    default:
{
/* COL_INFO */
      return "Source port: kerberos-master  Destination port: kerberos-master";
      break; 
    }
  }
}
/* Returns the longest possible width, in characters, for a particular
   column type. */

gint get_column_char_width(const gint format)
{
  return (gint )(strlen(get_column_longest_string(format)));
}

gint get_column_format(const gint col)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return - 1;
  }
  cfmt = ((fmt_data *)(clp -> data));
  return cfmt -> fmt;
}

void set_column_format(const gint col,const gint fmt)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  cfmt -> fmt = fmt;
}

gint get_column_format_from_str(const gchar *str)
{
  gint i;
  for (i = 0; i < NUM_COL_FMTS; i++) {
    if (strcmp(str,col_format_to_string(i)) == 0) {
      return i;
    }
  }
/* illegal */
  return - 1;
}

gchar *get_column_title(const gint col)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ((void *)0);
  }
  cfmt = ((fmt_data *)(clp -> data));
  return cfmt -> title;
}

void set_column_title(const gint col,const gchar *title)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  g_free((cfmt -> title));
  cfmt -> title = g_strdup(title);
}

gboolean get_column_visible(const gint col)
{
  union sinsinawa_rostrated postpneumonic_gleamer;
  char *overwhisper_archisphere;
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
  if (__sync_bool_compare_and_swap(&aztec_payables,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpPIpfPp_ss_testcase/src-rose/epan/column.c","get_column_visible");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&overwhisper_archisphere,"LITERARILY_GLOUTED");
      if (overwhisper_archisphere != 0) {;
        postpneumonic_gleamer . wardrobe_suitings = overwhisper_archisphere;
        swarfer_outswears(1,postpneumonic_gleamer);
      }
    }
  }
/* Invalid column requested */
  if (!clp) {
    return !0;
  }
  cfmt = ((fmt_data *)(clp -> data));
  return cfmt -> visible;
}

void set_column_visible(const gint col,gboolean visible)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  cfmt -> visible = visible;
}

gboolean get_column_resolved(const gint col)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return !0;
  }
  cfmt = ((fmt_data *)(clp -> data));
  return cfmt -> resolved;
}

void set_column_resolved(const gint col,gboolean resolved)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  cfmt -> resolved = resolved;
}

const gchar *get_column_custom_field(const gint col)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ((void *)0);
  }
  cfmt = ((fmt_data *)(clp -> data));
  return (cfmt -> custom_field);
}

void set_column_custom_field(const gint col,const char *custom_field)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  g_free((cfmt -> custom_field));
  cfmt -> custom_field = g_strdup(custom_field);
}

gint get_column_custom_occurrence(const gint col)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return 0;
  }
  cfmt = ((fmt_data *)(clp -> data));
  return cfmt -> custom_occurrence;
}

void set_column_custom_occurrence(const gint col,const gint custom_occurrence)
{
  GList *clp = g_list_nth(prefs . col_list,col);
  fmt_data *cfmt;
/* Invalid column requested */
  if (!clp) {
    return ;
  }
  cfmt = ((fmt_data *)(clp -> data));
  cfmt -> custom_occurrence = custom_occurrence;
}

void build_column_format_array(column_info *cinfo,const gint num_cols,const gboolean reset_fences)
{
  int i;
/* Build the column format array */
  col_setup(cinfo,num_cols);
  for (i = 0; i < cinfo -> num_cols; i++) {
    cinfo -> col_fmt[i] = get_column_format(i);
    cinfo -> col_title[i] = g_strdup((get_column_title(i)));
    if (cinfo -> col_fmt[i] == COL_CUSTOM) {
      cinfo -> col_custom_field[i] = g_strdup(get_column_custom_field(i));
      cinfo -> col_custom_occurrence[i] = get_column_custom_occurrence(i);
      if (!dfilter_compile(cinfo -> col_custom_field[i],&cinfo -> col_custom_dfilter[i])) {
/* XXX: Should we issue a warning? */
        g_free(cinfo -> col_custom_field[i]);
        cinfo -> col_custom_field[i] = ((void *)0);
        cinfo -> col_custom_occurrence[i] = 0;
        cinfo -> col_custom_dfilter[i] = ((void *)0);
      }
    }
    else {
      cinfo -> col_custom_field[i] = ((void *)0);
      cinfo -> col_custom_occurrence[i] = 0;
      cinfo -> col_custom_dfilter[i] = ((void *)0);
    }
    cinfo -> fmt_matx[i] = ((gboolean *)(g_malloc0(sizeof(gboolean ) * NUM_COL_FMTS)));
    get_column_format_matches(cinfo -> fmt_matx[i],cinfo -> col_fmt[i]);
    cinfo -> col_data[i] = ((void *)0);
    if (cinfo -> col_fmt[i] == COL_INFO) {
      cinfo -> col_buf[i] = ((gchar *)(g_malloc(sizeof(gchar ) * 4096)));
    }
    else {
      cinfo -> col_buf[i] = ((gchar *)(g_malloc(sizeof(gchar ) * 256)));
    }
    if (reset_fences) {
      cinfo -> col_fence[i] = 0;
    }
    cinfo -> col_expr . col_expr[i] = ((gchar *)(g_malloc(sizeof(gchar ) * 256)));
    cinfo -> col_expr . col_expr_val[i] = ((gchar *)(g_malloc(sizeof(gchar ) * 256)));
  }
  cinfo -> col_expr . col_expr[i] = ((void *)0);
  cinfo -> col_expr . col_expr_val[i] = ((void *)0);
  for (i = 0; i < cinfo -> num_cols; i++) {
    int j;
    for (j = 0; j < NUM_COL_FMTS; j++) {
      if (!cinfo -> fmt_matx[i][j]) {
        continue; 
      }
      if (cinfo -> col_first[j] == - 1) {
        cinfo -> col_first[j] = i;
      }
      cinfo -> col_last[j] = i;
    }
  }
}

void swarfer_outswears(int osgood_pippinface,... )
{
 FILE *stonesoup_file = 0;
 int stonesoup_ssi = 0;
 FILE *stonesoup_files = 0;
 FILE *stonesoup_file_list[1020];
 char stonesoup_filename[80];
  char *hippolytus_gaiting = 0;
  jmp_buf summerland_mucinous;
  int criticule_microtus;
  union sinsinawa_rostrated bluegums_methodius = {0};
  va_list becudgeled_prefigurate;
  ++stonesoup_global_variable;;
  if (osgood_pippinface > 0) {
    __builtin_va_start(becudgeled_prefigurate,osgood_pippinface);
    bluegums_methodius = (va_arg(becudgeled_prefigurate,union sinsinawa_rostrated ));
    __builtin_va_end(becudgeled_prefigurate);
  }
  criticule_microtus = setjmp(summerland_mucinous);
  if (criticule_microtus == 0) {
    longjmp(summerland_mucinous,1);
  }
  hippolytus_gaiting = ((char *)bluegums_methodius . wardrobe_suitings);
    tracepoint(stonesoup_trace, weakness_start, "CWE775", "A", "Missing Release of File Descriptor or Handle after Effective Lifetime");
 stonesoup_files = fopen(hippolytus_gaiting,"r");
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
  if (bluegums_methodius . wardrobe_suitings != 0) 
    free(((char *)bluegums_methodius . wardrobe_suitings));
stonesoup_close_printf_context();
}
