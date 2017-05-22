/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_goodB2G.cpp
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen.label.xml
Template File: source-sinks-82_goodB2G.tmpl.cpp
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
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82
{

void CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__fopen_82_goodB2G::action(FILE * data)
{
    /* FIX: Close the file from the source before pointing data to a new file handle */
    if (data != NULL)
    {
        fclose(data);
    }
    data = fopen("GoodSink_fopen.txt", "w+");
    /* avoid incidental for not closing the file handle */
    if (data != NULL)
    {
        fclose(data);
    }
}

}
#endif /* OMITGOOD */
