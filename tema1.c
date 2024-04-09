// Resuli Rrapo 321CD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema1.h"

int main() {

    FILE* fp = fopen("tema1.in", "r");
	FILE *fout = fopen("tema1.out","w");

    int var;
    char *c = malloc(20*sizeof(char));

	Train *train = init_train();
	Queue *queue = createQueue();

    fscanf(fp,"%d\n",&var);

    for(int i = 0; i < var; i++) {
        fgets(c, 20, fp);

		// compare the commands with the predefined ones
        if (!strncmp(c, SWITCH, 6) !=0 ) {
			Switch(queue);
		}
		else if (!strncmp(c,SHOW_C,12)) {
			show_current(train,fout);
		} 
        else if (!strncmp(c,SHOW,4)) {
			show(train,fout);
		}
		else if (strncmp(c,EXECUTE,7) != 0) {
			enqueue(queue, c);
		}
		 else {
			// checking if the queue is not empty
			if (queue->front) {

			if (!strcmp(queue->front->elem,MR)) {
				move_mechanic_right(train);
			}
			else if (!strcmp(queue->front->elem,ML)) {
				move_mechanic_left(train);
			}
			else if (!strcmp(queue->front->elem,CC)) {
				clear_cell(train);
			}
			else if (!strcmp(queue->front->elem,CA)) {
				clear_all(train);
			}
			else if (!strncmp(queue->front->elem, SR, 12))
			{
				char* searchStringRight = queue->front->elem + 13;
				search_right(train, searchStringRight, fout);
			}
			else if (!strncmp(queue->front->elem, SL, 11)) {
				char* searchStringLeft = queue->front->elem + 12;
				search_left(train, searchStringLeft, fout);
			}
			else if (!strncmp(queue->front->elem, SEARCH, 6)) {
				char* searchString = queue->front->elem + 7;
				search(train, searchString, fout);
			}
			else if (!strncmp(queue->front->elem,WRITE_X,5)) {
				write_char(train, queue->front->elem[6]);
			}
			else if (!strncmp(queue->front->elem,INSL,11)) {
				insert_left(train, queue->front->elem[12],fout);
			}
			else if (!strncmp(queue->front->elem,INSR,12)) {
				insert_right(train, queue->front->elem[13]);
			}
			}
		 dequeue(queue);
		}
    }

fclose(fp);
fclose(fout);
clear_all(train); // removes insert right from valgrind problem
free(queue);
free(c);
freetrain(train);

	return 0;
}
