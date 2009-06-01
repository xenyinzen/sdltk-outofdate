/*
  SDLTk

  Basic widget interface
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
#include <stdarg.h>

#include "SDLTk.h"
#include "SDL_Widget.h"

SDL_Surface *last_surface = NULL;
SDL_Surface *previous;

static SDL_mutex *MyMutex;
extern SDL_WidgetType SDL_ButtonGetType();
extern SDL_WidgetType SDL_RadioButtonGetType();
extern SDL_WidgetType SDL_HorizontalScrollbarGetType();
extern SDL_WidgetType SDL_VerticalScrollbarGetType();

int SDL_WidgetInit()
{
    /* Widget */
    SDL_SignalNew("mousebuttondown");
    SDL_SignalNew("mousebuttonup");
    SDL_SignalNew("mousedoubleclick");
    SDL_SignalNew("mousemotion");
    SDL_SignalNew("keydown");
    SDL_SignalNew("hide");
    SDL_SignalNew("show");
    SDL_SignalNew("realize");
    SDL_SignalNew("margins");
    SDL_SignalNew("activate");

    /* Button */
    SDL_SignalNew("clicked"); 

    /* Table */
    SDL_SignalNew("select-row");
    SDL_SignalNew("edited");

    /* Scrollbar */
    SDL_SignalNew("value-changed");

    /* Menu */
    SDL_SignalNew("entry-clicked");

    /* Model */
    SDL_SignalNew("insert-row");
    SDL_SignalNew("add-row");

    /* Image */
    SDL_SignalNew("frame-update");
    MyMutex=SDL_CreateMutex();
    SDL_ButtonGetType();
    SDL_RadioButtonGetType();
    SDL_HorizontalScrollbarGetType();
    SDL_VerticalScrollbarGetType();
    return 1;
}

/*
 * Closes the widget
 */
