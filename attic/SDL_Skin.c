/*
  Skinning support for SDLTk
  Copyright (C) 2005-2006 John Beuving (john.beuving@beatforce.org)

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

#include "SDLTk.h"
#include "SDL_Skin.h"
#include "SDL_SkinWindow.h"
#include "SDL_SkinStyle.h"

static SDL_SkinTextStyle *SDL_SkinParseTextStyle(xmlDocPtr doc,xmlNodePtr cur);

static int SDL_SkinParseImages(xmlDocPtr doc,xmlNodePtr cur,SDL_SkinImage **imagelist);


char skinpath[255];
SDL_Skin *skinlist;

SDL_Image *SDL_SkinGetImage(SDL_Skin *skin,char *id);

int SDL_SkinInit()
{
    skinlist=0;
    return 1;    
}

int AddStyles(SDL_Skin *skin,SDL_SkinStyle *st)
{
    SDL_Style *news = SDL_StyleNew();
    
    if(st->RadioButton)
    {
        SDL_StyleSetImage(news,"SDL_RadioButton",0,SDL_SkinGetImage(skin,st->RadioButton->Normal));
        SDL_StyleSetImage(news,"SDL_RadioButton",1,SDL_SkinGetImage(skin,st->RadioButton->Selected));
    }
    if(st->HorizontalScrollbar)
    {
        if(st->HorizontalScrollbar->Normal)
        {
            SDL_StyleSetImage(news,"SDL_Scrollbar",0,SDL_SkinGetImage(skin,st->HorizontalScrollbar->Normal->Arrow1));
            SDL_StyleSetImage(news,"SDL_Scrollbar",1,SDL_SkinGetImage(skin,st->HorizontalScrollbar->Normal->Arrow2));
            SDL_StyleSetImage(news,"SDL_Scrollbar",2,SDL_SkinGetImage(skin,st->HorizontalScrollbar->Normal->Scrollbar));
            SDL_StyleSetImage(news,"SDL_Scrollbar",3,SDL_SkinGetImage(skin,st->HorizontalScrollbar->Normal->Thumb));
        }
    }
    if(st->VerticalScrollbar)
    {
        printf("Style contains V scrollbar\n");
    }
    SDL_StylePush(news);
    return 1;
}

SDL_Skin *SDL_SkinLoad(char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *key;
    char *p;
    SDL_Skin *skin;
    SDL_Skin *tempskin;
    
    strcpy(skinpath,filename);
    p=strrchr(skinpath,'/');
    if(p)
        *(++p)=0;

    skin=skinlist;
	
    while(skin)
    {
        if(skin->filename && !strcmp(skin->filename,filename))
            return skin;
        skin=skin->next;
    }

    doc = xmlParseFile(filename);
    if(doc != NULL)
    {
        cur = xmlDocGetRootElement(doc);
        if(cur == NULL)
        {
            xmlFreeDoc(doc);
            return NULL;
			
        }
        if(xmlStrcmp(cur->name,(const xmlChar *)"sdltk"))
        {
            xmlFreeDoc(doc);
            return NULL;
        }
		
        skin=(SDL_Skin*)malloc(sizeof(SDL_Skin));
        memset(skin,0,sizeof(SDL_Skin));
        
        skin->filename = SDLTk_strdup(filename);
        key=xmlGetProp(cur,"name");
        xmlFree(key);

		
        cur=cur->xmlChildrenNode;
        while(cur != NULL)
        {

            if(!xmlStrcmp(cur->name,(const xmlChar *)"images"))
                SDL_SkinParseImages(doc,cur,&skin->images);
            else if(!xmlStrcmp(cur->name,(const xmlChar *)"style"))
                SDL_SkinParseStyle(doc,cur,&skin->styles);
            else if(!xmlStrcmp(cur->name,(const xmlChar *)"window"))
                SDL_SkinParseWindow(doc,cur,&skin->window);
            else if(!xmlStrcmp(cur->name,(const xmlChar *)"text"))
            {
                /* text as root node can definitly be ignored */
                /* printf(">%s<\n",xmlNodeGetContent(cur)); */
            }
            else
                printf("Not supported root tag |%s|\n",cur->name);
            cur=cur->next;
        }

        if(skinlist == NULL)
            skinlist = skin;
        else
        {
            tempskin = skinlist;
            while(tempskin->next)
                tempskin=tempskin->next;
            tempskin->next = skin;

        }
        AddStyles(skin,skin->styles);
        return skin;
    }
    return NULL;
}

