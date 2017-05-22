/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82.h
Label Definition File: CWE680_Integer_Overflow_to_Buffer_Overflow__new.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 680 Integer Overflow to Buffer Overflow
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Small number greater than zero that will not cause an integer overflow in the sink
 * Sinks:
 *    BadSink : Attempt to allocate array using length value from source
 * Flow Variant: 82 Data flow: data passed in a parameter to a virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

namespace CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82
{

class CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82_base
{
public:
    /* pure virtual function */
    virtual void action(int data) = 0;
};

#ifndef OMITBAD

class CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82_bad : public CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82_base
{
public:
    void action(int data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82_goodG2B : public CWE680_Integer_Overflow_to_Buffer_Overflow__new_connect_socket_82_base
{
public:
    void action(int data);
};

#endif /* OMITGOOD */

}
