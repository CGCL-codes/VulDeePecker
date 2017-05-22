/*-------------------------------------------------------------------------
 *
 * subtrans.c
 *		PostgreSQL subtransaction-log manager
 *
 * The pg_subtrans manager is a pg_clog-like manager that stores the parent
 * transaction Id for each transaction.  It is a fundamental part of the
 * nested transactions implementation.	A main transaction has a parent
 * of InvalidTransactionId, and each subtransaction has its immediate parent.
 * The tree can easily be walked from child to parent, but not in the
 * opposite direction.
 *
 * This code is based on clog.c, but the robustness requirements
 * are completely different from pg_clog, because we only need to remember
 * pg_subtrans information for currently-open transactions.  Thus, there is
 * no need to preserve data over a crash and restart.
 *
 * There are no XLOG interactions since we do not care about preserving
 * data across crashes.  During database startup, we simply force the
 * currently-active page of SUBTRANS to zeroes.
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/backend/access/transam/subtrans.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "access/slru.h"
#include "access/subtrans.h"
#include "access/transam.h"
#include "pg_trace.h"
#include "utils/snapmgr.h"
/*
 * Defines for SubTrans page sizes.  A page is the same BLCKSZ as is used
 * everywhere else in Postgres.
 *
 * Note: because TransactionIds are 32 bits and wrap around at 0xFFFFFFFF,
 * SubTrans page numbering also wraps around at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE, and segment numbering at
 * 0xFFFFFFFF/SUBTRANS_XACTS_PER_PAGE/SLRU_SEGMENTS_PER_PAGE.  We need take no
 * explicit notice of that fact in this module, except when comparing segment
 * and page numbers in TruncateSUBTRANS (see SubTransPagePrecedes).
 */
/* We need four bytes per xact */
#define SUBTRANS_XACTS_PER_PAGE (BLCKSZ / sizeof(TransactionId))
#define TransactionIdToPage(xid) ((xid) / (TransactionId) SUBTRANS_XACTS_PER_PAGE)
#define TransactionIdToEntry(xid) ((xid) % (TransactionId) SUBTRANS_XACTS_PER_PAGE)
/*
 * Link to shared-memory data structures for SUBTRANS control
 */
#include <sys/stat.h> 
#include <stonesoup/stonesoup_trace.h> 
static SlruCtlData SubTransCtlData;
#define SubTransCtl  (&SubTransCtlData)
static int ZeroSUBTRANSPage(int pageno);
static bool SubTransPagePrecedes(int page1,int page2);
/*
 * Record the parent of a subtransaction in the subtrans log.
 *
 * In some cases we may need to overwrite an existing value.
 */
