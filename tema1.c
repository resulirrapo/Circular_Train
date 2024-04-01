#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MMR "MOVE__MECHANIC_RIGHT"
#define MML "MOVE__MECHANIC_LEFT"
#define SRS "SEARCH_RIGHT_S"
#define SLS "SEARCH_LEFT_S"
#define WRITE_X "WRITE"
#define INSL "INSERT_LEFT"
#define INSR "INSERT_RIGHT"
#define SHOW "SHOW"
#define SHOW_C "SHOW_CURRENT"
#define SWITCH "SWITCH"
#define SEARCH "SEARCH"
#define EXECUTE "EXECUTE"


typedef struct node{
    char value;
    struct node* prev;
    struct node* next;
} Node;

typedef struct Train {
	Node* sant; //head of the list
    Node* current;  //current position of the mechanic
} Train;

typedef struct QueueNode{
	char* elem;
	struct QueueNode *next;
} QueueNode;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

Queue* createQueue(){
	Queue * coada = malloc(sizeof(Queue));
	coada->front = NULL;
	coada->rear = NULL;
	coada->size = 0;
	return coada;
} 

int isQueueEmpty(Queue *q){
	if (q->rear == NULL)
		return 1;
	return 0;
}

void enqueue(Queue *q, char* command){
	QueueNode* node = malloc(sizeof(QueueNode));
	if(isQueueEmpty(q)) {

		q->rear = node;
		q->front = node;

	} else {

		q->rear->next = node;
		q->rear = node;

	}
	q->rear->elem = malloc(20*sizeof(char));
	strcpy(node->elem,command);
	node->elem[strlen(command) - 1] = '\0';
	q->size++;
}

void dequeue(Queue* q){
	if (isQueueEmpty(q)) {
		return;
	}
		QueueNode* temp = q->front;

		if (q->front == q->rear) {
			free(q->rear);
			q->front = q->rear = NULL;
		} else 
		{
			q->front = q->front->next;
			free(temp);
		}
		q->size--;
}

void destroyQueue(Queue *q){
	while (isQueueEmpty(q)) {
		dequeue(q);
	}
	free(q);
}



Train* init_train() {
    Train  *train = (Train  *)malloc(sizeof(struct Train));
	train->sant = malloc(sizeof(Node));
    train->sant->value = '0';
    train->sant->prev = NULL;

	train->current = malloc(sizeof(Node));
    train->current->value = '#';
    train->current->prev = train->sant;
    train->current->next = NULL;
    train->sant->next = train->current;

	
    return train;
}

int move_mechanic_left(Train* mechanic) {
    if (mechanic->current != mechanic->sant->next) {
        mechanic->current = mechanic->current->prev;
	}
		else if (mechanic->current == mechanic->sant->next) {
		// connect the first vagon to the last vagon and move the mechanic there
		while (mechanic->current->next != NULL) {
			mechanic->current = mechanic->current->next;
		}
	}
}


void move_mechanic_right(Train* mechanic) {
    if (mechanic->current->next == NULL) {
        mechanic->current->next = malloc(sizeof(Node));
        mechanic->current->next->prev = mechanic->current;
        mechanic->current->next->next = NULL;
		mechanic->current->next->value = '#';
    }
    mechanic->current = mechanic->current->next;
}

void clear_cell(Train* train) {
	// return to initial state if there is only one vagon
    if (train->current == train->sant->next && train->current->next == NULL) {
        free(train->current);
        train->current = train->sant;
        train->sant->next = NULL;
        return;
    }

    Node* temp= train->current;

    // Check if the current is the first vagon
    if (train->current == train->sant->next) {
        while (train->current->next != NULL) {
            train->current = train->current->next;
        }
    } else {
        train->current = train->current->prev;
    }
    // Reconnect the previous and next vagons
    if (temp->prev) {
        temp->prev->next = temp->next;
    }
    if (temp->next) {
        temp->next->prev = temp->prev;
    }
    free(temp);
}

void clear_all(Train* train) {
    Node* current = train->sant->next;
    
	// iterating  and removing the vagons one by one
    while (current != NULL) {
        Node* temp= current;
        current = current->next;
        
        free(temp);
    }

    // After removing all vagons reset the train to its initial state
    train->sant->next = NULL;
    train->current = train->sant;
}

void search(Train* mechanic, char* str) {
    int len = strlen(str);
    Node* temp = mechanic->current;

    // Iterate through the train starting from the current vagon
    while (temp != NULL) {
		Node* start = mechanic->current;
		int found = 1;

		// If the vagon is empty, the string is not found
		if (mechanic->current == NULL) {
			found = 0;
			break;
	}

        // Iterate through each character of the string
        for (int i = 0; i < len; ++i) {
            if (temp == NULL || temp->value != str[i]) {
                found = 0;
                break;
            }
            temp = temp->next; // Move to the next vgon
        }

        if (found) {
            mechanic->current = temp; // Move the mechanic to the first vagon of the string
            return;
        }

        // If we've looped back to the start, break to avoid infinite loop
        if (mechanic->current->next == NULL) {
            mechanic->current = mechanic->sant; // Move to the first vagon to start circular search
        }
        if (mechanic->current->next == start) {
            break;
        }
    }

    printf("ERROR\n");
}



