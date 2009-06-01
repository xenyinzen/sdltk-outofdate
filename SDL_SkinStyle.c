/*
  Skinning support for SDLTk
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
#include <stdio.h>
#include <string.h>

#include "libxml/xmlmemory.h"
#include "libxml/parser.h"

#include "SDL_Skin.h"
#include "SDL_SkinWidget.h"

static SDL_SkinStyleRadioButton *SDL_SkinStyleParseRadioButton(xmlDocPtr doc,xmlNodePtr cur);
static SDL_SkinStyleScrollbar   *SDL_SkinStyleParseScrollbar(xmlDocPtr doc,xmlNodePtr cur);
static SDL_SkinStyleScrollbarState *SDL_SkinStyleParseScrollbarState(xmlDocPtr doc,xmlNodePtr cur);

int SDL_SkinParseStyle(xmlDocPtr doc,xmlNodePtr cur,SDL_SkinStyle **stylelist)
{
    SDL_SkinStyle *style;
    xmlChar *key;

    style=(SDL_SkinStyle*)malloc(sizeof(SDL_SkinStyle));
    memset(style,0,sizeof(SDL_SkinStyle));

    key=xmlGetProp(cur,"id");
    style->id = key;
	
    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        if(!xmlStrcmp(cur->name,(const xmlChar *)"radiobutton"))
        {
            style->RadioButton = SDL_SkinStyleParseRadioButton(doc,cur);
        }
        else if(!xmlStrcmp(cur->name,(const xmlChar *)"scrollbar"))
        {
            key=xmlGetProp(cur,"orientation");

            if(key)
            {
                if(!strcmp(key,"horizontal"))
                {
                    style->HorizontalScrollbar   = SDL_SkinStyleParseScrollbar(doc,cur);
                }
                else if(!strcmp(key,"vertical"))
                {
                    style->VerticalScrollbar     = SDL_SkinStyleParseScrollbar(doc,cur);
                }
            }
            else
            {
                printf("No type given for scrollbar\n");
            }
        }
        cur=cur->next;
    }

    if(*stylelist == NULL)
        *stylelist = style;
    else
    {
        SDL_SkinStyle *s = (SDL_SkinStyle *)(*stylelist);
        while(s->next != NULL)
            s=s->next;

        s->next=style;

    }
    return 1;


}


static SDL_SkinStyleRadioButton *SDL_SkinStyleParseRadioButton(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinStyleRadioButton *radiobutton;
    xmlChar *key;

    radiobutton=(SDL_SkinStyleRadioButton*)malloc(sizeof(SDL_SkinStyleRadioButton));
    memset(radiobutton,0,sizeof(SDL_SkinStyleRadioButton));

    key=xmlGetProp(cur,"normal");
    radiobutton->Normal   = key;
    key=xmlGetProp(cur,"selected");
    radiobutton->Selected = key;

    return radiobutton;
}

static SDL_SkinStyleScrollbar *SDL_SkinStyleParseScrollbar(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinStyleScrollbar *ScrollbarStyle;
    
    ScrollbarStyle=(SDL_SkinStyleScrollbar*)malloc(sizeof(SDL_SkinStyleScrollbar));
    memset(ScrollbarStyle,0,sizeof(SDL_SkinStyleScrollbar));

    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        if(!xmlStrcmp(cur->name,(const xmlChar *)"normal"))
            ScrollbarStyle->Normal       = SDL_SkinStyleParseScrollbarState(doc,cur);
        else if(!xmlStrcmp(cur->name,(const xmlChar *)"pressed"))
            ScrollbarStyle->Pressed      = SDL_SkinStyleParseScrollbarState(doc,cur);
        else if(!xmlStrcmp(cur->name,(const xmlChar *)"highlighted"))
            ScrollbarStyle->Highlighted  = SDL_SkinStyleParseScrollbarState(doc,cur);
        else if(!xmlStrcmp(cur->name,(const xmlChar *)"disabled"))
            ScrollbarStyle->Disabled     = SDL_SkinStyleParseScrollbarState(doc,cur);
        cur=cur->next;
    }

    return ScrollbarStyle;
}


static SDL_SkinStyleScrollbarState *SDL_SkinStyleParseScrollbarState(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinStyleScrollbarState *ScrollbarState;
    xmlChar *key;

    ScrollbarState=(SDL_SkinStyleScrollbarState*)malloc(sizeof(SDL_SkinStyleScrollbarState));
    memset(ScrollbarState,0,sizeof(SDL_SkinStyleScrollbarState));

    key=xmlGetProp(cur,"arrow1");
    ScrollbarState->Arrow1    = key;

    key=xmlGetProp(cur,"arrow2");
    ScrollbarState->Arrow2    = key;

    key=xmlGetProp(cur,"scrollbar");
    ScrollbarState->Scrollbar = key;

    key=xmlGetProp(cur,"thumb");
    ScrollbarState->Thumb     = key;

    return ScrollbarState;
}