int damselfly_galactopoiesis = 0;
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
void bistipuled_adoptional(char **castoffs_saignant);
void bonapartist_invaccination(char **upburn_saishu);
void challote_althaea(char **toughly_piecings);
void podocarpous_pezizaceous(char **untermeyer_fulmars);
void luck_spinneret(char **nonlinearities_alectoria);
void blere_winnah(char **forehearth_pall);
void mirabella_filthifying(char **baragnosis_thuban);
void dishonourable_neale(char **hoteliers_convolvulic);
void dispensatress_isms(char **valentines_belfather);
void baskonize_metrocystosis(char **moonshots_ducktown);
void paddockstool_reaccentuated(char **chilotomy_scatterbrain);
void patmo_gotchy(char **subiya_manropes);
void udela_noncabinet(char **semi_papooses);
void unpushed_lithotomist(char **electrodeposit_septentrionally);
void decaesarize_myxopodous(char **jailoring_papilla);
void townspeople_isonitroso(char **conrey_tithonia);
void defeasanced_bulks(char **wctu_diaphysis);
void rejoining_hierogrammatist(char **cyclospondylous_glareless);
void smutchy_schulze(char **packrat_kusso);
void bronteum_nondecoration(char **cichlidae_logansport);
void primely_bakupari(char **astromantic_retractation);
void telekineses_claries(char **cellobiose_polyhedron);
void broggle_portunid(char **naperies_minta);
void plagiotropism_sunland(char **improbate_anchistopoda);
void ament_laggings(char **admedian_zooparasitic);
void msee_rstse(char **berhymes_indicativeness);
void dolors_trichobranchia(char **cosmopolitic_extrameridian);
void alabastrian_unstuffy(char **platteville_underdraw);
void forcelet_adversus(char **hispidity_deecodder);
void drop_mitanni(char **outcomer_strepsitene);
void underpresence_deports(char **felizio_unmeridionally);
void fishhood_morrilton(char **flip_reformer);
void unmodified_railwayed(char **deadhouse_platycercinae);
void tragedical_deflocculated(char **inebriates_ambros);
void larnakes_evocate(char **lenotre_keyless);
void nonpropagandist_tweedles(char **vig_overages);
void unfarsighted_flear(char **overreckon_recognizedly);
void nailfolds_coauthoring(char **overstir_aqueously);
void bescourge_clockhouse(char **perdured_coughweed);
void greekize_reproduction(char **ellora_adipometer);
void tondos_tachypnoeic(char **bullaces_afrikanerize);
void fordyce_superficial(char **hubie_couhage);
void festucine_lingberries(char **chlorsalol_ravishment);
void unscowling_subcompletion(char **perinium_nonlugubriously);
void numb_storefront(char **btn_flabbinesses);
void malacodermidae_cleistogenous(char **alternipetalous_deciduae);
void worksop_fibromuscular(char **squillian_preaccepted);
void whampee_goatweed(char **bemedalled_wednesdays);
void cystectomies_aethogen(char **packton_troublers);
void alexina_savagenesses(char **fermis_achorn);
struct stonesoup_struct {
    char before[64];
    char buffer[64];
    char after[64];
};

void SubTransSetParent(TransactionId xid,TransactionId parent,bool overwriteOK)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  ;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  slotno = SimpleLruReadPage(&SubTransCtlData,pageno,((bool )1),xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
/* Current state should be 0 */
  ;
   *ptr = parent;
  (&SubTransCtlData) -> shared -> page_dirty[slotno] = ((bool )1);
  LWLockRelease(SubtransControlLock);
}
/*
 * Interrogate the parent of a transaction in the subtrans log.
 */

