/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__CWE135_12.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-12.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 12 Control flow: if(globalReturnsTrueOrFalse())
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

void CWE122_Heap_Based_Buffer_Overflow__CWE135_12_bad()
{
    void * data;
    data = NULL;
    if(globalReturnsTrueOrFalse())
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    else
    {
        {
            char * dataGoodBuffer = (char *)malloc(50*sizeof(char));
            memset(dataGoodBuffer, 'A', 50-1);
            dataGoodBuffer[50-1] = '\0';
            /* FIX: Set data to point to a char string */
            data = (void *)dataGoodBuffer;
        }
    }
    if(globalReturnsTrueOrFalse())
    {
        {
            /* POTENTIAL FLAW: treating pointer as a char* when it may point to a wide string */
            size_t dataLen = strlen((char *)data);
            void * dest = (void *)calloc(dataLen+1, 1);
            memcpy(dest, data, (dataLen+1));
            printLine((char *)dest);
            free(dest);
        }
    }
    else
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

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G() - use badsource and goodsink by changing the first "if" so that
   both branches use the BadSource and the second "if" so that both branches
   use the GoodSink */
static void goodB2G()
{
    void * data;
    data = NULL;
    if(globalReturnsTrueOrFalse())
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    else
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    if(globalReturnsTrueOrFalse())
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
    else
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

/* goodG2B() - use goodsource and badsink by changing the first "if" so that
   both branches use the GoodSource and the second "if" so that both branches
   use the BadSink */
static void goodG2B()
{
    void * data;
    data = NULL;
    if(globalReturnsTrueOrFalse())
    {
        {
            char * dataGoodBuffer = (char *)malloc(50*sizeof(char));
            memset(dataGoodBuffer, 'A', 50-1);
            dataGoodBuffer[50-1] = '\0';
            /* FIX: Set data to point to a char string */
            data = (void *)dataGoodBuffer;
        }
    }
    else
    {
        {
            char * dataGoodBuffer = (char *)malloc(50*sizeof(char));
            memset(dataGoodBuffer, 'A', 50-1);
            dataGoodBuffer[50-1] = '\0';
            /* FIX: Set data to point to a char string */
            data = (void *)dataGoodBuffer;
        }
    }
    if(globalReturnsTrueOrFalse())
    {
        {
            /* POTENTIAL FLAW: treating pointer as a char* when it may point to a wide string */
            size_t dataLen = strlen((char *)data);
            void * dest = (void *)calloc(dataLen+1, 1);
            memcpy(dest, data, (dataLen+1));
            printLine((char *)dest);
            free(dest);
        }
    }
    else
    {
        {
            /* POTENTIAL FLAW: treating pointer as a char* when it may point to a wide string */
            size_t dataLen = strlen((char *)data);
            void * dest = (void *)calloc(dataLen+1, 1);
            memcpy(dest, data, (dataLen+1));
            printLine((char *)dest);
            free(dest);
        }
    }
}

void CWE122_Heap_Based_Buffer_Overflow__CWE135_12_good()
{
    goodB2G();
    goodG2B();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_12_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_12_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
