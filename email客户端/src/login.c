#include<gtk/gtk.h>
#include<string.h>
#include<stdio.h>
#include"reg.h"
#include "error.h"
#include "mail_win.h"

int total=0;
char name[20] = {0};

typedef struct e_rec
{
	char topic[50];
	char kind[10];
	char sender[20];
	char reciever[200];
	char time[20];
	char textnum[20];
	char info[50];
}E_rec;
E_rec pioe[200]; //part information of email
char bigbuf[4096];
GtkWidget* entry_usrname;
GtkWidget* entry_pwd;

gboolean entry_focus_in_handler(GtkWidget* widget, GdkEventFocus* event, gpointer data)
{
	gtk_editable_select_region(GTK_EDITABLE(widget), 0, 3);
	return FALSE;
}

void on_login_clicked(GtkWidget* widget,gpointer* data)
{	
	const char* entry_usrname_text;
	const char* entry_pwd_text;
	entry_usrname_text = gtk_entry_get_text(GTK_ENTRY(entry_usrname));
	entry_pwd_text = gtk_entry_get_text(GTK_ENTRY(entry_pwd));

	if(strlen(entry_usrname_text) > 0 && strlen(entry_pwd_text) > 0)
	{
		g_printf("usrname: %s\n", entry_usrname_text);

                strcpy(name,entry_usrname_text);///

		g_printf("pwd: %s\n", entry_pwd_text);
                int flag=0;
		clientlogin(entry_usrname_text,entry_pwd_text,&flag);//socket
                 //sockfd
		if(flag == -2)
			create_error_window(4);
		//数据库 判断
		else if(flag == -3)
			create_error_window(5);
		//mail window
		else{
			char box[10]="recv";//receive 
			hostpage(box);         
			create_mail_window(data);
			gtk_widget_destroy(widget);
		}
	}	
	else
	{
		create_error_window(1);
	}
}

void widget_set_font(GtkWidget *widget , int font_size , char* font_script)
{
	PangoFontDescription *font ;
	int fontSize = font_size ;

	font = pango_font_description_from_string(font_script) ;//"Sans"字体名 
	pango_font_description_set_size(font , fontSize * PANGO_SCALE) ;//设置字体大小 
	gtk_widget_modify_font(GTK_WIDGET(widget) , font) ;//设置label的字体， 这样这个GtkButton上面显示的字体就变了 
	
}

int main(int argc, char* argv[])
{
	GtkWidget* window;
	GtkWidget* box;
	GtkWidget* box_main;
	GtkWidget* box_space;
	GtkWidget* box_usrname;
	GtkWidget* box_pwd;
	GtkWidget* box_button;
	GtkWidget* label_usrname;
	GtkWidget* label_pwd;
	GtkWidget* button;
	
	//gtk init
	gtk_init(&argc, &argv);

	gtk_rc_parse("./res/bg");

	//set window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Log in");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	//set bg image	
	GtkWidget * fixed = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), fixed);
	GtkWidget * image = gtk_image_new_from_file("./res/bg.png");
	gtk_fixed_put(GTK_FIXED(fixed), image,0,0);

	//set box
	box_main = gtk_vbox_new(FALSE, 0);

	box_space = gtk_vbox_new(FALSE, 0);	
	gtk_box_pack_start(GTK_BOX(box_main), box_space, TRUE, TRUE, 0);
	gtk_widget_set_size_request(GTK_WIDGET (box_space), 300, 140);

	box = gtk_vbox_new(FALSE, 30);
	gtk_container_set_border_width(GTK_CONTAINER(box), 25);

	box_usrname = gtk_hbox_new(FALSE, 0);
	box_pwd = gtk_hbox_new(FALSE, 0);

	//set label
	label_usrname = gtk_label_new("User name : ");
	gtk_box_pack_start(GTK_BOX(box_usrname), label_usrname, TRUE, TRUE, 0);

	label_pwd = gtk_label_new("Password :  ");
	gtk_box_pack_start(GTK_BOX(box_pwd), label_pwd, TRUE, TRUE, 0);

	//set entry
	entry_usrname = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_usrname), 16);
	g_signal_connect(G_OBJECT(entry_usrname), "focus_in_event", G_CALLBACK(entry_focus_in_handler), NULL);
	gtk_box_pack_start(GTK_BOX(box_usrname), entry_usrname, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), box_usrname, TRUE, TRUE, 0);

	entry_pwd = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_pwd), 16);
	gtk_entry_set_visibility(GTK_ENTRY(entry_pwd), FALSE);
	g_signal_connect(G_OBJECT(entry_pwd), "focus_in_event", G_CALLBACK(entry_focus_in_handler), NULL);
	g_signal_connect(G_OBJECT(entry_pwd), "activate", G_CALLBACK(on_login_clicked), window);
	gtk_box_pack_start(GTK_BOX(box_pwd), entry_pwd, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), box_pwd, TRUE, TRUE, 0);

	//set button
	button = gtk_button_new_with_label("Log in");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(on_login_clicked), window);
	gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

	button = gtk_button_new_with_label("Register");
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(show_regis), (gpointer*)2);
	gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

	//pack and container
	gtk_box_pack_start(GTK_BOX(box_main), box, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(fixed), box_main);

	//widget show
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
