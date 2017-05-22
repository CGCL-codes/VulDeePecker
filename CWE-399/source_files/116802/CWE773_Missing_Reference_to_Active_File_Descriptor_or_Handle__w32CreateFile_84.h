/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84.h
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile.label.xml
Template File: source-sinks-84.tmpl.h
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file handle using CreateFile()
 * Sinks:
 *    GoodSink: Close the file handle before reusing it
 *    BadSink : Reassign the file handle before closing it
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */

#include "std_testcase.h"

#include <windows.h>

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84
{

#ifndef OMITBAD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad
{
public:
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad(HANDLE dataCopy);
    ~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_bad();

private:
    HANDLE data;
};

#endif /* OMITBAD */

#ifndef OMITGOOD

class CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_goodB2G
{
public:
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_goodB2G(HANDLE dataCopy);
    ~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__w32CreateFile_84_goodB2G();

private:
    HANDLE data;
};

#endif /* OMITGOOD */

}
