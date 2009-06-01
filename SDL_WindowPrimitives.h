#ifndef __SDL_WINDOWPRIMITIVES_H__
#define __SDL_WINDOWPRIMITIVES_H__


int SDL_DrawLineWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);

void SDL_FillRectWindow(SDL_Window *window,SDL_Rect *r,Uint32 color);

void SDL_BlitToWindow(SDL_Surface *src,SDL_Rect *srcrect, SDL_Window *window,SDL_Rect *destrect);
int SDL_DrawBoxWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);

int SDL_DrawRectangleWindow(SDL_Window *window, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color);
int SDL_DrawCircleWindow(SDL_Window *window, Sint16 x, Sint16 y, Sint16 r, Uint32 color);

#endif  /* __SDL_WINDOWPRIMITIVES_H__ */
