#include"error.h"

void create_error_window( int data)
{
	GtkWidget* window;
	GtkWidget* button;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "HINT");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroy), NULL);

	if (data == 1){
		button = gtk_button_new_with_label("请输入用户名或密码");
	}
	else if (data == 2){
		button = gtk_button_new_with_label("请看好注册要求");
	}
	else if (data == 3){
		button = gtk_button_new_with_label("用户名已存在");
	}
	else if (data == 4){
		button = gtk_button_new_with_label("密码错误");
	}
	else if (data == 5){
		button = gtk_button_new_with_label("用户名不存在");
	}
	else if (data == 6){
		button = gtk_button_new_with_label("收件人不为空");
	}
	else if (data == 7){
		button = gtk_button_new_with_label("send succeed");
	}
	else if (data == 8){
		button = gtk_button_new_with_label("you have  new email");
	}
	g_signal_connect_swapped(G_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(window), button);
	gtk_widget_show(button);

	gtk_widget_show(window);
}
