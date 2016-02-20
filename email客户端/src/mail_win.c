#include"mail_win.h"
#include "color.h"
#include "text_view.h"
#include "font.h"
#include "gettext.h"
#include "jjy_text_view.h"
#include "gettext.h"
extern int total;
extern char name[20];
typedef struct e_rec
{
	char topic[50];
	char kind[10];//
	char sender[20];
	char reciever[200];
	char time[20];
	char textnum[20];
	char info[50];//numid
}E_rec;

extern E_rec pioe[200]; //part information of email 
extern char bigbuf[4096];
GtkListStore* list_store;
GtkTreeIter list_iter;
GtkTreeModel* model;
GtkTreeModel* tree_model;
GtkWidget* text; 
GtkTreeSelection* list_select;
GdkColor text_color;
extern GtkWidget* view;
int max_mail;
extern PangoFontDescription* chosen_font;
extern GtkTextBuffer *text_buffer;
extern GtkWidget *view;
int list_last = -1;
int isMulChoice = 0;

enum ListCols
{
	LIST_CHECKED,
	LIST_READ,
	LIST_NUM,
	LIST_NAME,
	LIST_MESSAGE,
	LIST_DATE,
	LIST_CNT
};

enum TreeCols
{
	TREE_NAME,
	TREE_CNT
};

void test_button_clicked(GtkWidget *button,gpointer data)
{
	g_print("yes is clicked\n");
}

static GtkItemFactoryEntry menu_items[] = { 
	{ "/_File",         NULL,         NULL, 0, "<Branch>" },
	{ "/File/_New",     "<control>N", new_mail, 0, NULL },
	{ "/File/sep1",     NULL,         NULL, 0, "<Separator>" },
	{ "/File/Quit",     "<control>Q", gtk_main_quit, 0, NULL },
	{ "/_Edit",      NULL,         NULL, 0, "<Branch>" },
	{ "/Edit/Color",  "F1",         area_event, 0, NULL },
	{ "/Edit/Font",  "F2",        on_font_show, 0, NULL },
	{ "/_Help",         NULL,         NULL, 0, "<Branch>" },
	{ "/_Help/About",   "F3",         create_about, 0, NULL },
};

void get_main_menu(GtkWidget *window, GtkWidget **menubar)
{
	GtkItemFactory *item_factory;
	GtkAccelGroup *accel_group;

	gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
	accel_group = gtk_accel_group_new ();
	item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);
	gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);
	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

	if (menubar)  
		*menubar = gtk_item_factory_get_widget (item_factory, "<main>");
}

void select_deleted(GtkWidget* button,gpointer data)
{
	isMulChoice = 1;
	toggle_button_callback(NULL, NULL);
}

void on_button_ok(GtkWidget* button,gpointer data)
{
	isMulChoice = 0;
	g_print("delete start\n");
	GtkTreeIter iter;
	gboolean valid;
	
	valid = gtk_tree_model_get_iter_first (model, &iter);
	
	gint row_count = 1;

	while(valid)
	{
		char* num;
		char *str_data1;
		char *str_data2;
		char *str_data3;
     		gboolean   int_data;

		gtk_tree_model_get (model, &iter, 
			LIST_CHECKED, &int_data,
			LIST_NUM, &num,
			LIST_NAME,&str_data1,
			LIST_MESSAGE,&str_data2,
			LIST_DATE, &str_data3,-1);

		if (int_data == 1 && row_count != max_mail)
		{
			g_print("row = %d\n",row_count);
			g_print("max_mail = %d\n",max_mail);
			g_print ("Row %d: (%d,%s, %s, %s, %s)\n", row_count, int_data, num, str_data1, str_data2, str_data3);
			

			char task[10]="dele";
			changemail(pioe[atoi(num)-1].info ,task,pioe[atoi(num)-1].kind);
			g_free (num);
      			g_free (str_data1);
			g_free (str_data2);
			g_free (str_data3);
			
			

			gtk_list_store_remove (GTK_LIST_STORE(list_store), &iter);
			
			
			max_mail--;
		}
		else if (int_data == 1 && row_count == max_mail)
		{
			g_print("row = %d\n",row_count); 
			g_print("max_mail = %d\n",max_mail);
			g_print ("Row %d: (%d,%s, %s, %s, %s)\n", row_count, int_data, num, str_data1, str_data2, str_data3);
			char task[10]="dele";
			changemail(pioe[atoi(num)-1].info ,task,pioe[atoi(num)-1].kind);
			g_free (num);
      			g_free (str_data1);
			g_free (str_data2);
			g_free (str_data3);
                        printf("name=%s,num=%s",name,num);
			//char delemail[10]="delete";
                       //changemail(name,num,delemail);
			

			gtk_list_store_remove (GTK_LIST_STORE(list_store), &iter);
			max_mail--;
			break;
		}
		else
		{
			row_count ++;
			valid = gtk_tree_model_iter_next (model, &iter);
		}
	}
}


