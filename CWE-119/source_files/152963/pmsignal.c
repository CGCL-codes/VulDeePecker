/*-------------------------------------------------------------------------
 *
 * pmsignal.c
 *	  routines for signaling the postmaster from its child processes
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/storage/ipc/pmsignal.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include <signal.h>
#include <unistd.h>
#include "miscadmin.h"
#include "postmaster/postmaster.h"
#include "replication/walsender.h"
#include "storage/pmsignal.h"
#include "storage/shmem.h"
/*
 * The postmaster is signaled by its children by sending SIGUSR1.  The
 * specific reason is communicated via flags in shared memory.	We keep
 * a boolean flag for each possible "reason", so that different reasons
 * can be signaled by different backends at the same time.	(However,
 * if the same reason is signaled more than once simultaneously, the
 * postmaster will observe it only once.)
 *
 * The flags are actually declared as "volatile sig_atomic_t" for maximum
 * portability.  This should ensure that loads and stores of the flag
 * values are atomic, allowing us to dispense with any explicit locking.
 *
 * In addition to the per-reason flags, we store a set of per-child-process
 * flags that are currently used only for detecting whether a backend has
 * exited without performing proper shutdown.  The per-child-process flags
 * have three possible states: UNUSED, ASSIGNED, ACTIVE.  An UNUSED slot is
 * available for assignment.  An ASSIGNED slot is associated with a postmaster
 * child process, but either the process has not touched shared memory yet,
 * or it has successfully cleaned up after itself.	A ACTIVE slot means the
 * process is actively using shared memory.  The slots are assigned to
 * child processes at random, and postmaster.c is responsible for tracking
 * which one goes with which PID.
 *
 * Actually there is a fourth state, WALSENDER.  This is just like ACTIVE,
 * but carries the extra information that the child is a WAL sender.
 * WAL senders too start in ACTIVE state, but switch to WALSENDER once they
 * start streaming the WAL (and they never go back to ACTIVE after that).
 */
#define PM_CHILD_UNUSED		0	/* these values must fit in sig_atomic_t */
#define PM_CHILD_ASSIGNED	1
#define PM_CHILD_ACTIVE		2
#define PM_CHILD_WALSENDER	3
/* "typedef struct PMSignalData PMSignalData" appears in pmsignal.h */
#include <sys/stat.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 

struct PMSignalData 
{
/* per-reason flags */
  sig_atomic_t PMSignalFlags[NUM_PMSIGNALS];
/* per-child-process flags */
/* # of entries in PMChildFlags[] */
  int num_child_flags;
/* next slot to try to assign */
  int next_child_flag;
/* VARIABLE LENGTH ARRAY */
  sig_atomic_t PMChildFlags[1];
}
;
static volatile PMSignalData *PMSignalState = ((void *)0);
/*
 * PMSignalShmemSize
 *		Compute space needed for pmsignal.c's shared memory
 */
int prionodont_meable = 0;
int stonesoup_global_variable;
typedef char *wedlocks_enamine;
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
void ualis_ellisville(wedlocks_enamine **************************************************enthrong_respectably);
int stonesoup_toupper(int c) {
    if (c >= 97 && c <= 122) {
        return c - 32;
    }
    return c;
}

