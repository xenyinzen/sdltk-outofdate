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



#include <stdio.h>
#include <string.h>

#include "SDL_WidgetType.h"


#define MAX_TYPE 100

struct SDL_WidgetTypeListItem WidgetTypeList[MAX_TYPE];

int SDL_WidgetTypeInit()
{
    int i;
    
    for(i=0;i<MAX_TYPE;i++)
    {
        memset(&WidgetTypeList[i],0,sizeof(struct SDL_WidgetTypeListItem));
    }
    return 1;
}

SDL_WidgetType SDL_WidgetTypeGet(char *id)
{
    int i;

    for(i=1;i<MAX_TYPE;i++)
    {
        if(WidgetTypeList[i].id != NULL)
        {
            if(!strcmp(WidgetTypeList[i].id,id))
                return i;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

SDL_WidgetType SDL_WidgetTypeRegister(char *id,SDL_WidgetFunctions **func)
{
    int i;

    for(i=1;i<MAX_TYPE;i++)
    {
        if(WidgetTypeList[i].id != NULL)
        {
            if(!strcmp(WidgetTypeList[i].id,id))
                return i;
        }
        else
        {
            WidgetTypeList[i].id=id;
            *func=&WidgetTypeList[i].func;
#if 0
            printf("%d %s\n",i,id);
#endif
            return i;
        }
    }
    return 0;
}


T_Widget_Draw SDL_WidgetTypeGetDraw(SDL_WidgetType type)
{
    return WidgetTypeList[type].func.draw;
}

SDL_WidgetFunctions SDL_WidgetTypeGetFunctions(SDL_WidgetType type)
{
    return WidgetTypeList[type].func;
}

T_Widget_StyleDraw SDL_WidgetTypeGetStyleDraw(SDL_WidgetType type)
{
    return WidgetTypeList[type].func.styledraw;
}

T_Widget_StyleSetImage SDL_WidgetTypeGetStyleSetImage(SDL_WidgetType type)
{
    return WidgetTypeList[type].func.stylesetimage;
}

T_Widget_StyleSizeInfo SDL_WidgetTypeGetStyleSizeInfo(SDL_WidgetType type)
{
    return WidgetTypeList[type].func.stylesizeinfo;
}

char *SDL_WidgetGetName(SDL_WidgetType type)
{
    return WidgetTypeList[type].id;
}
