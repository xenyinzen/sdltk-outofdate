/*
  SDLTk

  Implementation of a basic window manager
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
#include "SDL_WindowManager.h"

SDL_Window *SDL_WindowGetTopWindow();


static SDL_LinkedList WindowList;

int SDL_WindowManagerInit()
{
    SDL_LListInit(&WindowList);
    return 1;
}    

int SDL_WindowAvailable()
{
    return (WindowList.last != NULL);
}

int SDL_CloseAllWindows()
{
    while(SDL_WindowAvailable())
        SDL_WindowClose();

    return 1;
}

void SDL_WindowAddToWindowList(SDL_Window *Window)
{
    SDL_WindowListElem *WindowNode;
    
    WindowNode=(SDL_WindowListElem*)malloc(sizeof(SDL_WindowListElem));
    memset(WindowNode,0,sizeof(SDL_WindowListElem));
    
    WindowNode->Window=Window;
    
    SDL_LListAddToEnd(&WindowList,&WindowNode->ll);
}


int SDL_CloseTopWindow()
{
    SDL_WindowListElem *l;

    SDL_Window *window = SDL_WindowGetTopVisibleWindow();


    if(window == NULL)
        window = SDL_WindowGetTopWindow();

    l = (SDL_WindowListElem*) WindowList.last;

    while(l)
    {
        if(l->Window == window)
        {
            break;
        }
        l= (SDL_WindowListElem*)l->ll.prev;
    }
    
    SDL_LListRemove(&WindowList,&l->ll);
    l = (SDL_WindowListElem*) WindowList.last;

    if(l != NULL)
        SDL_WindowRedrawEvent(l->Window);
    return 1;
}



/* Get the top most visible window.
   On this window the drawing of widgets is done (window has the focus) */
SDL_Window *SDL_WindowGetTopVisibleWindow()
{
    SDL_WindowListElem *WindowListItem = (SDL_WindowListElem*)WindowList.last;
    
    if(WindowListItem == NULL)
        return NULL;
    do
    {
        if(WindowListItem->Window && WindowListItem->Window->Visible)
            return WindowListItem->Window;
     
        WindowListItem =(SDL_WindowListElem*) WindowListItem->ll.prev;
    }
    while(WindowListItem);
    
    return NULL;
}

/* Get the top most window, regardless if it is visible or not 
   On this window new widgets are created */
SDL_Window *SDL_WindowGetTopWindow()
{
    SDL_WindowListElem *WindowListItem = (SDL_WindowListElem*)WindowList.last;

    if(WindowListItem)
    {
        return WindowListItem->Window;
    }
    return NULL;
}

int SDL_WindowManagerWindowEvent(SDL_Event *event)
{
    SDL_WindowListElem *CurrentWindow  = (SDL_WindowListElem*)WindowList.last;
    SDL_WindowListElem *PreviousWindow = NULL;
    SDL_WindowListElem *StoredPreviousWindow = NULL;
    SDL_WindowListElem *StoredWindow         = NULL;

    switch(event->type)
    {
    case SDL_KEYDOWN:
        switch( event->key.keysym.sym ) 
        {
        case SDLK_TAB:
        {
            SDL_Widget     *FocusWidget;
            SDL_WidgetList *WidgetList;
            SDL_Widget     *FirstFocusWidget=NULL;
            int store=0;

            WidgetList  = SDL_WindowGetWidgetList();
            FocusWidget = SDL_WindowGetFocusWidget();
            
            while(WidgetList)
            {
                if(FocusWidget == NULL)
                {
                    if(WidgetList->Widget->flags & WIDGET_FOCUSABLE)
                    {
                        SDL_WindowSetFocusWidget(WidgetList->Widget);
                        break;
                    }
                }
                else
                {
                    if((WidgetList->Widget->flags & WIDGET_FOCUSABLE) && FirstFocusWidget == NULL)
                        FirstFocusWidget=WidgetList->Widget;

                    if(store && (WidgetList->Widget->flags & WIDGET_FOCUSABLE))
                    {
                        SDL_WindowSetFocusWidget(WidgetList->Widget);
                        break;
                    }
                    if(WidgetList->Widget == FocusWidget)
                        store=1;

                }
                WidgetList = WidgetList->Next;
            
            }
            if(WidgetList == NULL)
            {
                if(FirstFocusWidget)
                    SDL_WindowSetFocusWidget(FirstFocusWidget);
            }
            SDL_WidgetRedrawEvent(FocusWidget);
            
        }
        break;

        default:
            break;
        }
        break;
        
    }

    if(event->type == SDL_MOUSEBUTTONDOWN && CurrentWindow)
    {
        while(CurrentWindow->ll.next)
        {
            if(SDL_WidgetIsInside(&CurrentWindow->Window->Widget,event->motion.x,event->motion.y))
            {
                /* Remember the top most visible window which is clicked on */
                StoredWindow         = CurrentWindow;                
              	StoredPreviousWindow = PreviousWindow;
            }
            PreviousWindow = CurrentWindow;
            CurrentWindow  = (SDL_WindowListElem*)CurrentWindow->ll.next;
        }

        if(StoredWindow)
        {
            /* If there is a window click, make sure it becomes the top visible window */
            CurrentWindow->ll.next = &StoredWindow->ll;
            if(StoredPreviousWindow == NULL)
                WindowList.first = StoredWindow->ll.next;
            else
                StoredPreviousWindow->ll.next = StoredWindow->ll.next;
            StoredWindow->ll.next = NULL;
            return 1;
        }
   
    }
    return 0;
}

