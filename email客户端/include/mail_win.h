#ifndef _MAIL_WIN_H_
#define _MAIL_WIN_H_


#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "new_mail.h"
#include "about.h"

GtkWidget* create_tree();
void insert_text(GtkTextBuffer* buffer, int num);
void insert_list_text(char* str);
void get_main_menu(GtkWidget *window, GtkWidget **menubar);
void create_mail_window(gpointer* data);
void test_button_clicked(GtkWidget *button,gpointer data);

#endif
