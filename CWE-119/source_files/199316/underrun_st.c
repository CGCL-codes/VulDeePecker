/********Software Analysis - FY2013*************/
/*
* File Name: underrun_st.c
* Defect Classification
* ---------------------
* Defect Type: Static memory defects
* Defect Sub-type: Static buffer underrun
* Description: Defect Free Code to identify false positives in stack underun condition
*/

#include "HeaderFile.h"

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1-dimensional array	int	Constant	Loading
 */
void underrun_st_001 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	int ret;
	ret = buf[0]; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
        sink = ret;
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1-dimensional array	int	Constant	Write
 */
void underrun_st_002 ()
{
	int buf[5];
	buf[0] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
        sink = buf[idx];
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1-dimensional array	int	Variable	Write
 */
void underrun_st_003 ()
{
	int buf[5];
	int index = 0;
	buf[index] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
        sink = buf[idx];
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 single pointer	int	Alias for Subtraction	Constant-1	Loading
 */
void underrun_st_004 ()
{
	int buf[5] = {1, 2, 3, 4, 5};
	int *p;
	int ret;
	p = &buf[1];
	ret = *(p - 1); /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
        sink = ret;
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 single pointer	int	Alias for Subtraction	Constant -1	Write
 */
void underrun_st_005 ()
{
	int buf[5];
	int *p;
	p = &buf[1];
	*(p - 1) = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 single pointer	int	Alias for Subtraction	1 Variable	Write
 */
void underrun_st_006 ()
{
	int buf[5];
	int *p;
	int index = 1;
	p = &buf[1];
	*(p - index) = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 loop	1-Dimensional array
 */
void underrun_st_007 ()
{
	int buf[5];
	int i;
	for (i = 4; i > -1; i --)
	{
		buf[i] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
	}
        sink = buf[idx];
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: a decrement in a loop pointer value
 */
void underrun_st_008 ()
{
	int buf[5];
	int *p;
	int i;
	p = &buf[4];
	for (i = 0; i < 5; i ++)
	{
		*p = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
		p --;
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 loop	1-Dimensional array
 */
int underrun_st_009_gbl_buf[5];
void underrun_st_009 ()
{
	int i;
	for (i = 4; i > -1; i --)
	{
		underrun_st_009_gbl_buf[i] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: a decrement in a loop pointer value
 */
int underrun_st_010_gbl_buf[5];
void underrun_st_010 ()
{
	int *p;
	int i;
	p = &underrun_st_010_gbl_buf[4];
	for (i = 0; i < 5; i ++)
	{
		*p = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
		p --;
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 loop	1-Dimensional array
 */
int underrun_st_011_gbl_buf[5];
void underrun_st_011 ()
{
	int i=4;
	while(i > -1)
	{
		underrun_st_011_gbl_buf[i] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
	    i--;
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: a decrement in a loop pointer value
 */
int underrun_st_012_gbl_buf[5];
void underrun_st_012 ()
{
	int *p;
	p = &underrun_st_012_gbl_buf[4];
	int i=4;
	while(i > -1)
	{
		*p = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
		p --;
		i--;
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * Complexity: 1 loop	1-Dimensional array
 */
int underrun_st_013_gbl_buf[5];
void underrun_st_013 ()
{
	int i=4;
	int var=0;
	while(i > -1)
	{
		if(var==0)
		underrun_st_013_gbl_buf[i] = 1; /*Tool should not detect this line as error*/ /*No ERROR:Data Underrun*/
	    i--;
	}
}

/*
 * Types of defects: buffer underrun ( static buffer )
 * buffer underrun ( static buffer ) - Main function
 */
extern volatile int vflag;
void underrun_st_main ()
{
	if (vflag == 1 || vflag ==888)
	{
		underrun_st_001();
	}

	if (vflag == 2 || vflag ==888)
	{
		underrun_st_002();
	}

	if (vflag == 3 || vflag ==888)
	{
		underrun_st_003();
	}

	if (vflag == 4 || vflag ==888)
	{
		underrun_st_004();
	}

	if (vflag == 5 || vflag ==888)
	{
		underrun_st_005();
	}

	if (vflag == 6 || vflag ==888)
	{
		underrun_st_006();
	}

	if (vflag == 7 || vflag ==888)
	{
		underrun_st_007();
	}

	if (vflag == 8 || vflag ==888)
	{
		underrun_st_008();
	}

	if (vflag == 9 || vflag ==888)
	{
		underrun_st_009();
	}

	if (vflag == 10 || vflag ==888)
	{
		underrun_st_010();
	}

	if (vflag == 11 || vflag ==888)
	{
		underrun_st_011();
	}

	if (vflag == 12 || vflag ==888)
	{
		underrun_st_012();
	}

	if (vflag == 13 || vflag ==888)
	{
		underrun_st_013();
	}
}