SDL_Image *SDL_SkinGetImage(SDL_Skin *skin,char *id)
{
    SDL_SkinImage *i;


    if(skin == NULL || id == NULL)
        return NULL;

    i=skin->images;
    while(i)
    {
        if(i->id != NULL)
        {
            if(!strcmp(id,i->id))
            {
                if(i->image == NULL)
                {
                    char imagefile[260];
                    SDL_Rect selection;
					
                    sprintf(imagefile,"%s%s",skinpath,i->filename);
                    
                    selection.x = i->x;
                    selection.y = i->y;
                    selection.w = i->w;
                    selection.h = i->h;

                    i->image = SDL_ImageLoad(imagefile,&selection);
                    SDL_ImageSetMargins(i->image,i->left,i->top,i->bottom,i->right);

                }
               
                if(i->image == NULL)
                    printf("Could not load %s\n",i->filename);
                return i->image;
            }
        }
        else
        {
            int len;
            len=strlen(id);

            if(!strncmp(i->filename,id,len))
            {
                if(i->image == NULL)
                {
                    char imagefile[260];
                    sprintf(imagefile,"%s%s",skinpath,i->filename);
                    i->image = SDL_ImageLoad(imagefile,NULL);
                }
                if(i->image == NULL)
                    printf("Could not load %s\n",i->filename);
                return i->image;

            }

        }
        i=i->next;
    }
    printf("Could not find %s\n",id);
    return NULL;
}


