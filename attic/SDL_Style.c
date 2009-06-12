/*
  SDLTk

  Style stack
  Copyright (C) 2006 John Beuving (john.beuving@beatforce.org)

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

#include "SDLTk.h"
#include "SDL_Style.h"

SDL_LinkedList SDL_StyleStack;

void SDL_StyleInit()
{
    SDL_LListInit(&SDL_StyleStack);

    SDL_StyleDefaultInit(); 
    SDL_StyleXPInit();
    
    SDL_StyleDefaultSet(); /* Bottom of the stack */
}





T_Style_Paint findfunc(SDL_Style *style,char *widgetname)
{
    SDL_StyleWidget *widget = (SDL_StyleWidget*)style->WidgetStyles.first;

    while(widget)
    {
        if(!strcmp(widgetname,widget->WidgetName))
            return widget->PaintFunction;
        widget = (SDL_StyleWidget*)widget->ll.next;
    }
    return NULL;

}

/* void w is a widget , problems with forward declration */
void SDL_StylePaint(void *w,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
    SDL_Widget *widget = (SDL_Widget *)w;

    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style paint for widget %p\n",w);
        }
        else
        {
            if(s->ImageStyle)
            {
                T_Widget_StyleDraw draw;
                draw=SDL_WidgetTypeGetStyleDraw(widget->Type);
                if(draw)
                    return draw(widget,surface,Area,s->ImageStyle);
                }
                if(s->PaintFunction)
                {
                    return s->PaintFunction(widget,surface,Area);
                }
        }
    }


    if(elem && elem->style)
    {

        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));
        while(!s)
        {
            s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));
            elem = (SDL_StyleStackElem*)elem->ll.prev;
        }

        if(s->ImageStyle)
        {
            T_Widget_StyleDraw draw;
            draw=SDL_WidgetTypeGetStyleDraw(widget->Type);
            if(draw)
                return draw(widget,surface,Area,s->ImageStyle);
        }
        if(s->PaintFunction)
        {
            return s->PaintFunction(widget,surface,Area);
        }
    }
}


int SDL_StylePush(SDL_Style *style)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)malloc(sizeof(SDL_StyleStackElem));
    elem->style = style;
    SDL_LListAddToEnd(&SDL_StyleStack,&elem->ll);

    return 1;
}

int SDL_StyleUnpush(SDL_Style *style)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*) SDL_StyleStack.first;
    
    /* find the element */
    while(elem)
    {
        SDL_LListRemove(&SDL_StyleStack,&elem->ll);
        elem = (SDL_StyleStackElem*)elem->ll.next;
    }
    
    return 0;
}


SDL_Style *SDL_StyleNew()
{
    SDL_Style *style = (SDL_Style*)malloc(sizeof(SDL_Style));
    memset(style,0,sizeof(SDL_Style));

    return style;
}

SDL_StyleWidget *SDL_StyleGetWidgetEntry(SDL_Style *style,char *widgetname)
{
    SDL_StyleWidget *widget = (SDL_StyleWidget*)style->WidgetStyles.first;

    while(widget)
    {
        if(!strcmp(widget->WidgetName,widgetname))
            return widget;
        widget= (SDL_StyleWidget*)widget->ll.next;
    }
    

    return widget;
}

SDL_StyleWidget *SDL_StyleWidgetEntry(SDL_Style *style,char *widgetname)
{
    SDL_StyleWidget *widget = SDL_StyleGetWidgetEntry(style,widgetname);

    if(widget == NULL)
    {
        widget = malloc(sizeof(SDL_StyleWidget));
        memset(widget,0,sizeof(SDL_StyleWidget));
        widget->WidgetName = widgetname;
        SDL_LListAddToEnd(&style->WidgetStyles,&widget->ll);
    }
    return widget;
}

void SDL_StyleGetSizeInfo(void *w,int control,int state,SDL_Rect *dims)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
    SDL_Widget *widget = (SDL_Widget *)w;
    
    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style paint for widget %p\n",w);
        }
        else
        {
            if(s->ImageStyle)
            {
                T_Widget_StyleSizeInfo sizeinfo;
                draw=SDL_WidgetTypeGetStyleSizeInfo(widget->Type);
                sizeinfo(control,state,dims,s->ImageStyle);
            }
        }
    }


    if(elem && elem->style)
    {

        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));
        while(!s)
        {
            s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));
            elem = (SDL_StyleStackElem*)elem->ll.prev;
        }

        if(s->ImageStyle)
        {
            T_Widget_StyleSizeInfo sizeinfo;
            draw=SDL_WidgetTypeGetStyleSizeInfo(widget->Type);
            sizeinfo(control,state,dims,s->ImageStyle);
        }
    }
}


void SDL_StyleSetDrawFunction(SDL_Style *style,char *widgetname,T_Style_Paint paint)
{
    SDL_StyleWidget *widget = SDL_StyleWidgetEntry(style,widgetname);

    widget->PaintFunction   =  paint;
    widget->WidgetName      =  widgetname;
   
}


int SDL_StyleSetImage(SDL_Style *style,char *widgetname,int state,SDL_Image *image)
{
    T_Widget_StyleSetImage setimage = SDL_WidgetTypeGetStyleSetImage(SDL_WidgetTypeGet(widgetname));

    if(setimage)
        return setimage(style,state,image);

    return 0;
}

