/*
 * filediff.cc: Part of GNU CSSC.
 * 
 *    Copyright (C) 1998, Free Software Foundation, Inc. 
 * 
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * CSSC was originally Based on MySC, by Ross Ridge, which was 
 * placed in the Public Domain.
 *
 *
 * Functions for diffing two files.
 *
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "cssc.h"
#include "filediff.h"

#ifdef CONFIG_SCCS_IDS
static const char rcs_id[] = "CSSC $Id: filediff.cc,v 1.3 1998/03/10 00:19:23 james Exp $";
#endif


FileDiff::FileDiff(const char *n1, const char *n2)
  : name1(n1), name2(n2), fp(0)
{
}

FileDiff::~FileDiff()
{
  finish();
}

void
FileDiff::finish()
{
  if (fp)
    pclose(fp);
  fp = 0;
}

FILE*
FileDiff::start()
{
  const mystring space(" ");
  const mystring quote("'");
  mystring cmd(mystring(CONFIG_DIFF_COMMAND) + 
	       space + quote + name1 + quote + 
	       space + quote + name2 + quote);
  
  fp = popen(cmd.c_str(), "r");
  return fp;
}



/* Local variables: */
/* mode: c++ */
/* End: */
