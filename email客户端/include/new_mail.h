#ifndef _NEW_MAIL_H_
#define _NEW_MAIL_H_

#include <strings.h>
#include "mail_win.h"
#include "attachment.h"
#include "about.h"

void insert_text1();
GtkWidget* create_contact_tree();
void new_mail(GtkWidget *widget,gpointer data);

#endif
