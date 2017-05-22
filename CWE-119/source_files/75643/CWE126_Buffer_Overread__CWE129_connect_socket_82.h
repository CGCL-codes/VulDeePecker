/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE126_Buffer_Overread__CWE129_connect_socket_82.h
Label Definition File: CWE126_Buffer_Overread__CWE129.label.xml
Template File: sources-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 126 Buffer Overread
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

namespace CWE126_Buffer_Overread__CWE129_connect_socket_82
{

class CWE126_Buffer_Overread__CWE129_connect_socket_82_base
{
public:
    /* pure virtual function */
    virtual void action(int data) = 0;
};

#ifndef OMITBAD

class CWE126_Buffer_Overread__CWE129_connect_socket_82_bad : public CWE126_Buffer_Overread__CWE129_connect_socket_82_base
{
public:
    void action(int data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE126_Buffer_Overread__CWE129_connect_socket_82_goodG2B : public CWE126_Buffer_Overread__CWE129_connect_socket_82_base
{
public:
    void action(int data);
};

class CWE126_Buffer_Overread__CWE129_connect_socket_82_goodB2G : public CWE126_Buffer_Overread__CWE129_connect_socket_82_base
{
public:
    void action(int data);
};

#endif /* OMITGOOD */

}
