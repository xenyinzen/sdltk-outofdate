/*
  SDLTk

  SDL_String representation
  Copyright (C) 2003-2006 John Beuving (john.beuving@beatforce.org)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef __SDL_STRING_H__
#define __SDL_STRING_H__

typedef struct SDL_String
{
    unsigned int MaxLen;
    unsigned int Length;
    char *Data;
}SDL_String;

typedef unsigned int SDL_Char;

SDL_String *SDL_StringCreate(char *text);
int  SDL_StringSetText(SDL_String *String,char *text);
int  SDL_StringFree(SDL_String *String);
unsigned int SDL_StringLength(SDL_String *String);
SDL_String *SDL_StringSubString(SDL_String *String,int start,int count);
SDL_String *SDL_StringInsert(SDL_String *String,int pos,SDL_Char *c);
SDL_String *SDL_StringPrealloc(unsigned int size);
void SDL_StringRemove(SDL_String *String,int pos,int size);
#endif /* __SDL_STRING_H__ */
