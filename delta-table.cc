/*
 * sccsfile.c: Part of GNU CSSC.
 * 
 * 
 *    Copyright (C) 1997, Free Software Foundation, Inc. 
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
 * Common members of the class sccs_file and its subclasses.  Most of
 * the members in this file are used to read from the SCCS file.
 *
 */

#include "cssc.h"
#include "sccsfile.h"
#include "delta.h"
#include "delta-table.h"
#include "delta-iterator.h"

delta const &
cssc_delta_table::delta_at_seq(seq_no seq)
{
  ASSERT(seq > 0 && seq <= high_seqno);
  if (seq_table == NULL)
    {
      build_seq_table();
    }
  return l.select(seq_table[seq]);
}



cssc_delta_table::~cssc_delta_table()
{
  if (seq_table)
    free(seq_table);
}



seq_no cssc_delta_table::next_seqno() const
{
  return highest_seqno() + 1u;
}


/* Builds the seq_no to delta table index table. */

void
cssc_delta_table::build_seq_table()
{
  seq_table = (int *) xmalloc((high_seqno + 1) * sizeof(int));

  int i;
  for(i = 0; i < high_seqno + 1; i++)
    {
      seq_table[i] = -1;
    }
  
  delta_iterator iter(this);
  while (iter.next(1))
    {
      seq_no seq = iter->seq;
      if (seq_table[seq] != -1)
	{
	  quit(-1, "Sequence number %u is duplicated"
	       " in delta table [build].", seq);
	}
      seq_table[seq] = iter.index();
    }
}

void
cssc_delta_table::update_highest(const delta &it) 
{
  seq_no seq = it.seq;
  
  if (seq > high_seqno) 
    {
      if (seq_table != NULL) 
	{
	  seq_table = (int *) xrealloc(seq_table, 
				       (seq + 1) * sizeof(int));
	  int i;
	  for(i = high_seqno + 1; i < seq + 1; i++) 
	    {
	      seq_table[i] = -1;
	    }
	}
      high_seqno = it.seq;
    }

  /* high_release is initialised to {0} so 
   * any greater-than comparison always fails since 
   * operator> decides it's not comparable with it.id.
   */
  if (high_release.is_null())
    {
      if (it.id.on_trunk())
	high_release = it.id;
    }
  else if (it.id > high_release)
    {
      high_release = it.id;
    }

  if (seq_table) 
    {
      if (seq_table[seq] != -1) 
	{
	  sid sid_using_this_seqno = delta_at_seq(seq).id;

	  fprintf(stderr, "[<");
	  sid_using_this_seqno.print(stderr);
	  fprintf(stderr, " ## ");
	  it.id.print(stderr);
	  fprintf(stderr, ">]\n");

	  if (it.id != sid_using_this_seqno)
	    {
	      // diagnose...
	      for (int i = 1; i < high_seqno + 1; i++) 
		fprintf(stderr, "%u ", (unsigned)seq_table[i]);

	      fprintf(stderr,
		      "Sequence number %u is duplicated"
		      " in delta table [update] "
		      "(already used by entry %u: ", seq, seq_table[seq]);
	      sid_using_this_seqno.print(stderr);
	      fprintf(stderr, ")\n");
	  
	      quit(-1, "Sequence number %u is duplicated"
		   " in delta table [update]", seq);
	    }
	}
      seq_table[seq] = l.length() - 1;
    }
}



/* Adds a delta to the end of the delta_table. */

void
cssc_delta_table::add(const delta &it)
{
  l.add(it);
  update_highest(it);
}

/* for the prepend() operation, see sf-add.cc. */


/* Finds a delta in the delta table by its SID. */

delta const * cssc_delta_table::
find(sid id) const
{
  const_delta_iterator iter(this);
  
  while (iter.next())
    {
      if (iter->id == id)
	{
	  return iter.operator->();
	}
    }
  return NULL;
}

// This non-const variety is used by sf-cdc.cc.
delta * cssc_delta_table::
find(sid id)
{
  delta_iterator iter(this);
  
  while (iter.next())
    {
      if (iter->id == id)
	{
	  return iter.operator->();
	}
    }
  return NULL;
}


