/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__CWE135_82_goodB2G.cpp
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-82_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE122_Heap_Based_Buffer_Overflow__CWE135_82.h"

namespace CWE122_Heap_Based_Buffer_Overflow__CWE135_82
{

void CWE122_Heap_Based_Buffer_Overflow__CWE135_82_goodB2G::action(void * data)
{
    {
        /* FIX: treating pointer like a wchar_t*  */
        size_t dataLen = wcslen((wchar_t *)data);
        void * dest = (void *)calloc(dataLen+1, sizeof(wchar_t));
        memcpy(dest, data, (dataLen+1)*sizeof(wchar_t));
        printWLine((wchar_t *)dest);
        free(dest);
    }
}

}
#endif /* OMITGOOD */
