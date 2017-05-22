/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_61b.c
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-61b.tmpl.c
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: rand Set data to result of rand(), which may be zero
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 61 Data flow: data returned from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#define HELLO_STRING "hello"

#ifndef OMITBAD

size_t CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_61b_badSource(size_t data)
{
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
size_t CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_61b_goodG2BSource(size_t data)
{
    /* FIX: Use a relatively small number for memory allocation */
    data = 20;
    return data;
}

/* goodB2G() uses the BadSource with the GoodSink */
size_t CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_61b_goodB2GSource(size_t data)
{
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    return data;
}

#endif /* OMITGOOD */
