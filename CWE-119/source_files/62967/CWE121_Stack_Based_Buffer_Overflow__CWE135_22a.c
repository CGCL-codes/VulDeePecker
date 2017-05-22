/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE135_22a.c
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-22a.tmpl.c
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#define WIDE_STRING L"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
#define CHAR_STRING "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the sink function */
int CWE121_Stack_Based_Buffer_Overflow__CWE135_22_badGlobal = 0;

void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_badSink(void * data);

void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_bad()
{
    void * data;
    data = NULL;
    /* POTENTIAL FLAW: Set data to point to a wide string */
    data = (void *)WIDE_STRING;
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_badGlobal = 1; /* true */
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the sink functions. */
int CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G1Global = 0;
int CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G2Global = 0;
int CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodG2BGlobal = 0;

/* goodB2G1() - use badsource and goodsink by setting the static variable to false instead of true */
void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G1Sink(void * data);

static void goodB2G1()
{
    void * data;
    data = NULL;
    /* POTENTIAL FLAW: Set data to point to a wide string */
    data = (void *)WIDE_STRING;
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G1Global = 0; /* false */
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G1Sink(data);
}

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the if in the sink function */
void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G2Sink(void * data);

static void goodB2G2()
{
    void * data;
    data = NULL;
    /* POTENTIAL FLAW: Set data to point to a wide string */
    data = (void *)WIDE_STRING;
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G2Global = 1; /* true */
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodB2G2Sink(data);
}

/* goodG2B() - use goodsource and badsink */
void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodG2BSink(void * data);

static void goodG2B()
{
    void * data;
    data = NULL;
    /* FIX: Set data to point to a char string */
    data = (void *)CHAR_STRING;
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodG2BGlobal = 1; /* true */
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_goodG2BSink(data);
}

void CWE121_Stack_Based_Buffer_Overflow__CWE135_22_good()
{
    goodB2G1();
    goodB2G2();
    goodG2B();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE121_Stack_Based_Buffer_Overflow__CWE135_22_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