void on_button_select (GtkWidget *button,gpointer data)
{
	GtkTreeIter iter;
	gboolean valid;
	
	valid = gtk_tree_model_get_iter_first (model, &iter);
	
	gint row_count = 1;

	while(valid)
	{
		g_print("valid is %d\n",valid);
		gchar *str_data1;
		gchar *str_data2;
		gchar *str_data3;
     		gboolean   int_data;

		gtk_tree_model_get (model, &iter, 
		LIST_CHECKED, &int_data,
		LIST_NAME,&str_data1,
		LIST_MESSAGE,&str_data2,
		LIST_DATE, &str_data3,-1);

		g_print("row = %d\n",row_count);
		g_print("max_mail = %d\n",max_mail);
		g_print ("Row %d: (%d,%s, %s, %s)\n", row_count, int_data, str_data1, str_data2, str_data3);

		g_free (str_data1);
		g_free (str_data2);
		g_free (str_data3);	

		row_count ++;
		max_mail = max_mail > row_count ? max_mail : row_count ;
		
		if((int)data == 1)
			gtk_list_store_set (GTK_LIST_STORE(list_store), &iter, 0, 1, -1);
		else if((int)data == 2)
			gtk_list_store_set (GTK_LIST_STORE(list_store), &iter, 0, 0, -1);

		valid = gtk_tree_model_iter_next (model, &iter);
	}	
	g_print("select all\n");	
	max_mail --;
}

