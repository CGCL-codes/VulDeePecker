/********Software Analysis - FY2013*************/
/*
* File Name: double_free.c
* Defect Classification
* ---------------------
* Defect Type: Resource management defects
* Defect Sub-type: Double free
* Description: Defect Free Code to identify false positives in double free - resource management defects
*/

#include "HeaderFile.h"
/*
* Types of defects: Double free
* Complexity: Basic type where pointer is "freed"  twice
*/

void double_free_001()
{
	char* ptr= (char*) malloc(sizeof(char));

	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Basic type where pointer is "freed" in a if condition in a loop
*/

void double_free_002()
{
	char* ptr= (char*) malloc(10*sizeof(char));
	int i;
	
	for(i=0;i<10;i++)
	{
		ptr[i]='a';
		if(i==10)
		free(ptr);
	}
	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Basic type where pointer is "freed" in a loop and then outside the loop
*/

void double_free_003()
{
	char* ptr= (char*) malloc(10*sizeof(char));
	int i;
	
	for(i=0;i<10;i++)
	{
		*(ptr+i)='a';
		
	}

	free(ptr);  /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Memory is free in a if statement
*/

void double_free_004()
{
	char* ptr= (char*) malloc(10*sizeof(char));
	int i;
	for(i=0;i<10;i++)
	{
		*(ptr+i)='a';

	}
	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Memory is free in a if statement
*/

void double_free_005()
{
	char* ptr= (char*) malloc(sizeof(char));

	if(ptr)
	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Memory is free in a constant if statement
*/

void double_free_006()
{
	char* ptr= (char*) malloc(sizeof(char));
	if(0)
	free(ptr);

	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity: Memory is free in a variable if statement
*/

void double_free_007()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=0;
	
	if(flag<0)
	free(ptr);

	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity:Free in a function
*/
char *double_free_function_008_gbl_ptr;
void double_free_function_008()
{
	free (double_free_function_008_gbl_ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

void double_free_008()
{
	double_free_function_008_gbl_ptr= (char*) malloc(sizeof(char));

	double_free_function_008();
}

/*
* Types of defects: Double free
* Complexity:Free in a while loop with a variable
*/


void double_free_009()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=0;

	while(flag==1)
	{
		free(ptr);
		flag++;
	}
	free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
}

/*
* Types of defects: Double free
* Complexity:Free in a while loop with a constant
*/


void double_free_010()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=1;

	while(flag)
	{
		free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
		flag--;
	}
}

/*
* Types of defects: Double free
* Complexity:double Free in a while loop with a condition
*/


void double_free_011()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=1,a=0,b=1;

	while(a<b)
	{
		if(flag ==1)
		free(ptr);  /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
		a++;
	}
}

/*
* Types of defects: Double free
* Complexity:double Free in a for loop
*/


void double_free_012()
{
	char* ptr= (char*) malloc(sizeof(char));
	int a=0;

	for(a=0;a<1;a++)
	{
		free(ptr); /*Tool should Not detect this line as error*/ /*No ERROR:Double free*/
	}
}

/*
* Types of defects: Double free
* double free main function
*/
extern volatile int vflag;
void double_free_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		double_free_001 ();
	}

    if (vflag == 2 || vflag ==888)
    {
    	double_free_002 ();
    }

    if (vflag == 3 || vflag ==888)
    {
    	double_free_003 ();
    }

    if (vflag == 4 || vflag ==888)
    {
    	double_free_004 ();
    }

    if (vflag == 5 || vflag ==888)
    {
    	double_free_005 ();
    }

    if (vflag == 6 || vflag ==888)
    {
    	double_free_006 ();
    }

    if (vflag == 7 || vflag ==888)
    {
    	double_free_007 ();
    }

    if (vflag == 8 || vflag ==888)
    {
    	double_free_008 ();
    }

    if (vflag == 9 || vflag ==888)
    {
    	double_free_009 ();
    }

    if (vflag == 10 || vflag ==888)
    {
    	double_free_010 ();
    }

    if (vflag == 11 || vflag ==888)
    {
    	double_free_011 ();
    }

    if (vflag == 12 || vflag ==888)
    {
    	double_free_012 ();
    }

}
