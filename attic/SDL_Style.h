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



#ifndef __SDL_STYLE_H__
#define __SDL_STYLE_H__


typedef void SDL_StyleImage;

typedef void (*T_Style_Paint)(void *widget,SDL_Surface *surface,SDL_Rect *Area);
typedef void (*T_Style_SizeInfo)(int control,int state,SDL_Rect  *size);


typedef struct SDL_StyleWidget
{
    SDL_LinkedListElem ll;
    char *WidgetName;
    
    T_Style_Paint            PaintFunction;
    T_Style_SizeInfo         SizeInfo;
    /* the image style has the highest priority. The functions should be implemented by the widget itself    */
    /* This is important because of mouse/keyboard events. If there are problems the own drawing functions   */
    /* can be provided. To react correclty on mouse events the sizeinfo is introduced                        */
    SDL_StyleImage          *ImageStyle;
}SDL_StyleWidget;

typedef struct SDL_Style
{
    /* contains a list of widget */
    SDL_LinkedList      WidgetStyles;
}SDL_Style;

typedef struct SDL_StyleStackElem 
{
    SDL_LinkedListElem ll;
    SDL_Style          *style;
}SDL_StyleStackElem;

SDL_Style *SDL_StyleNew();

typedef enum
{
    STYLE_STATE_NORMAL,
    STYLE_STATE_PRESSED,
    STYLE_STATE_HIGHLIGHTED,
    STYLE_STATE_FOCUS,
    STYLE_STATE_SELECTED,
    STYLE_STATE_DISABLED,
}eStyle;

typedef enum
{
    SCROLLBAR_ARROW1,
    SCROLLBAR_ARROW2,
    SCROLLBAR_SCROLLBAR,
    SCROLLBAR_THUMB
}eControl;



void SDL_StyleInit();
void SDL_StylePaint(void *widget,SDL_Surface *surface,SDL_Rect *Area);
int  SDL_StylePush(SDL_Style *style);
int  SDL_StyleUnpush(SDL_Style *style);

void SDL_StyleSetDrawFunction(SDL_Style *style,char *widgetname,T_Style_Paint paint);
SDL_StyleWidget *SDL_StyleGetWidgetEntry(SDL_Style *style,char *widget);
SDL_StyleWidget *SDL_StyleWidgetEntry(SDL_Style *style,char *widget);
void SDL_StyleGetSizeInfo(void *w,int control,int state,SDL_Rect *dims);
#endif /* __SDL_STYLE_H__ */