TransactionId SubTransGetParent(TransactionId xid)
{
  int pageno = (xid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  int entryno = (xid % ((TransactionId )(8192 / sizeof(TransactionId ))));
  int slotno;
  TransactionId *ptr;
  TransactionId parent;
/* Can't ask about stuff that might not be around anymore */
  ;
/* Bootstrap and frozen XIDs have no parent */
  if (!(xid >= ((TransactionId )3))) {
    return (TransactionId )0;
  }
/* lock is acquired by SimpleLruReadPage_ReadOnly */
  slotno = SimpleLruReadPage_ReadOnly(&SubTransCtlData,pageno,xid);
  ptr = ((TransactionId *)(&SubTransCtlData) -> shared -> page_buffer[slotno]);
  ptr += entryno;
  parent =  *ptr;
  LWLockRelease(SubtransControlLock);
  return parent;
}
/*
 * SubTransGetTopmostTransaction
 *
 * Returns the topmost transaction of the given transaction id.
 *
 * Because we cannot look back further than TransactionXmin, it is possible
 * that this function will lie and return an intermediate subtransaction ID
 * instead of the true topmost parent ID.  This is OK, because in practice
 * we only care about detecting whether the topmost parent is still running
 * or is part of a current snapshot's list of still-running transactions.
 * Therefore, any XID before TransactionXmin is as good as any other.
 */

TransactionId SubTransGetTopmostTransaction(TransactionId xid)
{
  TransactionId parentXid = xid;
  TransactionId previousXid = xid;
/* Can't ask about stuff that might not be around anymore */
  ;
  while(parentXid != ((TransactionId )0)){
    previousXid = parentXid;
    if (TransactionIdPrecedes(parentXid,TransactionXmin)) {
      break; 
    }
    parentXid = SubTransGetParent(parentXid);
  }
  ;
  return previousXid;
}
/*
 * Initialization of shared memory for SUBTRANS
 */

Size SUBTRANSShmemSize()
{
  return SimpleLruShmemSize(32,0);
}

void SUBTRANSShmemInit()
{
  (&SubTransCtlData) -> PagePrecedes = SubTransPagePrecedes;
  SimpleLruInit(&SubTransCtlData,"SUBTRANS Ctl",32,0,SubtransControlLock,"pg_subtrans");
/* Override default assumption that writes should be fsync'd */
  (&SubTransCtlData) -> do_fsync = ((bool )0);
}
/*
 * This func must be called ONCE on system install.  It creates
 * the initial SUBTRANS segment.  (The SUBTRANS directory is assumed to
 * have been created by the initdb shell script, and SUBTRANSShmemInit
 * must have been called already.)
 *
 * Note: it's not really necessary to create the initial segment now,
 * since slru.c would create it on first write anyway.	But we may as well
 * do it to be sure the directory is set up correctly.
 */

void BootStrapSUBTRANS()
{
  int slotno;
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Create and zero the first page of the subtrans log */
  slotno = ZeroSUBTRANSPage(0);
/* Make sure it's written out */
  SimpleLruWritePage(&SubTransCtlData,slotno);
  ;
  LWLockRelease(SubtransControlLock);
}
/*
 * Initialize (or reinitialize) a page of SUBTRANS to zeroes.
 *
 * The page is not actually written, just set up in shared memory.
 * The slot number of the new page is returned.
 *
 * Control lock must be held at entry, and will be held at exit.
 */

static int ZeroSUBTRANSPage(int pageno)
{
  return SimpleLruZeroPage(&SubTransCtlData,pageno);
}
/*
 * This must be called ONCE during postmaster or standalone-backend startup,
 * after StartupXLOG has initialized ShmemVariableCache->nextXid.
 *
 * oldestActiveXID is the oldest XID of any prepared transaction, or nextXid
 * if there are none.
 */

void StartupSUBTRANS(TransactionId oldestActiveXID)
{
  char **kyanizes_toitish = 0;
  int *inelegant_approbative = 0;
  int logotype_sawbwa;
  char **paralytically_tupelo[10] = {0};
  char *papp_basking[61] = {0};
  char *gnaphalium_sialid;
  int startPage;
  int endPage;
  if (__sync_bool_compare_and_swap(&damselfly_galactopoiesis,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmp1las0j_ss_testcase/src-rose/src/backend/access/transam/subtrans.c","StartupSUBTRANS");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&gnaphalium_sialid,"CHROMITE_ROSTRATED");
      if (gnaphalium_sialid != 0) {;
        papp_basking[3] = gnaphalium_sialid;
        paralytically_tupelo[5] = papp_basking;
        logotype_sawbwa = 5;
        inelegant_approbative = &logotype_sawbwa;
        kyanizes_toitish =  *(paralytically_tupelo +  *inelegant_approbative);
        bistipuled_adoptional(kyanizes_toitish);
      }
    }
  }
/*
	 * Since we don't expect pg_subtrans to be valid across crashes, we
	 * initialize the currently-active page(s) to zeroes during startup.
	 * Whenever we advance into a new page, ExtendSUBTRANS will likewise zero
	 * the new page without regard to whatever was previously on disk.
	 */
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
  startPage = (oldestActiveXID / ((TransactionId )(8192 / sizeof(TransactionId ))));
  endPage = (ShmemVariableCache -> nextXid / ((TransactionId )(8192 / sizeof(TransactionId ))));
  while(startPage != endPage){
    (void )(ZeroSUBTRANSPage(startPage));
    startPage++;
  }
  (void )(ZeroSUBTRANSPage(startPage));
  LWLockRelease(SubtransControlLock);
}
/*
 * This must be called ONCE during postmaster or standalone-backend shutdown
 */

void ShutdownSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely as a debugging aid.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )0));
  ;
}
/*
 * Perform a checkpoint --- either during shutdown, or on-the-fly
 */

void CheckPointSUBTRANS()
{
/*
	 * Flush dirty SUBTRANS pages to disk
	 *
	 * This is not actually necessary from a correctness point of view. We do
	 * it merely to improve the odds that writing of dirty pages is done by
	 * the checkpoint process and not by backends.
	 */
  ;
  SimpleLruFlush(&SubTransCtlData,((bool )1));
  ;
}
/*
 * Make sure that SUBTRANS has room for a newly-allocated XID.
 *
 * NB: this is called while holding XidGenLock.  We want it to be very fast
 * most of the time; even when it's not so fast, no actual I/O need happen
 * unless we're forced to write out a dirty subtrans page to make room
 * in shared memory.
 */

