/********Software Analysis - FY2013*************/
/*
* File Name: invalid_memory_access.c
* Defect Classification
* ---------------------
* Defect Type: Resource management defects
* Defect Sub-type: Invalid memory access to already freed area
* Description: Defect Free Code to identify false positives during invalid memory access
*/

/*
 * invalid_memory_access.c
 *
 *  Created on: Sep 20, 2013
 *      Author: hemalatha
 */

#include "HeaderFile.h"

char ** invalid_memory_access_016_doubleptr_gbl;
char *invalid_memory_access_017_doubleptr_gbl;

static int staticflag1=1;
/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity : Memory is allocated and accessed after free -  when using pointer to int and conditional operator
*/
void invalid_memory_access_001 ()
{
	int *ptr, a;
	int flag=10;

    (flag == 10)? (ptr= (int*) malloc(10*sizeof(int))) : ( ptr= NULL);

    if(ptr!=NULL)
    {
    	(flag == 10)? (*(ptr+1) = 10):(a=100);
        if(flag == 10)
        {
        	a = *(ptr+1); /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
        	printf("%d",a);
       }
        if(flag == 10)
        {
        	free(ptr);
    	    ptr = NULL;
         }
     }
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
*  Complexity : Memory is allocated and accessed after free -
*  when using pointer to double,conditional statement if ~else using static variable of function scope
*/

void invalid_memory_access_002 ()
{
	double *ptr, *dptr = 0,a;
	static int staticflag=10;
    if (staticflag == 10)
    	  	(ptr= (double*) malloc(10*sizeof(double)));
    else
    	  	(dptr = (double*) malloc(5*sizeof(double)));

    if  (staticflag == 10 && ptr!=NULL)
    		(*(ptr+1) = 10.5);
    else
    	    (*(dptr+1) = 5.5) ;

    if(staticflag == 10 && ptr!=NULL)
    	a = *(ptr+1); /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
    else
    	a = *(dptr+1);

	printf("%lf",a);

    if(staticflag == 10 && ptr!=NULL)
    {
    	free(ptr);
    	ptr = NULL;
    }
    else
    {
    	free(dptr);
    	dptr = NULL;
    }
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: Memory is allocated and accessed after free -
* When using a pointer to char in an infinite while loop and Strcpy
*/
void invalid_memory_access_003 ()
{
	int i=1;
	char *buf,*c = 0;
	while(i>0)
	{
		buf = (char *) malloc (25 * sizeof(char));
		if(buf!=NULL)
		{
			strcpy(buf,"This is String");
	        c = buf;
	        free(buf);
    	    buf = NULL; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
		}
	    i++;
	    if(i>=10)
	    	break;
	}
        psink = c;
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: Memory is allocated and accessed after free -
* When using a pointer to char and initialize memory block with memset and if condition using static variable of file scope
*/
void invalid_memory_access_004 ()
{
    char * buf = NULL;
    if(staticflag1)
    {
        buf = (char *)malloc(100*sizeof(char));
        if(buf !=NULL)
        {
        	memset(buf, 'A', 100-1);
            buf[100-1] = '\0'; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
            free(buf);
            buf = NULL;
        }
    }
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using a pointer to int in a recursive function
*/
int invalid_memory_access_005 (int count)
{
	int *ptr ;

	if (count ==0)
	{
    	//count = *ptr;
		return 1;
	}
    else
    {
    	ptr = (int *) calloc (count,sizeof(int));
    	if(ptr!=NULL)
    	{
    		*(ptr+(count-1)) = 5*count; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
    	    free(ptr);
    	    ptr = NULL;
    	}
    	return (count * invalid_memory_access_005(count-1));
    }
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using double pointers to the type float
*/
void invalid_memory_access_006 ()
{
	float *buf1=(float*)calloc(5,sizeof(float));
	float *buf2=(float*)calloc(5,sizeof(float));
	float *buf3=(float*)calloc(5,sizeof(float));
	float *buf4=(float*)calloc(5,sizeof(float));
	float *buf5=(float*)calloc(5,sizeof(float));
	float **pbuf[5] = {&buf1, &buf2, &buf3, &buf4, &buf5};
	int i,j=4;
	if(buf2 != NULL)
	{
	buf2[0] = 10.0;
	}
	for(i=0;i<5;i++)
	{
		*((*pbuf[i])+j)=5.0;
	}
	if(buf2 != NULL )
	*((*pbuf[1])+1) =buf2[0]; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	if(buf1 != NULL)
	free(buf1);
	if(buf2 != NULL)
	free(buf2);
	if(buf3 != NULL)
	free(buf3);
	if(buf4 != NULL)
	free(buf4);
	if(buf5 != NULL)
	free(buf5);
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using double pointers to the type char 2D array of size [5][15] ,strcpy
*/
void invalid_memory_access_007 ()
{
	char **ptr = (char**) malloc(5*sizeof(char*));
	int i;

	for(i=0;i<5;i++)
		ptr[i]=(char*) malloc(15*sizeof(char));

	for(i=0;i<5;i++)
	{
		strcpy(*(ptr+i),"String"); /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	    free(ptr[i]);
	    ptr[i] = NULL;
	}
	free(ptr);
	ptr = NULL;
	/* strcpy(*(ptr+2),"String"); */
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using 1 single char pointer  memcpy function
*/
void invalid_memory_access_008 ()
{
	char* buf=(char*) calloc(25, sizeof(char));
	char* buf1= "This is a string";
	if(buf != NULL)
	{
		memcpy(buf,buf1,11); /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	    free(buf);
	    buf = NULL;
	}
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using a Union with structure members - Aliasing
*/
typedef union {
	int *a;
	int b;
} invalid_memory_access_009_s_001;

typedef union {
	int a;
	int b;
} invalid_memory_access_009_s_002;

typedef union {
	int a;
	int b;
} invalid_memory_access_009_s_003;

typedef struct {
	invalid_memory_access_009_s_001 *s1;
	invalid_memory_access_009_s_002 s2;
	invalid_memory_access_009_s_003 s3;
} invalid_memory_access_009_uni_001;

void invalid_memory_access_009 ()
{
	invalid_memory_access_009_uni_001 *u = (invalid_memory_access_009_uni_001 * )malloc(5*sizeof( invalid_memory_access_009_uni_001 ));
	invalid_memory_access_009_uni_001 *p  = NULL;

    if(u!=NULL)
    {
	u->s1 = (invalid_memory_access_009_s_001 *) malloc(sizeof(invalid_memory_access_009_s_001));
	if(u->s1!=NULL)
	u->s1->a = (int *) malloc(5*sizeof(int));

	p = u;
	p->s1->a[0] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/

	free(u->s1->a);
	free(u->s1);
	free(u);
    }
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity Memory allocated in a function and Memory used in another function
*/
void invalid_memory_access_func_010 (int len ,int **Ptr)
{
	int * p = malloc(sizeof(int) * len);
	*Ptr = p;
}

void invalid_memory_access_010 ()
{
	int ptr[5] = {4,6,9,10,0};
	int *ptr1,i;
	invalid_memory_access_func_010(5,&ptr1);
	for(i=0;i<5;i++)
	{
		*(ptr1+i) = ptr[i]; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	}
	free(ptr1);
}

/*
* Types of defects: Invalid memory access
* Complexity: 1-dimensional array
*/
void invalid_memory_access_011 ()
{
	int *ptr,i=0,j;
	int index[4] = {3, 5, 4, 6};
	for(j=0;;j++)
	{
		ptr = (int *)malloc(sizeof(int) * 4);
		if(ptr!=NULL)
		{
		   while(i<4)
		   {
			   ptr[i] = index[i]; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	           i++;
	       }
	       free(ptr);
		}
	    if(j>10)
	    break;
	}
	/* *(ptr+i) = i; */
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using switch case
*/
typedef struct {
	int a;
	int b;
	int uninit;
} invalid_memory_access_012_s_001;

int invalid_memory_access_012_func_001 (int flag)
{

	invalid_memory_access_012_s_001 *s;
	s = (invalid_memory_access_012_s_001 *)calloc(1,sizeof(invalid_memory_access_012_s_001));
	int i=0;
	switch (flag)
	{
		case 1:
		{
			if(s!=NULL)
			{
			     s->a = 10;
			     s->b = 10;
			     s->uninit = 10; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
			     i=s->a;
     			 free(s);
			}
			break;
		}
		case 2:
		{
			if(s!=NULL)
			{
			     s->a = 20;
			     s->b = 20;
			     s->uninit = 20;
			     i=s->a;
     			 free(s);
			}
			break;
		}
		default:
		{
			free(s);
			break;
		}
	}
	/*return s->a;*/
	return i;
}

void invalid_memory_access_012 ()
{
	int ret;
	ret = invalid_memory_access_012_func_001 (1);
	printf("%d",ret);
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: Double pointer - double	Alias for 1 weight	Address	Constant	Write
*/
typedef struct {
	int a;
	int b;
	int uninit;
} invalid_memory_access_013_s_001;
invalid_memory_access_013_s_001 *invalid_memory_access_013_s_001_s_gbl;

void invalid_memory_access_013_func_001 (int flag)
{
	if(flag >0)
	invalid_memory_access_013_s_001_s_gbl = (invalid_memory_access_013_s_001 *)calloc(1,sizeof(invalid_memory_access_013_s_001));
}

int invalid_memory_access_013_func_002 (int flag)
{
	int i=0;
	switch (flag)
	{
		case 1:
		{
			if (invalid_memory_access_013_s_001_s_gbl != NULL)
			{
				invalid_memory_access_013_s_001_s_gbl->a = 10; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
				invalid_memory_access_013_s_001_s_gbl->b = 10;
				invalid_memory_access_013_s_001_s_gbl->uninit = 10;
				i=invalid_memory_access_013_s_001_s_gbl->a;
				free(invalid_memory_access_013_s_001_s_gbl);
			}
			break;
		}
		case 2:
		{
			if (invalid_memory_access_013_s_001_s_gbl != NULL)
			{
				invalid_memory_access_013_s_001_s_gbl->a = 20;
				invalid_memory_access_013_s_001_s_gbl->b = 20;
				invalid_memory_access_013_s_001_s_gbl->uninit = 20;
				i=invalid_memory_access_013_s_001_s_gbl->a;
				free(invalid_memory_access_013_s_001_s_gbl);
			}
			break;
		}
		default:
		{
			break;
		}
	}
	/*return invalid_memory_access_013_s_001_s_gbl->a;*/
	return i;
}

void invalid_memory_access_013_func_003 (int flag)
{
	invalid_memory_access_013_s_001 s;
	if(flag >0)
	{
		if (invalid_memory_access_013_s_001_s_gbl != NULL)
		{
			s.a = invalid_memory_access_013_s_001_s_gbl->a;
			s.b = invalid_memory_access_013_s_001_s_gbl->b;
			s.uninit = invalid_memory_access_013_s_001_s_gbl->uninit;
		}
	}
}

void invalid_memory_access_013 ()
{
	int ret;
	invalid_memory_access_013_func_001 (1);
	invalid_memory_access_013_func_003 (1);
	ret = invalid_memory_access_013_func_002 (1);
	
	printf("%d",ret);
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: When using a pointer to int and  goto
*/
int invalid_memory_access_014 (int flag)
{
	int ret = 0,i;
	int arr[]={3,8,9,10,4};
	int *ptr = malloc(sizeof(int)*5);
	if (flag == 1)
	{
		goto my_label;
	}
	if(ptr!=NULL)
    ret = ptr[2]; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	goto my_label2;
my_label:
	    {
            for(i=0;i<5;i++)
            {
            	if(ptr!=NULL)
            		ptr[i] = arr[i];
            }
	    }
my_label2:
    free(ptr);
	return ret;
}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity: Use a block of memory returned from a function after it has been freed
*/

static char * invalid_memory_access_015_func_001 (char *str1)
{
    int i = 0;
    int j;
    char * str_rev = NULL;
    if (str1 != NULL)
    {
        i = strlen(str1);
        str_rev = (char *) malloc(i+1);
        if (str_rev != NULL)
        {
        	for (j = 0; j < i; j++)
            {
                str_rev[j] = str1[i-j-1]; /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
            }
            str_rev[i] = '\0';
        }
        /*free(str_rev) ;
        str_rev = NULL;*/
        return str_rev;
    }
    else
    {
        return NULL;
    }
}
void invalid_memory_access_015 ()
{
    int j;
    char buf[][25]={"This is a String",
    		     "Second String"};
    for(j = 0; j <= 1; j++)
    {
        {
            char * str;
            str = invalid_memory_access_015_func_001(buf[j]);
            psink = str;
        }
    }
}

/*
* Types of defects: Uninitialized pointer
* Complexity: global char double pointer  variable initialized in another function
 */
int invalid_memory_access_016_func_001(int flag)
{
   int ret;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void invalid_memory_access_016_func_002()
{
	int i;
	if(invalid_memory_access_016_func_001(0)==0)
	{
		invalid_memory_access_016_doubleptr_gbl=(char**) malloc(10*sizeof(char*));
    	for(i=0;i<10;i++)
	    {
    		invalid_memory_access_016_doubleptr_gbl[i]=(char*) malloc(10*sizeof(char));
    		   strcpy(invalid_memory_access_016_doubleptr_gbl[i],"STRING00");
	    }
	}
}

void invalid_memory_access_016_func_003()
{
	char s[10] ;
	printf("invalid gbl= %s \n",invalid_memory_access_016_doubleptr_gbl[0]);
	strcpy(s,invalid_memory_access_016_doubleptr_gbl[0]);/*Tool should not detect this line as error*/ /* No ERROR:Invalid memory access to already freed area*/
	printf("invalid str= %s \n",s);
}

void invalid_memory_access_016()
{
	int flag=0,i;
	if(invalid_memory_access_016_func_001(flag)==0)
	{
		invalid_memory_access_016_func_002();
	}
	
	if(invalid_memory_access_016_func_001(flag)==0)
	{
		invalid_memory_access_016_func_003();
	}
	
	if(invalid_memory_access_016_doubleptr_gbl!=NULL)
	{
		for(i=0;i<10;i++)
		{
			if(invalid_memory_access_016_doubleptr_gbl[i] !=NULL)
			free (invalid_memory_access_016_doubleptr_gbl[i]);
	    }
	    free(invalid_memory_access_016_doubleptr_gbl);
	}

}
/*
* Types of defects: Uninitialized pointer
* Complexity: global char pointer  variable initialized in another function , freed in another function based in while loop
 */
int invalid_memory_access_017_func_001(int flag)
{
   int ret;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void invalid_memory_access_017_func_002()
{
	invalid_memory_access_017_doubleptr_gbl=(char*) malloc(10*sizeof(char));
	if(invalid_memory_access_017_doubleptr_gbl !=NULL)
	strcpy(invalid_memory_access_017_doubleptr_gbl,"TEST");
}

void invalid_memory_access_017_func_003()
{
	free(invalid_memory_access_017_doubleptr_gbl);
}

void invalid_memory_access_017_func_004()
{
	char s[10] ;
	printf("invalid gbl= %s \n",invalid_memory_access_017_doubleptr_gbl);
	strcpy(s,invalid_memory_access_017_doubleptr_gbl); /*Tool should not detect this line as error*/ /*No ERROR:Invalid memory access to already freed area*/
	printf("invalid str= %s \n",s);
}


void invalid_memory_access_017()
{
	int flag=10;
	if(invalid_memory_access_017_func_001(flag) == 0)
	{
		;
	}
	else
	{
		invalid_memory_access_017_func_002();
	}
	if(invalid_memory_access_017_func_001(flag) == 0)
	{
		;
	}
	else
	{
		invalid_memory_access_017_func_004();
	}
	if(invalid_memory_access_017_func_001(flag) == 0)
	{
		;
	}
	else
	{
		invalid_memory_access_017_func_003();
	}


}

/*
* Types of defects: Invalid memory access to already freed area(Dangling pointers)
* Complexity:Invalid memory access main function
*/
extern volatile int vflag;
void invalid_memory_access_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		invalid_memory_access_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		invalid_memory_access_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		invalid_memory_access_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		invalid_memory_access_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		invalid_memory_access_005 (5);
	}

	if (vflag == 6 || vflag ==888)
	{
		invalid_memory_access_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		invalid_memory_access_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		invalid_memory_access_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		invalid_memory_access_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		invalid_memory_access_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		invalid_memory_access_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		invalid_memory_access_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		invalid_memory_access_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		int i;
		i = invalid_memory_access_014 (1);
                sink = i;
	}

	if (vflag == 15 || vflag ==888)
	{
		invalid_memory_access_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		invalid_memory_access_016();
	}

	if (vflag == 17 || vflag ==888)
	{
		invalid_memory_access_017();
	}
}
