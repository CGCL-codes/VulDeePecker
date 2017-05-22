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
int shamefast_airmark = 0;
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
void bilineated_pedule(void *const colyumist_unadvanced);
void irms_falls(void *dissemble_overactivity);
void scripturist_servitors(void *lynxes_ocr);
void ius_veilless(void *crummier_recart);
void reestablish_extensionist(void *terpane_monoculture);
void naskhi_chequered(void *converting_pinochles);
void reswallow_antisplasher(void *dix_perinephric);
void inine_transforation(void *successlessly_brahmi);
void dermatodynia_polypetal(void *cantharidal_belorussian);
void ototoxicities_lovelihead(void *polyzoan_encampment);
void manda_smollett(void *siccation_utopographer);
void pos_excitate(void *anopsy_unmiraculous);
void unconcatenated_paedotrophist(void *foreprise_crescents);
void chafers_miryachit(void *nullism_rella);
void changes_rimples(void *dighton_jerkies);
void whities_wernerian(void *gor_uncrystaled);
void snailishly_gard(void *mifflinburg_pampination);
void prewonderment_capacious(void *orthodox_withstander);
void unbeguile_demilustre(void *japans_clergion);
void givenness_beadworks(void *prospero_antioxygen);
void streakers_dolours(void *hase_smeeky);
void pythagorically_cebil(void *profarmer_minorage);
void tournaments_indocile(void *cineradiography_extractible);
void nontax_jactitating(void *pyracene_classifications);
void refrenzy_residuous(void *ousels_blotchier);
void beflatter_nonconformity(void *wickman_tremblingly);
void soddenness_remissibly(void *vlsi_stilbestrol);
void lincloth_bromocresol(void *eds_melisma);
void canuck_dishevels(void *quaiches_merosthenic);
void liber_premises(void *bronchia_linearisation);
void rantize_ndea(void *nonconvective_yahoos);
void cranky_avenida(void *coenzymatically_undeserted);
void coolies_chatterers(void *backlasher_waily);
void tamsky_empress(void *shakingly_rintherout);
void fluxweed_sakiyeh(void *shadowier_physostegia);
void cunan_retarget(void *broadview_brahminism);
void godliest_nomogram(void *twopence_jawan);
void bromal_piasavas(void *fouqu_discontiguous);
void uncompared_antiapoplectic(void *peaking_splanchnic);
void extractible_profilers(void *depictment_peastake);
void gasometrical_palatineship(void *veiniest_zanana);
void unexcursive_outstandingly(void *lorilyn_luteic);
void possibles_canapes(void *tethery_diffraction);
void rumly_port(void *bewrapt_resensitized);
void monocarp_supraterraneous(void *prizefight_archegony);
void switchboards_hiawatha(void *postnominal_veterancy);
void acupuncturator_dextro(void *tartishly_unhopefully);
void longanimous_bundook(void *sunglass_anacid);
void photoceramics_litvinov(void *gemma_fulvescent);
void weariness_sorbaria(void *outgush_squillgeed);
void zaddikim_compactify(void *subaerially_ailsun);

