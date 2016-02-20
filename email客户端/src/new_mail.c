#include "new_mail.h"
#include "isQuit1.h"
#include "text_view.h"
#include "mail_win.h"
#include "font.h"
#include "color.h"

extern GtkWidget *Send_textview;
extern GtkTextBuffer *Send_buffer;
GtkWidget* entry_hiden_rec;
GtkWidget* entry_rec;
GtkWidget* entry_topic;
GtkWidget *entry_search;
GtkWidget* entry_srch_ppl;
GtkWidget* entry_chao_rec;
GtkTreeSelection* select1;
GtkTreeStore* tree_store;
GtkWidget* window;
GtkTreeIter tree_iter;
GtkWidget* tree;
char* contact_name;

enum TreeCols
{
	TREE_NAME,
	TREE_CNT
};


static void add_activated (GtkMenuItem *item,GtkWidget *button)
{
	g_print("add\n");
}

static void del_activated (GtkMenuItem *item,GtkWidget *button)
{
	gtk_tree_store_remove ((GtkTreeStore*) tree_store, &tree_iter);
	g_print("delete : %s\n", contact_name);
}

static void create_popup_menu (GtkWidget *menu)
{
	GtkWidget *add, *del,*separator;

	add = gtk_menu_item_new_with_label ("add");
	del = gtk_menu_item_new_with_label ("del");

	separator = gtk_separator_menu_item_new ();

	g_signal_connect (G_OBJECT (add), "activate",
		                  G_CALLBACK (add_activated),NULL);
	g_signal_connect (G_OBJECT (del), "activate",
		                  G_CALLBACK (del_activated),NULL);

	gtk_menu_shell_append (GTK_MENU_SHELL (menu), add);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), separator);
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), del);

	gtk_widget_show_all (menu);
}
void onButtonPressed(GtkWidget *widget,GdkEvent *event)
{
    	GdkEventButton *event_button;
    	GtkTreeModel *model;
//    	GtkTreeIter iter;
    	GtkTreePath *path = NULL;
    	GtkTreePath *cursor_path;
    	GtkTreeViewColumn *col;
    	int cell_x,cell_y; 
	GtkWidget* widget1 = NULL;
	GtkWidget *menu;

    if(event->type == GDK_BUTTON_PRESS)
    {
        event_button=(GdkEventButton *)event;

	int x = (gint)event_button->x;
	int y = (gint)event_button->y;
	gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree),x,y,&path,&col,&cell_x,&cell_y);

	if(path)
	{
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(tree),path,NULL,FALSE);
		model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
		gtk_tree_model_get_iter(model,&tree_iter,path);
		contact_name = NULL;
		gtk_tree_model_get(model, &tree_iter, TREE_NAME, &contact_name, -1);  
		widget = gtk_window_get_focus((GtkWindow *)window);
	}
  
	gtk_tree_path_free(path);

	if(contact_name)
	{
		if(event_button->button == 3)//right
		{
	//            	gtk_menu_popup(menu,NULL,NULL,NULL,NULL,event_button->button,event_button->time);
			menu = gtk_menu_new ();	
			create_popup_menu (menu);
			gtk_menu_popup(GTK_MENU(menu),NULL,NULL,NULL,NULL,event_button->button,event_button->time);
		
		}
		else if(event_button->button == 1)//left
		{
			if((widget1 = gtk_window_get_focus((GtkWindow *)window)))
			{
				gtk_entry_append_text((GtkEntry *)widget1,contact_name);
				gtk_entry_append_text((GtkEntry *)widget1," ; ");
			}
		}
	}
    }
}

