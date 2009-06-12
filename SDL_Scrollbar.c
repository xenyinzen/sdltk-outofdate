/*
  SDLTk

  Scrollbar widget, this widget is drawn completely at the moment
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
#include <stdlib.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Scrollbar.h"

static void SDL_ScrollbarSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollbarSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollbarSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollbarSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ScrollbarSignalRealize(SDL_Object *object,void *signaldata,void *userdata);

static int SDL_ScrollbarStep(SDL_Scrollbar *Scrollbar,int direction,int step);
static int SDL_ScrollbarPixeloffset(SDL_Scrollbar *Scrollbar);

static void SDL_ScrollbarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_ScrollbarStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *s);
int SDL_ScrollbarStyleSetImage(SDL_Style *style,char *widgetname,int state,SDL_Image *image);
void SDL_ScrollbarStyleSizeInfo(SDL_Widget *widget,int control,int state,SDL_Rect *dims,SDL_StyleImage* StyleImage);
void  SDL_ScrollbarStyleHitTest(SDL_Widget *widget,SDL_Event *event,SDL_StyleImage *,int *subcontrol);

#define SCROLLSIZE 16

SDL_WidgetType SDL_HorizontalScrollbarGetType()
{
    SDL_WidgetType scrollbar;
  
    scrollbar = SDL_WidgetTypeGet("SDL_HorizontalScrollbar");
    
    if(scrollbar == 0)
    {
		
        SDL_WidgetFunctions *func;
		 
        scrollbar=SDL_WidgetTypeRegister("SDL_HorizontalScrollbar",&func);
        func->draw          = SDL_ScrollbarDraw;
        func->styledraw     = SDL_ScrollbarStyleDraw;
        func->stylesetimage = SDL_ScrollbarStyleSetImage;
        func->stylesizeinfo = SDL_ScrollbarStyleSizeInfo;
        func->stylehittest  = SDL_ScrollbarStyleHitTest;
    }
    return scrollbar;
}

SDL_WidgetType SDL_VerticalScrollbarGetType()
{
    SDL_WidgetType scrollbar;
  
    scrollbar = SDL_WidgetTypeGet("SDL_VerticalScrollbar");
    
    if(scrollbar == 0)
    {
		
        SDL_WidgetFunctions *func;
		 
        scrollbar=SDL_WidgetTypeRegister("SDL_VerticalScrollbar",&func);
        func->draw          = SDL_ScrollbarDraw;
        func->styledraw     = SDL_ScrollbarStyleDraw;
        func->stylesetimage = SDL_ScrollbarStyleSetImage;
        func->stylesizeinfo = SDL_ScrollbarStyleSizeInfo;
        func->stylehittest  = SDL_ScrollbarStyleHitTest;
    }
    return scrollbar;
}



SDL_Widget* SDL_ScrollbarNew()
{
    SDL_Object *object;
    SDL_Scrollbar *scrollbar;
    SDL_Widget *widget;

    scrollbar = (SDL_Scrollbar*) malloc (sizeof(SDL_Scrollbar));
    memset(scrollbar,0,sizeof(SDL_Scrollbar));

    object    = (SDL_Object*) scrollbar;
    widget    = (SDL_Widget*) scrollbar;
    
    widget->Type      = SDL_HorizontalScrollbarGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;

    scrollbar->State        = SCROLLBAR_IDLE;

    /* default range settings */
    scrollbar->MinValue           = 0;
    scrollbar->MaxValue           = 100;
    scrollbar->CurrentValue       = 0;
    scrollbar->NormalStepValue    = 50;
    scrollbar->SmallStepValue     = 10;

    scrollbar->PixelValue         = 0;
    scrollbar->Orientation        = VERTICAL;

    SDL_SignalConnect(widget, "keydown",        SDL_ScrollbarSignalKeyDown,         widget);
    SDL_SignalConnect(widget, "mousebuttondown",SDL_ScrollbarSignalMouseButtonDown, widget);
    SDL_SignalConnect(widget, "mousebuttonup",  SDL_ScrollbarSignalMouseButtonUp,   widget);
    SDL_SignalConnect(widget, "mousemotion",    SDL_ScrollbarSignalMouseMotion,     widget);
    SDL_SignalConnect(widget, "realize",        SDL_ScrollbarSignalRealize,         widget);

    return (SDL_Widget*)scrollbar;
}


