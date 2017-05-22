/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_goodB2G.cpp
Label Definition File: CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close.label.xml
Template File: source-sinks-82_goodB2G.tmpl.cpp
*/
/*
 * @description
 * CWE: 775 Missing Release of File Descriptor or Handle After Effective Lifetime
 * BadSource:  Open a file using open()
 * Sinks:
 *    GoodSink: Close the file using close()
 *    BadSink : Do not close file
 * Flow Variant: 82 Data flow: data passed in a parameter to an virtual method called via a pointer
 *
 * */
#ifndef OMITGOOD

#include "std_testcase.h"
#include "CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82.h"

namespace CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82
{

void CWE775_Missing_Release_of_File_Descriptor_or_Handle__open_no_close_82_goodB2G::action(int data)
{
    /* FIX: If the file is still opened, close it */
    if (data != -1)
    {
        CLOSE(data);
    }
}

}
#endif /* OMITGOOD */
