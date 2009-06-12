/*
  SDLTk

  Edit widget
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
#ifndef __SDL_EDIT_H__
#define __SDL_EDIT_H__

#include "SDL_Widget.h"
#include "SDL_String.h"
#include "SDL_Font.h"

typedef struct SDL_Edit
{
    SDL_Widget  Widget;
    SDL_Font   *Font;

    SDL_String    Caption;

    unsigned int CursorPosition;
    Uint32   fgcolor;
    Uint32   bgcolor;

}SDL_Edit;


/* prototypes */
SDL_Widget *SDL_EditNew();
int         SDL_EditProperties(SDL_Widget *widget,int feature,va_list list);


char *SDL_EditGetText(SDL_Widget *widget);
int   SDL_EditSetFont(SDL_Widget *widget,SDL_Font *font);
void  SDL_EditSetText(SDL_Widget *widget,const char *text);

#endif /* __SDL_EDIT_H__ */
