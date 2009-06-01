/*
  SDLTk

  Basic widget modifiers
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

#ifndef __SDL_WIDGET_H__
#define __SDL_WIDGET_H__

#include <stdarg.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_LList.h"
#include "SDL_Style.h"



struct SDL_ImageTransition;
struct SDL_Window;


#define TRANSPARANT 0xff00ff
#define WHITE       0xffffff
#define BLACK       0x000000

#define FOREGROUND  0x200
#define BACKGROUND  0x201



typedef enum E_Widget_Properties
{
    SET_HIGHLIGHT_IMAGE, 

    SET_DISABLED_IMAGE,  
    SET_FONT,            
    SET_BG_COLOR,        
    SET_FG_COLOR,        

    GET_WIDTH,           
    GET_HEIGHT,          
  
    SET_MAX_VALUE,      
    SET_MIN_VALUE,


    SET_NORMAL_STEP_SIZE,
    


    
}E_Widget_Properties;


#define WIDGET_FOCUSABLE             0x00000001
#define WIDGET_REALIZED              0x00000002
#define WIDGET_VISIBLE               0x00000004
#define WIDGET_NOADD                 0x00000008
#define WIDGET_SURFACE_AVAILABLE     0x00000010
#define WIDGET_FRAME                 0x00000014

typedef void* SDL_Object;



typedef struct SDL_Widget
{
    SDL_Rect      Rect;
    SDL_Rect      Absolute;
    SDL_Rect      Clip;
    int           leftmargin,rightmargin,topmargin,bottommargin;
    int           Type;
    int           Inside;
    Uint32        flags;
    SDL_Surface   *Surface;
    void          *Style;
}SDL_Widget;

/* Widget list for a specific surface */
typedef struct SDL_WidgetList
{
    SDL_LinkedListElem   ll;
    SDL_Widget *Widget;
    struct SDL_WidgetList *Parent;

    struct SDL_WidgetList *Child;
    struct SDL_WidgetList *Next;
}SDL_WidgetList;


/* Window specified by the use */
typedef struct SDL_Window
{
    SDL_Widget    Widget;
    SDL_Widget    *Frame;
    SDL_Surface   *surface; /* Surface to draw to */

    int (*EventHandler)(SDL_Event event);
    int (*NotifyRedraw)();

    void *TransferData;

    Uint8	Type;
    Uint32	bgcolor;
    int Visible;
    SDL_Widget       *FocusWidget;
    SDL_WidgetList   *WidgetList;
}SDL_Window;


/**
 *  Converter function pointer types
 */
typedef SDL_Widget*       (*T_Widget_Create)       (SDL_Rect*);
typedef void              (*T_Widget_Draw)         (SDL_Widget*,SDL_Surface *surface,SDL_Rect*);
typedef int               (*T_Widget_Properties)   (SDL_Widget*,int,va_list ap);
typedef void              (*T_Widget_Close)        (SDL_Widget*);
typedef int               (*T_Widget_SizeRequest)  (SDL_Widget*,SDL_Rect *);

typedef void (*T_Widget_StyleDraw)    (SDL_Widget*,SDL_Surface *surface,SDL_Rect*,SDL_StyleImage *);
typedef void (*T_Widget_StyleSizeInfo)(SDL_Widget*,int control,int state,SDL_Rect  *size,SDL_StyleImage *styleimage);
typedef struct SDL_Image *(*T_Widget_StyleGetImage)(SDL_StyleImage *styleimage,int state);
typedef struct SDL_ImageTransition* (*T_Widget_StyleGetTransition)(SDL_StyleImage *styleimage,int state);
typedef int  (*T_Widget_StyleSetImage)(SDL_Style *style,char *widgetname,int state,struct SDL_Image *image);
typedef void  (*T_Widget_StyleHitTest)(SDL_Widget *widget,SDL_Event *event,SDL_StyleImage *,int *subcontrol);

int SDL_WidgetInit();
void SDL_WidgetMove(SDL_Widget *widget,int x,int y);

SDL_Surface *SDL_WidgetGetActiveSurface();
int SDL_WidgetForceRedraw(SDL_Surface *surface);


int SDL_WidgetMain();


int SDL_WidgetEvent(SDL_Widget *widget,SDL_Event *event);

int   SDL_WidgetHasFocus(SDL_Widget *widget);
int   SDL_WidgetLoseFocus();
int SDL_WidgetSetFocus(SDL_Widget *widget);

int SDL_WidgetNeedsRedraw();
int SDL_WidgetClose(SDL_Widget *widget);


void SDL_WidgetHide(SDL_Widget *widget);
void SDL_WidgetShow(SDL_Widget *widget);
void SDL_WidgetRedrawEvent(SDL_Widget *widget);
void SDL_WidgetResize(SDL_Widget *widget,int w,int h);
void SDL_WidgetDraw(SDL_Widget *Awidget,SDL_Rect *Rect);

int SDL_WidgetGetType(SDL_Widget *widget);

int SDL_WidgetSetDims(SDL_Widget *widget,short x, short y, short w, short h);
int SDL_WidgetSetClipArea(SDL_Widget *widget,Sint16 x,Sint16 y,Uint16 w,Uint16 h);
int SDL_WidgetAddChild(SDL_Widget *parent,SDL_Widget *child);
int SDL_WidgetSizeRequest(SDL_Widget *widget,SDL_Rect *rect);

#endif /* __SDL_WIDGET_H__ */


