/*
  SDLTk

  Frame widget
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
#include <string.h>

#include "SDLTk.h"
#include "SDL_Frame.h"


static void SDL_FrameDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);


SDL_WidgetType SDL_FrameGetType()
{
    SDL_WidgetType Frame;
    
    Frame=SDL_WidgetTypeGet("SDL_Frame");
    
    if(Frame == 0)
    {
        SDL_WidgetFunctions *func;

        Frame=SDL_WidgetTypeRegister("SDL_Frame",&func);
        func->draw        = SDL_FrameDraw;
    }
    return Frame;
}



SDL_Widget* SDL_FrameNew()
{
    SDL_Widget *widget;
    SDL_Frame  *Frame;
    
    Frame=(SDL_Frame*) malloc(sizeof(SDL_Frame));
    memset(Frame,0,sizeof(SDL_Frame));

    widget=(SDL_Widget*)Frame;

    widget->Type      = SDL_FrameGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;
    
    Frame->color  = WHITE;
    
    return (SDL_Widget*)Frame;
}

static void SDL_FrameDraw(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Frame *Frame=(SDL_Frame*)Widget;
    SDL_Rect dst;

#if 0
    SDL_FillRect(surface,0x000000,NULL);
#endif
    if(Frame->TitleLeft)
    {

        dst.x = 0;
        dst.y = 0;
        dst.w = Frame->TitleLeft->surface->w;
        dst.h = Frame->TitleLeft->surface->h;

        SDL_ImageBlitToSurface(Frame->TitleLeft,surface,&dst);

    }
    if(Frame->TitleRight)
    {
        dst.x = Widget->Rect.w - Frame->TitleRight->surface->w;
        dst.y = 0;
        dst.w = Frame->TitleRight->surface->w;
        dst.h = Frame->TitleRight->surface->h;

        SDL_ImageBlitToSurface(Frame->TitleRight,surface,&dst);
    }


    if(Frame->TitleCenter)
    {
        int width;

        width = Widget->Rect.w;
        dst.x = 0;
        if(Frame->TitleRight)
            width -= Frame->TitleRight->surface->w;

        if(Frame->TitleLeft)
        {
            dst.x  = Frame->TitleLeft->surface->w;
            width -= Frame->TitleLeft->surface->w;
        }
        else
        {

        }
			
        dst.y = 0;
        dst.w = width;
        dst.h = Frame->TitleCenter->surface->h;

        SDL_ImageBlitToSurface(Frame->TitleCenter,surface,&dst);

    }

    if(Frame->Left)
    {
        int height;

        height = Widget->Rect.h;
        if(Frame->TitleLeft)
        {
            height -= Frame->TitleLeft->surface->h;
            dst.y  =  Frame->TitleLeft->surface->h;
    	}
        else
        {
            dst.y = 0;
        }
			
        dst.x = Widget->Rect.x;
        dst.w = Frame->Left->surface->w;
        dst.h = height;
		
        SDL_ImageBlitToSurface(Frame->Left,surface,&dst);

    }

    if(Frame->Right)
    {
        int height;

        height = Widget->Rect.h;
        if(Frame->TitleRight)
        {
            height -= Frame->TitleRight->surface->h;
            dst.y  =  Frame->TitleRight->surface->h;
    	}
        else
        {
            dst.y = 0;
        }
			
        dst.x = Widget->Rect.w - Frame->Right->surface->w;
        dst.w = Frame->Right->surface->w;
        dst.h = height;

        SDL_ImageBlitToSurface(Frame->Right,surface,&dst);
    }

    if(Frame->BottomLeft)
    {
        dst.x = 0;
        dst.y = Widget->Rect.h - Frame->BottomLeft->surface->h;
        dst.w = Frame->BottomLeft->surface->w;
        dst.h = Frame->BottomLeft->surface->h;

        SDL_ImageBlitToSurface(Frame->BottomLeft,surface,&dst);
    }

	
    if(Frame->BottomRight)
    {
        dst.x = Widget->Rect.w - Frame->BottomRight->surface->w;
        dst.y = Widget->Rect.h - Frame->BottomRight->surface->h;
        dst.w = Frame->BottomRight->surface->w;
        dst.h = Frame->BottomRight->surface->h;

        SDL_ImageBlitToSurface(Frame->BottomRight,surface,&dst);
    }

    if(Frame->Bottom)
    {
        int width;

        width = Widget->Rect.w;
        if(Frame->BottomRight)
            width -= Frame->BottomRight->surface->w;
        if(Frame->BottomLeft)
        {
            dst.x  = Frame->BottomLeft->surface->w;
            width -= Frame->BottomLeft->surface->w;
        }
        else
        {
            dst.x = 0;
        }
			
        dst.y = Widget->Rect.h - Frame->Bottom->surface->h;
        dst.w = width;
        dst.h = Frame->Bottom->surface->h;
	
        SDL_ImageBlitToSurface(Frame->Bottom,surface,&dst);
    }
}


int SDL_FrameSetBackgroundColor(SDL_Widget *widget,Uint32 color)
{
    SDL_WidgetRedrawEvent(widget);
    return 1;
}

int SDL_FrameSetImage(SDL_Widget *widget,int which,SDL_Image *image)
{
    SDL_Frame *Frame=(SDL_Frame*)widget;


    if(widget==NULL || image== NULL)
        return 0;
        
    switch(which)
    {
    case FRAME_TITLE_CENTER:
        Frame->TitleCenter = image;
        break;
    case FRAME_TITLE_LEFT:
        Frame->TitleLeft   = image;
        break;
    case FRAME_TITLE_RIGHT:
        Frame->TitleRight  = image;
        break;
    case FRAME_LEFT:
        Frame->Left        = image;
        break;
    case FRAME_RIGHT:
        Frame->Right       = image;
        break;
    case FRAME_BOTTOM_LEFT:
        Frame->BottomLeft  = image;
        break;
    case FRAME_BOTTOM_RIGHT:
        Frame->BottomRight = image;
        break;
    case FRAME_BOTTOM:
        Frame->Bottom      = image;
        break;
    }
    return 1;

}