GtkWidget* create_contact_tree()
{
	GtkWidget *scrolled_window;
	GtkTreeIter iter_p;
	GtkTreeIter iter;
//	GtkTreeStore* tree_store;
	GtkCellRenderer* renderer;
	GtkTreeViewColumn* column;


	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (GTK_WIDGET (scrolled_window), 200, 400);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	tree_store = gtk_tree_store_new(TREE_CNT, G_TYPE_STRING);

	gtk_tree_store_append(GTK_TREE_STORE(tree_store), &iter_p, NULL);
	gtk_tree_store_set(GTK_TREE_STORE(tree_store), &iter_p, TREE_NAME, "Computer", -1);

	gtk_tree_store_append(GTK_TREE_STORE(tree_store), &iter, &iter_p);
	gtk_tree_store_set(GTK_TREE_STORE(tree_store), &iter, TREE_NAME, "Receive Box", -1);

	gtk_tree_store_append(GTK_TREE_STORE(tree_store), &iter, &iter_p);
	gtk_tree_store_set(GTK_TREE_STORE(tree_store), &iter, TREE_NAME, "Send Box", -1);

	gtk_tree_store_append(GTK_TREE_STORE(tree_store), &iter, &iter_p);
	gtk_tree_store_set(GTK_TREE_STORE(tree_store), &iter, TREE_NAME, "Draft Box", -1);

	gtk_tree_store_append(GTK_TREE_STORE(tree_store), &iter, &iter_p);
	gtk_tree_store_set(GTK_TREE_STORE(tree_store), &iter, TREE_NAME, "Trash Box", -1);

	tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));

	g_object_unref(tree_store);
	
	renderer = gtk_cell_renderer_text_new();
	column  = gtk_tree_view_column_new_with_attributes("Contact", renderer, "text", TREE_NAME, NULL);
	gtk_tree_view_append_column((GtkTreeView *)tree, (GtkTreeViewColumn *)column);
	gtk_tree_view_expand_all((GtkTreeView *)tree);

	//set search_bar
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW(tree),TRUE);
	gtk_tree_view_set_search_entry (GTK_TREE_VIEW(tree),(GtkEntry *)entry_srch_ppl);

	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(tree));
	
	//set select
	select1 = gtk_tree_view_get_selection(GTK_TREE_VIEW (tree));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(select1), GTK_SELECTION_SINGLE);
	gtk_tree_view_get_model(GTK_TREE_VIEW(tree));

	g_signal_connect(GTK_TREE_VIEW(tree),"button_press_event",(GCallback)onButtonPressed,NULL);

	gtk_widget_show_all (scrolled_window);

	return scrolled_window;
}

void insert_text1()
{
	GtkTextIter iter;

	gtk_text_buffer_get_iter_at_offset(Send_buffer, &iter, 0);
	
	gtk_text_buffer_insert(Send_buffer, &iter, 
		"From : jjy@bit.edu.cn\n"
		"To : pig@qq.com\n"
		"Subject : made it!\n"
		"\n"
		"you are a pig!\n"
		, -1);
}

GtkWidget* create_toolbar1()
{
	GtkWidget* toolbar;
	toolbar = gtk_toolbar_new();

	GtkWidget *iconw = gtk_image_new_from_file ("./res/11.png"); 
	GtkWidget *button_new = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "new",  "new", "Private", iconw,  GTK_SIGNAL_FUNC (new_mail),  "new");
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/12.png"); 
	GtkWidget * button_attach = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "attach",  "attachment", "Private", iconw,  GTK_SIGNAL_FUNC (add_attachment),  "attach");
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/13.png"); 
	GtkWidget * button_save = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "save",  "save", "Private", iconw,  GTK_SIGNAL_FUNC (save_content),  (gpointer)(window));
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/14.png"); 
	GtkWidget * button_cut = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "cut",  "cut", "Private", iconw,  GTK_SIGNAL_FUNC(c_c_p),(gpointer)(1));
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/15.png"); 
	GtkWidget * button_copy = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "copy",  "copy", "Private", iconw,  GTK_SIGNAL_FUNC(c_c_p),(gpointer)(2));
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/16.png"); 
	GtkWidget * button_paste = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "paste",  "paste", "Private", iconw,  GTK_SIGNAL_FUNC(c_c_p),(gpointer)(3));
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
	
	return toolbar;
}
GtkWidget* create_toolbar2()
{
	GtkWidget* toolbar;
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_DND);

	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_SELECT_COLOR
