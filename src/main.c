#include <gtk/gtk.h>

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

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
