/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__CWE135_08.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__CWE135.label.xml
Template File: sources-sinks-08.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Void pointer to a wchar_t array
 * GoodSource: Void pointer to a char array
 * Sinks:
 *    GoodSink: Allocate memory using wcslen() and copy data
 *    BadSink : Allocate memory using strlen() and copy data
 * Flow Variant: 08 Control flow: if(staticReturnsTrue()) and if(staticReturnsFalse())
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

/* The two function below always return the same value, so a tool
   should be able to identify that calls to the functions will always
   return a fixed value. */
static int staticReturnsTrue()
{
    return 1;
}

static int staticReturnsFalse()
{
    return 0;
}

#ifndef OMITBAD

void CWE122_Heap_Based_Buffer_Overflow__CWE135_08_bad()
{
    void * data;
    data = NULL;
    if(staticReturnsTrue())
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    if(staticReturnsTrue())
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

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G1() - use badsource and goodsink by changing the second staticReturnsTrue() to staticReturnsFalse() */
static void goodB2G1()
{
    void * data;
    data = NULL;
    if(staticReturnsTrue())
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    if(staticReturnsFalse())
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
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

/* goodB2G2() - use badsource and goodsink by reversing the blocks in the second if */
static void goodB2G2()
{
    void * data;
    data = NULL;
    if(staticReturnsTrue())
    {
        {
            wchar_t * dataBadBuffer = (wchar_t *)malloc(50*sizeof(wchar_t));
            wmemset(dataBadBuffer, L'A', 50-1);
            dataBadBuffer[50-1] = L'\0';
            /* POTENTIAL FLAW: Set data to point to a wide string */
            data = (void *)dataBadBuffer;
        }
    }
    if(staticReturnsTrue())
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

/* goodG2B1() - use goodsource and badsink by changing the first staticReturnsTrue() to staticReturnsFalse() */
static void goodG2B1()
{
    void * data;
    data = NULL;
    if(staticReturnsFalse())
    {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
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
    if(staticReturnsTrue())
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

/* goodG2B2() - use goodsource and badsink by reversing the blocks in the first if */
static void goodG2B2()
{
    void * data;
    data = NULL;
    if(staticReturnsTrue())
    {
        {
            char * dataGoodBuffer = (char *)malloc(50*sizeof(char));
            memset(dataGoodBuffer, 'A', 50-1);
            dataGoodBuffer[50-1] = '\0';
            /* FIX: Set data to point to a char string */
            data = (void *)dataGoodBuffer;
        }
    }
    if(staticReturnsTrue())
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

void CWE122_Heap_Based_Buffer_Overflow__CWE135_08_good()
{
    goodB2G1();
    goodB2G2();
    goodG2B1();
    goodG2B2();
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
    CWE122_Heap_Based_Buffer_Overflow__CWE135_08_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE122_Heap_Based_Buffer_Overflow__CWE135_08_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
