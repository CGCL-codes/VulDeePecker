/********Software Analysis - FY2013*************/
/*
* File Name: uninit_pointer.c
* Defect Classification
* ---------------------
* Defect Type: Pointer related defects
* Defect Sub-type: Uninitialized pointer
* Description: Defect Code to identify uninitialized pointer access in the code flow
*/
/*
 * uninit_pointer.c
 *
 *  Created on: Sep 20, 2013
 *      Author: hemalatha
 */

#include "HeaderFile.h"

char ** uninit_pointer_016_gbl_doubleptr;
/*
* Types of defects: Uninitialized pointer
* Complexity: When using a single pointer int	Variable - Loading
*/
void uninit_pointer_001 ()
{
	int a = 5;
	int *p ;  
	int ret;
	ret = *p; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: 1 single pointer	int	Variable	Write
*/
void uninit_pointer_002 ()
{
	int a;
	int *p ;
	*p = 1; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: When using a double pointer	int	Variable	Write
*/
void uninit_pointer_003 ()
{
	int **pp;
	int *p;
	int a = 0;
	int ret;
	pp = &p;
	ret = **pp; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: 1 single pointer	int	Function arguments	Loading
*/
void uninit_pointer_004_func_001 (int *p)
{
	int ret;
	ret = 0;
}
void uninit_pointer_004 ()
{
	int a = 0;
	int *p ;
	uninit_pointer_004_func_001(p);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: pointer arrays	int	Function arguments	Loading
*/
void uninit_pointer_005_func_001 (int *pbuf[])
{
	int buf1[6] = {1, 2, 3, 4, 5, 6};
	int buf2[6] = {1, 2, 3, 4, 5, 6};
	int buf3[6] = {1, 2, 3, 4, 5, 6};
	int buf4[6] = {1, 2, 3, 4, 5, 6};
	int buf5[6] = {1, 2, 3, 4, 5, 6};
	pbuf[0] = buf1;
	pbuf[2] = buf3;
	pbuf[3] = buf4;
	pbuf[4] = buf5;
	int ret;
	ret = pbuf[1][1];
}
void uninit_pointer_005 ()
{
	int *pbuf[5];
	uninit_pointer_005_func_001(pbuf);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: double pointer int	Function arguments	Write
*/
void uninit_pointer_006_func_001 (int **pp)
{
	**pp = 1;/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

void uninit_pointer_006 ()
{
	int a;
	int *p;
	uninit_pointer_006_func_001(&p);
}

/*
* Types of defects: Uninitialized pointer
* Complexity: When using a double char pointer arrays
*/
void uninit_pointer_007 ()
{
        char *buf1=strdup("String1");
	char *buf2;
	char *buf3=strdup("String3");
	char *buf4=strdup("String4");
	char *buf5=strdup("String5");
        if (!buf1 || !buf3 || !buf4 || !buf5) return;
	char **pbuf[5] = {&buf2, &buf3, &buf4, &buf5};
	int i,j=4;

	for(i=0;i<5;i++)
	{
		*((*pbuf[i])+j)='a';/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
	}
        free(buf1);
        free(buf3);
        free(buf4);
        free(buf5);
}

/*
* Types of defects: Uninitialized pointer
* Complexity: structure	int	Function arguments	Loading
*/
typedef struct {
	int a;
	int b;
	int uninit;
} uninit_pointer_008_s_001;

void uninit_pointer_008_func_001 (uninit_pointer_008_s_001 *p)
{
	int ret;
	p->uninit=ret;
}
void uninit_pointer_008 ()
{
	uninit_pointer_008_s_001 s;
	s.a = 1;
	s.b = 1;
	uninit_pointer_008_func_001(&s);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: When using a pointer to char and initialized based on return value of function
*/
int uninit_pointer_009_func_001(int flag)
{
   int ret;
   if (flag ==0)
	   ret = 0;
   else
	   ret=flag+1;
   return ret;
}

void uninit_pointer_009()
{
	char *buf,buf1[25];
	int flag=10;

	if(uninit_pointer_009_func_001(flag)==0)
	{
		buf = "This is a string";
	}
    if(uninit_pointer_009_func_001(flag)>0)
    {
    	strcpy(buf1,buf);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
    }
}

/*
* Types of defects: Uninitialized pointer
* Complexity: void pointer to data passed from one function to another
*/
void uninit_pointer_010_func_001 (void * vptr)
{
    /* cast void pointer to a pointer of the appropriate type */
    char * * cptr = (char * *)vptr;
    char * buf ;
    buf = (*cptr);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}
void uninit_pointer_010 ()
{
    void *buf1;
    uninit_pointer_010_func_001(&buf1);
}

/*
* Types of defects: Uninitialized pointer
* Complexity: When using a unsigned int pointer and partially initialized in a infinite loop
*/
void uninit_pointer_011 ()
{
    unsigned int * ptr,a=0;
    ptr = (unsigned int *)malloc(10*sizeof(unsigned int *));
    int i;
	if (ptr!=NULL)
	{
		while(1)
       {
           for(i=0; i<10/2; i++)
           {
            	ptr[i] = i;
           }
           break;
       }
       while(1)
       {
          for(i=0; i<10; i++)
          {
        	  a += ptr[i];/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
          }
          break;
       }
	}
}

/*
* Types of defects: Uninitialized pointer
* Complexity: When using a pointer to float , pointing to 1D array
*/
void uninit_pointer_012 ()
{
	float * fptr;
	float arr[10];
	fptr = arr;
	int i;
	for(i=0; i<10/2; i++)
	{
		fptr[i] = (float)i;
	}
	for(i=0; i<10; i++)
	{
		arr[i] = ++fptr[i];/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
	}
}

/*
* Types of defects: Uninitialized pointer
* Complexity: Double int pointer partially initialized with 2D array
*/
void uninit_pointer_013 ()
{
	int **ptr = (int**) malloc(5*sizeof(int*));
		int i,j;

		for(i=0;i<5;i++)
			ptr[i]=(int*) malloc(5*sizeof(int));
	    int arr[3][3] = {{1,2,3},
				         {11,12,13},
		                 {21,22,23}};

		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				*(*(ptr+i)+j)= arr[i][j];/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
			}
		    free(ptr[i]);
		    ptr[i] = NULL;
		}
		free(ptr);
		ptr = NULL;
}

/*
 * Types of defects: uninitialized variable
 * Complexity: Structure passed as function parameter and based on the return value of function another structure is initialized
 */
typedef struct {
	int a;
	int b;
	int uninit;
} uninit_pointer_014_s_001;

uninit_pointer_014_s_001 *s;
void uninit_pointer_014_func_001 (int flag)
{
	switch (flag)
	{
		case 1:
		{
			s = (uninit_pointer_014_s_001 *)calloc(1,sizeof(uninit_pointer_014_s_001));
			if(s!=NULL)
			{
				s->a = 10;
			    s->b = 10;
			}
			break;
		}
		case 2:
		{
			s = (uninit_pointer_014_s_001 *)calloc(1,sizeof(uninit_pointer_014_s_001));
			if(s!=NULL)
			{
				s->a = 20;
			    s->b = 20;
			}
			break;
		}
		default:
		{
			break;
		}

	}
}

void uninit_pointer_014 ()
{
	uninit_pointer_014_s_001 r;
	uninit_pointer_014_func_001 (1);
	if(s!=NULL)
	{
	    r = *s;/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
	    free(s);
	}
}

/*
* Types of defects: Uninitialized pointer
* Complexity: int variable initialized with function return value using enumeration
 */
typedef enum
            {ZERO,
	         ONE,
	         TWO } values;

void uninit_pointer_015_func_001 (int *ptr )
{
	values val = ONE;
	*ptr = val;
}

void uninit_pointer_015 ()
{
    int  *ptr;
    uninit_pointer_015_func_001(ptr);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

/*
* Types of defects: Uninitialized pointer
* Complexity: global char double pointer int variable initialized in another function
 */
int uninit_pointer_016_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void uninit_pointer_016_func_002()
{
	int i;
	if(uninit_pointer_016_func_001(0)==0)
	{
		uninit_pointer_016_gbl_doubleptr=(char**) malloc(10*sizeof(char*));
    	for(i=0;i<10;i++)
	    {
    		uninit_pointer_016_gbl_doubleptr[i]=(char*) malloc(10*sizeof(char));
			if(i<5)
    		   strcpy(uninit_pointer_016_gbl_doubleptr[i],"STRING00");
	    }
	}
}

void uninit_pointer_016()
{
	int flag=0,i;
	char *s=(char*) malloc(10*sizeof(char));
	if(uninit_pointer_016_func_001(flag)==0)
	{
		uninit_pointer_016_func_002();
	}

	if(uninit_pointer_016_gbl_doubleptr!=NULL)
	{
		for(i=0;i<10;i++)
		{
			if(uninit_pointer_016_gbl_doubleptr[i] !=NULL)
			{
				if(i==7)
				    strcpy(s,uninit_pointer_016_gbl_doubleptr[i]);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
			free (uninit_pointer_016_gbl_doubleptr[i]);
			}
	    }
	    free(uninit_pointer_016_gbl_doubleptr);
	    free(s);
	}
}
/*
* Types of defects: Uninitialized pointer
* Complexity:Uninitialized pointer main function
*/
extern volatile int vflag;
void uninit_pointer_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		uninit_pointer_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		uninit_pointer_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		uninit_pointer_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		uninit_pointer_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		uninit_pointer_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		uninit_pointer_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		uninit_pointer_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		uninit_pointer_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		uninit_pointer_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		uninit_pointer_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		uninit_pointer_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		uninit_pointer_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		uninit_pointer_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		uninit_pointer_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		uninit_pointer_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		uninit_pointer_016();
	}
}

