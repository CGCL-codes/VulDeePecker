/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54c.c
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE129.label.xml
Template File: sources-sinks-54c.tmpl.c
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

#ifndef OMITBAD

/* bad function declaration */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_badSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54c_badSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_goodG2BSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54c_goodG2BSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_goodB2GSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54c_goodB2GSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_54d_goodB2GSink(data);
}

#endif /* OMITGOOD */
