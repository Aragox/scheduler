#include <gtk/gtk.h>

//To check if a file is valid
int file_valid = 0;

//To check if a file was opened from the dialog
int a_file_was_opened = 0;

//Stores the id of the selected algorithm
int algorithm = 0;

GtkWidget *window_main; //Main window

// Custom structure that holds pointers to widgets and user variables 
typedef struct {
    // Add pointers to widgets below
    GSList *windows;      //List of windows
    GtkWidget *main_label; //Label in the main window 
    GtkWidget *error_label; //Label in the error window
} app_widgets;


/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTION THAT DISPLAYS WARNING MESSAGE IN THE ERROR WINDOW'S LABEL*/

void update_message(app_widgets *app_wdgts, gpointer data)
//Display warning message on the label with id tag filevalid_label, in file chooser dialog
{
 gtk_label_set_text(GTK_LABEL(app_wdgts->error_label), data);
}
/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS THAT CLOSE WINDOWS AND APPLICATION*/

// called when main window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}

// called when the exit button of the main window is clicked
void on_exit_main_clicked()
{
    gtk_main_quit();
}

void on_window_destroy (GtkWidget *widget, app_widgets *app_wdgts)
//Function called when destroying the popup. If the popup was the only window open,
// close the entire application.
{
        app_wdgts->windows = g_slist_remove (app_wdgts->windows, widget); //Remove the window from the list
        
        if (g_slist_length (app_wdgts->windows) == 0)
        {
                //If the popup was the only window open, the application closes
                
                g_debug ("Exiting...");
                g_slist_free (app_wdgts->windows);
                gtk_main_quit ();
        }
}

void close_emergent_window(GtkWidget *widget, app_widgets *app_wdgts)
//Function that closes the popup (from the code) and removes it from the list.
{
  gtk_window_close ((GtkWindow*)g_slist_nth(app_wdgts->windows, 0)->data); //Closes the window
  app_wdgts->windows = g_slist_remove (app_wdgts->windows, widget); //Remove the window from the list
}


/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS TO CREATE/OPEN A POPUP WINDOW*/

void open_resolve_window_or_error_window (app_widgets *app_wdgts)
//Function that makes the pop-up window where the algorithm runs, appear; or to make the pop-up of error window
{
  if (g_slist_length (app_wdgts->windows) == 2){ //Sólo forma la ventaja emergente si sólo está presente la ventana original
     GtkBuilder      *builder;

     builder = gtk_builder_new();

     if (file_valid) { // File is valid
     GtkWidget       *window_resolve;
     GtkWidget       *accept_button;
     GtkWidget       *cancel_button;

     gtk_builder_add_from_file (builder, "glade/window_resolve.glade", NULL);

     window_resolve = GTK_WIDGET(gtk_builder_get_object(builder, "window_resolve"));

     app_wdgts->windows = g_slist_prepend (app_wdgts->windows, window_resolve);  //Agregar ventana a la lista

     g_signal_connect (G_OBJECT (window_resolve), "destroy", G_CALLBACK (on_window_destroy), app_wdgts);  //Conectar señales

     accept_button = GTK_WIDGET(gtk_builder_get_object(builder, "accept_resolve"));
     g_signal_connect (G_OBJECT (accept_button), "clicked", G_CALLBACK (close_emergent_window), app_wdgts);   
     cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "cancel_resolve"));
     g_signal_connect (G_OBJECT (cancel_button), "clicked", G_CALLBACK (close_emergent_window), app_wdgts);
	                               
     gtk_widget_show_all (window_resolve);

     } else { // File is invalid
     GtkWidget       *window_error;
     GtkWidget       *accept_button;

     gtk_builder_add_from_file (builder, "glade/window_error.glade", NULL);

     window_error = GTK_WIDGET(gtk_builder_get_object(builder, "window_error"));

     app_wdgts->windows = g_slist_prepend (app_wdgts->windows, window_error);  //Agregar ventana a la lista

     g_signal_connect (G_OBJECT (window_error), "destroy", G_CALLBACK (on_window_destroy), app_wdgts);  //Conectar señales
   
     accept_button = GTK_WIDGET(gtk_builder_get_object(builder, "accept_error"));
     g_signal_connect (G_OBJECT (accept_button), "clicked", G_CALLBACK (close_emergent_window), app_wdgts);
	                               
     gtk_widget_show_all (window_error);
     }    
  }
}
/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS FOR OPENING, GET DATA AND VALIDATE .txt FILE */