int SDL_SkinApplyWindow(SDL_Window *window,SDL_Skin *skin,SDL_SkinWindow *w)
{
    SDL_SkinButton    *b;
    SDL_SkinFrame     *f;
    SDL_SkinLabel     *l;
    SDL_SkinPanel     *p;
    SDL_SkinScrollbar *s;
    SDL_SkinRadioButton    *r;
    SDL_SkinTable     *table;
    SDL_SkinTree      *tree;
    SDL_SkinWidget    *widget;
    char fontfile[260];


    skin->window->window = window;

    if(w->Background != NULL)
    {
        w->BackgroundWidget=SDL_PanelNew();
        SDL_PanelSetImage(w->BackgroundWidget,SDL_SkinGetImage(skin,w->Background));
        SDL_WidgetShow(w->BackgroundWidget);
    }
    widget=w->widget;
    while(widget)
    {
        switch(widget->type)
        {
        case SKINBUTTON:
            b=widget->w.button;		
            widget->widget=SDL_ButtonNew();
            SDL_ButtonSetImage(widget->widget, SDL_BUTTON_STATE_UP,   SDL_SkinGetImage(skin,b->normal));
            SDL_ButtonSetImage(widget->widget, SDL_BUTTON_STATE_DOWN, SDL_SkinGetImage(skin,b->pressed));
            SDL_WidgetSetDims(widget->widget,b->x,b->y,b->w,b->h);
            SDL_WidgetShow(widget->widget);
            break;
        case SKINLABEL:
            l=widget->w.label;
            widget->widget=SDL_LabelNew();
            SDL_LabelSetText(widget->widget,l->caption);
            
            if(l->textstyle != NULL)
            {
                SDL_LabelSetColor(widget->widget,BACKGROUND,BLACK);
                SDL_LabelSetColor(widget->widget,FOREGROUND,l->textstyle->textcolor);
                SDL_LabelSetAlignment(widget->widget,l->textstyle->justify);
                if(l->textstyle->fontfile != NULL)
                {
                    SDL_Font *font;
                    sprintf(fontfile,"%s%s",skinpath,l->textstyle->fontfile);
                    
                    /* Load a font */
                    font = SDL_FontLoad(fontfile);
                    SDL_LabelSetFont(widget->widget,font);
                    
                    if(l->textstyle->height > 0)
                        SDL_LabelSetFontHeight(widget->widget,l->textstyle->height);

                    printf("font %s %p\n",l->textstyle->fontfile,font);
                }
            }
            SDL_WidgetSetDims(widget->widget,l->x,l->y,l->w,l->h);
            SDL_WidgetShow(widget->widget);
            break;
        case SKINFRAME:
            f=widget->w.frame;
            widget->widget=SDL_FrameNew();
            if(f->titlecenter)
                SDL_FrameSetImage(widget->widget,FRAME_TITLE_CENTER,SDL_SkinGetImage(skin,f->titlecenter->image));
            if(f->titleleft)
                SDL_FrameSetImage(widget->widget,FRAME_TITLE_LEFT,SDL_SkinGetImage(skin,f->titleleft->image));
            if(f->titleright)
                SDL_FrameSetImage(widget->widget,FRAME_TITLE_RIGHT,SDL_SkinGetImage(skin,f->titleright->image));
            if(f->left)
                SDL_FrameSetImage(widget->widget,FRAME_LEFT,SDL_SkinGetImage(skin,f->left->image));
            if(f->right)
                SDL_FrameSetImage(widget->widget,FRAME_RIGHT,SDL_SkinGetImage(skin,f->right->image));
            if(f->bottomleft)
                SDL_FrameSetImage(widget->widget,FRAME_BOTTOM_LEFT,SDL_SkinGetImage(skin,f->bottomleft->image));
            if(f->bottomright)
                SDL_FrameSetImage(widget->widget,FRAME_BOTTOM_RIGHT,SDL_SkinGetImage(skin,f->bottomright->image));
            if(f->bottom)
                SDL_FrameSetImage(widget->widget,FRAME_BOTTOM,SDL_SkinGetImage(skin,f->bottom->image));
			
			
            SDL_WidgetSetDims(widget->widget,0,0,1,1);
            
            SDL_WidgetShow(widget->widget);
            break;
        case SKINPANEL:
            p=widget->w.panel;
            widget->widget=SDL_PanelNew();
            SDL_WidgetSetDims(widget->widget,p->x,p->y,p->w,p->h);
            SDL_PanelSetImage(widget->widget,SDL_SkinGetImage(skin,p->image));
            SDL_WidgetShow(widget->widget);
            break;
        case SKINRADIOBUTTON:
            r=widget->w.radiobutton;
            widget->widget=SDL_RadioButtonNew();
            SDL_WidgetSetDims(widget->widget,r->x,r->y,r->w,r->h);
            SDL_RadioButtonSetImage(widget->widget,0,SDL_SkinGetImage(skin,r->Style.Normal));
            SDL_WidgetShow(widget->widget);
            break;
        case SKINSCROLLBAR:
            s=widget->w.scrollbar;
            widget->widget=SDL_ScrollbarNew();
#if 0
            SDL_ScrollbarSetImage(widget->widget,SCROLLBAR_IMAGE_NORMAL_THUMB,  SDL_SkinGetImage(skin,s->thumb));
            SDL_ScrollbarSetImage(widget->widget,SCROLLBAR_IMAGE_NORMAL_BAR,    SDL_SkinGetImage(skin,s->scrollbar));
            SDL_ScrollbarSetImage(widget->widget,SCROLLBAR_IMAGE_PRESSED_THUMB, SDL_SkinGetImage(skin,s->thumbpressed));
#endif
            SDL_WidgetSetDims(widget->widget,s->x,s->y,s->w,s->h);
            SDL_WidgetShow(widget->widget);
            break;
        case SKINTABLE:
            table=widget->w.table;
            widget->widget=SDL_TableNew();
            SDL_TableSetFont(widget->widget,table->font);
            SDL_TableSetSelectionMode(widget->widget,table->mode);
            SDL_WidgetSetDims(widget->widget,table->x,table->y,table->w,table->h);
            SDL_WidgetShow(widget->widget);			
            break;
        case SKINTREE:
            tree=widget->w.tree;
            widget->widget=SDL_TreeNew();
            SDL_WidgetSetDims(widget->widget,tree->x,tree->y,tree->w,tree->h);
            SDL_WidgetShow(widget->widget);			
            break;


        }
        widget=widget->next;
    }
    return 1;
}

int SDL_SkinApplyByName(SDL_Window *window,SDL_Skin *skin,char *name)
{
    SDL_SkinWindow    *w;

    if(skin == NULL)
    {
        printf("Skin is NULL\n");
        return 0;
    }
    w=skin->window;
    while(w)
    {
        if(!strcmp(w->id,name))	
        {
            return SDL_SkinApplyWindow(window,skin,w);
        }
        w=w->next;
    }
    return 0;
}


