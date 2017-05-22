/* dfa.c - deterministic extended regexp routines for GNU
   Copyright (C) 1988, 1998, 2000, 2002, 2004-2005, 2007-2012 Free Software
   Foundation, Inc.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc.,
   51 Franklin Street - Fifth Floor, Boston, MA  02110-1301, USA */
/* Written June, 1988 by Mike Haertel
   Modified July, 1988 by Arthur David Olson to assist BMG speedups  */
#include <config.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#define STREQ(a, b) (strcmp (a, b) == 0)
/* ISASCIIDIGIT differs from isdigit, as follows:
   - Its arg may be any int or unsigned int; it need not be an unsigned char.
   - It's guaranteed to evaluate its argument exactly once.
   - It's typically faster.
   Posix 1003.2-1992 section 2.5.2.1 page 50 lines 1556-1558 says that
   only '0' through '9' are digits.  Prefer ISASCIIDIGIT to isdigit unless
   it's important to use the locale's definition of "digit" even when the
   host does not conform to Posix.  */
#define ISASCIIDIGIT(c) ((unsigned) (c) - '0' <= 9)
/* gettext.h ensures that we don't use gettext if ENABLE_NLS is not defined */
#include "gettext.h"
#define _(str) gettext (str)
#include "mbsupport.h"          /* defines MBS_SUPPORT if appropriate */
#include <wchar.h>
#include <wctype.h>
#if HAVE_LANGINFO_CODESET
# include <langinfo.h>
#endif
#include "regex.h"
#include "dfa.h"
#include "xalloc.h"
/* HPUX, define those as macros in sys/param.h */
#ifdef setbit
# undef setbit
#endif
#ifdef clrbit
# undef clrbit
#endif
/* Number of bits in an unsigned char. */
#ifndef CHARBITS
# define CHARBITS 8
#endif
/* First integer value that is greater than any character code. */
#define NOTCHAR (1 << CHARBITS)
/* INTBITS need not be exact, just a lower bound. */
#ifndef INTBITS
# define INTBITS (CHARBITS * sizeof (int))
#endif
/* Number of ints required to hold a bit for every character. */
#define CHARCLASS_INTS ((NOTCHAR + INTBITS - 1) / INTBITS)
/* Sets of unsigned characters are stored as bit vectors in arrays of ints. */
#include <sys/stat.h> 
#include <stdarg.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stonesoup/stonesoup_trace.h> 
#include <errno.h> 
typedef int charclass[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))];
/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
int extrajudicial_edrock = 0;
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
char stonesoup_process_buffer(char *buffer_param)
{
  tracepoint(stonesoup_trace, trace_location, "/tmp/tmpLkx1x__ss_testcase/src-rose/src/dfa.c", "stonesoup_process_buffer");
  char first_char;
  first_char = buffer_param[0] - 97;
  free(buffer_param);
  return first_char;
}
int stonesoup_toupper(int c)
{
  if (c >= 97 && c <= 122) {
    return c - 32;
  }
  return c;
}

inline static unsigned char to_uchar(char ch)
{
  return ch;
}
/* Contexts tell us whether a character is a newline or a word constituent.
   Word-constituent characters are those that satisfy iswalnum(), plus '_'.
   Each character has a single CTX_* value; bitmasks of CTX_* values denote
   a particular character class.
   A state also stores a context value, which is a bitmask of CTX_* values.
   A state's context represents a set of characters that the state's
   predecessors must match.  For example, a state whose context does not
   include CTX_LETTER will never have transitions where the previous
   character is a word constituent.  A state whose context is CTX_ANY
   might have transitions from any character.  */
#define CTX_NONE	1
#define CTX_LETTER	2
#define CTX_NEWLINE	4
#define CTX_ANY		7
/* Sometimes characters can only be matched depending on the surrounding
   context.  Such context decisions depend on what the previous character
   was, and the value of the current (lookahead) character.  Context
   dependent constraints are encoded as 8 bit integers.  Each bit that
   is set indicates that the constraint succeeds in the corresponding
   context.
   bit 8-11 - valid contexts when next character is CTX_NEWLINE
   bit 4-7  - valid contexts when next character is CTX_LETTER
   bit 0-3  - valid contexts when next character is CTX_NONE
   The macro SUCCEEDS_IN_CONTEXT determines whether a given constraint
   succeeds in a particular context.  Prev is a bitmask of possible
   context values for the previous character, curr is the (single-bit)
   context value for the lookahead character. */
#define NEWLINE_CONSTRAINT(constraint) (((constraint) >> 8) & 0xf)
#define LETTER_CONSTRAINT(constraint)  (((constraint) >> 4) & 0xf)
#define OTHER_CONSTRAINT(constraint)    ((constraint)       & 0xf)
#define SUCCEEDS_IN_CONTEXT(constraint, prev, curr) \
  ((((curr) & CTX_NONE      ? OTHER_CONSTRAINT(constraint) : 0) \
    | ((curr) & CTX_LETTER  ? LETTER_CONSTRAINT(constraint) : 0) \
    | ((curr) & CTX_NEWLINE ? NEWLINE_CONSTRAINT(constraint) : 0)) & (prev))
/* The following macros give information about what a constraint depends on. */
#define PREV_NEWLINE_CONSTRAINT(constraint) (((constraint) >> 2) & 0x111)
#define PREV_LETTER_CONSTRAINT(constraint)  (((constraint) >> 1) & 0x111)
#define PREV_OTHER_CONSTRAINT(constraint)    ((constraint)       & 0x111)
#define PREV_NEWLINE_DEPENDENT(constraint) \
  (PREV_NEWLINE_CONSTRAINT (constraint) != PREV_OTHER_CONSTRAINT (constraint))
#define PREV_LETTER_DEPENDENT(constraint) \
  (PREV_LETTER_CONSTRAINT (constraint) != PREV_OTHER_CONSTRAINT (constraint))
/* Tokens that match the empty string subject to some constraint actually
   work by applying that constraint to determine what may follow them,
   taking into account what has gone before.  The following values are
   the constraints corresponding to the special tokens previously defined. */
#define NO_CONSTRAINT         0x777
#define BEGLINE_CONSTRAINT    0x444
#define ENDLINE_CONSTRAINT    0x700
#define BEGWORD_CONSTRAINT    0x050
#define ENDWORD_CONSTRAINT    0x202
#define LIMWORD_CONSTRAINT    0x252
#define NOTLIMWORD_CONSTRAINT 0x525
/* The regexp is parsed into an array of tokens in postfix form.  Some tokens
   are operators and others are terminal symbols.  Most (but not all) of these
   codes are returned by the lexical analyzer. */
typedef ptrdiff_t token;
/* Predefined token values.  */
enum __anonymous_0x33cdfa0 {
/* END is a terminal symbol that matches the
                                   end of input; any value of END or less in
                                   the parse tree is such a symbol.  Accepting
                                   states of the DFA are those that would have
                                   a transition on END. */
END=-1,
/* Ordinary character values are terminal symbols that match themselves. */
/* EMPTY is a terminal symbol that matches
                                   the empty string. */
EMPTY=256,
/* BACKREF is generated by \<digit>; it
                                   is not completely handled.  If the scanner
                                   detects a transition on backref, it returns
                                   a kind of "semi-success" indicating that
                                   the match will have to be verified with
                                   a backtracking matcher. */
BACKREF=257,
/* BEGLINE is a terminal symbol that matches
                                   the empty string if it is at the beginning
                                   of a line. */
BEGLINE=258,
/* ENDLINE is a terminal symbol that matches
                                   the empty string if it is at the end of
                                   a line. */
ENDLINE=259,
/* BEGWORD is a terminal symbol that matches
                                   the empty string if it is at the beginning
                                   of a word. */
BEGWORD=260,
/* ENDWORD is a terminal symbol that matches
                                   the empty string if it is at the end of
                                   a word. */
ENDWORD=261,
/* LIMWORD is a terminal symbol that matches
                                   the empty string if it is at the beginning
                                   or the end of a word. */
LIMWORD=262,
/* NOTLIMWORD is a terminal symbol that
                                   matches the empty string if it is not at
                                   the beginning or end of a word. */
NOTLIMWORD=263,
/* QMARK is an operator of one argument that
                                   matches zero or one occurrences of its
                                   argument. */
QMARK=264,
/* STAR is an operator of one argument that
                                   matches the Kleene closure (zero or more
                                   occurrences) of its argument. */
STAR=265,
/* PLUS is an operator of one argument that
                                   matches the positive closure (one or more
                                   occurrences) of its argument. */
PLUS=266,
/* REPMN is a lexical token corresponding
                                   to the {m,n} construct.  REPMN never
                                   appears in the compiled token vector. */
REPMN=267,
/* CAT is an operator of two arguments that
                                   matches the concatenation of its
                                   arguments.  CAT is never returned by the
                                   lexical analyzer. */
CAT=268,
/* OR is an operator of two arguments that
                                   matches either of its arguments. */
OR=269,
/* LPAREN never appears in the parse tree,
                                   it is only a lexeme. */
LPAREN=270,
/* RPAREN never appears in the parse tree. */
RPAREN=271,
/* ANYCHAR is a terminal symbol that matches
                                   any multibyte (or single byte) characters.
                                   It is used only if MB_CUR_MAX > 1.  */
ANYCHAR=272,
/* MBCSET is similar to CSET, but for
                                   multibyte characters.  */
MBCSET=273,
/* Only returned by lex.  wctok contains
                                   the wide character representation.  */
WCHAR=274,
/* CSET and (and any value greater) is a
                                   terminal symbol that matches any of a
                                   class of characters. */
CSET=275} ;
/* States of the recognizer correspond to sets of positions in the parse
   tree, together with the constraints under which they may be matched.
   So a position is encoded as an index into the parse tree together with
   a constraint. */
typedef struct {
/* Index into the parse array. */
size_t index;
/* Constraint for matching this position. */
unsigned int constraint;}position;
/* Sets of positions are stored as arrays. */
typedef struct {
/* Elements of this position set. */
position *elems;
/* Number of elements in this set. */
size_t nelem;
/* Number of elements allocated in ELEMS.  */
size_t alloc;}position_set;
/* Sets of leaves are also stored as arrays. */
typedef struct {
/* Elements of this position set. */
size_t *elems;
/* Number of elements in this set. */
size_t nelem;}leaf_set;
/* A state of the dfa consists of a set of positions, some flags,
   and the token value of the lowest-numbered position of the state that
   contains an END token. */
typedef struct {
/* Hash of the positions of this state. */
size_t hash;
/* Positions this state could match. */
position_set elems;
/* Context from previous state. */
unsigned char context;
/* True if this state matches a \<digit>.  */
char backref;
/* Constraint for this state to accept. */
unsigned short constraint;
/* Token value of the first END in elems. */
token first_end;
/* Positions which can match multibyte
                                   characters.  e.g. period.
                                   These staff are used only if
                                   MB_CUR_MAX > 1.  */
position_set mbps;}dfa_state;
/* States are indexed by state_num values.  These are normally
   nonnegative but -1 is used as a special value.  */
typedef ptrdiff_t state_num;
/* A bracket operator.
   e.g. [a-c], [[:alpha:]], etc.  */

struct mb_char_classes 
{
  ptrdiff_t cset;
  int invert;
/* Normal characters.  */
  wchar_t *chars;
  size_t nchars;
/* Character classes.  */
  wctype_t *ch_classes;
  size_t nch_classes;
/* Range characters (start of the range).  */
  wchar_t *range_sts;
/* Range characters (end of the range).  */
  wchar_t *range_ends;
  size_t nranges;
/* Equivalence classes.  */
  char **equivs;
  size_t nequivs;
  char **coll_elems;
/* Collating elements.  */
  size_t ncoll_elems;
}
;
/* A compiled regular expression. */

struct dfa 
{
/* Fields filled by the scanner. */
/* Array of character sets for CSET tokens. */
  charclass *charclasses;
/* Index for adding new charclasses. */
  size_t cindex;
/* Number of charclasses currently allocated. */
  size_t calloc;
/* Fields filled by the parser. */
/* Postfix parse array. */
  token *tokens;
/* Index for adding new tokens. */
  size_t tindex;
/* Number of tokens currently allocated. */
  size_t talloc;
/* Depth required of an evaluation stack
                                   used for depth-first traversal of the
                                   parse tree. */
  size_t depth;
/* Number of leaves on the parse tree. */
  size_t nleaves;
/* Count of parallel regexps being built
                                   with dfaparse(). */
  size_t nregexps;
/* Cached value of MB_CUR_MAX.  */
  unsigned int mb_cur_max;
/* To lower ANYCHAR in UTF-8 locales.  */
  token utf8_anychar_classes[5];
/* The following are used only if MB_CUR_MAX > 1.  */
/* The value of multibyte_prop[i] is defined by following rule.
     if tokens[i] < NOTCHAR
     bit 0 : tokens[i] is the first byte of a character, including
     single-byte characters.
     bit 1 : tokens[i] is the last byte of a character, including
     single-byte characters.
     if tokens[i] = MBCSET
     ("the index of mbcsets corresponding to this operator" << 2) + 3
     e.g.
     tokens
     = 'single_byte_a', 'multi_byte_A', single_byte_b'
     = 'sb_a', 'mb_A(1st byte)', 'mb_A(2nd byte)', 'mb_A(3rd byte)', 'sb_b'
     multibyte_prop
     = 3     , 1               ,  0              ,  2              , 3
   */
  size_t nmultibyte_prop;
  int *multibyte_prop;
/* Array of the bracket expression in the DFA.  */
  struct mb_char_classes *mbcsets;
  size_t nmbcsets;
  size_t mbcsets_alloc;
/* Fields filled by the state builder. */
/* States of the dfa. */
  dfa_state *states;
/* Index for adding new states. */
  state_num sindex;
/* Number of states currently allocated. */
  state_num salloc;
/* Fields filled by the parse tree->NFA conversion. */
/* Array of follow sets, indexed by position
                                   index.  The follow of a position is the set
                                   of positions containing characters that
                                   could conceivably follow a character
                                   matching the given position in a string
                                   matching the regexp.  Allocated to the
                                   maximum possible position index. */
  position_set *follows;
/* True if we are supposed to build a searching
                                   as opposed to an exact matcher.  A searching
                                   matcher finds the first and shortest string
                                   matching a regexp anywhere in the buffer,
                                   whereas an exact matcher finds the longest
                                   string matching, but anchored to the
                                   beginning of the buffer. */
  int searchflag;
/* Fields filled by dfaexec. */
/* Number of transition tables that have
                                   slots so far. */
  state_num tralloc;
/* Number of transition tables that have
                                   actually been built. */
  int trcount;
/* Transition tables for states that can
                                   never accept.  If the transitions for a
                                   state have not yet been computed, or the
                                   state could possibly accept, its entry in
                                   this table is NULL. */
  state_num **trans;
/* Trans always points to realtrans + 1; this
                                   is so trans[-1] can contain NULL. */
  state_num **realtrans;
/* Transition tables after failing to accept
                                   on a state that potentially could do so. */
  state_num **fails;
/* Table of acceptance conditions used in
                                   dfaexec and computed in build_state. */
  int *success;
/* Transitions on newlines.  The entry for a
                                   newline in any transition table is always
                                   -1 so we can count lines without wasting
                                   too many cycles.  The transition for a
                                   newline is stored separately and handled
                                   as a special case.  Newline is also used
                                   as a sentinel at the end of the buffer. */
  state_num *newlines;
/* List of strings, at least one of which
                                   is known to appear in any r.e. matching
                                   the dfa. */
  struct dfamust *musts;
}
;
/* Some macros for user access to dfa internals. */
/* ACCEPTING returns true if s could possibly be an accepting state of r. */
#define ACCEPTING(s, r) ((r).states[s].constraint)
/* ACCEPTS_IN_CONTEXT returns true if the given state accepts in the
   specified context. */
#define ACCEPTS_IN_CONTEXT(prev, curr, state, dfa) \
  SUCCEEDS_IN_CONTEXT ((dfa).states[state].constraint, prev, curr)
static void dfamust(struct dfa *d);
static void regexp();
/* These two macros are identical to the ones in gnulib's xalloc.h,
   except that they not to case the result to "(t *)", and thus may
   be used via type-free CALLOC and MALLOC macros.  */
#undef XNMALLOC
#undef XCALLOC
/* Allocate memory for N elements of type T, with error checking.  */
/* extern t *XNMALLOC (size_t n, typename t); */
# define XNMALLOC(n, t) \
    (sizeof (t) == 1 ? xmalloc (n) : xnmalloc (n, sizeof (t)))
/* Allocate memory for N elements of type T, with error checking,
   and zero it.  */
/* extern t *XCALLOC (size_t n, typename t); */
# define XCALLOC(n, t) \
    (sizeof (t) == 1 ? xzalloc (n) : xcalloc (n, sizeof (t)))
#define CALLOC(p, n) do { (p) = XCALLOC (n, *(p)); } while (0)
#define MALLOC(p, n) do { (p) = XNMALLOC (n, *(p)); } while (0)
#define REALLOC(p, n) do {(p) = xnrealloc (p, n, sizeof (*(p))); } while (0)
/* Reallocate an array of type *P if N_ALLOC is <= N_REQUIRED. */
#define REALLOC_IF_NECESSARY(p, n_alloc, n_required)		\
  do								\
    {								\
      if ((n_alloc) <= (n_required))				\
        {							\
          size_t new_n_alloc = (n_required) + !(p);		\
          (p) = x2nrealloc (p, &new_n_alloc, sizeof (*(p)));	\
          (n_alloc) = new_n_alloc;				\
        }							\
    }								\
  while (false)
#ifdef DEBUG
#endif /* DEBUG */
/* Stuff pertaining to charclasses. */

static int tstbit(unsigned int b,const int c[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))])
{
  return c[b / (8 * sizeof(int ))] & 1 << b % (8 * sizeof(int ));
}

static void setbit(unsigned int b,charclass c)
{
  c[b / (8 * sizeof(int ))] |= 1 << b % (8 * sizeof(int ));
}

static void clrbit(unsigned int b,charclass c)
{
  c[b / (8 * sizeof(int ))] &= ~(1 << b % (8 * sizeof(int )));
}

