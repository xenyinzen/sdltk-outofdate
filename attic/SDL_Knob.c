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

#include "SDLTk.h"
#include "SDL_Knob.h"


static void SDL_KnobSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_KnobDraw(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_KnobGetType()
{
    SDL_WidgetType knob;

    knob = SDL_WidgetTypeGet("SDL_Knob");
    if(knob == 0)
    {

        SDL_WidgetFunctions *func;
		
        knob=SDL_WidgetTypeRegister("SDL_Knob",&func);
        func->draw = SDL_KnobDraw;

    }
    return knob;
}

SDL_Widget* SDL_KnobNew()
{
    SDL_Widget *Widget;
    SDL_Knob   *Knob;

    Knob   = (SDL_Knob*)   malloc(sizeof(SDL_Knob));
    Widget = (SDL_Widget*) Knob;
    
    Widget->Type      = SDL_KnobGetType();
    Widget->Rect.x    = 0;
    Widget->Rect.y    = 0;
    Widget->Rect.w    = 0;
    Widget->Rect.h    = 0;
    Widget->flags     = 0;
 
    SDL_SignalConnect(Widget,"mousebuttondown",SDL_KnobSignalMouseButtonDown,Knob);

    Knob->Value  = 0;
    Knob->Min    = 0;
    Knob->Max    = 360;
    Knob->color  = WHITE;

    Knob->Radius    = 7;
    Knob->DotRadius = 2;

    Knob->StartAngle = -150; /* In degrees ,0 degrees is at the top of the circle*/
    Knob->AngleRange =  290; /* In degrees */

    return Widget;
}

void SDL_KnobDraw(SDL_Widget *Widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Knob *Knob=(SDL_Knob*)Widget;
    float xc,yc,angle,s,c,radius;
    int value;

    /* compute center */
    xc = (float)Widget->Rect.w / 2 + Widget->Rect.x;
    yc = (float)Widget->Rect.h / 2 + Widget->Rect.y;
    
    value = (Knob->AngleRange*Knob->Value)/Knob->Max;
    /* compute pointer coords */
    angle  = (float)Knob->StartAngle + value;
    angle  = (float)angle/180*(float)M_PI;
    radius = (float)Knob->Radius;

    s = (float)sin (angle);
    c = (float)cos (angle);

    SDL_DrawCircle(surface,
                   (Sint16)(xc + s * radius),
                   (Sint16)(yc - c * radius),
                   Knob->DotRadius,0xff0000ff);
}



static void SDL_KnobSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Knob *Knob =(SDL_Knob*)object;
    SDL_Event *Event = (SDL_Event*)signaldata;
    SDL_Widget *Widget =(SDL_Widget*)object;

    if(!SDL_WidgetIsInside(Widget,Event->motion.x,Event->motion.y))
        return;

    if(Event->button.button == 1)
        Knob->Value += 10;

    else if(Event->button.button == 3)
        Knob->Value -= 10;

    if(Knob->Value < 0)
        Knob->Value = 0;

    if(Knob->Value >  Knob->Max)
        Knob->Value = Knob->Max;

    SDL_SignalEmit(Widget,"value-changed",NULL);
    SDL_WidgetRedrawEvent(Widget);
}



int SDL_KnobGetCurrentValue(SDL_Widget *Widget)
{
    SDL_Knob *Knob =(SDL_Knob*)Widget;

    return Knob->Value;
  
}

int SDL_KnobSetValue(SDL_Widget *widget,int type,int value)
{
    SDL_Knob *Knob=(SDL_Knob*)widget;

    if(Knob == NULL)
        return 0;

    switch(type)
    {
    case KNOB_CURRENT_VALUE:
        if(value != Knob->Value)
        {
            Knob->Value = value;
            if(Knob->Value > Knob->Max)
                Knob->Value = Knob->Max;
            SDL_SignalEmit(widget,"value-changed",NULL);
            SDL_WidgetRedrawEvent(widget);
        }
        break;
    case KNOB_MAX_VALUE:
        Knob->Max = value;
        break;
    case KNOB_MIN_VALUE:
        Knob->Min = value;
        break;
    default:
        break;
    }

    return 1;
}
