#include "isQuit1.h"

void on_yes_clicked1(GtkWidget * widget,gpointer data){
//	g_print("yes is clicked\n");
	gtk_widget_destroy((GtkWidget *)data);
}

void on_no_clicked1(GtkWidget * widget,gpointer data){
//	g_print("no is clicked\n");
}

void isQuit1(GtkWidget *widget,gpointer data){

	GtkWidget* window;
	GtkWidget* vbox;
	GtkWidget* hbox;
	GtkWidget* bbox;
	GtkWidget* button;
	GtkWidget* label;
	GtkWidget* image;
	GtkWidget* sep;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "确定要退出？");
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);			
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);

	vbox = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);

	image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING,GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 5);

	label = gtk_label_new("确定要退出？");
	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 5);

	sep = gtk_hseparator_new();
	gtk_box_pack_start (GTK_BOX (hbox), sep, FALSE, FALSE, 5);
	
	bbox = gtk_hbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_SPREAD);
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);	
	
	button = gtk_button_new_from_stock(GTK_STOCK_YES);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_yes_clicked1), widget);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);	
	
	button = gtk_button_new_from_stock(GTK_STOCK_NO);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_no_clicked1), NULL);
	g_signal_connect_swapped(G_OBJECT(button),"clicked",G_CALLBACK(gtk_widget_destroy),window);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,0);

	gtk_widget_show_all(window);
//	gtk_main();
	
}
