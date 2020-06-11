//#include <stdio.h>
#include "queue.h"
#include <gtk/gtk.h>
#include <time.h>

//To check if a file is valid
int file_valid = 1; 

//To check if a file was opened from the dialog
int a_file_was_opened = 0;

//Variables of the algorithm
int algorithm = 0; //Stores the id of the selected algorithm
int subalgorithm1 = 0; //Additional algorithm for mqs and mqsf algoritmhs only
int subalgorithm2 = 0; //Additional algorithm for mqs and mqsf algoritmhs only
int subalgorithm3 = 0; //Additional algorithm for mqs and mqsf algoritmhs only
int expropriation = 0; 
int number_of_processes = 0;
int quantum = 0;
int work_to_be_done = 0;

//Variables for all the processes 
int* arrivaltimes_array;
int* workunits_array;
int* priorities_array; 
int* queues_array;  


Node* arrivetime_queueMQS1; // queue of processes ordered by arrival times
Node* ready_queueMQS1; 
Node* arrivetime_queueMQS2; // queue of processes ordered by arrival times
Node* ready_queueMQS2; 
Node* arrivetime_queueMQS3; // queue of processes ordered by arrival times
Node* ready_queueMQS3; 
//Queues for schedulig algorithms
Node* arrivetime_queue; // queue of processes ordered by arrival times
Node* ready_queue; // queue of ready processes for ejecution
Node* ready_queue2; // queue of ready processes for ejecution

int count_cycle = 0; 
gpointer error_message = ""; // Stores the error message to display in the error dialogue

GtkWidget *window_main; //Main window

// Custom structure that holds pointers to widgets and user variables 
typedef struct {
    // Add pointers to widgets below
    GSList *windows;      //List of windows
    GtkWidget *main_label; //Label in the main window 
    GtkWidget *workunits_label; //Label in the window_resolve
    GtkWidget *arrivaltime_label; //Label in the window_resolve 
    GtkWidget *processid_label; //Label in the window_resolve 
    GtkWidget *sumpi_label; //Label in the window_resolve
    GtkWidget *numberoftermsdone_label; //Label in the window_resolve
    GtkWidget *workunits_label2; //Label in the window_resolve
//termsprogress_bar
} app_widgets;

int open_message_dialog ();
void open_resolve_window ();
void update (int time);

void execute_fcfs(app_widgets *app_wdgts, Node* arrivetime_queue);
void execute_sjf(app_widgets *app_wdgts, Node* arrive_queue);
void execute_rr(app_widgets *app_wdgts, Node* arrivetime_queue); 
void execute_ps(app_widgets *app_wdgts, Node* arrivetime_queue); 
//void execute_psrr(app_widgets *app_wdgts); SIN IMPLEMENTAR
void execute_mqs(app_widgets *app_wdgts);
//void execute_mfqs(app_widgets *app_wdgts);  SIN IMPLEMENTAR 


/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTION THAT SHOWS DATA IN WINDOW_RESOLVE*/
void update_window_resolve(app_widgets *app_wdgts, int time, Node** head){
// Function that shows data in the window_resolve
   char buffer[50];  //Shows data in the window_resolve
   sprintf(buffer, "%d", peek_workunits(head)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->workunits_label, buffer);
   sprintf(buffer, "%d", peek_arrivetime(head)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->arrivaltime_label, buffer);
   sprintf(buffer, "%d", peek_id(head)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->processid_label, buffer);
   sprintf(buffer, "%Lf", (long double)((peek_sumpi(head))/2)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->sumpi_label, buffer);
   sprintf(buffer, "%d", peek_numberofterms(head)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->numberoftermsdone_label, buffer);
   sprintf(buffer, "%d", peek_workunits(head)); 
   gtk_label_set_text((GtkLabel*)app_wdgts->workunits_label2, buffer);
   update(time);
}

/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTION FOR UPDATE*/
void update(int time){
//Function that makes pending updates of the entire application
     while (g_main_context_iteration(NULL, FALSE));
     usleep(time);
}
/*###########################################################################################################################################
---------------------------------------------------------------------------------------------------------------------------------------------
#############################################################################################################################################*/
/*FUNCTIONS THAT EXECUTE SCHEDULING ALGORITHMS */