static void copyset(const int src[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))],charclass dst)
{
  memcpy(dst,src,sizeof(charclass ));
}

static void zeroset(charclass s)
{
  memset(s,0,sizeof(charclass ));
}

static void notset(charclass s)
{
  int i;
  for (i = 0; i < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++i) 
    s[i] = ~s[i];
}

static int equal(const int s1[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))],const int s2[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))])
{
  return memcmp(s1,s2,sizeof(charclass )) == 0;
}
/* A pointer to the current dfa is kept here during parsing. */
static struct dfa *dfa;
/* Find the index of charclass s in dfa->charclasses, or allocate a new charclass. */

static size_t charclass_index(const int s[((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))])
{
  size_t i;
  for (i = 0; i < dfa -> cindex; ++i) 
    if (equal(s,dfa -> charclasses[i])) {
      return i;
    }
  do {
    if (dfa -> calloc <= dfa -> cindex + 1) {
      size_t new_n_alloc = dfa -> cindex + 1 + (!dfa -> charclasses);
      dfa -> charclasses = (x2nrealloc((dfa -> charclasses),&new_n_alloc,sizeof(( *dfa -> charclasses))));
      dfa -> calloc = new_n_alloc;
    }
  }while (0);
  ++dfa -> cindex;
  copyset(s,dfa -> charclasses[i]);
  return i;
}
/* Syntax bits controlling the behavior of the lexical analyzer. */
static reg_syntax_t syntax_bits;
static reg_syntax_t syntax_bits_set;
/* Flag for case-folding letters into sets. */
static int case_fold;
/* End-of-line byte in data.  */
static unsigned char eolbyte;
/* Cache of char-context values.  */
static int sbit[1 << 8];
/* Set of characters considered letters. */
static charclass letters;
/* Set of characters that are newline. */
static charclass newline;
/* Add this to the test for whether a byte is word-constituent, since on
   BSD-based systems, many values in the 128..255 range are classified as
   alphabetic, while on glibc-based systems, they are not.  */
#ifdef __GLIBC__
# define is_valid_unibyte_character(c) 1
#else
# define is_valid_unibyte_character(c) (! (MBS_SUPPORT && btowc (c) == WEOF))
#endif
/* Return non-zero if C is a "word-constituent" byte; zero otherwise.  */
#define IS_WORD_CONSTITUENT(C) \
  (is_valid_unibyte_character (C) && (isalnum (C) || (C) == '_'))

static int char_context(unsigned char c)
{
  if (c == eolbyte || c == 0) {
    return 4;
  }
  if (1 && (( *__ctype_b_loc())[(int )c] & ((unsigned short )_ISalnum) || c == '_')) {
    return 2;
  }
  return 1;
}

static int wchar_context(wint_t wc)
{
  if (wc == ((wchar_t )eolbyte) || wc == 0) {
    return 4;
  }
  if (wc == '_' || iswalnum(wc)) {
    return 2;
  }
  return 1;
}
/* Entry point to set syntax options. */

void dfasyntax(reg_syntax_t bits,int fold,unsigned char eol)
{
  unsigned int i;
  syntax_bits_set = 1;
  syntax_bits = bits;
  case_fold = fold;
  eolbyte = eol;
  for (i = 0; i < (1 << 8); ++i) {
    sbit[i] = char_context(i);
    switch(sbit[i]){
      case 2:
{
        setbit(i,letters);
        break; 
      }
      case 4:
{
        setbit(i,newline);
        break; 
      }
    }
  }
}
/* Set a bit in the charclass for the given wchar_t.  Do nothing if WC
   is represented by a multi-byte sequence.  Even for MB_CUR_MAX == 1,
   this may happen when folding case in weird Turkish locales where
   dotless i/dotted I are not included in the chosen character set.
   Return whether a bit was set in the charclass.  */
#if MBS_SUPPORT

static _Bool setbit_wc(wint_t wc,charclass c)
{
  int b = wctob(wc);
  if (b == - 1) {
    return 0;
  }
  setbit(b,c);
  return 1;
}
/* Set a bit in the charclass for the given single byte character,
   if it is valid in the current character set.  */

static void setbit_c(int b,charclass c)
{
/* Do nothing if b is invalid in this character set.  */
  if (__ctype_get_mb_cur_max() > 1 && btowc(b) == 0xffffffffu) {
    return ;
  }
  setbit(b,c);
}
#else
# define setbit_c setbit
/*NOTREACHED*/
#endif
/* Like setbit_c, but if case is folded, set both cases of a letter.  For
   MB_CUR_MAX > 1, the resulting charset is only used as an optimization,
   and the caller takes care of setting the appropriate field of struct
   mb_char_classes.  */

static void setbit_case_fold_c(int b,charclass c)
{
  if (__ctype_get_mb_cur_max() > 1) {
    wint_t wc = btowc(b);
    if (wc == 0xffffffffu) {
      return ;
    }
    setbit(b,c);
    if (case_fold && iswalpha(wc)) {
      setbit_wc((iswupper(wc)?towlower(wc) : towupper(wc)),c);
    }
  }
  else {
    setbit(b,c);
    if (case_fold && ( *__ctype_b_loc())[(int )b] & ((unsigned short )_ISalpha)) {
      setbit_c((( *__ctype_b_loc())[(int )b] & ((unsigned short )_ISupper)?tolower(b) : toupper(b)),c);
    }
  }
}
/* UTF-8 encoding allows some optimizations that we can't otherwise
   assume in a multibyte encoding. */

inline static int using_utf8()
{
  static int utf8 = - 1;
  if (utf8 == - 1) {
#if defined HAVE_LANGINFO_CODESET && MBS_SUPPORT
    utf8 = strcmp((nl_langinfo(CODESET)),"UTF-8") == 0;
#else
#endif
  }
  return utf8;
}
/* Lexical analyzer.  All the dross that deals with the obnoxious
   GNU Regex syntax bits is located here.  The poor, suffering
   reader is referred to the GNU Regex documentation for the
   meaning of the @#%!@#%^!@ syntax bits. */
/* Pointer to next input character. */
static const char *lexptr;
/* Number of characters remaining. */
static size_t lexleft;
/* Previous token returned; initially END. */
static token lasttok;
/* True if we're separated from beginning or (, |
                                   only by zero-width characters. */
static int laststart;
/* Count of outstanding left parens. */
static size_t parens;
/* Repeat counts for {m,n}. */
static int minrep;
static int maxrep;
/* Length of the multibyte representation of
                                   wctok.  */
static int cur_mb_len = 1;
/* These variables are used only if (MB_CUR_MAX > 1).  */
/* Mbstate for mbrlen().  */
static mbstate_t mbs;
/* Wide character representation of the current
                                   multibyte character.  */
static wchar_t wctok;
/* Correspond to the input buffer in dfaexec().
                                           Each element store the amount of remain
                                           byte of corresponding multibyte character
                                           in the input string.  A element's value
                                           is 0 if corresponding character is a
                                           single byte character.
                                           e.g. input : 'a', <mb(0)>, <mb(1)>, <mb(2)>
                                           mblen_buf :   0,       3,       2,       1
                                         */
static unsigned char *mblen_buf;
/* Wide character representation of input
                                   string in dfaexec().
                                   The length of this array is same as
                                   the length of input string(char array).
                                   inputstring[i] is a single-byte char,
                                   or 1st byte of a multibyte char.
                                   And inputwcs[i] is the codepoint.  */
static wchar_t *inputwcs;
/* reference to begin in dfaexec().  */
static const unsigned char *buf_begin;
/* reference to end in dfaexec().  */
static const unsigned char *buf_end;
#if MBS_SUPPORT
/* Note that characters become unsigned here. */
# define FETCH_WC(c, wc, eoferr)		\
  do {						\
    if (! lexleft)				\
      {						\
        if ((eoferr) != 0)			\
          dfaerror (eoferr);			\
        else					\
          return lasttok = END;			\
      }						\
    else					\
      {						\
        wchar_t _wc;				\
        cur_mb_len = mbrtowc (&_wc, lexptr, lexleft, &mbs); \
        if (cur_mb_len <= 0)			\
          {					\
            cur_mb_len = 1;			\
            --lexleft;				\
            (wc) = (c) = to_uchar (*lexptr++);  \
          }					\
        else					\
          {					\
            lexptr += cur_mb_len;		\
            lexleft -= cur_mb_len;		\
            (wc) = _wc;				\
            (c) = wctob (wc);			\
          }					\
      }						\
  } while(0)
# define FETCH(c, eoferr)			\
  do {						\
    wint_t wc;					\
    FETCH_WC (c, wc, eoferr);			\
  } while (0)
#else
/* Note that characters become unsigned here. */
# define FETCH(c, eoferr)	      \
  do {				      \
    if (! lexleft)		      \
      {				      \
        if ((eoferr) != 0)	      \
          dfaerror (eoferr);	      \
        else			      \
          return lasttok = END;	      \
      }				      \
    (c) = to_uchar (*lexptr++);       \
    --lexleft;			      \
  } while(0)
# define FETCH_WC(c, unused, eoferr) FETCH (c, eoferr)
#endif /* MBS_SUPPORT */
#ifndef MIN
# define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
typedef int predicate(int );
/* The following list maps the names of the Posix named character classes
   to predicate functions that determine whether a given character is in
   the class.  The leading [ has already been eaten by the lexical analyzer. */

struct dfa_ctype 
{
  const char *name;
  predicate *func;
  _Bool single_byte_only;
}
;
static const struct dfa_ctype prednames[] = {{("alpha"), (isalpha), (0)}, {("upper"), (isupper), (0)}, {("lower"), (islower), (0)}, {("digit"), (isdigit), (1)}, {("xdigit"), (isxdigit), (1)}, {("space"), (isspace), (0)}, {("punct"), (ispunct), (0)}, {("alnum"), (isalnum), (0)}, {("print"), (isprint), (0)}, {("graph"), (isgraph), (0)}, {("cntrl"), (iscntrl), (0)}, {("blank"), (isblank), (0)}, {(((void *)0)), (((void *)0)), (0)}};

static const struct dfa_ctype *find_pred(const char *str)
{
  unsigned int i;
  for (i = 0; prednames[i] . name; ++i) 
    if (strcmp(str,prednames[i] . name) == 0) {
      break; 
    }
  return &prednames[i];
}
/* Multibyte character handling sub-routine for lex.
   This function  parse a bracket expression and build a struct
   mb_char_classes.  */

