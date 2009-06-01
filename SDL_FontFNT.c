/*
  FNT font implementation
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
#include "SDL_FontFNT.h"


#include "SDL_GLPrimitives.h"

int  FONT_FNT_IsFNT(char *filename);
int  FONT_FNT_Read(char *filename,SDL_Font *font);
int  FONT_FNT_DrawChar(SDL_Surface *dest,SDL_Font *font,char character,SDL_Rect *pos);

void FNT_ReadFont(FILE *fp,SDL_Font *font);


extern void DrawPixel(SDL_Surface *screen, int x, int y,unsigned int color2);

int fastPixelColorNolock(SDL_Surface * dst, Sint16 x, Sint16 y, Uint32 color);

const struct S_Font_FunctionList FONT_FNT_FunctionList =
{
	NULL,
        FONT_FNT_IsFNT,
        FONT_FNT_Read,
        NULL,
        FONT_FNT_DrawChar
};


int FONT_FNT_IsFNT(char *filename)
{
    char *ext;
    ext=strrchr(filename,'.');

    if(!strcmp(".fnt",ext) || !strcmp(".FNT",ext))
        return 1;
    else
        return 0;
}


int FONT_FNT_Read(char *filename,SDL_Font *font)
{
    FILE *fp;
    fp=fopen(filename,"rb");

    if(fp == NULL)
    {
        return 0;
    }

    FNT_ReadFont(fp,font);
    font->type = FNT_FONT;
    fclose(fp);
    return 1;
}
 


int  FONT_FNT_DrawChar(SDL_Surface *dest,SDL_Font *font,char character,SDL_Rect *pos)
{
    int x1,y1;
    int i=0;
    int bit,newx,newy;
    int it=0;
    int start;

    int width;
    int xoffset=pos->x;
    int yoffset=pos->y;
    int which;

    FNT_Font *fnt=(FNT_Font*)font->fontdata;
    if(fnt == NULL)
        return 0;

    which = character - fnt->firstchar;


    width=fnt->chartable[which*4+1]<<8|fnt->chartable[which*4];

    if(which < 0)
        return 0;

    start=fnt->chartable[which*4+3]<<8|fnt->chartable[which*4+2];
    start-=fnt->offset;
    
    bit=fnt->font[start];

    /* run through all bytes (from left to right) */
    for(it=0;it<3;it++)
    {
        /*run through the font height */
        for(y1=0;y1<font->Height;y1++)
        {
            /*run through the current byte */
            for(x1=0;x1<8;x1++)
            {
                newx=x1+it*8;
                newy=y1;
                /* if we reach the current width we are finished */
                if(newx>=width)
                    break;

                if(bit & 0x80)
                {
#if 0
                    if(clip && (((newx+xoffset) < clip->x) || ((newx+xoffset+1) > (clip->x +clip->w))))
                        ;
                    else
#endif
                    {
                        if((pos->y + pos->h) >= y1+yoffset)
                        {
                            if(dest->flags & SDL_OPENGL)
                            {
                                /* SDL_GLPoint(dest,newx+xoffset,y1+yoffset, font->color); */
                            }
                            else
                            {
                                fastPixelColorNolock(dest,newx+xoffset,y1+yoffset,font->color);
                            }
                        }
                    }
                }
                bit=bit<<1;
            }
            i++;
            bit=fnt->font[i+start];
        }
    }
    return width;
}




void FNT_ReadFont(FILE *fp,SDL_Font *font)
{
    unsigned char buf[2000];    
    int height;
    int bytes;

    FNT_Font *fnt=malloc(sizeof(FNT_Font));
    memset(fnt,0,sizeof(FNT_Font));
    memset(buf,0,2000);

    fread(buf,2,1,fp); /*/version                         2 */
    fread(buf,4,1,fp); /*/size                            6  */
    fread(buf,60,1,fp);/*/copyright information          66 */
    fread(buf,2,1,fp);/*/font type                       68 */
    fread(buf,2,1,fp);/*/point size                      70 */
    fread(buf,2,1,fp);/*/vert res                        72 */
    fread(buf,2,1,fp);/*/Hor res                         74 */
    fread(buf,2,1,fp);/*/ascent                          76 */
    fread(buf,2,1,fp);/*/internal leading                78 */
    fread(buf,2,1,fp);/*/external leading                80 */
    fread(buf,1,1,fp);/*/italic                          81 */
    fread(buf,1,1,fp);/*/underline                       82 */
    fread(buf,1,1,fp);/*/strike out                      83 */
    fread(buf,2,1,fp);/*/ weight                         85 */
    fread(buf,1,1,fp);/*/ character set                  86 */
    fread(buf,2,1,fp);/*/ pix width (for vector fonts)   88 */
    fread(buf,2,1,fp);/*/ pix height (for raster fonts)  90 */
    height=buf[0];
    fread(buf,1,1,fp);/*/ pitch and family               91 */
    fread(buf,2,1,fp);/*/ average width                  93 */
    fread(buf,2,1,fp);/*/ max width                      95 */
    fread(buf,1,1,fp);/*/ first char                     96 */
    fnt->firstchar=buf[0];
    fread(buf,1,1,fp);/*/ last char                      97    */   
    fnt->lastchar=buf[0];
    fread(buf,1,1,fp);/*/ default char                   98 */
    fread(buf,1,1,fp);/*/ break char                     99 */
    fread(buf,2,1,fp);/*/ width bytes                    101 */
    bytes=buf[1] | buf[0];
    fread(buf,4,1,fp);/*/ offset to device name          105 */
    fread(buf,4,1,fp);/*/ offset to face name            109 */
    fread(buf,4,1,fp);/*/ bits pointer                   113 */
    fread(buf,4,1,fp);/*/ offset to bitmap data          117 */
    fnt->offset=(int)(buf[0]|(buf[1]<<8));
    fnt->font=(unsigned char *)(buf[1]<<8|buf[0]);    
    

    fread(buf,1,1,fp);/*/ reserved                       118 */
   
    {
        int mallocsize=(int)((unsigned char*)fnt->font - (unsigned char*)118);
       
        if(mallocsize > 0)
        {
            fnt->chartable = malloc(mallocsize+1);
            fread(fnt->chartable,1,mallocsize,fp);    
        }
        else
        {
            printf("Error in file\n");
            fnt->offset = 0;
            fnt->font   = NULL;
            return;
        }
    }
    
    fseek(fp,(long)fnt->offset,SEEK_SET);
    {
        int mallocsize=bytes* height *(fnt->lastchar - fnt->firstchar);
        fnt->font      = malloc(mallocsize+1);
        fread(fnt->font, 1,mallocsize,fp);    
    }

    font->fontdata = fnt;
    font->Height   = height;
}
