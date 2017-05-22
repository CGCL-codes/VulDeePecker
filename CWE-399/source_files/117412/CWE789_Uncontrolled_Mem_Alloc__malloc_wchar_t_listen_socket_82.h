/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82.h
Label Definition File: CWE789_Uncontrolled_Mem_Alloc__malloc.label.xml
Template File: sources-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 789 Uncontrolled Memory Allocation
 * BadSource: listen_socket Read data using a listen socket (server side)
 * GoodSource: Small number greater than zero
 * Sinks:
 *    GoodSink: Allocate memory with malloc() and check the size of the memory to be allocated
 *    BadSink : Allocate memory with malloc(), but incorrectly check the size of the memory to be allocated
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

namespace CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82
{

class CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_base
{
public:
    /* pure virtual function */
    virtual void action(size_t data) = 0;
};

#ifndef OMITBAD

class CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_bad : public CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_base
{
public:
    void action(size_t data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_goodG2B : public CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_base
{
public:
    void action(size_t data);
};

class CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_goodB2G : public CWE789_Uncontrolled_Mem_Alloc__malloc_wchar_t_listen_socket_82_base
{
public:
    void action(size_t data);
};

#endif /* OMITGOOD */

}
