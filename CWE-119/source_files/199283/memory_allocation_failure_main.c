#include "HeaderFile.h"

volatile int vflag;
volatile int vflag_copy;
volatile int vflag_file;

int idx, sink;
double dsink;
void *psink;

int main(int argc,char*argv[])
{
	if(argv[1])
	{

		vflag_copy = atoi(argv[1]);
		vflag_file = (int)floor((double)vflag_copy/1000.0);
		vflag = (int)floor((int)vflag_copy%1000);
		printf("vflag_file = %d vflag_func = %d vflag_copy =%d \n" , vflag_file, vflag,vflag_copy);

		/*Memory allocation failure main*/
		if (vflag_file ==28 || vflag_file == 888)
		{
			memory_allocation_failure_main();
		}

		printf("Printed from main function ");
	}
	else
	{
		printf("Enter File XXX and Function XXX \n");
		printf("Example: To Execute 2nd File 3rd Function , Enter 002003 \n");
		printf("Example: To Execute All Files ,Enter 888888 \n");
		printf("Example: To Execute All functions in a File :Sample - To execute all functions in 3rd file,Enter 003888 \n");
	}
    return 0;
}
