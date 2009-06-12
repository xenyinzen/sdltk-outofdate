/*
  SDLTk

  Implementation of window specific behaviour
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
#include <stdio.h>
#include <string.h>

#include "SDL.h"
#include "SDL_thread.h"

#include "SDLTk.h"
#include "SDL_Window.h"
#include "SDL_WindowManager.h"

#include <GL/gl.h>

void SDL_WindowRedrawEvent();


SDL_Window *SDL_WindowGetTopWindow();

void SDL_WindowAddToWindowList(SDL_Window *Window);
void SDL_WidgetDraw(SDL_Widget *widget,SDL_Rect *Rect);

int SDL_DrawAllWidgets(SDL_Window *Window);

SDL_Surface *SDL_GetSubSurface(SDL_Surface *surface,SDL_Widget *widget);

/* Create a new window, this has to be inside the video surface */
SDL_Window *SDL_WindowNew(short x,short y,short width,short height)
{
    SDL_Window *Window;
    SDL_Widget *Widget;
    SDL_Surface *video;


    video = SDLTk_GetVideoSurface();

    if(video == NULL)
        return NULL;

    Window = malloc(sizeof(SDL_Window));
    memset(Window,0,sizeof(SDL_Window));

    Widget = (SDL_Widget*)Window;

    Widget->Rect.x = x;
    Widget->Rect.y = y;
    Widget->Rect.w = width;
    Widget->Rect.h = height;

    Window->Visible      = 0;
    Window->Type         = 0;
    Window->FocusWidget  = NULL;
    if(video->flags & SDL_OPENGL)
    {
        /* width and height have to be a power of 2 for nice textures*/
        Window->surface   = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,
                                                 0xff000000,0x00ff0000,0x0000ff00,0x000000ff);
        SDL_SetAlpha(Window->surface,0,0);
    }
    else
        Window->surface   = SDL_GetSubSurface(video,Widget);

    SDL_WindowAddToWindowList(Window);

    return Window;
}

int SDL_WindowOpen(SDL_Window *Window)
{
    if(Window == NULL)
        return 0;
       
        
    Window->Type    = 0;
    Window->bgcolor =  SDL_MapRGB(Window->surface->format,0xd4,0xd4,0xd4);

    Window->Visible = 1;

    SDL_WindowRealizeEvent(Window); /* Post a realize event */
    SDL_WindowRedrawEvent(Window);  /* Post a redraw event */
    return 1;
}

void SDL_WindowOpenPopup(SDL_Window *Window)
{
    if(Window == NULL)
        return;

    Window->Type = 1;
    /* Add it to the window list it could be closed in
       the mean time */
    SDL_WindowAddToWindowList(Window);
    SDL_WindowRedrawEvent(Window); /* Post a redraw event */
}

void SDL_WindowClose()
{
    SDL_CloseTopWindow();
}



int SDL_WindowAddWidgetToWindow(SDL_Window *Window,SDL_Widget *widget)
{
    SDL_WidgetList *Item;


    Item = malloc(sizeof(SDL_WidgetList));
    memset(Item,0,sizeof(SDL_WidgetList));

    Item->Widget = widget;
    if(widget->Surface == NULL)
        widget->Surface = SDL_GetSubSurface(Window->surface,widget);

    if(Window->WidgetList==NULL)
    {
        Window->WidgetList = Item;
        /* Set the focus to the new widget if edit widget */
        if((widget->flags & WIDGET_FOCUSABLE) && SDL_WindowGetFocusWidget() == NULL)
        {
            Window->FocusWidget=widget;
        }
    }
    else
    {
        SDL_WidgetList *temp;
        
        temp = Window->WidgetList;
        
        while(temp->Next)
            temp=temp->Next;
        
        temp->Next=Item;
    }
    SDL_SignalEmit(widget,"show",NULL);
    return 1;
}


int SDL_WindowAddWidget(SDL_Widget *widget)
{
    SDL_Window *Window;
 

    Window = SDL_WindowGetTopWindow();

    if(Window == NULL)
        return 0;
	
    return SDL_WindowAddWidgetToWindow(Window,widget);

}

