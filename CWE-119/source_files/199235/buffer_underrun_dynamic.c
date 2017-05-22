#include "HeaderFile.h"

/********Software Analysis - FY2013*************/
/*
* File Name: buffer_underrun_dynamic.c
* Defect Classification
* ---------------------
* Defect Type: Dynamic memory defects
* Defect Sub-type: Dynamic buffer underrun
*
*  Created on: Sep 27, 2012
*      Author: caesaru01
*/

/*
 * Dynamic buffer underrun
 * When using a pointer to char
 */
void dynamic_buffer_underrun_001 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int i;
	printf("print bufferundr");
	if(buf!=NULL)
	{
	for (i=4;i>=-1;i--)
	    {
		buf[i]=1;  /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    }
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 *When using a pointer to short
 */
void dynamic_buffer_underrun_002 ()
{
	printf("underrun2");
	short *buf=(short*) calloc(5,sizeof(short));
	if(buf!=NULL)
	{
	    *(buf-5)=1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 *When using a pointer to int
 */
void dynamic_buffer_underrun_003 ()
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
	ret = buf[-1]; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
		printf("%d",ret);
	}
}

/*
 * Dynamic buffer underrun
 *When using a pointer to int
 */
void dynamic_buffer_underrun_004 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    *(buf-5) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When using a pointer to long
 */
void dynamic_buffer_underrun_005 ()
{
	long *buf=(long*) calloc(5,sizeof(long));
	int i;
	if(buf!=NULL)
	{
	    for(i=-1;i<5;i++)
		{
	        buf[i]=1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When using a pointer to float
 */
void dynamic_buffer_underrun_006 ()
{
	float *buf=(float*) calloc(5,sizeof(float));
	int i;
	if(buf!=NULL)
	{
		for(i=-1;i<5;i++)
		{
			buf[i]=1.0; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When using a pointer to double
 */
void dynamic_buffer_underrun_007 ()
{
	double *buf=(double*) calloc(5,sizeof(double));
	int i;
	if(buf!=NULL)
	{
		for(i=-1;i<5;i++)
		{
			buf[i]=1.0; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When using double pointers to the type int
 */
void dynamic_buffer_underrun_008 ()
{
	int **buf = (int**) calloc(5,sizeof(int*));
	int i,j;

	for(i=0;i<5;i++)
		buf[i]=(int*) calloc(5,sizeof(int));

	for(i=-1;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			*(*(buf+i)+j)=i; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
		if(i>0)
		free(buf[i]);
	}
	free(buf);
}

/*
 * Dynamic buffer underrun
 * When using double pointers to the type int
 */
void dynamic_buffer_underrun_009 ()
{
	int *buf1=(int*)calloc(5,sizeof(int));
	int *buf2=(int*)calloc(5,sizeof(int));
	int *buf3=(int*)calloc(5,sizeof(int));
	int *buf4=(int*)calloc(5,sizeof(int));
	int *buf5=(int*)calloc(5,sizeof(int));
	int **pbuf[5] = {&buf1, &buf2, &buf3, &buf4, &buf5};
	int i,j=4;
	for(i=0;i<5;i++)
	{
		*((*pbuf[i-3])+j)=5; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	}
	free(buf1);
	free(buf2);
	free(buf3);
	free(buf4);
	free(buf5);
}

/*
 * Dynamic buffer underrun
 * When using pointers to the structure
 */
typedef struct {
	int a;
	int b;
	int c;
} dynamic_buffer_underrun_010_s_001;

void dynamic_buffer_underrun_010 ()
{
	dynamic_buffer_underrun_010_s_001* sbuf= (dynamic_buffer_underrun_010_s_001*) calloc(5,sizeof(dynamic_buffer_underrun_010_s_001)) ;
	if(sbuf!=NULL)
	{
	    sbuf[-1].a = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(sbuf);
	}
}

/*
 * Dynamic buffer underrun
 * When using pointers to the structure with an array as a member of the structure
 */
typedef struct {
	int a;
	int b;
	int buf[5];
} dynamic_buffer_underrun_011_s_001;

void dynamic_buffer_underrun_011 ()
{
	dynamic_buffer_underrun_011_s_001* s=(dynamic_buffer_underrun_011_s_001*) calloc(5,sizeof(dynamic_buffer_underrun_011_s_001)) ;
	if(s!=NULL)
	{
		(s-5)->buf[4] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(s);
	}
}

/*
 * Dynamic buffer underrun
 * When using a variable to access the array of pointers
 */
void dynamic_buffer_underrun_012 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	if(buf!=NULL)
	{
	    *(buf-index)=9; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}
/*
 * Dynamic buffer underrun
 * When using a variable which changes on every iteration to access the array of pointers
 */
void  dynamic_buffer_underrun_013()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 5;
	index = rand()-2;
	if(buf!=NULL)
    {

	    buf[-index] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When performing arithmetic operations on the index variable causing it to go out of bounds.
 */
void dynamic_buffer_underrun_014 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 3;
	if(buf!=NULL)
	{
    	*(buf +((-2 * index) + 1)) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * When performing arithmetic operations on the index variable causing it to go out of bounds.
 */
void dynamic_buffer_underrun_015 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = 2;
	if(buf!=NULL)
	{
	    buf[(index * index) - 5] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
    }
}

/*
 * Dynamic buffer underrun
 * When using a return value from a function to access memory
 */

int dynamic_buffer_underrun_016_func_001 ()
{
	return -1;
}

void dynamic_buffer_underrun_016 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    buf[dynamic_buffer_underrun_016_func_001 ()] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun in the function called
 */
void dynamic_buffer_underrun_017_func_001 (int index)
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    *(buf -index) = 1;/*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

void dynamic_buffer_underrun_017 ()
{
	dynamic_buffer_underrun_017_func_001(5);
}

/*
 * Dynamic buffer underrun
 * underrun in when using member of some other array as the index
 */
void dynamic_buffer_underrun_018 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int indexes[4] = {3, 4, 5, 6};
	int index = 2;
	if(buf!=NULL)
	{
	    *(buf-indexes[index]) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using a variable assigned by some other variable
 */
void dynamic_buffer_underrun_019 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = -5;
	int index1;
	index1 = index;
	if(buf!=NULL)
	{
	    buf[index1] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using 2 aliases
 */
void dynamic_buffer_underrun_020 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int index = -5;
	int index1;
	int index2;
	index1 = index;
	index2 = index1;
	if(buf!=NULL)
	{
	    buf[index2] = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using 2 pointer aliases
 */
void dynamic_buffer_underrun_021 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p1;
	int *p2;
	if(buf!=NULL)
	{
		p1 = buf;
		p2 = p1;
		*(p2-5) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using 1 single pointer alias
 */
void dynamic_buffer_underrun_022 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p;
	if(buf!=NULL)
	{
	    p = buf;
	    *(p-5) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun IN A FOR LOOP
 */
void dynamic_buffer_underrun_023 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	int *p;
	int i;
	if(buf!=NULL)
	{
	    p = buf;
	    for (i = 4; i >=-1; i--)
	    {
	    	p[i]='1'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    }
	    free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when passing the base pointer to a function
 */
void dynamic_buffer_underrun_024_func_001 (int *buf)
{
	*(buf-5) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
}

void dynamic_buffer_underrun_024 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	if(buf!=NULL)
	{
	    dynamic_buffer_underrun_024_func_001(buf);
	    free(buf);
	}
}


/*
 * Dynamic buffer underrun
 * underrun when using a char pointer
 */
void dynamic_buffer_underrun_025 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int i;

	if(buf!=NULL)
	{
	    for(i=4;i>=-1;i--)
	    {
	    	buf[i]='1'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    }
	    free(buf);
	}
}


/*
 * Dynamic buffer underrun
 * underrun when using casting to pointers of larger datatypes (For eg Cast a char to int)
 */
void dynamic_buffer_underrun_026 ()
{
	char *buf=(char*) calloc(5,sizeof(char));
	int *p;
	if(buf!=NULL)
	{
	     p = (int*)buf;
	     *(p - 10) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	     free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using casting to pointers of smaller datatypes (For eg Cast a int to char)
 */
void dynamic_buffer_underrun_027 ()
{
	int *buf=(int*) calloc(5,sizeof(int));
	char *p;
	if(buf!=NULL)
	{
		p = (char*)buf;
		*(p - 10) = 1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(buf);
	}
}

/*
 * Dynamic buffer underrun
 * underrun when using values in a different array allocated dynamically
 */
void dynamic_buffer_underrun_028 ()
{
	int *buf1=(int*) calloc(5,sizeof(int));
	int *buf2=(int*) calloc(3,sizeof(int));
	int i;

	for(i=0;i<5;i++)
	{
		*(buf1+i)=i;
	}
	*(buf2-*(buf1+4))=1; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	free(buf1);
	free(buf2);
}

void dynamic_buffer_underrun_029()
{

	while (1)
	{
		char* buf= (char*) malloc(sizeof(char));
		if(buf!=NULL)
		{
		    buf[-1]='a'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		    free(buf);
		}
	    break;
	}
}


/* Types of defects: heap underrun(dynamic buffer underrun)
* Complexity: ---- Write outside the allocated buffer in a 2D array
*/

void dynamic_buffer_underrun_030()
{
	int i,j;
	char ** doubleptr=(char**) malloc(10*sizeof(char*));

	for(i=0;i<10;i++)
	{
		doubleptr[i]=calloc(10,sizeof(char));
	}


	for(i=0;i<10;i++)
	{
		for(j=9;j>=-1;j--)
		{
		  doubleptr[i][j]='a'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
		free(doubleptr[i]);
	}
	free(doubleptr);
}

/* Types of defects: heap underrun(dynamic buffer underrun)
* Complexity: ---- underrun while using memcpy function
*/

void dynamic_buffer_underrun_031()
{
	int i;
	char* ptr1=(char*) calloc(12, sizeof(char));
	char a[12],*ptr2 = a;
	if(ptr1!=NULL)
	{
	    ptr1[11]='\0';
	    for(i=10;i>=-1;i--) /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    {
	        ptr1[i]='a';
	    }
	    memcpy(ptr2,ptr1,12);
	    free(ptr1);
	}
}

/* Types of defects: heap underrun(dynamic buffer underrun)
* Complexity: ---- underrun when using a structure
*/


typedef struct
{
	char arr[10];
	int arri[10];
}dynamic_buffer_underrun_s_032;

void dynamic_buffer_underrun_032()
{
	dynamic_buffer_underrun_s_032* ptr_s= calloc(10,sizeof(dynamic_buffer_underrun_s_032));
	int i;

	if(ptr_s!=NULL)
	{
		for(i=-1;i<10;i++)
	    ptr_s[i].arr[i]='a'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		free(ptr_s);
	}
}

void dynamic_buffer_underrun_033()
{
	char *message = (char*) calloc(12, sizeof(char));
	int index=12;
	int len=index-12;
	int c;
    if(message !=NULL)
    {
	    while(len>=-2 )
	    {
		   c = message[len];
		   if(isspace(c))
		   {
			   message[len]='\n'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		   }

	    len--;
        }
		free(message);
	}
}

/*
* Types of defects: Buffer underrun
* Complexity: Buffer Underrun when using strings
*/

void dynamic_buffer_underrun_034()
{
	char* srcbuf="Test Code";
	int i,loc=1;
	char* destbuf=(char*) malloc(10*sizeof(char));
	char ch='o';
	if(destbuf!=NULL)
	{
		for(i=-1;i<10;i++)
	    {
		     if(srcbuf[i]==ch) /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		     {
			     loc=i;
		     }
	    }
	    strncpy(&destbuf[loc],&srcbuf[loc],1);
	    free(destbuf);
	}
}

/*
* Types of defects: Buffer underrun
* Complexity: Buffer Underrun when using double pointers
*/

void dynamic_buffer_underrun_035()
{
	int i,loc1=0,loc2=0;
	char** doubleptr=(char**) malloc(10* sizeof(char*));
	
	for (i=0;i<10;i++)
	doubleptr[i]=(char*) malloc(10*sizeof(char));
	
	if (loc1==0)
	loc1--;

	doubleptr[loc1][loc2]='T';	
	
	if(loc2==0)
	loc2--;

	doubleptr[0][loc2]='T'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	for (i=0;i<10;i++)
	{
		free(doubleptr[i]);
	}
	free(doubleptr);
}


/*
* Types of defects: Buffer underrun 
* Complexity: Buffer Underrun when doing a short read using memcpy
*/

void dynamic_buffer_underrun_036()
{
	  char test[]="This is a test code";
	  char *newTest= (char*) malloc(10*sizeof(char));
	  if(newTest!=NULL)
	  {
	     memcpy (newTest,test,10);
	     char c ;
	     c = newTest[-10]; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	     free(newTest);
	  }
 }


/*
* Types of defects: Buffer underrun 
* Complexity: Buffer Underrun when doing a short read
*/

void dynamic_buffer_underrun_037()
{
	int i;
	char** doubleptr=(char**) malloc(10* sizeof(char*));
	
	if(doubleptr!=NULL)
	{
	    for (i=0;i<10;i++)
	    {
	    	doubleptr[i-10]=(char*) malloc(10*sizeof(char)); /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    	if(doubleptr[i]!=NULL)
	    	{
	    	    doubleptr[0][0]='T';
			    free(doubleptr[i]);
	    	}
	    }
		free(doubleptr);
	}
}

/*
* Types of defects: Buffer underrun 
* Complexity: Buffer Underrun when using structures
*/

typedef struct
{
	char arr[10];
	int arri[10];

}dynamic_buffer_underrun_s_038;
void dynamic_buffer_underrun_038()
{
	dynamic_buffer_underrun_s_038* new_s = malloc(10*sizeof(dynamic_buffer_underrun_s_038));
/*	dynamic_buffer_underrun_s_007* ptr_s=malloc(10*sizeof(dynamic_buffer_underrun_s_007));*/
	int loc=-1,i=0;

	if(new_s!=NULL)
	{
		new_s[loc].arr[i]='a'; /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
	    new_s[0].arri[i]=2;
	    free(new_s);
	}
}

/*
* Types of defects: Buffer underrun 
* Complexity: Buffer Underrun when using memcpy to copy structures
*/
typedef struct
{
	char arr[10];
	int arri[10];

}dynamic_buffer_underrun_s_008;
void dynamic_buffer_underrun_039()
{
	/*dynamic_buffer_underrun_s_008* new_s;*/
	dynamic_buffer_underrun_s_008* ptr_s1=malloc(15*sizeof(dynamic_buffer_underrun_s_008));
	dynamic_buffer_underrun_s_008  ptr_s2[15];
	int i;

	if(ptr_s1 != NULL)
	{
		for(i=-10;i<15;i++)
		{
			memset(ptr_s1,1,15*sizeof(dynamic_buffer_underrun_s_008)); /*Tool should detect this line as error*/ /*ERROR:Buffer Underrun*/
		}
	    memcpy(ptr_s2,ptr_s1,15*sizeof(dynamic_buffer_underrun_s_008));
	    free(ptr_s1);
	}
}


/*
* Types of defects: Buffer underrun
* Buffer Underrun main function
*/
extern volatile int vflag;
void dynamic_buffer_underrun_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		dynamic_buffer_underrun_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		dynamic_buffer_underrun_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		dynamic_buffer_underrun_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		dynamic_buffer_underrun_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		dynamic_buffer_underrun_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		dynamic_buffer_underrun_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		dynamic_buffer_underrun_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		dynamic_buffer_underrun_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		dynamic_buffer_underrun_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		dynamic_buffer_underrun_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		dynamic_buffer_underrun_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		dynamic_buffer_underrun_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		dynamic_buffer_underrun_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		dynamic_buffer_underrun_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		dynamic_buffer_underrun_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		dynamic_buffer_underrun_016();
	}

	if (vflag == 17 || vflag ==888)
	{
		dynamic_buffer_underrun_017();
	}

	if (vflag == 18 || vflag ==888)
	{
		dynamic_buffer_underrun_018();
	}

	if (vflag == 19 || vflag ==888)
	{
		dynamic_buffer_underrun_019();
	}

	if (vflag == 20 || vflag ==888)
	{
		dynamic_buffer_underrun_020();
	}

	if (vflag == 21 || vflag ==888)
	{
		dynamic_buffer_underrun_021();
	}

	if (vflag == 22 || vflag ==888)
	{
		dynamic_buffer_underrun_022();
	}

	if (vflag == 23 || vflag ==888)
	{
		dynamic_buffer_underrun_023();
	}

	if (vflag == 24 || vflag ==888)
	{
		dynamic_buffer_underrun_024();
	}

	if (vflag == 25 || vflag ==888)
	{
		dynamic_buffer_underrun_025();
	}

	if (vflag == 26 || vflag ==888)
	{
		dynamic_buffer_underrun_026();
	}

	if (vflag == 27 || vflag ==888)
	{
		dynamic_buffer_underrun_027();
	}

	if (vflag == 28 || vflag ==888)
	{
		dynamic_buffer_underrun_028();
	}

	if (vflag == 29 || vflag ==888)
	{
		dynamic_buffer_underrun_029();
	}

	if (vflag == 30 || vflag ==888)
	{
		dynamic_buffer_underrun_030();
	}

	if (vflag == 31 || vflag ==888)
	{
		dynamic_buffer_underrun_031();
	}

	if (vflag == 32 || vflag ==888)
	{
		dynamic_buffer_underrun_032();
	}

	if (vflag == 33 || vflag ==888)
	{
		dynamic_buffer_underrun_033();
	}

	if (vflag == 34 || vflag ==888)
	{
		dynamic_buffer_underrun_034();
	}

	if (vflag == 35 || vflag ==888)
	{
		dynamic_buffer_underrun_035();
	}

	if (vflag == 36 || vflag ==888)
	{
		dynamic_buffer_underrun_036();
	}

	if (vflag == 37 || vflag ==888)
	{
		dynamic_buffer_underrun_037();
	}

	if (vflag == 38 || vflag ==888)
	{
		dynamic_buffer_underrun_038();
	}

	if (vflag == 39 || vflag ==888)
	{
		dynamic_buffer_underrun_039();
	}

}

