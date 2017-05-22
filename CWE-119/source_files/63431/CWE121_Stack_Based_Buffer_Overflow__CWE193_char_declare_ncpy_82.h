/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82.h
Label Definition File: CWE121_Stack_Based_Buffer_Overflow__CWE193.label.xml
Template File: sources-sink-82.tmpl.h
*/
/*
 * @description
 * CWE: 121 Stack Based Buffer Overflow
 * BadSource:  Point data to a buffer that does not have space for a NULL terminator
 * GoodSource: Point data to a buffer that includes space for a NULL terminator
 *    BadSink : Copy string to data using strncpy()
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

/* MAINTENANCE NOTE: The length of this string should equal the 10 */
#define SRC_STRING "AAAAAAAAAA"

namespace CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82
{

class CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82_base
{
public:
    /* pure virtual function */
    virtual void action(char * data) = 0;
};

#ifndef OMITBAD

class CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82_bad : public CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82_goodG2B : public CWE121_Stack_Based_Buffer_Overflow__CWE193_char_declare_ncpy_82_base
{
public:
    void action(char * data);
};

#endif /* OMITGOOD */

}