Size PMSignalShmemSize()
{
  void (*gratulated_curney)(wedlocks_enamine **************************************************) = ualis_ellisville;
  wedlocks_enamine **************************************************druidesses_smdr = 0;
  wedlocks_enamine *************************************************wishek_apomorphin = 0;
  wedlocks_enamine ************************************************leviticus_sarada = 0;
  wedlocks_enamine ***********************************************unthinking_steven = 0;
  wedlocks_enamine **********************************************atterminement_talca = 0;
  wedlocks_enamine *********************************************pachypod_stork = 0;
  wedlocks_enamine ********************************************rathite_euphonically = 0;
  wedlocks_enamine *******************************************melodram_inhere = 0;
  wedlocks_enamine ******************************************counterpray_exodermal = 0;
  wedlocks_enamine *****************************************abstersion_monika = 0;
  wedlocks_enamine ****************************************triflorate_donsy = 0;
  wedlocks_enamine ***************************************disadvance_acholoe = 0;
  wedlocks_enamine **************************************kefifrel_corseting = 0;
  wedlocks_enamine *************************************appurtenance_augmentedly = 0;
  wedlocks_enamine ************************************telfords_prenebular = 0;
  wedlocks_enamine ***********************************hylobatic_kautsky = 0;
  wedlocks_enamine **********************************marsilia_hydrolyze = 0;
  wedlocks_enamine *********************************empetraceae_botels = 0;
  wedlocks_enamine ********************************denouements_lichees = 0;
  wedlocks_enamine *******************************othilia_retranslated = 0;
  wedlocks_enamine ******************************pardner_inviscidity = 0;
  wedlocks_enamine *****************************bigamously_temp = 0;
  wedlocks_enamine ****************************ashikaga_bantry = 0;
  wedlocks_enamine ***************************bastard_previgilance = 0;
  wedlocks_enamine **************************allumine_behah = 0;
  wedlocks_enamine *************************unwanted_unhatcheled = 0;
  wedlocks_enamine ************************referentially_decipherment = 0;
  wedlocks_enamine ***********************polyzoan_nadja = 0;
  wedlocks_enamine **********************gertrud_residencer = 0;
  wedlocks_enamine *********************colback_armies = 0;
  wedlocks_enamine ********************puplike_torments = 0;
  wedlocks_enamine *******************austrian_toked = 0;
  wedlocks_enamine ******************unmunched_sperable = 0;
  wedlocks_enamine *****************reincidency_fanon = 0;
  wedlocks_enamine ****************discorporate_oscheoplasty = 0;
  wedlocks_enamine ***************aubusson_ginkgos = 0;
  wedlocks_enamine **************monascidian_eyases = 0;
  wedlocks_enamine *************unnarcotic_fingerprinted = 0;
  wedlocks_enamine ************also_illiquid = 0;
  wedlocks_enamine ***********mammee_unprecipitantly = 0;
  wedlocks_enamine **********legendarily_anmia = 0;
  wedlocks_enamine *********pharmacopeial_deliberatively = 0;
  wedlocks_enamine ********tackers_bedazzling = 0;
  wedlocks_enamine *******empexa_definite = 0;
  wedlocks_enamine ******calloused_chefrinia = 0;
  wedlocks_enamine *****accessively_gloat = 0;
  wedlocks_enamine ****kanari_phratries = 0;
  wedlocks_enamine ***fibularia_noradrenergic = 0;
  wedlocks_enamine **dermatatrophia_fastland = 0;
  wedlocks_enamine *synapticulate_ultrastellar = 0;
  wedlocks_enamine apicilar_perspicable = 0;
  wedlocks_enamine victorianize_squillitic = 0;
  int morita_fortescue = 1001;
  char *bystander_salsas;
  Size size;
  if (__sync_bool_compare_and_swap(&prionodont_meable,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpO89o3k_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&bystander_salsas,"1088",morita_fortescue);
      if (bystander_salsas != 0) {;
        victorianize_squillitic = bystander_salsas;
        synapticulate_ultrastellar = &victorianize_squillitic;
        dermatatrophia_fastland = &synapticulate_ultrastellar;
        fibularia_noradrenergic = &dermatatrophia_fastland;
        kanari_phratries = &fibularia_noradrenergic;
        accessively_gloat = &kanari_phratries;
        calloused_chefrinia = &accessively_gloat;
        empexa_definite = &calloused_chefrinia;
        tackers_bedazzling = &empexa_definite;
        pharmacopeial_deliberatively = &tackers_bedazzling;
        legendarily_anmia = &pharmacopeial_deliberatively;
        mammee_unprecipitantly = &legendarily_anmia;
        also_illiquid = &mammee_unprecipitantly;
        unnarcotic_fingerprinted = &also_illiquid;
        monascidian_eyases = &unnarcotic_fingerprinted;
        aubusson_ginkgos = &monascidian_eyases;
        discorporate_oscheoplasty = &aubusson_ginkgos;
        reincidency_fanon = &discorporate_oscheoplasty;
        unmunched_sperable = &reincidency_fanon;
        austrian_toked = &unmunched_sperable;
        puplike_torments = &austrian_toked;
        colback_armies = &puplike_torments;
        gertrud_residencer = &colback_armies;
        polyzoan_nadja = &gertrud_residencer;
        referentially_decipherment = &polyzoan_nadja;
        unwanted_unhatcheled = &referentially_decipherment;
        allumine_behah = &unwanted_unhatcheled;
        bastard_previgilance = &allumine_behah;
        ashikaga_bantry = &bastard_previgilance;
        bigamously_temp = &ashikaga_bantry;
        pardner_inviscidity = &bigamously_temp;
        othilia_retranslated = &pardner_inviscidity;
        denouements_lichees = &othilia_retranslated;
        empetraceae_botels = &denouements_lichees;
        marsilia_hydrolyze = &empetraceae_botels;
        hylobatic_kautsky = &marsilia_hydrolyze;
        telfords_prenebular = &hylobatic_kautsky;
        appurtenance_augmentedly = &telfords_prenebular;
        kefifrel_corseting = &appurtenance_augmentedly;
        disadvance_acholoe = &kefifrel_corseting;
        triflorate_donsy = &disadvance_acholoe;
        abstersion_monika = &triflorate_donsy;
        counterpray_exodermal = &abstersion_monika;
        melodram_inhere = &counterpray_exodermal;
        rathite_euphonically = &melodram_inhere;
        pachypod_stork = &rathite_euphonically;
        atterminement_talca = &pachypod_stork;
        unthinking_steven = &atterminement_talca;
        leviticus_sarada = &unthinking_steven;
        wishek_apomorphin = &leviticus_sarada;
        druidesses_smdr = &wishek_apomorphin;
        gratulated_curney(druidesses_smdr);
      }
    }
  }
  size = ((size_t )(&((PMSignalData *)0) -> PMChildFlags));
  size = add_size(size,mul_size((MaxLivePostmasterChildren()),sizeof(sig_atomic_t )));
  return size;
}
/*
 * PMSignalShmemInit - initialize during shared-memory creation
 */

