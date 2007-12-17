/*
 * myset.h: Part of GNU CSSC.
 * 
 * 
 *    Copyright (C) 2002,2004,2007 Free Software Foundation, Inc. 
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
 * 
 * CSSC was originally Based on MySC, by Ross Ridge, which was 
 * placed in the Public Domain.
 *
 *
 * Defines the template myset.
 *
 */
#ifndef CSSC__MYSETLIST_H__
#define CSSC__MYSETLIST_H__

template <class TYPE>
class myset
{
    mylist<TYPE> members;
    
public:
    myset()
        {
        }
    
    
    int count() const
        {
            return members.length();
        }

    const mylist<TYPE>& list() const
        {
            return members;
        }
    
    bool is_member(TYPE const &ent) const
        {
            const int n = count();
            for (int i=0; i<n; ++i)
            {
                if (members[i] == ent)
                    return true;
            }
            return false;
        }
    
    void add(TYPE const &ent)
        {
            if (!is_member(ent))
                members.add(ent);
        }
};

#endif
