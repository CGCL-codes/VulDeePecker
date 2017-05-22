/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54d.c
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__malloc.label.xml
Template File: sources-sink-54d.tmpl.c
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sink:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

/* all the sinks are the same, we just want to know where the hit originated if a tool flags one */

#ifndef OMITBAD

/* bad function declaration */
void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54e_badSink(int data);

void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54d_badSink(int data)
{
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54e_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* good function declaration */
void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54e_goodG2BSink(int data);

/* goodG2B uses the GoodSource with the BadSink */
void CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54d_goodG2BSink(int data)
{
    CWE680_Integer_Overflow_to_Buffer_Overflow__malloc_fgets_54e_goodG2BSink(data);
}

#endif /* OMITGOOD */