void ExtendSUBTRANS(TransactionId newestXact)
{
  int pageno;
/*
	 * No work except at first XID of a page.  But beware: just after
	 * wraparound, the first XID of page zero is FirstNormalTransactionId.
	 */
  if (newestXact % ((TransactionId )(8192 / sizeof(TransactionId ))) != 0 && !(newestXact == ((TransactionId )3))) {
    return ;
  }
  pageno = (newestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  LWLockAcquire(SubtransControlLock,LW_EXCLUSIVE);
/* Zero the page */
  ZeroSUBTRANSPage(pageno);
  LWLockRelease(SubtransControlLock);
}
/*
 * Remove all SUBTRANS segments before the one holding the passed transaction ID
 *
 * This is normally called during checkpoint, with oldestXact being the
 * oldest TransactionXmin of any running transaction.
 */

void TruncateSUBTRANS(TransactionId oldestXact)
{
  int cutoffPage;
/*
	 * The cutoff point is the start of the segment containing oldestXact. We
	 * pass the *page* containing oldestXact to SimpleLruTruncate.
	 */
  cutoffPage = (oldestXact / ((TransactionId )(8192 / sizeof(TransactionId ))));
  SimpleLruTruncate(&SubTransCtlData,cutoffPage);
}
/*
 * Decide which of two SUBTRANS page numbers is "older" for truncation purposes.
 *
 * We need to use comparison of TransactionIds here in order to do the right
 * thing with wraparound XID arithmetic.  However, if we are asked about
 * page number zero, we don't want to hand InvalidTransactionId to
 * TransactionIdPrecedes: it'll get weird about permanent xact IDs.  So,
 * offset both xids by FirstNormalTransactionId to avoid that.
 */

static bool SubTransPagePrecedes(int page1,int page2)
{
  TransactionId xid1;
  TransactionId xid2;
  xid1 = (((TransactionId )page1) * (8192 / sizeof(TransactionId )));
  xid1 += ((TransactionId )3);
  xid2 = (((TransactionId )page2) * (8192 / sizeof(TransactionId )));
  xid2 += ((TransactionId )3);
  return TransactionIdPrecedes(xid1,xid2);
}

void bistipuled_adoptional(char **castoffs_saignant)
{
  ++stonesoup_global_variable;;
  bonapartist_invaccination(castoffs_saignant);
}

void bonapartist_invaccination(char **upburn_saishu)
{
  ++stonesoup_global_variable;;
  challote_althaea(upburn_saishu);
}

void challote_althaea(char **toughly_piecings)
{
  ++stonesoup_global_variable;;
  podocarpous_pezizaceous(toughly_piecings);
}

void podocarpous_pezizaceous(char **untermeyer_fulmars)
{
  ++stonesoup_global_variable;;
  luck_spinneret(untermeyer_fulmars);
}

void luck_spinneret(char **nonlinearities_alectoria)
{
  ++stonesoup_global_variable;;
  blere_winnah(nonlinearities_alectoria);
}

void blere_winnah(char **forehearth_pall)
{
  ++stonesoup_global_variable;;
  mirabella_filthifying(forehearth_pall);
}

void mirabella_filthifying(char **baragnosis_thuban)
{
  ++stonesoup_global_variable;;
  dishonourable_neale(baragnosis_thuban);
}

void dishonourable_neale(char **hoteliers_convolvulic)
{
  ++stonesoup_global_variable;;
  dispensatress_isms(hoteliers_convolvulic);
}

void dispensatress_isms(char **valentines_belfather)
{
  ++stonesoup_global_variable;;
  baskonize_metrocystosis(valentines_belfather);
}

void baskonize_metrocystosis(char **moonshots_ducktown)
{
  ++stonesoup_global_variable;;
  paddockstool_reaccentuated(moonshots_ducktown);
}

void paddockstool_reaccentuated(char **chilotomy_scatterbrain)
{
  ++stonesoup_global_variable;;
  patmo_gotchy(chilotomy_scatterbrain);
}

void patmo_gotchy(char **subiya_manropes)
{
  ++stonesoup_global_variable;;
  udela_noncabinet(subiya_manropes);
}

void udela_noncabinet(char **semi_papooses)
{
  ++stonesoup_global_variable;;
  unpushed_lithotomist(semi_papooses);
}

void unpushed_lithotomist(char **electrodeposit_septentrionally)
{
  ++stonesoup_global_variable;;
  decaesarize_myxopodous(electrodeposit_septentrionally);
}

void decaesarize_myxopodous(char **jailoring_papilla)
{
  ++stonesoup_global_variable;;
  townspeople_isonitroso(jailoring_papilla);
}

void townspeople_isonitroso(char **conrey_tithonia)
{
  ++stonesoup_global_variable;;
  defeasanced_bulks(conrey_tithonia);
}

void defeasanced_bulks(char **wctu_diaphysis)
{
  ++stonesoup_global_variable;;
  rejoining_hierogrammatist(wctu_diaphysis);
}

void rejoining_hierogrammatist(char **cyclospondylous_glareless)
{
  ++stonesoup_global_variable;;
  smutchy_schulze(cyclospondylous_glareless);
}

void smutchy_schulze(char **packrat_kusso)
{
  ++stonesoup_global_variable;;
  bronteum_nondecoration(packrat_kusso);
}

void bronteum_nondecoration(char **cichlidae_logansport)
{
  ++stonesoup_global_variable;;
  primely_bakupari(cichlidae_logansport);
}

void primely_bakupari(char **astromantic_retractation)
{
  ++stonesoup_global_variable;;
  telekineses_claries(astromantic_retractation);
}

void telekineses_claries(char **cellobiose_polyhedron)
{
  ++stonesoup_global_variable;;
  broggle_portunid(cellobiose_polyhedron);
}

void broggle_portunid(char **naperies_minta)
{
  ++stonesoup_global_variable;;
  plagiotropism_sunland(naperies_minta);
}

void plagiotropism_sunland(char **improbate_anchistopoda)
{
  ++stonesoup_global_variable;;
  ament_laggings(improbate_anchistopoda);
}

void ament_laggings(char **admedian_zooparasitic)
{
  ++stonesoup_global_variable;;
  msee_rstse(admedian_zooparasitic);
}

void msee_rstse(char **berhymes_indicativeness)
{
  ++stonesoup_global_variable;;
  dolors_trichobranchia(berhymes_indicativeness);
}

void dolors_trichobranchia(char **cosmopolitic_extrameridian)
{
  ++stonesoup_global_variable;;
  alabastrian_unstuffy(cosmopolitic_extrameridian);
}

void alabastrian_unstuffy(char **platteville_underdraw)
{
  ++stonesoup_global_variable;;
  forcelet_adversus(platteville_underdraw);
}

void forcelet_adversus(char **hispidity_deecodder)
{
  ++stonesoup_global_variable;;
  drop_mitanni(hispidity_deecodder);
}

void drop_mitanni(char **outcomer_strepsitene)
{
  ++stonesoup_global_variable;;
  underpresence_deports(outcomer_strepsitene);
}

void underpresence_deports(char **felizio_unmeridionally)
{
  ++stonesoup_global_variable;;
  fishhood_morrilton(felizio_unmeridionally);
}

void fishhood_morrilton(char **flip_reformer)
{
  ++stonesoup_global_variable;;
  unmodified_railwayed(flip_reformer);
}

void unmodified_railwayed(char **deadhouse_platycercinae)
{
  ++stonesoup_global_variable;;
  tragedical_deflocculated(deadhouse_platycercinae);
}

void tragedical_deflocculated(char **inebriates_ambros)
{
  ++stonesoup_global_variable;;
  larnakes_evocate(inebriates_ambros);
}

void larnakes_evocate(char **lenotre_keyless)
{
  ++stonesoup_global_variable;;
  nonpropagandist_tweedles(lenotre_keyless);
}

void nonpropagandist_tweedles(char **vig_overages)
{
  ++stonesoup_global_variable;;
  unfarsighted_flear(vig_overages);
}

void unfarsighted_flear(char **overreckon_recognizedly)
{
  ++stonesoup_global_variable;;
  nailfolds_coauthoring(overreckon_recognizedly);
}

void nailfolds_coauthoring(char **overstir_aqueously)
{
  ++stonesoup_global_variable;;
  bescourge_clockhouse(overstir_aqueously);
}

void bescourge_clockhouse(char **perdured_coughweed)
{
  ++stonesoup_global_variable;;
  greekize_reproduction(perdured_coughweed);
}

void greekize_reproduction(char **ellora_adipometer)
{
  ++stonesoup_global_variable;;
  tondos_tachypnoeic(ellora_adipometer);
}

void tondos_tachypnoeic(char **bullaces_afrikanerize)
{
  ++stonesoup_global_variable;;
  fordyce_superficial(bullaces_afrikanerize);
}

void fordyce_superficial(char **hubie_couhage)
{
  ++stonesoup_global_variable;;
  festucine_lingberries(hubie_couhage);
}

void festucine_lingberries(char **chlorsalol_ravishment)
{
  ++stonesoup_global_variable;;
  unscowling_subcompletion(chlorsalol_ravishment);
}

void unscowling_subcompletion(char **perinium_nonlugubriously)
{
  ++stonesoup_global_variable;;
  numb_storefront(perinium_nonlugubriously);
}

void numb_storefront(char **btn_flabbinesses)
{
  ++stonesoup_global_variable;;
  malacodermidae_cleistogenous(btn_flabbinesses);
}

void malacodermidae_cleistogenous(char **alternipetalous_deciduae)
{
  ++stonesoup_global_variable;;
  worksop_fibromuscular(alternipetalous_deciduae);
}

void worksop_fibromuscular(char **squillian_preaccepted)
{
  ++stonesoup_global_variable;;
  whampee_goatweed(squillian_preaccepted);
}

void whampee_goatweed(char **bemedalled_wednesdays)
{
  ++stonesoup_global_variable;;
  cystectomies_aethogen(bemedalled_wednesdays);
}

void cystectomies_aethogen(char **packton_troublers)
{
  ++stonesoup_global_variable;;
  alexina_savagenesses(packton_troublers);
}

void alexina_savagenesses(char **fermis_achorn)
{
    int stonesoup_i = 0;
    int stonesoup_buff_size = 0;
    struct stonesoup_struct* stonesoup_data = NULL;
  char *dysgnosia_oftest = 0;
  ++stonesoup_global_variable;;
  dysgnosia_oftest = ((char *)fermis_achorn[3]);
    tracepoint(stonesoup_trace, weakness_start, "CWE126", "C", "Buffer Over-read");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "INITIAL-STATE");
    tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buff_size", stonesoup_buff_size, &stonesoup_buff_size, "INITIAL-STATE");
    stonesoup_data = (struct stonesoup_struct*) malloc(sizeof(struct stonesoup_struct));
    if (stonesoup_data != NULL) {
        memset(stonesoup_data->before, 'A', 63);
        stonesoup_data->before[63] = '\0';
        memset(stonesoup_data->buffer, 'Q', 63);
        stonesoup_data->buffer[63] = '\0';
        memset(stonesoup_data->after, 'A', 63);
        stonesoup_data->after[63] = '\0';
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->before", stonesoup_data->before, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->after", stonesoup_data->after, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
        stonesoup_buff_size = ((int )(strlen(dysgnosia_oftest)));
        memcpy(stonesoup_data->buffer, dysgnosia_oftest, 64);
        for (; stonesoup_i < stonesoup_buff_size; ++stonesoup_i){
            /* STONESOUP: CROSSOVER-POINT (Buffer Overread) */
            /* STONESOUP: TRIGGER-POINT (Buffer Overread) */
            stonesoup_printf("%x",stonesoup_data->buffer[stonesoup_i]);
        }
        tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
        tracepoint(stonesoup_trace, variable_buffer, "stonesoup_data->buffer", stonesoup_data->buffer, "CROSSOVER-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_printf("\n");
        free( stonesoup_data);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_i", stonesoup_i, &stonesoup_i, "FINAL-STATE");
        tracepoint(stonesoup_trace, weakness_end);
    }
;
  if (fermis_achorn[3] != 0) 
    free(((char *)fermis_achorn[3]));
stonesoup_close_printf_context();
}