static token parse_bracket_exp()
{
  int invert;
  int c;
  int c1;
  int c2;
  charclass ccl;
/* Used to warn about [:space:].
     Bit 0 = first character is a colon.
     Bit 1 = last character is a colon.
     Bit 2 = includes any other character but a colon.
     Bit 3 = includes ranges, char/equiv classes or collation elements.  */
  int colon_warning_state;
  wint_t wc;
  wint_t wc2;
  wint_t wc1 = 0;
/* Work area to build a mb_char_classes.  */
  struct mb_char_classes *work_mbc;
  size_t chars_al;
  size_t range_sts_al;
  size_t range_ends_al;
  size_t ch_classes_al;
  size_t equivs_al;
  size_t coll_elems_al;
  chars_al = 0;
  range_sts_al = range_ends_al = 0;
  ch_classes_al = equivs_al = coll_elems_al = 0;
  if (__ctype_get_mb_cur_max() > 1) {
    do {
      if (dfa -> mbcsets_alloc <= dfa -> nmbcsets + 1) {
        size_t new_n_alloc = dfa -> nmbcsets + 1 + (!dfa -> mbcsets);
        dfa -> mbcsets = (x2nrealloc((dfa -> mbcsets),&new_n_alloc,sizeof(( *dfa -> mbcsets))));
        dfa -> mbcsets_alloc = new_n_alloc;
      }
    }while (0);
/* dfa->multibyte_prop[] hold the index of dfa->mbcsets.
         We will update dfa->multibyte_prop[] in addtok(), because we can't
         decide the index in dfa->tokens[].  */
/* Initialize work area.  */
    work_mbc = &dfa -> mbcsets[dfa -> nmbcsets++];
    memset(work_mbc,0,sizeof(( *work_mbc)));
  }
  else {
    work_mbc = ((void *)0);
  }
  memset(ccl,0,sizeof(ccl));
  do {
    if (!lexleft) {
      if (gettext("unbalanced [") != 0) {
        dfaerror((gettext("unbalanced [")));
      }
      else {
        return lasttok = END;
      }
    }
    else {
      wchar_t _wc;
      cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
      if (cur_mb_len <= 0) {
        cur_mb_len = 1;
        --lexleft;
        wc = (c = (to_uchar( *(lexptr++))));
      }
      else {
        lexptr += cur_mb_len;
        lexleft -= cur_mb_len;
        wc = _wc;
        c = wctob(wc);
      }
    }
  }while (0);
  if (c == '^') {
    do {
      if (!lexleft) {
        if (gettext("unbalanced [") != 0) {
          dfaerror((gettext("unbalanced [")));
        }
        else {
          return lasttok = END;
        }
      }
      else {
        wchar_t _wc;
        cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
        if (cur_mb_len <= 0) {
          cur_mb_len = 1;
          --lexleft;
          wc = (c = (to_uchar( *(lexptr++))));
        }
        else {
          lexptr += cur_mb_len;
          lexleft -= cur_mb_len;
          wc = _wc;
          c = wctob(wc);
        }
      }
    }while (0);
    invert = 1;
  }
  else {
    invert = 0;
  }
  colon_warning_state = c == ':';
  do {
/* mark c1 is not initialized".  */
    c1 = - 1;
    colon_warning_state &= ~2;
/* Note that if we're looking at some other [:...:] construct,
         we just treat it as a bunch of ordinary characters.  We can do
         this because we assume regex has checked for syntax errors before
         dfa is ever called. */
    if (c == '[' && syntax_bits & ((unsigned long )1) << 1 << 1) {
#define BRACKET_BUFFER_SIZE 128
      char str[128];
      do {
        if (!lexleft) {
          if (gettext("unbalanced [") != 0) {
            dfaerror((gettext("unbalanced [")));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wc1 = (c1 = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wc1 = _wc;
            c1 = wctob(wc1);
          }
        }
      }while (0);
/* If pattern contains '[[:', '[[.', or '[[='.  */
      if (c1 == ':' || __ctype_get_mb_cur_max() > 1 && (c1 == '.' || c1 == '=')) 
/* TODO: handle '[[.' and '[[=' also for MB_CUR_MAX == 1.  */
{
        size_t len = 0;
        for (; ; ) {
          do {
            if (!lexleft) {
              if (gettext("unbalanced [") != 0) {
                dfaerror((gettext("unbalanced [")));
              }
              else {
                return lasttok = END;
              }
            }
            else {
              wchar_t _wc;
              cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
              if (cur_mb_len <= 0) {
                cur_mb_len = 1;
                --lexleft;
                wc = (c = (to_uchar( *(lexptr++))));
              }
              else {
                lexptr += cur_mb_len;
                lexleft -= cur_mb_len;
                wc = _wc;
                c = wctob(wc);
              }
            }
          }while (0);
          if (c == c1 && ( *lexptr) == ']' || lexleft == 0) {
            break; 
          }
          if (len < 128) {
            str[len++] = c;
          }
          else {
/* This is in any case an invalid class name.  */
            str[0] = '\0';
          }
        }
        str[len] = '\0';
/* Fetch bracket.  */
        do {
          if (!lexleft) {
            if (gettext("unbalanced [") != 0) {
              dfaerror((gettext("unbalanced [")));
            }
            else {
              return lasttok = END;
            }
          }
          else {
            wchar_t _wc;
            cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
            if (cur_mb_len <= 0) {
              cur_mb_len = 1;
              --lexleft;
              wc = (c = (to_uchar( *(lexptr++))));
            }
            else {
              lexptr += cur_mb_len;
              lexleft -= cur_mb_len;
              wc = _wc;
              c = wctob(wc);
            }
          }
        }while (0);
        if (c1 == ':') 
/* build character class.  */
{
          const char *class = (case_fold && (strcmp(str,"upper") == 0 || strcmp(str,"lower") == 0)?"alpha" : str);
          const struct dfa_ctype *pred = find_pred(class);
          if (!pred) {
            dfaerror((gettext("invalid character class")));
          }
          if (__ctype_get_mb_cur_max() > 1 && !pred -> single_byte_only) {
/* Store the character class as wctype_t.  */
            wctype_t wt = wctype(class);
            do {
              if (ch_classes_al <= work_mbc -> nch_classes + 1) {
                size_t new_n_alloc = work_mbc -> nch_classes + 1 + (!work_mbc -> ch_classes);
                work_mbc -> ch_classes = (x2nrealloc((work_mbc -> ch_classes),&new_n_alloc,sizeof(( *work_mbc -> ch_classes))));
                ch_classes_al = new_n_alloc;
              }
            }while (0);
            work_mbc -> ch_classes[work_mbc -> nch_classes++] = wt;
          }
          for (c2 = 0; c2 < 1 << 8; ++c2) 
            if ((pred -> func)(c2)) {
              setbit_case_fold_c(c2,ccl);
            }
        }
        else {
          if (1 && (c1 == '=' || c1 == '.')) {
            char *elem = (xmemdup(str,len + 1));
            if (c1 == '=') 
/* build equivalence class.  */
{
              do {
                if (equivs_al <= work_mbc -> nequivs + 1) {
                  size_t new_n_alloc = work_mbc -> nequivs + 1 + (!work_mbc -> equivs);
                  work_mbc -> equivs = (x2nrealloc((work_mbc -> equivs),&new_n_alloc,sizeof(( *work_mbc -> equivs))));
                  equivs_al = new_n_alloc;
                }
              }while (0);
              work_mbc -> equivs[work_mbc -> nequivs++] = elem;
            }
            if (c1 == '.') 
/* build collating element.  */
{
              do {
                if (coll_elems_al <= work_mbc -> ncoll_elems + 1) {
                  size_t new_n_alloc = work_mbc -> ncoll_elems + 1 + (!work_mbc -> coll_elems);
                  work_mbc -> coll_elems = (x2nrealloc((work_mbc -> coll_elems),&new_n_alloc,sizeof(( *work_mbc -> coll_elems))));
                  coll_elems_al = new_n_alloc;
                }
              }while (0);
              work_mbc -> coll_elems[work_mbc -> ncoll_elems++] = elem;
            }
          }
        }
        colon_warning_state |= 8;
/* Fetch new lookahead character.  */
        do {
          if (!lexleft) {
            if (gettext("unbalanced [") != 0) {
              dfaerror((gettext("unbalanced [")));
            }
            else {
              return lasttok = END;
            }
          }
          else {
            wchar_t _wc;
            cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
            if (cur_mb_len <= 0) {
              cur_mb_len = 1;
              --lexleft;
              wc1 = (c1 = (to_uchar( *(lexptr++))));
            }
            else {
              lexptr += cur_mb_len;
              lexleft -= cur_mb_len;
              wc1 = _wc;
              c1 = wctob(wc1);
            }
          }
        }while (0);
        continue; 
      }
/* We treat '[' as a normal character here.  c/c1/wc/wc1
             are already set up.  */
    }
    if (c == '\\' && syntax_bits & ((unsigned long )1)) {
      do {
        if (!lexleft) {
          if (gettext("unbalanced [") != 0) {
            dfaerror((gettext("unbalanced [")));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wc = (c = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wc = _wc;
            c = wctob(wc);
          }
        }
      }while (0);
    }
    if (c1 == - 1) {
      do {
        if (!lexleft) {
          if (gettext("unbalanced [") != 0) {
            dfaerror((gettext("unbalanced [")));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wc1 = (c1 = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wc1 = _wc;
            c1 = wctob(wc1);
          }
        }
      }while (0);
    }
    if (c1 == '-') 
/* build range characters.  */
{
      do {
        if (!lexleft) {
          if (gettext("unbalanced [") != 0) {
            dfaerror((gettext("unbalanced [")));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wc2 = (c2 = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wc2 = _wc;
            c2 = wctob(wc2);
          }
        }
      }while (0);
      if (c2 == ']') {
/* In the case [x-], the - is an ordinary hyphen,
                 which is left in c1, the lookahead character. */
        lexptr -= cur_mb_len;
        lexleft += cur_mb_len;
      }
    }
    if (c1 == '-' && c2 != ']') {
      if (c2 == '\\' && syntax_bits & ((unsigned long )1)) {
        do {
          if (!lexleft) {
            if (gettext("unbalanced [") != 0) {
              dfaerror((gettext("unbalanced [")));
            }
            else {
              return lasttok = END;
            }
          }
          else {
            wchar_t _wc;
            cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
            if (cur_mb_len <= 0) {
              cur_mb_len = 1;
              --lexleft;
              wc2 = (c2 = (to_uchar( *(lexptr++))));
            }
            else {
              lexptr += cur_mb_len;
              lexleft -= cur_mb_len;
              wc2 = _wc;
              c2 = wctob(wc2);
            }
          }
        }while (0);
      }
      if (__ctype_get_mb_cur_max() > 1) {
/* When case folding map a range, say [m-z] (or even [M-z])
                 to the pair of ranges, [m-z] [M-Z].  */
        do {
          if (range_sts_al <= work_mbc -> nranges + 1) {
            size_t new_n_alloc = work_mbc -> nranges + 1 + (!work_mbc -> range_sts);
            work_mbc -> range_sts = (x2nrealloc((work_mbc -> range_sts),&new_n_alloc,sizeof(( *work_mbc -> range_sts))));
            range_sts_al = new_n_alloc;
          }
        }while (0);
        do {
          if (range_ends_al <= work_mbc -> nranges + 1) {
            size_t new_n_alloc = work_mbc -> nranges + 1 + (!work_mbc -> range_ends);
            work_mbc -> range_ends = (x2nrealloc((work_mbc -> range_ends),&new_n_alloc,sizeof(( *work_mbc -> range_ends))));
            range_ends_al = new_n_alloc;
          }
        }while (0);
        work_mbc -> range_sts[work_mbc -> nranges] = ((case_fold?towlower(wc) : ((wchar_t )wc)));
        work_mbc -> range_ends[work_mbc -> nranges++] = ((case_fold?towlower(wc2) : ((wchar_t )wc2)));
#ifndef GREP
#endif
      }
      else {
/* Defer to the system regex library about the meaning
                 of range expressions.  */
        regex_t re;
        char pattern[6] = {('['), (0), ('-'), (0), (']'), (0)};
        char subject[2] = {(0), (0)};
        c1 = c;
        if (case_fold) {
          c1 = tolower(c1);
          c2 = tolower(c2);
        }
        pattern[1] = c1;
        pattern[3] = c2;
        rpl_regcomp(&re,pattern,1 << 3);
        for (c = 0; c < 1 << 8; ++c) {
          if (case_fold && ( *__ctype_b_loc())[(int )c] & ((unsigned short )_ISupper) || __ctype_get_mb_cur_max() > 1 && btowc(c) == 0xffffffffu) {
            continue; 
          }
          subject[0] = c;
          if (rpl_regexec((&re),subject,0,((void *)0),0) != _REG_NOMATCH) {
            setbit_case_fold_c(c,ccl);
          }
        }
        rpl_regfree(&re);
      }
      colon_warning_state |= 8;
      do {
        if (!lexleft) {
          if (gettext("unbalanced [") != 0) {
            dfaerror((gettext("unbalanced [")));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wc1 = (c1 = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wc1 = _wc;
            c1 = wctob(wc1);
          }
        }
      }while (0);
      continue; 
    }
    colon_warning_state |= (c == ':'?2 : 4);
    if (__ctype_get_mb_cur_max() == 1) {
      setbit_case_fold_c(c,ccl);
      continue; 
    }
    if (case_fold && iswalpha(wc)) {
      wc = towlower(wc);
      if (!setbit_wc(wc,ccl)) {
        do {
          if (chars_al <= work_mbc -> nchars + 1) {
            size_t new_n_alloc = work_mbc -> nchars + 1 + (!work_mbc -> chars);
            work_mbc -> chars = (x2nrealloc((work_mbc -> chars),&new_n_alloc,sizeof(( *work_mbc -> chars))));
            chars_al = new_n_alloc;
          }
        }while (0);
        work_mbc -> chars[work_mbc -> nchars++] = wc;
      }
#ifdef GREP
      continue; 
#else
#endif
    }
    if (!setbit_wc(wc,ccl)) {
      do {
        if (chars_al <= work_mbc -> nchars + 1) {
          size_t new_n_alloc = work_mbc -> nchars + 1 + (!work_mbc -> chars);
          work_mbc -> chars = (x2nrealloc((work_mbc -> chars),&new_n_alloc,sizeof(( *work_mbc -> chars))));
          chars_al = new_n_alloc;
        }
      }while (0);
      work_mbc -> chars[work_mbc -> nchars++] = wc;
    }
  }while ((wc = wc1 , (c = c1) != ']'));
  if (colon_warning_state == 7) {
    dfawarn((gettext("character class syntax is [[:space:]], not [:space:]")));
  }
  if (__ctype_get_mb_cur_max() > 1) {
    static charclass zeroclass;
    work_mbc -> invert = invert;
    work_mbc -> cset = ((equal(ccl,zeroclass)?(- 1) : charclass_index(ccl)));
    return MBCSET;
  }
  if (invert) {
    __ctype_get_mb_cur_max() == 1?((void )0) : __assert_fail("(__ctype_get_mb_cur_max ()) == 1","dfa.c",1184,__PRETTY_FUNCTION__);
    notset(ccl);
    if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
      clrbit(eolbyte,ccl);
    }
  }
  return (CSET + charclass_index(ccl));
}

static token lex()
{
  unsigned int c;
  unsigned int c2;
  int backslash = 0;
  charclass ccl;
  int i;
/* Basic plan: We fetch a character.  If it's a backslash,
     we set the backslash flag and go through the loop again.
     On the plus side, this avoids having a duplicate of the
     main switch inside the backslash case.  On the minus side,
     it means that just about every case begins with
     "if (backslash) ...".  */
  for (i = 0; i < 2; ++i) {
    if (__ctype_get_mb_cur_max() > 1) {
      do {
        if (!lexleft) {
          if (((void *)0) != 0) {
            dfaerror(((void *)0));
          }
          else {
            return lasttok = END;
          }
        }
        else {
          wchar_t _wc;
          cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
          if (cur_mb_len <= 0) {
            cur_mb_len = 1;
            --lexleft;
            wctok = (c = (to_uchar( *(lexptr++))));
          }
          else {
            lexptr += cur_mb_len;
            lexleft -= cur_mb_len;
            wctok = _wc;
            c = (wctob(wctok));
          }
        }
      }while (0);
      if (((int )c) == - 1) {
        goto normal_char;
      }
    }
    else {
      do {
        wint_t wc;
        do {
          if (!lexleft) {
            if (((void *)0) != 0) {
              dfaerror(((void *)0));
            }
            else {
              return lasttok = END;
            }
          }
          else {
            wchar_t _wc;
            cur_mb_len = (mbrtowc(&_wc,lexptr,lexleft,&mbs));
            if (cur_mb_len <= 0) {
              cur_mb_len = 1;
              --lexleft;
              wc = c = (to_uchar( *(lexptr++)));
            }
            else {
              lexptr += cur_mb_len;
              lexleft -= cur_mb_len;
              wc = _wc;
              c = (wctob(wc));
            }
          }
        }while (0);
      }while (0);
    }
    switch(c){
      case '\\':
{
        if (backslash) {
          goto normal_char;
        }
        if (lexleft == 0) {
          dfaerror((gettext("unfinished \\ escape")));
        }
        backslash = 1;
        break; 
      }
      case '^':
{
        if (backslash) {
          goto normal_char;
        }
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 || lasttok == END || lasttok == LPAREN || lasttok == OR) {
          return lasttok = BEGLINE;
        }
        goto normal_char;
      }
      case '$':
{
        if (backslash) {
          goto normal_char;
        }
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 || lexleft == 0 || ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1?lexleft > 0 && ( *lexptr) == ')' : lexleft > 1 && lexptr[0] == '\\' && lexptr[1] == ')')) || ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1?lexleft > 0 && ( *lexptr) == '|' : lexleft > 1 && lexptr[0] == '\\' && lexptr[1] == '|')) || syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 && lexleft > 0 && ( *lexptr) == 10) {
          return lasttok = ENDLINE;
        }
        goto normal_char;
      }
      case '1':
{
      }
      case '2':
{
      }
      case '3':
{
      }
      case '4':
{
      }
      case '5':
{
      }
      case '6':
{
      }
      case '7':
{
      }
      case 56:
{
      }
      case '9':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          laststart = 0;
          return lasttok = BACKREF;
        }
        goto normal_char;
      }
      case '`':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
/* FIXME: should be beginning of string */
          return lasttok = BEGLINE;
        }
        goto normal_char;
      }
      case '\'':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
/* FIXME: should be end of string */
          return lasttok = ENDLINE;
        }
        goto normal_char;
      }
      case '<':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          return lasttok = BEGWORD;
        }
        goto normal_char;
      }
      case '>':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          return lasttok = ENDWORD;
        }
        goto normal_char;
      }
      case 'b':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          return lasttok = LIMWORD;
        }
        goto normal_char;
      }
      case 'B':
{
        if (backslash && !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          return lasttok = NOTLIMWORD;
        }
        goto normal_char;
      }
      case '?':
{
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1) != 0)) {
          goto normal_char;
        }
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1) && laststart) {
          goto normal_char;
        }
        return lasttok = QMARK;
      }
      case '*':
{
        if (backslash) {
          goto normal_char;
        }
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1) && laststart) {
          goto normal_char;
        }
        return lasttok = STAR;
      }
      case '+':
{
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1) != 0)) {
          goto normal_char;
        }
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1) && laststart) {
          goto normal_char;
        }
        return lasttok = PLUS;
      }
      case '{':
{
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          goto normal_char;
        }
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) == 0)) {
          goto normal_char;
        }
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1) && laststart) {
          goto normal_char;
        }
/* Cases:
             {M} - exact count
             {M,} - minimum count, maximum is infinity
             {,N} - 0 through N
             {,} - 0 to infinity (same as '*')
             {M,N} - M through N */
{
          const char *p = lexptr;
          const char *lim = p + lexleft;
          minrep = maxrep = - 1;
          for (; p != lim && ((unsigned int )( *p)) - 48 <= 9; p++) {
            if (minrep < 0) {
              minrep = ( *p) - 48;
            }
            else {
              minrep = (0x7fff + 1 < minrep * 10 + ( *p) - 48?0x7fff + 1 : minrep * 10 + ( *p) - 48);
            }
          }
          if (p != lim) {
            if (( *p) != ',') {
              maxrep = minrep;
            }
            else {
              if (minrep < 0) {
                minrep = 0;
              }
              while(++p != lim && ((unsigned int )( *p)) - 48 <= 9){
                if (maxrep < 0) {
                  maxrep = ( *p) - 48;
                }
                else {
                  maxrep = (0x7fff + 1 < maxrep * 10 + ( *p) - 48?0x7fff + 1 : maxrep * 10 + ( *p) - 48);
                }
              }
            }
          }
          if (!((!backslash || p != lim && ( *(p++)) == '\\') && p != lim && ( *(p++)) == '}' && 0 <= minrep && (maxrep < 0 || minrep <= maxrep))) {
            if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
              goto normal_char;
            }
            dfaerror((gettext("Invalid content of \\{\\}")));
          }
          if (0x7fff < maxrep) {
            dfaerror((gettext("Regular expression too big")));
          }
          lexptr = p;
          lexleft = (lim - p);
        }
        laststart = 0;
        return lasttok = REPMN;
      }
      case '|':
{
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) == 0)) {
          goto normal_char;
        }
        laststart = 1;
        return lasttok = OR;
      }
      case 10:
{
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 || backslash || !(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1)) {
          goto normal_char;
        }
        laststart = 1;
        return lasttok = OR;
      }
      case 40:
{
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) == 0)) {
          goto normal_char;
        }
        ++parens;
        laststart = 1;
        return lasttok = LPAREN;
      }
      case ')':
{
        if (backslash != ((syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) == 0)) {
          goto normal_char;
        }
        if (parens == 0 && syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        --parens;
        laststart = 0;
        return lasttok = RPAREN;
      }
      case '.':
{
        if (backslash) {
          goto normal_char;
        }
        if (__ctype_get_mb_cur_max() > 1) {
/* In multibyte environment period must match with a single
                 character not a byte.  So we use ANYCHAR.  */
          laststart = 0;
          return lasttok = ANYCHAR;
        }
        zeroset(ccl);
        notset(ccl);
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1)) {
          clrbit(eolbyte,ccl);
        }
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          clrbit('\0',ccl);
        }
        laststart = 0;
        return lasttok = (CSET + charclass_index(ccl));
      }
      case 's':
{
      }
      case 'S':
{
        if (!backslash || syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        zeroset(ccl);
        for (c2 = 0; c2 < (1 << 8); ++c2) 
          if (( *__ctype_b_loc())[(int )c2] & ((unsigned short )_ISspace)) {
            setbit(c2,ccl);
          }
        if (c == 'S') {
          notset(ccl);
        }
        laststart = 0;
        return lasttok = (CSET + charclass_index(ccl));
      }
      case 'w':
{
      }
      case 'W':
{
        if (!backslash || syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          goto normal_char;
        }
        zeroset(ccl);
        for (c2 = 0; c2 < (1 << 8); ++c2) 
          if (1 && (( *__ctype_b_loc())[(int )c2] & ((unsigned short )_ISalnum) || c2 == '_')) {
            setbit(c2,ccl);
          }
        if (c == 'W') {
          notset(ccl);
        }
        laststart = 0;
        return lasttok = (CSET + charclass_index(ccl));
      }
      case '[':
{
        if (backslash) {
          goto normal_char;
        }
        laststart = 0;
        return lasttok = parse_bracket_exp();
      }
      default:
{
        normal_char:
        laststart = 0;
/* For multibyte character sets, folding is done in atom.  Always
             return WCHAR.  */
        if (__ctype_get_mb_cur_max() > 1) {
          return lasttok = WCHAR;
        }
        if (case_fold && ( *__ctype_b_loc())[(int )c] & ((unsigned short )_ISalpha)) {
          zeroset(ccl);
          setbit_case_fold_c(c,ccl);
          return lasttok = (CSET + charclass_index(ccl));
        }
        return lasttok = c;
      }
    }
  }
/* The above loop should consume at most a backslash
     and some other character. */
  abort();
/* keeps pedantic compilers happy. */
  return END;
}
/* Recursive descent parser for regular expressions. */
/* Lookahead token. */
static token tok;
/* Current depth of a hypothetical stack
                                   holding deferred productions.  This is
                                   used to determine the depth that will be
                                   required of the real stack later on in
                                   dfaanalyze(). */
static size_t depth;

static void addtok_mb(token t,int mbprop)
{
  if (__ctype_get_mb_cur_max() > 1) {
    do {
      if (dfa -> nmultibyte_prop <= dfa -> tindex + 1) {
        size_t new_n_alloc = dfa -> tindex + 1 + (!dfa -> multibyte_prop);
        dfa -> multibyte_prop = (x2nrealloc((dfa -> multibyte_prop),&new_n_alloc,sizeof(( *dfa -> multibyte_prop))));
        dfa -> nmultibyte_prop = new_n_alloc;
      }
    }while (0);
    dfa -> multibyte_prop[dfa -> tindex] = mbprop;
  }
  do {
    if (dfa -> talloc <= dfa -> tindex + 1) {
      size_t new_n_alloc = dfa -> tindex + 1 + (!dfa -> tokens);
      dfa -> tokens = (x2nrealloc((dfa -> tokens),&new_n_alloc,sizeof(( *dfa -> tokens))));
      dfa -> talloc = new_n_alloc;
    }
  }while (0);
  dfa -> tokens[dfa -> tindex++] = t;
  switch(t){
    case QMARK:
{
    }
    case STAR:
{
    }
    case PLUS:
    break; 
    case CAT:
{
    }
    case OR:
{
      --depth;
      break; 
    }
    default:
    ++dfa -> nleaves;
    case EMPTY:
{
      ++depth;
      break; 
    }
  }
  if (depth > dfa -> depth) {
    dfa -> depth = depth;
  }
}
static void addtok_wc(wint_t wc);
/* Add the given token to the parse tree, maintaining the depth count and
   updating the maximum depth if necessary. */

