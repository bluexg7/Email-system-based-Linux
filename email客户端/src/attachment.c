#include "attachment.h"

void file_ok_sel( GtkWidget  *w, GtkFileSelection *fs )
{
   	g_print ("%s!!\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
   	gtk_widget_destroy((GtkWidget*)fs);
}

void add_attachment(){
	GtkWidget *filew;
	filew = gtk_file_selection_new ("add");
	g_signal_connect (GTK_FILE_SELECTION (filew)->ok_button,"clicked", G_CALLBACK (file_ok_sel), (gpointer) filew);
    	g_print ("%s!!\n", gtk_file_selection_get_filename (GTK_FILE_SELECTION (filew)));
    
        /* Connect the cancel_button to destroy the widget */
        g_signal_connect_swapped (GTK_FILE_SELECTION (filew)->cancel_button,"clicked", G_CALLBACK (gtk_widget_destroy), filew);
    
    	/* Lets set the filename, as if this were a save dialog, and we are giving
     	a default filename */
    	gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "penguin.png");    
    	gtk_widget_show (filew);
}