void insert_text(GtkTextBuffer* buffer, int num)
{      
       
	GtkTextIter iter;
        int text_id;
        text_id=num-49;
	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);      
	gettext(pioe[text_id].textnum);    
	char prit[4096] = {0};
        int i=0,j=0;
        /*char text_style[50]={0};
        char getred[15]={0};
        char getgreen[15]={0};
	char getblue[15]={0};
      
        for(i=0;i<strlen(bigbuf);i++)
        { 
          if(bigbuf[i]!=';')
             text_style[i]=bigbuf[i];
          else
          {text_style[i]='\0';j=i;break;}
        }
    printf("text=%s\n",text_style);
        for(i=j+1;i<strlen(bigbuf);i++)
        { 
          if(bigbuf[i]!=';')
             getred[i]=bigbuf[i];
          else
          {getred[i]='\0';j=i;break;}
        }
printf("text=%s\n",getred);
        for(i=j+1;i<strlen(bigbuf);i++)
        { 
          if(bigbuf[i]!=';')
             getgreen[i]=bigbuf[i];
          else
          {getgreen[i]='\0';j=i;break;}
        }
  printf("text=%s\n",getgreen);
	for(i=j+1;i<strlen(bigbuf);i++)
        { 
          if(bigbuf[i]!='#')
             getblue[i]=bigbuf[i];
          else
          {getblue[i]='\0';j=i;break;}
        }
printf("text=%s\n",getblue);
        text_color.red =atoi(getred);
	text_color.green =atoi(getgreen);
	text_color.blue =atoi(getblue);
*/
        char outtext[5000]={0};
        strcpy(outtext,&bigbuf[j+1]);	
	sprintf(prit, "From : %s \n     To: %s \nSubject: %s \n  %s \n", pioe[text_id].sender, pioe[text_id].reciever, pioe[text_id].topic, outtext);
	gtk_text_buffer_insert(buffer, &iter, prit, -1);
       
        memset(bigbuf,0,sizeof(bigbuf));
	gtk_widget_modify_text(view, GTK_STATE_NORMAL, &text_color); 

	/*char *text_font = (char*)malloc(50);
       
	strcpy(text_font,text_style);
	PangoFontDescription* chosen_font;	
	chosen_font = pango_font_description_from_string(text_font);
	gtk_widget_modify_font (view,chosen_font);
        free(text_font);*/

}
void toggle_button_callback (GtkWidget* widget,  gpointer data)
{
	GtkTreeIter iter;
	gboolean valid;
      
	valid = gtk_tree_model_get_iter_first (model, &iter);
        printf("valid= %d\n",valid);
	gint row_count = 0;
	while(valid)
	{
		char *num;
		char* read;
                //read=(char *)malloc(2);
                //strcpy(read,"1");
		gchar *str_data1;
		gchar *str_data2;
		gchar *str_data3;
     		gboolean   int_data;
		
		row_count ++;
		max_mail = max_mail > row_count ? max_mail : row_count ;
	      
		if (gtk_tree_selection_iter_is_selected(GTK_TREE_SELECTION(list_select), &iter))
		{
			gtk_tree_model_get (model, &iter, 
				LIST_CHECKED, &int_data,
				LIST_READ, &read,
				LIST_NUM,&num,
				LIST_NAME,&str_data1,
				LIST_MESSAGE,&str_data2,
				LIST_DATE, &str_data3,
                        	  -1);

			g_print ("Row %d: (%d, %s, %s, %s, %s, %s)\n", row_count, int_data, read, num, str_data1, str_data2, str_data3);

			int flag;
			if(int_data == 1)
				flag = 0;
			else
				flag = 1;

			gtk_list_store_set (GTK_LIST_STORE(list_store), &iter, 0, flag,  LIST_READ,GTK_STOCK_APPLY ,-1);
			gtk_text_buffer_set_text((GtkTextBuffer*)text_buffer, "", 0);
                        strcpy(pioe[(int)*num-49].kind,"recr");
                        printf("shifouyidu=%s\n",pioe[0].kind);
			insert_text((GtkTextBuffer *)text_buffer, (int)*num);
                        printf("shifouyidu=%s\n",pioe[0].kind);                   
			g_free(num);
			g_free (read);
      			g_free (str_data1);
			g_free (str_data2);
			g_free (str_data3);

			if(!isMulChoice)
			{
				if(list_last != -1)
				{
					gtk_tree_model_get_iter_first (model, &iter);
					int j;
					for(j=1; j< list_last; j++)
						gtk_tree_model_iter_next (model, &iter);
					gtk_list_store_set (GTK_LIST_STORE(list_store), &iter, 0, 0, -1);
				}
				list_last = row_count;
		
				break;
			}
		}
		valid = gtk_tree_model_iter_next (model, &iter);
          
	}
	
}




void insert_list_text(char* str)
{
	//database
	int i;
        char task[10]={0};
        if(strcmp(str,"Receive Box")==0) 
        strcpy(task,"recv");
        if(strcmp(str,"Send Box")==0) 
        strcpy(task,"send");
        if(strcmp(str,"Draft Box")==0) 
        strcpy(task,"draf");
        if(strcmp(str,"Trash Box")==0) 
        strcpy(task,"tras");
        hostpage(task);
       
	for(i=0; i<total; i++)
	{
		char *num=(char *)malloc(20);
		char  *name=(char *)malloc(20);
		char  *subject=(char *)malloc(50);
		char  *data=(char *)malloc(20);
		sprintf(num,"%d",i+1);
		strcpy(name,pioe[i].sender);//
		strcpy (subject,pioe[i].topic);//
		strcpy(data,pioe[i].time);//

		num = g_strdup_printf("%s", num); 
		name = g_strdup_printf("%s", name);
		subject = g_strdup_printf("%s", subject);
		data = g_strdup_printf("%s", data);

		gtk_list_store_append(GTK_LIST_STORE(list_store), &list_iter);
		gtk_list_store_set(GTK_LIST_STORE(list_store), &list_iter, 
					LIST_CHECKED, FALSE,
					LIST_READ,GTK_STOCK_ABOUT,
					LIST_NUM, num,
					LIST_NAME, name, 
					LIST_MESSAGE, subject, 
					LIST_DATE, data, 
					-1);
		free(num);
		free(name);
		free(subject);
		free(data);
	}
}

