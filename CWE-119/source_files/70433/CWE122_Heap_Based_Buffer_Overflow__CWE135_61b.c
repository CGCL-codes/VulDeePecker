/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__CWE135_61b.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-61b.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 61 Data flow: data returned from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

void * CWE122_Heap_Based_Buffer_Overflow__CWE135_61b_badSource(void * data)
{
    {
        wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
        wmemset(dataBadBuffer, L'A', 50-1);
        dataBadBuffer[50-1] = L'\0';
        /* POTENTIAL FLAW: Set data to point to a wide string */
        data = (void *)dataBadBuffer;
    }
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
void * CWE122_Heap_Based_Buffer_Overflow__CWE135_61b_goodG2BSource(void * data)
{
    {
        char * dataGoodBuffer = (char *)malloc(50*sizeof(char));
        memset(dataGoodBuffer, 'A', 50-1);
        dataGoodBuffer[50-1] = '\0';
        /* FIX: Set data to point to a char string */
        data = (void *)dataGoodBuffer;
    }
    return data;
}

/* goodB2G() uses the BadSource with the GoodSink */
void * CWE122_Heap_Based_Buffer_Overflow__CWE135_61b_goodB2GSource(void * data)
{
    {
        wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
        wmemset(dataBadBuffer, L'A', 50-1);
        dataBadBuffer[50-1] = L'\0';
        /* POTENTIAL FLAW: Set data to point to a wide string */
        data = (void *)dataBadBuffer;
    }
    return data;
}

#endif /* OMITGOOD */
