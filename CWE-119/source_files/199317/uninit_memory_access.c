/********Software Analysis - FY2013*************/
/*
* File Name: uninit_memory_access.c
* Defect Classification
* ---------------------
* Defect Type: Resource management defects
* Defect Sub-type: Uninitialized memory access
* Description: Defect Code to identify access to memory locations which are uninitialized
*  Created on: Sep 26, 2013
*      Author: hemalatha
*/

#include "HeaderFile.h"

long ** uninit_memory_access_009_doubleptr_gbl;

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using unsigned long variable - stack access
*/
void uninit_memory_access_001 ()
{
	unsigned long a;
	unsigned long *ret;
	ret = &a;
    printf("%ld ",*ret);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using unsigned long variable - Heap access
*/
void uninit_memory_access_002 ()
{
	double *a = (double *) calloc(3,sizeof(double));
	if(a!=NULL)
	{
	printf("%lf ",a[1]);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
	    free(a);
	}
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a char pointer and strcpy
*/
void uninit_memory_access_003 ()
{
	char *str1 = (char *) calloc(25,sizeof(char));
	char *str2 ;
	if (str1!=NULL)
	{
		strcpy(str1, str2);
        printf("%s %s\n",str1,str2);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
        free(str1);
	}
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using double short pointer and aliasing
*/
void uninit_memory_access_004 ()
{
	short **ptr = NULL;
	short *p1 = (short *) calloc(10,sizeof(short));
	short *p2 = NULL;
	if(p1 !=NULL)
	{
     p2 = p1;
	}
     ptr = &p2;
     printf("%d \n",**ptr);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
     free(p1);
}

/*
* Types of defects: Uninitialized Memory Access
 Complexity: Float 3D array partially initialized from another 3D array
*/
void uninit_memory_access_005 ()
{
    int i,j,k;
	int arr[2][3][4] = { { {1, 2, 3, 4}, {12, 22, 32, 42}, {13, 23, 33, 43} },
                          { {21, 22, 23, 24}, {31, 32, 33, 34}, {41, 42, 43, 44} } };
	int arr1[2][3][4];
	for(i=0;i<1;i++)
		{
			for(j=0;j<1;j++)
			{
				for(k=0;k<1;k++)
				{
                    arr1[i][j][k] = arr[i][j][k];
					}
			}
		}
	k = arr1[1][2][3];/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to char , and initialized in another function
*/
void uninit_memory_access_006_func_001(char *dst, const char *src)
{
     if(src == NULL)
     {
         printf("Null Assignment:\n");
         return;
     }
     else
     {
    	 while(*src != '\0')
        {
    		 *dst = *src;
             src++;
             dst++;
        }
        *dst = '\0';
     }
 }
void uninit_memory_access_006 ()
{
    char *str1 = (char *) calloc(25,sizeof(char));
    char *str2 ;
    uninit_memory_access_006_func_001(str1, str2);
    printf("%s\n", str1);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: Structure passed as function parameter and initialized partially in another function using mem set
*/
#define MAX 10
typedef struct {
    int arr[MAX];
    int a;
    int b;
    int c;
} uninit_memory_access_007_s_001;

void uninit_memory_access_007_func_001(uninit_memory_access_007_s_001* st)
{
    /*memset(st, 0, sizeof(*st));*/
     memset(st->arr, 0, 1);
    st->a = 1;
}
void uninit_memory_access_007_func_002(uninit_memory_access_007_s_001 *st)
{
    int temp=0;
    int i;
    for (i = 0; i < MAX; i++)
    {
        temp += st->arr[i];/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
    }
}

void uninit_memory_access_007_func_003(uninit_memory_access_007_s_001 *st)
{
    st->b = 10;
	st->c =20;
}

void uninit_memory_access_007_func_004(int num)
{
    int temp;
    if(num != 0) {
        temp = num;
    }
}
void uninit_memory_access_007 ()
{
	uninit_memory_access_007_s_001 st;
	uninit_memory_access_007_func_001(&st);
	uninit_memory_access_007_func_002(&st);
	uninit_memory_access_007_func_003(&st);
	uninit_memory_access_007_func_004(st.a);
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to ( signed) bit field variable ,initialized by return value of function
*/
typedef struct {
	signed int a : 7;
	signed int b : 7;
} uninit_memory_access_008_s_001;

uninit_memory_access_008_s_001 * uninit_memory_access_008_func_001()
{
	uninit_memory_access_008_s_001 *ret = (uninit_memory_access_008_s_001 *) malloc (5*sizeof(uninit_memory_access_008_s_001));
	return ret;
}

void uninit_memory_access_008 ()
{
	uninit_memory_access_008_s_001 *s = NULL;
	s = uninit_memory_access_008_func_001();
	s->b = s->a;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}

/*
* Types of defects: Uninitialized Memory Access
 *  When using array of double pointer to long -  Memory is  allocated based on return value of function ,initialized and accessed
 */
int uninit_memory_access_009_func_001(int flag)
{
   int ret;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void uninit_memory_access_009_func_002()
{
	int i,j;
	uninit_memory_access_009_doubleptr_gbl=(long**) malloc(10*sizeof(long*));

	for(i=0;i<10;i++)
	{
		uninit_memory_access_009_doubleptr_gbl[i]=(long*) malloc(10*sizeof(long));
	}
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			;/*uninit_memory_access_009_doubleptr_gbl[i][j]=i;*/
		}
	}
}

void uninit_memory_access_009()
{
	int flag=0,i,j;

	if(uninit_memory_access_009_func_001(flag)==0)
	{
		uninit_memory_access_009_func_002();
	}
    if(uninit_memory_access_009_func_001(flag)==0)
	{
		for(i=0;i<10;i++)
		{
			for(j=0;j<10;j++)
			{
				uninit_memory_access_009_doubleptr_gbl[i][j] += 1;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
			}
			free (uninit_memory_access_009_doubleptr_gbl[i]);
			uninit_memory_access_009_doubleptr_gbl[i] = NULL;
	   }
	    free(uninit_memory_access_009_doubleptr_gbl);
	    uninit_memory_access_009_doubleptr_gbl = NULL;
	}
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to volatile structure in a do while loop and bit manipulations
*/
typedef struct {
        unsigned short  csr;
        unsigned short  data;
}uninit_memory_access_010_s_001;

#define ERROR   0x1
#define READY   0x2
#define RESET   0x4

#define NDEVS   4

volatile uninit_memory_access_010_s_001  *uninit_memory_access_010_s_001_arr_gbl;
void uninit_memory_access_010_func_001(unsigned devno)
{
	uninit_memory_access_010_s_001_arr_gbl = (uninit_memory_access_010_s_001 *) malloc (5*sizeof(uninit_memory_access_010_s_001));
    if(uninit_memory_access_010_s_001_arr_gbl!=NULL)
    {
	uninit_memory_access_010_s_001_arr_gbl->csr = READY;
    if(devno < NDEVS)
    do
    {
    	uninit_memory_access_010_s_001_arr_gbl->data = READY;
        if(uninit_memory_access_010_s_001_arr_gbl->csr & ERROR)
        {
        	uninit_memory_access_010_s_001_arr_gbl->csr = RESET;
        }
    }while( (uninit_memory_access_010_s_001_arr_gbl->csr & (READY | ERROR)) == 0);
    }
}

void uninit_memory_access_010 ()
{
	uninit_memory_access_010_func_001(5);
    if(uninit_memory_access_010_s_001_arr_gbl!=NULL)
    {
    	++uninit_memory_access_010_s_001_arr_gbl->data;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
	    free((void *)uninit_memory_access_010_s_001_arr_gbl);
    }
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to constant  char and snprintf
*/
enum {max_buffer = 24};

void uninit_memory_access_011_func_001 (const char *msg) {
  const char *error_log = msg;
  char buffer[max_buffer];

  snprintf(buffer, sizeof(buffer), "Error: %s", error_log);
  printf(" test %s\n", buffer);
}
void uninit_memory_access_011 ()
{
	char *str ;
	uninit_memory_access_011_func_001(str);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using structure padding and memcpy
*/
typedef struct {
int int_a;
short short_b;
short pad;
}uninit_memory_access_012_s_001;

void uninit_memory_access_012()
{
	uninit_memory_access_012_s_001  *s1, s;
    s1 = (uninit_memory_access_012_s_001*)calloc(1,sizeof(uninit_memory_access_012_s_001));
    if(s1 !=NULL)
    {
    s1->int_a = 10;
    memcpy(&s, s1, sizeof(uninit_memory_access_012_s_001));/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
    free(s1);
    }
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using structure padding, enumeration and memcpy
*/
typedef enum { true=1, false=0} bool;

typedef struct {
int int_a;
bool bool_b;
bool dummy[3];
int int_c;
}uninit_memory_access_013_s_001;

void uninit_memory_access_013 ()
{
  int i;
  uninit_memory_access_013_s_001 *s1, s;

  s1 = (uninit_memory_access_013_s_001*)calloc(1,sizeof(uninit_memory_access_013_s_001));
  s1->int_a = 10;
  s1->int_c = 20;

   for (i=0; i<1; i++)
	   s1->dummy[i]= false;

  memcpy(&s, s1, sizeof(uninit_memory_access_013_s_001));/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
  free(s1);
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to union in an switch case and random function
*/
typedef union {
	int a;
	int b;
} uninit_memory_access_014_u_001;


uninit_memory_access_014_u_001 * uninit_memory_access_014_func_001 ()
{
	int flag = rand();
	uninit_memory_access_014_u_001 *u;
	switch (flag)
	{
		case 1:
		{
			u = (uninit_memory_access_014_u_001 *)calloc(1,sizeof(uninit_memory_access_014_u_001));
			u->a = 40;
			return u;
		}
		case 2:
		{
			u = (uninit_memory_access_014_u_001 *)calloc(2,sizeof(uninit_memory_access_014_u_001));
			u->a = 20;
			return u;
		}
		case 3:
		{
			u = (uninit_memory_access_014_u_001 *)calloc(3,sizeof(uninit_memory_access_014_u_001));
			u->a = 30;
			return u;
		}
		default:
			return (uninit_memory_access_014_u_001 *)(-1);
	}
}

void uninit_memory_access_014 ()
{
	int ret;
	uninit_memory_access_014_u_001 *p;
	p = uninit_memory_access_014_func_001 ();
	if(p != NULL)
	{
	ret = p->b;/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
	free(p);
	p= NULL;
	}
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity: When using a pointer to array of Int, memory allocated and initialized using memcpy by return value of function
*/
int (*uninit_memory_access_015_func_001())[4]
{
	int (*p)[4];
	int arr[4][4];
	int i,j;
	p= (int (*)[]) malloc (sizeof arr);
	memcpy(p, arr, sizeof(arr));
	for (i = 0;i< 4; i++)
	{
		for ( j=0 ;j<4; j++)
		{
			*(p[i]+j) += *(p[i]+j);/*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
		}
	}
	return p;
}

void uninit_memory_access_015 ()
{
	int (*ptr)[4];
	ptr = uninit_memory_access_015_func_001();
	free(ptr);
	ptr = NULL;
}

/*
* Types of defects: Uninitialized Memory Access
* Complexity:Uninitialized Memory Access main function
*/

extern volatile int vflag;
void uninit_memory_access_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		uninit_memory_access_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		uninit_memory_access_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		uninit_memory_access_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		uninit_memory_access_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		uninit_memory_access_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		uninit_memory_access_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		uninit_memory_access_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		uninit_memory_access_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		uninit_memory_access_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		uninit_memory_access_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		uninit_memory_access_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		uninit_memory_access_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		uninit_memory_access_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		uninit_memory_access_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		uninit_memory_access_015();
	}
}

