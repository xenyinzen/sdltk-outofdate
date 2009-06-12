/*
  SDLTk

  Main toolkit interface, entry point for events
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

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "SDL.h"
#include "SDLTk.h"

#include "SDL_Window.h"
#include "SDL_WindowManager.h"

#include <GL/gl.h>

static int ScreenWidth;
static int ScreenHeight;
int running;

static SDL_Surface     *VideoSurface;          /* Drawing is done to this surface of the video driver */ 
static SDL_TimerID     DoubleClickTimer;
static int DoubleClick;

#define DOUBLECLICK_TIMEOUT 300 

Uint32  DoubleClickTimerExpired(Uint32  interval,void *params)
{
    params=params;
    interval=interval;
    DoubleClick=0;
    return 0;
}

/* Init the video surface information and init the global variables*/
int SDLTk_Init(void)
{
    static int opengl;
    VideoSurface    = SDL_GetVideoSurface();

    if(VideoSurface->flags & SDL_OPENGL)
        opengl = 1;
    

    if(VideoSurface == NULL)
        return 0;

    /* Initialize global variables */
    ScreenWidth     = VideoSurface->w;
    ScreenHeight    = VideoSurface->h;

    DoubleClick=0;
    running=0;
    if(SDL_WidgetTypeInit() == 0)
        return 0;
    SDL_ModelTypeInit();
    if(SDL_SignalInit() == 0)
        return 0;
    if(SDL_WidgetInit() == 0)
        return 0;
    if(SDL_WindowManagerInit() == 0)
        return 0;

    /* Enable keyboard settings */
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

    SDL_FontInit();
    SDL_StyleInit();

    
    DoubleClickTimer = SDL_AddTimer(DOUBLECLICK_TIMEOUT, DoubleClickTimerExpired,NULL);
    return 1;
}


/* Handle internal events */
static void SDLTk_InternalEvent(SDL_Event *Event)
{
    SDL_Widget *widget;
    SDL_Rect   *rect;
    switch(Event->user.code)
    {
    case SDLTK_WIDGET_HIDE:
        widget = Event->user.data1;
        if(SDL_WidgetActive(widget))
        {
            if(widget->flags & WIDGET_VISIBLE)
            {
                widget->flags = widget->flags & ~WIDGET_VISIBLE;
                SDL_SignalEmit(widget,"hide",NULL);
                SDL_WidgetDraw(widget,&widget->Rect);
            }
        }
        break;

    case SDLTK_WIDGET_SHOW:
        widget = Event->user.data1;
        widget->flags |= WIDGET_VISIBLE;

        if((widget->flags & WIDGET_REALIZED)  != WIDGET_REALIZED)
        {
            widget->flags |= WIDGET_REALIZED;
            SDL_WindowAddWidget(widget);
        }
                    
        if(widget->Rect.w > 0 || widget->Rect.h > 0)
            SDL_WidgetDraw(widget,&widget->Rect);	
        break;
    case SDLTK_WIDGET_RESIZE:
    case SDLTK_WIDGET_MOVE:
        widget = Event->user.data1;
        rect   = Event->user.data2;
        if(SDL_WidgetActive(widget))
        {
            SDL_Rect   r;

            r.x = widget->Rect.x;
            r.y = widget->Rect.y;
            r.w = widget->Rect.w;
            r.h = widget->Rect.h;
                        
            widget->Rect.x = rect->x;
            widget->Rect.y = rect->y;
            widget->Rect.w = rect->w;
            widget->Rect.h = rect->h;
            SDL_WidgetDraw(widget,rect);
            SDL_WidgetDraw(widget,&r);
            free(rect);
        }
        break;

    case SDLTK_WIDGET_REDRAW:
        widget = Event->user.data1;
        if(SDL_WidgetActive(widget))
            SDL_WidgetDraw(widget,&widget->Rect);
        break;
    case SDLTK_WINDOW_REDRAW:
        SDL_WindowRedraw(Event->user.data1);
        break;
    case SDLTK_WINDOW_REALIZE:
        SDL_WindowRealize(Event->user.data1);
        break;
    case SDLTK_IMAGE_EVENT:
        SDL_ImageEvent(Event);
        break;

                
    }
                
}