static void addtok(token t)
{
  if (__ctype_get_mb_cur_max() > 1 && t == MBCSET) {
    _Bool need_or = 0;
    struct mb_char_classes *work_mbc = &dfa -> mbcsets[dfa -> nmbcsets - 1];
/* Extract wide characters into alternations for better performance.
         This does not require UTF-8.  */
    if (!work_mbc -> invert) {
      size_t i;
      for (i = 0; i < work_mbc -> nchars; i++) {
        addtok_wc(work_mbc -> chars[i]);
        if (need_or) {
          addtok(OR);
        }
        need_or = 1;
      }
      work_mbc -> nchars = 0;
    }
/* UTF-8 allows treating a simple, non-inverted MBCSET like a CSET.  */
    if (work_mbc -> invert || !using_utf8() && work_mbc -> cset != (- 1) || work_mbc -> nchars != 0 || work_mbc -> nch_classes != 0 || work_mbc -> nranges != 0 || work_mbc -> nequivs != 0 || work_mbc -> ncoll_elems != 0) {
      addtok_mb(MBCSET,((dfa -> nmbcsets - 1 << 2) + 3));
      if (need_or) {
        addtok(OR);
      }
    }
    else {
/* Characters have been handled above, so it is possible
             that the mbcset is empty now.  Do nothing in that case.  */
      if (work_mbc -> cset != (- 1)) {
        using_utf8()?((void )0) : __assert_fail("using_utf8 ()","dfa.c",1577,__PRETTY_FUNCTION__);
        addtok(CSET + work_mbc -> cset);
        if (need_or) {
          addtok(OR);
        }
      }
    }
  }
  else {
    addtok_mb(t,3);
  }
}
#if MBS_SUPPORT
/* We treat a multibyte character as a single atom, so that DFA
   can treat a multibyte character as a single expression.
   e.g. We construct following tree from "<mb1><mb2>".
   <mb1(1st-byte)><mb1(2nd-byte)><CAT><mb1(3rd-byte)><CAT>
   <mb2(1st-byte)><mb2(2nd-byte)><CAT><mb2(3rd-byte)><CAT><CAT> */

static void addtok_wc(wint_t wc)
{
  unsigned char buf[16];
  mbstate_t s;
  int i;
  memset((&s),0,sizeof(s));
  cur_mb_len = (wcrtomb(((char *)buf),wc,&s));
/* This is merely stop-gap.  When cur_mb_len is 0 or negative,
     buf[0] is undefined, yet skipping the addtok_mb call altogether
     can result in heap corruption.  */
  if (cur_mb_len <= 0) {
    buf[0] = 0;
  }
  addtok_mb(buf[0],(cur_mb_len == 1?3 : 1));
  for (i = 1; i < cur_mb_len; i++) {
    addtok_mb(buf[i],(i == cur_mb_len - 1?2 : 0));
    addtok(CAT);
  }
}
#else
#endif

static void add_utf8_anychar()
{
#if MBS_SUPPORT
  static const int utf8_classes[5][287UL / 32UL] = {
/* 80-bf: non-lead bytes */
{(0), (0), (0), (0), (~0), (~0), (0), (0)}, 
/* 00-7f: 1-byte sequence */
{(~0), (~0), (~0), (~0), (0), (0), (0), (0)}, 
/* c2-df: 2-byte sequence */
{(0), (0), (0), (0), (0), (0), (~3), (0)}, 
/* e0-ef: 3-byte sequence */
{(0), (0), (0), (0), (0), (0), (0), (0xffff)}, 
/* f0-f7: 4-byte sequence */
{(0), (0), (0), (0), (0), (0), (0), (0xff0000)}};
  const unsigned int n = (sizeof(utf8_classes) / sizeof(utf8_classes[0]));
  unsigned int i;
/* Define the five character classes that are needed below.  */
  if (dfa -> utf8_anychar_classes[0] == 0) {
    for (i = 0; i < n; i++) {
      charclass c;
      copyset(utf8_classes[i],c);
      if (i == 1) {
        if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1)) {
          clrbit(eolbyte,c);
        }
        if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
          clrbit('\0',c);
        }
      }
      dfa -> utf8_anychar_classes[i] = (CSET + charclass_index(c));
    }
  }
/* A valid UTF-8 character is
     ([0x00-0x7f]
     |[0xc2-0xdf][0x80-0xbf]
     |[0xe0-0xef[0x80-0xbf][0x80-0xbf]
     |[0xf0-f7][0x80-0xbf][0x80-0xbf][0x80-0xbf])
     which I'll write more concisely "B|CA|DAA|EAAA".  Factor the [0x00-0x7f]
     and you get "B|(C|(D|EA)A)A".  And since the token buffer is in reverse
     Polish notation, you get "B C D E A CAT OR A CAT OR A CAT OR".  */
  for (i = 1; i < n; i++) 
    addtok(dfa -> utf8_anychar_classes[i]);
  while(--i > 1){
    addtok(dfa -> utf8_anychar_classes[0]);
    addtok(CAT);
    addtok(OR);
  }
#endif
}
/* The grammar understood by the parser is as follows.
   regexp:
     regexp OR branch
     branch
   branch:
     branch closure
     closure
   closure:
     closure QMARK
     closure STAR
     closure PLUS
     closure REPMN
     atom
   atom:
     <normal character>
     <multibyte character>
     ANYCHAR
     MBCSET
     CSET
     BACKREF
     BEGLINE
     ENDLINE
     BEGWORD
     ENDWORD
     LIMWORD
     NOTLIMWORD
     LPAREN regexp RPAREN
     <empty>
   The parser builds a parse tree in postfix form in an array of tokens. */

static void atom()
{
  if (0) {
/* empty */
  }
  else {
    if (1 && tok == WCHAR) {
      addtok_wc((case_fold?towlower(wctok) : wctok));
#ifndef GREP
#endif
      tok = lex();
    }
    else {
      if (1 && tok == ANYCHAR && using_utf8()) {
/* For UTF-8 expand the period to a series of CSETs that define a valid
         UTF-8 character.  This avoids using the slow multibyte path.  I'm
         pretty sure it would be both profitable and correct to do it for
         any encoding; however, the optimization must be done manually as
         it is done above in add_utf8_anychar.  So, let's start with
         UTF-8: it is the most used, and the structure of the encoding
         makes the correctness more obvious.  */
        add_utf8_anychar();
        tok = lex();
      }
      else {
        if (tok >= 0 && tok < (1 << 8) || tok >= CSET || tok == BACKREF || tok == BEGLINE || tok == ENDLINE || tok == BEGWORD || tok == ANYCHAR || tok == MBCSET || tok == ENDWORD || tok == LIMWORD || tok == NOTLIMWORD) 
#if MBS_SUPPORT
#endif /* MBS_SUPPORT */
{
          addtok(tok);
          tok = lex();
        }
        else {
          if (tok == LPAREN) {
            tok = lex();
            regexp();
            if (tok != RPAREN) {
              dfaerror((gettext("unbalanced (")));
            }
            tok = lex();
          }
          else {
            addtok(EMPTY);
          }
        }
      }
    }
  }
}
/* Return the number of tokens in the given subexpression. */

static size_t nsubtoks(size_t tindex)
{
  size_t ntoks1;
  switch(dfa -> tokens[tindex - 1]){
    default:
    return 1;
    case QMARK:
{
    }
    case STAR:
{
    }
    case PLUS:
    return 1 + nsubtoks(tindex - 1);
    case CAT:
{
    }
    case OR:
{
      ntoks1 = nsubtoks(tindex - 1);
      return 1 + ntoks1 + nsubtoks(tindex - 1 - ntoks1);
    }
  }
}
/* Copy the given subexpression to the top of the tree. */

static void copytoks(size_t tindex,size_t ntokens)
{
  size_t i;
  for (i = 0; i < ntokens; ++i) {
    addtok(dfa -> tokens[tindex + i]);
/* Update index into multibyte csets.  */
    if (__ctype_get_mb_cur_max() > 1 && dfa -> tokens[tindex + i] == MBCSET) {
      dfa -> multibyte_prop[dfa -> tindex - 1] = dfa -> multibyte_prop[tindex + i];
    }
  }
}

static void closure()
{
  int i;
  size_t tindex;
  size_t ntokens;
  atom();
  while(tok == QMARK || tok == STAR || tok == PLUS || tok == REPMN)
    if (tok == REPMN && (minrep || maxrep)) {
      ntokens = nsubtoks(dfa -> tindex);
      tindex = dfa -> tindex - ntokens;
      if (maxrep < 0) {
        addtok(PLUS);
      }
      if (minrep == 0) {
        addtok(QMARK);
      }
      for (i = 1; i < minrep; ++i) {
        copytoks(tindex,ntokens);
        addtok(CAT);
      }
      for (; i < maxrep; ++i) {
        copytoks(tindex,ntokens);
        addtok(QMARK);
        addtok(CAT);
      }
      tok = lex();
    }
    else {
      if (tok == REPMN) {
        dfa -> tindex -= nsubtoks(dfa -> tindex);
        tok = lex();
        closure();
      }
      else {
        addtok(tok);
        tok = lex();
      }
    }
}

static void branch()
{
  closure();
  while(tok != RPAREN && tok != OR && tok >= 0){
    closure();
    addtok(CAT);
  }
}

static void regexp()
{
  branch();
  while(tok == OR){
    tok = lex();
    branch();
    addtok(OR);
  }
}
/* Main entry point for the parser.  S is a string to be parsed, len is the
   length of the string, so s can include NUL characters.  D is a pointer to
   the struct dfa to parse into. */

void dfaparse(const char *s,size_t len,struct dfa *d)
{
  dfa = d;
  lexptr = s;
  lexleft = len;
  lasttok = END;
  laststart = 1;
  parens = 0;
  if (__ctype_get_mb_cur_max() > 1) {
    cur_mb_len = 0;
    memset((&mbs),0,sizeof(mbs));
  }
  if (!syntax_bits_set) {
    dfaerror((gettext("no syntax specified")));
  }
  tok = lex();
  depth = d -> depth;
  regexp();
  if (tok != END) {
    dfaerror((gettext("unbalanced )")));
  }
  addtok((END - d -> nregexps));
  addtok(CAT);
  if (d -> nregexps) {
    addtok(OR);
  }
  ++d -> nregexps;
}
/* Some primitives for operating on sets of positions. */
/* Copy one set to another; the destination must be large enough. */

static void copy(const position_set *src,position_set *dst)
{
  do {
    if (dst -> alloc <= src -> nelem) {
      size_t new_n_alloc = src -> nelem + (!dst -> elems);
      dst -> elems = (x2nrealloc((dst -> elems),&new_n_alloc,sizeof(( *dst -> elems))));
      dst -> alloc = new_n_alloc;
    }
  }while (0);
  memcpy((dst -> elems),(src -> elems),sizeof(dst -> elems[0]) * src -> nelem);
  dst -> nelem = src -> nelem;
}

static void alloc_position_set(position_set *s,size_t size)
{
  do {
    s -> elems = ((sizeof(( *s -> elems)) == 1?xmalloc(size) : xnmalloc(size,sizeof(( *s -> elems)))));
  }while (0);
  s -> alloc = size;
  s -> nelem = 0;
}
/* Insert position P in set S.  S is maintained in sorted order on
   decreasing index.  If there is already an entry in S with P.index
   then merge (logically-OR) P's constraints into the one in S.
   S->elems must point to an array large enough to hold the resulting set. */

static void insert(position p,position_set *s)
{
  size_t count = s -> nelem;
  size_t lo = 0;
  size_t hi = count;
  size_t i;
  while(lo < hi){
    size_t mid = lo + hi >> 1;
    if (s -> elems[mid] . index > p . index) {
      lo = mid + 1;
    }
    else {
      hi = mid;
    }
  }
  if (lo < count && p . index == s -> elems[lo] . index) {
    s -> elems[lo] . constraint |= p . constraint;
    return ;
  }
  do {
    if (s -> alloc <= count + 1) {
      size_t new_n_alloc = count + 1 + (!s -> elems);
      s -> elems = (x2nrealloc((s -> elems),&new_n_alloc,sizeof(( *s -> elems))));
      s -> alloc = new_n_alloc;
    }
  }while (0);
  for (i = count; i > lo; i--) 
    s -> elems[i] = s -> elems[i - 1];
  s -> elems[lo] = p;
  ++s -> nelem;
}
/* Merge two sets of positions into a third.  The result is exactly as if
   the positions of both sets were inserted into an initially empty set. */

static void merge(const position_set *s1,const position_set *s2,position_set *m)
{
  size_t i = 0;
  size_t j = 0;
  do {
    if (m -> alloc <= s1 -> nelem + s2 -> nelem) {
      size_t new_n_alloc = s1 -> nelem + s2 -> nelem + (!m -> elems);
      m -> elems = (x2nrealloc((m -> elems),&new_n_alloc,sizeof(( *m -> elems))));
      m -> alloc = new_n_alloc;
    }
  }while (0);
  m -> nelem = 0;
  while(i < s1 -> nelem && j < s2 -> nelem)
    if (s1 -> elems[i] . index > s2 -> elems[j] . index) {
      m -> elems[m -> nelem++] = s1 -> elems[i++];
    }
    else {
      if (s1 -> elems[i] . index < s2 -> elems[j] . index) {
        m -> elems[m -> nelem++] = s2 -> elems[j++];
      }
      else {
        m -> elems[m -> nelem] = s1 -> elems[i++];
        m -> elems[m -> nelem++] . constraint |= s2 -> elems[j++] . constraint;
      }
    }
  while(i < s1 -> nelem)
    m -> elems[m -> nelem++] = s1 -> elems[i++];
  while(j < s2 -> nelem)
    m -> elems[m -> nelem++] = s2 -> elems[j++];
}
/* Delete a position from a set. */

static void delete(position p,position_set *s)
{
  size_t i;
  for (i = 0; i < s -> nelem; ++i) 
    if (p . index == s -> elems[i] . index) {
      break; 
    }
  if (i < s -> nelem) {
    for (--s -> nelem; i < s -> nelem; ++i) 
      s -> elems[i] = s -> elems[i + 1];
  }
}
/* Find the index of the state corresponding to the given position set with
   the given preceding context, or create a new state if there is no such
   state.  Context tells whether we got here on a newline or letter. */

static state_num state_index(struct dfa *d,const position_set *s,int context)
{
  size_t hash = 0;
  int constraint;
  state_num i;
  state_num j;
  for (i = 0; i < s -> nelem; ++i) 
    hash ^= s -> elems[i] . index + s -> elems[i] . constraint;
/* Try to find a state that exactly matches the proposed one. */
  for (i = 0; i < d -> sindex; ++i) {
    if (hash != d -> states[i] . hash || s -> nelem != d -> states[i] . elems . nelem || context != d -> states[i] . context) {
      continue; 
    }
    for (j = 0; j < s -> nelem; ++j) 
      if (s -> elems[j] . constraint != d -> states[i] . elems . elems[j] . constraint || s -> elems[j] . index != d -> states[i] . elems . elems[j] . index) {
        break; 
      }
    if (j == s -> nelem) {
      return i;
    }
  }
/* We'll have to create a new state. */
  do {
    if (d -> salloc <= d -> sindex + 1) {
      size_t new_n_alloc = (d -> sindex + 1 + (!d -> states));
      d -> states = (x2nrealloc((d -> states),&new_n_alloc,sizeof(( *d -> states))));
      d -> salloc = new_n_alloc;
    }
  }while (0);
  d -> states[i] . hash = hash;
  alloc_position_set(&d -> states[i] . elems,s -> nelem);
  copy(s,&d -> states[i] . elems);
  d -> states[i] . context = context;
  d -> states[i] . backref = 0;
  d -> states[i] . constraint = 0;
  d -> states[i] . first_end = 0;
  if (1) {
    d -> states[i] . mbps . nelem = 0;
    d -> states[i] . mbps . elems = ((void *)0);
  }
  for (j = 0; j < s -> nelem; ++j) 
    if (d -> tokens[s -> elems[j] . index] < 0) {
      constraint = s -> elems[j] . constraint;
      if ((((7 & 1?constraint & 0xf : 0)) | ((7 & 2?constraint >> 4 & 0xf : 0)) | ((7 & 4?constraint >> 8 & 0xf : 0))) & context) {
        d -> states[i] . constraint |= constraint;
      }
      if (!d -> states[i] . first_end) {
        d -> states[i] . first_end = d -> tokens[s -> elems[j] . index];
      }
    }
    else {
      if (d -> tokens[s -> elems[j] . index] == BACKREF) {
        d -> states[i] . constraint = 0x777;
        d -> states[i] . backref = 1;
      }
    }
  ++d -> sindex;
  return i;
}
/* Find the epsilon closure of a set of positions.  If any position of the set
   contains a symbol that matches the empty string in some context, replace
   that position with the elements of its follow labeled with an appropriate
   constraint.  Repeat exhaustively until no funny positions are left.
   S->elems must be large enough to hold the result. */

