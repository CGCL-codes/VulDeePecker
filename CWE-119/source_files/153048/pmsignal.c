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
int merula_fantoddish = 0;

struct watchword_mydriatine 
{
  char *figitidae_isseis;
  double rampire_ulvan;
  char *virginhead_preeruptively;
  char bilboes_restudies;
  int proplastid_sart;
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
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

Size PMSignalShmemSize()
{
    int stonesoup_i = 0;
    int stonesoup_buff_size = 63;
    int stonesoup_taint_len;
    char *stonesoup_heap_buff_64 = 0;
  char *splats_cleome = 0;
  int lubeck_moingwena;
  int stapedez_hygrometer;
  struct watchword_mydriatine **************************************************spaulding_housebote = 0;
  struct watchword_mydriatine *************************************************chubb_macroeconomic = 0;
  struct watchword_mydriatine ************************************************irriguousness_unspiritually = 0;
  struct watchword_mydriatine ***********************************************renoir_teethful = 0;
  struct watchword_mydriatine **********************************************fortressed_cataclysmically = 0;
  struct watchword_mydriatine *********************************************gasperi_mountant = 0;
  struct watchword_mydriatine ********************************************pulicine_gairfowl = 0;
  struct watchword_mydriatine *******************************************gct_fibro = 0;
  struct watchword_mydriatine ******************************************plexicose_asssembler = 0;
  struct watchword_mydriatine *****************************************swinishness_chrysatropic = 0;
  struct watchword_mydriatine ****************************************aani_smatters = 0;
  struct watchword_mydriatine ***************************************frenatae_sonneratiaceous = 0;
  struct watchword_mydriatine **************************************companionship_unponderable = 0;
  struct watchword_mydriatine *************************************denationalised_bard = 0;
  struct watchword_mydriatine ************************************unpadlocked_hydride = 0;
  struct watchword_mydriatine ***********************************epidaurus_quoitlike = 0;
  struct watchword_mydriatine **********************************codist_restoration = 0;
  struct watchword_mydriatine *********************************corruptive_elshin = 0;
  struct watchword_mydriatine ********************************deviances_winterizes = 0;
  struct watchword_mydriatine *******************************bielbrief_bend = 0;
  struct watchword_mydriatine ******************************rethreading_noosing = 0;
  struct watchword_mydriatine *****************************resuming_saye = 0;
  struct watchword_mydriatine ****************************toothily_evaporometer = 0;
  struct watchword_mydriatine ***************************solvently_sweeper = 0;
  struct watchword_mydriatine **************************fassbinder_aswithe = 0;
  struct watchword_mydriatine *************************swingel_interchurch = 0;
  struct watchword_mydriatine ************************diphasic_cosectarian = 0;
  struct watchword_mydriatine ***********************acaulose_baskonize = 0;
  struct watchword_mydriatine **********************yeowoman_homilies = 0;
  struct watchword_mydriatine *********************queersome_cornbin = 0;
  struct watchword_mydriatine ********************extradural_outright = 0;
  struct watchword_mydriatine *******************blockish_timberyard = 0;
  struct watchword_mydriatine ******************ssme_perplication = 0;
  struct watchword_mydriatine *****************insurrectionism_autocarpian = 0;
  struct watchword_mydriatine ****************suborbicularly_splendrous = 0;
  struct watchword_mydriatine ***************unlowered_tubicolous = 0;
  struct watchword_mydriatine **************gar_phlegmonous = 0;
  struct watchword_mydriatine *************culturological_pitomie = 0;
  struct watchword_mydriatine ************preremitted_costogenic = 0;
  struct watchword_mydriatine ***********liegely_proteosaurus = 0;
  struct watchword_mydriatine **********yowlring_embowed = 0;
  struct watchword_mydriatine *********holophrases_heracleidae = 0;
  struct watchword_mydriatine ********schmooze_dazed = 0;
  struct watchword_mydriatine *******coppered_aicc = 0;
  struct watchword_mydriatine ******cordeau_melatope = 0;
  struct watchword_mydriatine *****outvociferating_listerised = 0;
  struct watchword_mydriatine ****fermage_inseminators = 0;
  struct watchword_mydriatine ***contractive_foppy = 0;
  struct watchword_mydriatine **procambium_companionways = 0;
  struct watchword_mydriatine *expanse_deuterosy = 0;
  struct watchword_mydriatine twyers_toxinosis = {0};
  struct watchword_mydriatine overlaness_arist;
  int almeta_nondecadence = 596;
  char *gasped_melilites;
  Size size;
  if (__sync_bool_compare_and_swap(&merula_fantoddish,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp5iA15w_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&gasped_melilites,"1045",almeta_nondecadence);
      if (gasped_melilites != 0) {;
        overlaness_arist . figitidae_isseis = ((char *)gasped_melilites);
        expanse_deuterosy = &overlaness_arist;
        procambium_companionways = &expanse_deuterosy;
        contractive_foppy = &procambium_companionways;
        fermage_inseminators = &contractive_foppy;
        outvociferating_listerised = &fermage_inseminators;
        cordeau_melatope = &outvociferating_listerised;
        coppered_aicc = &cordeau_melatope;
        schmooze_dazed = &coppered_aicc;
        holophrases_heracleidae = &schmooze_dazed;
        yowlring_embowed = &holophrases_heracleidae;
        liegely_proteosaurus = &yowlring_embowed;
        preremitted_costogenic = &liegely_proteosaurus;
        culturological_pitomie = &preremitted_costogenic;
        gar_phlegmonous = &culturological_pitomie;
        unlowered_tubicolous = &gar_phlegmonous;
        suborbicularly_splendrous = &unlowered_tubicolous;
        insurrectionism_autocarpian = &suborbicularly_splendrous;
        ssme_perplication = &insurrectionism_autocarpian;
        blockish_timberyard = &ssme_perplication;
        extradural_outright = &blockish_timberyard;
        queersome_cornbin = &extradural_outright;
        yeowoman_homilies = &queersome_cornbin;
        acaulose_baskonize = &yeowoman_homilies;
        diphasic_cosectarian = &acaulose_baskonize;
        swingel_interchurch = &diphasic_cosectarian;
        fassbinder_aswithe = &swingel_interchurch;
        solvently_sweeper = &fassbinder_aswithe;
        toothily_evaporometer = &solvently_sweeper;
        resuming_saye = &toothily_evaporometer;
        rethreading_noosing = &resuming_saye;
        bielbrief_bend = &rethreading_noosing;
        deviances_winterizes = &bielbrief_bend;
        corruptive_elshin = &deviances_winterizes;
        codist_restoration = &corruptive_elshin;
        epidaurus_quoitlike = &codist_restoration;
        unpadlocked_hydride = &epidaurus_quoitlike;
        denationalised_bard = &unpadlocked_hydride;
        companionship_unponderable = &denationalised_bard;
        frenatae_sonneratiaceous = &companionship_unponderable;
        aani_smatters = &frenatae_sonneratiaceous;
        swinishness_chrysatropic = &aani_smatters;
        plexicose_asssembler = &swinishness_chrysatropic;
        gct_fibro = &plexicose_asssembler;
        pulicine_gairfowl = &gct_fibro;
        gasperi_mountant = &pulicine_gairfowl;
        fortressed_cataclysmically = &gasperi_mountant;
        renoir_teethful = &fortressed_cataclysmically;
        irriguousness_unspiritually = &renoir_teethful;
        chubb_macroeconomic = &irriguousness_unspiritually;
        spaulding_housebote = &chubb_macroeconomic;
        stapedez_hygrometer = 5;
        while(1 == 1){
          stapedez_hygrometer = stapedez_hygrometer * 2;
          stapedez_hygrometer = stapedez_hygrometer + 2;
          if (stapedez_hygrometer > 1000) {
            break; 
          }
        }
        lubeck_moingwena = stapedez_hygrometer;
        splats_cleome = ((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *spaulding_housebote)))))))))))))))))))))))))))))))))))))))))))))))))) . figitidae_isseis);
    tracepoint(stonesoup_trace, weakness_start, "CWE124", "B", "Buffer Underwrite");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_heap_buff_64 = (char*) malloc(64 * sizeof(char));
    if (stonesoup_heap_buff_64 != NULL) {
        memset(stonesoup_heap_buff_64, 'A' ,64);
        stonesoup_heap_buff_64[64 - 1] = '\0';
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_taint_len = ((int )(strlen(splats_cleome)));
        for (; stonesoup_taint_len >= 0; (--stonesoup_buff_size , --stonesoup_taint_len)) {
    /* STONESOUP: CROSSOVER-POINT (Buffer Underwrite) */
    /* STONESOUP: TRIGGER-POINT (Buffer Underwrite: Heap Underflow) */
            stonesoup_heap_buff_64[stonesoup_buff_size] = splats_cleome[stonesoup_taint_len];
        }
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "CROSSOVER-STATE");
        for (; stonesoup_i < 64; ++stonesoup_i) {
            stonesoup_heap_buff_64[stonesoup_i] = stonesoup_toupper(stonesoup_heap_buff_64[stonesoup_i]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("%s\n",&(stonesoup_heap_buff_64[stonesoup_buff_size+1]));
        free(stonesoup_heap_buff_64);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_heap_buff_64", stonesoup_heap_buff_64, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
        if (( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *spaulding_housebote)))))))))))))))))))))))))))))))))))))))))))))))))) . figitidae_isseis != 0) 
          free(((char *)( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *spaulding_housebote)))))))))))))))))))))))))))))))))))))))))))))))))) . figitidae_isseis));
stonesoup_close_printf_context();
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
