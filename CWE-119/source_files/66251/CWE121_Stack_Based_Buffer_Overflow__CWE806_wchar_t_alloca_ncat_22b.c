/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22b.c
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE806.label.xml
Template File: sources-sink-22b.tmpl.c
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Initialize data as a large string
 * GoodSource: Initialize data as a small string
 * Sink: ncat
 *    BadSink : Copy data to string using wcsncat
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
extern int CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_badGlobal;

wchar_t * CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_badSource(wchar_t * data)
{
    if(CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_badGlobal)
    {
        /* FLAW: Initialize data as a large buffer that is larger than the small buffer used in the sink */
        wmemset(data, L'A', 100-1); /* fill with L'A's */
        data[100-1] = L'\0'; /* null terminate */
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
extern int CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B1Global;
extern int CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B2Global;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
wchar_t * CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B1Source(wchar_t * data)
{
    if(CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        /* FIX: Initialize data as a small buffer that as small or smaller than the small buffer used in the sink */
        wmemset(data, L'A', 50-1); /* fill with L'A's */
        data[50-1] = L'\0'; /* null terminate */
    }
    return data;
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
wchar_t * CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B2Source(wchar_t * data)
{
    if(CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_alloca_ncat_22_goodG2B2Global)
    {
        /* FIX: Initialize data as a small buffer that as small or smaller than the small buffer used in the sink */
        wmemset(data, L'A', 50-1); /* fill with L'A's */
        data[50-1] = L'\0'; /* null terminate */
    }
    return data;
}

#endif /* OMITGOOD */