int power(int x, unsigned int y) 
/* Function to calculate x raised to the power y */
//Obtained from:
//https://www.geeksforgeeks.org/write-a-c-program-to-calculate-powxn/ 
{ 
    if (y == 0) 
        return 1; 
    else if (y%2 == 0) 
        return power(x, y/2)*power(x, y/2); 
    else
        return x*power(x, y/2)*power(x, y/2); 
} 


void taylor_series(int workunits, int numberoftermsdone, long double sumpi, long long int fact, Node** head)
/* Function to execute taylor series for an expropiative or non expropiative algorithm */
 {
   int totalnumberofterms = workunits;
   int terms_to_do_now = work_to_be_done;
   int time_available = quantum;
   int original = numberoftermsdone;

   if (expropriation) {

     int time = 0; //Count
     if (peek_numberofterms((head)) < peek_workunits((head))) { // If process is NOT finished
       do {
      fact = fact*numberoftermsdone;
      sumpi = sumpi + (power(2,numberoftermsdone)/fact) ;

      numberoftermsdone = numberoftermsdone + 1;

                  time = time + 100;
                  count_cycle = count_cycle + 1; 

       } while ((time < time_available) && (numberoftermsdone < totalnumberofterms));
     }
   } else {
    while (((numberoftermsdone - original) < terms_to_do_now) && (numberoftermsdone < totalnumberofterms)) {
          fact = (long long int)(fact*numberoftermsdone);
          sumpi = sumpi + (long double)(power(2,numberoftermsdone))/(long double)(fact) ;

          numberoftermsdone = numberoftermsdone + 1;
     }
   }

   if (numberoftermsdone >= totalnumberofterms) {
      sumpi = sumpi + 1; //Since series starts with 1
   }

   // SET NODE DATA FOR HEAD THAT BELONGS TO A QUEUE 
   set_sumpi((head), sumpi); // Set sumpi 
   set_numberofterms((head), numberoftermsdone); // Set number of terms 
   set_fact((head), fact); // Set factorial  
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//#######################################################################################################################################################################################################
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void execute_fcfs(app_widgets *app_wdgts, Node* arrivetime_queue)
{

    Node* ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
    pop(&arrivetime_queue); //remove head in arrivetime_queue 

    int sec = 0; // Time count  

   update_window_resolve(app_wdgts, 1000000, &ready_queue); // Show data before do while

  do {
   printf("\n");
   printf("\n");
   printf("Process ID: %d", peek_id(&ready_queue)); 

   taylor_series(peek_workunits(&ready_queue), peek_numberofterms(&ready_queue), peek_sumpi(&ready_queue), peek_fact(&ready_queue), &ready_queue); 

   update_window_resolve(app_wdgts, 250000, &ready_queue);

   sec = sec + 1;

   if (peek_numberofterms(&ready_queue) >= peek_workunits(&ready_queue)) { // If process finished all work
      printf("\n");
      printf("COMPLETE!!: %d", peek_id(&ready_queue));     
      pop(&ready_queue); //remove head in ready_queue
   } else {
  printf("\n");
  printf("PRIORITY: %d", peek_priority(&ready_queue)); 
   }

   if (isEmpty(&ready_queue) && !isEmpty(&arrivetime_queue)) { // ready_queue is empty and arrivetime_queue is not empty
      ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
      sec = peek_arrivetime(&arrivetime_queue);
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
   } else {

   if (!isEmpty(&arrivetime_queue)) { // arrivetime_queue is not empty
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue)); 

   }
  }

  } while (!isEmpty(&ready_queue) || !isEmpty(&arrivetime_queue));

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void execute_rr(app_widgets *app_wdgts, Node* arrivetime_queue)
// RR
{
  Node* ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
  pop(&arrivetime_queue); //remove head in arrivetime_queue 

  Node* ready_queue2 = NULL; 

  int queue_turn = 1;

  int sec = 0; // Time count  

   update_window_resolve(app_wdgts, 1000000, &ready_queue); // Show data before do while

  do {
if ((isEmpty(&ready_queue) && queue_turn == 1) || (isEmpty(&ready_queue2) && queue_turn == -1)) { 
printf("\n");
   printf("FUUUUUUUUUUUCK");
   break; 
} else {
   if (queue_turn == 1) { // Turn of ready_queue 
   printf("\n");
   printf("\n");
   printf("Process ID: %d", peek_id(&ready_queue)); 
      taylor_series(peek_workunits(&ready_queue), peek_numberofterms(&ready_queue), peek_sumpi(&ready_queue), peek_fact(&ready_queue), &ready_queue); 
      update_window_resolve(app_wdgts, 250000, &ready_queue);
   } else { // Turn of ready_queue2
     printf("\n");
     printf("\n");
     printf("Process ID: %d", peek_id(&ready_queue2)); 
       taylor_series(peek_workunits(&ready_queue2), peek_numberofterms(&ready_queue2), peek_sumpi(&ready_queue2), peek_fact(&ready_queue2), &ready_queue2); 
       update_window_resolve(app_wdgts, 250000, &ready_queue2);
   }

   sec = sec + 1;
if (queue_turn == 1) {
   if (peek_numberofterms(&ready_queue) >= peek_workunits(&ready_queue)) { // If process finished all work
      printf("\n");
      printf("COMPLETE!!: %d", peek_id(&ready_queue));     
      pop(&ready_queue); //remove head in ready_queue
   } else {
  printf("\n");
  printf("PRIORITY: %d", peek_priority(&ready_queue)); 

        if (isEmpty(&ready_queue2)) {
          ready_queue2 = newNode_head(&ready_queue);
        } else {
          push_head(&ready_queue2, &ready_queue); //get head node from ready_queue 
        }
        pop(&ready_queue); //remove head in ready_queue
   }
} else {
   if (peek_numberofterms(&ready_queue2) >= peek_workunits(&ready_queue2)) { // If process finished all work
      printf("\n");
      printf("COMPLETE!!: %d", peek_id(&ready_queue2));     
      pop(&ready_queue2); //remove head in ready_queue2
   } else {
  printf("\n");
  printf("PRIORITY: %d", peek_priority(&ready_queue2));

        if (isEmpty(&ready_queue)) {
          ready_queue = newNode_head(&ready_queue2);
        } else {
          push_head(&ready_queue, &ready_queue2); //get head node from ready_queue 
        }
        pop(&ready_queue2); //remove head in ready_queue
   }
}
if (!isEmpty(&arrivetime_queue)) {
   if (isEmpty(&ready_queue) && queue_turn == 1) { // ready_queue is empty and is turn of ready_queue
      ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
      sec = peek_arrivetime(&arrivetime_queue);
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
   } else {

     if (!isEmpty(&ready_queue) && queue_turn == 1) { // ready_queue is NOT empty and is turn of ready_queue
        printf("\n");
        printf("actual_time: %d", sec); 
        printf("\n");
        printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));

        if (peek_arrivetime(&arrivetime_queue) <= sec) {
      push(&ready_queue, peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue  
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
        }
     }
   }
   if (isEmpty(&ready_queue2) && queue_turn == -1) { // ready_queue2 is empty and is turn of ready_queue2
      ready_queue2 = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
      sec = peek_arrivetime(&arrivetime_queue);
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
   } else {

     if (!isEmpty(&ready_queue2) && queue_turn == -1) { // ready_queue2 is NOT empty and is turn of ready_queue2
        printf("\n");
        printf("actual_time: %d", sec); 
        printf("\n");
        printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));

        if (peek_arrivetime(&arrivetime_queue) <= sec) {
      push(&ready_queue2, peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue  
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
        }
     }
   }
}


   if (queue_turn == 1 && isEmpty(&ready_queue)) { // turn of ready_queue and ready_queue is empty 
   printf("\n");
   printf("READY 1, TURN PASS"); 

      queue_turn = queue_turn * -1; // Change queue turn

   } else {

     if (queue_turn == -1 && isEmpty(&ready_queue2)) { // turn of ready_queue2 and ready_queue2 
   printf("\n");
   printf("READY 2, TURN PASS"); 

        queue_turn = queue_turn * -1; // Change queue turn

     }

   }
 
}
  } while (!isEmpty(&ready_queue) || !isEmpty(&ready_queue2) || !isEmpty(&arrivetime_queue));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void execute_MQS(app_widgets *app_wdgts)
{

 if(!isEmpty(&arrivetime_queueMQS1)){ 
    switch (subalgorithm1)
       {
        case 1: 
            execute_fcfs(app_wdgts, arrivetime_queueMQS1);
            break;
        case 2: 
            execute_sjf(app_wdgts, arrivetime_queueMQS1);
            break; 
        case 3: 
            execute_rr(app_wdgts, arrivetime_queueMQS1);
            break;
        case 4: 
            execute_ps(app_wdgts, arrivetime_queueMQS1);
            break;
        case 5:
            //execute_psrr(app_wdgts, arrivetime_queueMQS1); 
            break;
       }
  } 
 if(!isEmpty(&arrivetime_queueMQS2)){ 
    switch (subalgorithm2)
       {
        case 1: 
            execute_fcfs(app_wdgts, arrivetime_queueMQS2);
            break;
        case 2: 
            execute_sjf(app_wdgts, arrivetime_queueMQS2);
            break; 
        case 3: 
            execute_rr(app_wdgts, arrivetime_queueMQS2);
            break;
        case 4: 
            execute_ps(app_wdgts, arrivetime_queueMQS2);
            break;
        case 5: 
            //execute_psrr(app_wdgts, arrivetime_queueMQS2); 
            break;
       } 
  } 
  if(!isEmpty(&arrivetime_queueMQS3)){ 
    switch (subalgorithm3)
       {
        case 1: 
            execute_fcfs(app_wdgts, arrivetime_queueMQS3);
            break;
        case 2: 
            execute_sjf(app_wdgts, arrivetime_queueMQS3);
            break; 
        case 3: 
            execute_rr(app_wdgts, arrivetime_queueMQS3);
            break;
        case 4: 
            execute_ps(app_wdgts, arrivetime_queueMQS3);
            break;
        case 5: 
            //execute_psrr(app_wdgts, arrivetime_queueMQS3); 
            break;
       }
  }

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void execute_sjf(app_widgets *app_wdgts, Node* arrive_queue)
// SJF 
{
  Node* ready_queue = newNode(peek_id(&arrive_queue), peek_arrivetime(&arrive_queue), peek_workunits(&arrive_queue), peek_numberofterms(&arrive_queue), peek_sumpi(&arrive_queue), peek_fact(&arrive_queue), peek_optional(&arrive_queue), peek_optional(&arrive_queue)); //get head node from arrivetime_queue
  pop(&arrive_queue); //remove head in arrivetime_queue 
//PSEUDOCÓDIGO DEL ALGORITMO
/*
Empezar a contar tiempo
Do {
Ejecutar terminos de serie
Actualizar datos de window_resolve
Aumentar contador de tiempo
Si se terminó todo el trabajo del proceso 
   pop(&ready_queue)
else 
   Restar unidades de trabajo de la prioridad del proceso actual (head de ready_queue)
Si arrivetime_queue no está vacío
   Obtener tiempo de llegada  peek_arrivetime(&arrivetime_queue)
   Si tiempo de llegada <= tiempo contado
      Meter head de arrivetime_queue en ready_queue
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
} While ready_queue no está vacío || arrivetime_queue no está vacío*/
  int sec = 0; // Time count  

   update_window_resolve(app_wdgts, 1000000, &ready_queue); // Show data before do while

  do {
   printf("\n");
   printf("\n");
   printf("Process ID: %d", peek_id(&ready_queue)); 

   taylor_series(peek_workunits(&ready_queue), peek_numberofterms(&ready_queue), peek_sumpi(&ready_queue), peek_fact(&ready_queue), &ready_queue); 

   update_window_resolve(app_wdgts, 250000, &ready_queue);

   sec = sec + 1;

   if (peek_numberofterms(&ready_queue) >= peek_workunits(&ready_queue)) { // If process finished all work
      printf("\n");
      printf("COMPLETE!!: %d", peek_id(&ready_queue));     
      pop(&ready_queue); //remove head in ready_queue
   } else {
        if (expropriation) {
        set_priority(&ready_queue, peek_priority(&ready_queue) - (int)(quantum/100)); // Decrease priority
        } else {
        set_priority(&ready_queue, peek_priority(&ready_queue) - work_to_be_done); // Decrease priority
        }
  printf("\n");
  printf("PRIORITY: %d", peek_priority(&ready_queue)); 
   }

   if (isEmpty(&ready_queue) && !isEmpty(&arrive_queue)) { // ready_queue is empty and arrivetime_queue is not empty
      ready_queue = newNode(peek_id(&arrive_queue), peek_arrivetime(&arrive_queue), peek_workunits(&arrive_queue), peek_numberofterms(&arrive_queue), peek_sumpi(&arrive_queue), peek_fact(&arrive_queue), peek_optional(&arrive_queue), peek_optional(&arrive_queue)); //get head node from arrivetime_queue
      sec = peek_arrivetime(&arrive_queue);
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrive_queue));
      pop(&arrive_queue); //remove head in arrivetime_queue 
   } else {

     if (!isEmpty(&ready_queue) && !isEmpty(&arrive_queue)) { // arrivetime_queue is not empty
        printf("\n");
        printf("actual_time: %d", sec); 
        printf("\n");
        printf("next arrivetime: %d", peek_arrivetime(&arrive_queue));

        if (peek_arrivetime(&arrive_queue) <= sec) {
      push(&ready_queue, peek_id(&arrive_queue), peek_arrivetime(&arrive_queue), peek_workunits(&arrive_queue), peek_numberofterms(&arrive_queue), peek_sumpi(&arrive_queue), peek_fact(&arrive_queue), peek_optional(&arrive_queue), peek_optional(&arrive_queue)); //get head node from arrivetime_queue  
      pop(&arrive_queue); //remove head in arrivetime_queue 
        }
     }
   }

  } while (!isEmpty(&ready_queue) || !isEmpty(&arrive_queue));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void execute_ps(app_widgets *app_wdgts, Node* arrivetime_queue)
