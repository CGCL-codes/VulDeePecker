/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__src.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Initialize data as a large string
 * GoodSource: Initialize data as a small string
 *    BadSink : Copy data to string using strcpy
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82
{

class CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82_bad : public CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__src_char_declare_cpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}
