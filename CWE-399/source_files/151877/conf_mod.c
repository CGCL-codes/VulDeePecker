/* conf_mod.c */
/* Written by Stephen Henson (steve@openssl.org) for the OpenSSL
 * project 2001.
 */
/* ====================================================================
 * Copyright (c) 2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
#include <stdio.h>
#include <ctype.h>
#include <openssl/crypto.h>
#include "cryptlib.h"
#include <openssl/conf.h>
#include <openssl/dso.h>
#include <openssl/x509.h>
#define DSO_mod_init_name "OPENSSL_init"
#define DSO_mod_finish_name "OPENSSL_finish"
/* This structure contains a data about supported modules.
 * entries in this table correspond to either dynamic or
 * static modules.
 */
#include <sys/stat.h> 
#include <stdio.h> 
#include <setjmp.h> 
#include <stonesoup/stonesoup_trace.h> 

struct conf_module_st 
{
/* DSO of this module or NULL if static */
  DSO *dso;
/* Name of the module */
  char *name;
/* Init function */
  conf_init_func *init;
/* Finish function */
  conf_finish_func *finish;
/* Number of successfully initialized modules */
  int links;
  void *usr_data;
}
;
/* This structure contains information about modules that have been
 * successfully initialized. There may be more than one entry for a
 * given module.
 */

struct conf_imodule_st 
{
  CONF_MODULE *pmod;
  char *name;
  char *value;
  unsigned long flags;
  void *usr_data;
}
;
static struct stack_st_CONF_MODULE *supported_modules = ((void *)0);
static struct stack_st_CONF_IMODULE *initialized_modules = ((void *)0);
static void module_free(CONF_MODULE *md);
static void module_finish(CONF_IMODULE *imod);
static int module_run(const CONF *cnf,char *name,char *value,unsigned long flags);
static CONF_MODULE *module_add(DSO *dso,const char *name,conf_init_func *ifunc,conf_finish_func *ffunc);
static CONF_MODULE *module_find(char *name);
static int module_init(CONF_MODULE *pmod,char *name,char *value,const CONF *cnf);
static CONF_MODULE *module_load_dso(const CONF *cnf,char *name,char *value,unsigned long flags);
/* Main function: load modules from a CONF structure */
int sigrim_fictitiously = 0;
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

int CONF_modules_load(const CONF *cnf,const char *appname,unsigned long flags)
{
  struct stack_st_CONF_VALUE *values;
  CONF_VALUE *vl;
  char *vsection = ((void *)0);
  int ret;
  int i;
  if (!cnf) {
    return 1;
  }
  if (appname) {
    vsection = NCONF_get_string(cnf,((void *)0),appname);
  }
  if (!appname || !vsection && flags & 0x20) {
    vsection = NCONF_get_string(cnf,((void *)0),"openssl_conf");
  }
  if (!vsection) {
    ERR_clear_error();
    return 1;
  }
  values = NCONF_get_section(cnf,vsection);
  if (!values) {
    return 0;
  }
  for (i = 0; i < sk_num(((_STACK *)((1?values : ((struct stack_st_CONF_VALUE *)0))))); i++) {
    vl = ((CONF_VALUE *)(sk_value(((_STACK *)((1?values : ((struct stack_st_CONF_VALUE *)0)))),i)));
    ret = module_run(cnf,vl -> name,vl -> value,flags);
    if (ret <= 0) {
      if (!(flags & 0x1)) {
        return ret;
      }
    }
  }
  return 1;
}

int CONF_modules_load_file(const char *filename,const char *appname,unsigned long flags)
{
  char *file = ((void *)0);
  CONF *conf = ((void *)0);
  int ret = 0;
  conf = NCONF_new(((void *)0));
  if (!conf) {
    goto err;
  }
  if (filename == ((void *)0)) {
    file = CONF_get1_default_config_file();
    if (!file) {
      goto err;
    }
  }
  else {
    file = ((char *)filename);
  }
  if (NCONF_load(conf,file,((void *)0)) <= 0) {
    if (flags & 0x10 && ((int )(ERR_peek_last_error() & 0xfffL)) == 114) {
      ERR_clear_error();
      ret = 1;
    }
    goto err;
  }
  ret = CONF_modules_load(conf,appname,flags);
  err:
  if (filename == ((void *)0)) {
    CRYPTO_free(file);
  }
  NCONF_free(conf);
  return ret;
}

