/*
 * seqstate.h
 *
 * By Ross Ridge
 * Public Domain
 *
 * Defines the class seqstate.  
 *
 * @(#) MySC seqstate.h 1.2 93/11/13 00:14:54
 *
 */

#ifndef __SEQSTATE_H__
#define __SEQSTATE_H__

#include "stack.h"

#ifdef __GNUC__
#pragma interface
#endif

/* This class is used to decide which lines of the body of a SCCS file
   should be included in a gotten file. */

class seq_state {
	enum flags {
		INCLUDED    = 001,
		EXCLUDED    = 002,
		MASK        = 003,
		PREDECESSOR = 004,
		EXPLICIT    = 010,
		ACTIVE 	    = 020,
		INSERTING   = 040,
	};

	unsigned char *p;
	seq_no last;
	int deleting;
	stack<seq_no> active_stack;
	seq_no active;

	void
        copy(class seq_state const &it) {
		last = it.last;
		p = (unsigned char *) xmalloc(sizeof(unsigned char)
					      * (last + 1));
		memcpy(p, it.p, sizeof(unsigned char) * (last + 1));
		deleting = it.deleting;
		active = it.active;
	}

public:
	seq_state(seq_no l): active_stack(l + 1) {
		p = (unsigned char *) xcalloc(l + 1, sizeof(unsigned char));
		last = l;
		deleting = 0;
		active = 0;
	}

	seq_state(class seq_state const &it): active_stack(it.active_stack) {
		copy(it);
	}

	class seq_state &
	operator =(class seq_state const &it) {
		if (this != &it) {
			free(p);
			copy(it);
			active_stack = it.active_stack;
		}
		return *this;
	}

	int
	is_included(seq_no seq) const { 
		return (p[seq] & MASK) == INCLUDED;
	}

	int
	is_excluded(seq_no seq) const { 
		return (p[seq] & MASK) == EXCLUDED;
	}

	int
	is_predecessor(seq_no seq) const {
		return (p[seq] & PREDECESSOR) != 0;
	}

	int
	is_explicit(seq_no seq) const {
		return (p[seq] & EXPLICIT) != 0;
	}
	
	void
	set_predecessor(seq_no seq) {
		assert(seq > 0 && seq <= last);
		if ((p[seq] & MASK) == 0) {
			p[seq] |= INCLUDED;
		}
		p[seq] |= PREDECESSOR;
	}

	void
	pred_include(seq_no seq) {
		assert(seq > 0 && seq <= last);
		p[seq] = (p[seq] & ~MASK) | INCLUDED;
	}

	void
	pred_exclude(seq_no seq) {
		assert(seq > 0 && seq <= last);
		p[seq] = (p[seq] & ~MASK) | EXCLUDED;
	}

	void
	include(seq_no seq) {
		assert(seq > 0 && seq <= last);
		if (!is_included(seq)) {
			p[seq] = (p[seq] & ~MASK) | INCLUDED | EXPLICIT;
		}
	}

	void
	exclude(seq_no seq) {
		assert(seq > 0 && seq <= last);
		if (is_included(seq)) {
			p[seq] = (p[seq] & ~MASK) | EXCLUDED | EXPLICIT;
		}

	}

	const char *
	start(seq_no seq, int insert) {
		assert(seq > 0 && seq <= last);

		if (insert) {
			active_stack.push(active);
			active = seq;
		}

		if (!is_included(seq)) {
			insert = !insert;
		}
		
		if (p[seq] & ACTIVE) {
			return "Seq already active.";
		}

		p[seq] |= ACTIVE;

		if (insert) {
			p[seq] |= INSERTING;
		} else {
			p[seq] &= ~INSERTING;
			deleting++;
		}

		return NULL;
	}

	const char *
	end(seq_no seq) {
		assert(seq > 0 && seq <= last);

		if (!(p[seq] & ACTIVE)) {
			return "Seq not active.";
		}

		p[seq] &= ~ACTIVE;

		int insert = ((p[seq] & INSERTING) != 0);

		if (!insert) {
			assert(deleting > 0);
			deleting--;
		}

		if (!is_included(seq) ^ insert) {
			if (seq == active) {
				active = active_stack.pop();
			} else {
				return "Overlapping insertions";
			}
		}

		return NULL;
	}

	seq_no active_seq() const { return active; }
	int include_line() const { return deleting == 0; }

	~seq_state() {
		free(p);
	}
};

#endif /* __SEQSTATE_H__ */

/* Local variables: */
/* mode: c++ */
/* End: */
