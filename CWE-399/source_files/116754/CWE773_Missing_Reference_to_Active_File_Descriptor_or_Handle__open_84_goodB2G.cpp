/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G.cpp
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open.label.xml
Template File: source-sinks-84_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 773 Missing Reference to Active File Descriptor or Handle
 * BadSource:  Create a file descriptor using open()
 * Sinks:
 *    GoodSink: Close the file before reusing the file descriptor
 *    BadSink : Reassign the file descriptor before closing the file
 * Flow Variant: 84 Data flow: data passed to class constructor and destructor by declaring the class object on the heap and deleting it after use
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84
{
CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G::CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G(int dataCopy)
{
    data = dataCopy;
    /* POTENTIAL FLAW: Create a file descriptor using open() that may not be closed properly */
    data = OPEN("BadSource_open.txt", O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
}

CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G::~CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G()
{
    /* FIX: Close the file from the source before reassigning data to a new file descriptor */
    if (data != -1)
    {
        CLOSE(data);
    }
    data = OPEN("GoodSink_open.txt", O_RDWR|O_CREAT, S_IREAD|S_IWRITE);
    /* avoid incidental for not closing the file */
    if (data != -1)
    {
        CLOSE(data);
    }
}
}
#endif /* OMITGOOD */