static void epsclosure(position_set *s,const struct dfa *d)
{
  size_t i;
  size_t j;
/* array of booleans, enough to use char, not int */
  char *visited;
  position p;
  position old;
  do {
    visited = ((sizeof(( *visited)) == 1?xzalloc(d -> tindex) : xcalloc(d -> tindex,sizeof(( *visited)))));
  }while (0);
  for (i = 0; i < s -> nelem; ++i) 
    if (d -> tokens[s -> elems[i] . index] >= (1 << 8) && d -> tokens[s -> elems[i] . index] != BACKREF && d -> tokens[s -> elems[i] . index] != ANYCHAR && d -> tokens[s -> elems[i] . index] != MBCSET && d -> tokens[s -> elems[i] . index] < CSET) 
#if MBS_SUPPORT
#endif
{
      old = s -> elems[i];
      p . constraint = old . constraint;
      delete(s -> elems[i],s);
      if (visited[old . index]) {
        --i;
        continue; 
      }
      visited[old . index] = 1;
      switch(d -> tokens[old . index]){
        case BEGLINE:
{
          p . constraint &= 0x444;
          break; 
        }
        case ENDLINE:
{
          p . constraint &= 0x700;
          break; 
        }
        case BEGWORD:
{
          p . constraint &= 0x050;
          break; 
        }
        case ENDWORD:
{
          p . constraint &= 0x202;
          break; 
        }
        case LIMWORD:
{
          p . constraint &= 0x252;
          break; 
        }
        case NOTLIMWORD:
{
          p . constraint &= 0x525;
          break; 
        }
        default:
        break; 
      }
      for (j = 0; j < d -> follows[old . index] . nelem; ++j) {
        p . index = d -> follows[old . index] . elems[j] . index;
        insert(p,s);
      }
/* Force rescan to start at the beginning. */
      i = (- 1);
    }
  free(visited);
}
/* Returns the set of contexts for which there is at least one
   character included in C.  */

static int charclass_context(charclass c)
{
  int context = 0;
  unsigned int j;
  if (tstbit(eolbyte,c)) {
    context |= 4;
  }
  for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++j) {
    if (c[j] & letters[j]) {
      context |= 2;
    }
    if (c[j] & ~(letters[j] | newline[j])) {
      context |= 1;
    }
  }
  return context;
}
/* Returns the contexts on which the position set S depends.  Each context
   in the set of returned contexts (let's call it SC) may have a different
   follow set than other contexts in SC, and also different from the
   follow set of the complement set (sc ^ CTX_ANY).  However, all contexts
   in the complement set will have the same follow set.  */

static int state_separate_contexts(const position_set *s)
{
  int separate_contexts = 0;
  size_t j;
  for (j = 0; j < s -> nelem; ++j) {
    if ((s -> elems[j] . constraint >> 2 & 0x111) != (s -> elems[j] . constraint & 0x111)) {
      separate_contexts |= 4;
    }
    if ((s -> elems[j] . constraint >> 1 & 0x111) != (s -> elems[j] . constraint & 0x111)) {
      separate_contexts |= 2;
    }
  }
  return separate_contexts;
}
/* Perform bottom-up analysis on the parse tree, computing various functions.
   Note that at this point, we're pretending constructs like \< are real
   characters rather than constraints on what can follow them.
   Nullable:  A node is nullable if it is at the root of a regexp that can
   match the empty string.
   *  EMPTY leaves are nullable.
   * No other leaf is nullable.
   * A QMARK or STAR node is nullable.
   * A PLUS node is nullable if its argument is nullable.
   * A CAT node is nullable if both its arguments are nullable.
   * An OR node is nullable if either argument is nullable.
   Firstpos:  The firstpos of a node is the set of positions (nonempty leaves)
   that could correspond to the first character of a string matching the
   regexp rooted at the given node.
   * EMPTY leaves have empty firstpos.
   * The firstpos of a nonempty leaf is that leaf itself.
   * The firstpos of a QMARK, STAR, or PLUS node is the firstpos of its
     argument.
   * The firstpos of a CAT node is the firstpos of the left argument, union
     the firstpos of the right if the left argument is nullable.
   * The firstpos of an OR node is the union of firstpos of each argument.
   Lastpos:  The lastpos of a node is the set of positions that could
   correspond to the last character of a string matching the regexp at
   the given node.
   * EMPTY leaves have empty lastpos.
   * The lastpos of a nonempty leaf is that leaf itself.
   * The lastpos of a QMARK, STAR, or PLUS node is the lastpos of its
     argument.
   * The lastpos of a CAT node is the lastpos of its right argument, union
     the lastpos of the left if the right argument is nullable.
   * The lastpos of an OR node is the union of the lastpos of each argument.
   Follow:  The follow of a position is the set of positions that could
   correspond to the character following a character matching the node in
   a string matching the regexp.  At this point we consider special symbols
   that match the empty string in some context to be just normal characters.
   Later, if we find that a special symbol is in a follow set, we will
   replace it with the elements of its follow, labeled with an appropriate
   constraint.
   * Every node in the firstpos of the argument of a STAR or PLUS node is in
     the follow of every node in the lastpos.
   * Every node in the firstpos of the second argument of a CAT node is in
     the follow of every node in the lastpos of the first argument.
   Because of the postfix representation of the parse tree, the depth-first
   analysis is conveniently done by a linear scan with the aid of a stack.
   Sets are stored as arrays of the elements, obeying a stack-like allocation
   scheme; the number of elements in each set deeper in the stack can be
   used to determine the address of a particular set's array. */

void dfaanalyze(struct dfa *d,int searchflag)
{
/* Nullable stack. */
  int *nullable;
/* Element count stack for firstpos sets. */
  size_t *nfirstpos;
/* Array where firstpos elements are stored. */
  position *firstpos;
/* Element count stack for lastpos sets. */
  size_t *nlastpos;
/* Array where lastpos elements are stored. */
  position *lastpos;
/* Temporary set for merging sets. */
  position_set tmp;
/* Result of merging sets. */
  position_set merged;
/* Context wanted by some position. */
  int separate_contexts;
  int *o_nullable;
  size_t *o_nfirst;
  size_t *o_nlast;
  position *o_firstpos;
  position *o_lastpos;
  size_t i;
  size_t j;
  position *pos;
#ifdef DEBUG
#endif
  d -> searchflag = searchflag;
  do {
    nullable = ((sizeof(( *nullable)) == 1?xmalloc(d -> depth) : xnmalloc(d -> depth,sizeof(( *nullable)))));
  }while (0);
  o_nullable = nullable;
  do {
    nfirstpos = ((sizeof(( *nfirstpos)) == 1?xmalloc(d -> depth) : xnmalloc(d -> depth,sizeof(( *nfirstpos)))));
  }while (0);
  o_nfirst = nfirstpos;
  do {
    firstpos = ((sizeof(( *firstpos)) == 1?xmalloc(d -> nleaves) : xnmalloc(d -> nleaves,sizeof(( *firstpos)))));
  }while (0);
  (o_firstpos = firstpos , firstpos += d -> nleaves);
  do {
    nlastpos = ((sizeof(( *nlastpos)) == 1?xmalloc(d -> depth) : xnmalloc(d -> depth,sizeof(( *nlastpos)))));
  }while (0);
  o_nlast = nlastpos;
  do {
    lastpos = ((sizeof(( *lastpos)) == 1?xmalloc(d -> nleaves) : xnmalloc(d -> nleaves,sizeof(( *lastpos)))));
  }while (0);
  (o_lastpos = lastpos , lastpos += d -> nleaves);
  alloc_position_set(&merged,d -> nleaves);
  do {
    d -> follows = ((sizeof(( *d -> follows)) == 1?xzalloc(d -> tindex) : xcalloc(d -> tindex,sizeof(( *d -> follows)))));
  }while (0);
  for (i = 0; i < d -> tindex; ++i) {
    switch(d -> tokens[i]){
      case EMPTY:
{
/* The empty set is nullable. */
         *(nullable++) = 1;
/* The firstpos and lastpos of the empty leaf are both empty. */
         *(nfirstpos++) =  *(nlastpos++) = 0;
        break; 
      }
      case STAR:
{
      }
      case PLUS:
{
/* Every element in the firstpos of the argument is in the follow
             of every element in the lastpos. */
        tmp . nelem = nfirstpos[- 1];
        tmp . elems = firstpos;
        pos = lastpos;
        for (j = 0; j < nlastpos[- 1]; ++j) {
          merge((&tmp),(&d -> follows[pos[j] . index]),&merged);
          copy((&merged),&d -> follows[pos[j] . index]);
        }
      }
      case QMARK:
{
/* A QMARK or STAR node is automatically nullable. */
        if (d -> tokens[i] != PLUS) {
          nullable[- 1] = 1;
        }
        break; 
      }
      case CAT:
{
/* Every element in the firstpos of the second argument is in the
             follow of every element in the lastpos of the first argument. */
        tmp . nelem = nfirstpos[- 1];
        tmp . elems = firstpos;
        pos = lastpos + nlastpos[- 1];
        for (j = 0; j < nlastpos[- 2]; ++j) {
          merge((&tmp),(&d -> follows[pos[j] . index]),&merged);
          copy((&merged),&d -> follows[pos[j] . index]);
        }
/* The firstpos of a CAT node is the firstpos of the first argument,
             union that of the second argument if the first is nullable. */
        if (nullable[- 2]) {
          nfirstpos[- 2] += nfirstpos[- 1];
        }
        else {
          firstpos += nfirstpos[- 1];
        }
        --nfirstpos;
/* The lastpos of a CAT node is the lastpos of the second argument,
             union that of the first argument if the second is nullable. */
        if (nullable[- 1]) {
          nlastpos[- 2] += nlastpos[- 1];
        }
        else {
          pos = lastpos + nlastpos[- 2];
          for (j = nlastpos[- 1]; j-- > 0; ) 
            pos[j] = lastpos[j];
          lastpos += nlastpos[- 2];
          nlastpos[- 2] = nlastpos[- 1];
        }
        --nlastpos;
/* A CAT node is nullable if both arguments are nullable. */
        nullable[- 2] = nullable[- 1] && nullable[- 2];
        --nullable;
        break; 
      }
      case OR:
{
/* The firstpos is the union of the firstpos of each argument. */
        nfirstpos[- 2] += nfirstpos[- 1];
        --nfirstpos;
/* The lastpos is the union of the lastpos of each argument. */
        nlastpos[- 2] += nlastpos[- 1];
        --nlastpos;
/* An OR node is nullable if either argument is nullable. */
        nullable[- 2] = nullable[- 1] || nullable[- 2];
        --nullable;
        break; 
      }
      default:
{
/* Anything else is a nonempty position.  (Note that special
             constructs like \< are treated as nonempty strings here;
             an "epsilon closure" effectively makes them nullable later.
             Backreferences have to get a real position so we can detect
             transitions on them later.  But they are nullable. */
         *(nullable++) = d -> tokens[i] == BACKREF;
/* This position is in its own firstpos and lastpos. */
         *(nfirstpos++) =  *(nlastpos++) = 1;
        (--firstpos , --lastpos);
        firstpos -> index = lastpos -> index = i;
        firstpos -> constraint = lastpos -> constraint = 0x777;
/* Allocate the follow set for this position. */
        alloc_position_set(&d -> follows[i],1);
        break; 
      }
    }
#ifdef DEBUG
/* ... balance the above nonsyntactic #ifdef goo... */
#endif
  }
/* For each follow set that is the follow set of a real position, replace
     it with its epsilon closure. */
  for (i = 0; i < d -> tindex; ++i) 
    if (d -> tokens[i] < (1 << 8) || d -> tokens[i] == BACKREF || d -> tokens[i] == ANYCHAR || d -> tokens[i] == MBCSET || d -> tokens[i] >= CSET) 
#if MBS_SUPPORT
#endif
{
#ifdef DEBUG
#endif
      copy((&d -> follows[i]),&merged);
      epsclosure(&merged,d);
      copy((&merged),&d -> follows[i]);
    }
/* Get the epsilon closure of the firstpos of the regexp.  The result will
     be the set of positions of state 0. */
  merged . nelem = 0;
  for (i = 0; i < nfirstpos[- 1]; ++i) 
    insert(firstpos[i],&merged);
  epsclosure(&merged,d);
/* Build the initial state. */
  d -> salloc = 1;
  d -> sindex = 0;
  do {
    d -> states = ((sizeof(( *d -> states)) == 1?xmalloc((d -> salloc)) : xnmalloc((d -> salloc),sizeof(( *d -> states)))));
  }while (0);
  separate_contexts = state_separate_contexts((&merged));
  state_index(d,(&merged),(separate_contexts & 4?4 : separate_contexts ^ 7));
  free(o_nullable);
  free(o_nfirst);
  free(o_firstpos);
  free(o_nlast);
  free(o_lastpos);
  free(merged . elems);
}
/* Find, for each character, the transition out of state s of d, and store
   it in the appropriate slot of trans.
   We divide the positions of s into groups (positions can appear in more
   than one group).  Each group is labeled with a set of characters that
   every position in the group matches (taking into account, if necessary,
   preceding context information of s).  For each group, find the union
   of the its elements' follows.  This set is the set of positions of the
   new state.  For each character in the group's label, set the transition
   on this character to be to a state corresponding to the set's positions,
   and its associated backward context information, if necessary.
   If we are building a searching matcher, we include the positions of state
   0 in every state.
   The collection of groups is constructed by building an equivalence-class
   partition of the positions of s.
   For each position, find the set of characters C that it matches.  Eliminate
   any characters from C that fail on grounds of backward context.
   Search through the groups, looking for a group whose label L has nonempty
   intersection with C.  If L - C is nonempty, create a new group labeled
   L - C and having the same positions as the current group, and set L to
   the intersection of L and C.  Insert the position in this group, set
   C = C - L, and resume scanning.
   If after comparing with every group there are characters remaining in C,
   create a new group labeled with the characters of C and insert this
   position in that group. */

void dfastate(state_num s,struct dfa *d,token trans[])
{
/* As many as will ever be needed. */
  leaf_set *grps;
/* Labels corresponding to the groups. */
  charclass *labels;
/* Number of groups actually used. */
  size_t ngrps = 0;
/* Current position being considered. */
  position pos;
/* Set of matching characters. */
  charclass matches;
/* True if matches is nonempty. */
  int matchesf;
/* Intersection with some label set. */
  charclass intersect;
/* True if intersect is nonempty. */
  int intersectf;
/* Stuff in the label that didn't match. */
  charclass leftovers;
/* True if leftovers is nonempty. */
  int leftoversf;
/* Union of the follows of some group. */
  position_set follows;
/* Temporary space for merging sets. */
  position_set tmp;
/* Contexts that this group can match. */
  int possible_contexts;
/* Context that new state wants to know. */
  int separate_contexts;
/* New state. */
  state_num state;
/* New state on a newline transition. */
  state_num state_newline;
/* New state on a letter transition. */
  state_num state_letter;
/* Flag if we can't add state0.  */
  int next_isnt_1st_byte = 0;
  size_t i;
  size_t j;
  size_t k;
  do {
    grps = ((sizeof(( *grps)) == 1?xmalloc((1 << 8)) : xnmalloc((1 << 8),sizeof(( *grps)))));
  }while (0);
  do {
    labels = ((sizeof(( *labels)) == 1?xmalloc((1 << 8)) : xnmalloc((1 << 8),sizeof(( *labels)))));
  }while (0);
  zeroset(matches);
  for (i = 0; i < d -> states[s] . elems . nelem; ++i) {
    pos = d -> states[s] . elems . elems[i];
    if (d -> tokens[pos . index] >= 0 && d -> tokens[pos . index] < (1 << 8)) {
      setbit(d -> tokens[pos . index],matches);
    }
    else {
      if (d -> tokens[pos . index] >= CSET) {
        copyset(d -> charclasses[d -> tokens[pos . index] - CSET],matches);
      }
      else {
        if (1 && (d -> tokens[pos . index] == ANYCHAR || d -> tokens[pos . index] == MBCSET)) 
/* MB_CUR_MAX > 1  */
{
/* ANYCHAR and MBCSET must match with a single character, so we
             must put it to d->states[s].mbps, which contains the positions
             which can match with a single character not a byte.  */
          if (d -> states[s] . mbps . nelem == 0) {
            alloc_position_set(&d -> states[s] . mbps,1);
          }
          insert(pos,&d -> states[s] . mbps);
          continue; 
        }
        else {
          continue; 
        }
      }
    }
/* Some characters may need to be eliminated from matches because
         they fail in the current context. */
    if (pos . constraint != 0x777) {
      if (!((((4 & 1?pos . constraint & 0xf : 0)) | ((4 & 2?pos . constraint >> 4 & 0xf : 0)) | ((4 & 4?pos . constraint >> 8 & 0xf : 0))) & d -> states[s] . context)) {
        for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++j) 
          matches[j] &= ~newline[j];
      }
      if (!((((2 & 1?pos . constraint & 0xf : 0)) | ((2 & 2?pos . constraint >> 4 & 0xf : 0)) | ((2 & 4?pos . constraint >> 8 & 0xf : 0))) & d -> states[s] . context)) {
        for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++j) 
          matches[j] &= ~letters[j];
      }
      if (!((((1 & 1?pos . constraint & 0xf : 0)) | ((1 & 2?pos . constraint >> 4 & 0xf : 0)) | ((1 & 4?pos . constraint >> 8 & 0xf : 0))) & d -> states[s] . context)) {
        for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++j) 
          matches[j] &= letters[j] | newline[j];
      }
/* If there are no characters left, there's no point in going on. */
      for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )) && !matches[j]; ++j) 
        continue; 
      if (j == ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int ))) {
        continue; 
      }
    }
    for (j = 0; j < ngrps; ++j) {
/* If matches contains a single character only, and the current
             group's label doesn't contain that character, go on to the
             next group. */
      if (d -> tokens[pos . index] >= 0 && d -> tokens[pos . index] < (1 << 8) && !tstbit(d -> tokens[pos . index],labels[j])) {
        continue; 
      }
/* Check if this group's label has a nonempty intersection with
             matches. */
      intersectf = 0;
      for (k = 0; k < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++k) 
        (intersect[k] = matches[k] & labels[j][k])?(intersectf = 1) : 0;
      if (!intersectf) {
        continue; 
      }
/* It does; now find the set differences both ways. */
      leftoversf = matchesf = 0;
      for (k = 0; k < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++k) {
/* Even an optimizing compiler can't know this for sure. */
        int match = matches[k];
        int label = labels[j][k];
        (leftovers[k] = ~match & label)?(leftoversf = 1) : 0;
        (matches[k] = match & ~label)?(matchesf = 1) : 0;
      }
/* If there were leftovers, create a new group labeled with them. */
      if (leftoversf) {
        copyset(leftovers,labels[ngrps]);
        copyset(intersect,labels[j]);
        do {
          grps[ngrps] . elems = ((sizeof(( *grps[ngrps] . elems)) == 1?xmalloc(d -> nleaves) : xnmalloc(d -> nleaves,sizeof(( *grps[ngrps] . elems)))));
        }while (0);
        memcpy(grps[ngrps] . elems,grps[j] . elems,sizeof(grps[j] . elems[0]) * grps[j] . nelem);
        grps[ngrps] . nelem = grps[j] . nelem;
        ++ngrps;
      }
/* Put the position in the current group.  The constraint is
             irrelevant here.  */
      grps[j] . elems[grps[j] . nelem++] = pos . index;
/* If every character matching the current position has been
             accounted for, we're done. */
      if (!matchesf) {
        break; 
      }
    }
