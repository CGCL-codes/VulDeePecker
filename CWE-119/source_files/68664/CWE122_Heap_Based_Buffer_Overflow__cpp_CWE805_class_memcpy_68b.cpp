/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68b.cpp
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805.label.xml
Template File: sources-sink-68b.tmpl.cpp
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Allocate using new[] and set data pointer to a small buffer
 * GoodSource: Allocate using new[] and set data pointer to a large buffer
 * Sink: memcpy
 *    BadSink : Copy TwoIntsClass array to data using memcpy
 * Flow Variant: 68 Data flow: data passed as a global variable from one function to another in different source files
 *
 * */

#include "std_testcase.h"

extern TwoIntsClass * CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68_badData;
extern TwoIntsClass * CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68_goodG2BData;

namespace CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68
{

/* all the sinks are the same, we just want to know where the hit originated if a tool flags one */

#ifndef OMITBAD

void badSink()
{
    TwoIntsClass * data = CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68_badData;
    {
        TwoIntsClass source[100];
        {
            size_t i;
            /* Initialize array */
            for (i = 0; i < 100; i++)
            {
                source[i].intOne = 0;
                source[i].intTwo = 0;
            }
        }
        /* POTENTIAL FLAW: Possible buffer overflow if data < 100 */
        memcpy(data, source, 100*sizeof(TwoIntsClass));
        printIntLine(data[0].intOne);
        delete [] data;
    }
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodG2B uses the GoodSource with the BadSink */
void goodG2BSink()
{
    TwoIntsClass * data = CWE122_Heap_Based_Buffer_Overflow__cpp_CWE805_class_memcpy_68_goodG2BData;
    {
        TwoIntsClass source[100];
        {
            size_t i;
            /* Initialize array */
            for (i = 0; i < 100; i++)
            {
                source[i].intOne = 0;
                source[i].intTwo = 0;
            }
        }
        /* POTENTIAL FLAW: Possible buffer overflow if data < 100 */
        memcpy(data, source, 100*sizeof(TwoIntsClass));
        printIntLine(data[0].intOne);
        delete [] data;
    }
}

#endif /* OMITGOOD */

} /* close namespace */
