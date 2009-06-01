/*
  SDLTk

  Multi line edit widget
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
#include <stdarg.h>
#include <malloc.h>
#include <string.h>

#include "SDLTk.h"
#include "SDL_MultiLineEdit.h"
#include "SDL_Paragraph.h"

static void SDL_MultiLineEditDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area);
static void SDL_MultiLineEditSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata);
void calcbreak(int width,SDL_Font *font,SDL_EditParagraph *p);
static void SDL_MultiLineEditInsert(SDL_MultiLineEdit *edit,unsigned int c);
void drawcursor(int x,int y,SDL_Surface *surface);
SDL_EditParagraph *NewParagraph();
void drawparagraph(SDL_Widget *widget,SDL_EditParagraph *p,SDL_Surface *surface,int *y);

SDL_WidgetType SDL_MultiLineEditGetType()
{
    SDL_WidgetType mle;
    mle = SDL_WidgetTypeGet("SDL_MultiLineEdit");
    
    if(mle == 0)
    {
        SDL_WidgetFunctions *func;

        mle = SDL_WidgetTypeRegister("SDL_MultiLineEdit",&func);
        func->draw        = SDL_MultiLineEditDraw;
    }
    return mle;
}

SDL_Widget* SDL_MultiLineEditNew()
{
    SDL_Widget *Widget;
    SDL_MultiLineEdit  *MultiLineEdit;

    MultiLineEdit=(SDL_MultiLineEdit*)malloc(sizeof(SDL_MultiLineEdit));
    memset(MultiLineEdit,0,sizeof(SDL_MultiLineEdit));

    Widget=(SDL_Widget*)MultiLineEdit;
    Widget->Type      = SDL_MultiLineEditGetType();
    Widget->Rect.x    = 0;
    Widget->Rect.y    = 0;
    Widget->Rect.w    = 0;
    Widget->Rect.h    = 0;
    Widget->flags     = WIDGET_FOCUSABLE;

    SDL_SignalConnect(Widget,"keydown",SDL_MultiLineEditSignalKeyDown,Widget);

    MultiLineEdit->Font    = &DefaultFont;

    MultiLineEdit->bgcolor = 0xfffff7;
    MultiLineEdit->fgcolor = 0x000000;

    return Widget;
}



void SDL_MultiLineEditDraw(SDL_Widget *widget,SDL_Surface *surface,SDL_Rect *Area)
{
    SDL_Rect StringPos;
    SDL_MultiLineEdit *Edit=(SDL_MultiLineEdit*)widget;
    
    SDL_FillRect(surface,NULL,0xffffff);
    SDL_FillRect(surface,NULL,BLACK);
    {
        SDL_Rect r;
        r.x = 1;
        r.y = 1;
        r.w = widget->Rect.w-2;
        r.h = widget->Rect.h-2;
        SDL_FillRect(surface,&r,Edit->bgcolor);
    }
    StringPos.y = 5;
    StringPos.x = 2;
    StringPos.w = widget->Rect.w - 2;
    StringPos.h = widget->Rect.h;


    if(Edit->Paragraph)
    {
        SDL_EditParagraph *p = Edit->Paragraph;
        
        while(p)
        {
            calcbreak(widget->Rect.w,Edit->Font,p);
            drawparagraph(widget,p,surface,(int*)&StringPos.y);
            /* tangg added */
            if (StringPos.y >= StringPos.h) {
		SDL_Rect r;
		r.x = 1;
		r.y = 1;
		r.w = widget->Rect.w-2;
		r.h = widget->Rect.h-2;
		SDL_FillRect(surface,&r,Edit->bgcolor);
            	
            	StringPos.y = 5;
            	StringPos.x = 2;
            	
            	Edit->Paragraph = p;
            	p = p->next;
            }
            else 
            	p=p->next;
        }
    }
    else
    {
        if(!SDL_WidgetHasFocus(widget))
            return;

        drawcursor(StringPos.x,StringPos.y,surface);
    }
}

