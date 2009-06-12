/*
  SDLTk

  XP style implementation
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


static SDL_Style *SDL_StyleXP;

static void SDL_StyleXPPaintScrollbar(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);


int SDL_StyleXPInit()
{
    SDL_StyleXP = SDL_StyleNew();
    
    SDL_StyleSetDrawFunction(SDL_StyleXP,"SDL_Scrollbar",SDL_StyleXPPaintScrollbar);
    return 1;
}



int SDL_StyleXPSet()
{
    return SDL_StylePush(SDL_StyleXP);
}

static void SDL_StyleXPPaintScrollbar(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
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
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,249,248,244));
       
        R.x = 0;
        R.y = Scrollbar->PixelValue;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,186,209,252));


        x      = 0;
        y      = Scrollbar->PixelValue;
        width  = widget->Rect.w;
        height = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;

        if(height < 6)
            height = 6;

        /* bottomest line */
        SDL_DrawLine (surface, x + 2, y + height - 1, x + width - 3, y + height - 1,0x9fb5D2); /* light BLUE */
        /* bottom line */
        SDL_DrawLine (surface, x + 1, y + height - 2, x + width - 2, y + height - 2,0xffffffff); /* WHITE */

        /* most right line */
        SDL_DrawLine (surface, x + width - 1, y + 1, x + width - 1, y + height - 2,0x7C9FD3ff); /* darker blue */
        /* right line */
        SDL_DrawLine (surface, x + width - 2, y + 1, x + width - 2, y + height - 2,0xffffffff); /* white */

        /* most upper line */
        SDL_DrawLine (surface, x, y, x + width - 1, y,0xffffffff); /* white */
        /* upper line */
        SDL_DrawLine (surface, x + 1, y + 1, x + width - 2, y + 1,0xb8cbf6ff); /* light blue */

        /* most left line */
        SDL_DrawLine (surface, x, y, x, y + height - 1,0xffffffff); /* white */              
        /* left line */
        SDL_DrawLine (surface, x + 1, y + 1, x + 1, y + height - 2,0xb8cbf6ff);
        
        /* the grip */
        {
            int i;
            int grip_width = 6;
            int grip_x = x + (width  / 2 - 5);
            int grip_y = y + (height / 2 - 8);

            for(i=0;i<4;i++)
            {
                SDL_DrawLine (surface, grip_x    , grip_y, grip_x + grip_width,      grip_y,0xffffffff); /* base */
                grip_y++;
                SDL_DrawLine (surface, grip_x + 1, grip_y, grip_x + 1 + grip_width,  grip_y,0x8cb0f8ff); /* shadow */
                grip_y++;
            }
        }
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
            SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,249,248,244));
       
            R.x = Scrollbar->PixelValue;
            R.y = 0;
            R.w = widget->Rect.w * Scrollbar->NormalStepValue / Scrollbar->MaxValue;
            R.h = widget->Rect.h;
            SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,186,209,252));


            x      = Scrollbar->PixelValue;
            y      = 0;
            width  = widget->Rect.w * Scrollbar->NormalStepValue / Scrollbar->MaxValue;
            height = widget->Rect.h;

            if(width < 6)
                width = 6;

            /* bottomest line */
            SDL_DrawLine (surface, x + 2, y + height - 1, x + width - 3, y + height - 1,0x9fb5D2); /* light BLUE */
            /* bottom line */
            SDL_DrawLine (surface, x + 1, y + height - 2, x + width - 2, y + height - 2,0xffffffff); /* WHITE */

            /* most right line */
            SDL_DrawLine (surface, x + width - 1, y + 1, x + width - 1, y + height - 2,0x7C9FD3ff); /* darker blue */
            /* right line */
            SDL_DrawLine (surface, x + width - 2, y + 1, x + width - 2, y + height - 2,0xffffffff); /* white */

            /* most upper line */
            SDL_DrawLine (surface, x, y, x + width - 1, y,0xffffffff); /* white */
            /* upper line */
            SDL_DrawLine (surface, x + 1, y + 1, x + width - 2, y + 1,0xb8cbf6ff); /* light blue */

            /* most left line */
            SDL_DrawLine (surface, x, y, x, y + height - 1,0xffffffff); /* white */              
            /* left line */
            SDL_DrawLine (surface, x + 1, y + 1, x + 1, y + height - 2,0xb8cbf6ff);
        
            /* the grip */
            {
                int i;
                int grip_height = 6;
                int grip_x = x + (width  / 2 - 8);
                int grip_y = y + (height / 2 - 5);

                for(i=0;i<4;i++)
                {
                    SDL_DrawLine (surface, grip_x ,grip_y, grip_x, grip_y + grip_height,0xffffffff); /* base */
                    grip_x++;
                    SDL_DrawLine (surface, grip_x ,grip_y + 1,grip_x,grip_y + 1 + grip_height,0x8cb0f8ff); /* shadow */
                    grip_x++;
                }
            }
        }
}
