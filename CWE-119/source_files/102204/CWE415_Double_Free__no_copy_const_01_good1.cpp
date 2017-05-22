/*
 * @description Double Free
 * 
 * */

#include "std_testcase.h"

namespace CWE415_Double_Free__no_copy_const_01
{

#ifndef OMITGOOD

class GoodClass 
{
    public:
        GoodClass(const char *data)
        {
            if (data) 
            {
                this->data = new char[strlen(data) + 1];
                strcpy(this->data, data);
            } 
            else 
            {
                this->data = new char[1];
                *(this->data) = '\0';
            }
        }

        ~GoodClass()
        {
            delete [] data;
        }

        void printData()
        {
            printLine(data);
        }

        GoodClass(GoodClass &goodClassObject)
        { 
            this->data = new char[strlen(goodClassObject.data) + 1];
            strcpy(this->data, goodClassObject.data);
        }

        GoodClass& operator=(const GoodClass &goodClassObject) 
        { 
            if (&goodClassObject != this) 
            { 
                this->data = new char[strlen(goodClassObject.data) + 1];
                strcpy(this->data, goodClassObject.data); 
            } 
            return *this; 
        }

    private:
        char *data;
};

static void good1()
{
    GoodClass goodClassObject("One");

    /* FIX: There is a copy constructor defined in the class */
    GoodClass goodClassObjectCopy(goodClassObject);

    goodClassObjectCopy.printData();
}

void good()
{
    good1();
}

#endif /* OMITGOOD */

} /* close namespace */

/* Below is the main(). It is only used when building this testcase on 
 * its own for testing or for building a binary to use in testing binary 
 * analysis tools. It is not used when compiling all the testcases as one 
 * application, which is how source code analysis tools are tested. 
 */ 

#ifdef INCLUDEMAIN

using namespace CWE415_Double_Free__no_copy_const_01; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
#ifndef OMITGOOD
    printLine("Calling good()...");
    good();
    printLine("Finished good()");
#endif /* OMITGOOD */
    return 0;
}

#endif
