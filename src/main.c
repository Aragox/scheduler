//#include <stdio.h>
#include "queue.h"
#include <gtk/gtk.h>

//To check if a file is valid
int file_valid = 1; 

//To check if a file was opened from the dialog
int a_file_was_opened = 0;

//Variables of the algorithm
int algorithm = 0; //Stores the id of the selected algorithm
int subalgoritmn = 0; //Additional algorithm for mqs and mqsf algoritmhs only
int expropriation = 0; 
int number_of_processes = 0;
int quantum = 0;
int work_to_be_done = 0;

//Variables of the process
int priority = 0;
int arrive_time = 0;
int work_units = 0;
int number_of_terms = 0;
int pi = 0;

gpointer error_message = ""; // Stores the error message to display in the error dialogue

GtkWidget *window_main; //Main window

// Custom structure that holds pointers to widgets and user variables 
typedef struct {
    // Add pointers to widgets below
    GSList *windows;      //List of windows
    GtkWidget *main_label; //Label in the main window 
} app_widgets;

int open_message_dialog ();
void open_resolve_window (app_widgets *app_wdgts);  

/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTION THAT DISPLAYS WARNING MESSAGE IN THE ERROR WINDOW'S LABEL*/ 

/*void update_message(app_widgets *app_wdgts, gpointer data) NOT USED
//Display warning message on the label with id tag filevalid_label, in file chooser dialog
{
 gtk_label_set_text(GTK_LABEL(app_wdgts->error_label), data);
}*/
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

        gtk_widget_show_all ((GtkWidget*)GTK_WINDOW(window_main)); // Show main window
        
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
  gtk_widget_show_all ((GtkWidget*)GTK_WINDOW(window_main)); // Show main window
}

/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS FOR OPENING, GET DATA AND VALIDATE .txt FILE */

int getfiledata(char *filename,  app_widgets *app_wdgts)
//Function that gets the data from the file
{
  FILE *file;

  file = fopen(filename, "r");

  int i;

  int* arrivaltimes_array;
  int* workunits_array;
  int* priorities_array; 

  if (file){

     fscanf(file, "%d", &i); //Get Algorithm
     if (i != algorithm){ // The algorithm value obtained from the file is different from the one selected in the main menu
       file_valid = 0; // Invalid file for the selected algorithm
       error_message = "The algorithm value obtained from the file is different from the one selected in the main menu";
       return 0;
     } 

     fscanf(file, "%d", &i); //Get Expropriation
     expropriation = i;
     if (expropriation == 0){ // There is no expropriation

       fscanf(file, "%d", &i); //Get amount of work to be done before voluntarily giving up the procesor (in work units)
       work_to_be_done = i;
      
     } else { // There is expropriation

       fscanf(file, "%d", &i); //Get quantum
       quantum = i;

       switch (algorithm)
       {
        case 1: // There should be no expropriation in the FCFS Algorithm
            file_valid = 0; // Invalid file for the selected algorithm
            error_message = "There should be no expropriation in the FCFS Algorithm";
            return 0;
            break;
        case 2: 
            break; 
        case 3: 
            break;
        case 4: 
            break;
        case 5: 
            break;
        case 6: 
            break;
        case 7: 
            break;
       }
     }

     fscanf(file, "%d", &i); //Get number of processes
     number_of_processes = i;
     if (number_of_processes < 5 || number_of_processes > 25) { // Number of processes not in range 5-25
       file_valid = 0; // Invalid file 
       error_message = "Number of processes not in range 5-25";
       return 0;
     }
     
     arrivaltimes_array = calloc(number_of_processes, sizeof(int) );  // Make dynamic array 
     workunits_array = calloc(number_of_processes, sizeof(int) );  // Make dynamic array 
     priorities_array = calloc(number_of_processes, sizeof(int) );  // Make dynamic array 
     
     int cont = 0; 
     while (cont < number_of_processes && (!feof (file))){ //OBTAIN THE ARRIVAL TIMES OF THE PROCESSES
        fscanf(file, "%d", &i);
        *(arrivaltimes_array + cont) = i;            // put value i in array element cont
        cont = cont + 1;          
     }
     if (cont < number_of_processes) { // File is incomplete
       file_valid = 0; // Invalid file 
       error_message = "Incomplete File in “ARRIVAL TIMES OF THE PROCESSES” (Not valid)";
       return 0;
     }

     cont = 0; 
     while (cont < number_of_processes && (!feof (file))){ // GET THE WORK UNITS OF THE PROCESSES
        fscanf(file, "%d", &i);
        *(workunits_array + cont) = i;            // put value i in array element index
        cont = cont + 1;          
     }
     if (cont < number_of_processes) { // File is incomplete
       file_valid = 0; // Invalid file 
       error_message = "Incomplete File in “WORK UNITS OF THE PROCESSES” (Not valid)";
       return 0;
     }

     switch (algorithm)
     {
	case 1: // FCFS Algorithm
	    break;
	case 2: // SJF Algorithm
	    break; 
	case 3: // RR Algorithm
	    break;
	case 4: // PS Algorithm
	    cont = 0; 
	    while (cont < number_of_processes && (!feof (file))){ // OBTAIN THE PRIORITIES OF THE PROCESSES
	       fscanf(file, "%d", &i);
               *(priorities_array + cont) = i;            // put value i in array element index
	       cont = cont + 1;          
	    }
            if (cont < number_of_processes) { // File is incomplete
              file_valid = 0; // Invalid file 
              error_message = "Incomplete File in “PRIORITIES OF THE PROCESSES” (Not valid)";
              return 0;
            }
	    break;
	case 5: // PSRR Algorithm
	    cont = 0; 
	    while (cont < number_of_processes && (!feof (file))){ // OBTAIN THE PRIORITIES OF THE PROCESSES
	       fscanf(file, "%d", &i);
               *(priorities_array + cont) = i;            // put value i in array element index
	       cont = cont + 1;          
	    }
            if (cont < number_of_processes) { // File is incomplete
              file_valid = 0; // Invalid file 
              error_message = "Incomplete File in “PRIORITIES OF THE PROCESSES” (Not valid)";
              return 0;
            }
	    break;
	case 6: // MQS Algorithm
	    fscanf(file, "%d", &i); //Get Sub-Algorithm
	    subalgoritmn = i;
            if (subalgoritmn > 5 || subalgoritmn < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm not in range 1-5";
              return 0;
            }
	    break;
	case 7: // MFQS Algorithm
	    fscanf(file, "%d", &i); //Get Sub-Algorithm
	    subalgoritmn = i;
            if (subalgoritmn > 5 || subalgoritmn < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm not in range 1-5";
              return 0;
            }
	    break;
     }
      
    fclose (file);
//    update_message(app_wdgts, "Archivo cargado"); //Actualizar mensaje de notificacion
//    update(); FUNCIÓN QUE NO ESTÁ HECHA
printf("\n");
    for (int index = 0; index < number_of_processes; index++ ) {
   	printf("%d", *(arrivaltimes_array+index) );
    }
printf("\n");
    for (int index = 0; index < number_of_processes; index++ ) {
   	printf("%d", *(workunits_array+index) );
    }
printf("\n");
    for (int index = 0; index < number_of_processes; index++ ) {
   	printf("%d", *(priorities_array+index) );
    }
    free(arrivaltimes_array); // Un-reserve the array
    free(workunits_array); // Un-reserve the array
    free(priorities_array); // Un-reserve the array
  }
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
    getfiledata(filename, app_wdgts); // HERE YOU GET THE DATA OF THE FILE
    g_free (filename);
    a_file_was_opened = 1;
  }

  gtk_widget_destroy (dialog);

  if (a_file_was_opened) { //Open next window
     if (!file_valid) { //Open file is not valid 
        open_message_dialog (error_message);
     } else { //Open file is valid
        open_resolve_window (app_wdgts);
     }
  }
  file_valid = 1; // Reset the validity value to its standard value
  return 0;
}