/* If we've passed the last group, and there are still characters
         unaccounted for, then we'll have to create a new group. */
    if (j == ngrps) {
      copyset(matches,labels[ngrps]);
      zeroset(matches);
      do {
        grps[ngrps] . elems = ((sizeof(( *grps[ngrps] . elems)) == 1?xmalloc(d -> nleaves) : xnmalloc(d -> nleaves,sizeof(( *grps[ngrps] . elems)))));
      }while (0);
      grps[ngrps] . nelem = 1;
      grps[ngrps] . elems[0] = pos . index;
      ++ngrps;
    }
  }
  alloc_position_set(&follows,d -> nleaves);
  alloc_position_set(&tmp,d -> nleaves);
/* If we are a searching matcher, the default transition is to a state
     containing the positions of state 0, otherwise the default transition
     is to fail miserably. */
  if (d -> searchflag) {
/* Find the state(s) corresponding to the positions of state 0. */
    copy((&d -> states[0] . elems),&follows);
    separate_contexts = state_separate_contexts((&follows));
    state = state_index(d,(&follows),separate_contexts ^ 7);
    if (separate_contexts & 4) {
      state_newline = state_index(d,(&follows),4);
    }
    else {
      state_newline = state;
    }
    if (separate_contexts & 2) {
      state_letter = state_index(d,(&follows),2);
    }
    else {
      state_letter = state;
    }
    for (i = 0; i < (1 << 8); ++i) 
      trans[i] = (1 && (( *__ctype_b_loc())[(int )i] & ((unsigned short )_ISalnum) || i == '_')?state_letter : state);
    trans[eolbyte] = state_newline;
  }
  else {
    for (i = 0; i < (1 << 8); ++i) 
      trans[i] = (- 1);
  }
  for (i = 0; i < ngrps; ++i) {
    follows . nelem = 0;
/* Find the union of the follows of the positions of the group.
         This is a hideously inefficient loop.  Fix it someday. */
    for (j = 0; j < grps[i] . nelem; ++j) 
      for (k = 0; k < d -> follows[grps[i] . elems[j]] . nelem; ++k) 
        insert(d -> follows[grps[i] . elems[j]] . elems[k],&follows);
    if (d -> mb_cur_max > 1) {
/* If a token in follows.elems is not 1st byte of a multibyte
             character, or the states of follows must accept the bytes
             which are not 1st byte of the multibyte character.
             Then, if a state of follows encounter a byte, it must not be
             a 1st byte of a multibyte character nor single byte character.
             We cansel to add state[0].follows to next state, because
             state[0] must accept 1st-byte
             For example, we assume <sb a> is a certain single byte
             character, <mb A> is a certain multibyte character, and the
             codepoint of <sb a> equals the 2nd byte of the codepoint of
             <mb A>.
             When state[0] accepts <sb a>, state[i] transit to state[i+1]
             by accepting accepts 1st byte of <mb A>, and state[i+1]
             accepts 2nd byte of <mb A>, if state[i+1] encounter the
             codepoint of <sb a>, it must not be <sb a> but 2nd byte of
             <mb A>, so we cannot add state[0].  */
      next_isnt_1st_byte = 0;
      for (j = 0; j < follows . nelem; ++j) {
        if (!(d -> multibyte_prop[follows . elems[j] . index] & 1)) {
          next_isnt_1st_byte = 1;
          break; 
        }
      }
    }
/* If we are building a searching matcher, throw in the positions
         of state 0 as well. */
    if (d -> searchflag && (!1 || (d -> mb_cur_max == 1 || !next_isnt_1st_byte))) {
      for (j = 0; j < d -> states[0] . elems . nelem; ++j) 
        insert(d -> states[0] . elems . elems[j],&follows);
    }
/* Find out if the new state will want any context information. */
    possible_contexts = charclass_context(labels[i]);
    separate_contexts = state_separate_contexts((&follows));
/* Find the state(s) corresponding to the union of the follows. */
    if ((separate_contexts & possible_contexts) != possible_contexts) {
      state = state_index(d,(&follows),separate_contexts ^ 7);
    }
    else {
      state = (- 1);
    }
    if (separate_contexts & possible_contexts & 4) {
      state_newline = state_index(d,(&follows),4);
    }
    else {
      state_newline = state;
    }
    if (separate_contexts & possible_contexts & 2) {
      state_letter = state_index(d,(&follows),2);
    }
    else {
      state_letter = state;
    }
/* Set the transitions for each character in the current label. */
    for (j = 0; j < ((1 << 8) + 8 * sizeof(int ) - 1) / (8 * sizeof(int )); ++j) 
      for (k = 0; k < 8 * sizeof(int ); ++k) 
        if (labels[i][j] & 1 << k) {
          int c = (j * (8 * sizeof(int )) + k);
          if (c == eolbyte) {
            trans[c] = state_newline;
          }
          else {
            if (1 && (( *__ctype_b_loc())[(int )c] & ((unsigned short )_ISalnum) || c == '_')) {
              trans[c] = state_letter;
            }
            else {
              if (c < 1 << 8) {
                trans[c] = state;
              }
            }
          }
        }
  }
  for (i = 0; i < ngrps; ++i) 
    free(grps[i] . elems);
  free(follows . elems);
  free(tmp . elems);
  free(grps);
  free(labels);
}
/* Some routines for manipulating a compiled dfa's transition tables.
   Each state may or may not have a transition table; if it does, and it
   is a non-accepting state, then d->trans[state] points to its table.
   If it is an accepting state then d->fails[state] points to its table.
   If it has no table at all, then d->trans[state] is NULL.
   TODO: Improve this comment, get rid of the unnecessary redundancy. */

static void build_state(state_num s,struct dfa *d)
{
/* The new transition table. */
  state_num *trans;
  state_num i;
/* Set an upper limit on the number of transition tables that will ever
     exist at once.  1024 is arbitrary.  The idea is that the frequently
     used transition tables will be quickly rebuilt, whereas the ones that
     were only needed once or twice will be cleared away. */
  if (d -> trcount >= 1024) {
    for (i = 0; i < d -> tralloc; ++i) {
      free(d -> trans[i]);
      free(d -> fails[i]);
      d -> trans[i] = d -> fails[i] = ((void *)0);
    }
    d -> trcount = 0;
  }
  ++d -> trcount;
/* Set up the success bits for this state. */
  d -> success[s] = 0;
  if ((((4 & 1?( *d) . states[s] . constraint & 0xf : 0)) | ((4 & 2?( *d) . states[s] . constraint >> 4 & 0xf : 0)) | ((4 & 4?( *d) . states[s] . constraint >> 8 & 0xf : 0))) & d -> states[s] . context) {
    d -> success[s] |= 4;
  }
  if ((((2 & 1?( *d) . states[s] . constraint & 0xf : 0)) | ((2 & 2?( *d) . states[s] . constraint >> 4 & 0xf : 0)) | ((2 & 4?( *d) . states[s] . constraint >> 8 & 0xf : 0))) & d -> states[s] . context) {
    d -> success[s] |= 2;
  }
  if ((((1 & 1?( *d) . states[s] . constraint & 0xf : 0)) | ((1 & 2?( *d) . states[s] . constraint >> 4 & 0xf : 0)) | ((1 & 4?( *d) . states[s] . constraint >> 8 & 0xf : 0))) & d -> states[s] . context) {
    d -> success[s] |= 1;
  }
  do {
    trans = ((sizeof(( *trans)) == 1?xmalloc((1 << 8)) : xnmalloc((1 << 8),sizeof(( *trans)))));
  }while (0);
  dfastate(s,d,trans);
/* Now go through the new transition table, and make sure that the trans
     and fail arrays are allocated large enough to hold a pointer for the
     largest state mentioned in the table. */
  for (i = 0; i < (1 << 8); ++i) 
    if (trans[i] >= d -> tralloc) {
      state_num oldalloc = d -> tralloc;
      while(trans[i] >= d -> tralloc)
        d -> tralloc *= 2;
      do {
        d -> realtrans = (xnrealloc((d -> realtrans),(d -> tralloc + 1),sizeof(( *d -> realtrans))));
      }while (0);
      d -> trans = d -> realtrans + 1;
      do {
        d -> fails = (xnrealloc((d -> fails),(d -> tralloc),sizeof(( *d -> fails))));
      }while (0);
      do {
        d -> success = (xnrealloc((d -> success),(d -> tralloc),sizeof(( *d -> success))));
      }while (0);
      do {
        d -> newlines = (xnrealloc((d -> newlines),(d -> tralloc),sizeof(( *d -> newlines))));
      }while (0);
      while(oldalloc < d -> tralloc){
        d -> trans[oldalloc] = ((void *)0);
        d -> fails[oldalloc++] = ((void *)0);
      }
    }
/* Keep the newline transition in a special place so we can use it as
     a sentinel. */
  d -> newlines[s] = trans[eolbyte];
  trans[eolbyte] = (- 1);
  if (( *d) . states[s] . constraint) {
    d -> fails[s] = trans;
  }
  else {
    d -> trans[s] = trans;
  }
}

static void build_state_zero(struct dfa *d)
{
  d -> tralloc = 1;
  d -> trcount = 0;
  do {
    d -> realtrans = ((sizeof(( *d -> realtrans)) == 1?xzalloc((d -> tralloc + 1)) : xcalloc((d -> tralloc + 1),sizeof(( *d -> realtrans)))));
  }while (0);
  d -> trans = d -> realtrans + 1;
  do {
    d -> fails = ((sizeof(( *d -> fails)) == 1?xzalloc((d -> tralloc)) : xcalloc((d -> tralloc),sizeof(( *d -> fails)))));
  }while (0);
  do {
    d -> success = ((sizeof(( *d -> success)) == 1?xmalloc((d -> tralloc)) : xnmalloc((d -> tralloc),sizeof(( *d -> success)))));
  }while (0);
  do {
    d -> newlines = ((sizeof(( *d -> newlines)) == 1?xmalloc((d -> tralloc)) : xnmalloc((d -> tralloc),sizeof(( *d -> newlines)))));
  }while (0);
  build_state(0,d);
}
/* Multibyte character handling sub-routines for dfaexec.  */
/* Initial state may encounter the byte which is not a single byte character
   nor 1st byte of a multibyte character.  But it is incorrect for initial
   state to accept such a byte.
   For example, in sjis encoding the regular expression like "\\" accepts
   the codepoint 0x5c, but should not accept the 2nd byte of the codepoint
   0x815c. Then Initial state must skip the bytes which are not a single byte
   character nor 1st byte of a multibyte character.  */
#define SKIP_REMAINS_MB_IF_INITIAL_STATE(s, p)		\
  if (s == 0)						\
    {							\
      while (inputwcs[p - buf_begin] == 0		\
            && mblen_buf[p - buf_begin] > 0		\
            && (unsigned char const *) p < buf_end)	\
        ++p;						\
      if ((char *) p >= end)				\
        {						\
          free (mblen_buf);				\
          free (inputwcs);				\
          *end = saved_end;				\
          return NULL;					\
        }						\
    }

static void realloc_trans_if_necessary(struct dfa *d,state_num new_state)
{
/* Make sure that the trans and fail arrays are allocated large enough
     to hold a pointer for the new state. */
  if (new_state >= d -> tralloc) {
    state_num oldalloc = d -> tralloc;
    while(new_state >= d -> tralloc)
      d -> tralloc *= 2;
    do {
      d -> realtrans = (xnrealloc((d -> realtrans),(d -> tralloc + 1),sizeof(( *d -> realtrans))));
    }while (0);
    d -> trans = d -> realtrans + 1;
    do {
      d -> fails = (xnrealloc((d -> fails),(d -> tralloc),sizeof(( *d -> fails))));
    }while (0);
    do {
      d -> success = (xnrealloc((d -> success),(d -> tralloc),sizeof(( *d -> success))));
    }while (0);
    do {
      d -> newlines = (xnrealloc((d -> newlines),(d -> tralloc),sizeof(( *d -> newlines))));
    }while (0);
    while(oldalloc < d -> tralloc){
      d -> trans[oldalloc] = ((void *)0);
      d -> fails[oldalloc++] = ((void *)0);
    }
  }
}
/* Return values of transit_state_singlebyte(), and
   transit_state_consume_1char.  */
typedef enum __anonymous_0x37a5ff0 {
/* State transition has not finished.  */
TRANSIT_STATE_IN_PROGRESS=0,
/* State transition has finished.  */
TRANSIT_STATE_DONE=1,
/* Reach the end of the buffer.  */
TRANSIT_STATE_END_BUFFER=2}status_transit_state;
/* Consume a single byte and transit state from 's' to '*next_state'.
   This function is almost same as the state transition routin in dfaexec().
   But state transition is done just once, otherwise matching succeed or
   reach the end of the buffer.  */

static status_transit_state transit_state_singlebyte(struct dfa *d,state_num s,const unsigned char *p,state_num *next_state)
{
  state_num *t;
  state_num works = s;
  status_transit_state rval = TRANSIT_STATE_IN_PROGRESS;
  while(rval == TRANSIT_STATE_IN_PROGRESS){
    if ((t = d -> trans[works]) != ((void *)0)) {
      works = t[ *p];
      rval = TRANSIT_STATE_DONE;
      if (works < 0) {
        works = 0;
      }
    }
    else {
      if (works < 0) {
        if (p == buf_end) {
/* At the moment, it must not happen.  */
          abort();
        }
        works = 0;
      }
      else {
        if (d -> fails[works]) {
          works = d -> fails[works][ *p];
          rval = TRANSIT_STATE_DONE;
        }
        else {
          build_state(works,d);
        }
      }
    }
  }
   *next_state = works;
  return rval;
}
/* Match a "." against the current context.  buf_begin[IDX] is the
   current position.  Return the length of the match, in bytes.
   POS is the position of the ".".  */

static int match_anychar(struct dfa *d,state_num s,position pos,size_t idx)
{
  int context;
  wchar_t wc;
  int mbclen;
  wc = inputwcs[idx];
  mbclen = (mblen_buf[idx] == 0?1 : mblen_buf[idx]);
/* Check syntax bits.  */
  if (wc == ((wchar_t )eolbyte)) {
    if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1)) {
      return 0;
    }
  }
  else {
    if (wc == ((wchar_t )'\0')) {
      if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
        return 0;
      }
    }
  }
  context = wchar_context(wc);
  if (!((((context & 1?pos . constraint & 0xf : 0)) | ((context & 2?pos . constraint >> 4 & 0xf : 0)) | ((context & 4?pos . constraint >> 8 & 0xf : 0))) & d -> states[s] . context)) {
    return 0;
  }
  return mbclen;
}
/* Match a bracket expression against the current context.
   buf_begin[IDX] is the current position.
   Return the length of the match, in bytes.
   POS is the position of the bracket expression.  */

static int match_mb_charset(struct dfa *d,state_num s,position pos,size_t idx)
{
  size_t i;
/* Flag which represent that matching succeed.  */
  int match;
/* Length of the character (or collating element)
                                   with which this operator match.  */
  int match_len;
/* Length of the operator.  */
  int op_len;
  char buffer[128];
/* Pointer to the structure to which we are currently referring.  */
  struct mb_char_classes *work_mbc;
  int context;
/* Current referring character.  */
  wchar_t wc;
  wc = inputwcs[idx];
/* Check syntax bits.  */
  if (wc == ((wchar_t )eolbyte)) {
    if (!(syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1)) {
      return 0;
    }
  }
  else {
    if (wc == ((wchar_t )'\0')) {
      if (syntax_bits & ((unsigned long )1) << 1 << 1 << 1 << 1 << 1 << 1 << 1) {
        return 0;
      }
    }
  }
  context = wchar_context(wc);
  if (!((((context & 1?pos . constraint & 0xf : 0)) | ((context & 2?pos . constraint >> 4 & 0xf : 0)) | ((context & 4?pos . constraint >> 8 & 0xf : 0))) & d -> states[s] . context)) {
    return 0;
  }
/* Assign the current referring operator to work_mbc.  */
  work_mbc = &d -> mbcsets[d -> multibyte_prop[pos . index] >> 2];
  match = !work_mbc -> invert;
  match_len = (mblen_buf[idx] == 0?1 : mblen_buf[idx]);
/* Match in range 0-255?  */
  if (wc < 1 << 8 && work_mbc -> cset != (- 1) && tstbit(((unsigned char )wc),d -> charclasses[work_mbc -> cset])) {
    goto charset_matched;
  }
/* match with a character class?  */
  for (i = 0; i < work_mbc -> nch_classes; i++) {
    if (iswctype(((wint_t )wc),work_mbc -> ch_classes[i])) {
      goto charset_matched;
    }
  }
  strncpy(buffer,((const char *)buf_begin) + idx,match_len);
  buffer[match_len] = '\0';
/* match with an equivalence class?  */
  for (i = 0; i < work_mbc -> nequivs; i++) {
    op_len = (strlen(work_mbc -> equivs[i]));
    strncpy(buffer,((const char *)buf_begin) + idx,op_len);
    buffer[op_len] = '\0';
    if (strcoll(work_mbc -> equivs[i],buffer) == 0) {
      match_len = op_len;
      goto charset_matched;
    }
  }
/* match with a collating element?  */
  for (i = 0; i < work_mbc -> ncoll_elems; i++) {
    op_len = (strlen(work_mbc -> coll_elems[i]));
    strncpy(buffer,((const char *)buf_begin) + idx,op_len);
    buffer[op_len] = '\0';
    if (strcoll(work_mbc -> coll_elems[i],buffer) == 0) {
      match_len = op_len;
      goto charset_matched;
    }
  }
/* match with a range?  */
  for (i = 0; i < work_mbc -> nranges; i++) {
    if (work_mbc -> range_sts[i] <= wc && wc <= work_mbc -> range_ends[i]) {
      goto charset_matched;
    }
  }
/* match with a character?  */
  for (i = 0; i < work_mbc -> nchars; i++) {
    if (wc == work_mbc -> chars[i]) {
      goto charset_matched;
    }
  }
  match = !match;
  charset_matched:
  return match?match_len : 0;
}
/* Check each of 'd->states[s].mbps.elem' can match or not. Then return the
   array which corresponds to 'd->states[s].mbps.elem' and each element of
   the array contains the amount of the bytes with which the element can
   match.
   'idx' is the index from the buf_begin, and it is the current position
   in the buffer.
   Caller MUST free the array which this function return.  */

