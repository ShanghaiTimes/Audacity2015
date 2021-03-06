/*
 * libid3tag - ID3 tag manipulation library
 * Copyright (C) 2000-2004 Underbit Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: render.h,v 1.3 2004-06-08 06:38:15 dmazzoni Exp $
 */

# ifndef LIBID3TAG_RENDER_H
# define LIBID3TAG_RENDER_H

# include "id3tag.h"

id3_length_t id3_render_immediate(id3_byte_t **, char const *, unsigned int);
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long long, unsigned int);
id3_length_t id3_render_int(id3_byte_t **, signed long long, unsigned int);
#else
#define ENVIRONMENT32
id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long, unsigned int);
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long long, unsigned int);
id3_length_t id3_render_int(id3_byte_t **, signed long long, unsigned int);
#else
#define ENVIRONMENT32
id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long long, unsigned int);
#endif
#endif 

//id3_length_t id3_render_syncsafe(id3_byte_t **, unsigned long, unsigned int);
//id3_length_t id3_render_int(id3_byte_t **, signed long, unsigned int);

id3_length_t id3_render_binary(id3_byte_t **,
			       id3_byte_t const *, id3_length_t);
id3_length_t id3_render_latin1(id3_byte_t **, id3_latin1_t const *, int);
id3_length_t id3_render_string(id3_byte_t **, id3_ucs4_t const *,
			       enum id3_field_textencoding, int);
id3_length_t id3_render_padding(id3_byte_t **, id3_byte_t, id3_length_t);

id3_length_t id3_render_paddedstring(id3_byte_t **, id3_ucs4_t const *,
				     id3_length_t);

# endif
