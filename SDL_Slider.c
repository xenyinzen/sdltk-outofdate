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
#include <stdlib.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Slider.h"

static void SDL_SliderStep(SDL_Slider *slider,int direction,int step);
static int  SDL_SliderPixeloffset(SDL_Slider *Slider);
static void SDL_SliderCurrentValue(SDL_Slider *Slider);
static void SDL_SliderPaint(SDL_Slider *Slider,SDL_Surface *surface);

static void SDL_SliderEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_SliderEventMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_SliderEventMouseMotion(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_SliderDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_SliderGetType()
{
    SDL_WidgetType slider;

    slider = SDL_WidgetTypeGet("SDL_Slider");
    if(slider == 0)
    {
        SDL_WidgetFunctions *func;
        /* Slider */
        SDL_SignalNew("value-changed");
        slider=SDL_WidgetTypeRegister("SDL_Slider",&func);

        func->draw        = SDL_SliderDraw;
		

    }
    
    return slider;
}

SDL_Widget* SDL_SliderNew()
{
    SDL_Object *object;
    SDL_Slider *slider;
    SDL_Widget *widget;

    slider = (SDL_Slider*) malloc (sizeof(SDL_Slider));
    memset(slider,0,sizeof(SDL_Slider));

    object = (SDL_Object*) slider;
    widget = (SDL_Widget*) slider;
    
    widget->Type      = SDL_SliderGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;


    SDL_SignalConnect(widget,"mousebuttondown",SDL_SliderEventMouseButtonDown,widget);
    SDL_SignalConnect(widget,"mousebuttonup",SDL_SliderEventMouseButtonUp,widget);
    SDL_SignalConnect(widget,"mousemotion",SDL_SliderEventMouseMotion,widget);

    slider->SliderBar   =NULL;
    
    slider->percentage   = 0; 
    slider->SliderButton        = NULL;
    slider->SliderButtonPressed = NULL;
    slider->pixeloffset  = 0;
    slider->state        = SLIDER_IDLE;
    slider->changed = 0;


    slider->MinValue           = 0;
    slider->MaxValue           = 100;
    slider->CurrentValue       = 0;

    slider->ValueLocked        = 0;

    slider->NormalStepSize     = 5;
    slider->orientation        = HORIZONTAL;
    slider->SliderButtonWidth  = 50;
    
    return (SDL_Widget*)slider;
}


void SDL_SliderDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{

    SDL_Slider *Slider=(SDL_Slider*)widget;
    SDL_Rect   button;
    int x_offset=0;
    int y_offset=0;

    if(widget->Rect.w > widget->Rect.h)
        Slider->orientation        = HORIZONTAL;
    else
        Slider->orientation        = VERTICAL;


    if(Slider->SliderButton == NULL)
    {
        SDL_SliderPaint(Slider,surface);
        return;
    }
    Slider->SliderButtonWidth =  widget->Rect.w * Slider->NormalStepSize * 2 / (Slider->MaxValue - Slider->MinValue);

    if(Slider->SliderButtonWidth < Slider->SliderButton->surface->w)
        Slider->SliderButtonWidth = Slider->SliderButton->surface->w;

    if(Slider->SliderBar)
    {
        SDL_ImageBlitToSurface(Slider->SliderBar,NULL,surface,&widget->Rect);
    }

    if(Slider->orientation == HORIZONTAL)
    {
        x_offset = Slider->pixeloffset;
    }
    else
    {
        y_offset = Slider->pixeloffset;
        x_offset = (widget->Rect.w - Slider->SliderButtonWidth) ;
    }

    if(Slider->SliderButton == NULL)
    {


    }
    else
    {
        button.x = x_offset ;
        button.y = y_offset;
        button.w = Slider->SliderButtonWidth;
        button.h = Slider->SliderButton->surface->h;

        if(Slider->state && Slider->SliderButtonPressed)
            SDL_ImageBlitToSurface(Slider->SliderButtonPressed,NULL,surface,&button);
        else
            SDL_ImageBlitToSurface(Slider->SliderButton,NULL,surface,&button);
    }


}


int SDL_SliderSetImage(SDL_Widget *widget,int which,SDL_Image *image)
{
    SDL_Slider *Slider=(SDL_Slider*)widget;


    if(image == NULL || Slider == NULL)
        return 0;

    switch(which)
    {
    case SLIDER_IMAGE_NORMAL_THUMB:
        if(Slider->SliderButton == NULL)
        {
            Slider->SliderButton = image;
            Slider->SliderButtonWidth =  widget->Rect.w * Slider->NormalStepSize * 2 / (Slider->MaxValue - Slider->MinValue);
            if(Slider->SliderButtonWidth < image->surface->w)
                Slider->SliderButtonWidth = image->surface->w;
            return 1;
        }
        return 0;
        break;
    case SLIDER_IMAGE_NORMAL_BAR:
        if(Slider->SliderBar == NULL)
        {
            Slider->SliderBar = image;
            return 1;
        }
        return 0;
        break;
    case SLIDER_IMAGE_PRESSED_THUMB:
        if(Slider->SliderButtonPressed == NULL)
        {
            Slider->SliderButtonPressed = image;
            return 1;
        }
        return 0;
        break;
    default:
        break;
    }
    return 1;
}

/* Internal helper functions */
static void SDL_SliderStep(SDL_Slider *Slider,int direction,int step)
{
    switch(direction)
    {
    case UP:
    case LEFT:
        Slider->CurrentValue -= Slider->NormalStepSize;
        Slider->changed=SDL_SliderPixeloffset(Slider);
        break;
    case DOWN:
    case RIGHT:
        Slider->CurrentValue += Slider->NormalStepSize;
	Slider->changed=SDL_SliderPixeloffset(Slider);
        break;
    }

}

static int SDL_SliderPixeloffset(SDL_Slider *Slider)
{
    SDL_Widget *widget=(SDL_Widget*)Slider;
    int old = Slider->pixeloffset;
    int retval = 0;

    if(Slider->CurrentValue < Slider->MinValue)
        Slider->CurrentValue = Slider->MinValue;

    if(Slider->CurrentValue > Slider->MaxValue)
        Slider->CurrentValue = Slider->MaxValue;

    if(Slider->orientation == HORIZONTAL)
    {
        if(Slider->SliderButton)
        {
            Slider->pixeloffset = ((widget->Rect.w - Slider->SliderButtonWidth) * Slider->CurrentValue) 
                / (Slider->MaxValue - Slider->MinValue);

            if(old != Slider->pixeloffset)
                retval=1;
        }
    }
    else
    {
        if(Slider->SliderButton)
        {
            Slider->pixeloffset = ((widget->Rect.h - Slider->SliderButton->surface->h) * Slider->CurrentValue) 
                / (Slider->MaxValue - Slider->MinValue);

	    if(old!= Slider->pixeloffset)
                retval=1;
           
        }
    }
    return retval;
}



static void SDL_SliderCurrentValue(SDL_Slider *Slider)
{
    SDL_Widget *widget=(SDL_Widget*)Slider;
    /* First do a check on the current value of pixeloffset */

    if(Slider->pixeloffset < 0)
        Slider->pixeloffset = 0;
    
    /* then recalculate CurrentValue */
    if(Slider->SliderButton)
    {
        if(Slider->orientation == HORIZONTAL)
        {
            if(Slider->pixeloffset > (widget->Rect.w - Slider->SliderButtonWidth))
                Slider->pixeloffset = (widget->Rect.w - Slider->SliderButtonWidth) ;

            Slider->CurrentValue = (int)((Slider->MaxValue -Slider->MinValue)* Slider->pixeloffset) 
                /(int)(widget->Rect.w - Slider->SliderButtonWidth);
        }
        else
        {
            if(Slider->pixeloffset > (widget->Rect.h - Slider->SliderButton->surface->h))
                Slider->pixeloffset = (widget->Rect.h - Slider->SliderButton->surface->h);

            Slider->CurrentValue =((int)((Slider->MaxValue -Slider->MinValue)* Slider->pixeloffset) 
                                   / (int)(widget->Rect.h - Slider->SliderButton->surface->h));
        }
    }
}

static void SDL_SliderPaint(SDL_Slider *Slider,SDL_Surface *surface)
{
    SDL_Widget *widget = (SDL_Widget*)Slider;


    if(Slider->orientation == VERTICAL)
    {   
        SDL_Rect R;
        int x,y,height,width;

        R.x = 0;
        R.y = 0;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xf4,0xf0,0xe8));
            
        R.x = widget->Rect.x;
        R.y = widget->Rect.y + Slider->pixeloffset;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h / Slider->MaxValue;
        
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xd4,0xd0,0xc8));

        x      = widget->Rect.x;
        y      = widget->Rect.y + Slider->pixeloffset;
        width  = widget->Rect.w;
        height = widget->Rect.h/Slider->MaxValue;

        if(height < 6)
        {
            height = 6;
        }
            
        {
            SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1,0x404040ff);
            SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1,0x404040ff);
                
            SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2,0x808080ff);
            SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2,0x808080ff);
                
            SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1,0xffffffff);
            SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2,0xffffffff);
                
            SDL_DrawLine (surface,x, y, x + width - 1, y,0xd4d0c8ff);
            SDL_DrawLine (surface,x, y, x, y + height - 1,0xd4d0c8ff);       
        }
    }
    else
    {
        SDL_Rect R;
        int x,y,height,width;

        printf("Paint\n");

        R.x = 0;
        R.y = 0;
        R.w = widget->Rect.w;
        R.h = widget->Rect.h;
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xf4,0xf0,0xe8));
            
        R.x = Slider->pixeloffset;
        R.y = 0;
        R.w = widget->Rect.w/Slider->MaxValue;
        R.h = widget->Rect.h;
        
        SDL_FillRect(surface,&R,SDL_MapRGB(surface->format,0xd4,0xd0,0xc8));

        x      = Slider->pixeloffset;
        y      = 0;
        width  = widget->Rect.w * Slider->NormalStepSize / Slider->MaxValue;
        height = widget->Rect.h;

        if(width < 6)
        {
            width = 6;
        }
            
        {
            SDL_DrawLine (surface,x, y + height - 1, x + width - 1, y + height - 1,0x404040ff);
            SDL_DrawLine (surface,x + width - 1, y, x + width - 1, y + height - 1,0x404040ff);
                
            SDL_DrawLine (surface,x + 1, y + height - 2, x + width - 2, y + height - 2,0x808080ff);
            SDL_DrawLine (surface,x + width - 2, y + 1, x + width - 2, y + height - 2,0x808080ff);
                
            SDL_DrawLine (surface,x + 1, y + 1, x + width - 2, y + 1,0xffffffff);
            SDL_DrawLine (surface,x + 1, y + 1, x + 1, y + height - 2,0xffffffff);
                
            SDL_DrawLine (surface,x, y, x + width - 1, y,0xd4d0c8ff);
            SDL_DrawLine (surface,x, y, x, y + height - 1,0xd4d0c8ff);       
        }


    }


}

