/*
  SDLTk

  Declaration of window manager stuff
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
#ifndef __SDL_WINDOWMANAGER_H__
#define __SDL_WINDOWMANAGER_H__

#include "SDL.h" 

#include "SDL_Widget.h"
#include "SDL_Window.h"

/* Surface with widget settings */
typedef struct SDL_WindowListElem
{
    SDL_LinkedListElem   ll;
    SDL_Surface      *Parent;
    SDL_Window       *Window;
}SDL_WindowListElem;

/* Initialize the main video window */
int SDL_WindowManagerInit();

int SDL_WindowAvailable();


int SDL_CloseAllWindows();
int SDL_CloseTopWindow();


int SDL_WindowManagerEvent(SDL_Event *event);

SDL_Window *SDL_WindowManagerGetWindow(SDL_Widget *widget);

#endif /* __SDL_WINDOWMANAGER_H__ */