void AddParagraph(SDL_MultiLineEdit *Edit)
{
    SDL_EditParagraph *p = Edit->Paragraph;
    
    if(p)
    {
        while(p->next)
            p=p->next;
        
        p->next         = NewParagraph();
        p->next->prev   = p;
        Edit->Cursor.p  = p->next;
        Edit->Cursor.CursorPosition  = 1;
    }
    else
    {
        Edit->Paragraph = NewParagraph();
        Edit->Cursor.p  = Edit->Paragraph;
        Edit->Cursor.CursorPosition  = 1;
    }
}

void merge(SDL_EditParagraph *first,SDL_EditParagraph *second)
{
    first->next = second->next;
    if(second->next)
        second->next->prev = first;
}

static void SDL_MultiLineEditSignalKeyDown(SDL_Object *object,void *signaldata,void *userdata)
{
    SDL_MultiLineEdit *Edit=(SDL_MultiLineEdit*)object;
    SDL_Event *event = (SDL_Event *)signaldata;
    SDL_Widget *widget=(SDL_Widget*)object;
    SDLMod mod;
    int key;


    if(!SDL_WidgetHasFocus(widget))
        return;

    mod = event->key.keysym.mod;

    switch(event->key.keysym.sym)
    {
    case SDLK_DELETE:
    {
        
#if 0
        if(Edit->CursorPosition >= 0)
        {
            strcpy(Edit->Caption.Data + Edit->CursorPosition,Edit->Caption.Data + Edit->CursorPosition+1);
            SDL_WidgetRedrawEvent(widget);
            SDL_SignalEmit(widget,"changed",NULL);
        }
#endif
    }
    break;
    case SDLK_BACKSPACE:
        if(Edit->Cursor.p)
        {
            if(Edit->Cursor.CursorPosition > 0)
            {
                SDL_ParagraphRemove(Edit->Cursor.p,--Edit->Cursor.CursorPosition,1);
            }
            else if(Edit->Cursor.p->prev != NULL)
            {
                Edit->Cursor.p = Edit->Cursor.p->prev;
                merge(Edit->Cursor.p,Edit->Cursor.p->next);/* should merge both paragraphs*/;
                Edit->Cursor.CursorPosition = strlen(Edit->Cursor.p->Caption->Data);
            }
            SDL_SignalEmit(widget,"changed",NULL);
            SDL_WidgetRedrawEvent(widget);
        }
        break;
    case SDLK_HOME:
#if 0
        Edit->CursorPosition = 0; 
        SDL_WidgetRedrawEvent(widget);
#endif
        break;
    case SDLK_END:
#if 0
        Edit->CursorPosition = SDL_StringLength(&Edit->Caption)+1; 
        SDL_WidgetRedrawEvent(widget);
#endif
        break;
    case SDLK_LEFT:
        if(Edit->Cursor.CursorPosition > 0)
            Edit->Cursor.CursorPosition--;
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_RIGHT:
        Edit->Cursor.CursorPosition++;
        SDL_WidgetRedrawEvent(widget);
        break;
    case SDLK_RETURN:
        if(SDL_WidgetHasFocus(widget))
            SDL_SignalEmit(widget,"activate",NULL);
        AddParagraph(Edit);
        SDL_WidgetRedrawEvent(widget);
        break;
    default:
        key=event->key.keysym.unicode;
        SDL_MultiLineEditInsert(Edit,key);
        SDL_WidgetRedrawEvent(widget);
        SDL_SignalEmit(widget,"changed",NULL);
        break;
    }
}

int removelastword(SDL_Font *font,char *string,int end)
{
    char *retval=&string[end];
    
    while(*retval != 0x20)
    {
        retval--;
        end--;
    }
    return end;
}

int *NewEditLine(int linecount,int *previous,int oldsize)
{
    int *line;
    int i;
    linecount++;
    line=malloc(sizeof(int)*linecount);
    memset(line,0,sizeof(int)*linecount);
    if(previous != NULL)
        for(i=0;i<=oldsize;i++)
            line[i]=previous[i];
    return line;
}

void AddPos(int line,int newpos,SDL_EditParagraph *p)
{
    if(line > p->linecount)
    {
        p->linepos = NewEditLine(line,p->linepos,p->linecount);
        p->linecount = line;
    }
    p->linepos[line]=newpos;
}

