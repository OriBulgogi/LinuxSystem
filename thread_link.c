#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct param { //argv[] parameter 전달을 위한 구조체
    char* node_n;
    char* repeat_n;
    } param_t;

typedef struct node {
        int data;
        struct node *next;
    } Node_t;

    typedef struct list {
        Node_t *head;
        Node_t *tail;
        pthread_mutex_t lock;
    } List_t;

    List_t total;

void List_append(Node_t *h, Node_t *t);
void *make_piece (void *arg);

int main (int argc, char* argv[])
{
    int thread_num = atoi(argv[1]);
    pthread_t threads[thread_num];
    int i;
   
    param_t arg;
    arg.node_n = argv[2];
    arg.repeat_n = argv[3];

    for(i=0; i<thread_num; i++)
        pthread_create(&threads[i], NULL, make_piece, (void *) &arg);

    pthread_mutex_destroy(&total.lock);

    pthread_exit(NULL);
    return 0;
}

void List_append(Node_t *h, Node_t *t){ //total에 연결
    
    if(total.head == NULL){
        total.head = h;
        total.tail = t;
    }else{
        total.tail->next->next = h->next;
        total.tail = t;
    }
    while(total.head->next != NULL){
        printf("%d", total.head->next->data);
        total.head =  total.head->next;
    }
    //printf("\n");
    
}

void *make_piece (void *arg){ 
    param_t *data = (param_t*)arg;
    int node_num = atoi(data->node_n);
    int repeat_num = atoi(data->repeat_n); //parameter

    List_t *sub_list = (List_t*)malloc(sizeof(List_t));
    Node_t *new_node[node_num];
    sub_list->head = malloc(sizeof(Node_t));
    sub_list->tail = malloc(sizeof(Node_t));
    new_node[0] = (malloc(sizeof(Node_t)));
    new_node[0] -> next = NULL;
    new_node[0] -> data = 0;
    sub_list->tail->next=new_node[0];

    for (int i = 1; i<node_num; i++){
        new_node[i] = malloc(sizeof(Node_t));
        new_node[i] -> data = i;
        new_node[i] -> next = new_node[i-1]; 
    }
    sub_list->head->next=new_node[node_num-1];

    total.lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER; 
    pthread_mutex_lock(&total.lock);
    for(int i =0; i<repeat_num; i++){    
        List_append(sub_list->head, sub_list->tail);
    }
    pthread_mutex_unlock(&total.lock);
    
    pthread_exit(NULL);
}