/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#define LOCAL_DEBUG

#include "../../../../configure.h"
#include "../../../../libAfterImage/afterimage.h"
#include "../../../../libAfterStep/asapp.h"
#include "../../../../libAfterStep/screen.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdk.h>	
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GdkColor* color_name2GdkColor( const char *name, GdkColor *color )
{
	ARGB32 argb ;

	parse_argb_color( name, &argb );
	color->pixel = argb ;
	color->red = ARGB32_RED16(argb);
	color->green = ARGB32_GREEN16(argb);
	color->blue = ARGB32_BLUE16(argb);
	return color;
}

static void free_buffer (guchar *pixels, gpointer data)
{
	g_free (pixels);
}


GdkPixbuf *
ASImage2GdkPixbuf( ASImage *im, Bool copy ) 
{
	GdkPixbuf *pb = NULL ; 
	if( im ) 
	{
		ARGB32 *argb = im->alt.argb32 ;
		ASImage *tmp = NULL ;
		int i, k = 0;
		int size = im->width*im->height;
		guchar *data ;

		if( argb == NULL) 
		{
			tmp = tile_asimage( Scr.asv, im, 0, 0, im->width, im->height, TINT_LEAVE_SAME, ASA_ARGB32, 0, ASIMAGE_QUALITY_DEFAULT );
			if( tmp == NULL ) 
				return NULL;
			argb = tmp->alt.argb32 ;
		}
		
		data = safemalloc( size*4 );
		for( i = 0 ; i < size ; ++i ) 
		{
			data[k] = ARGB32_RED8(argb[i]);
			data[++k] = ARGB32_GREEN8(argb[i]);
			data[++k] = ARGB32_BLUE8(argb[i]);
			data[++k] = ARGB32_ALPHA8(argb[i]);
			++k;
		}
		
		pb = gdk_pixbuf_new_from_data( data, GDK_COLORSPACE_RGB, True, 8, im->width, im->height, im->width*4, free_buffer, NULL );
		if( pb == NULL ) 
			free( data );
			
		if( tmp ) 
			destroy_asimage( &tmp );
			
	}	 
	return pb;
}	 

static void
on_main_list_selection_change(GtkTreeSelection *selection, gpointer user_data)
{
  	GtkTreeIter iter;
  	GtkTreeModel *model;

  	if (gtk_tree_selection_get_selected (selection, &model, &iter)) 
	{
		gpointer p = NULL ;
		ASImageListEntry *curr ;

    	gtk_tree_model_get (model, &iter, 1, &p, -1);
		if( (curr = p) != NULL && curr->preview ) 
		{
			GdkPixbuf *pb = NULL ; 
			ASImage *scaled = NULL, *tiled = NULL; 
			int scaled_w = (curr->preview->width * WallpaperState.preview_width )/ Scr.MyDisplayWidth ; 
			int scaled_h = (curr->preview->height * WallpaperState.preview_height )/ Scr.MyDisplayHeight ;
			ASAltImFormats format = ( scaled_w != WallpaperState.preview_width || scaled_h != WallpaperState.preview_height )? ASA_ASImage: ASA_ARGB32 ;
			
			LOCAL_DEBUG_OUT( "scaled size is %dx%d", scaled_w, scaled_h );
			scaled = scale_asimage( Scr.asv, curr->preview, scaled_w, scaled_h, format, 0, ASIMAGE_QUALITY_DEFAULT );			
			if( scaled && ( scaled_w != WallpaperState.preview_width || scaled_h != WallpaperState.preview_height ))
			{
				tiled = tile_asimage( Scr.asv, scaled, 0, 0, 
									  WallpaperState.preview_width, 
									  WallpaperState.preview_height, 
									  TINT_LEAVE_SAME, ASA_ARGB32, 0, ASIMAGE_QUALITY_DEFAULT );				   
				LOCAL_DEBUG_OUT( "tiled size is %dx%d", tiled->width, tiled->height );
			}	 
			pb = ASImage2GdkPixbuf( tiled?tiled:(scaled?scaled:curr->preview), False );
			if( tiled ) 
				destroy_asimage( &tiled );
			if( scaled ) 
				destroy_asimage( &scaled );
			gtk_image_set_from_pixbuf( WallpaperState.list_preview, pb );
			gdk_pixbuf_unref( pb ); 		   
		    LOCAL_DEBUG_OUT( "####!!! recquisition is %dx%d",  
							 GTK_WIDGET(WallpaperState.list_preview)->requisition.width,  
							 GTK_WIDGET(WallpaperState.list_preview)->requisition.height );
		}	 
  	}
}

