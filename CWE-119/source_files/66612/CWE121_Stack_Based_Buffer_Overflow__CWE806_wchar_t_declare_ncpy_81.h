/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE806.label.xml
Template File: sources-sink-81.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Initialize data as a large string
 * GoodSource: Initialize data as a small string
 * Sinks: ncpy
 *    BadSink : Copy data to string using wcsncpy
 * Flow Variant: 81 Data flow: data passed in a parameter to an virtual method called via a reference
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81
{

class CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81_base
{
public:
    /* pure virtual function */
    virtual void action(wchar_t * data) const = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81_bad : public CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__CWE806_wchar_t_declare_ncpy_81_base
{
public:
    void action(wchar_t * data) const;
};

#endif /* OMITGOOD */

}