void calcbreak(int width,SDL_Font *font,SDL_EditParagraph *p)
{
    SDL_String *string = p->Caption;
    int len       = strlen(string->Data);
    
    int newwidth=0;
    int stringsize = len;
    int pos=0;
    int line=1;

    while(pos < stringsize)
    {
        while(newwidth < width)
        {
            newwidth += SDL_FontGetCharWidth(font,string->Data[pos]);
            pos++;
            if(pos ==  stringsize)
                break;
        }
        newwidth = 0;
        AddPos(line,pos,p);
        line++;
    }    
        
}

void drawcursor(int x,int y,SDL_Surface *surface)
{
    SDL_Rect cursor;

    cursor.x = x;
    cursor.y = y;
    cursor.w = 1;
    cursor.h = 15;
    
    SDL_DrawLine(surface,cursor.x,cursor.y,cursor.x,cursor.y+cursor.h,0x000007ff);
}

void drawparagraph(SDL_Widget *widget,SDL_EditParagraph *p,SDL_Surface *surface,int *y)
{
    SDL_MultiLineEdit *Edit = (SDL_MultiLineEdit*)widget;
    SDL_String *string;
    SDL_String *substring;
    int count = p->linecount;
    int i=0;
    SDL_Rect StringPos;
    int width;

    StringPos.y = *y;
    StringPos.x = 2;
    StringPos.w = widget->Rect.w - 2;
    StringPos.h = widget->Rect.h;

    string = p->Caption;

    if(count == 0)
    {
        if(Edit->Cursor.p == p && SDL_WidgetHasFocus(widget))
           drawcursor(StringPos.x ,StringPos.y,surface);
        StringPos.y += Edit->Font->Height;
    }

    while(i < count)
    {
        substring = SDL_StringSubString(string,p->linepos[i],p->linepos[i+1]);
        width = SDL_FontGetStringWidth(Edit->Font,substring->Data);
        if(Edit->Cursor.p == p && SDL_WidgetHasFocus(widget))
            drawcursor(width ,StringPos.y,surface);
        SDL_FontDrawString(surface,Edit->Font,substring,Edit->fgcolor,&StringPos);
        SDL_StringFree(substring);
        StringPos.y += Edit->Font->Height;
        i++;
    }
    *y = StringPos.y;
}



SDL_EditParagraph *NewParagraph()
{
    SDL_EditParagraph *para;

    para = malloc(sizeof(SDL_EditParagraph));
    memset(para,0,sizeof(SDL_EditParagraph));
    
    para->Caption = SDL_StringPrealloc(4096);
   
    return para;
}



static void SDL_MultiLineEditInsert(SDL_MultiLineEdit *edit,unsigned int c)
{
    if(edit->Paragraph == NULL)
    {
        edit->Paragraph  = NewParagraph();
        edit->Cursor.p   = edit->Paragraph;
    }
    if(c >= 32 && c <= 126)
    {
        SDL_ParagraphInsertChar(edit->Cursor.p,edit->Cursor.CursorPosition,c);
        edit->Cursor.CursorPosition++;
    }
}    
    
void SDL_MultiLineEditSetText(SDL_Widget *widget,char *text)
{
    SDL_MultiLineEdit *Edit = (SDL_MultiLineEdit*)widget;
    
    Edit->Paragraph = NULL;
    
    /* split text on  \n */
    /* create a paragraph for each section */

    while(*text)
    {
        if(*text == '\n')
            AddParagraph(Edit);
        else
            SDL_MultiLineEditInsert(Edit,*text);
        text++;
    }
}  
    
SDL_String *SDL_MultiLineEditGetText(SDL_Widget *widget)
{
    static char buf[4096] = {0}; 
    SDL_EditParagraph *p;
    SDL_MultiLineEdit *Edit = (SDL_MultiLineEdit*)widget;
    int len = 0;
    /* Attention: here will occur memory leak!*/
    SDL_String *s = (SDL_String *)malloc(sizeof(SDL_String));
    memset(s, 0, sizeof(SDL_String));

    p = Edit->Paragraph;
    while (p) {
         sprintf(buf + len, "%s\n", p->Caption->Data);
         len = strlen(buf);
         p = p->next;
    }
    s->Data = buf;
    s->Length = len;
    
    return s;
}
