

#ifndef __SDL_TABLECELL_H__
#define __SDL_TABLECELL_H__
#include "SDL.h"
#include "SDL_String.h"

typedef struct SDL_TableCell
{
  SDL_String String;
  Uint32     fgcolor;
  Uint32     bgcolor;
}SDL_TableCell;

#endif /* __SDL_TABLECELL_H__ */