void SDL_ScrollbarDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_StylePaint(widget,surface,Area);
}




int SDL_ScrollbarGetCurrentValue(SDL_Widget *widget)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;

    return Scrollbar->CurrentValue;
    
}

int SDL_ScrollbarSetMaxValue(SDL_Widget *widget,int MaxValue)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;

    if(MaxValue == 0)
        return 0;

    if(MaxValue < Scrollbar->CurrentValue)
    {
        Scrollbar->CurrentValue = MaxValue;
        SDL_SignalEmit(widget,"value-changed",widget);
    }

    Scrollbar->MaxValue     = MaxValue;

    SDL_ScrollbarPixeloffset(Scrollbar);
    SDL_WidgetRedrawEvent(widget);
    return 1;
}

void SDL_ScrollbarSetCurrentValue(SDL_Widget *widget,int CurrentValue)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;
    if(CurrentValue < 0)
        CurrentValue = 0;

    if(CurrentValue >= Scrollbar->MaxValue)
        CurrentValue = Scrollbar->MaxValue - 1;

    if(Scrollbar->CurrentValue != CurrentValue)
    {
        Scrollbar->CurrentValue = CurrentValue;
        Scrollbar->PixelValue   = Scrollbar->CurrentValue * widget->Rect.h / Scrollbar->MaxValue;

        SDL_SignalEmit(widget,"value-changed",widget);
        SDL_WidgetRedrawEvent(widget);
    }
}

static void SDL_ScrollbarSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata)
{


}

int SDL_ScrollbarSetImage(SDL_Widget *widget,int which,SDL_Image *image)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)widget;
    if(image == NULL)
        return 0;
    
    if(widget->Style == NULL)
        widget->Style = SDL_StyleNew();
    
    if(Scrollbar->Orientation == HORIZONTAL)
        SDL_ScrollbarStyleSetImage(widget->Style,"SDL_HorizontalScrollbar",which,image);
    else
        SDL_ScrollbarStyleSetImage(widget->Style,"SDL_VerticalScrollbar",which,image);
    return 0;
}

static void SDL_ScrollbarSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)object;
    SDL_Widget *widget =(SDL_Widget*)object;
    SDL_Event *Event = (SDL_Event *)signaldata;

    if(!SDL_WidgetIsInside(widget,Event->motion.x,Event->motion.y))
        return;


    if(Scrollbar->Orientation == VERTICAL)
    {
        int ymotion;
        int height;

        ymotion = Event->motion.y;
        height = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */
        if(height < 6)
            height = 6;

        if(Event->button.button == SDL_BUTTON_LEFT)
        {
            /* if the button click is below the button */
            if(ymotion > (widget->Rect.y + Scrollbar->PixelValue + height))
            {
                SDL_ScrollbarStep(Scrollbar,DOWN,NORMAL_STEP);
            }
            /* or on the button */
            else if (ymotion > widget->Rect.y + Scrollbar->PixelValue &&
                     ymotion < widget->Rect.y + Scrollbar->PixelValue + height)
            {
                Scrollbar->offset = ymotion - widget->Rect.y - Scrollbar->PixelValue;
                Scrollbar->State = SCROLLBAR_DRAG;
            }
            else
            {
                SDL_ScrollbarStep(Scrollbar,UP,NORMAL_STEP);
            }
            
        }
        else if( SDL_WidgetIsInside( widget, Event->motion.x, ymotion))
        {
                if( Event->button.button == 4)
                {
                        /* up  */
                        SDL_ScrollbarStep(Scrollbar,UP,NORMAL_STEP);
                }
                else if( Event->button.button == 5)
                {
                        /*down */
                        SDL_ScrollbarStep(Scrollbar,DOWN,NORMAL_STEP);
                }                 
        }

    }
    else
    {
        int xmotion;
        int ymotion;
        int width;

        xmotion = Event->motion.x - widget->Rect.x;
        ymotion = Event->motion.y - widget->Rect.y;
        width = widget->Rect.w  *Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */
        if(width < 6)
            width = 6;

        if(Event->button.button == SDL_BUTTON_LEFT)
        {
            SDL_Rect rect;
            int subc;
            

            SDL_StyleHitTest(widget,Event,&subc);
            SDL_StyleGetSizeInfo(widget,SCROLLBAR_THUMB,STYLE_STATE_NORMAL,&rect);
            switch(subc)
            {

            case SCROLLBAR_ARROW1:
                SDL_ScrollbarStep(Scrollbar,LEFT,SMALL_STEP);
                break;
            case SCROLLBAR_ARROW2:
                SDL_ScrollbarStep(Scrollbar,RIGHT,SMALL_STEP);
                break;
            case SCROLLBAR_SCROLLBAR:
                if(xmotion < rect.x)
                    SDL_ScrollbarStep(Scrollbar,LEFT,NORMAL_STEP);
                else
                    SDL_ScrollbarStep(Scrollbar,RIGHT,NORMAL_STEP);
                break;
            case SCROLLBAR_THUMB:
                printf("Clicked inside the thumb\n");
                break;
            }
        }
    }
}