int SDL_SkinApply(SDL_Window *window,SDL_Skin *skin)
{
    SDL_SkinWindow    *w;

    w=skin->window;
    return SDL_SkinApplyWindow(window,skin,w);
}





static SDL_SkinImage *SDL_SkinParseImageMargins(SDL_SkinImage *image,xmlDocPtr doc,xmlNodePtr cur)
{
    xmlChar *key;

    key=xmlGetProp(cur,"left");
    if(key)
        image->left = atoi(key);
    
    key=xmlGetProp(cur,"top");
    if(key)
        image->top = atoi(key);

    key=xmlGetProp(cur,"bottom");
    if(key)
        image->bottom = atoi(key);

    key=xmlGetProp(cur,"right");
    if(key)
        image->right = atoi(key);

    return image;

}

static SDL_SkinImage *SDL_SkinParseImage(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinImage *image;
    xmlChar *key;

    image=(SDL_SkinImage*)malloc(sizeof(SDL_SkinImage));
    memset(image,0,sizeof(SDL_SkinImage));

    key=xmlGetProp(cur,"id");
    image->id = key;
	
    key=xmlGetProp(cur,"file");
    image->filename=key;

    key=xmlGetProp(cur,"x");
    if(key)
        image->x = atoi(key);
    
    key=xmlGetProp(cur,"y");
    if(key)
        image->y = atoi(key);

    key=xmlGetProp(cur,"w");
    if(key)
        image->w = atoi(key);

    key=xmlGetProp(cur,"h");
    if(key)
        image->h = atoi(key);

    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {    
        if(!xmlStrcmp(cur->name,(const xmlChar *)"margins"))
            image=SDL_SkinParseImageMargins(image,doc,cur);
        cur=cur->next;
    }
    return image;
}

static int SDL_SkinParseImages(xmlDocPtr doc,xmlNodePtr cur,SDL_SkinImage **imagelist)
{

    SDL_SkinImage *image;

	

    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        image=NULL;
		
        if(!xmlStrcmp(cur->name,(const xmlChar *)"image"))
        {
            image=SDL_SkinParseImage(doc,cur);
        }
        if(image != NULL)
        {
            SDL_SkinImage *i;

            i=*imagelist;
            if(i == NULL)
                *imagelist=image;
            else
            {
                while(i->next)
                    i=i->next;
                i->next = image;

            }

        }
        cur=cur->next;
    }

    return 1;
}


SDL_SkinWidget *SDL_SkinParseLabel(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinLabel  *label;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));

    label=(SDL_SkinLabel*)malloc(sizeof(SDL_SkinLabel));
    memset(label,0,sizeof(SDL_SkinLabel));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    label->x = atoi(key);
    key=xmlGetProp(cur,"y");
    label->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        label->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        label->h = atoi(key);

    key=xmlGetProp(cur,"caption");
    label->caption = key;
	
    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        if(!xmlStrcmp(cur->name,(const xmlChar *)"textstyle"))
        {
            label->textstyle=SDL_SkinParseTextStyle(doc,cur);
        }
        cur=cur->next;
    }
    widget->type   = SKINLABEL;
    widget->w.label = label;

    return widget;
}





