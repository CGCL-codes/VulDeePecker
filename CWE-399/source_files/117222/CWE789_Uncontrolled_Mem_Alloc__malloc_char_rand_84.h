/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84.h
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-84.tmpl.h
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: rand Set data to result of rand(), which may be zero
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84
{

#ifndef OMITBAD

class CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_bad
{
public:
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_bad(size_t dataCopy);
    ~CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_bad();

private:
    size_t data;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodG2B
{
public:
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodG2B(size_t dataCopy);
    ~CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodG2B();

private:
    size_t data;
};

class CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodB2G
{
public:
    CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodB2G(size_t dataCopy);
    ~CWE789_Uncontrolled_Mem_Alloc__malloc_char_rand_84_goodB2G();

private:
    size_t data;
};

#endif /* OMITGOOD */

}