// void search_left_s(Train* mechanic, char* value, FILE* fp) {
// 	Node* temp = mechanic->current;

// 	while (temp != mechanic->sant) {
// 		if (temp->value != value) {
// 			temp = temp->prev;
// 		} else {
// 			mechanic->current = temp;
// 			return;
// 		}
// 	} 
// 	fprintf(fp,"ERROR\n");
	
// }

// void search_right_s(Train* mechanic, char* value, FILE* fp) {
//     Node* temp = mechanic->current;

// 		while (temp->next != NULL) {
// 			if (temp->value == value) {
// 				mechanic->current = temp;
// 				return;
// 			}
// 			temp = temp->next;
// 		} 
// 		if (temp->value == value) {
// 			mechanic->current = temp;
// 			return;
// 		}
// 		if (temp->next == NULL) {
// 			fprintf(fp,"ERROR\n");
// 		}
// }

void write_char(Train* mechanic, char value) {
    mechanic->current->value = value;
}

void insert_left(Train* mechanic, char value, FILE* fp) {
	if(mechanic->current->prev == mechanic->sant) {
			fprintf(fp,"ERROR\n");
	} else {
		// create a new node
		Node* newnode2 = malloc(sizeof(Node));
		newnode2->next = mechanic->current;
		newnode2->prev = mechanic->current->prev;

		// update the mechanic's position
		mechanic->current->prev->next = newnode2;
		mechanic->current->prev = newnode2;
		newnode2->value = value;
		mechanic->current = mechanic->current->prev;
	}
}

void insert_right(Train* mechanic, char value) {

	Node* temp = mechanic->current;
	Node* newnode3 = malloc(sizeof(Node));

	if (temp->next != NULL)
	{
		newnode3->next = temp->next;
		temp->next = newnode3;
		temp->next->next->prev = newnode3;
		newnode3->prev = temp;
	} else {
		temp->next = newnode3;
		newnode3->prev = temp;
		newnode3->next = NULL;
	}
	newnode3->value = value;
	mechanic->current = newnode3;
}

void show_current(Train* train, FILE *fp) {
	fprintf(fp,"%c\n", train->current->value);
}


void show(Train* train, FILE *fp) {
    Node* temp = train->sant->next;
    while (temp != NULL) {
        if (temp == train->current) {
            fprintf(fp,"|%c|", temp->value);
        } else {
            fprintf(fp,"%c", temp->value);
        }
        temp = temp->next;
    }
    fprintf(fp,"\n");
}

void Switch(Queue* q) {
    if (q == NULL || q->front == NULL || q->front == q->rear) {
        return;
    }

    QueueNode* prev = NULL;
    QueueNode* current = q->front;
    QueueNode* next = NULL;
    q->rear = q->front; // The front will now be the rear

    // Traverse the list and reverse the direction of each link
    while (current != NULL) {
        next = current->next; // Store next node
        current->next = prev; // Reverse current node's pointer
        prev = current; // Move pointers one position ahead for next iteration
        current = next;
    }

    q->front = prev; // The last node processed becomes the front
}

int main() {

    FILE* fp = fopen("tema1.in", "r");
	FILE *fout = fopen("tema1.out","w");

    int var;
    char *c = malloc(20*sizeof(char));

	Train *train = init_train();
	Queue *queue = createQueue();

    fscanf(fp,"%d\n",&var);
    printf("%d\n", var);

    for(int i = 0; i < var; i++) {
        fgets(c, 20, fp); 

		// if (!strncmp(c,SHOW_C,12)) {
		// 	show_current(train,fout);
		// }
		if(!strncmp(c,SHOW,4)){
			show(train,fout);
		}
		 else if (strncmp(c,EXECUTE,7) != 0) {
			enqueue(queue, c);
		}
		// else if (!strncmp(c, SWITCH, 6) !=0 ) {
		// 	Switch(queue);
		// }
		 else {

			if (queue->front) {

			// if (!strcmp(queue->front->elem,MMR)) {
			// 	move_mechanic_right(train);

			// } else if (!strcmp(queue->front->elem,MML)) {
			// 	move_mechanic_left(train);

			// } else if (!strncmp(queue->front->elem,SEARCH,6)) {
			// 	search(train, queue->front->elem[7]);
			// }
			//  else if (!strncmp(queue->front->elem,SRS,14)) {
			// 	search_right_s(train, queue->front->elem[15],fout);

			// } else if (!strncmp(queue->front->elem,SLS,13)) {
			// 	search_left_s(train, queue->front->elem[14],fout);

			// } 
			if (!strncmp(queue->front->elem,WRITE_X,5)) {
				write_char(train, queue->front->elem[6]);}

			// } else if (!strncmp(queue->front->elem,INSL,11)) {
			// 	insert_left(train, queue->front->elem[12],fout);

			// } else if (!strncmp(queue->front->elem,INSR,12)) {
			// 	insert_right(train, queue->front->elem[13]);
			// }
			}
		  dequeue(queue);
		}
    }

fclose(fp);
fclose(fout);

	return 0;
}