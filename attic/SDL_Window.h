/*
  SDLTk

  Declaration of the window structures
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
#ifndef __SDL_WINDOW_H__
#define __SDL_WINDOW_H__

#include "SDL_LList.h"



/* Initialize the main video window */
int SDL_WindowInit(SDL_Surface *s,int width,int height,int bpp);

/* Create a new window */
SDL_Window *SDL_WindowNew(short x,short y,short width,short height);

/* Open the newly created window */
int SDL_WindowOpen(SDL_Window *window);

/* Close the top most window */
void SDL_WindowClose();

void SDL_WindowRedraw();
SDL_Window *SDL_WindowGetTopVisibleWindow();
int SDL_WidgetActive(SDL_Widget *widget);
SDL_WidgetList *SDL_WindowGetWidgetList();

/* Adds a widget to a window, this is done after 
   creating of the widgets */
void SDL_WindowRemoveWidget(SDL_Widget *widget);

void SDL_WindowRealizeEvent(SDL_Window *Window);
void SDL_WindowRedrawEvent(SDL_Window *Window);
int  SDL_WindowAddWidget(SDL_Widget *widget);

void SDL_WindowSetFocusWidget(SDL_Widget *focus_widget);
SDL_Widget *SDL_WindowGetFocusWidget();
int SDL_WindowGetMouseState(int *x,int *y);


void SDL_WindowRealize(SDL_Window *Window);
SDL_Surface *SDL_GetSubSurface(SDL_Surface *surface,SDL_Widget *widget);
#endif /* __SDL_WINDOW_H__ */
