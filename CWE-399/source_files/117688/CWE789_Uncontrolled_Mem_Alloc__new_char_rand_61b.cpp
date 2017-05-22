/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__new_char_rand_61b.cpp
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__new.label.xml
Template File: sources-sinks-61b.tmpl.cpp
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: rand Set data to result of rand(), which may be zero
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with new [] and check the size of the memory to be allocated
 *    BadSink : Allocate memory with new [], but incorrectly check the size of the memory to be allocated
 * Flow Variant: 61 Data flow: data returned from one function to another in different source files
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

#define HELLO_STRING "hello"

namespace CWE789_Uncontrolled_Mem_Alloc__new_char_rand_61
{

#ifndef OMITBAD

size_t badSource(size_t data)
{
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    return data;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B() uses the GoodSource with the BadSink */
size_t goodG2BSource(size_t data)
{
    /* FIX: Use a relatively small number for memory allocation */
    data = 20;
    return data;
}

/* goodB2G() uses the BadSource with the GoodSink */
size_t goodB2GSource(size_t data)
{
    /* POTENTIAL FLAW: Set data to a random value */
    data = rand();
    return data;
}

#endif /* OMITGOOD */

} /* close namespace */
