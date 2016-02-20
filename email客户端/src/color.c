#include "color.h"

extern GtkWidget *Send_textview;
GtkWidget *colorseldlg = NULL;
GtkWidget *drawingarea = NULL;
GdkColor color;
GdkColor ncolor;
GtkColorSelection *colorsel;

/* Color changed handler */
static void color_changed_cb( GtkWidget  *widget, gpointer data)
{
	gtk_color_selection_get_current_color (colorsel, &ncolor);
//	g_print("color1 is %d,%d,%d\n",ncolor.red,ncolor.green,ncolor.blue);
	gtk_widget_modify_text(Send_textview, GTK_STATE_NORMAL, &ncolor); 
}

void area_event(GtkWidget* widget, gpointer data)
{
	gint response;

	if (colorseldlg == NULL)
			colorseldlg = gtk_color_selection_dialog_new ("Select background color");
	colorsel = GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (colorseldlg)->colorsel);

	gtk_color_selection_set_has_palette (colorsel, TRUE);

	g_signal_connect (colorsel, "color_changed", G_CALLBACK (color_changed_cb), NULL);

	response = gtk_dialog_run (GTK_DIALOG (colorseldlg));

	if (response == GTK_RESPONSE_OK)
		gtk_color_selection_get_current_color (colorsel, &color);		
	else 
		gtk_widget_modify_bg (drawingarea, GTK_STATE_NORMAL, &color);
	
	gtk_widget_hide (colorseldlg);
}