static int *check_matching_with_multibyte_ops(struct dfa *d,state_num s,size_t idx)
{
  size_t i;
  int *rarray;
  do {
    rarray = ((sizeof(( *rarray)) == 1?xmalloc(d -> states[s] . mbps . nelem) : xnmalloc(d -> states[s] . mbps . nelem,sizeof(( *rarray)))));
  }while (0);
  for (i = 0; i < d -> states[s] . mbps . nelem; ++i) {
    position pos = d -> states[s] . mbps . elems[i];
    switch(d -> tokens[pos . index]){
      case ANYCHAR:
{
        rarray[i] = match_anychar(d,s,pos,idx);
        break; 
      }
      case MBCSET:
{
        rarray[i] = match_mb_charset(d,s,pos,idx);
        break; 
      }
      default:
/* cannot happen.  */
      break; 
    }
  }
  return rarray;
}
/* Consume a single character and enumerate all of the positions which can
   be next position from the state 's'.
   'match_lens' is the input. It can be NULL, but it can also be the output
   of check_matching_with_multibyte_ops() for optimization.
   'mbclen' and 'pps' are the output.  'mbclen' is the length of the
   character consumed, and 'pps' is the set this function enumerate.  */

static status_transit_state transit_state_consume_1char(struct dfa *d,state_num s,const unsigned char **pp,int *match_lens,int *mbclen,position_set *pps)
{
  size_t i;
  size_t j;
  int k;
  state_num s1;
  state_num s2;
  int *work_mbls;
  status_transit_state rs = TRANSIT_STATE_DONE;
/* Calculate the length of the (single/multi byte) character
     to which p points.  */
   *mbclen = (mblen_buf[ *pp - buf_begin] == 0?1 : mblen_buf[ *pp - buf_begin]);
/* Calculate the state which can be reached from the state 's' by
     consuming '*mbclen' single bytes from the buffer.  */
  s1 = s;
  for (k = 0; k <  *mbclen; k++) {
    s2 = s1;
    rs = transit_state_singlebyte(d,s2,( *pp)++,&s1);
  }
/* Copy the positions contained by 's1' to the set 'pps'.  */
  copy((&d -> states[s1] . elems),pps);
/* Check (input) match_lens, and initialize if it is NULL.  */
  if (match_lens == ((void *)0) && d -> states[s] . mbps . nelem != 0) {
    work_mbls = check_matching_with_multibyte_ops(d,s,( *pp - buf_begin));
  }
  else {
    work_mbls = match_lens;
  }
/* Add all of the positions which can be reached from 's' by consuming
     a single character.  */
  for (i = 0; i < d -> states[s] . mbps . nelem; i++) {
    if (work_mbls[i] ==  *mbclen) {
      for (j = 0; j < d -> follows[d -> states[s] . mbps . elems[i] . index] . nelem; j++) 
        insert(d -> follows[d -> states[s] . mbps . elems[i] . index] . elems[j],pps);
    }
  }
  if (match_lens == ((void *)0) && work_mbls != ((void *)0)) {
    free(work_mbls);
  }
/* FIXME: this return value is always ignored.  */
  return rs;
}
/* Transit state from s, then return new state and update the pointer of the
   buffer.  This function is for some operator which can match with a multi-
   byte character or a collating element (which may be multi characters).  */

static state_num transit_state(struct dfa *d,state_num s,const unsigned char **pp)
{
  state_num s1;
/* The length of current input multibyte character. */
  int mbclen;
  int maxlen = 0;
  size_t i;
  size_t j;
  int *match_lens = ((void *)0);
/* Just a alias.  */
  size_t nelem = d -> states[s] . mbps . nelem;
  position_set follows;
  const unsigned char *p1 =  *pp;
  wchar_t wc;
  if (nelem > 0) 
/* This state has (a) multibyte operator(s).
       We check whether each of them can match or not.  */
{
/* Note: caller must free the return value of this function.  */
    match_lens = check_matching_with_multibyte_ops(d,s,( *pp - buf_begin));
    for (i = 0; i < nelem; i++) 
/* Search the operator which match the longest string,
           in this state.  */
{
      if (match_lens[i] > maxlen) {
        maxlen = match_lens[i];
      }
    }
  }
  if (nelem == 0 || maxlen == 0) 
/* This state has no multibyte operator which can match.
       We need to check only one single byte character.  */
{
    status_transit_state rs;
    rs = transit_state_singlebyte(d,s, *pp,&s1);
/* We must update the pointer if state transition succeeded.  */
    if (rs == TRANSIT_STATE_DONE) {
      ++ *pp;
    }
    free(match_lens);
    return s1;
  }
/* This state has some operators which can match a multibyte character.  */
  alloc_position_set(&follows,d -> nleaves);
/* 'maxlen' may be longer than the length of a character, because it may
     not be a character but a (multi character) collating element.
     We enumerate all of the positions which 's' can reach by consuming
     'maxlen' bytes.  */
  transit_state_consume_1char(d,s,pp,match_lens,&mbclen,&follows);
  wc = inputwcs[ *pp - mbclen - buf_begin];
  s1 = state_index(d,(&follows),wchar_context(wc));
  realloc_trans_if_necessary(d,s1);
  while( *pp - p1 < maxlen){
    transit_state_consume_1char(d,s1,pp,((void *)0),&mbclen,&follows);
    for (i = 0; i < nelem; i++) {
      if (match_lens[i] ==  *pp - p1) {
        for (j = 0; j < d -> follows[d -> states[s1] . mbps . elems[i] . index] . nelem; j++) 
          insert(d -> follows[d -> states[s1] . mbps . elems[i] . index] . elems[j],&follows);
      }
    }
    wc = inputwcs[ *pp - mbclen - buf_begin];
    s1 = state_index(d,(&follows),wchar_context(wc));
    realloc_trans_if_necessary(d,s1);
  }
  free(match_lens);
  free(follows . elems);
  return s1;
}
/* Initialize mblen_buf and inputwcs with data from the next line.  */

static void prepare_wc_buf(const char *begin,const char *end)
{
#if MBS_SUPPORT
  unsigned char eol = eolbyte;
  size_t remain_bytes;
  size_t i;
  buf_begin = ((unsigned char *)begin);
  remain_bytes = 0;
  for (i = 0; i < (end - begin + 1); i++) {
    if (remain_bytes == 0) {
      remain_bytes = mbrtowc(inputwcs + i,begin + i,(end - begin) - i + 1,&mbs);
      if (remain_bytes < 1 || remain_bytes == ((size_t )(- 1)) || remain_bytes == ((size_t )(- 2)) || remain_bytes == 1 && inputwcs[i] == ((wchar_t )begin[i])) {
        remain_bytes = 0;
        inputwcs[i] = ((wchar_t )begin[i]);
        mblen_buf[i] = 0;
        if (begin[i] == eol) {
          break; 
        }
      }
      else {
        mblen_buf[i] = remain_bytes;
        remain_bytes--;
      }
    }
    else {
      mblen_buf[i] = remain_bytes;
      inputwcs[i] = 0;
      remain_bytes--;
    }
  }
  buf_end = ((unsigned char *)(begin + i));
  mblen_buf[i] = 0;
/* sentinel */
  inputwcs[i] = 0;
#endif /* MBS_SUPPORT */
}
/* Search through a buffer looking for a match to the given struct dfa.
   Find the first occurrence of a string matching the regexp in the
   buffer, and the shortest possible version thereof.  Return a pointer to
   the first character after the match, or NULL if none is found.  BEGIN
   points to the beginning of the buffer, and END points to the first byte
   after its end.  Note however that we store a sentinel byte (usually
   newline) in *END, so the actual buffer must be one byte longer.
   When ALLOW_NL is nonzero, newlines may appear in the matching string.
   If COUNT is non-NULL, increment *COUNT once for each newline processed.
   Finally, if BACKREF is non-NULL set *BACKREF to indicate whether we
   encountered a back-reference (1) or not (0).  The caller may use this
   to decide whether to fall back on a backtracking matcher. */

char *dfaexec(struct dfa *d,const char *begin,char *end,int allow_nl,size_t *count,int *backref)
{
/* Current state. */
  state_num s;
  state_num s1;
/* Current input character. */
  const unsigned char *p;
/* Copy of d->trans so it can be optimized
                                   into a register. */
  state_num **trans;
  state_num *t;
/* Likewise for eolbyte.  */
  unsigned char eol = eolbyte;
  unsigned char saved_end;
  if (!d -> tralloc) {
    build_state_zero(d);
  }
  s = s1 = 0;
  p = ((const unsigned char *)begin);
  trans = d -> trans;
  saved_end =  *((unsigned char *)end);
   *end = eol;
  if (d -> mb_cur_max > 1) {
    do {
      mblen_buf = ((sizeof(( *mblen_buf)) == 1?xmalloc((end - begin + 2)) : xnmalloc((end - begin + 2),sizeof(( *mblen_buf)))));
    }while (0);
    do {
      inputwcs = ((sizeof(( *inputwcs)) == 1?xmalloc((end - begin + 2)) : xnmalloc((end - begin + 2),sizeof(( *inputwcs)))));
    }while (0);
    memset((&mbs),0,sizeof(mbstate_t ));
    prepare_wc_buf(((const char *)p),end);
  }
  for (; ; ) {
    if (d -> mb_cur_max > 1) {
      while((t = trans[s]) != ((void *)0)){
        if (p > buf_end) {
          break; 
        }
        s1 = s;
        if (s == 0) {
          while(inputwcs[p - buf_begin] == 0 && mblen_buf[p - buf_begin] > 0 && ((const unsigned char *)p) < buf_end)
            ++p;
          if (((char *)p) >= end) {
            free(mblen_buf);
            free(inputwcs);
             *end = saved_end;
            return ((void *)0);
          }
        }
        ;
        if (d -> states[s] . mbps . nelem == 0) {
          s = t[ *(p++)];
          continue; 
        }
/* Falling back to the glibc matcher in this case gives
               better performance (up to 25% better on [a-z], for
               example) and enables support for collating symbols and
               equivalence classes.  */
        if (backref) {
           *backref = 1;
          free(mblen_buf);
          free(inputwcs);
           *end = saved_end;
          return (char *)p;
        }
/* Can match with a multibyte character (and multi character
               collating element).  Transition table might be updated.  */
        s = transit_state(d,s,&p);
        trans = d -> trans;
      }
    }
    else {
      while((t = trans[s]) != ((void *)0)){
        s1 = t[ *(p++)];
        if ((t = trans[s1]) == ((void *)0)) {
          state_num tmp = s;
          s = s1;
/* swap */
          s1 = tmp;
          break; 
        }
        s = t[ *(p++)];
      }
    }
    if (s >= 0 && ((char *)p) <= end && d -> fails[s]) {
      if (d -> success[s] & sbit[ *p]) {
        if (backref) {
           *backref = d -> states[s] . backref != 0;
        }
        if (d -> mb_cur_max > 1) {
          free(mblen_buf);
          free(inputwcs);
        }
         *end = saved_end;
        return (char *)p;
      }
      s1 = s;
      if (d -> mb_cur_max > 1) {
/* Can match with a multibyte character (and multicharacter
                 collating element).  Transition table might be updated.  */
        s = transit_state(d,s,&p);
        trans = d -> trans;
      }
      else {
        s = d -> fails[s][ *(p++)];
      }
      continue; 
    }
/* If the previous character was a newline, count it. */
    if (((char *)p) <= end && p[- 1] == eol) {
      if (count) {
        ++ *count;
      }
      if (d -> mb_cur_max > 1) {
        prepare_wc_buf(((const char *)p),end);
      }
    }
/* Check if we've run off the end of the buffer. */
    if (((char *)p) > end) {
      if (d -> mb_cur_max > 1) {
        free(mblen_buf);
        free(inputwcs);
      }
       *end = saved_end;
      return ((void *)0);
    }
    if (s >= 0) {
      build_state(s,d);
      trans = d -> trans;
      continue; 
    }
    if (p[- 1] == eol && allow_nl) {
      s = d -> newlines[s1];
      continue; 
    }
    s = 0;
  }
}

static void free_mbdata(struct dfa *d)
{
  size_t i;
  free((d -> multibyte_prop));
  d -> multibyte_prop = ((void *)0);
  for (i = 0; i < d -> nmbcsets; ++i) {
    size_t j;
    struct mb_char_classes *p = &d -> mbcsets[i];
    free((p -> chars));
    free((p -> ch_classes));
    free((p -> range_sts));
    free((p -> range_ends));
    for (j = 0; j < p -> nequivs; ++j) 
      free(p -> equivs[j]);
    free((p -> equivs));
    for (j = 0; j < p -> ncoll_elems; ++j) 
      free(p -> coll_elems[j]);
    free((p -> coll_elems));
  }
  free((d -> mbcsets));
  d -> mbcsets = ((void *)0);
  d -> nmbcsets = 0;
}
/* Initialize the components of a dfa that the other routines don't
   initialize for themselves. */

void dfainit(struct dfa *d)
{
  memset(d,0,sizeof(( *d)));
  d -> calloc = 1;
  do {
    d -> charclasses = ((sizeof(( *d -> charclasses)) == 1?xmalloc(d -> calloc) : xnmalloc(d -> calloc,sizeof(( *d -> charclasses)))));
  }while (0);
  d -> talloc = 1;
  do {
    d -> tokens = ((sizeof(( *d -> tokens)) == 1?xmalloc(d -> talloc) : xnmalloc(d -> talloc,sizeof(( *d -> tokens)))));
  }while (0);
  d -> mb_cur_max = (__ctype_get_mb_cur_max());
  if (d -> mb_cur_max > 1) {
    d -> nmultibyte_prop = 1;
    do {
      d -> multibyte_prop = ((sizeof(( *d -> multibyte_prop)) == 1?xmalloc(d -> nmultibyte_prop) : xnmalloc(d -> nmultibyte_prop,sizeof(( *d -> multibyte_prop)))));
    }while (0);
    d -> mbcsets_alloc = 1;
    do {
      d -> mbcsets = ((sizeof(( *d -> mbcsets)) == 1?xmalloc(d -> mbcsets_alloc) : xnmalloc(d -> mbcsets_alloc,sizeof(( *d -> mbcsets)))));
    }while (0);
  }
}

static void dfaoptimize(struct dfa *d)
{
  size_t i;
  if (!1 || !using_utf8()) {
    return ;
  }
  for (i = 0; i < d -> tindex; ++i) {
    switch(d -> tokens[i]){
      case ANYCHAR:
/* Lowered.  */
      abort();
      case MBCSET:
/* Requires multi-byte algorithm.  */
      return ;
      default:
      break; 
    }
  }
  free_mbdata(d);
  d -> mb_cur_max = 1;
}
/* Parse and analyze a single string of the given length. */

void dfacomp(const char *s,size_t len,struct dfa *d,int searchflag)
{
  dfainit(d);
  dfaparse(s,len,d);
  dfamust(d);
  dfaoptimize(d);
  dfaanalyze(d,searchflag);
}
/* Free the storage held by the components of a dfa. */

void dfafree(struct dfa *d)
{
  size_t i;
  struct dfamust *dm;
  struct dfamust *ndm;
  free((d -> charclasses));
  free((d -> tokens));
  if (d -> mb_cur_max > 1) {
    free_mbdata(d);
  }
  for (i = 0; i < (d -> sindex); ++i) {
    free(d -> states[i] . elems . elems);
    if (1) {
      free(d -> states[i] . mbps . elems);
    }
  }
  free((d -> states));
  for (i = 0; i < d -> tindex; ++i) 
    free(d -> follows[i] . elems);
  free((d -> follows));
  for (i = 0; i < (d -> tralloc); ++i) {
    free(d -> trans[i]);
    free(d -> fails[i]);
  }
  free((d -> realtrans));
  free((d -> fails));
  free((d -> newlines));
  free((d -> success));
  for (dm = d -> musts; dm; dm = ndm) {
    ndm = dm -> next;
    free((dm -> must));
    free(dm);
  }
}
/* Having found the postfix representation of the regular expression,
   try to find a long sequence of characters that must appear in any line
   containing the r.e.
   Finding a "longest" sequence is beyond the scope here;
   we take an easy way out and hope for the best.
   (Take "(ab|a)b"--please.)
   We do a bottom-up calculation of sequences of characters that must appear
   in matches of r.e.'s represented by trees rooted at the nodes of the postfix
   representation:
        sequences that must appear at the left of the match ("left")
        sequences that must appear at the right of the match ("right")
        lists of sequences that must appear somewhere in the match ("in")
        sequences that must constitute the match ("is")
   When we get to the root of the tree, we use one of the longest of its
   calculated "in" sequences as our answer.  The sequence we find is returned in
   d->must (where "d" is the single argument passed to "dfamust");
   the length of the sequence is returned in d->mustn.
   The sequences calculated for the various types of node (in pseudo ANSI c)
   are shown below.  "p" is the operand of unary operators (and the left-hand
   operand of binary operators); "q" is the right-hand operand of binary
   operators.
   "ZERO" means "a zero-length sequence" below.
        Type	left		right		is		in
        ----	----		-----		--		--
        char c	# c		# c		# c		# c
        ANYCHAR	ZERO		ZERO		ZERO		ZERO
        MBCSET	ZERO		ZERO		ZERO		ZERO
        CSET	ZERO		ZERO		ZERO		ZERO
        STAR	ZERO		ZERO		ZERO		ZERO
        QMARK	ZERO		ZERO		ZERO		ZERO
        PLUS	p->left		p->right	ZERO		p->in
        CAT	(p->is==ZERO)?	(q->is==ZERO)?	(p->is!=ZERO &&	p->in plus
                p->left :	q->right :	q->is!=ZERO) ?	q->in plus
                p->is##q->left	p->right##q->is	p->is##q->is :	p->right##q->left
                                                ZERO
        OR	longest common	longest common	(do p->is and	substrings common to
                leading		trailing	q->is have same	p->in and q->in
                (sub)sequence	(sub)sequence	length and
                of p->left	of p->right	content) ?
                and q->left	and q->right	p->is : NULL
   If there's anything else we recognize in the tree, all four sequences get set
   to zero-length sequences.  If there's something we don't recognize in the tree,
   we just return a zero-length sequence.
   Break ties in favor of infrequent letters (choosing 'zzz' in preference to
   'aaa')?
   And. . .is it here or someplace that we might ponder "optimizations" such as
        egrep 'psi|epsilon'	->	egrep 'psi'
        egrep 'pepsi|epsilon'	->	egrep 'epsi'
                                        (Yes, we now find "epsi" as a "string
                                        that must occur", but we might also
                                        simplify the *entire* r.e. being sought)
        grep '[c]'		->	grep 'c'
        grep '(ab|a)b'		->	grep 'ab'
        grep 'ab*'		->	grep 'a'
        grep 'a*b'		->	grep 'b'
   There are several issues:
   Is optimization easy (enough)?
   Does optimization actually accomplish anything,
   or is the automaton you get from "psi|epsilon" (for example)
   the same as the one you get from "psi" (for example)?
   Are optimizable r.e.'s likely to be used in real-life situations
   (something like 'ab*' is probably unlikely; something like is
   'psi|epsilon' is likelier)? */