void SDL_WindowRemoveWidget(SDL_Widget *widget)
{
    if(widget == NULL)
        return;
    {
        SDL_WidgetList *temp;
        SDL_WidgetList *prev;

        temp = SDL_WindowGetWidgetList();

	prev=NULL;

        while(temp)
        {
	    if(temp->Widget == widget)
            {
		if(prev == NULL)
                {
                    printf("Not yet implemented = temp->Next;\n");
                }
		else
                {
                    SDL_WidgetHide(widget);
                    prev->Next =temp->Next;
		    break;
                }
		
            }
	    prev=temp;
            temp=temp->Next;
	      
        }
	
	if(temp == NULL)
            printf("Widget not found\n");
    }
    
    SDL_WidgetDraw(NULL,&widget->Rect);
}


/* Return the widget list of the top visible (active) window */
SDL_WidgetList *SDL_WindowGetWidgetList()
{
    SDL_Window     *Win;

    Win=SDL_WindowGetTopVisibleWindow();
    if(Win)
    {
        return Win->WidgetList;
    }
    else
    {
        return NULL;
    }
}

void SDL_WindowSetFocusWidget(SDL_Widget *focus_widget)
{
    SDL_WindowGetTopVisibleWindow()->FocusWidget=focus_widget;
}

SDL_Widget *SDL_WindowGetFocusWidget()
{
    SDL_Window *TopVisible;

    TopVisible=SDL_WindowGetTopVisibleWindow();
	
    if(TopVisible == NULL)
        return NULL;

    return TopVisible->FocusWidget;
}





/* Post a realize event, this event will trigger a position location of all widget in this window */
void SDL_WindowRealizeEvent(SDL_Window *Window)
{
    SDL_Event event;

    event.type       = SDLTK_EVENT;
    event.user.code  = SDLTK_WINDOW_REALIZE;
    event.user.data1 = Window;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}

/* Post a redraw event, this event will trigger a redraw of the top most visible window */
void SDL_WindowRedrawEvent(SDL_Window *Window)
{
    SDL_Event event;

    event.type       = SDLTK_EVENT;
    event.user.code  = SDLTK_WINDOW_REDRAW;
    event.user.data1 = Window;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}

void SDL_WindowRealize(SDL_Window *Window)
{
    SDL_WidgetList *Item;
    SDL_Rect rect,win;
    Item=Window->WidgetList;
	
    win.w = 0;
    win.h = 0;

    while(Item)
    {
        SDL_WidgetSizeRequest(Item->Widget,&rect);

        if(rect.x + rect.w > win.w)
            win.w = rect.x + rect.w;
        if(rect.y + rect.h > win.h)
            win.h = rect.y + rect.h;
        Item=Item->Next;
    }

    if(Window->Widget.Rect.h == 0)
        Window->Widget.Rect.h = win.h;
    if(Window->Widget.Rect.w == 0)
        Window->Widget.Rect.w = win.w;



}

/* Redraw the entire active window */
void SDL_WindowRedraw(SDL_Window *Window)
{

    if(Window == NULL)
        return;
    Window->Visible = 1;

    if(Window->NotifyRedraw)
    {

    }
    SDL_FillRect(Window->surface,NULL,Window->bgcolor);
    SDL_DrawAllWidgets(Window);
    SDLTk_UpdateRect(Window->surface,0,0,0,0);
}



int SDL_WidgetActive(SDL_Widget *widget)
{
    SDL_Window *Win;
    SDL_WidgetList *temp;

    Win = SDL_WindowGetTopVisibleWindow();

    if(Win)
    {
        temp=Win->WidgetList;
        
        while(temp)
        {
            if(temp->Widget == widget)
                return 1;
            temp=temp->Next;
        }
    }
    return 0;
}




int SDL_WindowGetMouseState(int *x,int *y)
{
    int abs_x,abs_y;
    SDL_Window *Win;
    Win = SDL_WindowGetTopVisibleWindow();

    if(Win)
    {
        SDL_GetMouseState(&abs_x,&abs_y);
        *x= abs_x - Win->Widget.Rect.x;
        *y= abs_y - Win->Widget.Rect.y;
        return 1;
    }

    return 0;
}

SDL_Surface *SDL_GetSubSurface(SDL_Surface *surface,SDL_Widget *widget)
{
    SDL_Surface *subSurface;
    SDL_Surface *mainvid=surface;

    void *pixels = (char*)mainvid->pixels +  mainvid->pitch * widget->Rect.y
        + mainvid->format->BytesPerPixel * widget->Rect.x;

    subSurface = SDL_CreateRGBSurfaceFrom(pixels,widget->Rect.w,widget->Rect.h,mainvid->format->BitsPerPixel,mainvid->pitch,
                                          mainvid->format->Rmask,mainvid->format->Gmask,mainvid->format->Bmask,mainvid->format->Amask);

	
	

    return subSurface;
}
