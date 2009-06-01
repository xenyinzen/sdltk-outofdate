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
#include <malloc.h>
#include <string.h>

#include "SDL_Images.h"
#include "SDL_ImageGIF.h"

/* Load an image from a file */
SDL_Surface *SDL_ImageSurfaceLoad(const char *file,int frame,int *delay)
{
    char *ext = strrchr(file, '.');
	
    if(ext) 
		ext++;

	if(ext && !strcmp(ext, "gif"))
	{
		SDL_Surface *surface;
		SDL_RWops *src = SDL_RWFromFile(file, "rb");
		surface=SDL_ImageGIFLoad(src,frame,delay);
		src->close(src);
		return surface;
	}
	return IMG_Load(file);
}

SDL_ImageFrame *SDL_ImageFrameLoad(const char *filename,int framec)
{
	SDL_ImageFrame *frame = NULL;
	SDL_Surface *surface;
	int delay;
	
	surface = SDL_ImageSurfaceLoad(filename,framec,&delay);
	if(surface)
	{
		frame = malloc(sizeof(SDL_ImageFrame));
		memset(frame,0,sizeof(SDL_ImageFrame));

		frame->surface = surface;
		frame->delay   = delay;

	}
	return frame;
}