static void SDL_ScrollbarSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)object;

    Scrollbar->offset = 0;
    Scrollbar->State  = SCROLLBAR_IDLE;
}

static void SDL_ScrollbarSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)object;    
    SDL_Widget *widget=(SDL_Widget*)object;
    SDL_Event *Event=(SDL_Event*)signaldata;


    if(Scrollbar->State == SCROLLBAR_DRAG)
    {
        if(Scrollbar->Orientation == VERTICAL)
        {
            int height;
            height = widget->Rect.h * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */
            if(height < 6)
                height = 6;

            Scrollbar->PixelValue   = Event->motion.y - widget->Rect.y - Scrollbar->offset;

            if(Scrollbar->PixelValue >  widget->Rect.h - height)
                Scrollbar->PixelValue = widget->Rect.h - height;
            if(Scrollbar->PixelValue < 0)
                Scrollbar->PixelValue = 0;


            Scrollbar->CurrentValue = ((Scrollbar->MaxValue - Scrollbar->MinValue)* Scrollbar->PixelValue) 
                / (widget->Rect.h - height);

            if(Scrollbar->CurrentValue < 0)
                Scrollbar->CurrentValue = 0;            
            
            SDL_SignalEmit(widget,"value-changed",widget);
            SDL_WidgetRedrawEvent(widget);
        }
        else
        {
            int width;
            width = widget->Rect.w  *Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */

            if(width < 6)
                width = 6;
            
            Scrollbar->PixelValue   = Event->motion.x - widget->Rect.x - Scrollbar->offset;

            if(Scrollbar->PixelValue >  widget->Rect.w - width)
                Scrollbar->PixelValue = widget->Rect.w - width;
            
            if(Scrollbar->PixelValue < 0)
                Scrollbar->PixelValue = 0;
            
            Scrollbar->CurrentValue = ((Scrollbar->MaxValue - Scrollbar->MinValue) * Scrollbar->PixelValue) 
                / (widget->Rect.w - width);
            
            if(Scrollbar->CurrentValue < 0)
                Scrollbar->CurrentValue = 0;
            SDL_SignalEmit(widget,"value-changed",widget);
            SDL_WidgetRedrawEvent(widget);

        }
    }

}

static void SDL_ScrollbarSignalRealize(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar*)object;
    SDL_Widget *widget = (SDL_Widget*)object;

    if(widget->Rect.w > widget->Rect.h)
    {
        widget->Type = SDL_HorizontalScrollbarGetType();
        Scrollbar->Orientation = HORIZONTAL;
    }
    else
    {
        widget->Type = SDL_VerticalScrollbarGetType();
        Scrollbar->Orientation = VERTICAL;
    }
}


