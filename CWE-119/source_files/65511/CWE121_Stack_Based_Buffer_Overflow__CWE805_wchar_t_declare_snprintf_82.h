/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE805.string.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Set data pointer to the bad buffer
 * GoodSource: Set data pointer to the good buffer
 *    BadSink : Copy string to data using snprintf
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#include <wchar.h>

namespace CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82
{

class CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82_base
{
public:
    /* pure virtual function */
    virtual void action(wchar_t * data) = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82_bad : public CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82_base
{
public:
    void action(wchar_t * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__CWE805_wchar_t_declare_snprintf_82_base
{
public:
    void action(wchar_t * data);
};

#endif /* OMITGOOD */

}
