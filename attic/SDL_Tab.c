/*
  SDLTk

  one line to give the program's name and an idea of what it does.
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
#include <stdlib.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_Tab.h"

#define TAB_DEFAULTHEIGHT    16
#define TAB_LINE_WIDTH       1
#define TAB_OVERLAY          6



/* Prototypes for local tab functions */

static void Tab_AddArrows(SDL_Tab *tab);


static void Tab_Recalculate(SDL_Tab *tab);
static int  Tab_DrawTabWithCaption(SDL_Surface *dest,SDL_Tab *tab,
                                   SDL_TabList * tl, int high);


/* Prototypes for editwidget callbacks */
void Tab_EditAnyKeyPressed();
void Tab_EditReturnKeyPressed();

/* Prototypes for button callbacks */

static void Tab_ArrowLeftPressed(SDL_Object *object,void *signaldata,void *userdata);
static void Tab_ArrowRightPressed(SDL_Object *object,void *signaldata,void *userdata);

/* signal handler prototype */
static void SDL_NotebookEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata);

extern void DrawPixel(SDL_Surface *screen, int x, int y,unsigned int color2);
static void SDL_NotebookDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);

SDL_WidgetType SDL_NotebookGetType()
{
    SDL_WidgetType notebook;
    
    notebook=SDL_WidgetTypeGet("SDL_Notebook");
    
    if(notebook == 0)
    {
        SDL_WidgetFunctions *func;

        SDL_SignalNew("switch-tab");
		
        notebook=SDL_WidgetTypeRegister("SDL_Notebook",&func);
        func->draw = SDL_NotebookDraw;
    }

    return notebook;
}

SDL_Widget *SDL_NotebookNew()
{
    SDL_Tab     *newtab;
    SDL_Widget  *Widget;

    newtab=(SDL_Tab*)malloc(sizeof(SDL_Tab));
    newtab->NoOfTabs=0;

    Widget=(SDL_Widget*)newtab;
    Widget->Type   = SDL_NotebookGetType();
    Widget->Rect.x = 0;
    Widget->Rect.y = 0;
    Widget->Rect.w = 0;
    Widget->Rect.h = 0;

    newtab->min_width  = 35;
    newtab->min_height = TAB_DEFAULTHEIGHT;
    newtab->tabs  = NULL;
    newtab->hl    = NULL;
    newtab->bgcolor     = 0xdd0000;
    newtab->fgcolor     = WHITE;
    newtab->normal      = NULL;
    newtab->highlighted = NULL;
    newtab->startx      = 0;
    newtab->maxstartx   = 0;
    newtab->doesntfit   = 0;
    newtab->overlay     = TAB_OVERLAY;
    newtab->edit        = NULL;
    newtab->ArrowLeft   = NULL;
    newtab->ArrowRight  = NULL;

    
    SDL_SignalConnect(Widget,"mousebuttondown",SDL_NotebookEventMouseButtonDown,Widget);

    newtab->font        = &DefaultFont;
    return (SDL_Widget*)newtab;

}

void SDL_NotebookDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Tab     *Tab=(SDL_Tab*)widget;
    SDL_TabList *tablist;

    if(Tab->tabs==NULL)
    {
        return;
    }
    else
    {
        tablist=Tab->tabs;
    }

  
    Tab_Recalculate(Tab); /* we can only run this with a valid surface */

    rectangleColor(surface,widget->Rect.x,widget->Rect.y,
                   widget->Rect.x+widget->Rect.w,
                   widget->Rect.y+widget->Rect.h - Tab->tabs->rect->h -1,
                   0x000000ee);

    while(tablist)
    {
        Tab_DrawTabWithCaption(surface,Tab,tablist,0);
        tablist=tablist->next;
    }
    /* Redraw the highlighted tab */
    Tab_DrawTabWithCaption(surface,Tab,Tab->hl,1);
    if(Tab->edit)
    {

    }
}

int SDL_NotebookSetFont(SDL_Widget *widget,SDL_Font *font)
{ 
    SDL_Tab *Tab=(SDL_Tab*)widget;

    if(Tab == NULL)
        return 0;

    Tab->font=font;
    return 1;

}

int SDL_NotebookSetColor(SDL_Widget *widget,int type,Uint32 color)
{
    SDL_Tab *Tab=(SDL_Tab*)widget;

    if(Tab == 0)
        return 0;

    switch(type)
    {
    case BG_COLOR:
        Tab->bgcolor = color;
        break;

    case FG_COLOR:
        Tab->fgcolor = color;
        break;
    }
    
    return 1;

}


static void SDL_NotebookEventMouseButtonDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Tab *Tab=(SDL_Tab*)object;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDL_Event *event = (SDL_Event*)signaldata;
    SDL_TabList *tl = Tab->tabs;

    if(SDL_WidgetIsInside(widget,event->motion.x,event->motion.y))
    {
        if(event->button.button == 1)
        {
            if(tl == NULL)
                return;

            /* Dont process the event when clicked above the tabs */
            if(event->motion.y < widget->Rect.y + widget->Rect.h - tl->rect->h)
                return;

            /* Don't handle events when clicked on one of the arrows */
            if(Tab->ArrowLeft && SDL_WidgetIsInside(Tab->ArrowLeft,event->motion.x,event->motion.y))
                return;
            
            if(Tab->ArrowRight && SDL_WidgetIsInside(Tab->ArrowRight,event->motion.x,event->motion.y))
                return;

              
            while(tl && event->motion.x > (tl->rect->x + widget->Rect.x - Tab->startx) && 
                  event->motion.x > (widget->Rect.x + tl->rect->x + tl->rect->w - Tab->startx))
                tl=tl->next;

            if(tl && tl != Tab->hl)
            {   
                SDL_WidgetHide(Tab->hl->Child);
                SDL_WidgetShow(tl->Child);
                Tab->hl=tl;
                    
                SDL_SignalEmit(widget,"switch-tab",NULL);
                SDL_WidgetRedrawEvent(widget);
            }
        }
        if(event->button.button == 4)
        {
            if(Tab->doesntfit)
                Tab->startx+=5;
        }
        if(event->button.button == 5)
        {
            if(Tab->doesntfit)
                Tab->startx-=5;
        }
            
    }
}



void Tab_AddArrows(SDL_Tab *tab)
{
    SDL_Widget *widget=(SDL_Widget*)tab;
    SDL_Rect LeftArrow;
    SDL_Rect RightArrow;

    LeftArrow.x = widget->Rect.x + widget->Rect.w - (widget->Rect.h *2) - 2 - 1;
    LeftArrow.y = widget->Rect.y + 1;
    LeftArrow.w = widget->Rect.h - 2;
    LeftArrow.h = widget->Rect.h - 2;

    tab->ArrowLeft=SDL_ButtonNew(&LeftArrow);
    SDL_SignalConnect(tab->ArrowLeft,"clicked",Tab_ArrowLeftPressed,tab);
    
    RightArrow.x = widget->Rect.x + widget->Rect.w - widget->Rect.h - 2;
    RightArrow.y = widget->Rect.y + 1;
    RightArrow.w = widget->Rect.h - 2;
    RightArrow.h = widget->Rect.h - 2;

    tab->ArrowLeft=SDL_ButtonNew(&RightArrow);
    SDL_SignalConnect(tab->ArrowRight,"clicked",Tab_ArrowRightPressed,tab);
    
}



static int Tab_DrawSlope(SDL_Surface *dest,SDL_Tab *tab,SDL_TabList * tl, int high,int left)
{
    SDL_Widget *widget=(SDL_Widget*)tab;
    SDL_Rect rect;
    unsigned int color;
    int i=0,j=0;
    int omde;
    int start;
    int end;
    int mw,w;
    if(left)
        rect.x = widget->Rect.x + tl->rect->x - 6;
    else
        rect.x = widget->Rect.x + tl->rect->x + tl->rect->w;

    rect.x -= tab->startx;

    rect.y = widget->Rect.y + tl->rect->y;
    rect.w = 6;
    rect.h = tl->rect->h;

    if(high)
        color=tab->bgcolor;
    else
        color=0x000099;
    
    if(rect.x > widget->Rect.x + widget->Rect.w)
        return 0;

    if((rect.x + rect.w) < widget->Rect.x)
        return 0;

    w  = widget->Rect.x - rect.x;
    mw = widget->Rect.x + widget->Rect.w - rect.x;

    omde=rect.h / (rect.w-1);
    start = 0;
    end   = rect.w; 

    for(i=0;i<rect.h;i++)
    {
        for(j=w;j<mw;j++)
        {
         

            if(j >= start && j < end)
                fastPixelColorNolockNoclip(dest,rect.x + j,rect.y + i,color);
            if(j == start && left)
                fastPixelColorNolockNoclip(dest,rect.x + j,rect.y + i,0xfffefe);

            if(j == end && left == 0)
                fastPixelColorNolockNoclip(dest,rect.x + j,rect.y + i,0x000101);
        }
        if( (i % omde) == 0)
        {
            if(left)
                start++;    
            else
                end --;
        }

    }

    return 1;

    
}

