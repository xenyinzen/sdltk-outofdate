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


SDL_SkinWidget *SDL_SkinParseButton(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinButton *button;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    button=(SDL_SkinButton*)malloc(sizeof(SDL_SkinButton));
    memset(button,0,sizeof(SDL_SkinButton));

    key=xmlGetProp(cur,"id");
    widget->id = key;
	
    key=xmlGetProp(cur,"x");
    button->x = atoi(key);
    key=xmlGetProp(cur,"y");
    button->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        button->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        button->h = atoi(key);
	
    key=xmlGetProp(cur,"normal");
    button->normal=key;
    key=xmlGetProp(cur,"pressed");
    button->pressed=key;
    key=xmlGetProp(cur,"label");
    button->label=key;


    widget->type   = SKINBUTTON;
    widget->w.button = button;
    return widget;
}


SDL_SkinWidget *SDL_SkinParseFrame(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinWidget *panel;
    SDL_SkinFrame *frame;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    frame=(SDL_SkinFrame*)malloc(sizeof(SDL_SkinFrame));
    memset(frame,0,sizeof(SDL_SkinFrame));

	
    cur=cur->xmlChildrenNode;

    while(cur != NULL)
    {
        panel=NULL;
	        
        if(!xmlStrcmp(cur->name,(const xmlChar *)"panel"))
        {
            panel=SDL_SkinParsePanel(doc,cur);
            if(panel->w.panel->type)
            {
                if(!strcmp(panel->w.panel->type,"TL"))
                    frame->titleleft = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"TB"))
                    frame->titlecenter = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"TR"))
                    frame->titleright = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"L"))
                    frame->left = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"R"))
                    frame->right = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"BL"))
                    frame->bottomleft = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"BR"))
                    frame->bottomright = panel->w.panel;
                if(!strcmp(panel->w.panel->type,"B"))
                    frame->bottom  = panel->w.panel;

            }
        }
        cur=cur->next;
    }
    widget->type   = SKINFRAME;
    widget->w.frame = frame;
    return widget;
}


SDL_SkinWidget *SDL_SkinParsePanel(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinPanel  *panel;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    panel=(SDL_SkinPanel*)malloc(sizeof(SDL_SkinPanel));
    memset(panel,0,sizeof(SDL_SkinPanel));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    if(key)
        panel->x = atoi(key);
    key=xmlGetProp(cur,"y");
    if(key)
        panel->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        panel->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        panel->h = atoi(key);

    key=xmlGetProp(cur,"image");
    if(key)
        panel->image = key;
	
    key=xmlGetProp(cur,"type");
    if(key)
        panel->type = key;
	
    widget->type   = SKINPANEL;
    widget->w.panel = panel;
    return widget;
}

SDL_SkinWidget *SDL_SkinParseRadioButton(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinRadioButton  *radiobutton;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    radiobutton=(SDL_SkinRadioButton*)malloc(sizeof(SDL_SkinRadioButton));
    memset(radiobutton,0,sizeof(SDL_SkinRadioButton));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    if(key)
        radiobutton->x = atoi(key);
    key=xmlGetProp(cur,"y");
    if(key)
        radiobutton->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        radiobutton->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        radiobutton->h = atoi(key);

    key=xmlGetProp(cur,"normal");
    if(key)
        radiobutton->Style.Normal = key;
	
    key=xmlGetProp(cur,"selected");
    if(key)
        radiobutton->Style.Selected = key;
	
    widget->type   = SKINRADIOBUTTON;
    widget->w.radiobutton = radiobutton;
    return widget;
}


SDL_SkinWidget *SDL_SkinParseScrollbar(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinScrollbar *scrollbar;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    scrollbar=(SDL_SkinScrollbar*)malloc(sizeof(SDL_SkinScrollbar));
    memset(scrollbar,0,sizeof(SDL_SkinScrollbar));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    scrollbar->x = atoi(key);
    key=xmlGetProp(cur,"y");
    scrollbar->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        scrollbar->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        scrollbar->h = atoi(key);

    cur=cur->children;
    while(cur != NULL)
    {
        if(!xmlStrcmp(cur->name,(const xmlChar *)"normal"))
        {
            if(scrollbar->Style.Normal == NULL)
            {
                scrollbar->Style.Normal = malloc(sizeof(SDL_SkinStyleScrollbarState));
                memset(scrollbar->Style.Normal,0,sizeof(SDL_SkinStyleScrollbarState));
            }
            key=xmlGetProp(cur,"scrollbar");
            if(key != NULL)
            {
                scrollbar->Style.Normal->Scrollbar  = key;
            }
            key=xmlGetProp(cur,"thumb");
            if(key != NULL)
                scrollbar->Style.Normal->Thumb      = key;

            key=xmlGetProp(cur,"arrow1");
            if(key != NULL)
                scrollbar->Style.Normal->Arrow1     = key;

            key=xmlGetProp(cur,"arrow2");
            if(key != NULL)
                scrollbar->Style.Normal->Arrow2     = key;

        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"pressed"))
        {
            if(scrollbar->Style.Pressed == NULL)
            {
                scrollbar->Style.Pressed = malloc(sizeof(SDL_SkinStyleScrollbarState));
                memset(scrollbar->Style.Pressed,0,sizeof(SDL_SkinStyleScrollbarState));
            }
            key=xmlGetProp(cur,"scrollbar");
            if(key != NULL)
            {
                scrollbar->Style.Pressed->Scrollbar  = key;
            }
            key=xmlGetProp(cur,"thumb");
            if(key != NULL)
                scrollbar->Style.Pressed->Thumb      = key;

            key=xmlGetProp(cur,"arrow1");
            if(key != NULL)
                scrollbar->Style.Pressed->Arrow1     = key;

            key=xmlGetProp(cur,"arrow2");
            if(key != NULL)
                scrollbar->Style.Pressed->Arrow2     = key;
        }
        cur=cur->next;
    }

	
    widget->type   = SKINSCROLLBAR;
    widget->w.scrollbar = scrollbar;
    return widget;
}