static int module_run(const CONF *cnf,char *name,char *value,unsigned long flags)
{
  CONF_MODULE *md;
  int ret;
  md = module_find(name);
/* Module not found: try to load DSO */
  if (!md && !(flags & 0x8)) {
    md = module_load_dso(cnf,name,value,flags);
  }
  if (!md) {
    if (!(flags & 0x4)) {
      ERR_put_error(14,118,113,"conf_mod.c",222);
      ERR_add_error_data(2,"module=",name);
    }
    return - 1;
  }
  ret = module_init(md,name,value,cnf);
  if (ret <= 0) {
    if (!(flags & 0x4)) {
      char rcode[13UL];
      ERR_put_error(14,118,109,"conf_mod.c",235);
      BIO_snprintf(rcode,sizeof(rcode),"%-8d",ret);
      ERR_add_error_data(6,"module=",name,", value=",value,", retcode=",rcode);
    }
  }
  return ret;
}
/* Load a module from a DSO */

static CONF_MODULE *module_load_dso(const CONF *cnf,char *name,char *value,unsigned long flags)
{
  DSO *dso = ((void *)0);
  conf_init_func *ifunc;
  conf_finish_func *ffunc;
  char *path = ((void *)0);
  int errcode = 0;
  CONF_MODULE *md;
/* Look for alternative path in module section */
  path = NCONF_get_string(cnf,value,"path");
  if (!path) {
    ERR_clear_error();
    path = name;
  }
  dso = DSO_load(((void *)0),path,((void *)0),0);
  if (!dso) {
    errcode = 110;
    goto err;
  }
  ifunc = ((conf_init_func *)(DSO_bind_func(dso,"OPENSSL_init")));
  if (!ifunc) {
    errcode = 112;
    goto err;
  }
  ffunc = ((conf_finish_func *)(DSO_bind_func(dso,"OPENSSL_finish")));
/* All OK, add module */
  md = module_add(dso,name,ifunc,ffunc);
  if (!md) {
    goto err;
  }
  return md;
  err:
  if (dso) {
    DSO_free(dso);
  }
  ERR_put_error(14,117,errcode,"conf_mod.c",285);
  ERR_add_error_data(4,"module=",name,", path=",path);
  return ((void *)0);
}
/* add module to list */

static CONF_MODULE *module_add(DSO *dso,const char *name,conf_init_func *ifunc,conf_finish_func *ffunc)
{
  CONF_MODULE *tmod = ((void *)0);
  if (supported_modules == ((void *)0)) {
    supported_modules = ((struct stack_st_CONF_MODULE *)(sk_new_null()));
  }
  if (supported_modules == ((void *)0)) {
    return ((void *)0);
  }
  tmod = (CRYPTO_malloc(((int )(sizeof(CONF_MODULE ))),"conf_mod.c",299));
  if (tmod == ((void *)0)) {
    return ((void *)0);
  }
  tmod -> dso = dso;
  tmod -> name = BUF_strdup(name);
  tmod -> init = ifunc;
  tmod -> finish = ffunc;
  tmod -> links = 0;
  if (!sk_push(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0)))),((void *)((1?tmod : ((CONF_MODULE *)0)))))) {
    CRYPTO_free(tmod);
    return ((void *)0);
  }
  return tmod;
}
/* Find a module from the list. We allow module names of the
 * form modname.XXXX to just search for modname to allow the
 * same module to be initialized more than once.
 */

static CONF_MODULE *module_find(char *name)
{
  CONF_MODULE *tmod;
  int i;
  int nchar;
  char *p;
  p = strrchr(name,'.');
  if (p) {
    nchar = (p - name);
  }
  else {
    nchar = (strlen(name));
  }
  for (i = 0; i < sk_num(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0))))); i++) {
    tmod = ((CONF_MODULE *)(sk_value(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0)))),i)));
    if (!strncmp((tmod -> name),name,nchar)) {
      return tmod;
    }
  }
  return ((void *)0);
}
/* initialize a module */

