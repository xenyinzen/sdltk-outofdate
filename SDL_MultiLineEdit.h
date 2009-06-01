/*
  SDLTk

  Multi line edit
  Copyright (C) 2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDL_MULTILINEEDIT_H__
#define __SDL_MULTILINEEDIT_H__

typedef struct SDL_TextCursor
{
    struct SDL_EditParagraph *p;
    int CursorPosition;
}SDL_TextCursor;

typedef struct SDL_EditParagraph
{
    SDL_String    *Caption;
    int *linepos;
    int linecount;

    struct SDL_EditParagraph *next;
    struct SDL_EditParagraph *prev;
}SDL_EditParagraph;

typedef struct SDL_MultiLineEdit
{
    SDL_Widget  Widget;
    SDL_Font   *Font;

    struct SDL_TextCursor    Cursor;
    struct SDL_EditParagraph *Paragraph;

    Uint32   fgcolor;
    Uint32   bgcolor;
}SDL_MultiLineEdit;





SDL_Widget *SDL_MultiLineEditNew();
void SDL_MultiLineEditSetText(SDL_Widget *widget,char *text);

#endif /* __SDL_MULTILINEEDIT_H__ */




