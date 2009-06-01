/*
  SDLTk

  Basic widget interface
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

#ifndef __SDL_WIDGETTYPE_H__
#define __SDL_WIDGETTYPE_H__

#include "SDL_Widget.h"

typedef int SDL_WidgetType;


typedef struct SDL_WidgetFunctions
{
    T_Widget_Draw           draw;            /* Draw function of the widget                */
    T_Widget_SizeRequest    sizerequest;    
    T_Widget_Close          close;

    T_Widget_StyleDraw             styledraw;
    T_Widget_StyleSetImage         stylesetimage;
    T_Widget_StyleGetImage	   stylegetimage;
    T_Widget_StyleGetTransition    stylegettransition;
    T_Widget_StyleSizeInfo	   stylesizeinfo;
    T_Widget_StyleHitTest          stylehittest;
}SDL_WidgetFunctions;

struct SDL_WidgetTypeListItem
{
    char *id;
    SDL_WidgetFunctions func;
};

int SDL_WidgetTypeInit();
SDL_WidgetType SDL_WidgetTypeGet(char *id);

SDL_WidgetType SDL_WidgetTypeRegister(char *id,SDL_WidgetFunctions **func);
T_Widget_Draw SDL_WidgetTypeGetDraw(SDL_WidgetType type);
T_Widget_StyleDraw SDL_WidgetTypeGetStyleDraw(SDL_WidgetType type);
SDL_WidgetFunctions SDL_WidgetTypeGetFunctions(SDL_WidgetType type);
char *SDL_WidgetGetName(SDL_WidgetType type);
T_Widget_StyleSetImage SDL_WidgetTypeGetStyleSetImage(SDL_WidgetType type);
T_Widget_StyleSizeInfo SDL_WidgetTypeGetStyleSizeInfo(SDL_WidgetType type);
T_Widget_StyleGetImage SDL_WidgetTypeGetStyleGetImage(SDL_WidgetType type);
T_Widget_StyleHitTest SDL_WidgetTypeGetStyleHitTest(SDL_WidgetType type);
T_Widget_StyleGetTransition SDL_WidgetTypeGetStyleGetTransition(SDL_WidgetType type);

#endif