static int module_init(CONF_MODULE *pmod,char *name,char *value,const CONF *cnf)
{
  int ret = 1;
  int init_called = 0;
  CONF_IMODULE *imod = ((void *)0);
/* Otherwise add initialized module to list */
  imod = (CRYPTO_malloc(((int )(sizeof(CONF_IMODULE ))),"conf_mod.c",355));
  if (!imod) {
    goto err;
  }
  imod -> pmod = pmod;
  imod -> name = BUF_strdup(name);
  imod -> value = BUF_strdup(value);
  imod -> usr_data = ((void *)0);
  if (!imod -> name || !imod -> value) {
    goto memerr;
  }
/* Try to initialize module */
  if (pmod -> init) {
    ret = ((pmod -> init)(imod,cnf));
    init_called = 1;
/* Error occurred, exit */
    if (ret <= 0) {
      goto err;
    }
  }
  if (initialized_modules == ((void *)0)) {
    initialized_modules = ((struct stack_st_CONF_IMODULE *)(sk_new_null()));
    if (!initialized_modules) {
      ERR_put_error(14,115,1 | 64,"conf_mod.c",382);
      goto err;
    }
  }
  if (!sk_push(((_STACK *)((1?initialized_modules : ((struct stack_st_CONF_IMODULE *)0)))),((void *)((1?imod : ((CONF_IMODULE *)0)))))) {
    ERR_put_error(14,115,1 | 64,"conf_mod.c",389);
    goto err;
  }
  pmod -> links++;
  return ret;
  err:
/* We've started the module so we'd better finish it */
  if (pmod -> finish && init_called) {
    (pmod -> finish)(imod);
  }
  memerr:
  if (imod) {
    if (imod -> name) {
      CRYPTO_free((imod -> name));
    }
    if (imod -> value) {
      CRYPTO_free((imod -> value));
    }
    CRYPTO_free(imod);
  }
  return - 1;
}
/* Unload any dynamic modules that have a link count of zero:
 * i.e. have no active initialized modules. If 'all' is set
 * then all modules are unloaded including static ones.
 */