int SDL_SliderSetMaxValue(SDL_Widget *widget,int MaxValue)
{
    return SDL_SliderSetValue(widget,SLIDER_MAX_VALUE,MaxValue);
}

int SDL_SliderSetValue(SDL_Widget *widget,int type,int value)
{
    SDL_Slider *Slider=(SDL_Slider*)widget;

    if(Slider == NULL)
        return 0;

    if(widget->Rect.w > widget->Rect.h)
        Slider->orientation        = HORIZONTAL;
    else
        Slider->orientation        = VERTICAL;

    switch(type)
    {
    case SLIDER_CURRENT_VALUE:
        /* The currentvalue is used as an integer and a new pixeloffset */
        /* is calculated but the size of the button is not used yet     */
        /* because it doesn't have to be available at this moment       */
        
        if(value != Slider->CurrentValue)
        {
            Slider->CurrentValue = value;
            SDL_SliderPixeloffset(Slider);
            if(Slider->CurrentValue > Slider->MaxValue)
                Slider->CurrentValue = Slider->MaxValue;
            SDL_SignalEmit(widget,"value-changed",NULL);
            SDL_WidgetRedrawEvent(widget);
        }
        break;
    case SLIDER_MIN_VALUE:
        Slider->MinValue = value;
        break;
    case SLIDER_MAX_VALUE:
        Slider->MaxValue = value;
        if(Slider->CurrentValue > Slider->MaxValue)
        {
            Slider->CurrentValue = Slider->MaxValue;
            SDL_SignalEmit(widget,"value-changed",NULL);
            SDL_WidgetRedrawEvent(widget);
        }
        break;

    case SLIDER_NORMAL_STEP_SIZE:
        Slider->NormalStepSize = value;
        break;
    }
    return 1;
}