static int Tab_DrawTabWithCaption(SDL_Surface *dest,SDL_Tab *tab,SDL_TabList * tl, int high)
{
    SDL_Widget *widget=(SDL_Widget*)tab;
    SDL_Rect dst,set;
    int xoffset=0;
    int yoffset=0;
    int ml = tab->startx;

    xoffset = widget->Rect.x;
    yoffset = widget->Rect.y;

    dst.x = tl->rect->x + xoffset -ml;
    dst.y = tl->rect->y + yoffset;

    dst.w = tl->rect->w;
    dst.h = tl->rect->h;

    set.x = tl->rect->x + xoffset - ml;
    set.y = dst.y;
    set.w = dst.w;
    set.h = dst.h;

    /* Check if the tab doesn't start on the left side of the tab area */
    if(dst.x < widget->Rect.x)
    {
        /* If the entire tab is on the left side of the area return and don't draw */
        if(((tl->rect->x + tl->rect->w) - ml) < 0)
            return 0; 
        dst.w = dst.w - (widget->Rect.x - dst.x);
        dst.x = widget->Rect.x;
    }

    /* Check if the width doesn't go beyong the tab area */
    if((dst.x + dst.w) > (widget->Rect.x + widget->Rect.w))
        dst.w = (widget->Rect.x + widget->Rect.w) - dst.x;
    
    /* Check if the tab starts outside the area */
    if(dst.x > (widget->Rect.x + widget->Rect.w))
        return 0;

    if(high)
    {
        Tab_DrawSlope(dest,tab,tl,high,1);
        SDL_FillRect(dest,&dst,tab->bgcolor);
        Tab_DrawSlope(dest,tab,tl,high,0);
    }
    else
    {
        Tab_DrawSlope(dest,tab,tl,high,1);
        SDL_FillRect(dest,&dst,0x000099);
        Tab_DrawSlope(dest,tab,tl,high,0);
    }

#if 0
    if(tl->caption)
    {
        SDL_FontDrawStringLimited(NULL,tab->font,tl->caption,tab->fgcolor,&set,&dst);
    }
#endif    
    return 1;
}


static void Tab_Recalculate(SDL_Tab *tab)
{
    SDL_TabList *tablist;
    SDL_Widget  *widget  = (SDL_Widget*)tab;
    int width=0;
    int maxstartx=0;
   
    tablist=tab->tabs;
    
    while(tablist)
    {
        width = 0;
        /*just take a value of 10 for additional space */
        if(tablist->caption && tab->font)
            width = SDL_FontGetStringWidth(tab->font,tablist->caption) + 2;
        if(width < 35)
            width = 35;

        tablist->rect->w = width;
        if(tab->font)
            tablist->rect->h = SDL_FontGetHeight(tab->font);
        else
            tablist->rect->h = 14;
        
        if(tablist->prev == NULL)
        {
            /* this is the first item in the tabtablist */
            tablist->rect->x = 10;
            tablist->rect->y = widget->Rect.h - tablist->rect->h - 1;
        }
        else
        {
            /*just take a value of 10 for additional space */
            tablist->rect->x = tablist->prev->rect->x + tablist->prev->rect->w + 10;
            tablist->rect->y = tablist->prev->rect->y;
            if( (tablist->rect->x + tablist->rect->w) > widget->Rect.w)
            {
                if(tab->ArrowLeft == NULL && tab->ArrowRight == NULL)
                    Tab_AddArrows(tab);
                tab->doesntfit = 1;
            }

        }
        maxstartx = maxstartx + tablist->rect->w + 10;
        tablist=tablist->next;
    }
    maxstartx -= widget->Rect.w;
    if(tab->startx > maxstartx)
        tab->startx = maxstartx;
    if(tab->startx < 0)
        tab->startx = 0;
}


/*
 * Functions used for the edit widget when renaming a tab
 */
void Tab_EditAnyKeyPressed(void *data)
{


}


static void Tab_ArrowLeftPressed(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Tab *Tab = (SDL_Tab*)userdata;
    Tab->startx -=25;
}

static void Tab_ArrowRightPressed(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_Tab *Tab = (SDL_Tab*)userdata;

    Tab->startx +=25;

}

