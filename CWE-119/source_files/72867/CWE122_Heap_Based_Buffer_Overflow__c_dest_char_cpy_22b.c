/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22b.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__c_dest.label.xml
Template File: sources-sink-22b.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Allocate using malloc() and set data pointer to a small buffer
 * GoodSource: Allocate using malloc() and set data pointer to a large buffer
 * Sink: cpy
 *    BadSink : Copy string to data using strcpy
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
extern int CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_badGlobal;

char * CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_badSource(char * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_badGlobal)
    {
        /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
        data = (char *)malloc(50*sizeof(char));
        data[0] = '\0'; /* null terminate */
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
extern int CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B1Global;
extern int CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B2Global;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
char * CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B1Source(char * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        /* FIX: Allocate and point data to a large buffer that is at least as large as the large buffer used in the sink */
        data = (char *)malloc(100*sizeof(char));
        data[0] = '\0'; /* null terminate */
    }
    return data;
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
char * CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B2Source(char * data)
{
    if(CWE122_Heap_Based_Buffer_Overflow__c_dest_char_cpy_22_goodG2B2Global)
    {
        /* FIX: Allocate and point data to a large buffer that is at least as large as the large buffer used in the sink */
        data = (char *)malloc(100*sizeof(char));
        data[0] = '\0'; /* null terminate */
    }
    return data;
}

#endif /* OMITGOOD */
