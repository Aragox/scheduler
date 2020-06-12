/*
Implementación del Queue obtenida de: https://www.geeksforgeeks.org/priority-queue-using-linked-list/ 
*/

// C code to implement Priority Queue 
// using Linked List 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
  
// Node 
typedef struct node { 
    //Variables of the node process
    int id;
    int arrive_time;
    int work_units;
    int number_of_terms;
    long double sum_pi;
    long long int fact; 
    int priority; // Lower values indicate higher priority 
    int optional; // An extra argument 
  
    struct node* next; 
  
} Node; 
 

// Operations that change the direction of variables returned. YEAH!!!
int change_direction1(int value) {
    return value;
}

long double change_direction2(long double value) {
    return value;
}

long long int change_direction3(long long int value) {
    return value;
}

// PEEK OPERATIONS
// Return the id at head 
int peek_id(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->id); 
}
  
// Return the arrive_time at head 
int peek_arrivetime(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->arrive_time); 
}

// Return the work_units at head 
int peek_workunits(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->work_units); 
} 

// Return the number_of_terms at head 
int peek_numberofterms(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->number_of_terms); 
}  

// Return the sum_pi at head 
long double peek_sumpi(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1.00000;
    }
    return change_direction2((*head)->sum_pi); 
} 

// Return the fact at head 
long long int peek_fact(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction3((*head)->fact); 
} 

// Return the priority at head 
int peek_priority(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->priority); 
} 

// Return the optional argument at head 
int peek_optional(Node** head) 
{ 
    if ((*head) == NULL) {
       return -1;
    }
    return change_direction1((*head)->optional); 
} 

// Return the priority at head->next 
int get_nextpriority(Node** head) 
{ 
    if ((*head) == NULL || (*head)->next == NULL) {
       return -1;
    }
    return change_direction1((*head)->next->priority); 
}        
 

//SET OPERATIONS
// Set the number_of_terms at head 
void set_numberofterms(Node** head, int number_of_terms) 
{ 
    (*head)->number_of_terms = number_of_terms; 
}  

// Set the sum_pi at head 
void set_sumpi(Node** head, long double sum_pi) 
{ 
    (*head)->sum_pi = sum_pi; 
} 

// Set the fact at head 
void set_fact(Node** head, long long int fact) 
{ 
    (*head)->fact = fact; 
} 

// Set the priority at head 
void set_priority(Node** head, int priority) 
{ 
    (*head)->priority = priority; 
} 

// Set the optional argument at head 
void set_optional(Node** head, int optional) 
{ 
    (*head)->optional = optional; 
}        


// Function to Create A New Node 
Node* newNode(int id, int arrive_time, int work_units, int number_of_terms, long double sum_pi, long long int fact, int priority, int optional) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->id = id; 
    temp->arrive_time = arrive_time; 
    temp->work_units = work_units; 
    temp->number_of_terms = number_of_terms; 
    temp->sum_pi = sum_pi; 
    temp->fact = fact;
    temp->priority = priority;
    temp->optional = optional; 
    temp->next = NULL; 
  
    return temp; 
} 

// Function to Create A New Node. It acepts a node as parameter 
Node* newNode_head(Node** head) 
{ 
    Node* temp = (Node*)malloc(sizeof(Node)); 
    temp->id = peek_id(head); 
    temp->arrive_time = peek_arrivetime(head); 
    temp->work_units = peek_workunits(head); 
    temp->number_of_terms = peek_numberofterms(head); 
    temp->sum_pi = peek_sumpi(head); 
    temp->fact = peek_fact(head);
    temp->priority = peek_priority(head);
    temp->optional = peek_optional(head); 
    temp->next = NULL; 
  
    return temp; 
} 

//POP
// Removes the element with the 
// highest priority form the list 
void pop(Node** head) 
{ 
    Node* temp = *head; 
    (*head) = (*head)->next; 
    free(temp); 
} 
 
//PUSH NEW NODE
// Function to push according to priority 
void push(Node** head, int id, int arrive_time, int work_units, int number_of_terms, long double sum_pi, long long int fact, int p, int optional) 
{ 
    Node* start = (*head);

    if (start != NULL) { // Head is not null
  
	    // Create new Node 
	    Node* temp = newNode(id, arrive_time, work_units, number_of_terms, sum_pi, fact, p, optional);
	  
	    // Special Case: The head of list has lesser 
	    // priority than new node. So insert new 
	    // node before head node and change head node. 
	    if ((*head)->priority > p) { 
	  
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
    } else {
            start = newNode(id, arrive_time, work_units, number_of_terms, sum_pi, fact, p, optional);
    }

} 

//PUSH HEAD OF QUEUE2 IN QUEUE1
// Function to push according to priority of head2
void push_head(Node** head1, Node** head2)
{ 
    Node* start = (*head1);

    if (start != NULL) { // Head is not null
  
	    // Create new Node 
	    //Node* temp = newNode((*head2)->id, (*head2)->arrive_time, (*head2)->work_units, (*head2)->number_of_terms, (*head2)->sum_pi, (*head2)->fact, (*head2)->priority, (*head2)->optional);
	    Node* temp = newNode(peek_id(head2), peek_arrivetime(head2), peek_workunits(head2), peek_numberofterms(head2), peek_sumpi(head2), peek_fact(head2), peek_optional(head2), peek_optional(head2));
	    // Special Case: The head of list has lesser 
	    // priority than new node. So insert new 
	    // node before head node and change head node. 
	//    if ((*head1)->priority > (*head2)->priority) { 
	    if ((*head1)->priority > temp->priority) { 
	  
		// Insert New Node before head 
		temp->next = *head1; 
		(*head1) = temp;

	    } else {
		// Traverse the list and find a 
		// position to insert new node 
		while (start->next != NULL && 
	//               start->next->priority <= (*head2)->priority) { 
		       start->next->priority <= temp->priority) { 
		    start = start->next; 
		} 
	  
		// Either at the ends of the list 
		// or at required position 
		temp->next = start->next; 
		start->next = temp; 
	    }
     } else {
	    // Create new Node 
	    //start = newNode((*head2)->id, (*head2)->arrive_time, (*head2)->work_units, (*head2)->number_of_terms, (*head2)->sum_pi, (*head2)->fact, (*head2)->priority, (*head2)->optional);
	    start = newNode(peek_id(head2), peek_arrivetime(head2), peek_workunits(head2), peek_numberofterms(head2), peek_sumpi(head2), peek_fact(head2), peek_optional(head2), peek_optional(head2));
     } 
} 

//ISEMPTY 
// Function to check is list is empty 
int isEmpty(Node** head) 
{ 
    return (*head) == NULL; 
} 
  
// EXAMPLE CODE 
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

