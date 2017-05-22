/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE127_Buffer_Underread__malloc_wchar_t_cpy_61a.c
Label Definition File: CWE127_Buffer_Underread__malloc.label.xml
Template File: sources-sink-61a.tmpl.c
*/
/*
 * @description
 * CWE: 127 Buffer Under-read
 * BadSource:  Set data pointer to before the allocated memory buffer
 * GoodSource: Set data pointer to the allocated memory buffer
 * Sinks: cpy
 *    BadSink : Copy data to string using wcscpy
 * Flow Variant: 61 Data flow: data returned from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

#ifndef OMITBAD

/* bad function declaration */
wchar_t * CWE127_Buffer_Underread__malloc_wchar_t_cpy_61b_badSource(wchar_t * data);

void CWE127_Buffer_Underread__malloc_wchar_t_cpy_61_bad()
{
    wchar_t * data;
    data = NULL;
    data = CWE127_Buffer_Underread__malloc_wchar_t_cpy_61b_badSource(data);
    {
        wchar_t dest[100*2];
        wmemset(dest, L'C', 100*2-1); /* fill with 'C's */
        dest[100*2-1] = L'\0'; /* null terminate */
        /* POTENTIAL FLAW: Possibly copy from a memory location located before the source buffer */
        wcscpy(dest, data);
        printWLine(dest);
        /* INCIDENTAL CWE-401: Memory Leak - data may not point to location
         * returned by malloc() so can't safely call free() on it */
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
wchar_t * CWE127_Buffer_Underread__malloc_wchar_t_cpy_61b_goodG2BSource(wchar_t * data);

static void goodG2B()
{
    wchar_t * data;
    data = NULL;
    data = CWE127_Buffer_Underread__malloc_wchar_t_cpy_61b_goodG2BSource(data);
    {
        wchar_t dest[100*2];
        wmemset(dest, L'C', 100*2-1); /* fill with 'C's */
        dest[100*2-1] = L'\0'; /* null terminate */
        /* POTENTIAL FLAW: Possibly copy from a memory location located before the source buffer */
        wcscpy(dest, data);
        printWLine(dest);
        /* INCIDENTAL CWE-401: Memory Leak - data may not point to location
         * returned by malloc() so can't safely call free() on it */
    }
}

void CWE127_Buffer_Underread__malloc_wchar_t_cpy_61_good()
{
    goodG2B();
}

#endif /* OMITGOOD */

/* Below is the main(). It is only used when building this testcase on
 * its own for testing or for building a binary to use in testing binary
 * analysis tools. It is not used when compiling all the testcases as one
 * application, which is how source code analysis tools are tested.
 */

#ifdef INCLUDEMAIN

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    CWE127_Buffer_Underread__malloc_wchar_t_cpy_61_good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    CWE127_Buffer_Underread__malloc_wchar_t_cpy_61_bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