,"color","color",GTK_SIGNAL_FUNC(area_event),NULL,-1);	
	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));	
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_BOLD,"font","font",GTK_SIGNAL_FUNC(on_font_show),NULL,-1);
	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));	
//	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_SAVE,"s","save",GTK_SIGNAL_FUNC(save_content),(gpointer)(window),-1);
//	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));	

	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
	
	return toolbar;
}


GtkWidget *cre_button_box(GtkWidget *window)
{	
    	GtkWidget *Button_Box,*SaveButton,*CloseButton;
	//set send_button
    	SaveButton = gtk_button_new_with_label("Send");
    	g_signal_connect(G_OBJECT(SaveButton),"clicked",G_CALLBACK(get_content),(gpointer)window);

    	//set close_button
    	CloseButton = gtk_button_new_with_label("Close");
    	g_signal_connect_swapped(G_OBJECT(CloseButton),"clicked",G_CALLBACK(on_close),(gpointer)window);

    	//set Button_Box 
    	Button_Box = gtk_hbutton_box_new();
    	gtk_box_set_spacing(GTK_BOX(Button_Box),1);
    	gtk_button_box_set_layout(GTK_BUTTON_BOX(Button_Box),GTK_BUTTONBOX_END);
    	gtk_container_set_border_width(GTK_CONTAINER(Button_Box),5);

    	gtk_box_pack_start(GTK_BOX(Button_Box),CloseButton,TRUE,TRUE,5);
    	gtk_box_pack_start(GTK_BOX(Button_Box),SaveButton,TRUE,TRUE,5);

	return Button_Box;

}

