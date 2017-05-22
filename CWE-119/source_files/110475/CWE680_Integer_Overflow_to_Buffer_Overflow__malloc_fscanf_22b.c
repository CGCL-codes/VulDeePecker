/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22b.c
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc.label.xml
Template File: sources-sink-22b.tmpl.c
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: fscanf Read data from the console using fscanf()
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sink:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 22 Control flow: Flow controlled by value of a global variable. Sink functions are in a separate file from sources.
 *
 * */

#include "std_testcase.h"

#ifndef OMITBAD

/* The global variable below is used to drive control flow in the source function */
extern int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badGlobal;

int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badSource(int data)
{
    if(CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_badGlobal)
    {
        /* POTENTIAL FLAW: Read data from the console using fscanf() */
        fscanf(stdin, "%d", &data);
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* The global variables below are used to drive control flow in the source functions. */
extern int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Global;
extern int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Global;

/* goodG2B1() - use goodsource and badsink by setting the static variable to false instead of true */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Source(int data)
{
    if(CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B1Global)
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    }
    else
    {
        /* FIX: Set data to a relatively small number greater than zero */
        data = 20;
    }
    return data;
}

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the if in the source function */
int CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Source(int data)
{
    if(CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fscanf_22_goodG2B2Global)
    {
        /* FIX: Set data to a relatively small number greater than zero */
        data = 20;
    }
    return data;
}

#endif /* OMITGOOD */