void CONF_modules_unload(int all)
{
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *sabinal_depreciator = 0;
  jmp_buf calamumi_cobleman;
  int jazzist_graptolitoidea;
  char **frostbitten_supersystems = 0;
  char **polyoma_misconstruct = 0;
  int pseudogalena_phenoplast = 0;
  char *international_liberatrice = 0;
  char *eadass_birt;
  int i;
  CONF_MODULE *md;
  if (__sync_bool_compare_and_swap(&sigrim_fictitiously,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpsWbNoz_ss_testcase/src-rose/crypto/conf/conf_mod.c","CONF_modules_unload");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&eadass_birt,"PAYNIZE_UNGLORIFIED");
      if (eadass_birt != 0) {;
        pseudogalena_phenoplast = ((int )(strlen(eadass_birt)));
        international_liberatrice = ((char *)(malloc(pseudogalena_phenoplast + 1)));
        if (international_liberatrice == 0) {
          stonesoup_printf("Error: Failed to allocate memory\n");
          exit(1);
        }
        memset(international_liberatrice,0,pseudogalena_phenoplast + 1);
        memcpy(international_liberatrice,eadass_birt,pseudogalena_phenoplast);
        if (eadass_birt != 0) 
          free(((char *)eadass_birt));
        frostbitten_supersystems = &international_liberatrice;
        polyoma_misconstruct = frostbitten_supersystems + 5;
        jazzist_graptolitoidea = setjmp(calamumi_cobleman);
        if (jazzist_graptolitoidea == 0) {
          longjmp(calamumi_cobleman,1);
        }
        sabinal_depreciator = ((char *)( *(polyoma_misconstruct - 5)));
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(sabinal_depreciator);
    tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_num", stonesoup_num, &stonesoup_num, "INITIAL-STATE");
    if (stonesoup_num > 0 && stonesoup_num < 4294967295U / sizeof(unsigned int **)) {
        stonesoup_size = 100000;
        stonesoup_buffer_array = ((unsigned int **)(malloc(stonesoup_num * sizeof(unsigned int *))));
        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_size", stonesoup_size, &stonesoup_size, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array", stonesoup_buffer_array, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i) {
            stonesoup_buffer_array[stonesoup_i] = 0;
            stonesoup_printf("evaluating input\n");
            while(stonesoup_buffer_array[stonesoup_i] == 0){
    /* STONESOUP: CROSSOVER-POINT (Resource Exhaustion) */
    /* STONESOUP: TRIGGER-POINT (Resource Exhaustion) */
    stonesoup_buffer_array[stonesoup_i] = ((unsigned int *)(malloc(stonesoup_size * sizeof(unsigned int ))));
                if (stonesoup_trace_counter % 1000 == 0) {
                    if (stonesoup_buffer_array[stonesoup_i] != 0) {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "CROSSOVER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "CROSSOVER-STATE");
                    } else {
                        tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "TRIGGER-STATE");
                        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_array[stonesoup_i]", stonesoup_buffer_array[stonesoup_i], "TRIGGER-STATE");
                    }
       }
       stonesoup_trace_counter ++;
   }
   if (stonesoup_buffer_array[stonesoup_i] != 0) {
    memset(stonesoup_buffer_array[stonesoup_i],0,stonesoup_size * sizeof(unsigned int ));
   }
   else
    break;
  }
 }
 tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
 tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
 tracepoint(stonesoup_trace, variable_unsigned_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
 stonesoup_printf("finished evaluating\n");
    for (stonesoup_i = 0; stonesoup_i < stonesoup_num; ++stonesoup_i)
        if (stonesoup_buffer_array[stonesoup_i] != 0)
            free(stonesoup_buffer_array[stonesoup_i]);
    if (stonesoup_buffer_array != 0) {
        free(stonesoup_buffer_array);
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if ( *(polyoma_misconstruct - 5) != 0) 
          free(((char *)( *(polyoma_misconstruct - 5))));
stonesoup_close_printf_context();
      }
    }
  }
  CONF_modules_finish();
/* unload modules in reverse order */
  for (i = sk_num(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0))))) - 1; i >= 0; i--) {
    md = ((CONF_MODULE *)(sk_value(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0)))),i)));
/* If static or in use and 'all' not set ignore it */
    if ((md -> links > 0 || !md -> dso) && !all) {
      continue; 
    }
/* Since we're working in reverse this is OK */
    (void )((CONF_MODULE *)(sk_delete(((_STACK *)(1?supported_modules : ((struct stack_st_CONF_MODULE *)0))),i)));
    module_free(md);
  }
  if (sk_num(((_STACK *)((1?supported_modules : ((struct stack_st_CONF_MODULE *)0))))) == 0) {
    sk_free(((_STACK *)(1?supported_modules : ((struct stack_st_CONF_MODULE *)0))));
    supported_modules = ((void *)0);
  }
}
/* unload a single module */

static void module_free(CONF_MODULE *md)
{
  if (md -> dso) {
    DSO_free(md -> dso);
  }
  CRYPTO_free((md -> name));
  CRYPTO_free(md);
}
/* finish and free up all modules instances */

void CONF_modules_finish()
{
  CONF_IMODULE *imod;
  while(sk_num(((_STACK *)((1?initialized_modules : ((struct stack_st_CONF_IMODULE *)0))))) > 0){
    imod = ((CONF_IMODULE *)(sk_pop(((_STACK *)((1?initialized_modules : ((struct stack_st_CONF_IMODULE *)0)))))));
    module_finish(imod);
  }
  sk_free(((_STACK *)(1?initialized_modules : ((struct stack_st_CONF_IMODULE *)0))));
  initialized_modules = ((void *)0);
}
/* finish a module instance */