void new_mail(GtkWidget *widget,gpointer data)
{
	GtkWidget* box_main;
	GtkWidget* box_parent;
	GtkWidget* box_toolbar;
	GtkWidget* toolbar;
	GtkWidget* toolbar1;
	GtkWidget* box_edit;
	GtkWidget* box_rec;
	GtkWidget* box_topic;
	GtkWidget* box_hiden;
	GtkWidget* box_chao;
	GtkWidget* box_contact;
	GtkWidget *button_search;
	GtkWidget *button_reply;
	GtkWidget *button_resend;
	GtkWidget *button_del;
	GtkWidget *label_rec;
	GtkWidget *label_topic;
	GtkWidget *label_hiden_rec;
	GtkWidget *label_chao_rec;
	GtkWidget *text_view;
	GtkWidget *Button_Box;
	GtkWidget *Button_rec;
	GtkWidget *Button_hiden_rec;
	GtkWidget *Button_topic;
	GtkWidget *tree;
	GtkWidget *menubar;

	//set window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "New_Mail");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(isQuit1), NULL);

	//set box_main
	box_main = gtk_vbox_new (FALSE, 10);
	gtk_container_add (GTK_CONTAINER (window), box_main);

	//set menubar
	get_main_menu (window, &menubar);
	gtk_box_pack_start(GTK_BOX(box_main),menubar,FALSE,TRUE,0);

	//set box_toolbar
  	box_toolbar = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_main),box_toolbar,FALSE,TRUE,0);

	//set toolbar
	toolbar = create_toolbar1();
	gtk_box_pack_start(GTK_BOX(box_toolbar),toolbar,TRUE,TRUE,0);

	//box_parent
	box_parent = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start(GTK_BOX(box_main),box_parent,TRUE,TRUE,0);

	//set box_rec box_topic box_hiden box_edit
	box_edit = gtk_vbox_new (FALSE, 10);
	gtk_box_pack_start(GTK_BOX(box_parent),box_edit,TRUE,TRUE,0);

	box_rec = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_edit),box_rec,FALSE,TRUE,0);
	
	box_hiden = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_edit),box_hiden,FALSE,TRUE,0);

	box_chao = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_edit),box_chao,FALSE,TRUE,0);

	box_topic = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box_edit),box_topic,FALSE,TRUE,0);

	//set inside label
	label_rec = gtk_label_new("    Receiver(T)     :      ");
	gtk_box_pack_start(GTK_BOX(box_rec), label_rec, FALSE, TRUE, 0);
	gtk_widget_show(label_rec);

	label_hiden_rec = gtk_label_new("   Blind Copy (B)    :    ");
	gtk_box_pack_start(GTK_BOX(box_hiden), label_hiden_rec, FALSE, TRUE, 0);
	gtk_widget_show(label_hiden_rec);

	label_chao_rec = gtk_label_new("  Carbon Copy(C)  :    ");
	gtk_box_pack_start(GTK_BOX(box_chao), label_chao_rec, FALSE, TRUE, 0);
	gtk_widget_show(label_chao_rec);

	label_topic = gtk_label_new("         Topic (T)    :       ");
	gtk_box_pack_start(GTK_BOX(box_topic), label_topic, FALSE, TRUE, 0);
	gtk_widget_show(label_topic);

	//set inside entry
	entry_rec = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_rec), 100);
	gtk_box_pack_start(GTK_BOX(box_rec), entry_rec, TRUE, TRUE, 0);

	entry_hiden_rec = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_hiden_rec), 100);
	gtk_box_pack_start(GTK_BOX(box_hiden), entry_hiden_rec, TRUE, TRUE, 0);

	entry_chao_rec = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_chao_rec), 100);
	gtk_box_pack_start(GTK_BOX(box_chao), entry_chao_rec, TRUE, TRUE, 0);

	entry_topic = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry_topic), 100);
	gtk_box_pack_start(GTK_BOX(box_topic), entry_topic, TRUE, TRUE, 0);
	
	if((int)data == 1);//new
	else if((int)data == 2)//reply		
	{
		gtk_entry_set_text(GTK_ENTRY(entry_rec),"haha1");
		gtk_entry_set_text(GTK_ENTRY(entry_hiden_rec),"haha2");
		gtk_entry_set_text(GTK_ENTRY(entry_chao_rec),"haha3");
		gtk_entry_set_text(GTK_ENTRY(entry_topic),"haha4");
	}
	else if((int)data == 3)//resend
	{
		gtk_entry_set_text(GTK_ENTRY(entry_topic),"haha5");
	}

	toolbar1 = create_toolbar2();
	gtk_box_pack_start(GTK_BOX(box_edit),toolbar1,TRUE,TRUE,0);

	//set text_view
	text_view = create_text(window,800, 340);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(Send_textview),TRUE);
	gtk_box_pack_start(GTK_BOX(box_edit), text_view, TRUE, TRUE, 0);

	if((int)data == 2 || (int)data == 3)
	{	
		g_print("aa\n");	
		gtk_text_buffer_set_text((GtkTextBuffer*)Send_buffer, "", 0);
		insert_text1();
		g_print("bb\n");
		
	}

	//set button_box
	Button_Box = cre_button_box(window);
    	gtk_box_pack_start(GTK_BOX(box_edit), Button_Box, FALSE, TRUE, 0);
	
	//set box contact
	box_contact = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_parent), box_contact, FALSE, TRUE, 0);	
	
	//set tree
	entry_srch_ppl = gtk_entry_new();
	gtk_box_pack_start (GTK_BOX (box_contact), entry_srch_ppl, FALSE, FALSE, 0);

	tree = create_contact_tree();
	gtk_box_pack_start (GTK_BOX (box_contact), tree, TRUE, TRUE, 0);

	//show new_mail window
	gtk_widget_show_all(window);

//	gtk_main();
}
