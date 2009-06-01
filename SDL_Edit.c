/*
  SDLTk

  Edit widget, single line
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
#include "SDL_Edit.h"

void        SDL_EditDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_EditSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata);

SDL_WidgetType SDL_EditGetType()
{
    SDL_WidgetType edit;
    
    edit = SDL_WidgetTypeGet("SDL_Edit");
    
    if(edit == 0)
    {
        SDL_WidgetFunctions *func;
        /* Edit */
        SDL_SignalNew("changed");
        edit=SDL_WidgetTypeRegister("SDL_Edit",&func);
        func->draw        = SDL_EditDraw;
    }
    
    return edit;
}

SDL_Widget *SDL_EditNew()
{
    SDL_Widget *Widget;
    SDL_Edit   *Edit;


    Edit=(SDL_Edit*)malloc(sizeof(SDL_Edit));
    memset(Edit,0,sizeof(SDL_Edit));

    Widget=(SDL_Widget*)Edit;

    Widget->Type      = SDL_EditGetType();
    Widget->Rect.x    = 0;
    Widget->Rect.y    = 0;
    Widget->Rect.w    = 0;
    Widget->Rect.h    = 0;
    Widget->flags     = WIDGET_FOCUSABLE;
    
    
    SDL_SignalConnect(Widget,"keydown",SDL_EditSignalKeyDown,Widget);
    
    Edit->Caption.Data = (char*)malloc(1024);
    memset(Edit->Caption.Data,0,1024);
    Edit->Caption.Length = 0;

    Edit->Font    = &DefaultFont;

    Edit->CursorPosition = 0;
    Edit->bgcolor = 0xfffff7;
    Edit->fgcolor = 0x000000;

    return (SDL_Widget*)Edit;
}

void  SDL_EditDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Edit *Edit=(SDL_Edit*)widget;
    SDL_Rect cursor;
    SDL_Rect StringPos;
    int StringWidth;
    int TempStringWidth;
    SDL_String str;

    if(Edit->Font == NULL)
    {
        return;
    }
    SDL_FillRect(surface,NULL,BLACK);
    {
        SDL_Rect r;
        r.x = 1;
        r.y = 1;
        r.w = widget->Rect.w-2;
        r.h = widget->Rect.h-2;
        SDL_FillRect(surface,&r,Edit->bgcolor);
    }
    StringPos.y = ((widget->Rect.h - SDL_FontGetHeight(Edit->Font))/2);
    StringPos.x = 2;
    StringPos.w = widget->Rect.w - 2;
    StringPos.h = widget->Rect.h;
    
    StringWidth = SDL_FontGetStringWidth(Edit->Font,Edit->Caption.Data);

    {
        char tmp[255];
        unsigned int size;
        int size2;

        size=SDL_StringLength(&Edit->Caption);
        SDL_StringSubString(&Edit->Caption,0,0);
        {
            size = size - (size - Edit->CursorPosition);
            if(size >= 0)
            {
                char *caption;

                
                strncpy(tmp,Edit->Caption.Data,size);
                tmp[size]=0;
                
                
                TempStringWidth = SDL_FontGetStringWidth(Edit->Font,tmp);
                
                caption=(char*)&tmp;
                
                while(TempStringWidth > widget->Rect.w)
                {
                    TempStringWidth = SDL_FontGetStringWidth(Edit->Font,caption);
                    caption++;
                }
                SDL_StringSetText(&str,caption);
                SDL_FontDrawString(surface,Edit->Font,&str,Edit->fgcolor,&StringPos);

                if(SDL_WidgetHasFocus(widget))
                {
                    StringWidth = SDL_FontGetStringWidth(Edit->Font,caption);

                    if(StringWidth > widget->Rect.w)
                        cursor.x = widget->Rect.w - 4;
                    else
                        cursor.x = SDL_FontGetStringWidth(Edit->Font,caption)+3;

                    cursor.y = StringPos.y;
                    cursor.w = 1;
                    cursor.h = SDL_FontGetHeight(Edit->Font);
                    
                    SDL_DrawLine(surface,cursor.x,cursor.y,cursor.x,cursor.y+cursor.h,0x000007ff);
                }

                /* Draw the part behind the cursor */
                size2=SDL_StringLength(&Edit->Caption) - size;
                if(size2 > 0)
                {
                    memset(tmp,0,255);
                    strncpy(tmp,Edit->Caption.Data + size,size2);
                    StringPos.x = cursor.x + 1;
					
                    printf("Srin %d\n",StringPos.x);
                    SDL_StringSetText(&str,tmp);
                    SDL_FontDrawString(surface,Edit->Font,&str,Edit->fgcolor,&StringPos);
                }
            }

        }
    }
    
   
}

