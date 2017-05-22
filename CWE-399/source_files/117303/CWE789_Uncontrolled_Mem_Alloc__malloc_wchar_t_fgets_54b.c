/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54b.c
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-54b.tmpl.c
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: fgets Read data from the console using fgets()
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 54 Data flow: data passed as an argument from one function through three others to a fifth; all five functions are in different source files
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#define CHAR_ARRAY_SIZE (3 * sizeof(data) + 2)

#define HELLO_STRING L"hello"

#ifndef OMITBAD

/* bad function declaration */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_badSink(size_t data);

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54b_badSink(size_t data)
{
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_badSink(data);
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_goodG2BSink(size_t data);

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54b_goodG2BSink(size_t data)
{
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_goodG2BSink(data);
}

/* goodB2G uses the BadSource with the GoodSink */
void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_goodB2GSink(size_t data);

void CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54b_goodB2GSink(size_t data)
{
    CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_fgets_54c_goodB2GSink(data);
}

#endif /* OMITGOOD */
