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

SDL_Surface *p_surf_black;
/* Load an image from a file */
SDL_Surface *SDL_LoadGIF(const char *file,int frame)
{
    SDL_RWops *src = SDL_RWFromFile(file, "rb");
	
    return SDL_ImageGIFLoad(src,frame);
}

SDL_Image *SDL_ImageLoad(char *filename, SDL_Rect *selection)
{
    SDL_Surface *surface;
    SDL_Surface *surface1;
    SDL_Surface *surface2;
    SDL_Image   *image = NULL;

    surface  = SDL_LoadGIF(filename,1);
    surface1 = SDL_LoadGIF(filename,2);
    surface2 = SDL_LoadGIF(filename,3);

    surface = IMG_Load(filename);

    if(surface)
    {
        image=(SDL_Image *)malloc(sizeof(SDL_Image));
        memset(image,0,sizeof(SDL_Image));
        image->nof_frames = 1;
        image->cur_frame = 1;
        image->filename = filename;

        if(selection && selection->w > 0 && selection->h > 0)
        {
            image->surface=SDL_CreateRGBSurface(SDL_HWSURFACE,selection->w,selection->h,32,0xff00000,0x00ff0000,
                                                0x0000ff00,0x000000ff);

            image->surface1=SDL_CreateRGBSurface(SDL_HWSURFACE,selection->w,selection->h,32,0xff00000,0x00ff0000,
                                                 0x0000ff00,0x000000ff);

            image->surface2=SDL_CreateRGBSurface(SDL_HWSURFACE,selection->w,selection->h,32,0xff00000,0x00ff0000,
                                                 0x0000ff00,0x000000ff);

            image->surface=SDL_DisplayFormat(image->surface);
            image->surface1=SDL_DisplayFormat(image->surface1);
            image->surface2=SDL_DisplayFormat(image->surface2);


            SDL_BlitSurface(surface,selection,image->surface,NULL);
            SDL_FreeSurface(surface);
            SDL_BlitSurface(surface1,selection,image->surface,NULL);
            SDL_FreeSurface(surface1);
            SDL_BlitSurface(surface2,selection,image->surface,NULL);
            SDL_FreeSurface(surface2);

            image->selection.x = selection->x;
            image->selection.y = selection->y;
            image->selection.w = selection->w;
            image->selection.h = selection->h;
        }
        else
        {
            image->surface=surface;
            image->surface1 = surface1;
            image->surface2 = surface2;
        }
    }
    return image;
}

void SDL_ImagePostEvent(SDL_Image *image)
{
    SDL_Event event;
    
    event.type		 = SDLTK_EVENT;
    event.user.code      = SDLTK_IMAGE_EVENT;
    event.user.data1     = image;
    event.user.data2     = 0;
    
    SDL_PushEvent(&event);
}

void SDL_ImageEvent(SDL_Event *Event)
{
    SDL_Image   *image = (SDL_Image*)Event->user.data1;
    SDL_Surface *p_surf_screen = SDL_GetVideoSurface();

    Uint32 ui_curr_time;

    printf(">%d\n",image->alpha);
    if(p_surf_black)
    {
        SDL_FillRect(p_surf_screen,NULL,0xff0000);
/*        SDL_BlitSurface(p_surf_img,NULL,p_surf_screen,NULL);*/
        SDL_SetAlpha(p_surf_black,SDL_SRCALPHA,(Uint8)image->alpha);
        SDL_BlitSurface(p_surf_black,NULL, p_surf_screen,NULL);

        SDL_Flip(SDL_GetVideoSurface());
    }
    image->old_time  = ui_curr_time;
    ui_curr_time = SDL_GetTicks();

    image->alpha ++;

    if(image->alpha < 255)
        SDL_ImagePostEvent(image);
    else
    {
        SDL_FreeSurface(p_surf_black);
        p_surf_black = NULL;
    }
}

#if 0
int SDL_ImageBlitToWindow(SDL_Image *image,SDL_Window *window,SDL_Rect *destrect)
{

    SDL_Surface *dest = window->surface;
    SDL_Rect wr;
    Uint32 ui_flags = SDL_SRCALPHA;

    if((dest->flags & SDL_HWSURFACE))
    {
        ui_flags |= SDL_HWSURFACE;
    }
    else
    {
        ui_flags |= SDL_SWSURFACE;
    }
        
    wr.x = destrect->x + window->Widget.Rect.x;
    wr.y = destrect->y + window->Widget.Rect.y;
    wr.w = destrect->w;
    wr.h = destrect->h;

    if((p_surf_black = SDL_CreateRGBSurface(ui_flags,image->surface->w,image->surface->h,image->surface->format->BitsPerPixel,
                                            image->surface->format->Rmask,image->surface->format->Gmask,image->surface->format->Bmask, 
                                            image->surface->format->Amask)) == NULL)
    {
        fprintf(stderr,"fade: could not create the black image (%s)\n",SDL_GetError());
        return 0;
    }


    return SDL_ImageBlitToSurface(image,dest,&wr);
    image->alpha = 25;
    image->old_time = SDL_GetTicks();
    SDL_ImagePostEvent(image);

    return 0;

}
#endif


int SDL_ImageBlitToSurface(SDL_Image *image,SDL_Surface *surface,const SDL_Rect *destrect)
{
    SDL_Rect src;
    SDL_Rect dest;

    int width,height;
    int x,y,w,h;

    int oldy = destrect->y;
    int oldx = destrect->x;
    int oldw = destrect->w;
    SDL_Surface *img_surface;

    dest.x = destrect->x;
    dest.y = destrect->y;
    dest.w = destrect->w;
    dest.h = destrect->h;

    if(image == NULL)
        return 0;

    img_surface=image->surface;

    if(image->nof_frames > 1)
    {
        if(image->cur_frame == 2)
        {
            img_surface=image->surface1;
            image->cur_frame=3;
        }
        else if(image->cur_frame == 3)
        {
            img_surface=image->surface2;
            image->cur_frame=1;
        }
        else if(image->cur_frame == 1)
            image->cur_frame ++;
    }



    w = width  = destrect->w;
    h = height = destrect->h;

    x = destrect->x;
    y = destrect->y;

    /* draw the left part, untiled */
    if(image->margin.left >0)
    {
        src.x = 0;
        src.y = 0;
        src.w = image->margin.left;
        src.h = img_surface->h;
        if(src.h > destrect->h)
            src.h = destrect->h;
        
        SDL_BlitSurface(img_surface,&src,surface,&dest);

    }

    dest.x = oldx + image->margin.left;
    dest.y = oldy;

    src.x = image->margin.left;
    src.y = 0;
    src.w = img_surface->w - image->margin.left - image->margin.right;
    src.h = img_surface->h;
    if(src.h > destrect->h)
        src.h = destrect->h;

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
        dest.x = x;
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


SDL_Image *SDL_ImageTransition(SDL_Image *from1,SDL_Image *to2,int progess,int how,int direction)
{

    return NULL;
}
