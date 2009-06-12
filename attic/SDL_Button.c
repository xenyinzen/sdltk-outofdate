/*
  SDLTk

  Implementation of button widget (painted or image based)
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
#include <stdarg.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Button.h"

void SDL_ButtonDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
int SDL_ButtonSizeRequest(SDL_Widget *widget,SDL_Rect *size);


static void SDL_ButtonSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ButtonSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ButtonSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ButtonSignalRealize(SDL_Object *object,void *signaldata,void *userdata);
static void SDL_ButtonSignalShow(SDL_Object *object,void *signaldata,void *userdata);

static void SDL_ButtonStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *style);
static int SDL_ButtonStyleSetImage(SDL_Style *style,int state,SDL_Image *image);

SDL_WidgetType SDL_ButtonGetType()
{
    SDL_WidgetType button;
    button = SDL_WidgetTypeGet("SDL_Button");
   
    if(button == 0)
    {

        SDL_WidgetFunctions *func;
        button=SDL_WidgetTypeRegister("SDL_Button",&func);
        
        func->draw          = SDL_ButtonDraw;
        func->sizerequest   = SDL_ButtonSizeRequest;
        func->close         = SDL_ButtonClose;
        func->styledraw     = SDL_ButtonStyleDraw;
        func->stylesetimage = SDL_ButtonStyleSetImage;
    }
    return button;
}

SDL_Widget* SDL_ButtonNew()
{    
    SDL_Button *button;
    SDL_Widget *widget;
    SDL_Object *object;

    button=(SDL_Button*)malloc(sizeof(SDL_Button));
    memset(button,0,sizeof(SDL_Button));

    widget=(SDL_Widget*)button;
    widget->Type            = SDL_ButtonGetType();
    widget->flags           = 0;
    
    object=(SDL_Object*)widget;
    
    SDL_SignalConnect(widget,"mousebuttondown",SDL_ButtonSignalMouseButtonDown,widget);
    SDL_SignalConnect(widget,"mousebuttonup",  SDL_ButtonSignalMouseButtonUp,  widget);
    SDL_SignalConnect(widget,"mousemotion",    SDL_ButtonSignalMouseMotion,    widget);
    SDL_SignalConnect(widget,"realize",        SDL_ButtonSignalRealize,        widget);
    SDL_SignalConnect(widget,"show",           SDL_ButtonSignalShow,           widget);

    button->State        = SDL_BUTTON_STATE_UP;
    button->Label        = NULL;

    button->Font         = &DefaultFont;
    button->height       = 10;
   
    return (SDL_Widget*)button;
}


void SDL_ButtonDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_StylePaint(widget,surface,Area);
    return;
#if 0
    ButtonImage=Button->Style->Normal;

    switch(Button->State)
    {
    case SDL_BUTTON_STATE_HIGHLIGHTED:
        if(Button->Style->Highlighted)
            ButtonImage = Button->Style->Highlighted;
        break;
    case SDL_BUTTON_STATE_DOWN:
        if(Button->Style->Pressed)
            ButtonImage = Button->Style->Pressed;
        break;
    case SDL_BUTTON_STATE_FOCUSED:
        if(Button->Style->Focused)
            ButtonImage = Button->Style->Focused;
        break;
    default:
        break;
    }

    rect.x = 0;
    rect.y = 0;
    rect.w = widget->Rect.w;
    rect.h = widget->Rect.h;
    SDL_ImageBlitToSurface(ButtonImage,surface,&rect);           
#endif
}


static void SDL_ButtonSignalMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Button *Button = (SDL_Button*) object;
    SDL_Event  *event  = (SDL_Event *) signaldata;
    SDL_Widget *widget = (SDL_Widget*) object;

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y) &&
       event->button.button == 1)
    {
        Button->State = SDL_BUTTON_STATE_DOWN;
        SDL_SignalEmit(widget,"clicked",NULL);
        SDL_WidgetRedrawEvent(widget);
    }
    else
        Button->State = SDL_BUTTON_STATE_UP;        
}

static void SDL_ButtonSignalMouseButtonUp(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Button *Button=(SDL_Button*)object;
    SDL_Event *event=(SDL_Event *)signaldata;
    SDL_Widget *widget=(SDL_Widget*)object;

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        Button->State = SDL_BUTTON_STATE_HIGHLIGHTED;
        SDL_WidgetRedrawEvent(widget);
    }
    else
    {
        if(Button->State != SDL_BUTTON_STATE_UP)
        {
            Button->State = SDL_BUTTON_STATE_UP;
            SDL_WidgetRedrawEvent(widget);
        }
    }


}

static void SDL_ButtonSignalMouseMotion(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Button *Button=(SDL_Button*)object;
    SDL_Event *event = (SDL_Event *)signaldata;
    SDL_Widget *widget=(SDL_Widget*)object;    

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        if(Button->State == SDL_BUTTON_STATE_UP)
        {
            Button->State = SDL_BUTTON_STATE_HIGHLIGHTED;
#if 0
            if(Button->Style != NULL && Button->Style->Highlighted != NULL)
            {
                SDL_WidgetRedrawEvent(widget);
            }
#endif
        }
    }
}


void SDL_ButtonClose(SDL_Widget *widget)
{
    SDL_Button *Button=(SDL_Button*)widget;
    free(Button);
}





int SDL_ButtonSetLabel(SDL_Widget *widget,char *title)
{
    SDL_Button *Button=(SDL_Button*)widget;

    if(Button->Label == NULL)
    {
        Button->Label = SDL_LabelNew();
        SDL_WidgetAddChild(widget,Button->Label);
    }
    SDL_WidgetSetDims(Button->Label,widget->Rect.x+8,widget->Rect.y+3,widget->Rect.w-10,widget->Rect.h-5);

    SDL_LabelSetText(Button->Label,title);
    SDL_LabelSetFont(Button->Label,Button->Font);

    
    return 1;
}

int SDL_ButtonSetImage(SDL_Widget *widget,SDL_eButtonState state,SDL_Image *image)
{
    if(image == NULL)
        return 0;

    if(widget->Style == NULL)
        widget->Style = SDL_StyleNew();

    SDL_ButtonStyleSetImage(widget->Style,state,image);
    return 1;
}

int SDL_ButtonSetTransition(SDL_Widget *widget,SDL_eButtonState state,int how)
{
    

    return 0;
}

static void SDL_ButtonSignalRealize(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Button *Button=(SDL_Button*)object;
    SDL_Widget *widget=(SDL_Widget*)object;

    if(Button->Label != NULL)
    {
        SDL_WidgetSetDims(Button->Label,widget->Rect.x+8,  widget->Rect.y+3,
                          widget->Rect.w-10, widget->Rect.h-5);
    }   
    SDL_WidgetSizeRequest(widget,&widget->Rect);
}


static void SDL_ButtonSignalShow(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Button *Button=(SDL_Button*)object;
    
    if(Button->Label != NULL)
        SDL_WidgetShow(Button->Label);

}


int SDL_ButtonSizeRequest(SDL_Widget *widget,SDL_Rect *size)
{
    size->x = widget->Rect.x;
    size->y = widget->Rect.y;
	
    if(widget->Rect.w == 0)
    {
        size->w = 60;
    }

    if(widget->Rect.h == 0)
    {
        size->h = 18;
    }
    return 1;
}		


int SDL_ButtonSetFont(SDL_Widget *widget,SDL_Font *Font)
{
    SDL_Button *Button=(SDL_Button*)widget;
    SDL_Rect FontInfo;
    SDL_String *caption;

    Button->Font = Font;

    if(Button->Label)
    {
        caption = SDL_LabelGetText(Button->Label);

        SDL_FontSetHeight(Font,Button->height);
        SDL_FontCalcDims(Font,caption,&FontInfo);        

        SDL_WidgetResize(widget,FontInfo.w+25,FontInfo.h+16); 
        SDL_WidgetResize(Button->Label,FontInfo.w+14,FontInfo.h+10);

        SDL_LabelSetFont(Button->Label,Font);
        SDL_LabelSetFontHeight(Button->Label,Button->height);
        return 1;
    }
    
    return 0;
}


int SDL_ButtonSetFontHeight(SDL_Widget *widget,int height)
{
    SDL_Button *Button=(SDL_Button*)widget;
    SDL_String *caption;
    SDL_Rect FontInfo;

    Button->height = height;
    
    if(Button->Label)
    {
        caption = SDL_LabelGetText(Button->Label);

        SDL_FontSetHeight(Button->Font,Button->height);
        SDL_FontCalcDims(Button->Font,caption,&FontInfo);        

        SDL_WidgetResize(widget,FontInfo.w+25,FontInfo.h+16); 
        SDL_WidgetResize(Button->Label,FontInfo.w+14,FontInfo.h+10);

        SDL_LabelSetFont(Button->Label,Button->Font);
        SDL_LabelSetFontHeight(Button->Label,Button->height);
        return 1;
    }
    return 0;
}


static void SDL_ButtonStyleDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area,SDL_StyleImage *style)
{
    SDL_Button  *Button=(SDL_Button*)widget;
    SDL_ButtonStyle *s = (SDL_ButtonStyle*)style;
    SDL_Image        *ButtonImage;
    SDL_Rect         R;

    ButtonImage = s->Normal;


    switch(Button->State)
    {
    case SDL_BUTTON_STATE_HIGHLIGHTED:
        if(s->Highlighted)
            ButtonImage = s->Highlighted;
        break;
    case SDL_BUTTON_STATE_DOWN:
        if(s->Pressed)
            ButtonImage = s->Pressed;
        break;
    case SDL_BUTTON_STATE_FOCUSED:
        if(s->Focused)
            ButtonImage = s->Focused;
        break;
    default:
        break;
    }
    R.x = 0;
    R.y = 0;
    R.w = widget->Rect.w;
    R.h = widget->Rect.h;
    SDL_ImageBlitToSurface(ButtonImage,surface,&R);
}

static int SDL_ButtonStyleSetImage(SDL_Style *style,int state,SDL_Image *image)
{
    SDL_ButtonStyle *ButtonStyle;
    
    SDL_StyleWidget *widget = SDL_StyleWidgetEntry(style,"SDL_Button");

    if(widget->ImageStyle == NULL)
    {
        widget->ImageStyle = malloc(sizeof(SDL_ButtonStyle));
        memset(widget->ImageStyle,0,sizeof(SDL_ButtonStyle));
    }
    ButtonStyle = widget->ImageStyle;
        
    switch(state)
    {
    case SDL_BUTTON_STATE_UP:
        if(ButtonStyle->Normal==NULL)
            ButtonStyle->Normal = image;
        break;
    case SDL_BUTTON_STATE_DOWN:
        if(ButtonStyle->Pressed == NULL)
            ButtonStyle->Pressed = image;
        break;
    case SDL_BUTTON_STATE_HIGHLIGHTED:
        if(ButtonStyle->Highlighted == NULL)
            ButtonStyle->Highlighted = image;
        break;
    case SDL_BUTTON_STATE_FOCUSED:
        if(ButtonStyle->Pressed == NULL)
            ButtonStyle->Pressed = image;
        break;
    default:
        break;
    }

    return 1;
}
