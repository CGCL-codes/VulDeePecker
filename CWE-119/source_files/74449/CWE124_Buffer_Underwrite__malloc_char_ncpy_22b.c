/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__malloc_char_ncpy_22b.c
Label Definition File: CWE124_Buffer_Underwrite__malloc.label.xml
Template File: sources-sink-22b.tmpl.c
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sink: ncpy
 *    BadSink : Copy string to data using strncpy
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
extern int CWE124_Buffer_Underwrite__malloc_char_ncpy_22_badGlobal;

char * CWE124_Buffer_Underwrite__malloc_char_ncpy_22_badSource(char * data)
{
    if(CWE124_Buffer_Underwrite__malloc_char_ncpy_22_badGlobal)
    {
        {
            char * dataBuffer = (char *)malloc(100*sizeof(char));
            memset(dataBuffer, 'A', 100-1);
            dataBuffer[100-1] = '\0';
            /* FLAW: Set data pointer to before the allocated memory buffer */
            data = dataBuffer - 8;
        }
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
extern int CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B1Global;
extern int CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B2Global;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
char * CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B1Source(char * data)
{
    if(CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        {
            char * dataBuffer = (char *)malloc(100*sizeof(char));
            memset(dataBuffer, 'A', 100-1);
            dataBuffer[100-1] = '\0';
            /* FIX: Set data pointer to the allocated memory buffer */
            data = dataBuffer;
        }
    }
    return data;
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
char * CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B2Source(char * data)
{
    if(CWE124_Buffer_Underwrite__malloc_char_ncpy_22_goodG2B2Global)
    {
        {
            char * dataBuffer = (char *)malloc(100*sizeof(char));
            memset(dataBuffer, 'A', 100-1);
            dataBuffer[100-1] = '\0';
            /* FIX: Set data pointer to the allocated memory buffer */
            data = dataBuffer;
        }
    }
    return data;
}

#endif /* OMITGOOD */