static SDL_SkinTextStyle *SDL_SkinParseTextStyle(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinTextStyle *style;
    xmlChar *key;
    int r=0,g=0,b=0;

    style=(SDL_SkinTextStyle*)malloc(sizeof(SDL_SkinTextStyle));
    memset(style,0,sizeof(SDL_SkinTextStyle));

    key=xmlGetProp(cur,"justify");
    if(key)
    {
        if(!strcmp(key,"top-left"))
            style->justify=TOPLEFT;
        else if(!strcmp(key,"top-center"))
            style->justify=TOPCENTER;
        else if(!strcmp(key,"top-right"))
            style->justify=TOPRIGHT;
        else if(!strcmp(key,"center-left"))
            style->justify=CENTERLEFT;
        else if(!strcmp(key,"center"))
            style->justify=CENTER;
        else if(!strcmp(key,"center-right"))
            style->justify=CENTERRIGHT;
        else if(!strcmp(key,"bottom-left"))
            style->justify=BOTTOMLEFT;
        else if(!strcmp(key,"bottom-center"))
            style->justify=BOTTOMCENTER;
        else if(!strcmp(key,"bottom-right"))
            style->justify=BOTTOMRIGHT;
        else
            style->justify = TOPLEFT;

    }
    else
        style->justify = TOPLEFT;

    cur=cur->xmlChildrenNode;
    while(cur != NULL)
    {
        if(!xmlStrcmp(cur->name,(const xmlChar *)"textcolor"))
        {
            key=xmlGetProp(cur,"r");
            if(key != NULL)
                r = atoi(key);
            key=xmlGetProp(cur,"g");
            if(key != NULL)
                g = atoi(key);
            key=xmlGetProp(cur,"b");
            if(key != NULL)
                b = atoi(key);
            style->textcolor = SDL_MapRGB(SDLTk_GetVideoSurface()->format,r,g,b);
        }
        if(!xmlStrcmp(cur->name,(const xmlChar *)"textfont"))
        {
            key=xmlGetProp(cur,"file");
            if(key != NULL)
                style->fontfile = SDLTk_strdup(key);
            key=xmlGetProp(cur,"height");
            if(key != NULL)
                style->height = atoi(key);
        }
        cur=cur->next;
    }

	
    return style;
}




SDL_SkinWidget *SDL_SkinParseTable(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinTable *table;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));


    table=(SDL_SkinTable*)malloc(sizeof(SDL_SkinTable));
    memset(table,0,sizeof(SDL_SkinTable));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    if(key != NULL)
        table->x = atoi(key);
    key=xmlGetProp(cur,"y");
    if(key != NULL)
        table->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        table->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        table->h = atoi(key);
	
    key=xmlGetProp(cur,"font");
    if(key != NULL)
    {
        char fontpath[255];
        sprintf(fontpath,"%s%s",skinpath,key);
        
        table->font = SDL_FontLoad(fontpath);
    }

    key=xmlGetProp(cur,"mode");
    if(key != NULL)
    {
        if(!strcmp(key,"none"))
            table->mode = TABLE_MODE_NONE;       /* No selection possible             */
        if(!strcmp(key,"single"))
            table->mode = TABLE_MODE_SINGLE;     /* Only one can be selected          */
        if(!strcmp(key,"multiple"))
            table->mode = TABLE_MODE_MULTIPLE;    /* Multiple entries can be selected  */
    }
    widget->type   = SKINTABLE;
    widget->w.table = table;
    return widget;
}


SDL_SkinWidget *SDL_SkinParseTree(xmlDocPtr doc,xmlNodePtr cur)
{
    SDL_SkinWidget *widget;
    SDL_SkinTree *tree;
    xmlChar *key;

    widget=(SDL_SkinWidget*)malloc(sizeof(SDL_SkinWidget));
    memset(widget,0,sizeof(SDL_SkinWidget));


    tree=(SDL_SkinTree*)malloc(sizeof(SDL_SkinTree));
    memset(tree,0,sizeof(SDL_SkinTree));

    key=xmlGetProp(cur,"id");
    widget->id = key;
    key=xmlGetProp(cur,"x");
    tree->x = atoi(key);
    key=xmlGetProp(cur,"y");
    tree->y = atoi(key);
	
    key=xmlGetProp(cur,"w");
    if(key != NULL)
        tree->w = atoi(key);
	
    key=xmlGetProp(cur,"h");
    if(key != NULL)
        tree->h = atoi(key);
	
    widget->type   = SKINTREE;
    widget->w.tree = tree;
    return widget;
}



SDL_Widget *SDL_SkinGetWidget(SDL_Skin *skin,char *id)
{
    SDL_SkinWindow *wind;
    SDL_SkinWidget *w;

    if(skin == NULL)
        return NULL;
    
    wind=skin->window;
    while(wind)
    {
        w=wind->widget;
        while(w)
        {
            if(w->id && !strcmp(w->id,id))
                return w->widget;
            w=w->next;
        }
        wind=wind->next;
    }
    return NULL;
}

SDL_Skin *SDL_SkinGetSkin(SDL_SkinWindow *window)
{
    SDL_Skin *list;

    list=skinlist;

    while(list)
    {
        if(list->window && list->window == window)
            return list;
        list=list->next;
		
    }
    return NULL;
}
