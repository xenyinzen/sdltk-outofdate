/*
  SDLTk

  
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
#ifndef __SDL_FONTTTF_H__
#define __SDL_FONTTTF_H__

typedef struct TTF_Font
{
    unsigned char *chartable;           
    unsigned char *font;
    int firstchar;                      
    int lastchar;
    char height;
    int offset;
}TTF_Font;


int FONT_TTF_CharInfo(SDL_Font *font,char charcode,int *x,int *y);

#endif /* __SDL_FONTTTF_H__ */