SDL_Window *SDL_WindowManagerGetWindow(SDL_Widget *widget)
{
    SDL_WindowListElem *WindowListItem = (SDL_WindowListElem*)WindowList.first;
    SDL_WidgetList* wl;
    SDL_Window *TopVisible;

	
    while(WindowListItem)
    {
        if(WindowListItem->Window && WindowListItem->Window->Visible)
        {
            TopVisible = WindowListItem->Window;
            wl=WindowListItem->Window->WidgetList;        

            while(wl)
            {
                if(wl->Widget == widget)
                    return WindowListItem->Window;

                wl=wl->Next;
            }
        }
        WindowListItem = (SDL_WindowListElem*)WindowListItem->ll.next;
    }
    return NULL;
   
}

/* Will handle events for windows and focus widgets */
/* return 0 if no windows are available */
int SDL_WindowManagerEvent(SDL_Event *event)
{
    SDL_WidgetList* WidgetList;
    SDL_Widget* focus_widget=NULL;
    SDL_Window *window;

    SDL_WindowManagerWindowEvent(event);
    window = SDL_WindowGetTopVisibleWindow();

    if(SDL_WindowAvailable() == 0)
        return 0;

    if(window && event->type >= SDL_MOUSEMOTION)
    {
        event->motion.xrel = event->motion.x;
        event->motion.yrel = event->motion.y;
        event->motion.x -= window->Widget.Rect.x;
        event->motion.y -= window->Widget.Rect.y;
    }

    focus_widget = SDL_WindowGetFocusWidget();

    if(event->type == SDL_MOUSEBUTTONDOWN &&
       focus_widget != NULL &&
       SDL_WidgetIsInside(focus_widget,event->motion.x,event->motion.y))
    {
        SDL_WidgetEvent(focus_widget,event);
        return 1;
    }
    if(event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_WidgetList*  focus_widget = SDL_WindowGetWidgetList();
        while(focus_widget)
        {
            SDL_Widget *w=(SDL_Widget*)focus_widget->Widget;
            
            if(SDL_WidgetIsInside(w,event->motion.x,event->motion.y))
            {
                if(focus_widget->Widget->flags & WIDGET_FOCUSABLE)
                {
                    SDL_WindowSetFocusWidget(focus_widget->Widget);
                    SDL_WidgetRedrawEvent(focus_widget->Widget);
                }
                /*  Bug found when there are overlapping widgets the focus is set to the wrong widget */
            }
            focus_widget=focus_widget->Next;
        }
    }

    WidgetList = SDL_WindowGetWidgetList();
    while(WidgetList)
    {
	/* ATTEN: here should be the old value of the WidgetList, if the win->WidgetList value changed in the following 
	 * SDL_WidgetEvent() function, then this pointer here could not update immediately.
	 */
        SDL_Widget *Widget=(SDL_Widget*)WidgetList->Widget;

        /* tangg added */
        if(Widget && (Widget->flags & WIDGET_VISIBLE))
        {
            SDL_WidgetEvent(Widget,event);
        }
        WidgetList=WidgetList->Next;
    }

    /* If the widgets don't handle the event pass
       the event to the event handler of the window */
    {
        SDL_Window *Win;

        Win=SDL_WindowGetTopVisibleWindow();
        
        if(Win && Win->EventHandler)
        {

            Win->EventHandler(*event);
        }
    }
    return 1;
}
