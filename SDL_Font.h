/*
  SDLTk

  font handling
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

#ifndef __SDL_FONT_H__
#define __SDL_FONT_H__

#include "SDL.h"
#include "SDL_Widget.h"
#include "SDL_Window.h"
#include "SDL_String.h"

typedef enum FontType
{
    TTF_FONT,
    FNT_FONT,
    BDF_FONT,
    LAST_FONT
}FontType;

typedef struct SDL_Font
{
    FontType type;
    void *fontdata;
   
    int Height;
    unsigned int color;
}SDL_Font;

typedef struct SDL_FontList
{
    struct SDL_Font     *Font;
    char *FontId;
    char *Filename;
    struct SDL_FontList *Next;
}SDL_FontList;


typedef int               (*T_Font_IsFormat)       (char*);
typedef int               (*T_Font_Read)           (char*,SDL_Font*);
typedef int               (*T_Font_DrawString)     (SDL_Surface *,SDL_Font *,SDL_String *,unsigned int,SDL_Rect*);
typedef int               (*T_Font_DrawChar)       (SDL_Surface *,SDL_Font *,char,SDL_Rect*);
typedef void              (*T_Font_Init)           ();
typedef int               (*T_Font_Metrics)        (SDL_Font* font,SDL_String *string,SDL_Rect *rect);

struct S_Font_FunctionList
{
    T_Font_Init             init;
    T_Font_IsFormat         isformat;
    T_Font_Read             read;
    T_Font_DrawString       drawstring;
    T_Font_DrawChar         drawchar;
    T_Font_Metrics          metrics;
};

extern const struct S_Font_FunctionList FONT_FNT_FunctionList;
extern const struct S_Font_FunctionList FONT_BDF_FunctionList;
extern const struct S_Font_FunctionList FONT_TTF_FunctionList;

static const struct S_Font_FunctionList * const FontTable[] =
{
    &FONT_TTF_FunctionList,      /* FONT_TTF */
    &FONT_FNT_FunctionList,      /* FONT_FNT */
    &FONT_BDF_FunctionList       /* FONT_BDF */
};


void SDL_FontInit();


int SDL_FontDrawString(SDL_Surface *surface,SDL_Font *font,
                       SDL_String *string,unsigned int color,SDL_Rect *rect);

int SDL_FontDrawStringWindow(SDL_Window *window,SDL_Font *font,SDL_String *string,unsigned int color,SDL_Rect *rect);

int SDL_FontGetStringWidth(SDL_Font* font,char* string);
int SDL_FontGetHeight(SDL_Font *font);
int SDL_FontSetHeight(SDL_Font *font,int height);

SDL_Font *SDL_FontLoad(char *filename);
int SDL_FontCalcDims(SDL_Font* font,SDL_String *string,SDL_Rect *rect);
int SDL_FontGetCharWidth(SDL_Font* font,char character);

extern SDL_Font DefaultFont;

#endif /* __SDL_FONT_H__ */
