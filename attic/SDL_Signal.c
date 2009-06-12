/*
  Signal handling for widgets

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
#include <malloc.h>
#include <string.h>

#include "SDL.h"
#include "SDL_Widget.h"
#include "SDL_Signal.h"
#include "SDLTk_libc.h"

SignalList *SList;

int SDL_SignalInit()
{
    SList = NULL;
    return 1;
}

int SDL_SignalNew(char *signal)
{
    if(SList == NULL)
    {
        SList=(SignalList*)malloc(sizeof(SignalList));
        memset(SList,0,sizeof(SignalList));
        SList->Signal   = SDLTk_strdup(signal);
        SList->Type     = 0;
        SList->CallList = NULL;
        SList->Next     = NULL;
    }
    else
    {
        SignalList *l;
   
        l=SList;
        while(l->Next)
        {
            if(!strcmp(l->Signal,signal))
            {
                printf("%s:%d: Signal (%s) already exists\n",__FILE__,__LINE__,signal);
            }
            l=l->Next;
        }

        l->Next = (SignalList*)malloc(sizeof(SignalList));
        memset(l->Next,0,sizeof(SignalList));
        l->Next->Signal   = SDLTk_strdup(signal);
        l->Next->Type     = 0;
        l->Next->CallList = NULL;
        l->Next->Next     = NULL;
    }
    return 1;
}

int SDL_SignalConnect(SDL_Widget *widget,char *signal,T_Signal_Callback callback,void *userdata)
{
    return SDL_SignalObjectConnect((SDL_Object*)widget,signal,callback,userdata);
}

int SDL_SignalEmit(SDL_Widget *widget,char *signal,void *signaldata)
{
    return SDL_SignalObjectEmit((SDL_Object*)widget,signal,signaldata);
}



int SDL_SignalObjectConnect(SDL_Object *object,char *signal,T_Signal_Callback callback,void *userdata)
{
    if(SList == NULL)
    { 
        printf("%s:%d: Signal doesn't exists \"%s\"\n",__FILE__,__LINE__,signal);
        return 0;
    }
    else
    {
        SignalList *l;
   
        l=SList;
        while(l)
        {
            if(!strcmp(signal,l->Signal))
                break;
            l=l->Next;
        }

        if(l == NULL)
        { 
            printf("%s:%d: Signal doesn't exists \"%s\"\n",__FILE__,__LINE__,signal);
            return 0;
        }

        if(l->CallList == NULL)
        {
            l->CallList = (CallbackList*)malloc(sizeof(CallbackList));
            memset(l->CallList,0,sizeof(CallbackList));
            l->CallList->Object   = object;
            l->CallList->callback = callback;
            l->CallList->userdata = userdata;
            l->CallList->Next     = NULL;
            return 1;
        }
        else
        {
            CallbackList *cbl;

            cbl=l->CallList;
            while(cbl->Next)
                cbl=cbl->Next;

            cbl->Next = (CallbackList*)malloc(sizeof(CallbackList));
            memset(cbl->Next,0,sizeof(CallbackList));
            cbl->Next->Object   = object;
            cbl->Next->callback = callback;
            cbl->Next->userdata = userdata;
            cbl->Next->Next     = NULL;
            return 1;

        }
    }
}


int SDL_SignalObjectEmit(SDL_Object *object,char *signal,void *signaldata)
{
    SignalList *l;
    CallbackList *cbl;
    T_Signal_Callback callback;

    l=SList;
    while(l)
    {
        if(!strcmp(l->Signal,signal))
        {
            if(l->CallList)
            {
                cbl=l->CallList;
                while(cbl)
                {
					if(object == cbl->Object)
                    {
                        callback=cbl->callback;
                        callback(object,signaldata,cbl->userdata);
                    }
                    cbl=cbl->Next;
                }

            }
            else
            {
#if 0
                printf("No handlers for signal %s\n",signal); 
#endif
            }
        }

        l=l->Next;
    }
    return 1;
}


void SDL_SignalQuit()
{
    /* Sorry ?!?!*/
    free(SList);
    SList = NULL;
}
