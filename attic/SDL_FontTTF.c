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
#include <string.h>
#include <malloc.h>

#include "SDLTk.h"
#include "SDL_FontTTF.h"
#include <ft2build.h>
#include FT_FREETYPE_H


void FONT_TTF_Init();
int  FONT_TTF_IsTTF(char *filename);
int  FONT_TTF_Read(char *filename,SDL_Font *font);
int  FONT_TTF_DrawString(SDL_Surface *screen,SDL_Font *font,SDL_String *string,unsigned int color,SDL_Rect *pos);
int  FONT_TTF_DrawChar(SDL_Surface *dest,SDL_Font *font,char character,SDL_Rect *pos);



	
FT_Library library;



extern void DrawPixel(SDL_Surface *screen, int x, int y,unsigned int color2);

int fastPixelColorNolock(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color);

const struct S_Font_FunctionList FONT_TTF_FunctionList =
{
    FONT_TTF_Init,
    FONT_TTF_IsTTF,
    FONT_TTF_Read,
    FONT_TTF_DrawString,
    FONT_TTF_DrawChar
};

void FONT_TTF_Init()
{
    int error = FT_Init_FreeType(&library);
    if(error)
    {
        printf("Could not init freetyep\n");
    }
}

int FONT_TTF_IsTTF(char *filename)
{
    char *ext;
    ext=strrchr(filename,'.');

    return 1;
    if(!strcmp(".ttf",ext) || !strcmp(".TTF",ext)|| !strcmp(".fon",ext))
        return 1;
    else
        return 0;
}


int FONT_TTF_Read(char *filename,SDL_Font *font)
{
    int error;
    FT_Face    face;


    error = FT_New_Face(library,filename,0,&face);
    if(error) 
    {
        printf("Something wrong with your ttf file %s:%d\n",filename,error);
        return 0;
    }

    font->type		 = TTF_FONT;
    if(face->available_sizes)
        font->Height	 = face->available_sizes->height;
    else
        font->Height = 20;

    font->fontdata   = face;
    return 1;
}

static void drawbitmap(SDL_Surface *screen,FT_Bitmap *bitmap,FT_Int x, FT_Int y,int color)
{
    FT_Int  i, j, p, q;
    FT_Int  x_max;
    FT_Int  y_max;

    x_max = x + bitmap->width;
    y_max = y + bitmap->rows;

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if(bitmap->pixel_mode > 1)
            {
                if(bitmap->buffer[q * bitmap->width + p])
                {
                    if(bitmap->buffer[q * bitmap->width + p] > 128)
                    {
                        fastPixelColorNolock(screen,i,j,bitmap->buffer[q * bitmap->width + p]);
                    }
                }

            }
            else
            { 
                int byte =(q * bitmap->pitch)+p/8 ;
	
                if((bitmap->buffer[byte]) & (0x80 >> p))
                {
                    fastPixelColorNolock(screen,i,j,color);
                    
                }
            }

        }
    }
}
 
int  FONT_TTF_DrawString(SDL_Surface *screen,SDL_Font *font,SDL_String *string,unsigned int color,SDL_Rect *pos)
{
    FT_Face    face=font->fontdata;
    int error;
    int x= pos->x;
    int y= pos->y;
    unsigned int n;

    FT_GlyphSlot  slot;    
    FT_Bitmap*  bitmap;
    int use_kerning;
    int max_height = 0;

    slot  = face->glyph;    
    bitmap = &slot->bitmap;
    use_kerning = FT_HAS_KERNING(face);

    error = FT_Set_Char_Size(face,0,font->Height*64,100,100);

    pos->h = 0;
    if(pos->h > 0)
    {
        max_height = pos->h;
    }
    else
    {
        for(n=0;n<string->Length;n++)
        {
            error = FT_Load_Char( face, string->Data[n], FT_LOAD_RENDER );
            error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
            if(slot->bitmap_top > max_height)
                max_height = slot->bitmap_top;
        }     
    }
    for(n=0;n<string->Length;n++)
    {
        error = FT_Load_Char( face, string->Data[n], FT_LOAD_RENDER );
        error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
        drawbitmap(screen,&slot->bitmap,x +slot->bitmap_left, max_height + y - slot->bitmap_top,font->color);

        x += slot->advance.x >> 6;
        y += slot->advance.y >> 6;

    }
    return 1;
}




int FONT_TTF_DrawChar(SDL_Surface *dest,SDL_Font *font,char charcode,SDL_Rect *pos)
{
    FT_Face    face=font->fontdata;
    int error;
    FT_Int  i, j, p, q;
    int x= pos->x;
    int y= pos->y;

    FT_GlyphSlot  slot;    
    FT_Bitmap*  bitmap;
    int use_kerning;
    FT_Int  x_max;
    FT_Int  y_max;

    error = FT_Set_Char_Size(face,0,font->Height*64,100,100);
    error = FT_Load_Char( face, charcode, FT_LOAD_RENDER );

    slot  = face->glyph;    
    bitmap = &slot->bitmap;
    use_kerning = FT_HAS_KERNING(face);

    error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
   
    y -= slot->bitmap_top;
    x += slot->bitmap_left;
    y_max = y + bitmap->rows;
    x_max = x + bitmap->width;

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if(bitmap->pixel_mode > 1)
            {
                if(bitmap->buffer[q * bitmap->width + p])
                {
                    if(bitmap->buffer[q * bitmap->width + p] > 128)
                    {
                        fastPixelColorNolock(dest,i,j,bitmap->buffer[q * bitmap->width + p]);
                    }
                }

            }
            else
            { 
                int byte =(q * bitmap->pitch)+p/8 ;
	
                if((bitmap->buffer[byte]) & (0x80 >> p))
                {
                    fastPixelColorNolock(dest,i,j,font->color);
                    
                }
            }

        }
    }
	
    return (slot->advance.x >> 6);
}




int FONT_TTF_CharInfo(SDL_Font *font,char charcode,int *w,int *h)
{
    FT_Face    face=font->fontdata;

    int error = FT_Set_Char_Size(face,0,font->Height*64,100,100);

    
    FT_GlyphSlot  slot;    
    FT_Bitmap*  bitmap;
    int use_kerning;

    FT_Load_Char( face, charcode, FT_LOAD_RENDER );
    slot  = face->glyph;    
    bitmap = &slot->bitmap;
    use_kerning = FT_HAS_KERNING(face);

    error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
   
    *w = slot->advance.x >> 6;
    *h = bitmap->rows + (bitmap->rows - slot->bitmap_top);
    return 1;
}

