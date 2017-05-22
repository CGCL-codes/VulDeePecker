#include "HeaderFile.h"

/********Software Analysis - FY2013*************/
/*
* File Name: buffer_overrun_dynamic.c
* Defect Classification
* ---------------------
* Defect Type: Dynamic memory defects
* Defect Sub-type: Dynamic buffer overrun
*
*  Created on: Sep 27, 2012
*      Author: caesaru01
*/

/* Dynamic buffer overrun
 *When using a pointer to char
*/
void dynamic_buffer_overrun_001 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int i;
	if(buf!=NULL)
	{
		for (i=0;i<=5;i++)
	    {
			buf[i]=1; /*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    }
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 *When using a pointer to short
 */
void dynamic_buffer_overrun_002 ()
{
	short *buf=(short*) calloc(5,sizeof(short));
	if(buf!=NULL)
	{
		*(buf+5)=1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer overrun
 *When using a pointer to int
 */
void dynamic_buffer_overrun_003 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int ret;
	int i;
	if(buf!=NULL)
	{
		for (i=0;i<5;i++)
		{
			buf[i]=1;
		}
		ret = buf[5];/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		free(buf);
		printf("%d",ret);
	}
}

/*
 * Dynamic buffer overrun
 *When using a pointer to int
 */
void dynamic_buffer_overrun_004 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
		*(buf+5) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using a pointer to long
 */
void dynamic_buffer_overrun_005 ()
{
	long *buf=(long*) calloc(5,sizeof(long));
	int i;
	if(buf!=NULL)
	{
		for(i=0;i<=5;i++)
		{
			buf[i]=1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
		free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using a pointer to float
 */
void dynamic_buffer_overrun_006 ()
{
	float *buf=(float*) calloc(5,sizeof(float));
	int i;
	if(buf!=NULL)
	{
		for(i=0;i<=5;i++)
		{
			buf[i]=1.0;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using a pointer to float
 */
void dynamic_buffer_overrun_007 ()
{
	double *buf=(double*) calloc(5,sizeof(double));
	int i;
	if(buf!=NULL)
	{
		for(i=0;i<=5;i++)
		{
			buf[i]=1.0;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using double pointers to the type int
 */
void dynamic_buffer_overrun_008 ()
{
	int **buf = (int**) calloc(5,sizeof(int*));
	int i,j;

	for(i=0;i<5;i++)
		buf[i]=(int*) calloc(5,sizeof(int));

	for(i=0;i<5;i++)
	{
		for(j=0;j<=5;j++)
		{
			*(*(buf+i)+j)=i;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
		free(buf[i]);
	}
	free(buf);
}

/*
 * Dynamic buffer overrun
 * When using double pointers to the type int
 */
void dynamic_buffer_overrun_009 ()
{
	int *buf1=(int*)calloc(5,sizeof(int));
	int *buf2=(int*)calloc(5,sizeof(int));
	int *buf3=(int*)calloc(5,sizeof(int));
	int *buf4=(int*)calloc(5,sizeof(int));
	int *buf5=(int*)calloc(5,sizeof(int));
	int **pbuf[5] = {&buf1, &buf2, &buf3, &buf4, &buf5};
	int i,j=6;
	for(i=0;i<5;i++)
	{
		*((*pbuf[i])+j)=5;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	}
	free(buf1);
	free(buf2);
	free(buf3);
	free(buf4);
	free(buf5);
}

/*
 * Dynamic buffer overrun
 * When using pointers to the structure
 */
typedef struct {
	int a;
	int b;
	int c;
} dynamic_buffer_overrun_010_s_001;

void dynamic_buffer_overrun_010 ()
{
	dynamic_buffer_overrun_010_s_001* sbuf= calloc(5,sizeof(dynamic_buffer_overrun_010_s_001)) ;
	if(sbuf!=NULL)
	{
	    sbuf[5].a = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(sbuf);
	}
}

/*
 * Dynamic buffer overrun
 * When using pointers to the structure with an array as a member of the structure
 */
typedef struct {
	int a;
	int b;
	int buf[5];
} dynamic_buffer_overrun_011_s_001;

void dynamic_buffer_overrun_011 ()
{
	dynamic_buffer_overrun_011_s_001* s=(dynamic_buffer_overrun_011_s_001*) calloc(5,sizeof(dynamic_buffer_overrun_011_s_001)) ;
	if(s!=NULL)
	{
		(s+5)->buf[4] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		free(s);
	}
}

/*
 * Dynamic buffer overrun
 * When using a variable to access the array of pointers
 */
void dynamic_buffer_overrun_012 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	if(buf!=NULL)
	{
		*(buf+index)=9;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using a variable which changes on every iteration to access the array of pointers
 */
void  dynamic_buffer_overrun_013()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	if(buf!=NULL)
	{
	    buf[index] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When performing arithmetic operations on the index variable causing it to go out of bounds.
 */
void dynamic_buffer_overrun_014 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 3;
	if(buf!=NULL)
	{
	    *(buf +((2 * index) + 1)) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When performing arithmetic operations on the index variable causing it to go out of bounds.
 */
void dynamic_buffer_overrun_015 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 2;
	if(buf!=NULL)
	{
	    buf[(index * index) + 1] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * When using a return value from a function to access memory
 */

int dynamic_buffer_overrun_016_func_001 ()
{
	return 5;
}

void dynamic_buffer_overrun_016 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    buf[dynamic_buffer_overrun_016_func_001 ()] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun in the function called
 */
void dynamic_buffer_overrun_017_func_001 (int index)
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    *(buf +index) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

void dynamic_buffer_overrun_017 ()
{
	dynamic_buffer_overrun_017_func_001(5);
}

/*
 * Dynamic buffer overrun
 * overrun in when using member of some other array as the index
 */
void dynamic_buffer_overrun_018 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int indexes[4] = {3, 4, 5, 6};
	int index = 4;
	if(buf!=NULL)
	{
	    *(buf+indexes[index]) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using a variable assigned by some other variable
 */
void dynamic_buffer_overrun_019 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	int index1;
	index1 = index;
	if(buf!=NULL)
	{
	    buf[index1] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using 2 aliases
 */
void dynamic_buffer_overrun_020 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	int index1;
	int index2;
	index1 = index;
	index2 = index1;
	if(buf!=NULL)
	{
	    buf[index2] = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using 2 pointer aliases
 */
void dynamic_buffer_overrun_021 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p1;
	int *p2;
	if(buf!=NULL)
	{
		p1 = buf;
		p2 = p1;
		*(p2+5) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using 1 single pointer alias
 */
void dynamic_buffer_overrun_022 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p;
	if(buf!=NULL)
	{
	    p = buf;
	    *(p+5) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun IN A FOR LOOP
 */
void dynamic_buffer_overrun_023 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p;
	int i;
	if(buf!=NULL)
	{
	    p = buf;
	    for (i = 0; i <= 5; i ++)
	    {
		    *p = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		     p ++;
	    }
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when passing the base pointer to a function
 */
void dynamic_buffer_overrun_024_func_001 (int *buf)
{
	*(buf+5) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
}

void dynamic_buffer_overrun_024 ()
{
	int *buf=(int*) calloc(5,sizeof(int));

	dynamic_buffer_overrun_024_func_001(buf);
	if(buf!=NULL)
	{
	    free(buf);
	}
}


/*
 * Dynamic buffer overrun
 * overrun when using a char pointer
 */
void dynamic_buffer_overrun_025 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int i;
	if(buf!=NULL)
	{
		for(i=0;i<=5;i++)
		{
			buf[i]='1';/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
	    free(buf);
	}
}


/*
 * Dynamic buffer overrun
 * overrun when using casting to pointers of larger datatypes (For eg Cast a char to int)
 */
void dynamic_buffer_overrun_026 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int *p;
	p = (int*)buf;
	if(buf!=NULL)
	{
	    *(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using casting to pointers of smaller datatypes (For eg Cast a int to char)
 */
void dynamic_buffer_overrun_027 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	char *p;
	p = (char*)buf;
	if(buf!=NULL)
	{
	    *(p + 30) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer overrun
 * overrun when using values in a different array allocated dynamically
 */
void dynamic_buffer_overrun_028 ()
{
	int *buf1=(int*) calloc(5,sizeof(int));
	int *buf2=(int*) calloc(3,sizeof(int));
	int i;
	    for(i=0;i<5;i++)
		{
	    	*(buf1+i)=i;
		}
    	*(buf2+*(buf1+5))=1;/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    free(buf1);
	    free(buf2);
}

/*
 * Dynamic buffer overrun
 * overrun in while loop
 */
void dynamic_buffer_overrun_029()
{

	int i=1;
	while (i)
	{
		char* buf= (char*) malloc(sizeof(char));
		if(buf!=NULL)
		{
		    buf[i+1]='a';/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		    free(buf);
		}
		i--;
	}
}


/* Types of defects: heap overrun(dynamic buffer overrun)
* Complexity: ---- Write outside the allocated buffer in a 2D array
*/

void dynamic_buffer_overrun_030()
{
	int i,j;
	char ** doubleptr=(char**) malloc(10*sizeof(char*));

	for(i=0;i<10;i++)
	{
		doubleptr[i]=(char*) malloc(10*sizeof(char));
	}


	for(i=0;i<10;i++)
	{
		for(j=0;j<=10;j++)
		{
		  doubleptr[i][j]='a';    	/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
		}
		free(doubleptr[i]);
	}	
	free(doubleptr);
}

/* Types of defects: heap overrun(dynamic buffer overrun)
* Complexity: ---- overrun while using memcpy function
*/

void dynamic_buffer_overrun_031()
{
	int i;
	char* ptr1=(char*) calloc(12, sizeof(char));
	char a[12],*ptr2 = a;

	if(ptr1!=NULL)
	{
	    for(i=0;i<=12;i++)
	    {
	    	ptr1[i]='a';/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*/
	    }
    	ptr1[11]='\0';
	    memcpy(ptr2,ptr1,12);//vm
	    free(ptr1);
	}
}

/* Types of defects: heap overrun(dynamic buffer overrun)
* Complexity: ---- overrun when using a structure
*/


typedef struct
{
	char arr[10];
	int arri[10];
}dynamic_buffer_overrun_s_005;

void dynamic_buffer_overrun_032()
{	
	dynamic_buffer_overrun_s_005* ptr_s= malloc(10*sizeof(dynamic_buffer_overrun_s_005));
	int i;
	if(ptr_s!=NULL)
	{
	    for(i=0;i<=10;i++)
	    {
	    	ptr_s[i].arr[i]='a';/*Tool should detect this line as error*/ /*ERROR:Buffer overrun*///vm - Changed arri(int) to arr(char);
	    }
	    free(ptr_s);
	}
}

/* Types of defects: heap overrun(dynamic buffer overrun)
*  dynamic buffer overrun main - Function call
*/
extern volatile int vflag;
void dynamic_buffer_overrun_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		dynamic_buffer_overrun_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		dynamic_buffer_overrun_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		dynamic_buffer_overrun_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		dynamic_buffer_overrun_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		dynamic_buffer_overrun_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		dynamic_buffer_overrun_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		dynamic_buffer_overrun_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		dynamic_buffer_overrun_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		dynamic_buffer_overrun_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		dynamic_buffer_overrun_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		dynamic_buffer_overrun_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		dynamic_buffer_overrun_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		dynamic_buffer_overrun_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		dynamic_buffer_overrun_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		dynamic_buffer_overrun_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		dynamic_buffer_overrun_016();
	}

	if (vflag == 17 || vflag ==888)
	{
		dynamic_buffer_overrun_017();
	}

	if (vflag == 18 || vflag ==888)
	{
		dynamic_buffer_overrun_018();
	}

	if (vflag == 19 || vflag ==888)
	{
		dynamic_buffer_overrun_019();
	}

	if (vflag == 20 || vflag ==888)
	{
		dynamic_buffer_overrun_020();
	}

	if (vflag == 21 || vflag ==888)
	{
		dynamic_buffer_overrun_021();
	}

	if (vflag == 22 || vflag ==888)
	{
		dynamic_buffer_overrun_022();
	}

	if (vflag == 23 || vflag ==888)
	{
		dynamic_buffer_overrun_023();
	}

	if (vflag == 24 || vflag ==888)
	{
		dynamic_buffer_overrun_024();
	}

	if (vflag == 25 || vflag ==888)
	{
		dynamic_buffer_overrun_025();
	}

	if (vflag == 26 || vflag ==888)
	{
		dynamic_buffer_overrun_026();
	}

	if (vflag == 27 || vflag ==888)
	{
		dynamic_buffer_overrun_027();
	}

	if (vflag == 28 || vflag ==888)
	{
		dynamic_buffer_overrun_028();
	}

	if (vflag == 29 || vflag ==888)
	{
		dynamic_buffer_overrun_029();
	}

	if (vflag == 30 || vflag ==888)
	{
		dynamic_buffer_overrun_030();
	}

	if (vflag == 31 || vflag ==888)
	{
		dynamic_buffer_overrun_031();
	}

	if (vflag == 32 || vflag ==888)
	{
		dynamic_buffer_overrun_032();
	}
}
