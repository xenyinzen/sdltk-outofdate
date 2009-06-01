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
void SDL_StylePaint(SDL_Widget *w,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
    SDL_Widget *widget = (SDL_Widget *)w;

    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style paint for widget %p (%s)\n",widget,SDL_WidgetGetName(widget->Type));
        }
        else
        {
            if(s->ImageStyle)
            {
                T_Widget_StyleDraw draw;
                draw=SDL_WidgetTypeGetStyleDraw(widget->Type);
                if(draw)
                {
                    draw(widget,surface,Area,s->ImageStyle);
                    return;
                }
            }
            if(s->PaintFunction)
            {
                s->PaintFunction(widget,surface,Area);
                return;
            }
        }
    }


    if(elem && elem->style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));

        while(!s && elem)
        {
            s = SDL_StyleGetWidgetEntry(elem->style,SDL_WidgetGetName(widget->Type));
            elem = (SDL_StyleStackElem*)elem->ll.prev;
        }
        
        if(s->ImageStyle)
        {
            T_Widget_StyleDraw draw;
            draw=SDL_WidgetTypeGetStyleDraw(widget->Type);
            if(draw)
            {
                draw(widget,surface,Area,s->ImageStyle);
                return;
            }
        }
        if(s->PaintFunction)
        {
            s->PaintFunction(widget,surface,Area);
            return;
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

void SDL_StyleGetSizeInfo(SDL_Widget *widget,int control,int state,SDL_Rect *dims)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
    
    dims->x = 0;
    dims->y = 0;
    dims->w = 0;
    dims->h = 0;

    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style size info for widget %p (%s)\n",widget,SDL_WidgetGetName(widget->Type));
        }
        else
        {
            if(s->ImageStyle)
            {
                T_Widget_StyleSizeInfo sizeinfo;
                sizeinfo=SDL_WidgetTypeGetStyleSizeInfo(widget->Type);
                sizeinfo(widget,control,state,dims,s->ImageStyle);
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
            sizeinfo=SDL_WidgetTypeGetStyleSizeInfo(widget->Type);
            sizeinfo(widget,control,state,dims,s->ImageStyle);
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
        return setimage(style,widgetname,state,image);

    return 0;
}

SDL_Image *SDL_StyleGetImage(SDL_Widget *widget,int state)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
        
    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style get image for widget %p (%s)\n",widget,SDL_WidgetGetName(widget->Type));
        }
        else
        {
            if(s->ImageStyle)
            {
                T_Widget_StyleGetImage getimage;
                getimage=SDL_WidgetTypeGetStyleGetImage(widget->Type);
                return getimage(s->ImageStyle,state);
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
            T_Widget_StyleGetImage getimage;
            getimage=SDL_WidgetTypeGetStyleGetImage(widget->Type);
            return getimage(s->ImageStyle,state);
        }
    }
    return NULL;
}

static SDL_StyleImage *SDL_StyleGetImageStyle(SDL_Widget *widget)
{
    SDL_StyleStackElem *elem = (SDL_StyleStackElem*)SDL_StyleStack.last;
        
    if(widget->Style)
    {
        SDL_StyleWidget *s = SDL_StyleGetWidgetEntry(widget->Style,SDL_WidgetGetName(widget->Type));
        if(s == NULL)
        {
            printf("Error in style get image style for widget %p (%s)\n",widget,SDL_WidgetGetName(widget->Type));
        }
        else
        {
            return s->ImageStyle;
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
        return s->ImageStyle;
    }
    return NULL;
}

SDL_Image *SDL_StyleImageGetImage(SDL_Widget *widget,SDL_StyleImage *style,int state)
{
    T_Widget_StyleGetImage getimage;
    getimage=SDL_WidgetTypeGetStyleGetImage(widget->Type);
    if(getimage)
        return getimage(style,state);
    return NULL;
}

SDL_ImageTransition *SDL_StyleImageGetTransition(SDL_Widget *widget,SDL_StyleImage *style,int state)
{
    T_Widget_StyleGetTransition gettransition;
    gettransition=SDL_WidgetTypeGetStyleGetTransition(widget->Type);
    if(gettransition)
        return gettransition(style,state);
    return NULL;
}

void SDL_StyleTransition(SDL_Widget *widget,int from_state, int to_state)
{
    SDL_Image				*from_image;
    SDL_Image				*to_image;
    SDL_ImageTransition		*transition;
    SDL_StyleImage			*style;
	
    style =  SDL_StyleGetImageStyle(widget);
    if(style)
    {
        from_image = SDL_StyleImageGetImage(widget,style,from_state);
        to_image   = SDL_StyleImageGetImage(widget,style,to_state);
        transition = SDL_StyleImageGetTransition(widget,style,to_state);
#if 0        
        if(from_image == to_image)
        {
            SDL_WidgetRedrawEvent(widget);
            return;
        }
#endif
        if(transition)
        {
            transition->Client = (void*)widget;
            transition->delay  = 10;
            transition->time   = 3000;
            SDL_ImageStartTransition(from_image,to_image,transition);
        }
        else
        {
            SDL_WidgetRedrawEvent(widget);
        }

    }
    else
    {
        SDL_WidgetRedrawEvent(widget);
    }
}


void SDL_StyleHitTest(SDL_Widget *widget,SDL_Event *event,int *subcontrol)
{
    T_Widget_StyleHitTest hittest;
    SDL_StyleImage        *style;
	
    style =  SDL_StyleGetImageStyle(widget);


    hittest=SDL_WidgetTypeGetStyleHitTest(widget->Type);
    if(hittest)
        hittest(widget,event,style,subcontrol);
}

