/*
  SDLTk

  Volumebar implementation 
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
#include <stdarg.h>
#include <malloc.h>

#include "SDL_Widget.h"
#include "SDL_VolumeBar.h"
#include "SDL_WidgetType.h"

/* Prototypes for Internal functions */
void VolumeBar_DrawStripe(SDL_Surface *dest,SDL_VolumeBar *VolumeBar,Sint16 line,Uint32 color);

static void SDL_VolumeBarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_VolumeBarGetType()
{
    SDL_WidgetType volumebar;

    volumebar = SDL_WidgetTypeGet("SDL_VolumeBar");
    if(volumebar == 0)
    {
        SDL_WidgetFunctions *func;
        volumebar=SDL_WidgetTypeRegister("SDL_VolumeBar",&func);

        func->draw = SDL_VolumeBarDraw;

    }
    return volumebar;
}

SDL_Widget* SDL_VolumeBarNew()
{
    SDL_VolumeBar *volumebar;
    SDL_Widget    *widget;

    volumebar=(SDL_VolumeBar*)malloc(sizeof(SDL_VolumeBar));
    widget   =(SDL_Widget*)volumebar;

    widget->Type = SDL_VolumeBarGetType();
    widget->Rect.x  = 0;
    widget->Rect.y  = 0;
    widget->Rect.w  = 0;
    widget->Rect.h  = 0;

    volumebar->color1  = 0x790000;
    volumebar->color2  = 0xfa0000;
    volumebar->color3  = 0x007c00;
    volumebar->color4  = 0x00fe00;

    /* Default range settings */
    volumebar->MaxValue      = 100;
    volumebar->MinValue      = 0;
    volumebar->CriticalValue = 70;
    volumebar->CurrentValue  = 0;

    /* Used for drawing */
    volumebar->CriticalLine  = (volumebar->CriticalValue * widget->Rect.h) / volumebar->MaxValue;
    volumebar->CurrentLine   = 0;


    return (SDL_Widget*)volumebar;
}

static void SDL_VolumeBarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_VolumeBar *VolumeBar=(SDL_VolumeBar*)widget;
    int line;
    int highlight=0;
    
    VolumeBar->CriticalLine  = (VolumeBar->CriticalValue * widget->Rect.h) / VolumeBar->MaxValue;
    SDL_FillRect(surface,NULL,0x000000);

    for( line=0; line < widget->Rect.h ; line++)
    {
        highlight= (line < VolumeBar->CurrentLine);

        if(line % 2  != 0)
        {
            if(line > VolumeBar->CriticalLine)
            {
                if(highlight)
                    VolumeBar_DrawStripe(surface,VolumeBar,line,VolumeBar->color2);
                else
                    VolumeBar_DrawStripe(surface,VolumeBar,line,VolumeBar->color1);
            }
            else
            {
                if(highlight)
                    VolumeBar_DrawStripe(surface,VolumeBar,line,VolumeBar->color4);
                else
                    VolumeBar_DrawStripe(surface,VolumeBar,line,VolumeBar->color3);
            }
        }
    }

    Area = Area;
}

int SDL_VolumeBarSetMaxValue(SDL_Widget *Widget,int max)
{
    SDL_VolumeBar *VolumeBar=(SDL_VolumeBar*)Widget;
    
    if(VolumeBar == NULL)
        return 0;

    VolumeBar->MaxValue = max;

    return 1;
}

int SDL_VolumeBarSetMinValue(SDL_Widget *Widget,int min)
{
    SDL_VolumeBar *VolumeBar=(SDL_VolumeBar*)Widget;

    if(VolumeBar == NULL)
        return 0;

    VolumeBar->MinValue = min;

    return 1;
}


int SDL_VolumeBarSetCurrentValue(SDL_Widget *Widget,int value)
{
    SDL_VolumeBar *VolumeBar=(SDL_VolumeBar*)Widget;

    if(VolumeBar == NULL)
        return 0;
    if(value != VolumeBar->CurrentValue)
    {
        VolumeBar->CurrentValue = value; 
        VolumeBar->CurrentLine  = (value * Widget->Rect.h) / (VolumeBar->MaxValue - VolumeBar->MinValue); 
        SDL_WidgetRedrawEvent(Widget);
    }
    return 1;
}

/* Internal Helper functions */


void VolumeBar_DrawStripe(SDL_Surface *dest,SDL_VolumeBar *VolumeBar,Sint16 line,Uint32 color)
{
    SDL_Widget *widget=(SDL_Widget*)VolumeBar;
    SDL_Rect rect;

    rect.x = 0;
    rect.y = widget->Rect.h - line;
    rect.w = widget->Rect.w;
    rect.h = 1;
    
    SDL_FillRect(dest,&rect,color);

}

