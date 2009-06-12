/*
  SDLTk

  String handling
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
#include "malloc.h"
#include <string.h>
#include <stdio.h>

#include "SDLTk_libc.h"
#include "SDL_String.h"

SDL_String *SDL_StringCreate(char *text)
{
    SDL_String *String;

    String=(SDL_String*)malloc(sizeof(SDL_String));
    memset(String,0,sizeof(SDL_String));

    String->Length = (int)strlen(text);
    String->Data   = SDLTk_strdup(text);
    String->MaxLen = String->Length;
    
    return String;
}

SDL_String *SDL_StringPrealloc(unsigned int size)
{
    SDL_String *String;

    String=(SDL_String*)malloc(sizeof(SDL_String));
    memset(String,0,sizeof(SDL_String));

    String->Data   = malloc(size);
    String->MaxLen = size - 1;
    return String;
}

int SDL_StringSetText(SDL_String *String,char *text)
{
    if(String == NULL || text == NULL)
        return 0;

    String->Length = (int)strlen(text);
    String->Data   = SDLTk_strdup(text);
    String->MaxLen = String->Length;
    return 1;
}


int SDL_StringFree(SDL_String *String)
{
  free(String->Data);
  String->Data   = NULL;
  String->Length = 0;
  return 1;
}

unsigned int SDL_StringLength(SDL_String *String)
{
    if(String == NULL)
        return 0;
    return String->Length;
}


SDL_String *SDL_StringSubString(SDL_String *String,int start,int count)
{
    SDL_String *tmp=NULL;
    char *dat;

    if(String == NULL)
        return NULL;

    tmp=(SDL_String*)malloc(sizeof(SDL_String));
    memset(tmp,0,sizeof(SDL_String));

    tmp->Data =(char*)malloc(count+1);
    dat = String->Data + start;
    strncpy(tmp->Data,dat,count);
    tmp->Data[count]=0;

    return tmp;
}


SDL_String *SDL_StringInsert(SDL_String *String,int pos,SDL_Char *c)
{
    char tmp[255];
    memset(tmp,0,255);

    sprintf(tmp,"%c%s",c,String->Data + pos);
    
    String->Data[pos+1]=0;
    sprintf(String->Data,"%s%s",String->Data,tmp);
    String->Length =(int)strlen(String->Data);
    
    return String;
}

void SDL_StringRemove(SDL_String *String,int pos,int size)
{
    strcpy(String->Data + pos,String->Data + pos + size);
}
