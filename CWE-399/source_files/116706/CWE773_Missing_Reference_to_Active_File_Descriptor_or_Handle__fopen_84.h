/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84.h
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen.label.xml
Template File: source-sinks-84.tmpl.h
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file handle using fopen()
 * Sinks:
 *    GoodSink: Close the file handle before reusing it
 *    BadSink : Reassign the file handle before closing it
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */

#include "std_testcase.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84
{

#ifndef OMITBAD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_bad
{
public:
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_bad(FILE * dataCopy);
    ~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_bad();

private:
    FILE * data;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_goodB2G
{
public:
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_goodB2G(FILE * dataCopy);
    ~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_84_goodB2G();

private:
    FILE * data;
};

#endif /* OMITGOOD */

}
