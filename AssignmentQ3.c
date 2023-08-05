#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>

#define stack 0
#define heap 1
#define nExt 5


typedef struct node_dl
{
    int val;
    struct node_dl* next,*prev;
} 
node;

// typedef means you no longer have to write struct all over the place
typedef struct
{
    node *head;
    node *tail;
    node *left;
    node *right;
} 
list;

void constructor(list *dm)
{
    dm->head=NULL;
    dm->tail=NULL;
    dm->left=NULL;
    dm->right=NULL;
}
void nExtra(list *z,int x,int v)
{
    node *ptr;
    for(int n=0;n<x;n++)
    {
        ptr=malloc(sizeof(node));
        ptr->next=NULL;
        ptr->prev=NULL;
        
        ptr->val=v;
        if(z->left->next==NULL)
        {
            z->left->next=ptr;
            z->right->prev=ptr;
        }
        else
        {
            ptr->next=z->left->next;
            ptr->prev=z->left;
            ptr->next->prev=ptr;
            z->left->next=ptr;
            
        }
    }
}

int insertAfter(list *dl,int x,int lst)
{
    node *ptr=(node*)malloc(sizeof(node));
    ptr->val=0;
    ptr->next=NULL;
    ptr->prev=NULL;
    if(lst==0)
    {
        if(dl->head==NULL)
        {
            dl->head=ptr;
            dl->left=ptr;
            return 1;
        }
    }
    else
    {
        if(dl->tail==NULL)
        {
            dl->tail=ptr;
            dl->right=ptr;
            return 1;
            
        }
    }
    return 0;
}

void deallocate(list *dl)
{
    node *ptr=dl->head;
    node *tmp;
    while(ptr!=NULL)
    {
        tmp=ptr;
        ptr=ptr->next;
        free(tmp);
    }
    ptr=dl->right;
    while(ptr!=NULL)
    {
        tmp=ptr;
        ptr=ptr->next;
        free(tmp);
    }
    dl->head=NULL;
    dl->tail=NULL;
    dl->left=NULL;
    dl->right=NULL;
}

void printList(list *dl)
{
    node *ptr=dl->head;
    printf("[");
    while(ptr!=NULL&&ptr!=dl->right)
    {
        printf("%3d,",ptr->val);
        ptr=ptr->next;
    }
    ptr=dl->right;
    while(ptr!=NULL)
    {
        if(ptr->next==NULL)
            printf("%3d",ptr->val);
        else
            printf("%3d,",ptr->val);
        ptr=ptr->next;
    }
    printf("]");
    printf("\n");
    
}

void makelist(list*ptr)
{
    constructor(ptr);
    insertAfter(ptr, 0, stack);
    insertAfter(ptr, 0, heap);
    nExtra(ptr, nExt, -1);
}

int alen=5;
int push(list *dl,int x)
{
    if(x%2==1)
    {
        if(dl->left->next!=NULL&&dl->right!=dl->left->next)
        {
            dl->left->next->val=x;
            dl->left=dl->left->next;
            alen--;
            return 1;
        }
        else
        {
            nExtra(dl, nExt, -1);
            push(dl,x);
            alen+=nExt;
            return 0;
        }
    }
    else
    {
        if(dl->right->prev!=NULL&&dl->right->prev!=dl->left)
        {
            dl->right->prev->val=x;
            dl->right=dl->right->prev;
            if(dl->right->next==NULL)
                dl->right->next=dl->tail;
            alen--;
            return 1;
            
        }
        else
        {
            nExtra(dl, nExt, -1);
            push(dl,x);
            alen+=nExt;
            return 0;
        }
    }
}

void delnext(list *dl)
{
    node *tmp;
    for(int i=0;i<nExt;i++)
    {
        tmp=dl->left->next;
        dl->left->next=dl->left->next->next;
        free(tmp);
    }
}

int pull(list *dl,int x)
{
    int v;
    if(x==stack)
    {
        if(dl->left!=dl->head)
        {
            v=dl->left->val;
            dl->left->val=-1;
            dl->left=dl->left->prev;
            alen++;
        }
        else
        {
            if(dl->right!=dl->tail)
            {
                v=dl->right->val;
                dl->right->val=-1;
                dl->right=dl->right->next;
                alen++;
            }
            else
            {
                printf("warning: no nodes to be deleted!\n");
                return -1;
            }
        }
        
    }
    else
    {
        if(dl->right!=dl->tail)
        {
            v=dl->right->val;
            dl->right->val=-1;
            dl->right=dl->right->next;
            alen++;
        }
        else
        {
            
            if(dl->left!=dl->head)
            {
                v=dl->left->val;
                dl->left->val=-1;
                dl->left=dl->left->prev;
                alen++;
            }
            else
            {
                printf("warning: no nodes to be deleted!\n");
                return -1;
            }
        }
    }
    if(alen>nExt)
    {
        delnext(dl);
        alen-=nExt;
    }
    return v;
}
list myList;

pthread_mutex_t lock;
pthread_cond_t cv;
void* pusher(void* p)
{
    int v;
    for(int i=0;i<15;i++)
    {
        v=100*((float)rand()/RAND_MAX);
        pthread_mutex_lock(&lock);
        push(&myList,i);
        printList(&myList);
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* puller(void* p)
{
    int v;
    for(int i=0;i<15;i++)
    {
        v=100*((float)rand()/RAND_MAX);
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cv, &lock);
        pull(&myList,(v+1)%2);
        printList(&myList);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    srand(getchar());
    pthread_t tid1,tid2;
    makelist(&myList);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cv, NULL);
    pthread_create(&tid1, NULL, &pusher, NULL);
    pthread_create(&tid2, NULL, &puller, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    deallocate(&myList);
    return 0;
}

