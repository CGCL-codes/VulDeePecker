/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84a.cpp
Label Definition File: CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open.label.xml
Template File: source-sinks-84a.tmpl.cpp
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

#include "std_testcase.h"
#include "CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84.h"

namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84
{

#ifndef OMITBAD

void bad()
{
    int data;
    /* Initialize data */
    data = -1;
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_bad * badObject = new CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_bad(data);
    delete badObject;
}

#endif /* OMITBAD */

#ifndef OMITGOOD

/* goodB2G uses the BadSource with the GoodSink */
static void goodB2G()
{
    int data;
    /* Initialize data */
    data = -1;
    CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G * goodB2GObject = new CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84_goodB2G(data);
    delete goodB2GObject;
}

void good()
{
    goodB2G();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on
   its own for testing or for building a binary to use in testing binary
   analysis tools. It is not used when compiling all the testcases as one
   application, which is how source code analysis tools are tested. */

#ifdef INCLUDEMAIN

using namespace CWE773_Missing_Reference_to_Active_File_Descriptor_or_Handle__open_84; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    good();
    printLine("Finished good()");
#endif /* OMITGOOD */
#ifndef OMITBAD
    printLine("Calling bad()...");
    bad();
    printLine("Finished bad()");
#endif /* OMITBAD */
    return 0;
}

#endif
