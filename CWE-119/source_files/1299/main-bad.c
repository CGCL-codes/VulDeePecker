
/*

MIT Copyright Notice

Copyright 2003 M.I.T.

Permission is hereby granted, without written agreement or royalty fee, to use, 
copy, modify, and distribute this software and its documentation for any 
purpose, provided that the above copyright notice and the following three 
paragraphs appear in all copies of this software.

IN NO EVENT SHALL M.I.T. BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, 
INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE 
AND ITS DOCUMENTATION, EVEN IF M.I.T. HAS BEEN ADVISED OF THE POSSIBILITY OF 
SUCH DAMANGE.

M.I.T. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED TO 
THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, 
AND NON-INFRINGEMENT.

THE SOFTWARE IS PROVIDED ON AN "AS-IS" BASIS AND M.I.T. HAS NO OBLIGATION TO 
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

$Author: tleek $
$Date: 2004/01/05 17:27:42 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/main-bad.c,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



*/


/*

Sendmail Copyright Notice


Copyright (c) 1998-2003 Sendmail, Inc. and its suppliers.
     All rights reserved.
Copyright (c) 1983, 1995-1997 Eric P. Allman.  All rights reserved.
Copyright (c) 1988, 1993
     The Regents of the University of California.  All rights reserved.

By using this file, you agree to the terms and conditions set
forth in the LICENSE file which can be found at the top level of
the sendmail distribution.


$Author: tleek $
$Date: 2004/01/05 17:27:42 $
$Header: /mnt/leo2/cvs/sabo/hist-040105/sendmail/s2/main-bad.c,v 1.1.1.1 2004/01/05 17:27:42 tleek Exp $



*/


/*

<source>

*/



#include <stdio.h>
#include <stdlib.h>
#include <sendmail-bad.h>

int main(){

  ADDRESS **sendq = NULL;
  ADDRESS *ret_address;
  int aliaslevel = 0;
  
  /* allocate single address */
  ADDRESS *a = (ADDRESS *) malloc(sizeof(struct address));

  a->q_flags = 0x00000000; /* initial flags */
  
  a->q_user = "rpc";

  ret_address = (ADDRESS *) recipient(a, sendq, aliaslevel);
  
  printf("Real name of user %s = %s\n", a->q_user, ret_address->q_fullname);
  
  return 0;
}

/*

</source>

*/

