/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE124_Buffer_Underwrite__CWE839_listen_socket_81.h
Label Definition File: CWE124_Buffer_Underwrite__CWE839.label.xml
Template File: sources-sinks-81.tmpl.h
*/
/*
 * @description
 * CWE: 124 Buffer Underwrite
 * BadSource: listen_socket Read data using a listen socket (server side)
 * GoodSource: Non-negative but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the lower bound
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

namespace CWE124_Buffer_Underwrite__CWE839_listen_socket_81
{

class CWE124_Buffer_Underwrite__CWE839_listen_socket_81_base
{
public:
    /* pure virtual function */
    virtual void action(int data) const = 0;
};

#ifndef OMITBAD

class CWE124_Buffer_Underwrite__CWE839_listen_socket_81_bad : public CWE124_Buffer_Underwrite__CWE839_listen_socket_81_base
{
public:
    void action(int data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE124_Buffer_Underwrite__CWE839_listen_socket_81_goodG2B : public CWE124_Buffer_Underwrite__CWE839_listen_socket_81_base
{
public:
    void action(int data) const;
};

class CWE124_Buffer_Underwrite__CWE839_listen_socket_81_goodB2G : public CWE124_Buffer_Underwrite__CWE839_listen_socket_81_base
{
public:
    void action(int data) const;
};

#endif /* OMITGOOD */

}