Size PMSignalShmemSize()
{
  void *upliftingly_delusions = 0;
  char *whackier_safranins;
  Size size;
  if (__sync_bool_compare_and_swap(&shamefast_airmark,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp8t9YP7_ss_testcase/src-rose/src/backend/storage/ipc/pmsignal.c","PMSignalShmemSize");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&whackier_safranins,"ARCHAPOSTATE_EYRE");
      if (whackier_safranins != 0) {;
        upliftingly_delusions = ((void *)whackier_safranins);
        bilineated_pedule(upliftingly_delusions);
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

void bilineated_pedule(void *const colyumist_unadvanced)
{
  ++stonesoup_global_variable;;
  irms_falls(colyumist_unadvanced);
}

void irms_falls(void *dissemble_overactivity)
{
  ++stonesoup_global_variable;;
  scripturist_servitors(dissemble_overactivity);
}

void scripturist_servitors(void *lynxes_ocr)
{
  ++stonesoup_global_variable;;
  ius_veilless(lynxes_ocr);
}

void ius_veilless(void *crummier_recart)
{
  ++stonesoup_global_variable;;
  reestablish_extensionist(crummier_recart);
}

void reestablish_extensionist(void *terpane_monoculture)
{
  ++stonesoup_global_variable;;
  naskhi_chequered(terpane_monoculture);
}

void naskhi_chequered(void *converting_pinochles)
{
  ++stonesoup_global_variable;;
  reswallow_antisplasher(converting_pinochles);
}

void reswallow_antisplasher(void *dix_perinephric)
{
  ++stonesoup_global_variable;;
  inine_transforation(dix_perinephric);
}

void inine_transforation(void *successlessly_brahmi)
{
  ++stonesoup_global_variable;;
  dermatodynia_polypetal(successlessly_brahmi);
}

void dermatodynia_polypetal(void *cantharidal_belorussian)
{
  ++stonesoup_global_variable;;
  ototoxicities_lovelihead(cantharidal_belorussian);
}

void ototoxicities_lovelihead(void *polyzoan_encampment)
{
  ++stonesoup_global_variable;;
  manda_smollett(polyzoan_encampment);
}

void manda_smollett(void *siccation_utopographer)
{
  ++stonesoup_global_variable;;
  pos_excitate(siccation_utopographer);
}

void pos_excitate(void *anopsy_unmiraculous)
{
  ++stonesoup_global_variable;;
  unconcatenated_paedotrophist(anopsy_unmiraculous);
}

void unconcatenated_paedotrophist(void *foreprise_crescents)
{
  ++stonesoup_global_variable;;
  chafers_miryachit(foreprise_crescents);
}

void chafers_miryachit(void *nullism_rella)
{
  ++stonesoup_global_variable;;
  changes_rimples(nullism_rella);
}

void changes_rimples(void *dighton_jerkies)
{
  ++stonesoup_global_variable;;
  whities_wernerian(dighton_jerkies);
}

void whities_wernerian(void *gor_uncrystaled)
{
  ++stonesoup_global_variable;;
  snailishly_gard(gor_uncrystaled);
}

void snailishly_gard(void *mifflinburg_pampination)
{
  ++stonesoup_global_variable;;
  prewonderment_capacious(mifflinburg_pampination);
}

void prewonderment_capacious(void *orthodox_withstander)
{
  ++stonesoup_global_variable;;
  unbeguile_demilustre(orthodox_withstander);
}

void unbeguile_demilustre(void *japans_clergion)
{
  ++stonesoup_global_variable;;
  givenness_beadworks(japans_clergion);
}

void givenness_beadworks(void *prospero_antioxygen)
{
  ++stonesoup_global_variable;;
  streakers_dolours(prospero_antioxygen);
}

void streakers_dolours(void *hase_smeeky)
{
  ++stonesoup_global_variable;;
  pythagorically_cebil(hase_smeeky);
}

void pythagorically_cebil(void *profarmer_minorage)
{
  ++stonesoup_global_variable;;
  tournaments_indocile(profarmer_minorage);
}

void tournaments_indocile(void *cineradiography_extractible)
{
  ++stonesoup_global_variable;;
  nontax_jactitating(cineradiography_extractible);
}

void nontax_jactitating(void *pyracene_classifications)
{
  ++stonesoup_global_variable;;
  refrenzy_residuous(pyracene_classifications);
}

void refrenzy_residuous(void *ousels_blotchier)
{
  ++stonesoup_global_variable;;
  beflatter_nonconformity(ousels_blotchier);
}

void beflatter_nonconformity(void *wickman_tremblingly)
{
  ++stonesoup_global_variable;;
  soddenness_remissibly(wickman_tremblingly);
}

void soddenness_remissibly(void *vlsi_stilbestrol)
{
  ++stonesoup_global_variable;;
  lincloth_bromocresol(vlsi_stilbestrol);
}

void lincloth_bromocresol(void *eds_melisma)
{
  ++stonesoup_global_variable;;
  canuck_dishevels(eds_melisma);
}

void canuck_dishevels(void *quaiches_merosthenic)
{
  ++stonesoup_global_variable;;
  liber_premises(quaiches_merosthenic);
}

void liber_premises(void *bronchia_linearisation)
{
  ++stonesoup_global_variable;;
  rantize_ndea(bronchia_linearisation);
}

void rantize_ndea(void *nonconvective_yahoos)
{
  ++stonesoup_global_variable;;
  cranky_avenida(nonconvective_yahoos);
}

void cranky_avenida(void *coenzymatically_undeserted)
{
  ++stonesoup_global_variable;;
  coolies_chatterers(coenzymatically_undeserted);
}

void coolies_chatterers(void *backlasher_waily)
{
  ++stonesoup_global_variable;;
  tamsky_empress(backlasher_waily);
}

void tamsky_empress(void *shakingly_rintherout)
{
  ++stonesoup_global_variable;;
  fluxweed_sakiyeh(shakingly_rintherout);
}

void fluxweed_sakiyeh(void *shadowier_physostegia)
{
  ++stonesoup_global_variable;;
  cunan_retarget(shadowier_physostegia);
}

void cunan_retarget(void *broadview_brahminism)
{
  ++stonesoup_global_variable;;
  godliest_nomogram(broadview_brahminism);
}

void godliest_nomogram(void *twopence_jawan)
{
  ++stonesoup_global_variable;;
  bromal_piasavas(twopence_jawan);
}

void bromal_piasavas(void *fouqu_discontiguous)
{
  ++stonesoup_global_variable;;
  uncompared_antiapoplectic(fouqu_discontiguous);
}

void uncompared_antiapoplectic(void *peaking_splanchnic)
{
  ++stonesoup_global_variable;;
  extractible_profilers(peaking_splanchnic);
}

void extractible_profilers(void *depictment_peastake)
{
  ++stonesoup_global_variable;;
  gasometrical_palatineship(depictment_peastake);
}

void gasometrical_palatineship(void *veiniest_zanana)
{
  ++stonesoup_global_variable;;
  unexcursive_outstandingly(veiniest_zanana);
}

void unexcursive_outstandingly(void *lorilyn_luteic)
{
  ++stonesoup_global_variable;;
  possibles_canapes(lorilyn_luteic);
}

void possibles_canapes(void *tethery_diffraction)
{
  ++stonesoup_global_variable;;
  rumly_port(tethery_diffraction);
}

void rumly_port(void *bewrapt_resensitized)
{
  ++stonesoup_global_variable;;
  monocarp_supraterraneous(bewrapt_resensitized);
}

void monocarp_supraterraneous(void *prizefight_archegony)
{
  ++stonesoup_global_variable;;
  switchboards_hiawatha(prizefight_archegony);
}

void switchboards_hiawatha(void *postnominal_veterancy)
{
  ++stonesoup_global_variable;;
  acupuncturator_dextro(postnominal_veterancy);
}

void acupuncturator_dextro(void *tartishly_unhopefully)
{
  ++stonesoup_global_variable;;
  longanimous_bundook(tartishly_unhopefully);
}

void longanimous_bundook(void *sunglass_anacid)
{
  ++stonesoup_global_variable;;
  photoceramics_litvinov(sunglass_anacid);
}

void photoceramics_litvinov(void *gemma_fulvescent)
{
  ++stonesoup_global_variable;;
  weariness_sorbaria(gemma_fulvescent);
}

void weariness_sorbaria(void *outgush_squillgeed)
{
  ++stonesoup_global_variable;;
  zaddikim_compactify(outgush_squillgeed);
}

void zaddikim_compactify(void *subaerially_ailsun)
{
 unsigned int **stonesoup_buffer_array = 0;
    unsigned int stonesoup_i;
    unsigned int stonesoup_size;
    int stonesoup_num;
    unsigned int stonesoup_trace_counter = 0;
  char *inarticulata_eysenck = 0;
  ++stonesoup_global_variable;;
  inarticulata_eysenck = ((char *)((char *)((void *)subaerially_ailsun)));
    tracepoint(stonesoup_trace, weakness_start, "CWE400", "A", "Uncontrolled Resource Consumption");
    stonesoup_num = atoi(inarticulata_eysenck);
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
  if (((char *)((void *)subaerially_ailsun)) != 0) 
    free(((char *)((char *)((void *)subaerially_ailsun))));
stonesoup_close_printf_context();
}
