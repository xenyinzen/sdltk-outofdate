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

int SDL_SkinParseWindow(xmlDocPtr doc,xmlNodePtr cur,SDL_SkinWindow **windowlist)
{
    SDL_SkinWindow *window;
    SDL_SkinWidget *widget;
    xmlChar *key;

    window=(SDL_SkinWindow*)malloc(sizeof(SDL_SkinWindow));
    memset(window,0,sizeof(SDL_SkinWindow));

    key=xmlGetProp(cur,"id");
    window->id = key;
	
    key=xmlGetProp(cur,"background");
    window->Background = key;
		
    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        widget=NULL;
		
        if(!xmlStrcmp(cur->name,(const xmlChar *)"button"))
        {
            widget=SDL_SkinParseButton(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"frame"))
        {
            widget=SDL_SkinParseFrame(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"label"))
        {
            widget=SDL_SkinParseLabel(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"panel"))
        {
            widget=SDL_SkinParsePanel(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"radiobutton"))
        {
            widget=SDL_SkinParseRadioButton(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"scrollbar"))
        {
            widget=SDL_SkinParseScrollbar(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"table"))
        {
            widget=SDL_SkinParseTable(doc,cur);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"tree"))
        {
            widget=SDL_SkinParseTree(doc,cur);
        }

	
        if(widget != NULL)
        {
            SDL_SkinWidget *w;

            w=window->widget;
            if(w == NULL)
                window->widget = widget;
            else
            {
                while(w->next)
                    w=w->next;
                w->next = widget;

            }

        }
        cur=cur->next;
    }

    if(*windowlist == NULL)
        *windowlist = window;
    else
    {
        SDL_SkinWindow *w = (SDL_SkinWindow *)(*windowlist);
        while(w->next != NULL)
            w=w->next;

        w->next=window;

    }
    return 1;
			
}


