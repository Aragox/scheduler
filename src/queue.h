/*
Implementación del Queue obtenida de: https://www.geeksforgeeks.org/priority-queue-using-linked-list/ 
*/

// C code to implement Priority Queue 
// using Linked List 
#include <stdio.h> 
#include <stdlib.h> 
  
// Node 
typedef struct node { 
    //Variables of the node process
    int arrive_time;
    int work_units;
    int number_of_terms;
    int sum_pi; 
    int priority; // Lower values indicate higher priority 
  
    struct node* next; 
  
} Node; 
  
// Function to Create A New Node 
Node* newNode(int arrive_time, int work_units, int number_of_terms, int sum_pi, int priority) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->arrive_time = arrive_time; 
    temp->work_units = work_units; 
    temp->number_of_terms = number_of_terms; 
    temp->sum_pi = sum_pi; 
    temp->priority = priority; 
    temp->next = NULL; 
  
    return temp; 
} 
  
// Return the arrive_time at head 
int peek_arrivetime(Node** head) 
{ 
    return (*head)->arrive_time; 
}

// Return the work_units at head 
int peek_workunits(Node** head) 
{ 
    return (*head)->work_units; 
} 

// Return the number_of_terms at head 
int peek_numberofterms(Node** head) 
{ 
    return (*head)->number_of_terms; 
}  

// Return the sum_pi at head 
int peek_sumpi(Node** head) 
{ 
    return (*head)->sum_pi; 
} 

// Return the priority at head 
int peek_priority(Node** head) 
{ 
    return (*head)->priority; 
}      
  
// Removes the element with the 
// highest priority form the list 
void pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
  
// Function to push according to priority 
void push(Node** head, int arrive_time, int work_units, int number_of_terms, int sum_pi, int p) 
{ 
    Node* start = (*head); 
  
    // Create new Node 
    Node* temp = newNode(arrive_time, work_units, number_of_terms, sum_pi, p); 
  
    // Special Case: The head of list has lesser 
    // priority than new node. So insert new 
    // node before head node and change head node. 
    if ((*head)->priority < p) { 
  
        // Insert New Node before head 
        temp->next = *head; 
        (*head) = temp;

    } else {
        // Traverse the list and find a 
        // position to insert new node 
        while (start->next != NULL && 
               start->next->priority <= p) { 
            start = start->next; 
        } 
  
        // Either at the ends of the list 
        // or at required position 
        temp->next = start->next; 
        start->next = temp; 
    } 
} 
  
// Function to check is list is empty 
int isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
} 
  
// Driver code 
/*int main() 
{ 
    // Create a Priority Queue 
    // 7->4->5->6 
    Node* pq = newNode(4, 1); 
    push(&pq, 5, 2); 
    push(&pq, 6, 3); 
    push(&pq, 7, 0); 
  
    while (!isEmpty(&pq)) { 
        printf("%d ", peek(&pq)); 
        pop(&pq); 
    } 
  
    return 0; 
} 
*/

