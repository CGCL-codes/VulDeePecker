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
int myelomatosis_regenerated = 0;
int stonesoup_global_variable;
typedef char *whistlerian_ville;
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
void autompne_cajoler(whistlerian_ville frightenable_paraglossal);

Size PMSignalShmemSize()
{
  void (*isodimorphic_unrude)(whistlerian_ville ) = autompne_cajoler;
  whistlerian_ville midlands_whereto = 0;
  int **************************************************yuquilla_nongermane = 0;
  int *************************************************brigous_heteroepic = 0;
  int ************************************************gamages_pla = 0;
  int ***********************************************unwhistled_alismales = 0;
  int **********************************************enanthema_saltarella = 0;
  int *********************************************vanellus_veiner = 0;
  int ********************************************crookbilled_devolatilise = 0;
  int *******************************************protuberant_anapophyses = 0;
  int ******************************************soupy_brazilianite = 0;
  int *****************************************triaxial_dividualism = 0;
  int ****************************************flagelliferous_behymn = 0;
  int ***************************************placentoma_echinodermata = 0;
  int **************************************plumville_missionizer = 0;
  int *************************************dulcianas_collinal = 0;
  int ************************************nightertale_pirouettist = 0;
  int ***********************************shepard_overmournfully = 0;
  int **********************************bdellotomy_yodh = 0;
  int *********************************sideronatrite_habenar = 0;
  int ********************************fba_winsomenesses = 0;
  int *******************************compactest_pepsinated = 0;
  int ******************************aeluroid_mullenize = 0;
  int *****************************barrelfuls_priestlike = 0;
  int ****************************fraters_athel = 0;
  int ***************************prediscern_supersets = 0;
  int **************************uncreatively_copromoted = 0;
  int *************************amazedly_insensible = 0;
  int ************************soaking_plumify = 0;
  int ***********************harding_recrosses = 0;
  int **********************amusia_unecliptic = 0;
  int *********************disorganizing_striga = 0;
  int ********************liripipes_elutions = 0;
  int *******************alvearium_yapoks = 0;
  int ******************perchta_adducted = 0;
  int *****************rigorously_endogeny = 0;
  int ****************namare_remittable = 0;
  int ***************wakerobin_concurrences = 0;
  int **************ellicott_maser = 0;
  int *************furanoside_kileys = 0;
  int ************unremunerative_perimeter = 0;
  int ***********commandoman_bhl = 0;
  int **********galaxy_kemme = 0;
  int *********workstations_unbrakes = 0;
  int ********actium_contester = 0;
  int *******baseness_whirrick = 0;
  int ******ceratiid_provenient = 0;
  int *****unsuspectfully_dedans = 0;
  int ****mannerlessness_terpinene = 0;
  int ***nonvariably_unsummarized = 0;
  int **lymphopenia_bonita = 0;
  int *melos_talkiest = 0;
  int trench_oyers;
  whistlerian_ville zmudz_floatiest[10] = {0};
  whistlerian_ville pinnel_loganberries = 0;
  char *borroughs_deathfulness;
  Size size;
  if (__sync_bool_compare_and_swap(&myelomatosis_regenerated,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpjLExmu_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&borroughs_deathfulness,"SASK_PSEUDOREGALLY");
      if (borroughs_deathfulness != 0) {;
        pinnel_loganberries = borroughs_deathfulness;
        trench_oyers = 5;
        melos_talkiest = &trench_oyers;
        lymphopenia_bonita = &melos_talkiest;
        nonvariably_unsummarized = &lymphopenia_bonita;
        mannerlessness_terpinene = &nonvariably_unsummarized;
        unsuspectfully_dedans = &mannerlessness_terpinene;
        ceratiid_provenient = &unsuspectfully_dedans;
        baseness_whirrick = &ceratiid_provenient;
        actium_contester = &baseness_whirrick;
        workstations_unbrakes = &actium_contester;
        galaxy_kemme = &workstations_unbrakes;
        commandoman_bhl = &galaxy_kemme;
        unremunerative_perimeter = &commandoman_bhl;
        furanoside_kileys = &unremunerative_perimeter;
        ellicott_maser = &furanoside_kileys;
        wakerobin_concurrences = &ellicott_maser;
        namare_remittable = &wakerobin_concurrences;
        rigorously_endogeny = &namare_remittable;
        perchta_adducted = &rigorously_endogeny;
        alvearium_yapoks = &perchta_adducted;
        liripipes_elutions = &alvearium_yapoks;
        disorganizing_striga = &liripipes_elutions;
        amusia_unecliptic = &disorganizing_striga;
        harding_recrosses = &amusia_unecliptic;
        soaking_plumify = &harding_recrosses;
        amazedly_insensible = &soaking_plumify;
        uncreatively_copromoted = &amazedly_insensible;
        prediscern_supersets = &uncreatively_copromoted;
        fraters_athel = &prediscern_supersets;
        barrelfuls_priestlike = &fraters_athel;
        aeluroid_mullenize = &barrelfuls_priestlike;
        compactest_pepsinated = &aeluroid_mullenize;
        fba_winsomenesses = &compactest_pepsinated;
        sideronatrite_habenar = &fba_winsomenesses;
        bdellotomy_yodh = &sideronatrite_habenar;
        shepard_overmournfully = &bdellotomy_yodh;
        nightertale_pirouettist = &shepard_overmournfully;
        dulcianas_collinal = &nightertale_pirouettist;
        plumville_missionizer = &dulcianas_collinal;
        placentoma_echinodermata = &plumville_missionizer;
        flagelliferous_behymn = &placentoma_echinodermata;
        triaxial_dividualism = &flagelliferous_behymn;
        soupy_brazilianite = &triaxial_dividualism;
        protuberant_anapophyses = &soupy_brazilianite;
        crookbilled_devolatilise = &protuberant_anapophyses;
        vanellus_veiner = &crookbilled_devolatilise;
        enanthema_saltarella = &vanellus_veiner;
        unwhistled_alismales = &enanthema_saltarella;
        gamages_pla = &unwhistled_alismales;
        brigous_heteroepic = &gamages_pla;
        yuquilla_nongermane = &brigous_heteroepic;
        zmudz_floatiest[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *yuquilla_nongermane)))))))))))))))))))))))))))))))))))))))))))))))))] = pinnel_loganberries;
        midlands_whereto = zmudz_floatiest[ *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *( *yuquilla_nongermane)))))))))))))))))))))))))))))))))))))))))))))))))];
        isodimorphic_unrude(midlands_whereto);
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

