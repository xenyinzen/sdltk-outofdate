/*
  Skinning support for SDLTk
  Copyright (C) 2005-2006 John Beuving (john.beuving@beatforce.org)

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
#ifndef __SDL_SKIN_H__
#define __SDL_SKIN_H__

#include "SDLTk.h"
#include "SDL_Table.h"


enum
{
    SKINBUTTON=1,
    SKINCHECKBUTTON,
    SKINFRAME,
    SKINLABEL,
    SKINPANEL,
    SKINRADIOBUTTON,
    SKINSCROLLBAR,
    SKINTABLE,
    SKINTREE
};

typedef struct SDL_SkinButton
{
    short x,y,w,h;
    char *normal;
    char *pressed;
    char *highlight;
    char *label;
    char *disabled;
}SDL_SkinButton;

typedef struct SDL_SkinStyleRadioButton
{
    char *Normal;
    char *Selected;
    char *GreyedNormal;
    char *GreyedSelected;
    char *HighlightedNormal;
    char *HighlightedSelected;
    char *Disabled;
}SDL_SkinStyleRadioButton;

typedef struct SDL_SkinRadioButton
{
    short x,y,w,h;
    SDL_SkinStyleRadioButton Style;
}SDL_SkinRadioButton;


typedef struct SDL_SkinTable
{
    short x,y,w,h;
    char *normal;
    enum E_TableMode mode;
    SDL_Font *font;
}SDL_SkinTable;

typedef struct SDL_SkinTextStyle
{
    Uint32 textcolor;
    char *fontfile;
    int justify;
    int fontsize;
    int height;
}SDL_SkinTextStyle;

typedef struct SDL_SkinLabel
{
    short x,y,w,h;
    char *caption;
    SDL_SkinTextStyle *textstyle;
}SDL_SkinLabel;


typedef struct SDL_SkinPanel
{
    short x,y,w,h;
    char *image;
    char *type;
}SDL_SkinPanel;



typedef struct SDL_SkinFrame
{
    SDL_SkinPanel *titleleft;
    SDL_SkinPanel *titlecenter;
    SDL_SkinPanel *titleright;
    SDL_SkinPanel *left;
    SDL_SkinPanel *right;
    SDL_SkinPanel *bottomleft;
    SDL_SkinPanel *bottomright;
    SDL_SkinPanel *bottom;
}SDL_SkinFrame;

typedef struct SDL_SkinStyleScrollbarState
{
    char *Arrow1;
    char *Arrow2;
    char *Scrollbar;
    char *Thumb;
}SDL_SkinStyleScrollbarState;

typedef struct SDL_SkinStyleScrollbar
{
    SDL_SkinStyleScrollbarState *Normal;
    SDL_SkinStyleScrollbarState *Pressed;
    SDL_SkinStyleScrollbarState *Highlighted;
    SDL_SkinStyleScrollbarState *Disabled;
}SDL_SkinStyleScrollbar;

typedef struct SDL_SkinScrollbar
{
    short x,y,w,h;
    SDL_SkinStyleScrollbar Style;
}SDL_SkinScrollbar;


typedef struct SDL_SkinTree
{
    short x,y,w,h;
}SDL_SkinTree;

typedef struct SDL_SkinWidget
{
    int type;
    char *id;
    union 
    {
        SDL_SkinButton      *button;
        SDL_SkinFrame       *frame;
        SDL_SkinLabel       *label;
        SDL_SkinPanel       *panel;
        SDL_SkinRadioButton *radiobutton;
        SDL_SkinScrollbar   *scrollbar;
        SDL_SkinTable       *table;
        SDL_SkinTree        *tree;
    }w;
    SDL_Widget *widget;
    struct SDL_SkinWidget *next;
}SDL_SkinWidget;


typedef struct SDL_SkinWindow
{
    char *id;
    short x,y,w,h;
    SDL_SkinWidget *widget;
    char *Background;
    SDL_Widget *BackgroundWidget;
    SDL_Window *window;
    struct SDL_SkinWindow *next;
}SDL_SkinWindow;


typedef struct SDL_SkinImage
{
    char *id;
    char *filename;
    short x,y,w,h;
    short left,right,top,bottom;
    SDL_Image *image;
    struct SDL_SkinImage *next;
}SDL_SkinImage;

typedef struct SDL_SkinStyle
{
    char *id;
    SDL_SkinStyleRadioButton *RadioButton;    
    SDL_SkinStyleScrollbar   *HorizontalScrollbar;
    SDL_SkinStyleScrollbar   *VerticalScrollbar;
    struct SDL_SkinStyle *next;
}SDL_SkinStyle;

typedef struct SDL_Skin
{
    char *filename;
    SDL_SkinWindow  *window;
    SDL_SkinImage   *images;
    SDL_SkinStyle   *styles;
    struct SDL_Skin *next; 
}SDL_Skin;

SDL_Skin *SDL_SkinLoad(char *filename);
int SDL_SkinApply(SDL_Window *window,SDL_Skin *skin);
int SDL_SkinApplyByName(SDL_Window *window,SDL_Skin *skin,char *name);
int SDL_SkinApplyWindow(SDL_Window *window,SDL_Skin *skin,SDL_SkinWindow *w);
SDL_Widget *SDL_SkinGetWidget(SDL_Skin *skin,char *id);
SDL_Skin *SDL_SkinGetSkin(SDL_SkinWindow *window);



#endif /* __SDL_SKIN_H__ */