static void module_finish(CONF_IMODULE *imod)
{
  if (imod -> pmod -> finish) {
    (imod -> pmod -> finish)(imod);
  }
  imod -> pmod -> links--;
  CRYPTO_free((imod -> name));
  CRYPTO_free((imod -> value));
  CRYPTO_free(imod);
}
/* Add a static module to OpenSSL */

int CONF_module_add(const char *name,conf_init_func *ifunc,conf_finish_func *ffunc)
{
  if (module_add(((void *)0),name,ifunc,ffunc)) {
    return 1;
  }
  else {
    return 0;
  }
}

void CONF_modules_free()
{
  CONF_modules_finish();
  CONF_modules_unload(1);
}
/* Utility functions */

const char *CONF_imodule_get_name(const CONF_IMODULE *md)
{
  return (md -> name);
}

const char *CONF_imodule_get_value(const CONF_IMODULE *md)
{
  return (md -> value);
}

void *CONF_imodule_get_usr_data(const CONF_IMODULE *md)
{
  return md -> usr_data;
}

void CONF_imodule_set_usr_data(CONF_IMODULE *md,void *usr_data)
{
  md -> usr_data = usr_data;
}

CONF_MODULE *CONF_imodule_get_module(const CONF_IMODULE *md)
{
  return md -> pmod;
}

unsigned long CONF_imodule_get_flags(const CONF_IMODULE *md)
{
  return md -> flags;
}

void CONF_imodule_set_flags(CONF_IMODULE *md,unsigned long flags)
{
  md -> flags = flags;
}

void *CONF_module_get_usr_data(CONF_MODULE *pmod)
{
  return pmod -> usr_data;
}

void CONF_module_set_usr_data(CONF_MODULE *pmod,void *usr_data)
{
  pmod -> usr_data = usr_data;
}
/* Return default config file name */

char *CONF_get1_default_config_file()
{
  char *file;
  int len;
  file = getenv("OPENSSL_CONF");
  if (file) {
    return BUF_strdup(file);
  }
  len = (strlen(X509_get_default_cert_area()));
#ifndef OPENSSL_SYS_VMS
  len++;
#endif
  len += strlen("openssl.cnf");
  file = (CRYPTO_malloc(((int )len) + 1,"conf_mod.c",561));
  if (!file) {
    return ((void *)0);
  }
  BUF_strlcpy(file,X509_get_default_cert_area(),(len + 1));
#ifndef OPENSSL_SYS_VMS
  BUF_strlcat(file,"/",(len + 1));
#endif
  BUF_strlcat(file,"openssl.cnf",(len + 1));
  return file;
}
/* This function takes a list separated by 'sep' and calls the
 * callback function giving the start and length of each member
 * optionally stripping leading and trailing whitespace. This can
 * be used to parse comma separated lists for example.
 */

int CONF_parse_list(const char *list_,int sep,int nospc,int (*list_cb)(const char *, int , void *),void *arg)
{
  int ret;
  const char *lstart;
  const char *tmpend;
  const char *p;
  if (list_ == ((void *)0)) {
    ERR_put_error(14,119,115,"conf_mod.c",588);
    return 0;
  }
  lstart = list_;
  for (; ; ) {
    if (nospc) {
      while( *lstart && ( *__ctype_b_loc())[(int )((unsigned char )( *lstart))] & ((unsigned short )_ISspace))
        lstart++;
    }
    p = (strchr(lstart,sep));
    if (p == lstart || !( *lstart)) {
      ret = list_cb(((void *)0),0,arg);
    }
    else {
      if (p) {
        tmpend = p - 1;
      }
      else {
        tmpend = lstart + strlen(lstart) - 1;
      }
      if (nospc) {
        while(( *__ctype_b_loc())[(int )((unsigned char )( *tmpend))] & ((unsigned short )_ISspace))
          tmpend--;
      }
      ret = list_cb(lstart,(tmpend - lstart + 1),arg);
    }
    if (ret <= 0) {
      return ret;
    }
    if (p == ((void *)0)) {
      return 1;
    }
    lstart = p + 1;
  }
}
