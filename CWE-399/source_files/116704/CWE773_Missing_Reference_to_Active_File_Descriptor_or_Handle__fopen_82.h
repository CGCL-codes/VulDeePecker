/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82.h
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen.label.xml
Template File: source-sinks-82.tmpl.h
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file handle using fopen()
 * Sinks:
 *    GoodSink: Close the file handle before reusing it
 *    BadSink : Reassign the file handle before closing it
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */

#include "std_testcase.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82
{

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_base
{
public:
    /* pure virtual function */
    virtual void action(FILE * data) = 0;
};

#ifndef OMITBAD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_bad : public CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_base
{
public:
    void action(FILE * data);
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_goodB2G : public CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_base
{
public:
    void action(FILE * data);
};

#endif /* OMITGOOD */

}