int getfiledata(char *filename,  app_widgets *app_wdgts)
//Function that gets the data from the file
{
/*FILE *file;

  file = fopen(filename, "r");

  int i;

  if (file){

     fscanf(file, "%d", &i); //Obtener Booleano
     if (i == 1){
        casilla_hexagonal = TRUE; //Casilla hexagonal
     }
     else{
         casilla_hexagonal = FALSE; //Casilla cuadrada
     } 

     fscanf(file, "%d", &i); //Obtener entrada laberinto
     entrada = i;
     fscanf(file, "%d", &i); //Obtener salida laberinto
     salida = i;
 
     fscanf(file, "%d", &i); //Obtener #filas
     files_gen = i;       
     fscanf(file, "%d", &i); //Obtener #columnas
     colums_gen = i;

     inicialize_maze_dimentions(); //Inicializar listas y matriz
     
     int f = 0;
     int k = 0; 
     fscanf(file, "%d", &i);
     while (!feof (file)){
           printf("%d ", i); 
           M[f][k] = i;
           fscanf(file, "%d", &i);
           k++;
           if (k > colums_gen - 1){
              k = 0;
              f++;
           }
           
     }
      
    fclose (file);
    update_message(app_wdgts, "Archivo cargado"); //Actualizar mensaje de notificacion
    update();
    erase_maze(); //Borrar Laberinto (el que se está desplegando en la ventana)
    suma_x = (gtk_widget_get_allocated_width (drawing_area)/2); //Resetear movimiento en los ejes
    suma_y = (gtk_widget_get_allocated_height (drawing_area)/2);
    draw_maze();//Dibujar laberinto
   
    free(R);  //Liberar memoria de R y C
    free(C);
    maze_showed = TRUE;
    maze_generated = FALSE;
  }*/
  return 0;
}

int readfile(GtkButton *button, app_widgets *app_wdgts)
//Function that uses the filechooser to read
//Obtained from:
//https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html
{
  a_file_was_opened = 0;
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                      GTK_WINDOW(window_main), //parent_window
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename (chooser);
   // getfiledata(filename, app_wdgts); //AQUÍ SE OBTIENE LOS DATOS DEL ARCHIVO PERO NO ESTÁ HECHO AÚN
    g_free (filename);
    a_file_was_opened = 1;
  }

  gtk_widget_destroy (dialog);

  if (a_file_was_opened) { //Open next window
     open_resolve_window_or_error_window (app_wdgts);
  }

  return 0;
}


/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS FOR THE BUTTONS IN THE MAIN WINDOW (TO SELECT ALGORITHM)*/

//Function that test/show if the algorithm is selected (in the main menu's buttons)
void test_main_button(app_widgets *app_wdgts) 
{
  char buffer[50]; 
  sprintf(buffer, "algoritmo: %d", algorithm); 
  gtk_label_set_text((GtkLabel*)app_wdgts->main_label, buffer);
}

// called when fcfs algorithm is choosed
void on_fcfs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 1;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}

// called when sjf algorithm is choosed
void on_sjf_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 2;
  test_main_button(app_wdgts); 
  readfile(button, app_wdgts);
}

// called when rr algorithm is choosed
void on_rr_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 3;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}

// called when ps algorithm is choosed
void on_ps_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 4;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}

// called when "ps with rr" algorithm is choosed
void on_psrr_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 5;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}

// called when mqs algorithm is choosed
void on_mqs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 6;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}

// called when mfqs algorithm is choosed
void on_mfqs_clicked(GtkButton *button, app_widgets *app_wdgts)
{
  algorithm = 7;
  test_main_button(app_wdgts);
  readfile(button, app_wdgts);
}



/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*MAIN*/
int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    // Instantiate structure, allocating memory for it
    app_widgets     *widgets = g_slice_new(app_widgets);
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/scheduler.glade");

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    // Get pointers to widgets here
    widgets->main_label = GTK_WIDGET(gtk_builder_get_object(builder, "main_label"));
    widgets->error_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_label"));
    
     // Widgets pointer are passed to all widget handler functions as the user_data parameter
    gtk_builder_connect_signals(builder, widgets);

    g_object_unref(builder);

    gtk_widget_show_all(window_main);                
    gtk_main();
    // Free up widget structure memory
    g_slice_free(app_widgets, widgets);

    return 0;
}

// Dummy handler function
/*void x(GtkButton *button, app_widgets *app_wdgts)
{
}*/