int SDL_SliderGetCurrentValue(SDL_Widget *widget)
{
    SDL_Slider *Slider=(SDL_Slider*)widget;
    return Slider->CurrentValue;
}

int SDL_SliderGetState(SDL_Widget *widget)
{
    SDL_Slider *Slider=(SDL_Slider*)widget;
    if(Slider == NULL)
        return 0;
    return Slider->state;
}


static void SDL_SliderEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Slider *Slider=(SDL_Slider*)object;
    SDL_Widget *widget= (SDL_Widget*)object;
    SDL_Event *event = (SDL_Event *)signaldata;


    if(Slider->SliderButton && SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        int motion;
 
        if(Slider->orientation == HORIZONTAL)
        {
            motion=event->motion.x;
                
            if(event->button.button == SDL_BUTTON_LEFT)
            {
                /* if the button click is on the left side of the button */
                if(motion < (widget->Rect.x + Slider->pixeloffset))
                {
                    SDL_SliderStep(Slider,LEFT,NORMAL_STEP);
                }
                /* the button click is on the right side off the button */
                else if (motion > widget->Rect.x + Slider->pixeloffset + Slider->SliderButtonWidth)
                {
                    SDL_SliderStep(Slider,RIGHT,NORMAL_STEP);
                }
                /* if the button click is on the button */
                else
                {
                    Slider->state=SLIDER_DRAG;
                    SDL_WidgetRedrawEvent(widget);
                    return;
                }
            }
            if(event->button.button == 4)
            {
                SDL_SliderStep(Slider,RIGHT,NORMAL_STEP);
            }
            if(event->button.button == 5)
            {
                SDL_SliderStep(Slider,LEFT,NORMAL_STEP);
            }
        }
        else /* Vertical orientation */
        {
            motion=event->motion.y;
                
            if(event->button.button == SDL_BUTTON_LEFT)
            {
                /* if the button click is below the button */
                if(motion > (widget->Rect.y + Slider->pixeloffset + Slider->SliderButton->surface->h))
                {
                    SDL_SliderStep(Slider,DOWN,NORMAL_STEP);
                }
                else if (motion > widget->Rect.y + Slider->pixeloffset)
                {
                    Slider->state=SLIDER_DRAG;
                }
                else
                {
                    SDL_SliderStep(Slider,UP,NORMAL_STEP);
                }
            }
            if(event->button.button == 4)
            {
                SDL_SliderStep(Slider,UP,NORMAL_STEP);
            }
            if(event->button.button == 5)
            {
                SDL_SliderStep(Slider,DOWN,NORMAL_STEP);
            }
        }
    }

    if(Slider->changed)
    {
        SDL_SignalEmit(widget,"value-changed",event);

        Slider->changed=0;
        SDL_WidgetRedrawEvent(widget);
    }

}

