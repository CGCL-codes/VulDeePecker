/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__src.label.xml
Template File: sources-sink-81.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Initialize data as a large string
 * GoodSource: Initialize data as a small string
 * Sinks: cat
 *    BadSink : Copy data to string using strcat
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81
{

class CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) const = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81_bad : public CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__src_char_alloca_cat_81_base
{
public:
    void action(char * data) const;
};

#endif /* OMITGOOD */

}