static int SDL_ScrollbarStep(SDL_Scrollbar *Scrollbar,int direction,int step)
{

    switch(direction)
    {
    case UP:
    case LEFT:
        switch(step)
        {
        case NORMAL_STEP:
            Scrollbar->CurrentValue -= Scrollbar->NormalStepValue;
            break;
        case SMALL_STEP:
            Scrollbar->CurrentValue -= Scrollbar->SmallStepValue;
            break;
        }
        break;
    case DOWN:
    case RIGHT:
        switch(step)
        {
        case NORMAL_STEP:
            Scrollbar->CurrentValue += Scrollbar->NormalStepValue;
            break;
        case SMALL_STEP:
            Scrollbar->CurrentValue += Scrollbar->SmallStepValue;
            break;
        }
        break;
    }
    return SDL_ScrollbarPixeloffset(Scrollbar);
}

static int SDL_ScrollbarPixeloffset(SDL_Scrollbar *Scrollbar)
{
    SDL_Widget *widget=(SDL_Widget*)Scrollbar;
    int retval = 0;

    if(Scrollbar->CurrentValue < Scrollbar->MinValue)
        Scrollbar->CurrentValue = Scrollbar->MinValue;

    if(Scrollbar->CurrentValue > Scrollbar->MaxValue)
        Scrollbar->CurrentValue = Scrollbar->MaxValue;


    if(Scrollbar->Orientation == VERTICAL)
    {
        SDL_Rect arrow1;
        SDL_Rect arrow2;
        int height;
        int widget_height;

        SDL_StyleGetSizeInfo(widget,SCROLLBAR_ARROW1,STYLE_STATE_NORMAL,&arrow1);
        SDL_StyleGetSizeInfo(widget,SCROLLBAR_ARROW2,STYLE_STATE_NORMAL,&arrow2);

        widget_height = widget->Rect.h - arrow1.h - arrow2.h;
        height =  widget_height * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Width is the button height */
           
        if(height < 6)
            height = 6;

        Scrollbar->PixelValue = Scrollbar->CurrentValue * (widget_height - height) / Scrollbar->MaxValue;
        

        if(Scrollbar->PixelValue >  widget_height - height)
            Scrollbar->PixelValue = widget_height - height;

        retval = 1;

    }
    else
    {
        SDL_Rect arrow1;
        SDL_Rect arrow2;
        int widget_width;
        int width;
            
        SDL_StyleGetSizeInfo(widget,SCROLLBAR_ARROW1,STYLE_STATE_NORMAL,&arrow1);
        SDL_StyleGetSizeInfo(widget,SCROLLBAR_ARROW2,STYLE_STATE_NORMAL,&arrow2);
        widget_width = widget->Rect.w - arrow1.w - arrow2.w;
        width =  widget_width * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Width is the button height */

        if(width < 6)
            width = 6;

        Scrollbar->PixelValue = Scrollbar->CurrentValue * (widget_width - width) / Scrollbar->MaxValue;
        

        if(Scrollbar->PixelValue >  widget_width - width)
            Scrollbar->PixelValue = widget_width - width;

        retval = 1;

    }
    SDL_SignalEmit(widget,"value-changed",widget);
    SDL_WidgetRedrawEvent(widget);
    return retval;
}


int SDL_ScrollbarStyleSetImage(SDL_Style *style,char *widgetname,int id,SDL_Image *image)
{
    SDL_ScrollbarStyle *ScrollbarStyle;

    SDL_StyleWidget *widget = SDL_StyleWidgetEntry(style,widgetname);

    if(widget->ImageStyle == NULL)
    {
        widget->ImageStyle = malloc(sizeof(SDL_ScrollbarStyle));
        memset(widget->ImageStyle,0,sizeof(SDL_ScrollbarStyle));
    }
    ScrollbarStyle = widget->ImageStyle;

    if(ScrollbarStyle->Normal == NULL)
    {
        ScrollbarStyle->Normal = malloc(sizeof(SDL_ScrollbarStyleState));
        memset(ScrollbarStyle->Normal,0,sizeof(SDL_ScrollbarStyleState));
    }

    switch(id)
    {
    case 0:
        if(ScrollbarStyle->Normal->Arrow1==NULL)
            ScrollbarStyle->Normal->Arrow1 = image;
        break;
    case 1:
        if(ScrollbarStyle->Normal->Arrow2==NULL)
            ScrollbarStyle->Normal->Arrow2 = image;
        break;
    case 2:
        if(ScrollbarStyle->Normal->Scrollbar==NULL)
            ScrollbarStyle->Normal->Scrollbar = image;
        break;
    case 3:
        if(ScrollbarStyle->Normal->Thumb ==NULL)
            ScrollbarStyle->Normal->Thumb = image;
        break;
    default:
        break;
    }
    return 1;
}

