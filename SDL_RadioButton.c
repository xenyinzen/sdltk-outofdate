/*
  SDLTk

  Implementation of radio button
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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_RadioButton.h"

static void SDL_SignalRadioButtonMouseButtonDown(SDL_Object *Object,void *signaldata,void *userdata);
static void SDL_RadioButtonDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_RadioButtonStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *style);
static int  SDL_RadioButtonStyleSetImage(SDL_Style *style,char *widgetname,int state,SDL_Image *image);

SDL_WidgetType SDL_RadioButtonGetType()
{
    SDL_WidgetType radiobutton;
    
    radiobutton=SDL_WidgetTypeGet("SDL_RadioButton");
    
    if(radiobutton == 0)
    {
        SDL_WidgetFunctions *func;

        radiobutton         = SDL_WidgetTypeRegister("SDL_RadioButton",&func);
        func->draw          = SDL_RadioButtonDraw;
        func->styledraw     = SDL_RadioButtonStyleDraw;
        func->stylesetimage = SDL_RadioButtonStyleSetImage;
    }
    return radiobutton;
}



SDL_Widget* SDL_RadioButtonNew()
{
    SDL_Widget *widget;
    SDL_RadioButton  *radiobutton;
    
    radiobutton=(SDL_RadioButton*) malloc(sizeof(SDL_RadioButton));
    memset(radiobutton,0,sizeof(SDL_RadioButton));

    widget=(SDL_Widget*)radiobutton;

    widget->Type      = SDL_RadioButtonGetType();
    widget->Rect.x    = 0;
    widget->Rect.y    = 0;
    widget->Rect.w    = 0;
    widget->Rect.h    = 0;
    widget->flags     = 0;

    radiobutton->State = 1;

    SDL_SignalConnect(widget,"mousebuttondown",SDL_SignalRadioButtonMouseButtonDown,radiobutton);
    
    return widget;
}

void SDL_RadioButtonClose( SDL_Widget *widget )
{
	SDL_RadioButton *radio_button = (SDL_RadioButton *)widget;
	free(radio_button);
}

static void SDL_RadioButtonDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_StylePaint(widget,surface,Area);
}


static void SDL_SignalRadioButtonMouseButtonDown(SDL_Object *Object,void *signaldata,void *userdata)
{
    SDL_RadioButton *Button=(SDL_RadioButton*)Object;
    SDL_Event *event=(SDL_Event*)signaldata;
    
    if(SDL_WidgetIsInside((SDL_Widget*)Object,event->motion.x,event->motion.y))
    {
        Button->State = !Button->State;
        SDL_SignalObjectEmit(Object,"clicked",NULL);
        SDL_WidgetRedrawEvent((SDL_Widget*)Object);
    }
}


int SDL_RadioButtonSetText(SDL_Widget *widget,char *text)
{
    SDL_RadioButton *Button=(SDL_RadioButton*)widget;

    if(Button->Label == NULL)
    {
        Button->Label = SDL_LabelNew();
        SDL_WidgetSetDims(Button->Label,widget->Rect.x+ 20,widget->Rect.y,widget->Rect.w - 20,widget->Rect.h);
        SDL_LabelSetText(Button->Label,text);
        SDL_WidgetShow(Button->Label);
    }
    else
    {
        SDL_LabelSetText(Button->Label,text);
        
    }
    return 1;
}


int SDL_RadioButtonSetImage(SDL_Widget *widget,int which,SDL_Image *image)
{
    
    return 0;
}


static void SDL_RadioButtonStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *s)
{
    SDL_RadioButton  *RadioButton=(SDL_RadioButton*)widget;
    SDL_Image        *RadioButtonImage;
    SDL_Rect         r;

    SDL_RadioButtonStyle *style = s;

    RadioButtonImage = style->Normal;
    
    switch(RadioButton->State)
    {
    case 0:
        if(style->Normal)
            RadioButtonImage = style->Normal;
        break;
    case 1:
        if(style->Selected)
            RadioButtonImage = style->Selected;
        break;
    default:
        break;
    }
    r.x = 0;
    r.y = 0;
    r.w = RadioButtonImage->surface->w;
    r.h = RadioButtonImage->surface->h;

    SDL_ImageBlitToSurface(RadioButtonImage,NULL,surface,&r);
}

static int  SDL_RadioButtonStyleSetImage(SDL_Style *style,char *widgetname,int state,SDL_Image *image)
{
    SDL_RadioButtonStyle *RadioButtonStyle;

    SDL_StyleWidget *widget = SDL_StyleWidgetEntry(style,widgetname);

    if(widget->ImageStyle == NULL)
    {
        widget->ImageStyle = malloc(sizeof(SDL_RadioButtonStyle));
        memset(widget->ImageStyle,0,sizeof(SDL_RadioButtonStyle));
    }
    RadioButtonStyle = widget->ImageStyle;

    switch(state)
    {
    case 0:
        if(RadioButtonStyle->Normal==NULL)
            RadioButtonStyle->Normal = image;
        break;
    case 1:
        if(RadioButtonStyle->Selected == NULL)
            RadioButtonStyle->Selected = image;
        break;
    default:
        break;
    }

    return 1;
}