int SDL_NotebookAppendTab(SDL_Widget *parent,SDL_Widget *child,char *caption)
{
    SDL_Tab *tab=(SDL_Tab*)parent;
    SDL_TabList *l;

    if(tab == NULL)
        return 0;

    if(tab->tabs == NULL)
    { 
        tab->tabs   = (SDL_TabList*)malloc(sizeof(SDL_TabList));
        tab->tabs->rect= (SDL_Rect *)malloc(sizeof(SDL_Rect));
        if(caption)
        {
            tab->tabs->caption = SDLTk_strdup(caption);
        }
        else
        {
            tab->tabs->caption = malloc(255 * sizeof(char));
            memset(tab->tabs->caption,0,255);
        }
        tab->NoOfTabs=0;
        tab->hl=tab->tabs;
        tab->tabs->draw    = 1;
        tab->tabs->rect->x = 0;
        tab->tabs->rect->y = 0;
        tab->tabs->rect->h = parent->Rect.h;
        tab->tabs->rect->w = 0;
        tab->tabs->Child = child;
        tab->tabs->next = NULL;
        tab->tabs->prev = NULL;
        tab->tabs->index = tab->NoOfTabs;

    }
    else
    {
          
        l=tab->tabs;

        while(l->next)
        {
            l=l->next;
        }
        l->next= (SDL_TabList*) malloc(sizeof(SDL_TabList));
        if(caption)
            l->next->caption = SDLTk_strdup(caption);
        else
            l->next->caption = NULL;
        l->next->rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
        l->next->rect->y = 0;
        l->next->rect->h = parent->Rect.h;
        l->next->rect->x = 0;
        l->next->rect->w = 0;
        l->next->Child   = child;
        l->next->index = tab->NoOfTabs;
        l->next->next = NULL;
        l->next->prev = l;
    }
    tab->NoOfTabs++;
 
    return 1;
}


int SDL_NotebookRemoveTab(SDL_Widget *widget,int tabnr)
{
    SDL_Tab *tab=(SDL_Tab*)widget;
    SDL_TabList *remove;

    if(tab->NoOfTabs > 0 && tabnr < tab->NoOfTabs)
    {
        remove=tab->tabs;
        if(tabnr == 0)
        {
            tab->tabs       = remove->next;
            tab->tabs->prev = NULL;
        }
        else
        {
            while(tabnr--)
            {
                remove=remove->next;
            }
            remove->prev->next=remove->next;
            remove->next->prev=remove->prev;

        }
        if(remove->caption)
            free(remove->caption);

        free(remove->rect);
        free(remove);
        tab->NoOfTabs--; 
        SDL_WidgetRedrawEvent(widget);
        return 1;
    }
    return 0;
}

int SDL_NotebookSetCurrentTab(SDL_Widget *widget,int tabnr)
{
    SDL_Tab *tab=(SDL_Tab*)widget;
    SDL_TabList *page;

    if(tab->NoOfTabs > 0 && tabnr < tab->NoOfTabs)
    {
        page=tab->tabs;
        while(tabnr--)
            page=page->next;

        SDL_WidgetHide(tab->hl->Child);
        SDL_WidgetShow(page->Child);
        tab->hl=page;
        SDL_WidgetRedrawEvent(widget);

    }
    return 1;

}

int SDL_NotebookGetCurrentPage(SDL_Widget *widget)
{
    SDL_Tab *tab=(SDL_Tab*)widget;
    
    if(tab && tab->hl)
        return tab->hl->index;
    else
        return 0;
}

SDL_Widget *SDL_NotebookGetChildWidget(SDL_Widget *widget)
{
    SDL_Tab *tab=(SDL_Tab*)widget;

    if(tab && tab->hl)
        return tab->hl->Child;
    else
        return NULL;
}

int SDL_NotebookClear(SDL_Widget *widget)
{
    SDL_Tab *Tab=(SDL_Tab*)widget;
    SDL_TabList *page;

    if(Tab == NULL)
        return 0;

    if(Tab->hl)
    {
        SDL_WidgetHide(Tab->hl->Child);
        SDL_WidgetClose(Tab->hl->Child);
    }
    Tab->hl       = NULL;
    
    page=Tab->tabs;
    while(page)
    {
        if(page->Child)
            page->Child->flags = 0;
        page=page->next;
    }

    Tab->tabs     = NULL;
    Tab->NoOfTabs = 0;
    
    return 1;
}
