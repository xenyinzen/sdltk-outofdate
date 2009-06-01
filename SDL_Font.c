/*
  SDLTk

  General font API
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "SDL_Font.h"
#include "SDL_FontBDF.h"
#include "SDL_FontFNT.h"
#include "SDL_FontTTF.h"
#include "SDL_String.h"


void SDL_ReadFont(FILE *fp,SDL_Font *font);

int SDL_FontDrawChar(SDL_Surface *dest,SDL_Font *font,
                     char character,SDL_Rect *pos);
void DrawPixel(SDL_Surface *screen, int x, int y,unsigned int color2);




SDL_FontList *LoadedFonts = NULL;

void SDL_FontInit()
{
    int i;
    for(i=0;i<LAST_FONT;i++)
    {
        if(FontTable[i]->init != NULL)
            FontTable[i]->init();
    }
}


SDL_Font *SDL_FontLoad(char *filename)
{
    SDL_Font *font=NULL;
    int i;        
    T_Font_IsFormat isformat;
    T_Font_Read     readfont;

    if(filename == NULL)
        return NULL;
    
    font=malloc(sizeof(SDL_Font));
    memset(font,0,sizeof(SDL_Font));
    
    for(i=0;i<LAST_FONT;i++)
    {
        isformat = FontTable[i]->isformat;
        if(isformat(filename))
        {
            readfont = FontTable[i]->read;
            if(readfont(filename,font) == 0)
                return NULL;
                
            if(font)
                font->color=0xfefefe;
            break;
        }
    }
    return font;     
}

#if 0
void SDL_FontLoad(char *fontid,char *filename)
{
    SDL_FontList *new_item=(SDL_FontList*)malloc(sizeof(SDL_FontList));
    new_item->FontId   =(char*)strdup(fontid);
    new_item->Filename =(char*)strdup(filename);
    new_item->Font     = NULL;
    new_item->Next     = NULL;
    
    if(LoadedFonts == NULL)
    {
        LoadedFonts=new_item;

    }
    else
    {
        SDL_FontList *l;

        l = LoadedFonts;

        while(l->Next)
        {
            if(!strcmp(l->FontId,fontid))
            {
                free(new_item);
                return;
            }
            l=l->Next;
        }
        l->Next=new_item;
    }
}
#endif

int SDL_FontDrawStringWindow(SDL_Window *window,SDL_Font *font,SDL_String *string,unsigned int color,SDL_Rect *rect)
{

    rect->x += window->Widget.Rect.x;
    rect->y += window->Widget.Rect.y;

    if(font->type >= 0 && font->type < LAST_FONT)
        return SDL_FontDrawString(window->surface,font,string,color,rect);
    return 0;
}

int SDL_FontDrawString(SDL_Surface *surface,SDL_Font *font,
                       SDL_String *string,unsigned int color,SDL_Rect *rect)
{
    char *p;
    int xoffset=0;
    int height;
    SDL_Rect pos;
    
    if(font == NULL || string == NULL)
        return 0;

    if(FontTable[font->type]->drawstring)
    {
        if(rect == NULL)
        {
            pos.x = 0;
            pos.y = 0;
            return FontTable[font->type]->drawstring(surface,font,string,color,&pos);
        }
        return FontTable[font->type]->drawstring(surface,font,string,color,rect);
    }
    
    font->color=color;

    height = SDL_FontGetHeight(font);

    p = string->Data;
    
    if(p == NULL)
        return 0;

    while(*p != 0)
    {

        if(rect)
        {
            if(SDL_FontGetCharWidth(font,*p) + xoffset > rect->w)
                return 0;
            pos.x=xoffset+rect->x;
            pos.y=rect->y;
            pos.w=rect->w;
            pos.h=rect->h;
            
        }
        else
        {
            pos.x=xoffset;
            pos.y=0;
            pos.w=0;
            pos.h=0;
        }
        xoffset+=SDL_FontDrawChar(surface,font,*p,&pos);
        p++;
    }
    return 1;
}




int SDL_FontDrawChar(SDL_Surface *dest,SDL_Font *font,
                     char character,SDL_Rect *pos)
{
    int width;
    T_Font_DrawChar DrawChar;
    
    if(font->type >= LAST_FONT)
        return 0;

    DrawChar = FontTable[font->type]->drawchar;
    
    width = DrawChar(dest,font,character,pos);

    return width;
}


int SDL_FontGetCharWidth(SDL_Font* font,char character)
{
    int width;
    int height;
    if(font->type ==FNT_FONT)
    {
        FNT_Font *fnt=(FNT_Font*)font->fontdata;
        if(fnt)
        {
            character -= (char)fnt->firstchar;
            return fnt->chartable[character*4+1]<<8|fnt->chartable[character*4];
        }
        else
        {
            return 19;
        }
    }
    else if(font->type == TTF_FONT)
    {
        FONT_TTF_CharInfo(font,character,&width,&height);
        return width;
    }
    else
    {
#if 0
        BDF_Font *fnt=(BDF_Font*)font->fontdata;
        width=fnt->bdffont[(int)character]->bbx_x;
        if(width == 0)
            return 9;
        else
            return width + 1;
#endif
    
    }
    return 19;
}



int SDL_FontGetStringWidth(SDL_Font* font,char *string)
{
    int length,width=0;
    int i;

    if(font == NULL || string == NULL)
        return 0;

    length=(int)strlen(string);
    for(i=0;i<length;i++)
        width+=SDL_FontGetCharWidth(font,string[i]);

    return width;

}

int SDL_FontGetHeight(SDL_Font *font)
{
    if(font != NULL)
        return font->Height;
    else
        return 0;
}

int SDL_FontSetHeight(SDL_Font *font,int height)
{
    if(font != NULL && font != &DefaultFont)
    {
        font->Height=height;

        return 1;
    }
    return 0;
}

#if 0
SDL_Font *SDL_FontGet(char *fontid)
{
    SDL_FontList *font;

    font=LoadedFonts;

    while(font && fontid)
    {
        if(!strcmp(fontid,font->FontId))
        {
            if(font->Font==NULL)
                font->Font=SDL_FontLoadData(font->Filename);
            return font->Font;
        }
        font=font->Next;
    }
    return NULL;
}
#endif

/* with font, which holds the height calc the rect */
int SDL_FontMetrics(SDL_Font* font,SDL_String *string,SDL_Rect *rect)
{

    return 0;
}

int SDL_FontCalcDims(SDL_Font* font,SDL_String *string,SDL_Rect *rect)
{
    int length,width=0;
    int height,width2;
    int yoffset=0;
    int i;

    if(font == NULL || string->Data == NULL)
        return 0;

    length=(int)strlen(string->Data);
    for(i=0;i<length;i++)
    {
        width+=SDL_FontGetCharWidth(font,string->Data[i]);

        width2=0;
        if(font->type == TTF_FONT)
        {
            FONT_TTF_CharInfo(font,string->Data[i],&width2,&height);
            if(height>yoffset)
                yoffset=height;
        }
    }

    rect->x = 0;
    rect->y = 0;
    if(yoffset)
        rect->h = yoffset;
    else
        rect->h = font->Height;
    rect->w = width;
    
    return 1;

}