// PS
{
  Node* ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
  pop(&arrivetime_queue); //remove head in arrivetime_queue 

  int sec = 0; // Time count  

   update_window_resolve(app_wdgts, 1000000, &ready_queue); // Show data before do while

  do {
   printf("\n");
   printf("\n");
   printf("Process ID: %d", peek_id(&ready_queue)); 

   taylor_series(peek_workunits(&ready_queue), peek_numberofterms(&ready_queue), peek_sumpi(&ready_queue), peek_fact(&ready_queue), &ready_queue); 

   update_window_resolve(app_wdgts, 250000, &ready_queue);

   sec = sec + 1;

   if (peek_numberofterms(&ready_queue) >= peek_workunits(&ready_queue)) { // If process finished all work
      printf("\n");
      printf("COMPLETE!!: %d", peek_id(&ready_queue));     
      pop(&ready_queue); //remove head in ready_queue
   } else {
  printf("\n");
  printf("PRIORITY: %d", peek_priority(&ready_queue)); 
   }

   if (isEmpty(&ready_queue) && !isEmpty(&arrivetime_queue)) { // ready_queue is empty and arrivetime_queue is not empty
      ready_queue = newNode(peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue
      sec = peek_arrivetime(&arrivetime_queue);
      printf("\n");
      printf("actual_time: %d", sec); 
      printf("\n");
      printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
   } else {

     if (!isEmpty(&ready_queue) && !isEmpty(&arrivetime_queue)) { // arrivetime_queue is not empty
        printf("\n");
        printf("actual_time: %d", sec); 
        printf("\n");
        printf("next arrivetime: %d", peek_arrivetime(&arrivetime_queue));

        if (peek_arrivetime(&arrivetime_queue) <= sec) {
      push(&ready_queue, peek_id(&arrivetime_queue), peek_arrivetime(&arrivetime_queue), peek_workunits(&arrivetime_queue), peek_numberofterms(&arrivetime_queue), peek_sumpi(&arrivetime_queue), peek_fact(&arrivetime_queue), peek_optional(&arrivetime_queue), peek_optional(&arrivetime_queue)); //get head node from arrivetime_queue  
      pop(&arrivetime_queue); //remove head in arrivetime_queue 
        }
     }
   }

  } while (!isEmpty(&ready_queue) || !isEmpty(&arrivetime_queue));
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
     queues_array = calloc(number_of_processes, sizeof(int) );
    
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
        *(workunits_array + cont) = i;            // put value i in array element cont
        cont = cont + 1;          
     }
     if (cont < number_of_processes) { // File is incomplete
       file_valid = 0; // Invalid file 
       error_message = "Incomplete File in “WORK UNITS OF THE PROCESSES” (Not valid)";
       return 0;
     }

     cont = 0; 
     while (cont < number_of_processes){ // Initialize PRIORITIES in 0's
           *(priorities_array + cont) = 0;            // put value i in array element cont
     cont = cont + 1;          
     }

     switch (algorithm)
     {
  case 1: // FCFS Algorithm
      break;
  case 2: // SJF Algorithm
            cont = 0; 
            while (cont < number_of_processes){ // make the priorities_array the same as workunits_array
               *(priorities_array + cont) = *(workunits_array+cont);            // put value i in array element cont
         cont = cont + 1;          
            }
      break; 
  case 3: // RR Algorithm
      break;
  case 4: // PS Algorithm
      cont = 0; 
      while (cont < number_of_processes && (!feof (file))){ // OBTAIN THE PRIORITIES OF THE PROCESSES
         fscanf(file, "%d", &i);
               *(priorities_array + cont) = i;            // put value i in array element cont
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
               *(priorities_array + cont) = i;            // put value i in array element cont
         cont = cont + 1;          
      }
            if (cont < number_of_processes) { // File is incomplete
              file_valid = 0; // Invalid file 
              error_message = "Incomplete File in “PRIORITIES OF THE PROCESSES” (Not valid)";
              return 0;
            }
      break;
  case 6: // MQS Algorithm 
        cont = 0; 
      while (cont < number_of_processes && (!feof (file))){ // OBTAIN THE PRIORITIES OF THE PROCESSES
         fscanf(file, "%d", &i);
               *(priorities_array + cont) = i;            // put value i in array element cont
         cont = cont + 1;          
      }
            if (cont < number_of_processes) { // File is incomplete
              file_valid = 0; // Invalid file 
              error_message = "Incomplete File in “PRIORITIES OF THE PROCESSES” (Not valid)";
              return 0;
            }

        cont = 0; 
      while (cont < number_of_processes && (!feof (file))){ // OBTAIN QUEUES OF THE PROCESSES
         fscanf(file, "%d", &i);
               *(queues_array + cont) = i;            // put value i in array element cont
         cont = cont + 1;          
      }
            if (cont < number_of_processes) { // File is incomplete
              file_valid = 0; // Invalid file 
              error_message = "Incomplete File in “QUEUES OF THE PROCESSES” (Not valid)";
              return 0;
            }
        cont = 0;

      fscanf(file, "%d", &i); //Get Sub-Algorithm1
      subalgorithm1 = i;
            if (subalgorithm1 > 5 || subalgorithm1 < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm1 not in range 1-5";
              return 0;
            }
      fscanf(file, "%d", &i); //Get Sub-Algorithm2
      subalgorithm2 = i;
            if (subalgorithm2 > 5 || subalgorithm2 < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm2 not in range 1-5";
              return 0;
            }
      fscanf(file, "%d", &i); //Get Sub-Algorithm3
      subalgorithm3 = i;
            if (subalgorithm3 > 5 || subalgorithm3 < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm3 not in range 1-5";
              return 0;
            }
        arrivetime_queueMQS1 = newNode(0, 0, 0, 0, 0, 0, 0, 0); //accommodate processes by arrival time
    arrivetime_queueMQS2 = newNode(0, 0, 0, 0, 0, 0, 0, 0);
    arrivetime_queueMQS3 = newNode(0, 0, 0, 0, 0, 0, 0, 0);
    for (int index = 0; index < number_of_processes; index++ ) {
      if(*(queues_array+index) == 0 ){
            push(&arrivetime_queueMQS1, index, *(arrivaltimes_array+index), *(workunits_array+index), 1, 0, 1, *(arrivaltimes_array+index), *(priorities_array+index)); //accommodate processes by arrival time
        }
        if(*(queues_array+index) == 1 ){
            push(&arrivetime_queueMQS2, index, *(arrivaltimes_array+index), *(workunits_array+index), 1, 0, 1, *(arrivaltimes_array+index), *(priorities_array+index)); //accommodate processes by arrival time
        }
         if(*(queues_array+index) == 2 ){
            push(&arrivetime_queueMQS3, index, *(arrivaltimes_array+index), *(workunits_array+index), 1, 0, 1, *(arrivaltimes_array+index), *(priorities_array+index)); //accommodate processes by arrival time
        }
      }
      pop(&arrivetime_queueMQS1);
      pop(&arrivetime_queueMQS2);
      pop(&arrivetime_queueMQS3);
      fclose (file);
      return 0; 
      break;
  case 7: // MFQS Algorithm FALTA AGREGAR CÓDIGO AQUÍ**********
      fscanf(file, "%d", &i); //Get Sub-Algorithm
      subalgorithm1 = i;
            if (subalgorithm1 > 5 || subalgorithm1 < 1) { // Sub-algorithm not in range 1-5
              file_valid = 0; // Invalid file 
              error_message = "Sub-algorithm1 not in range 1-5";
              return 0;
            }
      break;
     }
      
    fclose (file);
  }

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

