/*
  SDLTk

  Implementation of window specific behaviour
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


#include <GL/gl.h>

#include "SDLTk.h"


void SDL_GLFillRect(SDL_Surface *surface,const SDL_Rect *r,Uint32 color)
{
    float red   = (float)( color & 0xff0000 >> 16 ) / 255;
    float green = (float)( color & 0x00ff00 >> 8  ) / 255;
    float blue  = (float)( color & 0x0000ff >> 0  ) / 255;
    int y = surface->h + r->y - r->h;

    glBegin(GL_POLYGON);
    glColor4f( red , green, blue, 1.0f );  glVertex2i( r->x        , y        );
    glColor4f( red , green, blue, 1.0f );  glVertex2i( r->x + r->w , y        );
    glColor4f( red , green, blue, 1.0f );  glVertex2i( r->x + r->w , y + r->h );
    glColor4f( red , green, blue, 1.0f );  glVertex2i( r->x        , y + r->h );
    glEnd( );

}

void SDL_GLPoint(SDL_Surface *surface,int x, int y, Uint32 color)
{
    float red   = (float)( color & 0xff0000 >> 16 ) / 255;
    float green = (float)( color & 0x00ff00 >> 8  ) / 255;
    float blue  = (float)( color & 0x0000ff >> 0  ) / 255;

    y = surface->h - y;
    glBegin(GL_POINTS);
    glColor4f( red , green, blue, 1.0f );  glVertex2i( x,y);
    glEnd( );
}


