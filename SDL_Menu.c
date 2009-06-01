/*
  SDLTk

  one line to give the program's name and an idea of what it does.
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
#include <string.h>

#include "SDLTk.h"
#include "SDL_Menu.h"

static void SDL_MenuDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

static void SDL_MenuSignalHide(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_MenuSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_MenuSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata);

SDL_WidgetType SDL_MenuGetType()
{
    SDL_WidgetType menu;

    menu = SDL_WidgetTypeGet("SDL_Menu");
    
    if(menu == 0)
    {
        SDL_WidgetFunctions *func;

        menu=SDL_WidgetTypeRegister("SDL_Menu",&func);
		
        func->draw = SDL_MenuDraw;
    }
    return menu;
}

SDL_Widget* SDL_MenuNew()
{
    SDL_Object *object;
    SDL_Menu   *menu;
    SDL_Widget *widget;


    menu=(SDL_Menu*) malloc(sizeof(SDL_Menu));
    memset(menu,0,sizeof(SDL_Menu));

    object=(SDL_Object*)menu;
    widget=(SDL_Widget*)menu;

    widget->Type      = SDL_MenuGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;
	
    menu->Font	  = &DefaultFont;
    menu->bgcolor = 0xff0000;

    SDL_SignalConnect(widget,"mousebuttondown",SDL_MenuSignalMouseButtonDown,menu);
    SDL_SignalConnect(widget,"mousemotion",SDL_MenuSignalMouseMotion,menu);
    SDL_SignalConnect(widget,"hide",SDL_MenuSignalHide,menu);
    return widget;
}

void SDL_MenuDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Menu *Menu=(SDL_Menu*)widget;
    SDL_MenuEntry *Entry;
    SDL_Rect r;
    Uint32 color;
    int x,y;

    Entry = Menu->Entries;

    SDL_GetMouseState(&x,&y);


    SDL_FillRect(surface,&widget->Rect,SDL_MapRGB(surface->format,0xff,0xff,0xff));
#if 0
    rectangleColor(surface,widget->Rect.x,widget->Rect.y,widget->Rect.x + widget->Rect.w,
                   widget->Rect.y+ widget->Rect.h,SDL_MapRGB(surface->format,0xac,0xa8,0x99));
#endif
    while(Entry)
    {
        if(Menu->Selected == Entry)
        {
            SDL_Rect re;
            re.x = widget->Rect.x + Entry->Rect.x + 3;
            re.y = widget->Rect.y + Entry->Rect.y + 2;
            re.w = Menu->Width - 6;
            re.h = Entry->Rect.h;

            SDL_FillRect(surface,&re,SDL_MapRGB(surface->format,0x31,0x6a,0xc5));
            color=SDL_MapRGB(surface->format,0xff,0xff,0xff);
        }
        else
        {		
            color=SDL_MapRGB(surface->format,0x00,0x00,0x00);
        }
        
        r.x=widget->Rect.x + Entry->Rect.x + 2;
        r.y=widget->Rect.y + Entry->Rect.y + 2;
        r.w=widget->Rect.w;
        r.h=Entry->Rect.h+8;

        SDL_FontDrawString(surface,Menu->Font,&Entry->string,color,&r);
                               
        Entry=Entry->next;
    }
}




SDL_MenuEntry *SDL_MenuAppend(SDL_Widget *widget,char *txt)
{
    SDL_Menu *menu=(SDL_Menu*)widget;
	
    SDL_MenuEntry *list=menu->Entries;
    SDL_MenuEntry *Entry= (SDL_MenuEntry*)malloc(sizeof(SDL_MenuEntry));
    memset(Entry,0,sizeof(SDL_MenuEntry));

    Entry->Rect.x = 0;
    Entry->Rect.y = 0;
    Entry->Rect.w = SDL_FontGetStringWidth(menu->Font,txt) + 4;
    Entry->Rect.h = SDL_FontGetHeight(menu->Font) + 8;
    SDL_StringSetText(&Entry->string,txt);

    menu->Height += Entry->Rect.h;

    if(Entry->Rect.w > menu->Width)
        menu->Width = Entry->Rect.w;

    if(list == NULL)
    {
        menu->Entries = Entry;
    }
    else
    {
        Entry->Rect.y += list->Rect.h;

        while(list->next)
        {
            list=list->next;
            Entry->Rect.y += list->Rect.h;
        }


        list->next = Entry;
    }
    Entry->Root = widget;
    return Entry;
}


SDL_MenuEntry *SDL_MenuSubAppend(SDL_Widget *widget,SDL_MenuEntry *Parent,char *txt)
{	
    SDL_MenuEntry *Entry;

    if(Parent == NULL)
        return SDL_MenuAppend(widget,txt);
	
    if(Parent->Child == NULL)
    {
        Parent->Child  = SDL_MenuNew();
    }
	
    Entry=SDL_MenuAppend(Parent->Child,txt);
    Entry->Root = widget;

    return Entry;
}

void SDL_MenuPopup(SDL_Widget *widget)
{
    SDL_Menu *menu=(SDL_Menu*)widget;
    int x,y;
    if(widget == NULL)
        return;

    printf("SDL_MenuPopUp\n");

    SDL_WindowRemoveWidget(widget);


    SDL_WindowGetMouseState(&x,&y);
    SDL_WidgetSetDims(widget,x,y,menu->Width,menu->Height);

    widget->flags  = 0;
   
    SDL_WidgetShow(widget);
    SDL_WindowSetFocusWidget(widget);
}

void SDL_MenuAttach(SDL_Widget *widget,SDL_Widget *target)
{
    SDL_Menu *menu=(SDL_Menu*)widget;

    menu->Target = target;
}

static void SDL_MenuSignalHide(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Menu *menu=(SDL_Menu*)object;
    SDL_MenuEntry *e;

    e= menu->Entries;
    while(e)
    {
        if(e->Child != NULL)
            SDL_WidgetHide(e->Child);
	
        e=e->next;
    }

}

static void SDL_MenuSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Menu *menu=(SDL_Menu*)object;
    SDL_Event *event = (SDL_Event*)signaldata;
    SDL_Widget *widget=(SDL_Widget*)object;
    int x,y;

    SDL_GetMouseState(&x,&y);
    x=event->motion.x;
    y=event->motion.y;
    
    if(event->button.button == 1)
    {
        if(SDL_WidgetIsInside(widget,x,y))
        {
            SDL_MenuEntry *e;

            e= menu->Entries;

            x -= widget->Rect.x;
            y -= widget->Rect.y;

            while(e)
            {
                if(x >= e->Rect.x && x < e->Rect.x + e->Rect.w)
                    if(y >= e->Rect.y && y < e->Rect.y + e->Rect.h)
                    {
                        SDL_SignalEmit(e->Root,"entry-clicked",e);
                    }
     
                e=e->next;
            }

        }
        else
        {
            SDL_WidgetHide(widget);
        }
    }
    else
    {
        if(menu->Target != NULL && SDL_WidgetIsInside(menu->Target,x,y) == 0)
        {
            SDL_WidgetHide(widget);
        }
        else
            SDL_WidgetMove(widget,x,y);	

    }

}

static void SDL_MenuSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Menu *menu     = (SDL_Menu*)object;
    SDL_Event *event   = (SDL_Event*)signaldata;
    SDL_Widget *widget = (SDL_Widget*)object;

    if(SDL_WidgetIsInside(widget,event->button.x,event->button.y))
    {
        SDL_MenuEntry *e;
        int x,y;

        e= menu->Entries;
        x= event->button.x - widget->Rect.x;
        y= event->button.y - widget->Rect.y;
        while(e)
        {
            if(x >= e->Rect.x && x < e->Rect.x + menu->Width && y >= e->Rect.y && y < e->Rect.y + e->Rect.h)
            {
                menu->Selected = e;
                if(e->Child != NULL)
                {
                    SDL_Menu *m = (SDL_Menu*)e->Child;
                    e->Child->Rect.x = widget->Rect.x + menu->Width + 2;
                    e->Child->Rect.y = widget->Rect.y + e->Rect.y;
                    if(e->Child->Rect.w == 0)
                        e->Child->Rect.w = m->Width;
                    if(e->Child->Rect.h == 0)
                        e->Child->Rect.h = m->Height;

                    SDL_WidgetShow((SDL_Widget*)e->Child);

                    {
                        SDL_Menu *tmenu = (SDL_Menu*)e->Child;
                        SDL_MenuEntry *y;
                        y=tmenu->Entries;
                        while(y)
                        {
                            SDL_WidgetHide(y->Child);
                            y=y->next;
                        }

                    }
                }
            }
            else
            {
                if(e->Child != NULL)
                {
                    SDL_WidgetHide((SDL_Widget*)e->Child);
                }
            }
            e=e->next;
        }
    }
    else
    {
	
    }
    SDL_WidgetRedrawEvent(widget);
}

