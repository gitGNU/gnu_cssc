/*
 * quit.h: Part of GNU CSSC.
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
 * @(#) CSSC quit.h 1.1 93/11/09 17:17:49
 *
 */

#ifndef CSSC__QUIT_H__
#define CSSC__QUIT_H__

#ifdef __GNUC__
#pragma interface
#endif



// Cleaner is a class that you should have in scope in main;
// it ensures that al; required cleanups are run even if you don't
// call quit().
class Cleaner
{
public:
  Cleaner();
  ~Cleaner();
};


class cleanup {
	static class cleanup *head;
	static int running;
	static int all_disabled;
#if HAVE_FORK
	static int in_child_flag;
#endif

	class cleanup *next;
	
protected:
	cleanup();
	virtual void do_cleanup() = 0;

#ifdef __GNUC__
	virtual /* not needed but it gets rid of an anoying warning */
#endif
	~cleanup();

public:

	static void run_cleanups();
	static int active() { return running; }
	static void disable_all() { all_disabled++; }
#ifdef HAVE_FORK
	static void set_in_child() { in_child_flag = 1; disable_all(); }
	static int in_child() { return in_child_flag; }
#endif
};

extern const char *prg_name;

void set_prg_name(const char *name);

// errormsg(): emit an error message preceded by the program name.
//             then return to the caller (don't exit).
void errormsg(const char *fmt, ...);


// errormsg_with_errno(): emits 
//   prog: your-text-here: errno-error-message \n 
// to STDERR.
void errormsg_with_errno(const char *fmt, ...);


#ifdef __GNUC__
#if 0
NORETURN quit(int err, const char *fmt, ...)
	__attribute__((format(printf, 2, 3))) POSTDECL_NORETURN;
#endif
NORETURN fatal_quit(int err, const char *fmt, ...)
	__attribute__((format(printf, 2, 3))) POSTDECL_NORETURN;
NORETURN p_corrupt_quit(const char *fmt, ...)
	__attribute__((format(printf, 1, 2))) POSTDECL_NORETURN;
NORETURN s_corrupt_quit(const char *fmt, ...)
	__attribute__((format(printf, 1, 2))) POSTDECL_NORETURN;
NORETURN s_missing_quit(const char *fmt, ...)
	__attribute__((format(printf, 1, 2))) POSTDECL_NORETURN;
NORETURN ctor_fail(int err, const char *fmt, ...)
	__attribute__((format(printf, 2, 3))) POSTDECL_NORETURN;
#else
#if 0
NORETURN quit(int err, const char *fmt, ...)  POSTDECL_NORETURN;
#endif

NORETURN s_corrupt_quitquit(const char *fmt, ...)  POSTDECL_NORETURN;
NORETURN p_corrupt_quit(const char *fmt, ...)  POSTDECL_NORETURN;
NORETURN fatal_quit(int err, const char *fmt, ...)  POSTDECL_NORETURN;
NORETURN ctor_fail(int err, const char *fmt, ...)  POSTDECL_NORETURN;
#endif

NORETURN nomem()  POSTDECL_NORETURN;
NORETURN assert_failed(const char *file, int line, const char *func,
		       const char *test) POSTDECL_NORETURN;

#ifdef __GNUC__
#define ASSERT(test) ((test) ? (void) 0					\
		             : assert_failed(__FILE__, __LINE__,        \
					     __PRETTY_FUNCTION__, #test))
#else
#define ASSERT(test) ((test) ? (void) 0					\
		             : assert_failed(__FILE__, __LINE__, "", #test))
#endif

extern void usage();

#endif /* __QUIT_H__ */

/* Local variables: */
/* mode: c++ */
/* End: */