static void SDL_ScrollbarStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *s)
{
    int widget_width;
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar *)widget;
    SDL_ScrollbarStyle *style = (SDL_ScrollbarStyle*)s;

    SDL_Rect thumb;
    SDL_Rect scrollbar;
    SDL_Rect arrow1;
    SDL_Rect arrow2;

    scrollbar.x = 0;
    scrollbar.y = 0;
    scrollbar.w = widget->Rect.w;
    scrollbar.h = widget->Rect.h;
    SDL_ImageBlitToSurface(style->Normal->Scrollbar,NULL,surface,&scrollbar);

    arrow1.x = 0;
    arrow1.y = 0;
    arrow1.w = SDL_ImageGetWidth(style->Normal->Arrow1);
    arrow1.h = SDL_ImageGetHeight(style->Normal->Arrow1);
    SDL_ImageBlitToSurface(style->Normal->Arrow1,NULL,surface,&arrow1);
    
    
    if(Scrollbar->Orientation == HORIZONTAL)
    {
        arrow2.x = widget->Rect.w - style->Normal->Arrow2->surface->w;
        arrow2.y = 0;
        arrow2.w = style->Normal->Arrow2->surface->w;
        arrow2.h = style->Normal->Arrow2->surface->h;
        SDL_ImageBlitToSurface(style->Normal->Arrow2,NULL,surface,&arrow2);
    }
    else
    {
        arrow2.x = 0;
        arrow2.y = widget->Rect.h - SDL_ImageGetHeight(style->Normal->Arrow2);
        arrow2.w = style->Normal->Arrow2->surface->w;
        arrow2.h = style->Normal->Arrow2->surface->h;
        SDL_ImageBlitToSurface(style->Normal->Arrow2,NULL,surface,&arrow2);
    }

    if(Scrollbar->Orientation == HORIZONTAL)
    {
        widget_width = widget->Rect.w - style->Normal->Arrow1->surface->w - style->Normal->Arrow2->surface->w;
        thumb.x = arrow1.w + Scrollbar->PixelValue;       
        thumb.y = 0;
        thumb.w = widget_width * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Width is the button height */;
        thumb.h = style->Normal->Thumb->surface->h;
        SDL_ImageBlitToSurface(style->Normal->Thumb,NULL,surface,&thumb);
    }
    else
    {
        widget_width = widget->Rect.h - style->Normal->Arrow1->surface->h - style->Normal->Arrow2->surface->h;
        thumb.x = 0;
        thumb.y = arrow1.h + Scrollbar->PixelValue;       
        thumb.w = style->Normal->Thumb->surface->w;
        thumb.h = widget_width * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Width is the button height */;
        SDL_ImageBlitToSurface(style->Normal->Thumb,NULL,surface,&thumb);

    }
}