static void SDLTk_DoubleClickCheck(SDL_Event *Event)
{
    if(DoubleClick)
    {
        Event->type = SDL_MOUSEDOUBLECLICK;
    }
    DoubleClickTimer = SDL_AddTimer(DOUBLECLICK_TIMEOUT, DoubleClickTimerExpired,NULL);
    if(DoubleClickTimer)
        DoubleClick=1;			
}

int SDLTk_PushEvent(SDL_Event *Event)
{
    SDL_Event e;
	
    if(VideoSurface == NULL)
        return 0;
    
    running=1;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    
    switch(Event->type) 
    {
    case SDL_QUIT:
        SDL_CloseAllWindows();
        break;
    case SDL_VIDEOEXPOSE:
    case SDL_ACTIVEEVENT:
        return 1;
    case SDLTK_EVENT:
        SDLTk_InternalEvent(Event);
        break;
    case SDL_MOUSEBUTTONDOWN:
        SDLTk_DoubleClickCheck(Event);
        break;
    default:
        break;
    }
     
    /* Remove all mouse motion events from the queue */
    while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0);

    /* Will handle events for windows and focus widgets */
    return SDL_WindowManagerEvent(Event);
}

int SDLTk_IsRunning()
{
    return running;
}


int SDLTk_Main()
{
    SDL_Event Event;

    if(VideoSurface == NULL)
        return 0;
    
    running=1;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

    memset(&Event,0,sizeof(Event));
    while(SDL_WindowAvailable())
    {
        while(SDL_WaitEvent(&Event)) 
        {
            if(SDLTk_PushEvent(&Event) == 0)
            {
                running = 0;
                SDL_SignalQuit();
                return 0;
            }
        }   
        SDL_Delay(10);
    }
    SDL_SignalQuit();
    return 1;
}


SDL_Surface *SDLTk_GetVideoSurface()
{
    return VideoSurface;
}


/* has to be optimized */
void drawwind()
{
    SDL_Window *window = NULL;
    int id;

    window=SDL_WindowGetTopVisibleWindow();
    if(window)
    {
        SDL_WindowRedraw(window);

        glGenTextures(1,&id);
        glBindTexture(GL_TEXTURE_2D,id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        glTexImage2D(GL_TEXTURE_2D,0,window->surface->format->BytesPerPixel,window->surface->w,window->surface->h,0,
                     GL_RGBA,GL_UNSIGNED_BYTE,window->surface->pixels);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,id);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,1.0f);
        glVertex3f(0.0f,300.0f - 128.0f,0.0f);

        glTexCoord2f(1.0f,1.0f);
        glVertex3f(128.0f,300.0f - 128.0f,0.0f);

        glTexCoord2f(1.0f,0.0f);
        glVertex3f(128.0f,300.0f,0.0f);

        glTexCoord2f(0.0f,0.0f);
        glVertex3f(0.0f,300.0f,0.0f);
        glEnd();

        glDeleteTextures(1,&id);
    }
}


void SDLTK_OpenGLDraw()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0,300.0,0.0,300.0,-2.0,2.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable (GL_LIGHTING);

    drawwind();

    glEnable (GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();


    glMatrixMode(GL_MODELVIEW);



}


void SDLTk_UpdateRect(SDL_Surface *surface,short x,short y,short w,short h)
{
    SDL_Surface *video;
    video=SDL_GetVideoSurface();

    if(video->flags & SDL_OPENGL)
        SDL_UpdateRect(surface,x,y,w,h);
    else
        SDL_UpdateRect(video,x,y,w,h);
}
