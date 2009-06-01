/*
  SDLTk

  one line to give the program's name and an idea of what it does.
  Copyright (C) 2004-2006 John Beuving (john.beuving@beatforce.org)

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

#ifndef __SDLTK_H__
#define __SDLTK_H__

#include "SDL_LList.h"
#include "SDL_String.h"
#include "SDL_Widget.h"
#include "SDL_Window.h"
#include "SDL_WidgetType.h"
#include "SDL_ModelType.h"
#include "SDL_Color.h"
#include "SDLTk_libc.h"

#include "SDL_WidTool.h"
#include "SDL_Font.h"

#include "SDL_Button.h"
#include "SDL_Edit.h"

#include "SDL_Frame.h"
#include "SDL_Knob.h"
#include "SDL_Label.h"

#include "SDL_Menu.h"
#include "SDL_Model.h"
#include "SDL_ModelType.h"
#include "SDL_MultiLineEdit.h"
#include "SDL_Panel.h"
#include "SDL_Primitives.h"
#include "SDL_ProgressBar.h"
#include "SDL_RadioButton.h"
#include "SDL_ScrollArea.h"
#include "SDL_Scrollbar.h"
#include "SDL_SelectionModel.h"
#include "SDL_Signal.h"
#include "SDL_Skin.h"
#include "SDL_Slider.h"
#include "SDL_Style.h"
#include "SDL_StyleDefault.h"
#include "SDL_StyleXP.h"
#include "SDL_Tab.h"
#include "SDL_Table.h"
#include "SDL_ToggleButton.h"
#include "SDL_Tooltip.h"
#include "SDL_Tree.h"
#include "SDL_TreeModel.h"
#include "SDL_VolumeBar.h"
#include "SDL_WindowManager.h"
#include "SDL_WindowPrimitives.h"


int SDLTk_Init();
int SDLTk_Main();
int SDLTk_IsRunning();
int SDLTk_PushEvent(SDL_Event *Event);
SDL_Surface *SDLTk_GetVideoSurface();
void SDLTk_UpdateRect(SDL_Surface *surface,short x,short y,short w,short h);

/* OpenGL functions         */
void SDLTK_OpenGLDraw(); 

enum
{
	SDLTK_EVENT			 = SDL_USEREVENT,
	SDL_MOUSEDOUBLECLICK      
};

#define SDLTK_WIDGET_HIDE       1
#define SDLTK_WIDGET_MOVE       2
#define SDLTK_WIDGET_SHOW       3
#define SDLTK_WIDGET_REDRAW     4
#define SDLTK_WIDGET_RESIZE     5
#define SDLTK_WINDOW_REALIZE    6
#define SDLTK_WINDOW_REDRAW     7
#define SDLTK_IMAGE_EVENT       8

#endif /* __SDLTK_H__ */



