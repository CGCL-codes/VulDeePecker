/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52b.c
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE129.label.xml
Template File: sources-sinks-52b.tmpl.c
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 52 Data flow: data passed as an argument from one function to another to another in three different source files
 *
 * */

#include "std_testcase.h"

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

#ifndef OMITBAD

/* bad function declaration */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_badSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52b_badSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_goodG2BSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52b_goodG2BSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_goodB2GSink(int data);

void CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52b_goodB2GSink(int data)
{
    CWE121_Stack_Based_Buffer_Overflow__CWE129_fgets_52c_goodB2GSink(data);
}

#endif /* OMITGOOD */
