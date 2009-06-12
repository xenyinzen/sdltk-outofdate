

#ifndef __SDL_TABLEROW_H__
#define __SDL_TABLEROW_H__

#include "SDL.h"
#include "SDL_Table.h"
#include "SDL_TableCell.h"

struct SDL_TableRow *SDL_TableRowNew();

typedef struct SDL_TableRow
{
    SDL_TableCell *Cell;
    Uint32   fgcolor;
    Uint32   bgcolor;
    void *userdata;
    struct SDL_TableRow *Next;
}SDL_TableRow;

int SDL_TableRowSetColor(struct SDL_TableRow *row,Uint32 color);


#endif /* __SDL_TABLEROW_H__ */