GtkWidget* create_list()
{
	GtkWidget* scrolled_window;
	GtkWidget* tree_view;
	GtkCellRenderer* cell;
	GtkTreeViewColumn* column;

	//scrolled window
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_size_request (GTK_WIDGET (scrolled_window), 800, 400);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	
	//list store & tree view
	list_store = gtk_list_store_new(LIST_CNT, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	tree_view = gtk_tree_view_new();
	
	//pack
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW (scrolled_window), tree_view);
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(list_store));
	g_object_unref(list_store);

	insert_list_text("Receive Box");

	//set column
	cell = gtk_cell_renderer_toggle_new ();
	column = gtk_tree_view_column_new_with_attributes ("", cell, "active", LIST_CHECKED, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
	
	cell = gtk_cell_renderer_pixbuf_new ();
	column = gtk_tree_view_column_new_with_attributes ("", cell, "stock-id", LIST_READ, NULL);	
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));

	cell = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Num", cell, "text", LIST_NUM, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
	column = gtk_tree_view_column_new_with_attributes ("Name", cell, "text", LIST_NAME, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
	column = gtk_tree_view_column_new_with_attributes ("Subject", cell, "text", LIST_MESSAGE, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));
	column = gtk_tree_view_column_new_with_attributes ("Date", cell, "text", LIST_DATE, NULL);
  	gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_COLUMN (column));

	//set selectpioe[num].sender
	list_select = gtk_tree_view_get_selection(GTK_TREE_VIEW (tree_view));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(list_select), GTK_SELECTION_SINGLE);
	model=gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	g_signal_connect(G_OBJECT(list_select), "changed", G_CALLBACK(toggle_button_callback), NULL);

	return scrolled_window;
}

void on_tree_clicked(GtkTreeSelection* select, gpointer data)
{
	GtkTreeIter iter;  
//	GtkTreeModel* model;  
	char* str;  
  
	if(gtk_tree_selection_get_selected(select, &tree_model, &iter))  
	{  
		gtk_tree_model_get(tree_model, &iter, TREE_NAME, &str, -1);  
		printf("%s\n", str);
		gtk_list_store_clear(list_store);
		insert_list_text(str);
		g_free(str);  
	}  
}

GtkWidget* create_tree()
{
	GtkWidget *scrolled_window;
	GtkWidget* tree;
	GtkTreeIter iter_p;
	GtkTreeIter iter;
	GtkTreeStore* tree_store;
	GtkCellRenderer* renderer;
	GtkTreeViewColumn* column;
	GtkTreeSelection* select;

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_size_request (GTK_WIDGET (scrolled_window), 300, 700);
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
	column  = gtk_tree_view_column_new_with_attributes("1", renderer, "text", TREE_NAME, NULL);
	gtk_tree_view_append_column((GtkTreeView *)tree, (GtkTreeViewColumn *)column);
	gtk_tree_view_expand_all((GtkTreeView *)tree);

	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(tree));
	
	//set select
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW (tree));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(select), GTK_SELECTION_SINGLE);
	tree_model=gtk_tree_view_get_model(GTK_TREE_VIEW(tree));
	g_signal_connect(G_OBJECT(select), "changed", G_CALLBACK(on_tree_clicked), NULL);

	gtk_widget_show_all (scrolled_window);

	return scrolled_window;
}

