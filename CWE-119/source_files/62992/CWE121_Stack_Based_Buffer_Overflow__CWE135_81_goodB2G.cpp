/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE135_81_goodB2G.cpp
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-81_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE121_Stack_Based_Buffer_Overflow__CWE135_81.h"

namespace CWE121_Stack_Based_Buffer_Overflow__CWE135_81
{

void CWE121_Stack_Based_Buffer_Overflow__CWE135_81_goodB2G::action(void * data) const
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
