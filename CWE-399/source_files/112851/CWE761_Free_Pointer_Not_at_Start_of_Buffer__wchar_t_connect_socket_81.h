/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81.h
Label Definition File: CWE761_Free_Pointer_Not_at_Start_of_Buffer.label.xml
Template File: source-sinks-81.tmpl.h
*/
/*
 * @description
 * CWE: 761 Free Pointer not at Start of Buffer
 * BadSource: connect_socket Read data using a connect socket (client side)
 * Sinks:
 *    GoodSink: free() memory correctly at the start of the buffer
 *    BadSink : free() memory not at the start of the buffer
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81
{

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81_base
{
public:
    /* pure virtual function */
    virtual void action(wchar_t * data) const = 0;
};

#ifndef OMITBAD

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81_bad : public CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81_goodB2G : public CWE761_Free_Pointer_Not_at_Start_of_Buffer__wchar_t_connect_socket_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITGOOD */

}