// PUTS NODES OF PROCESS IN THE arrivetime_queue
    arrivetime_queue = newNode(0, *(arrivaltimes_array+0), *(workunits_array+0), 1, 0, 1, *(arrivaltimes_array+0), *(priorities_array+0)); //accommodate processes by arrival time
    for (int index = 1; index < number_of_processes; index++ ) {
        push(&arrivetime_queue, index, *(arrivaltimes_array+index), *(workunits_array+index), 1, 0, 1, *(arrivaltimes_array+index), *(priorities_array+index)); //accommodate processes by arrival time
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
        update(1000000);
        switch (algorithm)
        {
     case 1: // FCFS Algorithm
	       execute_fcfs(app_wdgts, arrivetime_queue);
         break;
     case 2: // SJF Algorithm 
               execute_sjf(app_wdgts, arrivetime_queue);   
         break; 
     case 3: // RR Algorithm
               execute_rr(app_wdgts, arrivetime_queue);
         break;
     case 4: // PS Algorithm
               execute_ps(app_wdgts, arrivetime_queue);
         break;
     case 5: // PSRR Algorithm
         break;
     case 6: // MQS Algorithm FALTA AGREGAR CÓDIGO AQUÍ***********
      	       execute_MQS(app_wdgts);
         break;
     case 7: // MFQS Algorithm FALTA AGREGAR CÓDIGO AQUÍ**********
         break;
        }
     }
  }
  file_valid = 1; // Reset the validity value to its standard value

  free(arrivaltimes_array); // Un-reserve the array
  free(workunits_array); // Un-reserve the array
  free(priorities_array); // Un-reserve the array
  free(queues_array);
  return 0;
}

int open_message_dialog (gpointer data)
//Function that opens the error dialog message
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

     // Get pointers to widgets here
     app_wdgts->workunits_label = GTK_WIDGET(gtk_builder_get_object(builder, "workunits_label"));
     app_wdgts->arrivaltime_label = GTK_WIDGET(gtk_builder_get_object(builder, "arrivaltime_label"));
     app_wdgts->processid_label = GTK_WIDGET(gtk_builder_get_object(builder, "processid_label"));
     app_wdgts->sumpi_label = GTK_WIDGET(gtk_builder_get_object(builder, "sumpi_label"));
     app_wdgts->numberoftermsdone_label = GTK_WIDGET(gtk_builder_get_object(builder, "numberoftermsdone_label"));
     app_wdgts->workunits_label2 = GTK_WIDGET(gtk_builder_get_object(builder, "workunits_label2"));

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