void PMSignalShmemInit()
{
  bool found;
  PMSignalState = ((PMSignalData *)(ShmemInitStruct("PMSignalState",PMSignalShmemSize(),&found)));
  if (!found) {
    do {
      void *_vstart = (void *)PMSignalState;
      int _val = 0;
      Size _len = PMSignalShmemSize();
      if ((((intptr_t )_vstart) & sizeof(long ) - 1) == 0 && (_len & sizeof(long ) - 1) == 0 && _val == 0 && _len <= 1024 && 1024 != 0) {
        long *_start = (long *)_vstart;
        long *_stop = (long *)(((char *)_start) + _len);
        while(_start < _stop)
           *(_start++) = 0;
      }
      else {
        memset(_vstart,_val,_len);
      }
    }while (0);
    PMSignalState -> num_child_flags = MaxLivePostmasterChildren();
  }
}
/*
 * SendPostmasterSignal - signal the postmaster from a child process
 */

void SendPostmasterSignal(PMSignalReason reason)
{
/* If called in a standalone backend, do nothing */
  if (!IsUnderPostmaster) {
    return ;
  }
/* Atomically set the proper flag */
  PMSignalState -> PMSignalFlags[reason] = ((bool )1);
/* Send signal to postmaster */
  kill(PostmasterPid,10);
}
/*
 * CheckPostmasterSignal - check to see if a particular reason has been
 * signaled, and clear the signal flag.  Should be called by postmaster
 * after receiving SIGUSR1.
 */

bool CheckPostmasterSignal(PMSignalReason reason)
{
/* Careful here --- don't clear flag if we haven't seen it set */
  if (PMSignalState -> PMSignalFlags[reason]) {
    PMSignalState -> PMSignalFlags[reason] = ((bool )0);
    return (bool )1;
  }
  return (bool )0;
}
/*
 * AssignPostmasterChildSlot - select an unused slot for a new postmaster
 * child process, and set its state to ASSIGNED.  Returns a slot number
 * (one to N).
 *
 * Only the postmaster is allowed to execute this routine, so we need no
 * special locking.
 */

int AssignPostmasterChildSlot()
{
  int slot = PMSignalState -> next_child_flag;
  int n;
/*
	 * Scan for a free slot.  We track the last slot assigned so as not to
	 * waste time repeatedly rescanning low-numbered slots.
	 */
  for (n = PMSignalState -> num_child_flags; n > 0; n--) {
    if (--slot < 0) {
      slot = PMSignalState -> num_child_flags - 1;
    }
    if (PMSignalState -> PMChildFlags[slot] == 0) {
      PMSignalState -> PMChildFlags[slot] = 1;
      PMSignalState -> next_child_flag = slot;
      return slot + 1;
    }
  }
/* Out of slots ... should never happen, else postmaster.c messed up */
  (elog_start("pmsignal.c",173,__func__) , elog_finish(21,"no free slots in PMChildFlags array"));
/* keep compiler quiet */
  return 0;
}
/*
 * ReleasePostmasterChildSlot - release a slot after death of a postmaster
 * child process.  This must be called in the postmaster process.
 *
 * Returns true if the slot had been in ASSIGNED state (the expected case),
 * false otherwise (implying that the child failed to clean itself up).
 */