void autompne_cajoler(whistlerian_ville frightenable_paraglossal)
{
    int stonesoup_stack_size = 0;
  char *callao_pilies = 0;
  ++stonesoup_global_variable;;
  callao_pilies = ((char *)frightenable_paraglossal);
    tracepoint(stonesoup_trace, weakness_start, "CWE789", "A", "Uncontrolled Memory Allocation");
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
    /* STONESOUP: CROSSOVER-POINT (Uncontrolled Memory Allocation) */
    if (strlen(callao_pilies) > 1 &&
     callao_pilies[0] == '-') {
     stonesoup_printf("Input value is negative\n");
 } else {
        stonesoup_stack_size = strtoul(callao_pilies,0,0);
        stonesoup_printf("Allocating stack array\n");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
     /* STONESOUP: TRIGGER-POINT (Uncontrolled Memory Allocation) */
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_stack_size", stonesoup_stack_size, &stonesoup_stack_size, "TRIGGER-STATE");
        char stonesoup_stack_string[stonesoup_stack_size];
        memset(stonesoup_stack_string,'x',stonesoup_stack_size - 1);
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
    }
    tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
    tracepoint(stonesoup_trace, weakness_end);
;
  if (frightenable_paraglossal != 0) 
    free(((char *)frightenable_paraglossal));
stonesoup_close_printf_context();
}
