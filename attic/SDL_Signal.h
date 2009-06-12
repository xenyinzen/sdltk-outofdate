/*
  SDLTk

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

#ifndef __SDL_SIGNAL_H__
#define __SDL_SIGNAL_H__

#include "SDL_Widget.h"

typedef void (*T_Signal_Callback)(SDL_Object *object,void *signaldata,void *userdata);

typedef struct CallbackList
{
    SDL_Object *Object;
    T_Signal_Callback callback;
    void *userdata;
    struct CallbackList *Next;
}CallbackList;


typedef struct SignalList
{
    char *Signal;
    int Type;
    struct CallbackList *CallList;
    struct SignalList   *Next;
}SignalList;

int SDL_SignalNew(char *signal);
int SDL_SignalInit();
void SDL_SignalQuit();

int SDL_SignalObjectConnect(SDL_Object *object,char *signal,T_Signal_Callback callback,void *userdata);
int SDL_SignalObjectEmit(SDL_Object *object,char *signal,void *signaldata);

int SDL_SignalConnect(SDL_Widget *widget,char *signal,T_Signal_Callback callback,void *userdata);
int SDL_SignalEmit(SDL_Widget *widget,char *signal,void *signaldata);

#endif /* __SDL_SIGNAL_H__ */