int SDL_WidgetClose(SDL_Widget *Widget)
{
    int i;
    extern struct SDL_WidgetTypeListItem WidgetTypeList[];
    SDL_WidgetList *current_widget,*prev;

    current_widget=SDL_WindowGetWidgetList();
    prev=NULL;

    while(current_widget)
    {
        if(current_widget->Widget == Widget)
        {
            /* tangg added */
            if (prev == NULL) {
		SDL_WindowSetTopOfWidgetList( current_widget->Next );
            }
            else {
	        prev->Next = current_widget->Next;
            }

#if 0	    
            /* tangg added */
            /* should make sure the type of the widget to free, need some work....
             */
            //i = current_widget->Widget->Type;
            i = SDL_WidgetGetType(current_widget);
            
            if (!strcmp(WidgetTypeList[i].id, "SDL_Label")) {
            	free((SDL_Label *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_Button")) {
            	free((SDL_Button *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_RadioButton")) {
            	free((SDL_RadioButton *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_Edit")) {
            	free((SDL_Edit *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_MultiLineEdit")) {
            	free((SDL_MultiLineEdit *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_Panel")) {
            	free((SDL_Panel *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_ProgressBar")) {
            	free((SDL_ProgressBar *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_ToggleButton")) {
            	free((SDL_ToggleButton *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_Table")) {
            	free((SDL_Table *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_Tree")) {
            	free((SDL_Tree *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_ViewItem")) {
            	free((SDL_ViewItem *)Widget);
            }
            else if (!strcmp(WidgetTypeList[i].id, "SDL_VolumeBar")) {
            	free((SDL_VolumeBar *)Widget);
            }
            else
            {
            	fprintf(stderr, "Can't free the momery of that widget.\n");
            }
	    /* fprintf(stderr, "widget free finished.\n"); */
	    current_widget->Widget = NULL;
#endif
#if 0           
	    free(current_widget);
	    current_widget = NULL;
#endif
            return 1;
        }
        prev=current_widget;
        current_widget=current_widget->Next;
    }
    return 0;
}


/*
 * Draws all widgets of the current stack onto the destiation screen
 */
int SDL_DrawAllWidgets(SDL_Window *Window)
{
    T_Widget_Draw draw; /* Draw function prototype */
    SDL_WidgetList *WidgetList;
    SDL_Surface    *active_surface = NULL;
    SDL_Widget     *Widget;


#if 0
    printf("SDL_DrawAllWidgets %p\n",Window);
    SDL_Rect dest;
    SDL_Rect src;

    if(target_surface == NULL && screen)
        target_surface = screen;

    active_surface = SDL_GetSurfaceStack();
    if(active_surface == NULL)
        return 0;

    if(previous != active_surface)
    {
        last_surface=previous;
        fadeon=1;
    }

    dest.x=0;
    dest.y=0;
    dest.w=0;
    dest.h=0;

    if(active_surface)
    {
        src.x=0;
        src.y=0;
        src.w=active_surface->w;
        src.h=active_surface->h;
    }
#endif

    WidgetList=Window->WidgetList;

    if(WidgetList == NULL)
        printf("Nothing to draw\n");

    SDL_mutexP(MyMutex);
    while(WidgetList)
    {
        Widget=(SDL_Widget*)WidgetList->Widget;
        if(Widget->flags & WIDGET_VISIBLE)
        {
            draw=SDL_WidgetTypeGetDraw(Widget->Type);
   
            if(draw)
            {
                draw(Widget,Widget->Surface,NULL);
            }
   
        }
        WidgetList=WidgetList->Next;
    }
    SDL_mutexV(MyMutex);
    
    SDL_UpdateRect(Window->surface,0,0,0,0);                

    
    if(previous!=active_surface)
        previous=active_surface;




    return 1;
}

int SDL_WidgetEvent(SDL_Widget *widget,SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        SDL_SignalEmit(widget,"keydown",event);
        break;
    case SDL_KEYUP:
        SDL_SignalEmit(widget,"keyup",event);
        break;
    case SDL_MOUSEMOTION:
        SDL_SignalEmit(widget,"mousemotion",event);
        break;
    case SDL_MOUSEBUTTONDOWN:
        SDL_SignalEmit(widget,"mousebuttondown",event);
        break;
    case SDL_MOUSEBUTTONUP:
        SDL_SignalEmit(widget,"mousebuttonup",event);
        break;
    case SDL_MOUSEDOUBLECLICK:
        SDL_SignalEmit(widget,"mousedoubleclick",event);
        break;
    default:
        break;
    }
    return 1;
}

int SDL_WidgetSetFocus(SDL_Widget *widget)
{
    if(widget && (widget->flags & WIDGET_FOCUSABLE))
    {
        SDL_WindowSetFocusWidget(widget);
        return 1;
    }
    return 0;
}

int SDL_WidgetHasFocus(SDL_Widget *widget)
{
    if(SDL_WindowGetFocusWidget() == widget)
        return 1;
    return 0;
}

int SDL_WidgetLoseFocus()
{
    SDL_WindowSetFocusWidget(NULL);
    return 1;
}

void SDL_WidgetRedrawEvent(SDL_Widget *widget)
{
    SDL_Event event;
    
    if(widget == NULL)
        return;

    if((widget->flags & WIDGET_VISIBLE) == WIDGET_VISIBLE)
    {
        event.type		 = SDLTK_EVENT;
        event.user.code  = SDLTK_WIDGET_REDRAW;
        event.user.data1 = widget;
        event.user.data2 = 0;

        SDL_PushEvent(&event);
    }
}

void SDL_WidgetHide(SDL_Widget *widget)
{
    SDL_Event event;
    
    if(widget == NULL)
        return;

    event.type       = SDLTK_EVENT;
    event.user.code  = SDLTK_WIDGET_HIDE;
    event.user.data1 = widget;
    event.user.data2 = 0;
    SDL_PushEvent(&event);
}


void SDL_WidgetShow(SDL_Widget *widget)
{
    SDL_Event event;
    
    if(widget == NULL)
        return;

    if((widget->flags & WIDGET_VISIBLE) != WIDGET_VISIBLE)
    {
        if(SDLTk_IsRunning())
        {
            event.type       = SDLTK_EVENT;
            event.user.code  = SDLTK_WIDGET_SHOW;
            event.user.data1 = widget;
            event.user.data2 = 0;
            SDL_PushEvent(&event);
        }
        else
        {
            widget->flags |= (WIDGET_VISIBLE);
            SDL_WindowAddWidget(widget);
        }
    }
}

void SDL_WidgetMove(SDL_Widget *widget,int x, int y)
{
    SDL_Event event;
    SDL_Rect  *rect;

    if(widget == NULL)
        return;
    
    rect = malloc(sizeof(SDL_Rect));
    
    rect->x = x;
    rect->y = y;
    rect->w = widget->Rect.w;
    rect->h = widget->Rect.h;

    event.type       = SDLTK_EVENT;
    event.user.code  = SDLTK_WIDGET_MOVE;
    event.user.data1 = widget;
    event.user.data2 = rect;
    SDL_PushEvent(&event);
}

void SDL_WidgetResize(SDL_Widget *widget,int w,int h)
{
    SDL_Event event;
    SDL_Rect  *rect;

    if(widget == NULL)
        return;
    
    rect = malloc(sizeof(SDL_Rect));
    
    rect->x = widget->Rect.x;
    rect->y = widget->Rect.y;
    rect->w = w;
    rect->h = h;

    event.type		 = SDLTK_EVENT;
    event.user.code      = SDLTK_WIDGET_RESIZE;
    event.user.data1     = widget;
    event.user.data2     = rect;
    SDL_PushEvent(&event);
}

int SDL_WidgetGetType(SDL_Widget *widget)
{
    if(widget != NULL)
        return widget->Type;
    return -1;
}

int SDL_WidgetSetDims(SDL_Widget *widget,short x, short y, short w, short h)
{
    SDL_Window *window;

    if(widget == NULL)
        return 0;

    window = SDL_WindowManagerGetWindow(widget);

    widget->Rect.x = x;
    widget->Rect.y = y;
    widget->Rect.w = w;
    widget->Rect.h = h;

    if(window)
    {
        if(widget->Surface)
            SDL_FreeSurface(widget->Surface);
        
        SDL_WindowCreateWidgetSurface(window,widget);
    }
    return 1;

}

void SDL_WidgetDraw(SDL_Widget *Awidget,SDL_Rect *Rect)
{
    SDL_WidgetList *temp;
    SDL_Window *Window;
    T_Widget_Draw  draw; /* Draw function prototype */
    SDL_Rect intersection;
    int doupdate=1;

   
    draw=NULL;


    Window = SDL_WindowManagerGetWindow(Awidget);
    if(Window == NULL)
        return;
    temp= Window->WidgetList;


    if(Window->Visible)
    {
        while(temp)
        {
            /* Check only the visible widgets */
            if(temp->Widget->flags & WIDGET_VISIBLE)
            {
                /* If the redraw area is intersecting with the widget from 
                   the list redraw it */
                if(SDL_IntersectRect(Rect,&temp->Widget->Rect,&intersection))
                {
                    draw=SDL_WidgetTypeGetDraw(temp->Widget->Type);
                    if(draw)
                        draw(temp->Widget,temp->Widget->Surface,&intersection);
                    else
                        doupdate=0;
                }
                /* Or the area is inside a widget */
                else if(SDL_RectInside(Rect,&temp->Widget->Rect))
                {
                    draw=SDL_WidgetTypeGetDraw(temp->Widget->Type);
                    if(draw)
                        draw(temp->Widget,temp->Widget->Surface,Rect);
                    else
                        doupdate=0;
                }
                /* Or a complete widget is inside the area */
                else if(SDL_RectInside(&temp->Widget->Rect,Rect))
                {
                    draw=SDL_WidgetTypeGetDraw(temp->Widget->Type);
                 
                    if(draw)
                        draw(temp->Widget,temp->Widget->Surface,Rect);
                    else
                        doupdate=0;
                }
                else
                {

                }
   
            }
            temp=temp->Next;
        }
        if(doupdate)
        {

            SDL_Rect update;
            update.x = Rect->x + Window->Widget.Rect.x;
            update.y = Rect->y + Window->Widget.Rect.y;
            update.w = Rect->w;
            update.h = Rect->h;
            /* OpenGL aware update rect */
            SDLTk_UpdateRect(Window->surface,update.x,update.y,update.w,update.h); 
        }

    }
    Awidget=Awidget;
}


int SDL_WidgetSetClipArea(SDL_Widget *widget,Sint16 x,Sint16 y,Uint16 w,Uint16 h)
{
    if(widget == NULL)
        return 0;

    widget->Clip.x = x;
    widget->Clip.y = y;
    widget->Clip.w = w;
    widget->Clip.h = h;

    return 1;
}


int SDL_WidgetAddChild(SDL_Widget *parent,SDL_Widget *child)
{
    parent=parent;
    child=child;
    return 0;
}


int SDL_WidgetSizeRequest(SDL_Widget *widget,SDL_Rect *rect)
{
    SDL_WidgetFunctions funcs;
	
    funcs=SDL_WidgetTypeGetFunctions(widget->Type);
	
    if(funcs.sizerequest)
    {
        return funcs.sizerequest(widget,rect);
    }
    else
    {
        rect->x = 0;
        rect->y = 0;
        rect->w = 0;
        rect->h = 0;

    }
    return 0;
}
