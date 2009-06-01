/*
  SDLTk

  Image handling
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
#include "SDL_Images.h"
#include "SDL_ImageGIF.h"
#include "SDL_ImageFrame.h"


void SDL_ImagePostEvent(SDL_Image *image)
{
    SDL_Event event;
    
    event.type		     = SDLTK_EVENT;
    event.user.code      = SDLTK_IMAGE_EVENT;
    event.user.data1     = image;
    event.user.data2     = 0;
    
    SDL_PushEvent(&event);
}

void SDL_ImageEvent(SDL_Event *Event)
{
    SDL_Image *image = (SDL_Image*)Event->user.data1;
    if(image->Transition && image->Transition->Client)
        SDL_SignalObjectEmit((SDL_Object*)image->Transition->Client,"frame-update",image);
    else
        SDL_SignalObjectEmit((SDL_Object*)image,"frame-update",image);
}	

static Uint32 SDL_ImageLoadNextFrame(Uint32 interval, void *param)
{
    SDL_Image   *image = (SDL_Image *)param;
    SDL_ImageFrame *frame = SDL_ImageFrameLoad(image->filename,++image->cur_frame);

    if(frame == NULL) 
    {
        image->cur_frame = 1;
        frame = SDL_ImageFrameLoad(image->filename,image->cur_frame);
    }

    /* if the frame is available (out of memory, file handle scenario) */
    if(frame)
    {
        SDL_FreeSurface(image->surface);
        image->surface    = frame->surface;
        interval          = frame->delay;
        SDL_ImagePostEvent(image);
        free(frame);
        return interval;
    }
    return 0;
}



SDL_Image *SDL_ImageLoad(char *filename, SDL_Rect *selection)
{
    SDL_Surface *surface;
    SDL_Image   *image = NULL;

    SDL_ImageFrame *frame = SDL_ImageFrameLoad(filename,1);
       
    if(frame)
    {
        surface = frame->surface;

        image=(SDL_Image *)malloc(sizeof(SDL_Image));
        memset(image,0,sizeof(SDL_Image));

        image->framecount = 1;
        image->cur_frame = 1;
        image->filename = filename;

        if(selection && selection->w > 0 && selection->h > 0)
        {
            image->surface=SDL_CreateRGBSurface(SDL_HWSURFACE,selection->w,selection->h,32,0xff00000,0x00ff0000,
                                                0x0000ff00,0x000000ff);


            image->surface=SDL_DisplayFormat(image->surface);


            SDL_BlitSurface(surface,selection,image->surface,NULL);
            SDL_FreeSurface(surface);

            image->selection.x = selection->x;
            image->selection.y = selection->y;
            image->selection.w = selection->w;
            image->selection.h = selection->h;

        }
        else
        {
            image->surface=surface;
        }
        if(frame->delay)
            SDL_AddTimer(frame->delay,SDL_ImageLoadNextFrame,image);

    }
    free(frame);
    return image;
}


void SDL_ImageTransitionSurface(SDL_ImageTransition *Transition)
{
    if(Transition)
    {
        if(Transition->ResultSurface == NULL)
        {
            Transition->ResultSurface = SDL_CreateRGBSurface(SDL_SRCALPHA|SDL_SWSURFACE,Transition->FromSurface->w,
                                                             Transition->FromSurface->h,Transition->FromSurface->format->BitsPerPixel,
                                                             Transition->FromSurface->format->Rmask,
                                                             Transition->FromSurface->format->Gmask,
                                                             Transition->FromSurface->format->Bmask, 
                                                             Transition->FromSurface->format->Amask);

        }
        SDL_SetAlpha(Transition->FromSurface,SDL_SRCALPHA,(Uint8)255-Transition->alpha);
        SDL_SetAlpha(Transition->ToSurface,  SDL_SRCALPHA,(Uint8)Transition->alpha);
        SDL_BlitSurface(Transition->FromSurface,NULL, Transition->ResultSurface,NULL);
        SDL_BlitSurface(Transition->ToSurface,NULL, Transition->ResultSurface,NULL);
        Transition->alpha += 2;		
        SDL_SetAlpha(Transition->FromSurface,SDL_SRCALPHA,(Uint8)255);
        SDL_SetAlpha(Transition->ToSurface,  SDL_SRCALPHA,(Uint8)255);
		
    }

}

static Uint32 SDL_ImageNextFrame(Uint32 interval, void *param)
{
    SDL_Image   *image = (SDL_Image*)param;
	
    Uint32 ui_curr_time;


    ui_curr_time = SDL_GetTicks();
    image->old_time  = ui_curr_time;
    
    if(image->Transition)
    {
        SDL_ImageTransitionSurface(image->Transition);
    }


    SDL_ImagePostEvent(image);
    if(image->Transition && image->Transition->delay && image->Transition->alpha < 255)
        return image->Transition->delay;

    if(image->Transition)
        image->Transition->alpha = 0;
    image->Transition   = NULL;
    return 0;
}