bool ReleasePostmasterChildSlot(int slot)
{
  bool result;
  ;
  slot--;
/*
	 * Note: the slot state might already be unused, because the logic in
	 * postmaster.c is such that this might get called twice when a child
	 * crashes.  So we don't try to Assert anything about the state.
	 */
  result = (PMSignalState -> PMChildFlags[slot] == 1);
  PMSignalState -> PMChildFlags[slot] = 0;
  return result;
}
/*
 * IsPostmasterChildWalSender - check if given slot is in use by a
 * walsender process.
 */

bool IsPostmasterChildWalSender(int slot)
{
  ;
  slot--;
  if (PMSignalState -> PMChildFlags[slot] == 3) {
    return (bool )1;
  }
  else {
    return (bool )0;
  }
}
/*
 * MarkPostmasterChildActive - mark a postmaster child as about to begin
 * actively using shared memory.  This is called in the child process.
 */

void MarkPostmasterChildActive()
{
  int slot = MyPMChildSlot;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 2;
}
/*
 * MarkPostmasterChildWalSender - mark a postmaster child as a WAL sender
 * process.  This is called in the child process, sometime after marking the
 * child as active.
 */

void MarkPostmasterChildWalSender()
{
  int slot = MyPMChildSlot;
  ;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 3;
}
/*
 * MarkPostmasterChildInactive - mark a postmaster child as done using
 * shared memory.  This is called in the child process.
 */

void MarkPostmasterChildInactive()
{
  int slot = MyPMChildSlot;
  ;
  slot--;
  ;
  PMSignalState -> PMChildFlags[slot] = 1;
}
/*
 * PostmasterIsAlive - check whether postmaster process is still alive
 */

bool PostmasterIsAlive()
{
#ifndef WIN32
  char c;
  ssize_t rc;
  rc = read(postmaster_alive_fds[0],(&c),1);
  if (rc < 0) {
    if ( *__errno_location() == 11 ||  *__errno_location() == 11) {
      return (bool )1;
    }
    else {
      (elog_start("pmsignal.c",284,__func__) , elog_finish(21,"read on postmaster death monitoring pipe failed: %m"));
    }
  }
  else {
    if (rc > 0) {
      (elog_start("pmsignal.c",287,__func__) , elog_finish(21,"unexpected data in postmaster death monitoring pipe"));
    }
  }
  return (bool )0;
#else							/* WIN32 */
#endif   /* WIN32 */
}

void ualis_ellisville(wedlocks_enamine **************************************************enthrong_respectably)
{
    int stonesoup_oc_i = 0;
 char stonesoup_stack_buffer_64[64];
  char *fudgy_undecidedly = 0;
  ++stonesoup_global_variable;;
  fudgy_undecidedly = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *enthrong_respectably)))))))))))))))))))))))))))))))))))))))))))))))))));
    tracepoint(stonesoup_trace, weakness_start, "CWE120", "A", "Buffer Copy without Checking Size of Input");
 memset(stonesoup_stack_buffer_64,0,64);
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_address, "__builtin_return_address(0)", __builtin_return_address(0), "INITIAL-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
 /* STONESOUP: CROSSOVER-POINT (Unchecked buffer copy) */
 strcpy(stonesoup_stack_buffer_64,fudgy_undecidedly);
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, variable_address, "__builtin_return_address(0)", __builtin_return_address(0), "CROSSOVER-STATE");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    for (; stonesoup_oc_i < 64; ++stonesoup_oc_i) {
        stonesoup_stack_buffer_64[stonesoup_oc_i] = stonesoup_toupper(stonesoup_stack_buffer_64[stonesoup_oc_i]);
    }
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "FINAL-STATE");
    tracepoint(stonesoup_trace, variable_buffer, "stonesoup_stack_buffer_64", stonesoup_stack_buffer_64, "FINAL-STATE");
 stonesoup_printf("%s\n",stonesoup_stack_buffer_64);
    tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
    tracepoint(stonesoup_trace, weakness_end);
    /* STONESOUP: TRIGGER-POINT (Buffer Overflow: Unchecked stack buffer copy) */
    /* Trigger point occurs on function return. */
;
  if ( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *enthrong_respectably))))))))))))))))))))))))))))))))))))))))))))))))) != 0) 
    free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *enthrong_respectably))))))))))))))))))))))))))))))))))))))))))))))))))));
stonesoup_close_printf_context();
}
