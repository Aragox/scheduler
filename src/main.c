#include <gtk/gtk.h>

//Algorithm 
int algorithm = 0;

// Custom structure that holds pointers to widgets and user variables
typedef struct {
    // Add pointers to widgets below
    //GtkWidget *w_x;
} app_widgets;

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;
    // Instantiate structure, allocating memory for it
    app_widgets     *widgets = g_slice_new(app_widgets);
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/scheduler.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    // Get pointers to widgets here
    //widgets->w_x  = GTK_WIDGET(gtk_builder_get_object(builder, "x"));
    
     // Widgets pointer are passed to all widget handler functions as the user_data parameter
    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show_all(window);                
    gtk_main();
    // Free up widget structure memory
    g_slice_free(app_widgets, widgets);

    return 0;
}

// Dummy handler function
/*void x(GtkButton *button, app_widgets *app_wdgts)
{
}*/

// called when fcfs algorithm is choosed
void on_fcfs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 1;
}

// called when sjf algorithm is choosed
void on_sjf_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 2;
}

// called when rr algorithm is choosed
void on_rr_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 3;
}

// called when ps algorithm is choosed
void on_ps_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 4;
}

// called when "ps with rr" algorithm is choosed
void on_psrr_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 5;
}

// called when mqs algorithm is choosed
void on_mqs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 6;
}

// called when mfqs algorithm is choosed
void on_mfqs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
algorithm = 7;
}

/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
//Funciones que cierran el programa
// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

// called when the exit button of the main window is clicked
void on_exit_main_clicked()
{
    gtk_main_quit();
}