int open_message_dialog (gpointer data)
{
  GtkWidget *dialog;
  GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_message_dialog_new (GTK_WINDOW(window_main),
                                 flags,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "%s", (char *)data);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog); 
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
/*FUNCTIONS TO CREATE/OPEN A POPUP WINDOW*/

void open_resolve_window (app_widgets *app_wdgts)
//Function that makes the pop-up window where the algorithm runs, appear
{
  if (g_slist_length (app_wdgts->windows) == 1){ //Sólo forma la ventaja emergente si sólo está presente la ventana original
     GtkBuilder      *builder;

     builder = gtk_builder_new();

     GtkWidget       *window_resolve;
     GtkWidget       *cancel_button;

     gtk_builder_add_from_file (builder, "glade/window_resolve.glade", NULL);

     window_resolve = GTK_WIDGET(gtk_builder_get_object(builder, "window_resolve"));

     app_wdgts->windows = g_slist_prepend (app_wdgts->windows, window_resolve);  //Agregar ventana a la lista

     g_signal_connect (G_OBJECT (window_resolve), "destroy", G_CALLBACK (on_window_destroy), app_wdgts);  //Conectar señales

     cancel_button = GTK_WIDGET(gtk_builder_get_object(builder, "cancel_resolve"));
     g_signal_connect (G_OBJECT (cancel_button), "clicked", G_CALLBACK (close_emergent_window), app_wdgts);
	                               
     gtk_widget_show_all (window_resolve);

     gtk_widget_hide ((GtkWidget*)GTK_WINDOW(window_main)); // Disable main window
  }
}

/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*MAIN*/
int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    // Instantiate structure, allocating memory for it
    app_widgets *widgets = g_slice_new(app_widgets);
    widgets->windows = NULL;
    
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/scheduler.glade");

    window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));

    widgets->windows = g_slist_prepend (widgets->windows, window_main);  //Agregar ventana a la lista 

    // Get pointers to widgets here
    widgets->main_label = GTK_WIDGET(gtk_builder_get_object(builder, "main_label"));
    
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

