#include "reg.h"

static GtkWidget* entry1;
static GtkWidget* entry2;
static GtkWidget* entry3;
static GtkWidget* entry4;
static GtkWidget *regis_window;

void on_button_clicked (GtkWidget* widget,gpointer data)
{

	if((int)data == 1){
		gtk_entry_set_text(GTK_ENTRY(entry1),"");
		gtk_entry_set_text(GTK_ENTRY(entry2),"");
		gtk_entry_set_text(GTK_ENTRY(entry3),"");
		gtk_entry_set_text(GTK_ENTRY(entry4),"");
	} else if((int)data == 3){
		/*改变文本空的可编辑状态*/
		gtk_editable_set_editable(GTK_EDITABLE(entry1),GTK_TOGGLE_BUTTON(widget)->active);
		gtk_editable_set_editable(GTK_EDITABLE(entry2),GTK_TOGGLE_BUTTON(widget)->active);
		gtk_editable_set_editable(GTK_EDITABLE(entry3),GTK_TOGGLE_BUTTON(widget)->active);
		gtk_editable_set_editable(GTK_EDITABLE(entry4),GTK_TOGGLE_BUTTON(widget)->active);
	}else {
		const gchar* username = gtk_entry_get_text(GTK_ENTRY(entry1));
		const gchar* password = gtk_entry_get_text(GTK_ENTRY(entry2));
		const gchar* password1 = gtk_entry_get_text(GTK_ENTRY(entry2));
		const gchar* phonenum = gtk_entry_get_text(GTK_ENTRY(entry4));

		gboolean isWrong = FALSE;
		int len = strlen(username);

		if(username[0] >= 'a' && username[0] <= 'z' || username[0] >= 'A' && username[0] <= 'Z' );
		else 
			isWrong = TRUE;
		if(isWrong == FALSE && (len <= 5 || len >16)){
			isWrong = TRUE;
		}
		
		int i;
		for(i = 1;isWrong == FALSE && i < len;i ++){
			if(username[i] >= 'a' && username[i] <= 'z' || username[i] >= 'A' && username[i] <= 'Z' || username[i] == '_' || username[i] >= '0' && username[i] <= '9' );
			else isWrong = TRUE;
		}

		if(isWrong == FALSE && strcmp(password,password1) != 0)
			isWrong = TRUE;

		len = strlen(password);

		if(isWrong == FALSE && (len <= 5 || len >16)){
			isWrong = TRUE;
		}
		if(isWrong == TRUE){
			create_error_window(2);
		}
		else{
			int flag=0;
			reginfo(username,password,phonenum,&flag);

			if(flag == -1)//fail
				create_error_window(3);
			else{//succeed
				gtk_widget_destroy(data);
			}
		}
	} 
    
}

GtkWidget * create_regis_win(){
    GtkWidget* window;
    GtkWidget* box;
    GtkWidget* box1;
    GtkWidget* box2;
    GtkWidget* box3;
    GtkWidget* box4;
    GtkWidget* box5;
    GtkWidget* box6;
    GtkWidget* label1;
    GtkWidget* label2;
    GtkWidget* label3;
    GtkWidget* label4;
    GtkWidget* label5;
    GtkWidget* label6;
    GtkWidget* label7;
    GtkWidget* label8;
    GtkWidget* label9;
    GtkWidget* button;
    GtkWidget* sep1;
    GtkWidget* sep2;
    gint isWrong;

    //设置窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_widget_destroy),NULL);
    gtk_window_set_title(GTK_WINDOW(window),"Register window");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);  

    box = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(window),box);
    box1 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box1,FALSE,FALSE,5);
    box2 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box2,FALSE,FALSE,5);
    box3 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box3,FALSE,FALSE,5);
    box4 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box4,FALSE,FALSE,5);
    box5 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box5,FALSE,FALSE,5);
    sep2 = gtk_hseparator_new();//分割线
    gtk_box_pack_start(GTK_BOX(box),sep2,FALSE,FALSE,5);
    box6 = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),box6,TRUE,TRUE,5);

    label1 = gtk_label_new("欢迎注册Wanted邮箱");
    sep1 = gtk_hseparator_new();//分割线
    gtk_box_pack_start(GTK_BOX(box1),label1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box1),sep1,FALSE,FALSE,5);

    label2 = gtk_label_new("*邮件地址");
    entry1 = gtk_entry_new();
    label3 = gtk_label_new("6-16个字符，包括字母、数字、下划线，以字母开头");
    gtk_box_pack_start(GTK_BOX(box2),label2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),entry1,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box2),label3,FALSE,FALSE,5);

    label4 = gtk_label_new("*密       码");
    entry2 = gtk_entry_new();
    label5 = gtk_label_new("6-16个字符，包括字母、数字、下划线，区分大小写");
    gtk_entry_set_visibility(GTK_ENTRY(entry2),FALSE);
    gtk_box_pack_start(GTK_BOX(box3),label4,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),entry2,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box3),label5,FALSE,FALSE,5);

    label6 = gtk_label_new("*确认密码");
    entry3 = gtk_entry_new();
    label7 = gtk_label_new("请再次输入密码");
    gtk_entry_set_visibility(GTK_ENTRY(entry3),FALSE);
    gtk_box_pack_start(GTK_BOX(box4),label6,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box4),entry3,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box4),label7,FALSE,FALSE,5);

    label8 = gtk_label_new(" 手机号码 ");
    entry4 = gtk_entry_new();
    label9 = gtk_label_new("密码遗忘或被盗时可通过手机短信取回密码");
    gtk_box_pack_start(GTK_BOX(box5),label8,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box5),entry4,FALSE,FALSE,5);
    gtk_box_pack_start(GTK_BOX(box5),label9,FALSE,FALSE,5);

    button = gtk_check_button_new_with_label("IsEditable");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),(gpointer)3);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button),TRUE);
    gtk_box_pack_start(GTK_BOX(box6),button,TRUE,TRUE,10);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("清空");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),(gpointer)1);
    gtk_box_pack_start(GTK_BOX(box6),button,TRUE,TRUE,10);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("确认");
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_button_clicked),window);
    gtk_box_pack_start(GTK_BOX(box6),button,TRUE,TRUE,5);
    gtk_widget_show(button);

    gtk_widget_show_all(window);
    return window;
}
void show_regis()
{
  	regis_window=create_regis_win();
  	gtk_widget_show(regis_window);

}