void 
list_preview_size_alloc   ( GtkWidget *widget,
							GtkAllocation *allocation,
							gpointer user_data )
{
	int w = allocation->width ;		   
	int h = allocation->height ;
	LOCAL_DEBUG_OUT( "####!!! SizeAlloc for %p is %dx%d%+d%+d", widget, allocation->width, allocation->height, allocation->x, allocation->y );
    LOCAL_DEBUG_OUT( "####!!! recquisition was %dx%d",  widget->requisition.width,  widget->requisition.height );
	if( WallpaperState.preview_width != w || WallpaperState.preview_height != h ) 
	{
		WallpaperState.preview_width = w ; 
		WallpaperState.preview_height = h ;
		on_main_list_selection_change(gtk_tree_view_get_selection(WallpaperState.backs_list), NULL);
	}	 
}								  

void
on_list_add_clicked(GtkButton *button, gpointer user_data)
{
	
}
void
on_list_del_clicked(GtkButton *button, gpointer user_data)
{
	
}

void
on_list_apply_clicked(GtkButton *button, gpointer user_data)
{
	
}

void
on_make_xml_clicked(GtkButton *button, gpointer user_data)
{
	
}

GtkStyle *ASGtkStyleNormal = NULL; 
GtkStyle *ASGtkStyleButton = NULL; 
GtkStyle *get_colorschemed_style_normal()
{
	if( ASGtkStyleNormal == NULL ) 
	{
		ASGtkStyleNormal = gtk_style_copy(GTK_WIDGET(WallpaperState.main_window)->style); 		
		color_name2GdkColor("InactiveText1"   , &(ASGtkStyleNormal->fg[GTK_STATE_NORMAL]));
		color_name2GdkColor("InactiveText2"   , &(ASGtkStyleNormal->fg[GTK_STATE_ACTIVE]));
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleNormal->fg[GTK_STATE_PRELIGHT]));
		color_name2GdkColor("ActiveText"	  , &(ASGtkStyleNormal->fg[GTK_STATE_SELECTED]));
		color_name2GdkColor("DisabledText"    , &(ASGtkStyleNormal->fg[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->bg[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2"   , &(ASGtkStyleNormal->bg[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactive", &(ASGtkStyleNormal->bg[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	  , &(ASGtkStyleNormal->bg[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->bg[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1Light"   , &(ASGtkStyleNormal->light[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2Light"   , &(ASGtkStyleNormal->light[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveLight", &(ASGtkStyleNormal->light[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveLight"  	   , &(ASGtkStyleNormal->light[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1Light"   , &(ASGtkStyleNormal->light[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1Dark"   , &(ASGtkStyleNormal->dark[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2Dark"   , &(ASGtkStyleNormal->dark[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveDark", &(ASGtkStyleNormal->dark[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveDark"	  , &(ASGtkStyleNormal->dark[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1Dark"   , &(ASGtkStyleNormal->dark[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->mid[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2"   , &(ASGtkStyleNormal->mid[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactive", &(ASGtkStyleNormal->mid[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	  , &(ASGtkStyleNormal->mid[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->mid[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("InactiveText1"   , &(ASGtkStyleNormal->text[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("InactiveText2"   , &(ASGtkStyleNormal->text[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleNormal->text[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveText"	  , &(ASGtkStyleNormal->text[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("InactiveText1"   , &(ASGtkStyleNormal->text[GTK_STATE_INSENSITIVE]));

		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->base[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2"   , &(ASGtkStyleNormal->base[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactive", &(ASGtkStyleNormal->base[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	  , &(ASGtkStyleNormal->base[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleNormal->base[GTK_STATE_INSENSITIVE]));
#if 0
		color_name2GdkColor("", &(ASGtkStyleNormal->text_aa[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("", &(ASGtkStyleNormal->text_aa[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("", &(ASGtkStyleNormal->text_aa[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("", &(ASGtkStyleNormal->text_aa[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("", &(ASGtkStyleNormal->text_aa[GTK_STATE_INSENSITIVE]));
#endif
	}	 
	return ASGtkStyleNormal;
}	 
GtkStyle *get_colorschemed_style_button()
{
	if( ASGtkStyleButton == NULL ) 
	{
		ASGtkStyleButton = gtk_style_copy(GTK_WIDGET(WallpaperState.main_window)->style); 		
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleButton->fg[GTK_STATE_NORMAL]));
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleButton->fg[GTK_STATE_ACTIVE]));
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleButton->fg[GTK_STATE_PRELIGHT]));
		color_name2GdkColor("ActiveText"	  , &(ASGtkStyleButton->fg[GTK_STATE_SELECTED]));
		color_name2GdkColor("DisabledText"    , &(ASGtkStyleButton->fg[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("HighInactiveBack" , &(ASGtkStyleButton->bg[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("HighInactiveLight", &(ASGtkStyleButton->bg[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactive"     , &(ASGtkStyleButton->bg[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	       , &(ASGtkStyleButton->bg[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("HighInactive"     , &(ASGtkStyleButton->bg[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1Light"   , &(ASGtkStyleButton->light[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2Light"   , &(ASGtkStyleButton->light[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveLight", &(ASGtkStyleButton->light[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveLight"  	   , &(ASGtkStyleButton->light[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1Light"   , &(ASGtkStyleButton->light[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1Dark"   , &(ASGtkStyleButton->dark[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2Dark"   , &(ASGtkStyleButton->dark[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveDark", &(ASGtkStyleButton->dark[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveDark"	  , &(ASGtkStyleButton->dark[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1Dark"   , &(ASGtkStyleButton->dark[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleButton->mid[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("Inactive2"   , &(ASGtkStyleButton->mid[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactive", &(ASGtkStyleButton->mid[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	  , &(ASGtkStyleButton->mid[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("Inactive1"   , &(ASGtkStyleButton->mid[GTK_STATE_INSENSITIVE]));
		color_name2GdkColor("InactiveText1"   , &(ASGtkStyleButton->text[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("InactiveText2"   , &(ASGtkStyleButton->text[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveText", &(ASGtkStyleButton->text[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("ActiveText"	  , &(ASGtkStyleButton->text[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("InactiveText1"   , &(ASGtkStyleButton->text[GTK_STATE_INSENSITIVE]));

		color_name2GdkColor("HighInactiveDark" , &(ASGtkStyleButton->base[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("HighInactive"     , &(ASGtkStyleButton->base[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("HighInactiveLight", &(ASGtkStyleButton->base[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("Active"	       , &(ASGtkStyleButton->base[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("HighInactive"     , &(ASGtkStyleButton->base[GTK_STATE_INSENSITIVE]));
#if 0
		color_name2GdkColor("", &(ASGtkStyleButton->text_aa[GTK_STATE_NORMAL]     ));
		color_name2GdkColor("", &(ASGtkStyleButton->text_aa[GTK_STATE_ACTIVE]     ));
		color_name2GdkColor("", &(ASGtkStyleButton->text_aa[GTK_STATE_PRELIGHT]   ));
		color_name2GdkColor("", &(ASGtkStyleButton->text_aa[GTK_STATE_SELECTED]   ));
		color_name2GdkColor("", &(ASGtkStyleButton->text_aa[GTK_STATE_INSENSITIVE]));
#endif
	}	 
	return ASGtkStyleButton;
}	 

void 
colorize_gtk_window( GtkWidget *window )
{
	GdkColor bg ;
	color_name2GdkColor("BaseLight", &bg);
  	gtk_widget_modify_bg( window, GTK_STATE_NORMAL, &bg );
}	

void 
colorize_gtk_tree_view( GtkCellRenderer *cell, GtkTreeViewColumn *column )
{
#if 0	  
	GdkColor bg;
	GdkColor fg;
	
	color_name2GdkColor("Inactive1", &bg);
	color_name2GdkColor("InactiveText1", &fg);

	g_object_set(G_OBJECT(cell), "background-gdk", &bg, "foreground-gdk", &fg, NULL );
	g_object_set(G_OBJECT(column), "background-gdk", &bg, "foreground-gdk", &fg, NULL );
#else
	gtk_widget_set_style( GTK_WIDGET(cell), get_colorschemed_style_normal());
#endif
	
}	   

void  
colorize_gtk_widget(GtkWidget *widget, gpointer data)
{
	GtkStyle *style = data?GTK_STYLE(data):NULL;
	int i ; 

	if( style == NULL ) 
		style = get_colorschemed_style_normal();

	gtk_widget_set_style( widget, style);
	for( i = 0 ; i < 5 ; ++i ) 
		gtk_widget_modify_fg(widget, i, &(style->fg[i]));
	LOCAL_DEBUG_OUT( "widget %p", widget );
	if( GTK_IS_CONTAINER(widget) )
		gtk_container_forall( GTK_CONTAINER(widget), colorize_gtk_widget, data );
}	 



void
create_main_window (void)
{
    GtkWidget *main_vbox;
  	GtkWidget *Quit;

	GtkWidget *separator, *buttons_hbox;

  	WallpaperState.main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title (GTK_WINDOW (WallpaperState.main_window), _("AfterStep Wallpaper Manager"));

 	colorize_gtk_window( WallpaperState.main_window ); 	
		
	main_vbox = gtk_vbox_new (FALSE, 0);
  	gtk_widget_show (main_vbox);
	gtk_container_add (GTK_CONTAINER (WallpaperState.main_window), main_vbox);

  	WallpaperState.list_hbox = gtk_hbox_new (FALSE, 0);
  	gtk_widget_show (WallpaperState.list_hbox);
  	gtk_box_pack_start (GTK_BOX (main_vbox), WallpaperState.list_hbox, TRUE, TRUE, 5);

  	buttons_hbox = gtk_hbutton_box_new ();
  	gtk_hbutton_box_set_layout_default(GTK_BUTTONBOX_SPREAD);
  	gtk_widget_show (buttons_hbox);
  	gtk_box_pack_end (GTK_BOX (main_vbox), buttons_hbox, FALSE, FALSE, 5);
    /* separator really goes above the buttons box, so it is added second from the end ! */
	separator = gtk_hseparator_new();
	gtk_widget_show (separator);
	gtk_box_pack_end (GTK_BOX (main_vbox), separator, FALSE, FALSE, 5);

	colorize_gtk_widget( GTK_WIDGET(separator), get_colorschemed_style_button() );  

	   
  	Quit = gtk_button_new_from_stock ("gtk-quit");
  	gtk_widget_show (Quit);
  	gtk_box_pack_start (GTK_BOX (buttons_hbox), Quit, FALSE, FALSE, 20);
  	gtk_widget_set_size_request (Quit, 150, -1);
	colorize_gtk_widget( GTK_WIDGET(Quit), get_colorschemed_style_button() );  

  	g_signal_connect ((gpointer) Quit, "clicked",
    	                G_CALLBACK (on_quit_clicked),
        	            NULL);

  	/* Store pointers to all widgets, for use by lookup_widget(). */
  	GLADE_HOOKUP_OBJECT_NO_REF (WallpaperState.main_window, WallpaperState.main_window, "main_window");
  	GLADE_HOOKUP_OBJECT (WallpaperState.main_window, Quit, "Quit");
}

GtkWidget*
create_list_button( GtkWidget *buttons_hbox, const char *stock, GCallback func )
{
	GtkWidget *btn = gtk_button_new_from_stock (stock);
  	gtk_widget_show (btn);
  	gtk_box_pack_start (GTK_BOX (buttons_hbox), btn, FALSE, FALSE, 0);
  	g_signal_connect ((gpointer) btn, "clicked", G_CALLBACK (func), NULL);
	colorize_gtk_widget( GTK_WIDGET(btn), get_colorschemed_style_button());
	return btn;	
}	   

void
create_backs_list()
{
	GtkWidget *list_window ;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;
	GtkWidget *vbox ;
	GtkWidget *buttons_hbox;
  	
	vbox = gtk_vbox_new (FALSE, 0);
  	gtk_widget_show (vbox);
  	gtk_box_pack_start (GTK_BOX (WallpaperState.list_hbox), vbox, FALSE, FALSE, 5);

  	list_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (list_window),
				    				GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (vbox), list_window, TRUE, TRUE, 0);
	gtk_widget_set_size_request (list_window, 200, INITIAL_PREVIEW_HEIGHT);
	gtk_widget_show (list_window);

	buttons_hbox = gtk_hbutton_box_new ();
  	gtk_hbutton_box_set_layout_default(GTK_BUTTONBOX_SPREAD);
  	gtk_widget_show (buttons_hbox);
  	gtk_box_pack_end (GTK_BOX (vbox), buttons_hbox, FALSE, FALSE, 5);
	
	/* creating the list widget itself */
	WallpaperState.backs_list = GTK_TREE_VIEW(gtk_tree_view_new());

    
	WallpaperState.list_model = GTK_TREE_MODEL(gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER));
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (list_window), GTK_WIDGET(WallpaperState.backs_list));
    gtk_tree_view_set_model (WallpaperState.backs_list, WallpaperState.list_model);
    gtk_widget_show (GTK_WIDGET(WallpaperState.backs_list));
    cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes ("Images in your private backgrounds folder:", cell, "text", 0, NULL);
    gtk_tree_view_append_column (WallpaperState.backs_list, GTK_TREE_VIEW_COLUMN (column));

	colorize_gtk_widget( GTK_WIDGET(WallpaperState.backs_list), get_colorschemed_style_button());
	gtk_widget_set_style( GTK_WIDGET(WallpaperState.backs_list), get_colorschemed_style_normal());
	{	
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW (WallpaperState.backs_list));
   		gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
   		g_signal_connect (selection, "changed",  G_CALLBACK (on_main_list_selection_change), NULL);
	}

	/* adding list manipulation buttons : */
  	
	WallpaperState.list_add_button = create_list_button( buttons_hbox, GTK_STOCK_ADD, G_CALLBACK(on_list_add_clicked) );
	WallpaperState.list_del_button = create_list_button( buttons_hbox, GTK_STOCK_DELETE, G_CALLBACK(on_list_del_clicked) );
	WallpaperState.list_apply_button = create_list_button( buttons_hbox, GTK_STOCK_APPLY, G_CALLBACK(on_list_apply_clicked) );
}

void 
create_list_preview()
{
	GtkWidget *vbox ;
	GtkWidget *buttons_hbox;

  	vbox = gtk_vbox_new (FALSE, 0);
  	gtk_widget_show (vbox);
  	gtk_box_pack_end (GTK_BOX (WallpaperState.list_hbox), vbox, TRUE, TRUE, 5);

	WallpaperState.preview_height = INITIAL_PREVIEW_HEIGHT ;
	WallpaperState.preview_width = (WallpaperState.preview_height *Scr.MyDisplayWidth)/Scr.MyDisplayHeight;


  	WallpaperState.list_preview_container = gtk_aspect_frame_new (NULL, 0.5, 0.5, (gfloat)Scr.MyDisplayWidth/(gfloat)Scr.MyDisplayHeight, FALSE );
	gtk_box_pack_start (GTK_BOX (vbox), WallpaperState.list_preview_container, TRUE, TRUE, 0);
	gtk_widget_set_size_request (WallpaperState.list_preview_container, 
								 WallpaperState.preview_width+2, 
								 WallpaperState.preview_height+2);
	gtk_widget_show (WallpaperState.list_preview_container);


	WallpaperState.list_preview = GTK_IMAGE(gtk_image_new_from_pixbuf(NULL));
  	gtk_widget_show (GTK_WIDGET(WallpaperState.list_preview));
  	gtk_widget_set_size_request( GTK_WIDGET(WallpaperState.list_preview),
								 WallpaperState.preview_width, 
								 WallpaperState.preview_height );
	
	gtk_container_add (GTK_CONTAINER (WallpaperState.list_preview_container), 
					    GTK_WIDGET(WallpaperState.list_preview));

	g_signal_connect ((gpointer) WallpaperState.list_preview, "size-allocate",
                       G_CALLBACK (list_preview_size_alloc), NULL);
	
	buttons_hbox = gtk_hbutton_box_new ();
  	gtk_hbutton_box_set_layout_default(GTK_BUTTONBOX_SPREAD);
  	gtk_widget_show (buttons_hbox);
  	gtk_box_pack_end (GTK_BOX (vbox), buttons_hbox, FALSE, FALSE, 5);

	WallpaperState.make_xml_button = create_list_button( buttons_hbox, GTK_STOCK_PROPERTIES, G_CALLBACK(on_make_xml_clicked) );
	gtk_button_set_label( GTK_BUTTON(WallpaperState.make_xml_button), "Tweak this wallpaper" );
	gtk_widget_set_size_request (WallpaperState.make_xml_button, WallpaperState.preview_width, -1);	
#if (GTK_MAJOR_VERSION>=2) && (GTK_MINOR_VERSION>=6)	
	gtk_button_set_image( GTK_BUTTON(WallpaperState.make_xml_button),gtk_image_new_from_stock(GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_BUTTON) );
#endif
	colorize_gtk_widget( GTK_WIDGET(WallpaperState.make_xml_button), get_colorschemed_style_button() );
}

void 
on_private_backs_changed()
{
	if( !get_flags( WallpaperState.flags, DISPLAY_SYSTEM_BACKS ) )
	{	
    	GtkTreeIter iter;
		ASImageListEntry *curr ;
	
		curr = WallpaperState.private_backs_list ;
		while( curr )
		{
			if( curr->preview ) 
			{	
        		gtk_list_store_append (GTK_LIST_STORE (WallpaperState.list_model), &iter);
   	    		gtk_list_store_set (GTK_LIST_STORE (WallpaperState.list_model), &iter, 0, curr->name, 1, curr, -1);
				curr = curr->next ;
			}
		}
	}
}

void
reload_private_backs_list()
{	
	char *private_back_dir = PutHome("~/.afterstep/backgrounds");
	ASImageListEntry *old = WallpaperState.private_backs_list ;
	WallpaperState.private_backs_list = get_asimage_list( Scr.asv, private_back_dir,
	              						       LOAD_PREVIEW, Scr.image_manager->gamma, 0, 0,
											   0, &WallpaperState.private_backs_count, ignore_dots );
	free( private_back_dir );

	on_private_backs_changed();

	if( old )
		destroy_asimage_list( &old );
}

void 
init_ASWallpaper()
{
	memset( &WallpaperState, 0x00, sizeof(ASWallpaperState));
	
	create_main_window(); 
	create_backs_list();
	create_list_preview();

	reload_private_backs_list();

	g_signal_connect (G_OBJECT (WallpaperState.main_window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
  	gtk_widget_show (WallpaperState.main_window);
}	 

GtkWidget*
create_filechooserdialog2 (void)
{
  GtkWidget *filechooserdialog2;
  GtkWidget *dialog_vbox2;
  GtkWidget *dialog_action_area2;
  GtkWidget *button_file_cancel;
  GtkWidget *button_file_open;

  filechooserdialog2 = gtk_file_chooser_dialog_new ("", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL);
  gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog2), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox2 = GTK_DIALOG (filechooserdialog2)->vbox;
  gtk_widget_show (dialog_vbox2);

  dialog_action_area2 = GTK_DIALOG (filechooserdialog2)->action_area;
  gtk_widget_show (dialog_action_area2);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area2), GTK_BUTTONBOX_END);

  button_file_cancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (button_file_cancel);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog2), button_file_cancel, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (button_file_cancel, GTK_CAN_DEFAULT);

  button_file_open = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (button_file_open);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog2), button_file_open, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (button_file_open, GTK_CAN_DEFAULT);

  g_signal_connect_swapped (G_OBJECT (button_file_cancel),
	                        "clicked", G_CALLBACK (gtk_widget_destroy),
		 			        G_OBJECT (filechooserdialog2));

  g_signal_connect ((gpointer) button_file_open, "clicked",
                    G_CALLBACK (on_file_open_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog2, filechooserdialog2, "filechooserdialog2");
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog2, dialog_vbox2, "dialog_vbox2");
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog2, dialog_action_area2, "dialog_action_area2");
  GLADE_HOOKUP_OBJECT (filechooserdialog2, button_file_cancel, "button_file_cancel");
  GLADE_HOOKUP_OBJECT (filechooserdialog2, button_file_open, "button_file_open");

  gtk_widget_grab_default (button_file_open);
  return filechooserdialog2;
}

