#ifndef __TEXT_VIEW_H__
#define __TEXT_VIEW_H__

#include<gtk/gtk.h>
#include<stdlib.h>
#include "new_mail.h"
#include "color.h"

void Clear_Local_message();
void Put_Local_message(const gchar *text);
GtkWidget * create_text(GtkWidget* window, int w, int h);
void on_close(GtkWidget *CloseButton,gpointer data);
void get_content(GtkButton *SaveButton, GtkWidget *Send_textview);
void save_content(GtkButton *SaveButton, GtkWidget *Send_textview);
void c_c_p(GtkWidget *Send_textview,gpointer data);

#endif