static void SDL_SliderEventMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Slider *Slider=(SDL_Slider*)object;
    SDL_Widget *widget= (SDL_Widget*)object;


    if(Slider->state == SLIDER_DRAG)
    {
        Slider->state   = SLIDER_IDLE;
        Slider->changed = 1;
    }

    if(Slider->changed)
    {
        SDL_SignalObjectEmit(object,"value-changed",NULL);

        Slider->changed=0;
        SDL_WidgetRedrawEvent(widget);
    }

}

static void SDL_SliderEventMouseMotion(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Slider *Slider=(SDL_Slider*)object;
    SDL_Event *event = (SDL_Event*)signaldata;
    SDL_Widget *widget= (SDL_Widget*)object;

    if(Slider->state == SLIDER_DRAG)
    {
        if(Slider->orientation == HORIZONTAL)
        {
            Slider->pixeloffset = event->motion.x - widget->Rect.x  - Slider->SliderButtonWidth;
        }
        else
        {
            Slider->pixeloffset = event->motion.y - widget->Rect.y - (Slider->SliderButton->surface->h/2);
        }
        SDL_SliderCurrentValue(Slider);
        Slider->changed=1;
    }
    if(Slider->changed)
    {

        Slider->changed=0;
        SDL_WidgetRedrawEvent(widget);
    }

}
