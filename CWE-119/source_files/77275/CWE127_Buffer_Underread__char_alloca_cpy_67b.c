/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE127_Buffer_Underread__char_alloca_cpy_67b.c
Label Definition File: CWE127_Buffer_Underread.stack.label.xml
Template File: sources-sink-67b.tmpl.c
*/
/*
 * @description
 * CWE: 127 Buffer Under-read
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sinks: cpy
 *    BadSink : Copy data to string using strcpy
 * Flow Variant: 67 Data flow: data passed in a struct from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

typedef struct _CWE127_Buffer_Underread__char_alloca_cpy_67_structType
{
    char * structFirst;
} CWE127_Buffer_Underread__char_alloca_cpy_67_structType;

#ifndef OMITBAD

void CWE127_Buffer_Underread__char_alloca_cpy_67b_badSink(CWE127_Buffer_Underread__char_alloca_cpy_67_structType myStruct)
{
    char * data = myStruct.structFirst;
    {
        char dest[100*2];
        memset(dest, 'C', 100*2-1); /* fill with 'C's */
        dest[100*2-1] = '\0'; /* null terminate */
        /* POTENTIAL FLAW: Possibly copy from a memory location located before the source buffer */
        strcpy(dest, data);
        printLine(dest);
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE127_Buffer_Underread__char_alloca_cpy_67b_goodG2BSink(CWE127_Buffer_Underread__char_alloca_cpy_67_structType myStruct)
{
    char * data = myStruct.structFirst;
    {
        char dest[100*2];
        memset(dest, 'C', 100*2-1); /* fill with 'C's */
        dest[100*2-1] = '\0'; /* null terminate */
        /* POTENTIAL FLAW: Possibly copy from a memory location located before the source buffer */
        strcpy(dest, data);
        printLine(dest);
    }
}

#endif /* OMITGOOD */