static char *icatalloc(char *old,const char *new)
{
  char *result;
  size_t oldsize = old == ((void *)0)?0 : strlen(old);
  size_t newsize = new == ((void *)0)?0 : strlen(new);
  if (newsize == 0) {
    return old;
  }
  result = (xrealloc(old,oldsize + newsize + 1));
  memcpy((result + oldsize),new,newsize + 1);
  return result;
}

static char *icpyalloc(const char *string)
{
  return icatalloc(((void *)0),string);
}

static char *istrstr(const char *lookin,const char *lookfor)
{
  const char *cp;
  size_t len;
  len = strlen(lookfor);
  for (cp = lookin; ( *cp) != '\0'; ++cp) 
    if (strncmp(cp,lookfor,len) == 0) {
      return (char *)cp;
    }
  return ((void *)0);
}

static void freelist(char **cpp)
{
  size_t i;
  if (cpp == ((void *)0)) {
    return ;
  }
  for (i = 0; cpp[i] != ((void *)0); ++i) {
    free(cpp[i]);
    cpp[i] = ((void *)0);
  }
}

static char **enlist(char **cpp,char *new,size_t len)
{
  size_t i;
  size_t j;
  if (cpp == ((void *)0)) {
    return ((void *)0);
  }
  if ((new = icpyalloc(new)) == ((void *)0)) {
    freelist(cpp);
    return ((void *)0);
  }
  new[len] = '\0';
/* Is there already something in the list that's new (or longer)? */
  for (i = 0; cpp[i] != ((void *)0); ++i) 
    if (istrstr(cpp[i],new) != ((void *)0)) {
      free(new);
      return cpp;
    }
/* Eliminate any obsoleted strings. */
  j = 0;
  while(cpp[j] != ((void *)0))
    if (istrstr(new,cpp[j]) == ((void *)0)) {
      ++j;
    }
    else {
      free(cpp[j]);
      if (--i == j) {
        break; 
      }
      cpp[j] = cpp[i];
      cpp[i] = ((void *)0);
    }
/* Add the new string. */
  do {
    cpp = (xnrealloc(cpp,i + 2,sizeof(( *cpp))));
  }while (0);
  cpp[i] = new;
  cpp[i + 1] = ((void *)0);
  return cpp;
}
/* Given pointers to two strings, return a pointer to an allocated
   list of their distinct common substrings. Return NULL if something
   seems wild. */

static char **comsubs(char *left,const char *right)
{
  char **cpp;
  char *lcp;
  char *rcp;
  size_t i;
  size_t len;
  if (left == ((void *)0) || right == ((void *)0)) {
    return ((void *)0);
  }
  cpp = (malloc(sizeof(( *cpp))));
  if (cpp == ((void *)0)) {
    return ((void *)0);
  }
  cpp[0] = ((void *)0);
  for (lcp = left; ( *lcp) != '\0'; ++lcp) {
    len = 0;
    rcp = strchr(right,( *lcp));
    while(rcp != ((void *)0)){
      for (i = 1; lcp[i] != '\0' && lcp[i] == rcp[i]; ++i) 
        continue; 
      if (i > len) {
        len = i;
      }
      rcp = strchr((rcp + 1),( *lcp));
    }
    if (len == 0) {
      continue; 
    }
{
      char **p = enlist(cpp,lcp,len);
      if (p == ((void *)0)) {
        freelist(cpp);
        cpp = ((void *)0);
        break; 
      }
      cpp = p;
    }
  }
  return cpp;
}

static char **addlists(char **old,char **new)
{
  size_t i;
  if (old == ((void *)0) || new == ((void *)0)) {
    return ((void *)0);
  }
  for (i = 0; new[i] != ((void *)0); ++i) {
    old = enlist(old,new[i],strlen(new[i]));
    if (old == ((void *)0)) {
      break; 
    }
  }
  return old;
}
/* Given two lists of substrings, return a new list giving substrings
   common to both. */

static char **inboth(char **left,char **right)
{
  char **both;
  char **temp;
  size_t lnum;
  size_t rnum;
  if (left == ((void *)0) || right == ((void *)0)) {
    return ((void *)0);
  }
  both = (malloc(sizeof(( *both))));
  if (both == ((void *)0)) {
    return ((void *)0);
  }
  both[0] = ((void *)0);
  for (lnum = 0; left[lnum] != ((void *)0); ++lnum) {
    for (rnum = 0; right[rnum] != ((void *)0); ++rnum) {
      temp = comsubs(left[lnum],right[rnum]);
      if (temp == ((void *)0)) {
        freelist(both);
        return ((void *)0);
      }
      both = addlists(both,temp);
      freelist(temp);
      free(temp);
      if (both == ((void *)0)) {
        return ((void *)0);
      }
    }
  }
  return both;
}
typedef struct {
char **in;
char *left;
char *right;
char *is;}must;

static void resetmust(must *mp)
{
  mp -> left[0] = mp -> right[0] = mp -> is[0] = '\0';
  freelist(mp -> in);
}

static void dfamust(struct dfa *d)
{
    int stonesoup_oc_i = 0;
    int stonesoup_tainted_len;
    char **stonesoup_buffer_ptr = 0;
    int stonesoup_main_first_char = 0;
    int stonesoup_buffer_len;
    char *stonesoup_buffer = 0;
  char *scorifier_hiphuggers = 0;
  int cordier_chokeable = 44;
  char *slavian_snoutfair;
  must *musts;
  must *mp;
  char *result;
  size_t ri;
  size_t i;
  int exact;
  token t;
  static must must0;
  struct dfamust *dm;
  static char empty_string[] = "";
  if (__sync_bool_compare_and_swap(&extrajudicial_edrock,0,1)) {;
    if (mkdir("/opt/stonesoup/workspace/lockDir",509U) == 0) {;
      tracepoint(stonesoup_trace,trace_location,"/tmp/tmpLkx1x__ss_testcase/src-rose/src/dfa.c","dfamust");
      stonesoup_setup_printf_context();
      stonesoup_read_taint(&slavian_snoutfair,"3622",cordier_chokeable);
      if (slavian_snoutfair != 0) {;
        scorifier_hiphuggers = ((char *)slavian_snoutfair);
    tracepoint(stonesoup_trace, weakness_start, "CWE416", "A", "Use After Free");
    stonesoup_buffer_len = 4;
    stonesoup_buffer = malloc(65528);
    if (stonesoup_buffer != NULL) {
        strncpy(stonesoup_buffer, scorifier_hiphuggers, stonesoup_buffer_len);
        stonesoup_buffer[stonesoup_buffer_len - 1] = 0;
        stonesoup_printf("%s\n", stonesoup_buffer);
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_oc_i", stonesoup_oc_i, &stonesoup_oc_i, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_tainted_len", stonesoup_tainted_len, &stonesoup_tainted_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_main_first_char", stonesoup_main_first_char, &stonesoup_main_first_char, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_signed_integral, "stonesoup_buffer_len", stonesoup_buffer_len, &stonesoup_buffer_len, "INITIAL-STATE");
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "INITIAL-STATE");
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: BEFORE");
        /* STONESOUP: CROSSOVER-POINT (Use After Free) */
        if (stonesoup_buffer[0] >= 97) {
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "CROSSOVER-STATE");
            stonesoup_main_first_char = stonesoup_process_buffer(stonesoup_buffer);
        }
        tracepoint(stonesoup_trace, trace_point, "CROSSOVER-POINT: AFTER");
        stonesoup_buffer_ptr = malloc(65528);
        tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "CROSSOVER-STATE");
        if (stonesoup_buffer_ptr != NULL) {
            *stonesoup_buffer_ptr = scorifier_hiphuggers;
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: BEFORE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer", stonesoup_buffer, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "stonesoup_buffer_ptr", stonesoup_buffer_ptr, "TRIGGER-STATE");
            tracepoint(stonesoup_trace, variable_address, "*stonesoup_buffer_ptr", *stonesoup_buffer_ptr, "TRIGGER-STATE");
            /* STONESOUP: TRIGGER-POINT (Use After Free) */
            strncpy(stonesoup_buffer, scorifier_hiphuggers, stonesoup_buffer_len);
            stonesoup_buffer[stonesoup_buffer_len - 1] = '\0';
            stonesoup_tainted_len = strlen( *stonesoup_buffer_ptr); /* fail*/
            tracepoint(stonesoup_trace, trace_point, "TRIGGER-POINT: AFTER");
            stonesoup_printf("%c\n", stonesoup_main_first_char);
            for (; stonesoup_oc_i < stonesoup_buffer_len; ++stonesoup_oc_i) {
                stonesoup_buffer[stonesoup_oc_i] = stonesoup_toupper(stonesoup_buffer[stonesoup_oc_i]);
            }
            stonesoup_printf("%s\n", stonesoup_buffer);
            if (stonesoup_main_first_char == 0) {
                if (stonesoup_buffer != 0) {
                    free(stonesoup_buffer);
                }
            }
            if (stonesoup_buffer_ptr != 0) {
                free(stonesoup_buffer_ptr);
            }
        }
    }
    tracepoint(stonesoup_trace, weakness_end);
;
        if (slavian_snoutfair != 0) 
          free(((char *)slavian_snoutfair));
stonesoup_close_printf_context();
      }
    }
  }
  result = empty_string;
  exact = 0;
  do {
    musts = ((sizeof(( *musts)) == 1?xmalloc(d -> tindex + 1) : xnmalloc(d -> tindex + 1,sizeof(( *musts)))));
  }while (0);
  mp = musts;
  for (i = 0; i <= d -> tindex; ++i) 
    mp[i] = must0;
  for (i = 0; i <= d -> tindex; ++i) {
    mp[i] . in = (xmalloc(sizeof(( *mp[i] . in))));
    mp[i] . left = (xmalloc(2));
    mp[i] . right = (xmalloc(2));
    mp[i] . is = (xmalloc(2));
    mp[i] . left[0] = mp[i] . right[0] = mp[i] . is[0] = '\0';
    mp[i] . in[0] = ((void *)0);
  }
#ifdef DEBUG
#endif
  for (ri = 0; ri < d -> tindex; ++ri) {
    switch(t = d -> tokens[ri]){
      case LPAREN:
{
      }
      case RPAREN:
      !"neither LPAREN nor RPAREN may appear here"?((void )0) : __assert_fail("!\"neither LPAREN nor RPAREN may appear here\"","dfa.c",3890,__PRETTY_FUNCTION__);
      case EMPTY:
{
      }
      case BEGLINE:
{
      }
      case ENDLINE:
{
      }
      case BEGWORD:
{
      }
      case ENDWORD:
{
      }
      case LIMWORD:
{
      }
      case NOTLIMWORD:
{
      }
      case BACKREF:
{
        resetmust(mp);
        break; 
      }
      case STAR:
{
      }
      case QMARK:
{
        musts < mp?((void )0) : __assert_fail("musts < mp","dfa.c",3903,__PRETTY_FUNCTION__);
        --mp;
        resetmust(mp);
        break; 
      }
      case OR:
{
        &musts[2] <= mp?((void )0) : __assert_fail("&musts[2] <= mp","dfa.c",3908,__PRETTY_FUNCTION__);
{
          char **new;
          must *lmp;
          must *rmp;
          size_t j;
          size_t ln;
          size_t rn;
          size_t n;
          rmp = --mp;
          lmp = --mp;
/* Guaranteed to be.  Unlikely, but. . . */
          if (!(strcmp((lmp -> is),(rmp -> is)) == 0)) {
            lmp -> is[0] = '\0';
          }
/* Left side--easy */
          i = 0;
          while(lmp -> left[i] != '\0' && lmp -> left[i] == rmp -> left[i])
            ++i;
          lmp -> left[i] = '\0';
/* Right side */
          ln = strlen((lmp -> right));
          rn = strlen((rmp -> right));
          n = ln;
          if (n > rn) {
            n = rn;
          }
          for (i = 0; i < n; ++i) 
            if (lmp -> right[ln - i - 1] != rmp -> right[rn - i - 1]) {
              break; 
            }
          for (j = 0; j < i; ++j) 
            lmp -> right[j] = lmp -> right[ln - i + j];
          lmp -> right[j] = '\0';
          new = inboth(lmp -> in,rmp -> in);
          if (new == ((void *)0)) {
            goto done;
          }
          freelist(lmp -> in);
          free((lmp -> in));
          lmp -> in = new;
        }
        break; 
      }
      case PLUS:
{
        musts < mp?((void )0) : __assert_fail("musts < mp","dfa.c",3946,__PRETTY_FUNCTION__);
        --mp;
        mp -> is[0] = '\0';
        break; 
      }
      case END:
{
        mp == &musts[1]?((void )0) : __assert_fail("mp == &musts[1]","dfa.c",3951,__PRETTY_FUNCTION__);
        for (i = 0; musts[0] . in[i] != ((void *)0); ++i) 
          if (strlen(musts[0] . in[i]) > strlen(result)) {
            result = musts[0] . in[i];
          }
        if (strcmp(result,musts[0] . is) == 0) {
          exact = 1;
        }
        goto done;
      }
      case CAT:
{
        &musts[2] <= mp?((void )0) : __assert_fail("&musts[2] <= mp","dfa.c",3959,__PRETTY_FUNCTION__);
{
          must *lmp;
          must *rmp;
          rmp = --mp;
          lmp = --mp;
/* In.  Everything in left, plus everything in
               right, plus concatenation of
               left's right and right's left. */
          lmp -> in = addlists(lmp -> in,rmp -> in);
          if (lmp -> in == ((void *)0)) {
            goto done;
          }
          if (lmp -> right[0] != '\0' && rmp -> left[0] != '\0') {
            char *tp;
            tp = icpyalloc((lmp -> right));
            tp = icatalloc(tp,(rmp -> left));
            lmp -> in = enlist(lmp -> in,tp,strlen(tp));
            free(tp);
            if (lmp -> in == ((void *)0)) {
              goto done;
            }
          }
/* Left-hand */
          if (lmp -> is[0] != '\0') {
            lmp -> left = icatalloc(lmp -> left,(rmp -> left));
            if (lmp -> left == ((void *)0)) {
              goto done;
            }
          }
/* Right-hand */
          if (rmp -> is[0] == '\0') {
            lmp -> right[0] = '\0';
          }
          lmp -> right = icatalloc(lmp -> right,(rmp -> right));
          if (lmp -> right == ((void *)0)) {
            goto done;
          }
/* Guaranteed to be */
          if (lmp -> is[0] != '\0' && rmp -> is[0] != '\0') {
            lmp -> is = icatalloc(lmp -> is,(rmp -> is));
            if (lmp -> is == ((void *)0)) {
              goto done;
            }
          }
          else {
            lmp -> is[0] = '\0';
          }
        }
        break; 
      }
      default:
{
        if (t < END) {
          !"oops! t >= END"?((void )0) : __assert_fail("!\"oops! t >= END\"","dfa.c",4010,__PRETTY_FUNCTION__);
        }
        else {
          if (t == '\0') {
/* not on *my* shift */
            goto done;
          }
          else {
            if (t >= CSET || !1 || t == ANYCHAR || t == MBCSET) {
/* easy enough */
              resetmust(mp);
            }
            else {
/* plain character */
              resetmust(mp);
              mp -> is[0] = mp -> left[0] = mp -> right[0] = t;
              mp -> is[1] = mp -> left[1] = mp -> right[1] = '\0';
              mp -> in = enlist(mp -> in,mp -> is,((size_t )1));
              if (mp -> in == ((void *)0)) {
                goto done;
              }
            }
          }
        }
        break; 
      }
    }
#ifdef DEBUG
#endif
    ++mp;
  }
  done:
  if (strlen(result)) {
    do {
      dm = ((sizeof(( *dm)) == 1?xmalloc(1) : xnmalloc(1,sizeof(( *dm)))));
    }while (0);
    dm -> exact = exact;
    dm -> must = (xmemdup(result,strlen(result) + 1));
    dm -> next = d -> musts;
    d -> musts = dm;
  }
  mp = musts;
  for (i = 0; i <= d -> tindex; ++i) {
    freelist(mp[i] . in);
    free(mp[i] . in);
    free(mp[i] . left);
    free(mp[i] . right);
    free(mp[i] . is);
  }
  free(mp);
}

struct dfa *dfaalloc()
{
  return (xmalloc(sizeof(struct dfa )));
}

struct dfamust *dfamusts(const struct dfa *d)
{
  return d -> musts;
}
/* vim:set shiftwidth=2: */
