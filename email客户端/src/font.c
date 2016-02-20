#include "font.h"
#include "text_view.h"

extern GtkWidget *Send_textview;
PangoFontDescription* chosen_font;
char *s;

void on_font_select_ok(GtkWidget *button,GtkFontSelectionDialog *fs)
{

	s = gtk_font_selection_dialog_get_font_name(fs);
	chosen_font = pango_font_description_from_string(s);
	gtk_widget_modify_font(Send_textview, chosen_font); 
	pango_font_description_free(chosen_font);   

	gtk_widget_destroy(GTK_WIDGET(fs));
}

void on_font_show()
{
	GtkWidget *dialog;
	dialog = gtk_font_selection_dialog_new("select a font :");
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_signal_connect(GTK_OBJECT(dialog),"destroy",GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	gtk_signal_connect(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(dialog)->ok_button),"clicked",GTK_SIGNAL_FUNC(on_font_select_ok),GTK_FONT_SELECTION_DIALOG(dialog));
	g_signal_connect_swapped(G_OBJECT(GTK_FONT_SELECTION_DIALOG(dialog)->cancel_button),"clicked",GTK_SIGNAL_FUNC(gtk_widget_destroy),GTK_OBJECT(dialog));
	gtk_widget_show(dialog);
}
/*
void font_load(GtkWidget *button)
{
	//s first

	chosen_font = pango_font_description_from_string(s);
	gtk_widget_modify_font(Send_textview, chosen_font); 
	pango_font_description_free(chosen_font);   

	gtk_widget_destroy(GTK_WIDGET(fs));
}*/
