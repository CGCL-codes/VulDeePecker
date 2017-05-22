/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22b.c
Label Definition File: CWE127_Buffer_Underread__malloc.label.xml
Template File: sources-sink-22b.tmpl.c
*/
/*
 * @description
 * CWE: 127 Buffer Under-read
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sink: ncpy
 *    BadSink : Copy data to string using wcsncpy
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
extern int CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_badGlobal;

wchar_t * CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_badSource(wchar_t * data)
{
    if(CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_badGlobal)
    {
        {
            wchar_t * dataBuffer = (wchar_t *)malloc(100*sizeof(wchar_t));
            wmemset(dataBuffer, L'A', 100-1);
            dataBuffer[100-1] = L'\0';
            /* FLAW: Set data pointer to before the allocated memory buffer */
            data = dataBuffer - 8;
        }
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
extern int CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B1Global;
extern int CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B2Global;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
wchar_t * CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B1Source(wchar_t * data)
{
    if(CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            wchar_t * dataBuffer = (wchar_t *)malloc(100*sizeof(wchar_t));
            wmemset(dataBuffer, L'A', 100-1);
            dataBuffer[100-1] = L'\0';
            /* FIX: Set data pointer to the allocated memory buffer */
            data = dataBuffer;
        }
    }
    return data;
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
wchar_t * CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B2Source(wchar_t * data)
{
    if(CWE127_Buffer_Underread__malloc_wchar_t_ncpy_22_goodG2B2Global)
    {
        {
            wchar_t * dataBuffer = (wchar_t *)malloc(100*sizeof(wchar_t));
            wmemset(dataBuffer, L'A', 100-1);
            dataBuffer[100-1] = L'\0';
            /* FIX: Set data pointer to the allocated memory buffer */
            data = dataBuffer;
        }
    }
    return data;
}

#endif /* OMITGOOD */