GtkWidget* create_toolbar()
{
	GtkWidget* toolbar;
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),GTK_ICON_SIZE_DND);
/*	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_NEW,"新建","new",GTK_SIGNAL_FUNC(new_mail),(gpointer)("new"),-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_OK,"回复","reply",GTK_SIGNAL_FUNC(new_mail),(gpointer)("reply"),-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_CONVERT,"转发","resend",GTK_SIGNAL_FUNC(new_mail),(gpointer)("resend"),-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_DELETE,"删除","delete",GTK_SIGNAL_FUNC(select_deleted),model,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_UNDELETE,"撤销删除","undel",GTK_SIGNAL_FUNC(test_button_clicked),list_store,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_FIND,"搜索","search",GTK_SIGNAL_FUNC(test_button_clicked),(gpointer)("search"),-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_SELECT_ALL,"全选","sel_all",GTK_SIGNAL_FUNC(on_button_all),model,-1);
	gtk_toolbar_insert_stock(GTK_TOOLBAR(toolbar),GTK_STOCK_APPLY,"确定","ok",GTK_SIGNAL_FUNC(on_button_ok),model,-1);
	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));	gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);
*/	
	GtkWidget *iconw = gtk_image_new_from_file ("./res/1.png"); 
	GtkWidget *button_new = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "new",  "新建", "Private", iconw,  GTK_SIGNAL_FUNC (new_mail),  (gpointer)1);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/2.png"); 
	GtkWidget * button_reply = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "reply",  "回复", "Private", iconw,  GTK_SIGNAL_FUNC (new_mail),  (gpointer)2);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/3.png"); 
	GtkWidget * button_resend = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "resend",  "转发", "Private", iconw,  GTK_SIGNAL_FUNC (new_mail),  (gpointer)3);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/4.png"); 
	GtkWidget * button_sel_del = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "sel_del",  "选择删除", "Private", iconw,  GTK_SIGNAL_FUNC (select_deleted),  model);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/5.png"); 
	GtkWidget * button_undel = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "undel",  "撤销删除", "Private", iconw,  GTK_SIGNAL_FUNC (test_button_clicked),  list_store);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/7.png"); 
	GtkWidget * button_sel_all = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "sel_all",  "全选", "Private", iconw,  GTK_SIGNAL_FUNC (on_button_select),  (gpointer)1);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/6.png"); 
	GtkWidget * button_sel_none = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "sel_none",  "反选", "Private", iconw,  GTK_SIGNAL_FUNC (on_button_select),  (gpointer)2);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 

	iconw = gtk_image_new_from_file ("./res/8.png"); 
	GtkWidget * button_del = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "delete",  "确定删除", "Private", iconw, GTK_SIGNAL_FUNC(on_button_ok),  model);
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_ICONS);

/*	iconw = gtk_image_new_from_file ("./res/6.png"); 
	GtkWidget * button_search = gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), "search",  "搜索", "Private", iconw,  GTK_SIGNAL_FUNC (test_button_clicked),  "search");
  	gtk_toolbar_append_space (GTK_TOOLBAR (toolbar)); 
*/
	return toolbar;
}

void create_mail_window(gpointer* data)
{
	GtkWidget* window;
	GtkWidget* box_main;
	GtkWidget* box_toolbar;
	GtkWidget* toolbar;
	GtkWidget* box_text;
	GtkWidget* box1;
	GtkWidget* box2;
	GtkWidget* entry_search;
	GtkWidget* button_search;
	GtkWidget* button_reply;
	GtkWidget* button_resend;
	GtkWidget* button_del;
	GtkWidget* vpaned;
	GtkWidget* tree;
	GtkWidget* list;
	GtkWidget* iconw;
	GtkWidget* menubar;

	//set window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Mail");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);

		gtk_widget_set_name (window, "first_win");

	//set box_main
	box_main = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (window), box_main);

	//menubar
	get_main_menu (window, &menubar);
	gtk_box_pack_start (GTK_BOX (box_main), menubar, FALSE, TRUE, 0);

	//set toolbar
  	box_toolbar = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (box_main), box_toolbar);

	toolbar = create_toolbar();
	gtk_box_pack_start(GTK_BOX(box_toolbar),toolbar,FALSE,TRUE,0);

	//set box text
	box_text = gtk_hbox_new (FALSE, 10);
	gtk_box_pack_start (GTK_BOX (box_main), box_text, TRUE, TRUE, 0);

	box1 = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_text), box1, FALSE, TRUE, 0);

	box2 = gtk_vbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_text), box2, TRUE, TRUE, 0);

	//set tree
	tree = create_tree();
	gtk_box_pack_start (GTK_BOX (box1), tree, TRUE, TRUE, 0);	

	//set list
	vpaned = gtk_vpaned_new();
	gtk_box_pack_start (GTK_BOX (box2), vpaned, TRUE, TRUE, 0);

	list = create_list();
	gtk_paned_add1(GTK_PANED(vpaned), list);

	//set text view
	text = jjy_create_text(800, 300);
	gtk_paned_add2(GTK_PANED(vpaned), text);

	//show mail window
	gtk_widget_show_all(window);

	//destroy log in window
	gtk_widget_destroy((GtkWidget*)data);

	gtk_main();
}
