/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82.h
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__c_CWE806.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Initialize data as a large string
 * GoodSource: Initialize data as a small string
 *    BadSink : Copy data to string using strncpy
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82
{

class CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82_bad : public CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82_goodG2B : public CWE122_Heap_Based_Buffer_Overflow__c_CWE806_char_ncpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}
