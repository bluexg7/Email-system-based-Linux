#include "text_view.h"
#include "sendpost.h"
#include "font.h"

extern char name[20];
GtkWidget *Send_textview;
GtkTextBuffer *Send_buffer;
extern GtkWidget* entry_rec;
extern GtkWidget* entry_topic;
extern GtkWidget *entry_hiden_rec;
extern GtkWidget* entry_chao_rec;
extern char *s;
extern GdkColor ncolor; 
GtkClipboard *myClipboard;

void get_content(GtkButton *SaveButton,GtkWidget *widget)
{
	const char* rec = gtk_entry_get_text(GTK_ENTRY(entry_rec));
	const char* h_rec = gtk_entry_get_text(GTK_ENTRY(entry_hiden_rec));
	const char* topic = gtk_entry_get_text(GTK_ENTRY(entry_topic));
	const char* chao = gtk_entry_get_text(GTK_ENTRY(entry_chao_rec));

	//存放到数据库
	g_printf("receiver is :%s\n",rec);//receiver 
	g_printf("hiden_receiver is :%s\n",h_rec);//sender
	g_printf("chao_receiver is :%s\n",chao);//chao_rec
	g_printf("topic is :%s\n",topic);// 
	if(strcmp(rec,"") == 0)
	{
		create_error_window(6);
	}
	else
	{
		GtkTextIter start,end;
		gchar *S_text;

		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
		S_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&start,&end,FALSE);

		//存放到数据库    	
		g_printf("message is :%s\n",S_text);//文本
		g_print("font2 is %s\n",s);
		g_print("color2 is %d,%d,%d\n",ncolor.red,ncolor.green,ncolor.blue);
		//---------------------
		char *kind="send";
		char  *time="";
		char  info[50];
                sprintf(info,"%s;%d;%d;%d",s,ncolor.red,ncolor.green,ncolor.blue);//text size ,color

		sendpost((char *)topic,(char *)kind,(char *)h_rec,(char *)rec,(char *)time,(char *)S_text,(char *)info);
		//---------------------

		g_free(s);
		free(S_text);
		create_error_window(7);
		gtk_widget_destroy(widget);
	}
}

void  save_content(GtkButton *SaveButton, GtkWidget *widget)
{
	const char* rec = gtk_entry_get_text(GTK_ENTRY(entry_rec));
	const char* h_rec = gtk_entry_get_text(GTK_ENTRY(entry_hiden_rec));
        const char* topic = gtk_entry_get_text(GTK_ENTRY(entry_topic));

	//存放到数据库
	g_printf("receiver is :%s\n",rec);//receiver 
	g_printf("hiden_receiver is :%s\n",h_rec);//sender
	g_printf("topic is :%s\n",topic);// 
	g_printf("color is %d, %d, %d", ncolor.red, ncolor.green, ncolor.blue);
        if(strcmp(rec,"") == 0)
	{
		create_error_window(6);
	}
    	else{
		GtkTextIter start,end;
	    	char *S_text;

	    	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(Send_buffer),&start,&end);
	    	S_text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(Send_buffer),&start,&end,FALSE);	    
	
		//存放到数据库    	
	       	g_printf("message is :%s\n",S_text);//文本
		g_print("font2 is %s\n",s);
		g_print("color2 is %d,%d,%d\n",ncolor.red,ncolor.green,ncolor.blue);

		//---------------------
		char *kind="save";
		char *time="";
		char  info[50];
                sprintf(info,"%s;%d;%d;%d;",s,ncolor.red,ncolor.green,ncolor.blue);//text size ,color

		sendpost((char *)topic,(char *)kind,(char *)h_rec,(char *)rec,time,(char *)S_text,(char *)info);
		//---------------------
		free(S_text);
		g_free(s);
		gtk_widget_destroy(widget);
	}
}

void on_close(GtkWidget *CloseButton,gpointer data)
{
	gtk_widget_destroy(CloseButton);
}

void c_c_p(GtkWidget *Send_textview,gpointer data){
	if((int)data == 1){
		gtk_text_buffer_cut_clipboard(Send_buffer,myClipboard,TRUE);
		g_print("cut\n");
	}
	else if((int)data == 2){
		gtk_text_buffer_copy_clipboard(Send_buffer,myClipboard);//复制
	}
	else if((int)data == 3){
		gtk_text_buffer_paste_clipboard(Send_buffer,myClipboard,NULL,TRUE);//粘贴
	}
}

GtkWidget * create_text(GtkWidget * window, int w, int h){

	GtkWidget *Send_scrolled_win;
	GdkColor color; 
 	GtkTextIter start, end;


	myClipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);//默认的剪切板;

        /*------------------------------设置Send_text view-------------------------*/
    	Send_textview = gtk_text_view_new();/*生成text view*/
    	gtk_text_view_set_justification(GTK_TEXT_VIEW(Send_textview),GTK_JUSTIFY_LEFT);/*控制文字显示方向的,对齐方式*/
    	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(Send_textview),TRUE);/*设置光标可见*/
        gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(Send_textview),5);/*设置上行距*/
    	gtk_text_view_set_pixels_below_lines(GTK_TEXT_VIEW(Send_textview),5);/*设置下行距*/
    	gtk_text_view_set_pixels_inside_wrap(GTK_TEXT_VIEW(Send_textview),5);/*设置词距*/
        gtk_text_view_set_left_margin(GTK_TEXT_VIEW(Send_textview),10);/*设置左边距*/
    	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(Send_textview),10);/*设置右边距*/
    	Send_buffer =  gtk_text_view_get_buffer(GTK_TEXT_VIEW(Send_textview));

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(Send_textview),GTK_WRAP_CHAR);
	gtk_text_view_set_indent (GTK_TEXT_VIEW(Send_textview), 30);

    	Send_scrolled_win = gtk_scrolled_window_new(NULL,NULL);
    	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(Send_scrolled_win),Send_textview);
    	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Send_scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_widget_set_size_request (GTK_WIDGET (Send_scrolled_win), w, h);

    	gtk_widget_show_all(Send_scrolled_win);
	return Send_scrolled_win;
}