int SDL_EditProperties(SDL_Widget *widget,int feature,va_list list)
{
    SDL_Edit *Edit=(SDL_Edit*)widget;

    switch(feature)
    {
    case SET_FG_COLOR:
        Edit->fgcolor = va_arg(list,Uint32);
        break;
    case SET_BG_COLOR:
        Edit->bgcolor = va_arg(list,Uint32);
        break;
    default:
        return 0;
    }
    return 1;
}

static void SDL_EditSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Edit *Edit=(SDL_Edit*)object;
    SDL_Event *event = (SDL_Event *)signaldata;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDLMod mod;
    int key;


    if(!SDL_WidgetHasFocus(widget))
        return;

    mod = event->key.keysym.mod;

    switch(event->key.keysym.sym)
    {
    case SDLK_DELETE:
    {
        if(Edit->CursorPosition >= 0)
        {
            strcpy(Edit->Caption.Data + Edit->CursorPosition,Edit->Caption.Data + Edit->CursorPosition+1);
            SDL_WidgetRedrawEvent(widget);
            SDL_SignalEmit(widget,"changed",NULL);
        }
    }
    break;
    case SDLK_BACKSPACE:
    {
        if(Edit->CursorPosition > 0)
        {
            Edit->CursorPosition--;
            strcpy(Edit->Caption.Data + Edit->CursorPosition,Edit->Caption.Data + Edit->CursorPosition+1);
            SDL_SignalEmit(widget,"changed",NULL);
            SDL_WidgetRedrawEvent(widget);
        }
    }
    break;
    case SDLK_HOME:
        Edit->CursorPosition = 0; 
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_END:
        Edit->CursorPosition = SDL_StringLength(&Edit->Caption)+1; 
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_LEFT:
        if(Edit->CursorPosition > 0)
            Edit->CursorPosition--;
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_RIGHT:
        if(Edit->CursorPosition < SDL_StringLength(&Edit->Caption))
            Edit->CursorPosition++;
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_RETURN:
        if(SDL_WidgetHasFocus(widget))
            SDL_SignalEmit(widget,"activate",NULL);
        break;
    default:
        key=event->key.keysym.unicode;
        /* numeric keypad translation */
        if ((key >= SDLK_KP0) && (key <= SDLK_KP9))
            key -= 0xd0;

        if ((key >= 0x20) && (key <= 0xFF))
        {
            if(mod & KMOD_SHIFT)
            {
                if(key>=SDLK_a && key <= SDLK_z)
                {
                    key -= 32;
                }
                if(key == SDLK_SEMICOLON)
                {
                    key=SDLK_COLON;
                }
            }
#if 0
            if(key < 127)
#endif
            {   
                char tmp[255];
                memset(tmp,0,255);
                sprintf(tmp,"%c%s",key,Edit->Caption.Data + Edit->CursorPosition);
                Edit->Caption.Data[Edit->CursorPosition+1]=0;
                sprintf(Edit->Caption.Data,"%s%s",Edit->Caption.Data,tmp);
                Edit->Caption.Length =(int)strlen(Edit->Caption.Data);

                Edit->CursorPosition++;
                SDL_WidgetRedrawEvent(widget);
                SDL_SignalEmit(widget,"changed",NULL);
            }
        }
        break;
    }
}




char *SDL_EditGetText(SDL_Widget *widget)
{
    SDL_Edit *Edit=(SDL_Edit*)widget;
    return Edit->Caption.Data;
}

int SDL_EditSetFont(SDL_Widget *widget,SDL_Font *font)
{
    SDL_Edit *Edit=(SDL_Edit*)widget;
    
    if(font == NULL)
        return 0;
    
    Edit->Font = font;
    
    return 1;
}

void SDL_EditSetText(SDL_Widget *widget,const char *text)
{
    SDL_Edit *Edit=(SDL_Edit*)widget;
    strcpy(Edit->Caption.Data,text);
    Edit->CursorPosition=SDL_StringLength(&Edit->Caption)+1;

}
