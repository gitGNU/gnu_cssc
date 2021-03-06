/*
 * bodyio.h: Part of GNU CSSC.
 *
 *
 *    Copyright (C) 1997,1998,2007 Free Software Foundation, Inc.
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

 * See also bodyio.cc, sf-admin.cc and encoding.cc.
 *
 */
#ifndef CSSC_INC_BODYIO_H
#define CSSC_INC_BODYIO_H 1

#include <cstdio>

bool body_insert_text(const char iname[], const char oname[],
		      FILE *in, FILE *out,
		      unsigned long int *lines,
		      bool *idkw, bool *binary, bool *fail);

bool body_insert_binary(const char iname[], const char oname[],
			FILE *in, FILE *out,
			unsigned long int *lines,
			bool *idkw);

bool body_insert(bool *binary,
		 const char iname[], const char oname[],
		 FILE *in, FILE *out,
		 unsigned long int *lines,
		 bool *idkw);


// Encoding functions...
void encode_line(const char in[], char out[], size_t len);
size_t decode_line(const char in[], char out[]);
int encode_stream(FILE *fin, FILE *fout); //encodes whole file.


// Decoding (output) functions
class cssc_linebuf;
int output_body_line_text  (FILE *fp, const cssc_linebuf*);
int output_body_line_binary(FILE *fp, const cssc_linebuf*);


bool check_id_keywords(const char *s, size_t len);

int encode_file(const char* name_in, const char* name_out);

// These are only really extern so that unit-tests/test_encoding can execute them.
void encode(const char in[3], char out[4]);
void decode(const char in[4], char out[3]);

#endif
