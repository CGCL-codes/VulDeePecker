/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53c.c
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__dest.label.xml
Template File: sources-sink-53c.tmpl.c
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Set data pointer to the bad buffer
 * GoodSource: Set data pointer to the good buffer
 * Sink: cpy
 *    BadSink : Copy string to data using wcscpy
 * Flow Variant: 53 Data flow: data passed as an argument from one function through two others to a fourth; all four functions are in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

/* all the sinks are the same, we just want to know where the hit originated if a tool flags one */

#ifndef OMITBAD

/* bad function declaration */
void CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53d_badSink(wchar_t * data);

void CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53c_badSink(wchar_t * data)
{
    CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53d_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* good function declaration */
void CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53d_goodG2BSink(wchar_t * data);

/* goodG2B uses the GoodSource with the BadSink */
void CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53c_goodG2BSink(wchar_t * data)
{
    CWE121_Stack_Based_Buffer_Overflow__dest_wchar_t_declare_cpy_53d_goodG2BSink(data);
}

#endif /* OMITGOOD */
