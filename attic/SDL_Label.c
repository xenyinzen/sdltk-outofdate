/*
  SDLTk

  Label widget
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
#include "SDL_Label.h"


static void Label_CalculatePattern(SDL_Label *Label,SDL_Rect *Rect);
static void SDL_LabelDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_LabelGetType()
{
    SDL_WidgetType label;
    label = SDL_WidgetTypeGet("SDL_Label");
    
    if(label == 0)
    {
        SDL_WidgetFunctions *func;

        label=SDL_WidgetTypeRegister("SDL_Label",&func);
        func->draw        = SDL_LabelDraw;
    }
    return label;
        
}

SDL_Widget* SDL_LabelNew()
{
    SDL_Widget *Widget;
    SDL_Label  *Label;

    Label=(SDL_Label*)malloc(sizeof(SDL_Label));
    memset(Label,0,sizeof(SDL_Label));

    Widget=(SDL_Widget*)Label;
    Widget->Type      = SDL_LabelGetType();
    Widget->Rect.x    = 0;
    Widget->Rect.y    = 0;
    Widget->Rect.w    = 0;
    Widget->Rect.h    = 0;
    Widget->flags     = 0;

    Label->Caption = NULL;
    Label->Font    = &DefaultFont;

    Label->height  = 10;
    Label->fgcolor = BLACK;
    Label->bgcolor = TRANSPARANT;
    Label->offset   = 0;
    Label->increase = 1;
    Label->Alignment = TOPLEFT;

    Label->Pattern    = LABEL_NORMAL;

    return (SDL_Widget*)Label;
}

void SDL_LabelDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Label *Label=(SDL_Label*)widget;
    SDL_Rect DrawPosition;
    SDL_Rect FontInfo;

    if(Label->bgcolor != TRANSPARANT)
    {
		SDL_Rect r;
		r.x = r.y = 0;
		r.w = widget->Rect.w;
		r.h = widget->Rect.h;
        SDL_FillRect(surface,&r,Label->bgcolor);
    }

    if(Label->Caption)
    {
        SDL_LabelSetFontHeight(widget,Label->height);

        Label_CalculatePattern(Label,&DrawPosition);
#if 0
        SDL_FillRectWindow(window,&DrawPosition,0xfff000);
#endif
        SDL_FontCalcDims(Label->Font,Label->Caption,&FontInfo);

        SDL_FontDrawString(surface,Label->Font,Label->Caption,Label->fgcolor,&DrawPosition);
    }
}

int SDL_LabelSetColor(SDL_Widget *widget,int which,Uint32 color)
{
    SDL_Label *Label=(SDL_Label*)widget;

    if(Label == NULL)
        return 0;

    switch(which)
    {
    case FOREGROUND:
        break;
    case BACKGROUND:
        break;
    default:
        break;
    }
    return 1;

}

static void Label_CalculatePattern(SDL_Label *Label,SDL_Rect *Rect)
{
    SDL_Widget *widget=(SDL_Widget*)Label;
    SDL_Rect dims;
  
    /* Calculate the total size of the string in pixels */

    SDL_FontCalcDims(Label->Font,Label->Caption,&dims);

    if(dims.w > widget->Rect.w)
    {
        switch(Label->Pattern)
        {
        case LABEL_NORMAL:
            Rect->x = 0;
            Rect->y = 0;
            Rect->w = widget->Rect.w;
            Rect->h = widget->Rect.h;
            break;
        case LABEL_BOUNCE:
            Rect->x =  - Label->offset;
            Rect->y = 0;
            Rect->w = widget->Rect.w + Label->offset;
            Rect->h = widget->Rect.h;
            if(Label->increase == 1)
                Label->offset++;
            else 
                Label->offset--;
            if(dims.w < Rect->w)
                Label->increase = 0;
            
            if(Label->offset == -1)
                Label->increase=1;
            break;
        case LABEL_SCROLL_LEFT:
            Rect->x =  - Label->offset;
            Rect->y = 0;
            Rect->w = widget->Rect.w + Label->offset;
            Rect->h = widget->Rect.h;
            Label->offset++;
            if(dims.w + widget->Rect.w < widget->Rect.w + Label->offset)
                Label->offset = -widget->Rect.w;
            break;
        case LABEL_SCROLL_RIGHT:
            Rect->x =  - Label->offset;
            Rect->y = 0;
            Rect->w = widget->Rect.w + Label->offset;
            Rect->h = widget->Rect.h;
            Label->offset--;
            if(Label->offset + widget->Rect.w < 0)
                Label->offset = dims.w;
            break;
        }
    }
    else
    {
        int height = Label->Font->Height;

        switch(Label->Alignment)
        {
        case TOPLEFT:
            /* no alignment string is in upper left corner */
            Rect->x = 0;
            Rect->y = 0 - dims.y + 1;
            Rect->w = dims.w;
            Rect->h = dims.h;
            break;
        case TOPCENTER:
            Rect->x = 0 + (widget->Rect.w - dims.w)/2 + 1;
            Rect->y = 0 - dims.y + 1;
            Rect->w = dims.w;
            Rect->h = dims.h;
            break;
        case CENTER:
            Rect->x = 0 + (widget->Rect.w - dims.w)/2;
            Rect->y = 0 + (widget->Rect.h - height)/2 + 1;
            Rect->w = widget->Rect.w;
            Rect->h = widget->Rect.h;
            break;
        }

    }

}

int SDL_LabelSetFont(SDL_Widget *widget,SDL_Font *Font)
{
    SDL_Label *Label=(SDL_Label*)widget;

    if(Font == NULL)
        return 0;

    Label->Font = Font;
    return 1;
}


int SDL_LabelSetText(SDL_Widget *widget,char *text)
{
    SDL_Label *Label=(SDL_Label*)widget;


    if(Label == NULL || text == NULL)
        return 0;


    if(Label->Caption)
        SDL_StringFree(Label->Caption);
    Label->Caption = SDL_StringCreate(text);
    
    SDL_WidgetRedrawEvent(widget);

    return 1;
}

int SDL_LabelSetAlignment(SDL_Widget *widget,int Alignment)
{
    SDL_Label *Label=(SDL_Label*)widget;

    Label->Alignment = Alignment;
    
    return 1;
}

int SDL_LabelSetFontHeight(SDL_Widget *widget,int height)
{
    SDL_Label *Label=(SDL_Label*)widget;
    
    Label->height = height;
    
    if(Label->Font != &DefaultFont)
        SDL_FontSetHeight(Label->Font,Label->height);
    return 0;
}

SDL_String *SDL_LabelGetText(SDL_Widget *widget)
{
    SDL_Label *Label=(SDL_Label*)widget;
    
    return Label->Caption;
}