void SDL_ScrollbarStyleSizeInfo(SDL_Widget *widget,int control,int state,SDL_Rect *dims,SDL_StyleImage* StyleImage)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar *)widget;
    SDL_ScrollbarStyle *ScrollbarStyle = (SDL_ScrollbarStyle*)StyleImage;
    SDL_ScrollbarStyleState *State;
    dims->x = 0;
    dims->y = 0;
    dims->w = 0;
    dims->h = 0;

    switch(state)
    {
    case STYLE_STATE_NORMAL:
        State = ScrollbarStyle->Normal;
        break;
    case STYLE_STATE_PRESSED:
        State = ScrollbarStyle->Pressed;
        break;
    case STYLE_STATE_HIGHLIGHTED:
        State = ScrollbarStyle->Highlighted;
        break;
    case STYLE_STATE_DISABLED:
        State = ScrollbarStyle->Disabled;
        break;
    }

    if(State != NULL)
    {
        switch(control)
        {
        case SCROLLBAR_ARROW1:
            if(State->Arrow1)
            {
                dims->w = State->Arrow1->surface->w;
                dims->h = State->Arrow1->surface->h;
            }
            break;
        case SCROLLBAR_ARROW2:
            if(State->Arrow2)
            {
                dims->w = State->Arrow2->surface->w;
                dims->h = State->Arrow2->surface->h;
            }
            break;
        case SCROLLBAR_SCROLLBAR:
            if(State->Scrollbar)
            {
                dims->w = State->Scrollbar->surface->w;
                dims->h = State->Scrollbar->surface->h;            
            }
            break;
        case SCROLLBAR_THUMB:
            if(State->Thumb)
            {
                if(State->Arrow1)
                    dims->x += SDL_ImageGetHeight(State->Arrow1);
                dims->x += Scrollbar->PixelValue;
                dims->w = State->Thumb->surface->w;
                dims->h = State->Thumb->surface->h;                      
            }
            break;
        }
    }
}

void  SDL_ScrollbarStyleHitTest(SDL_Widget *widget,SDL_Event *event,SDL_StyleImage *StyleImage,int *subcontrol)
{
    SDL_Scrollbar *Scrollbar=(SDL_Scrollbar *)widget;
    SDL_ScrollbarStyle *ScrollbarStyle = (SDL_ScrollbarStyle*)StyleImage;
    SDL_ScrollbarStyleState *State;
    int xmotion = event->motion.x - widget->Rect.x;

    if(ScrollbarStyle)
    {
        State = ScrollbarStyle->Normal;
        
        if(State)
        {
            if(State->Arrow1)
            {
                if(Scrollbar->Orientation == HORIZONTAL)
                {
                    if(xmotion < SDL_ImageGetWidth(State->Arrow1))
                    {
                        *subcontrol = SCROLLBAR_ARROW1;
                        return;
                    }
                }
                else
                    ;
            }
        
            if(State->Arrow2)
            {
                if(Scrollbar->Orientation == HORIZONTAL)
                {
                    if(xmotion > widget->Rect.w - SDL_ImageGetWidth(State->Arrow2))
                    {
                        *subcontrol = SCROLLBAR_ARROW2;
                        return;
                    }
                }
                else
                    ;
            }
        

            if(State->Thumb)
            {
                if(Scrollbar->Orientation == HORIZONTAL)
                {
                    int width;
                    width = widget->Rect.w  * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */
                
                    if(width < 6)
                        width = 6;
                    
                    if(State->Arrow1)
                        xmotion -= SDL_ImageGetWidth(State->Arrow1);

                    if(xmotion >= Scrollbar->PixelValue && xmotion <= Scrollbar->PixelValue + width)
                        *subcontrol = SCROLLBAR_THUMB;
                    else
                        *subcontrol = SCROLLBAR_SCROLLBAR;
                }
                else
                    ;
            }
        }
    
    }
    else
    {
        int width;
        width = widget->Rect.w  * Scrollbar->NormalStepValue / Scrollbar->MaxValue;   /* Height is the button height */
                
        if(width < 6)
            width = 6;
        /* if the button click is right of the thumb */
        if(xmotion > (widget->Rect.x + Scrollbar->PixelValue + width))
        {
            SDL_ScrollbarStep(Scrollbar,RIGHT,NORMAL_STEP);
        }
        /* or on the thumb */
        else if (xmotion > widget->Rect.x + Scrollbar->PixelValue &&
                 xmotion < widget->Rect.x + Scrollbar->PixelValue + width)
        {
            Scrollbar->offset = xmotion - widget->Rect.x - Scrollbar->PixelValue;
            Scrollbar->State = SCROLLBAR_DRAG;
        }
        else
        {
            /* left of the thumb */
            SDL_ScrollbarStep(Scrollbar,LEFT,NORMAL_STEP);
        }
    }
}

void SDL_ScrollbarClose(SDL_Widget *widget)
{
        SDL_Scrollbar *Scroll = (SDL_Scrollbar *)widget;
        free( Scroll);
}