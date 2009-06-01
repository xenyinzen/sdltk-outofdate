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

#ifndef __SDL_SKINWIDGET_H__
#define __SDL_SKINWIDGET_H__

SDL_SkinWidget *SDL_SkinParseButton(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseFrame(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseLabel(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParsePanel(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseRadioButton(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseScrollbar(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseTable(xmlDocPtr doc,xmlNodePtr cur);
SDL_SkinWidget *SDL_SkinParseTree(xmlDocPtr doc,xmlNodePtr cur);

#endif /* __SDL_SKINWIDGET_H__ */