int SDL_ImageBlitToSurface(SDL_Image *image,const SDL_Rect *srcrect,SDL_Surface *surface,const SDL_Rect *destrect)
{
    SDL_Rect src;
    SDL_Rect dest;

    int width,height;
    int dest_x,y,w,h;

    int oldy;
    int oldx;
    int oldw;

    SDL_Surface *img_surface;

    if(destrect)
    {
        dest.x = destrect->x;
        dest.y = destrect->y;
        dest.w = destrect->w;
        dest.h = destrect->h;

        oldy = destrect->y;
        oldx = destrect->x;
        oldw = destrect->w;
    }
    else
    {
        dest.x = 0;
        dest.y = 0;
        dest.w = image->surface->w;
        dest.h = image->surface->h;
        oldy = 0;
        oldx = 0;
        oldw = image->surface->w;

    }


    if(image == NULL)
        return 0;

    if(image->Transition && image->Transition->ResultSurface)
        img_surface = image->Transition->ResultSurface;
    else
        img_surface = image->surface;		


    if(srcrect)
    {
        src.x = srcrect->x;
        src.y = srcrect->y;
        src.w = srcrect->w;
        src.h = srcrect->h;
        SDL_BlitSurface(img_surface,&src,surface,&dest);
        return 1;
    }

    w = width  = dest.w;
    h = height = dest.h;

    dest_x = dest.x;
    y = dest.y;

    /* draw the left part, untiled */
    if(image->margin.left >0)
    {
        src.x = 0;
        src.y = 0;
        src.w = image->margin.left;
        src.h = img_surface->h;
        if(src.h > dest.h)
            src.h = dest.h;
        
        SDL_BlitSurface(img_surface,&src,surface,&dest);

    }

    dest.x = oldx + image->margin.left;
    dest.y = oldy;

    src.x = image->margin.left;
    src.y = 0;
    src.w = img_surface->w - image->margin.left - image->margin.right;
    src.h = img_surface->h;
    if(src.h > dest.h)
        src.h = dest.h;

    width = width - image->margin.left - image->margin.right;
    /* Tile the center part of the image */
    while(height > 0)
    {
        while(width > 0)
        {
            /* don't draw more than needed */
            if(width < src.w)
                src.w = width;

            SDL_BlitSurface(img_surface,&src,surface,&dest);
            dest.x   += src.w; 
            width    -= src.w;
        }
        src.w  = img_surface->w;
        dest.x = dest_x;
        width  = w;
        dest.w -= w;

        /* this can happen bcause SDL_BlitSurface modified the value */
        if(src.h == 0)
            break;

        dest.y  += src.h;
        height  -= src.h;

    }

    /* and the right part */
    if(image->margin.right >0)
    {
        dest.x = oldx + oldw - image->margin.right;
        dest.y = oldy;
        src.x = img_surface->w - image->margin.right;
        src.y = 0;
        src.w = image->margin.right;
        src.h = img_surface->h;
        if(src.h > dest.h)
            src.h = dest.h;
        SDL_BlitSurface(img_surface,&src,surface,&dest);
    }

    return 1;
}

int SDL_ImageSetMargins(SDL_Image *image,short left, short top,short bottom,short right)
{
    image->margin.left   = left;

    image->margin.top    = top;
    image->margin.bottom = bottom;
    image->margin.right  = right; 
    return 1;
}

void SDL_ImageStartTransition(SDL_Image *from,SDL_Image *to,SDL_ImageTransition *transition)
{
    int alpha = 0;

    if(from == to)
        return;


    if(from && from->Transition)
    {
        alpha = from->Transition->alpha;

        from->Transition->alpha = 0;
        from->Transition->delay = 0;
        from->Transition->ResultSurface = NULL;
        from->Transition->FromSurface = NULL;
        from->Transition->ToSurface = NULL;
        from->Transition = NULL;

    }


    if(to && transition && transition->delay)
    {
        transition->FromSurface = from->surface;
        transition->ToSurface   = to->surface;
        to->Transition = transition;
        if(transition->delay > 0)
            SDL_AddTimer(transition->delay,SDL_ImageNextFrame,to);
        else
            SDL_ImagePostEvent(to);
    }
    else
        SDL_ImagePostEvent(to);
}


int SDL_ImageGetHeight(SDL_Image *Image)
{
    return Image->surface->h;
}

int SDL_ImageGetWidth(SDL_Image *Image)
{
    return Image->surface->w;
}
