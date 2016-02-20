#include"jjy_text_view.h"

GtkTextBuffer *text_buffer;
GtkWidget *view;

GtkWidget* jjy_create_text(int width, int height)
{
	GtkWidget *scrolled_window;
	

	view = gtk_text_view_new ();
	text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view),GTK_WRAP_CHAR);
	gtk_text_view_set_indent (GTK_TEXT_VIEW(view), 30);

	gtk_widget_set_size_request (GTK_WIDGET (scrolled_window), width, height);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);
	gtk_widget_show_all (scrolled_window);

	return scrolled_window;
}
