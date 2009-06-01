/*
  SDLTk

  Default style implementation
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

#include "SDLTk.h"

static void SDL_StyleDefaultPaintButton(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_StyleDefaultPaintRadioButton(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_StyleDefaultPaintScrollbar(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);


static SDL_Style *SDL_StyleDefault;

int SDL_StyleDefaultInit()
{
    SDL_StyleDefault = SDL_StyleNew();
 
    SDL_StyleSetDrawFunction(SDL_StyleDefault,"SDL_Button",              SDL_StyleDefaultPaintButton);
    SDL_StyleSetDrawFunction(SDL_StyleDefault,"SDL_RadioButton",         SDL_StyleDefaultPaintRadioButton);
    SDL_StyleSetDrawFunction(SDL_StyleDefault,"SDL_HorizontalScrollbar", SDL_StyleDefaultPaintScrollbar);
    SDL_StyleSetDrawFunction(SDL_StyleDefault,"SDL_VerticalScrollbar",   SDL_StyleDefaultPaintScrollbar);
    return 1;
}

int SDL_StyleDefaultSet()
{
    return SDL_StylePush(SDL_StyleDefault);
}


static void SDL_StyleDefaultPaintButton(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Button *Button=(SDL_Button*)Widget;
    SDL_Rect r;
    short x,y,width,height;
    r.x = 0;
    r.y = 0; 
    r.w = Widget->Rect.w;
    r.h = Widget->Rect.h ;

    x=0;
    y=0;
    width  = r.w;
    height = r.h;
    
    SDL_FillRect(surface,&r,SDL_MapRGB(surface->format,0xd4,0xd0,0xc8));

    {
        /* normal horitontal */
        SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1,0x404040ff);
        SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1,0x404040ff);

        SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2,0x808080ff);
        SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2,0x808080ff);

        SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1,0xffffffff);
        SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2,0xffffffff);

        SDL_DrawLine (surface,x, y, x + width - 1, y,0xd4d0c8ff);
        SDL_DrawLine (surface,x, y, x, y + height - 1,0xd4d0c8ff);       
    }
    
    if(Button->State == SDL_BUTTON_STATE_DOWN)
    {
        SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1, 0xffffffff);
        SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1, 0xffffffff);

        SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2, 0xd4d0c8ff);
        SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2, 0xd4d0c8ff);
      
        SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1, 0x808080ff);
        SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2, 0x808080ff);
      
        SDL_DrawLine (surface,x, y, x + width - 1, y, 0x404040ff);
        SDL_DrawLine (surface,x, y, x, y + height - 1, 0x404040ff);

    }
}


static void SDL_StyleDefaultPaintRadioButton(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_RadioButton *RadioButton=(SDL_RadioButton*)widget;

    if(RadioButton->State)
    {
        SDL_DrawCircle(surface,widget->Rect.h / 2, 8, 5, 0x666666ff);
        SDL_DrawCircle(surface,widget->Rect.h / 2, 8, 4, 0xffffffff);
        SDL_DrawCircle(surface,widget->Rect.h / 2, 8, 2, 0x000000ff);
    }
    else
    {
        SDL_DrawCircle(surface,widget->Rect.h / 2, 8, 5, 0x666666ff);
        SDL_DrawCircle(surface,widget->Rect.h / 2, 8, 4, 0xffffffff);
    }
 
}




static void SDL_StyleDefaultPaintScrollbar(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    /* drawing order is always background, line , button */
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;
    SDL_Rect R;
    int x,y,height,width;

    if(Scrollbar->Orientation == VERTICAL)
    {
        R.x = 0;
        R.y = 0;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xf4,0xf0,0xe8));

        R.x = 0;
        R.y = Scrollbar->PixelValue;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;
        
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xd4,0xd0,0xc8));

        x      = 0;
        y      = Scrollbar->PixelValue;
        width  = widget->Rect.w;
        height = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;

        if(height < 6)
            height = 6;
            
        SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1,0x404040ff);
        SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1,0x404040ff);
        
        SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2,0x808080ff);
        SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2,0x808080ff);
        
        SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1,0xffffffff);
        SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2,0xffffffff);
        
        SDL_DrawLine (surface,x, y, x + width - 1, y,0xd4d0c8ff);
        SDL_DrawLine (surface,x, y, x, y + height - 1,0xd4d0c8ff);       

    }
    else
    {

        /* drawing order is always background, line , button */
        SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;
        SDL_Rect R;
        int x,y,height,width;

        R.x = 0;
        R.y = 0;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xf4,0xf0,0xe8));
       
        R.x = Scrollbar->PixelValue;
        R.y = 0;
        R.w = widget->Rect.w/Scrollbar->MaxValue;
        R.h = widget->Rect.h;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xd4,0xd0,0xc8));

        x      = Scrollbar->PixelValue;
        y      = 0;
        width  = widget->Rect.w * Scrollbar->NormalStepValue / Scrollbar->MaxValue;
        height = widget->Rect.h;

        if(width < 6)
            width = 6;
        
        SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1,0x404040ff);
        SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1,0x404040ff);
        
        SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2,0x808080ff);
        SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2,0x808080ff);
        
        SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1,0xffffffff);
        SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2,0xffffffff);
        
        SDL_DrawLine (surface,x, y, x + width - 1, y,0xd4d0c8ff);
        SDL_DrawLine (surface,x, y, x, y + height - 1,0xd4d0c8ff);       


    }
}

    
