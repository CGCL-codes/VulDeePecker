/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE129.label.xml
Template File: sources-sinks-81.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource: connect_socket Read data using a connect socket (client side)
 * GoodSource: Larger than zero but less than 10
 * Sinks:
 *    GoodSink: Ensure the array index is valid
 *    BadSink : Improperly check the array index by not checking the upper bound
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

namespace CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81
{

class CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_base
{
public:
    /* pure virtual function */
    virtual void action(int data) const = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_bad : public CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_base
{
public:
    void action(int data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_base
{
public:
    void action(int data) const;
};

class CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_goodB2G : public CWE121_Stack_Based_Buffer_Overflow__CWE129_connect_socket_81_base
{
public:
    void action(int data) const;
};

#endif /* OMITGOOD */

}
