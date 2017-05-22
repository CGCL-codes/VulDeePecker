/********Software Analysis - FY2013*************/
/*
* File Name: overrun_st.c
* Defect Classification
* ---------------------
* Defect Type: Static memory defects
* Defect Sub-type: Static buffer overrun
*
*/

#include "HeaderFile.h"
int rand (void);

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	char	Constant	Write
 */
void overrun_st_001 ()
{
	char buf[5];
	buf[5] = 1; /*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	short	Constant	Write
 */
void overrun_st_002 ()
{
	short buf[5];
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Constant	Loading
 */
void overrun_st_003 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	int ret;
	ret = buf[5];/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Constant	Write
 */
void overrun_st_004 ()
{
	int buf[5];
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	long	Constant	Write
 */
void overrun_st_005 ()
{
	long buf[5];
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	float	Constant	Write
 */
void overrun_st_006 ()
{
	float buf[5];
	buf[5] = 1.0;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	double	Constant	Write
 */
void overrun_st_007 ()
{
	double buf[5];
	buf[5] = 1.0;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: two dimensional array	int	Constant	Write
 */
void overrun_st_008 ()
{
	int buf[5][6];
	buf[5][5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx][idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 3-dimensional array	int	Constant	Write
 */
void overrun_st_009 ()
{
	int buf[5][6][7];
	buf[5][5][6] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx][idx][idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: pointer arrays	int	Constant	Write
 */
void overrun_st_010 ()
{
	int buf1[6];
	int buf2[6];
	int buf3[6];
	int buf4[6];
	int buf5[5];
	int *pbuf[5] = {buf1, buf2, buf3, buf4, buf5};
	pbuf[4][5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: struct array	-	Constant	Write
 */
typedef struct {
	int a;
	int b;
	int c;
} overrun_st_011_s_001;

void overrun_st_011 ()
{
	overrun_st_011_s_001 sbuf[5];
	sbuf[5].a = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: an array structure member	-	Constant	Write
 */
typedef struct {
	int a;
	int b;
	int buf[5];
} overrun_st_012_s_001;
overrun_st_012_s_001 overrun_st_012_s_gbl;
void overrun_st_012 ()
{

	overrun_st_012_s_gbl.buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Variable	Write
 */
void overrun_st_013 ()
{
	int buf[5];
	int index = 5;
	buf[index] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Value of random variable	Write
 */
void overrun_st_014 ()
{
	int buf[5];
	int index;
	index = rand();
	buf[index] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Linear equation	Write
 */
void overrun_st_015 ()
{
	int buf[5];
	int index = 2;
	buf[(2 * index) + 1] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Nonlinear equation	Write
 */
void overrun_st_016 ()
{
	int buf[5];
	int index = 2;
	buf[(index * index) + 1] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	The return value of the function	Write
 */
int overrun_st_017_func_001 ()
{
	return 5;
}

void overrun_st_017 ()
{
	int buf[5];
	buf[overrun_st_017_func_001()] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Function arguments	Write
 */
int overrun_st_018_buf[5];
void overrun_st_018_func_001 (int index)
{
	overrun_st_018_buf[index] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

void overrun_st_018 ()
{
	overrun_st_018_func_001(5);
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	An array of element values	Write
 */
void overrun_st_019 ()
{
	int buf[5];
	int indexes[4] = {3, 4, 5, 6};
	int index = 2;
	buf[indexes[index]] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Alias for 1 weight	Write
 */
void overrun_st_020 ()
{
	int buf[5];
	int index = 5;
	int index1;
	index1 = index;
	buf[index1] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1-dimensional array	int	Also known as double Write
 */
void overrun_st_021 ()
{
	int buf[5];
	int index = 5;
	int index1;
	int index2;
	index1 = index;
	index2 = index1;
	buf[index2] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity:  single char pointer	single Alias Constant Write
 */
void overrun_st_022 ()
{
	char buf[5];
	char *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single short pointer	single Alias Constant Write
 */
void overrun_st_023 ()
{
	short buf[5];
	short *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single int pointer	single Alias Constant 	Loading
 */
void overrun_st_024 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	int *p;
	int ret;
	p = buf;
	ret = *(p + 5);/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single int pointer	single Alias Constant 	Write
 */
void overrun_st_025 ()
{
	int buf[5];
	int *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single long pointer	single Alias Constant 	Write
 */
void overrun_st_026 ()
{
	long buf[5];
	long *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single float pointer	single Alias Constant 	Write
 */
void overrun_st_027 ()
{
	float buf[5];
	float *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: single double pointer double Alias Constant writ
 */
void overrun_st_028 ()
{
	double buf[5];
	double *p;
	p = buf;
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: double pointer	int	single Alias for Constant Write
 */
void overrun_st_029 ()
{
	int buf[5];
	int *p;
	int **pp;
	p = buf;
	pp = &p;
	*(*pp + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	Also known as double	alias Variable	Write
 */
void overrun_st_030 ()
{
	int buf[5];
	int *p1;
	int *p2;
	p1 = buf;
	p2 = p1;
	p2[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1 pointer	int	Pointer variables array	-	Constant	Write
 */
int overrun_st_031_buf_gbl[5];
void overrun_st_031 ()
{
	int *p;
	p = overrun_st_031_buf_gbl;
	p[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1 pointer	int	single Alias 	Variable	Write
 */
void overrun_st_032 ()
{
	int buf[5];
	int *p;
	int index = 5;
	p = buf;
	*(p + index) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1 pointer int	single Alias Value of random variable	Write
 */
void overrun_st_033 ()
{
	int buf[5];
	int *p;
	int index;
	p = buf;
	index = rand();
	*(p + index) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	single Alias Linear equation	Write
 */
void overrun_st_034 ()
{
	int buf[5];
	int *p;
	int index = 2;
	p = buf;
	*(p + ((2 * index) + 1)) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer int	single Alias 	Nonlinear equation	Write
 */
void overrun_st_035 ()
{
	int buf[5];
	int *p;
	int index = 2;
	p = buf;
	*(p + ((index * index) + 1)) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	single Alias 	The return value of the function	Write
 */
int overrun_st_036_func_001 ()
{
	return 5;
}

void overrun_st_036 ()
{
	int buf[5];
	int *p;
	p = buf;
	*(p + overrun_st_036_func_001()) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1 pointer int	single Alias Function arguments	Write
 */
void overrun_st_037_func_001 (int index)
{
	int buf[5];
	int *p;
	p = buf;
	*(p + index) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

void overrun_st_037 ()
{
	overrun_st_037_func_001(5);
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	Alias An array of element values	Write
 */
void overrun_st_038 ()
{
	int buf[5];
	int *p;
	int indexes[4] = {3, 4, 5, 6};
	int index = 2;
	p = buf;
	*(p + indexes[index]) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	Alias for variable 	Write
 */
void overrun_st_039 ()
{
	int buf[5];
	int *p;
	int index = 5;
	int index1;
	index1 = index;
	p = buf;
	*(p + index1) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1  pointer	int	Alias for variable double	Write
 */
void overrun_st_040 ()
{
	int buf[5];
	int *p;
	int index = 5;
	int index1;
	int index2;
	index1 = index;
	index2 = index1;
	p = buf;
	*(p + index2) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: 1 loop	1-Dimensional array
 */
void overrun_st_041 ()
{
	int buf[5];
	int i;
	for (i = 0; i <= 5; i ++)
	{
		buf[i] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
	}
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: for double-loop	2-Dimensional array
 */
void overrun_st_042 ()
{
	int buf[5][6];
	int i;
	int j;
	for (i = 0; i <= 5; i ++)
	{
		for (j = 0; j < 6; j ++)
		{
			buf[i][j] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
		}
	}
        sink = buf[idx][idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: for double-loop	Pointer arrays
 */
void overrun_st_043 ()
{
	int buf1[6];
	int buf2[6];
	int buf3[6];
	int buf4[6];
	int buf5[5];
	int *pbuf[5] = {buf1, buf2, buf3, buf4, buf5};
	int i;
	int j;

	for (i = 0; i < 5; i ++)
	{
		for (j = 0; j < 6; j ++)
		{
			pbuf[i][j] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
		}
	}
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: increment the pointer value in the loop
 */
void overrun_st_044 ()
{
	int buf[5];
	int *p;
	int i;
	p = buf;
	for (i = 0; i <= 5; i ++)
	{
		*p = 1;
		p ++;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
	}
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: An array	- An array as function parameter
 */
void overrun_st_045_func_001 (int buf[])
{
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

void overrun_st_045 ()
{
	int buf[5];
	overrun_st_045_func_001(buf);
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: pointer -Pointer as function parameter
 */
void overrun_st_046_func_001 (int *p)
{
	*(p + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

void overrun_st_046 ()
{
	int buf[5];
	overrun_st_046_func_001(buf);
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: pointer	- Pointer as function parameter accessed as array
 */
void overrun_st_047_func_001 (int *p)
{
	p[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

void overrun_st_047 ()
{
	int buf[5];
	overrun_st_047_func_001(buf);
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexities: pointer - An array as function parameter
 */
void overrun_st_048_func_001 (int buf[])
{
	*(buf + 5) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

void overrun_st_048 ()
{
	int buf[5];
	overrun_st_048_func_001(buf);
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: abbreviation for 1-dimensional array array size
 */
void overrun_st_049 ()
{
	int buf[] = {1, 2, 3, 4, 5};
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
        sink = buf[idx];
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: abbreviation for two-dimensional array array size
 */
void overrun_st_050 ()
{
	int buf[][6] =
		{
			{1, 2, 3, 4, 5, 6},
			{1, 2, 3, 4, 5, 6},
			{1, 2, 3, 4, 5, 6},
			{1, 2, 3, 4, 5, 6},
			{1, 2, 3, 4, 5, 6}
		};
	buf[5][5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: omission of the pointer array array size
 */
void overrun_st_051 ()
{
	int buf1[] = {1, 2, 3, 4, 5, 6};
	int buf2[] = {1, 2, 3, 4, 5, 6};
	int buf3[] = {1, 2, 3, 4, 5, 6};
	int buf4[] = {1, 2, 3, 4, 5, 6};
	int buf5[] = {1, 2, 3, 4, 5};
	int *pbuf[] = {buf1, buf2, buf3, buf4, buf5};
	pbuf[4][5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: omission of the size of the string pointer
 */
void overrun_st_052 ()
{
	char buf[] = "1234";
	buf[5] = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: cast large data size-small size
 */
void overrun_st_053 ()
{
	char buf[8];
	int *p;
	p = (int*)buf;
	*(p + 2) = 1;/*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}

/*
 * Types of defects: buffer overruns ( static buffer )
 * Complexity: cast small data size-large size
 */
int overrun_st_054_buf_gbl[5];
void overrun_st_054 ()
{
	char *p;
	p = (char*)overrun_st_054_buf_gbl;
	*(p + 50) = 1; /*Tool should detect this line as error*/ /*ERROR: buffer overrun */
}


/*
 * Types of defects: buffer overruns ( static buffer )
 * buffer overruns ( static buffer ) main function
 */
extern volatile int vflag;
void overrun_st_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		overrun_st_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		overrun_st_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		overrun_st_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		overrun_st_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		overrun_st_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		overrun_st_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		overrun_st_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		overrun_st_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		overrun_st_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		overrun_st_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		overrun_st_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		overrun_st_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		overrun_st_013();
	}

	if (vflag == 14 || vflag ==888)
	{
		overrun_st_014();
	}

	if (vflag == 15 || vflag ==888)
	{
		overrun_st_015();
	}

	if (vflag == 16 || vflag ==888)
	{
		overrun_st_016();
	}

	if (vflag == 17 || vflag ==888)
	{
		overrun_st_017();
	}

	if (vflag == 18 || vflag ==888)
	{
		overrun_st_018();
	}

	if (vflag == 19 || vflag ==888)
	{
		overrun_st_019();
	}

	if (vflag == 20 || vflag ==888)
	{
		overrun_st_020();
	}

	if (vflag == 21 || vflag ==888)
	{
		overrun_st_021();
	}

	if (vflag == 22 || vflag ==888)
	{
		overrun_st_022();
	}

	if (vflag == 23 || vflag ==888)
	{
		overrun_st_023();
	}

	if (vflag == 24 || vflag ==888)
	{
		overrun_st_024();
	}

	if (vflag == 25 || vflag ==888)
	{
		overrun_st_025();
	}

	if (vflag == 26 || vflag ==888)
	{
		overrun_st_026();
	}

	if (vflag == 27 || vflag ==888)
	{
		overrun_st_027();
	}

	if (vflag == 28 || vflag ==888)
	{
		overrun_st_028();
	}

	if (vflag == 29 || vflag ==888)
	{
		overrun_st_029();
	}

	if (vflag == 30 || vflag ==888)
	{
		overrun_st_030();
	}

	if (vflag == 31 || vflag ==888)
	{
		overrun_st_031();
	}

	if (vflag == 32 || vflag ==888)
	{
		overrun_st_032();
	}

	if (vflag == 33 || vflag ==888)
	{
		overrun_st_033();
	}

	if (vflag == 34 || vflag ==888)
	{
		overrun_st_034();
	}

	if (vflag == 35 || vflag ==888)
	{
		overrun_st_035();
	}

	if (vflag == 36 || vflag ==888)
	{
		overrun_st_036();
	}

	if (vflag == 37 || vflag ==888)
	{
		overrun_st_037();
	}

	if (vflag == 38 || vflag ==888)
	{
		overrun_st_038();
	}

	if (vflag == 39 || vflag ==888)
	{
		overrun_st_039();
	}

	if (vflag == 40 || vflag ==888)
	{
		overrun_st_040();
	}

	if (vflag == 41 || vflag ==888)
	{
		overrun_st_041();
	}

	if (vflag == 42 || vflag ==888)
	{
		overrun_st_042();
	}

	if (vflag == 43 || vflag ==888)
	{
		overrun_st_043();
	}

	if (vflag == 44 || vflag ==888)
	{
		overrun_st_044();
	}

	if (vflag == 45 || vflag ==888)
	{
		overrun_st_045();
	}

	if (vflag == 46 || vflag ==888)
	{
		overrun_st_046();
	}

	if (vflag == 47 || vflag ==888)
	{
		overrun_st_047();
	}

	if (vflag == 48 || vflag ==888)
	{
		overrun_st_048();
	}

	if (vflag == 49 || vflag ==888)
	{
		overrun_st_049();
	}

	if (vflag == 50 || vflag ==888)
	{
		overrun_st_050();
	}

	if (vflag == 51 || vflag ==888)
	{
		overrun_st_051();
	}

	if (vflag == 52 || vflag ==888)
	{
		overrun_st_052();
	}

	if (vflag == 53 || vflag ==888)
	{
		overrun_st_053();
	}

	if (vflag == 54 || vflag ==888)
	{
		overrun_st_054();
	}
}
