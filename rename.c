/* rename.c: Part of GNU CSSC.
 *
 *  Copyright (C) 1997,2007 Free Software Foundation, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *    
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *    
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if HAVE_UNISTD_H
#include <unistdh>
#endif

#ifndef HAVE_RENAME

int
rename(const char *from, const char *to) {
	if (link(from, to) == -1 || unlink(from) == -1) {
		return -1;
	}
	return 0;
}

#endif /* HAVE_RENAME */
