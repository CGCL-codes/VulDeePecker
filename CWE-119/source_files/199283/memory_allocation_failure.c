/********Software Analysis - FY2013*************/
/*
* File Name: memory_allocation_failure.c
* Defect Classification
* ---------------------
* Defect Type: Resource management defects
* Defect Sub-type: Memory allocation failure
* Description: memory_allocation_failure -  Memory could not be allocated / insufficient memory
*  Created on: Oct 14, 2013
*      Author: hemalatha
*/

/*
* MAX VALUE of INT & LONG 2147483647 , unsigned INT & LONG 4294967295
* MAX Heap memory in Windows 7 32 bit 2GB
*/
#include "HeaderFile.h"

#define MAX_VAL UINT_MAX

unsigned int memory_allocation_failure_005_gbl = 65536;

char * memory_allocation_failure_007_str_gbl;
typedef struct {
        int  csr;
        int  data;
}memory_allocation_failure_010_s_001;

memory_allocation_failure_010_s_001 *memory_allocation_failure_010_arr_gbl;

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to long in an infinite for loop
*/
void memory_allocation_failure_001 ()
{
	int i;
	long *buf;
	for (i=0;;i++)
	{
		buf=(long*) calloc(5,sizeof(long));
		if(buf !=NULL)
		{
		     buf[1]=1;/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
		}
	}
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: when using pointers to the type INT
*/
void memory_allocation_failure_002 ()
{
	long long int i=0;
	long long int *ptr=(long long*) malloc(MAX_VAL *sizeof(long long));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
	if(ptr !=NULL)
	{
	*(ptr+i) = i;
	free(ptr);
	}
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: when using double pointers to the type INT
*/
 #define MAX 10
void memory_allocation_failure_003 ()
{
	unsigned int **ptr = (unsigned int**) malloc(MAX*sizeof(unsigned int*));
	unsigned int i,j;
	if(ptr!=NULL)
	for(i=0;i<MAX;i++)
		ptr[i]=(unsigned int*) malloc(MAX_VAL*sizeof(unsigned int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */

	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			*(*(ptr+i)+j)=i;
		}
		free(ptr[i]);
	}
	free(ptr);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to unsigned int based on return value of function
*/
int memory_allocation_failure_004_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = MAX_VAL;
   else
	   ret=1;
   return ret;
}
void memory_allocation_failure_004 ()
{
	unsigned int *ptr = (unsigned int*) malloc(memory_allocation_failure_004_func_001(0)*sizeof(unsigned int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
	if(ptr !=NULL)
	{
		*(ptr+1) = 10;
		free(ptr);
	}
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a void pointer  int based on value of global variable inside a switch case statement
*/
void *vptr;
int memory_allocation_failure_005_func_001 (int flag)
{
	switch (flag)
	{
		case 1:
		{
			vptr = (int *)calloc(memory_allocation_failure_005_gbl*memory_allocation_failure_005_gbl, sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
			if(vptr != NULL)
			*((int*)vptr+1) = 10;
			return 1;
		}
		case 2:
		{
			vptr = (char *)calloc(10, sizeof(char));
			if(vptr != NULL)
			*((char*)vptr+2) = 'a';
			return 2;
		}
		case 3:
		{
			vptr = (float *)calloc(10, sizeof(float));
			if(vptr != NULL)
			*((float*)vptr+3) = 5.5;
			return 3;
		}
		default:
			return -1;
	}
}

void memory_allocation_failure_005 ()
{
	int ret;
	ret = memory_allocation_failure_005_func_001 (rand());
	if(ret >= 0)
		if(vptr != NULL)
		free(vptr);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a double pointer to char , memory allocated and returned from a function based on return value of function
*/

#define MAX_10 429496730
int **memory_allocation_failure_006_gbl_doubleptr;
int memory_allocation_failure_006_func_001(int flag)
{
   int ret;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void memory_allocation_failure_006_func_002()
{
	int i;

	if(memory_allocation_failure_006_func_001(0)==0)
	{
		memory_allocation_failure_006_gbl_doubleptr=(int**) malloc(10*sizeof(int*));
    	for(i=0;i<10;i++)
	    {
    		memory_allocation_failure_006_gbl_doubleptr[i]=(int*) malloc(MAX_10*sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
    		memory_allocation_failure_006_gbl_doubleptr[i][0] =10;
	    }
	}
}

void memory_allocation_failure_006()
{
	int flag=0,i;

	if(memory_allocation_failure_006_func_001(flag)==0)
	{
		memory_allocation_failure_006_func_002();
	}
    if(memory_allocation_failure_006_func_001(flag)==0)
	{
		for(i=0;i<10;i++)
		{
			if(memory_allocation_failure_006_gbl_doubleptr[i]!=NULL)
			{
     			free (memory_allocation_failure_006_gbl_doubleptr[i]);
			    memory_allocation_failure_006_gbl_doubleptr[i] = NULL;
			}
	    }
	    free(memory_allocation_failure_006_gbl_doubleptr);
	    memory_allocation_failure_006_gbl_doubleptr = NULL;
	}
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a global pointer to char in a for loop inside if condition
*/
static unsigned int static_var = MAX_VAL*2;
static char * memory_allocation_failure_007_func_001 (char *str1)
{
    int j;
    if (str1 != NULL) {
      memory_allocation_failure_007_str_gbl = (char *) malloc(static_var+1);/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
      if(memory_allocation_failure_007_str_gbl!=NULL) {
        for (j = 0; j < static_var; j++) {
          memory_allocation_failure_007_str_gbl[j] = str1[static_var-j-1];
        }
        memory_allocation_failure_007_str_gbl[static_var] = '\0';
      }
      return memory_allocation_failure_007_str_gbl;
    } else {
      return NULL;
    }
}

void memory_allocation_failure_007 ()
{
     int j;
    char buf[][25]={"This is a String",
    		     "Second String"};
    for(j = 0; j <= 1; j++)
    {
        if(buf[j]!= NULL)
    	{
        	memory_allocation_failure_007_str_gbl = memory_allocation_failure_007_func_001(buf[j]);
        }
    }
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to char in an infinite for loop
*/
enum {max_buffer = MAX_VAL*2};
char * memory_allocation_failure_008_func_001 (const char *msg) {
  const char *error_log = msg;
  char * buffer = 0;
  int i;
  for(i=0;i<max_buffer;i++)
  {
	   buffer = (char*) malloc(max_buffer * sizeof(char));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
       break;
  }
  if(buffer!=NULL)
    snprintf(buffer, max_buffer * sizeof(char), "Error: %s", error_log);
  return buffer;
}
void memory_allocation_failure_008 ()
{
	char *str = "STRINGMEM";
	memory_allocation_failure_008_func_001(str);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to char in a do ~ while loop and memory allocate and returned to another function ,
*/
#define MAX_BUFFER (429496728UL)

void memory_allocation_failure_009_func_001 (char * buf)
{
    int i=0;
    do
    {
    	buf = (char*) malloc(MAX_BUFFER * sizeof(char));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
    	i++;
    }while (i<MAX_VAL);
}

void memory_allocation_failure_009_func_002 (void * vptr)
{
	/* cast void pointer to a pointer of the appropriate type */
    char * * cptr = (char * *)vptr;
    char * buf =NULL;
    memory_allocation_failure_009_func_001 (buf);
    buf = (*cptr);
    printf("%s",buf);
}

void memory_allocation_failure_009 ()
{
    void *buf1;
	buf1 = "String Test123";
    memory_allocation_failure_009_func_002(&buf1);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to volatile structure in a do while loop and bit manipulations and global variable
*/
#define ERROR   0x1
#define READY   0x2
#define RESET   0x4

#define NDEVS   4

void memory_allocation_failure_010_func_001(int devno)
{
	int i=0;
	if (devno <= NDEVS)
	{
	    do
	    {
			memory_allocation_failure_010_arr_gbl = (memory_allocation_failure_010_s_001 *) malloc (10*sizeof(memory_allocation_failure_010_s_001));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
            i++;
	    }while( i<=MAX_VAL);
	}
}

void memory_allocation_failure_010_func_002(int devno)
{
    memory_allocation_failure_010_func_001(1);
	memory_allocation_failure_010_arr_gbl->csr = READY;
    do
    {
        memory_allocation_failure_010_arr_gbl->data = READY;
        if(memory_allocation_failure_010_arr_gbl->csr & ERROR)
        {
    	    memory_allocation_failure_010_arr_gbl->csr = RESET;
        }
    }while( (memory_allocation_failure_010_arr_gbl->csr & (READY | ERROR)) == 0);
}

void memory_allocation_failure_010 ()
{
	memory_allocation_failure_010_func_002(1);
	++memory_allocation_failure_010_arr_gbl->data;
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to union , with structure as element , memory allocated from other function
*/
typedef struct {
	int *a;
	int b;
} memory_allocation_failure_011_s_001;

typedef struct {
	int a;
	int b;
} memory_allocation_failure_011_s_002;

typedef struct {
	int a;
	int b;
} memory_allocation_failure_011_s_003;

typedef union {
	memory_allocation_failure_011_s_001 *s1;
	memory_allocation_failure_011_s_002 *s2;
	memory_allocation_failure_011_s_003 *s3;
} memory_allocation_failure_011_uni_001;


int memory_allocation_failure_011_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = MAX_VAL;
   else
	   ret=5;
   return ret;
}
memory_allocation_failure_011_uni_001 * memory_allocation_failure_011_gbl_u1;
void memory_allocation_failure_011_func_002(void)
{
	memory_allocation_failure_011_gbl_u1 = (memory_allocation_failure_011_uni_001 * )malloc(memory_allocation_failure_011_func_001(0)*sizeof( memory_allocation_failure_011_uni_001 )); /*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
	memory_allocation_failure_011_gbl_u1->s1 = (memory_allocation_failure_011_s_001 *) malloc(sizeof(memory_allocation_failure_011_s_001));
	memory_allocation_failure_011_gbl_u1->s1->a = (int *) calloc ( 5,sizeof(int));
}
void memory_allocation_failure_011 ()
{
	memory_allocation_failure_011_uni_001 *p =NULL;
	memory_allocation_failure_011_func_002();

	if( memory_allocation_failure_011_gbl_u1 != NULL)
	{
	p = memory_allocation_failure_011_gbl_u1;
	p->s1->a[1] = 10;

	free(memory_allocation_failure_011_gbl_u1->s1->a);
	free(memory_allocation_failure_011_gbl_u1->s1);
	free(memory_allocation_failure_011_gbl_u1);
	}
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer array of int , memory allocated in another function based on return value of a function in a ternary statement
*/
int *memory_allocation_failure_012_buf2_gbl;
int memory_allocation_failure_012_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = MAX_VAL;
   else
	   ret=5;
   return ret;
}

void memory_allocation_failure_012_func_002 (int *pbuf[])
{
	int buf1[5] = {1,2,3,4,5};
	memory_allocation_failure_012_buf2_gbl = (int *) calloc (memory_allocation_failure_012_func_001(0),sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
	int buf3[5] = {1,2,3,4,5};
	int buf4[5] = {1,2,3,4,5};
	int buf5[5] = {1,2,3,4,5};
	pbuf[0] = buf1;
	pbuf[1] = memory_allocation_failure_012_buf2_gbl;
	pbuf[2] = buf3;
	pbuf[3] = buf4;
	pbuf[4] = buf5;
	int ret;
	ret = pbuf[1][1];
	printf("%d",ret);
}

void memory_allocation_failure_012 ()
{
	int *ptr[5], a;
	int flag=10;

    (flag == 10)? (memory_allocation_failure_012_func_002(ptr)) : ( a =20);
    (flag == 10)? (ptr[1][1] = 200):(a=100);

    if(flag == 10){
    	if(memory_allocation_failure_012_buf2_gbl!=NULL)
    	{
    		a = ptr[1][1];
    	    free(memory_allocation_failure_012_buf2_gbl);
    	}
    }
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a double pointer to char in an if condition and memory allocated based on return value of function
*/
static int staticflag=1;
int memory_allocation_failure_013_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = MAX_VAL;
   else
	   ret=5;
   return ret;
}

void memory_allocation_failure_013 ()
{
	char  **dptr,a;
	double *ptr,b = 0.0;
	int i;


    if (staticflag == 10)
    {
    	  	ptr= (double*) malloc(10*sizeof(double));
    }
    else
    {
    	  	dptr=(char**) malloc(10*sizeof(char*));
    	  	if(1)
    	  	{
            	for(i=0;i<10;i++)
        	    {
            		dptr[i]=(char*) malloc(memory_allocation_failure_013_func_001(0)*sizeof(char));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
        	    }
    	  	}
    }

    if  (staticflag == 10)
    		(*(ptr+1) = 10.5);
    else
    	   strcpy( dptr[1],"STRING TEST" );

    if(staticflag == 10)
    	b = *(ptr+1);
    else
    	a = dptr[1][1];

    if(staticflag == 10)
    {
    	free(ptr);
    	ptr = NULL;
    }
    else
    {
    	for(i=0;i<10;i++)
    		free(dptr[i]);
    	free(dptr);
    	dptr = NULL;
    }
    printf("%d",a);
    sink = b;
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to an array of integer in if condition and
* memory allocated and return from another function based on value of array
*/
const int arr_value[2] = {1,MAX_VAL};
int (*memory_allocation_failure_014_func_001())[4]
{
	int (*p)[4];
	int arr[4][4] = {{1,2,3,4},
			         {11,22,33,44},
			         {33,44,55,66},
			         {55,66,77,88}};
	int i,j;
	p= (int (*)[]) malloc (arr_value[1] *1 );/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
	if(p!=NULL)
	{
	memcpy(p, arr, sizeof(arr));
	for (i = 0;i< 4; i++)
	{
		for ( j=0 ;j<4; j++)
		{
			*(p[i]+j) += *(p[i]+j);
		}
	}
	}
	return p;
}

void memory_allocation_failure_014 ()
{
	int (*ptr1)[4];
	char  **dptr,a = 0;
	double *ptr,b;
	int i,j;
	static int staticflag=10;
    if (staticflag == 10)
    {
    	  	ptr= (double*) malloc(10*sizeof(double));
    }
    else
    {
    	  	dptr=(char**) malloc(10*sizeof(char*));
    	  	if(1)
    	  	{
            	for(i=0;i<10;i++)
        	    {
            		dptr[i]=(char*) malloc(10*sizeof(char));
        	    }
            	ptr1 = memory_allocation_failure_014_func_001();
    	  	}
    }

    if  (staticflag == 10)
    {
    		(*(ptr+1) = 10.5);
    }
    else
    {
    	   strcpy( dptr[1],"STRING TEST" );
    	   if(1)
    	   {
        	   for (i = 0;i< 4; i++)
               	{
               		for ( j=0 ;j<4; j++)
               		{
               			*(ptr1[i]+j) += *(ptr1[i]+j);
               		}
               	}
    	   }


    }

    if(staticflag == 10)
    {
    	b = *(ptr+1);
    }
    else
    {
	  	if(1)
	  	{
	  		a = *(ptr1[1]+1);
	  	}
    }

    if(staticflag == 10)
    {
    	free(ptr);
    	ptr = NULL;
    }
    else
    {
    	for(i=0;i<10;i++)
    		free(dptr[i]);
    	free(dptr);
    	if(1)
    	{
    		free(ptr1);
    	}
    }
    printf("%d %lf",a,b);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to int in a goto statement  based on return value function , structure element
*/
#define MAX_V 4294967295UL
typedef struct {
	int a;
	int b;
} memory_allocation_failure_015_s_001;

int * memory_allocation_failure_015_gbl_ptr;

int memory_allocation_failure_015_func_001 ()
{
	memory_allocation_failure_015_s_001 s = {MAX_V,20};
	return s.a;
}

void memory_allocation_failure_015_func_002 ()
{
	memory_allocation_failure_015_gbl_ptr = (int *) malloc (memory_allocation_failure_015_func_001()*sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
}

int memory_allocation_failure_015_func_003 (int flag)
{
	int ret = 0;
	if (flag == 1)
	{
		goto my_label;
	}
	return ret;
my_label:
    if (flag == 1)
	{
        memory_allocation_failure_015_func_002();
	}
	*(memory_allocation_failure_015_gbl_ptr+1) = 7;
	ret ++;
	return ret;
}

void memory_allocation_failure_015 ()
{
    int flag;
    flag = memory_allocation_failure_015_func_003(1);
    printf("%d",flag);
}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity: When using a pointer to int 4 pointers
*/
int * memory_allocation_failure_016_gbl_ptr1;
int * memory_allocation_failure_016_gbl_ptr2;

#define MAX_VAL_4 1073741824
int memory_allocation_failure_016_func_001(int flag)
{
   int ret =0;
   if (flag ==0)
	   ret = MAX_VAL_4;
   else
	   ret=5;
   return ret;
}

void memory_allocation_failure_016_func_002(int flag)
{

   if (flag ==0)
   {
	   ;
   }
   else
   {
	   memory_allocation_failure_016_gbl_ptr1 = (int *) malloc (memory_allocation_failure_016_func_001(0)*sizeof(int));
	   memory_allocation_failure_016_gbl_ptr2 = (int *) malloc (memory_allocation_failure_016_func_001(0)*sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory allocation failure */
   }

}

void memory_allocation_failure_016 ()
{
	int * ptr1 = (int *) malloc (memory_allocation_failure_016_func_001(0)*sizeof(int));
	int * ptr2 = (int *) malloc (memory_allocation_failure_016_func_001(0)*sizeof(int));
    *(ptr1+1) = 10;
    memory_allocation_failure_016_func_002(0);

    free(memory_allocation_failure_016_gbl_ptr1);
    free(memory_allocation_failure_016_gbl_ptr2);
    free(ptr1);
    free(ptr2);

}

/*
* Type of defect: memory_allocation_failure - Memory could not be allocated / insufficient memory
* Complexity:Memory allocation failure main function
*/
extern volatile int vflag;
void memory_allocation_failure_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		memory_allocation_failure_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		memory_allocation_failure_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		memory_allocation_failure_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		memory_allocation_failure_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		memory_allocation_failure_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		memory_allocation_failure_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		memory_allocation_failure_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		memory_allocation_failure_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		memory_allocation_failure_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		memory_allocation_failure_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		memory_allocation_failure_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		memory_allocation_failure_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		memory_allocation_failure_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		memory_allocation_failure_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		memory_allocation_failure_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		memory_allocation_failure_016();
	}
}

