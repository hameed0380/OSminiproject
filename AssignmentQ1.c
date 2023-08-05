// Question 1
#include <stdio.h>
#include <stdlib.h>

#define stack 0
#define heap 1


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
    if(lst==0)
    {
        if(dl->left->next!=NULL&&dl->right!=dl->left)
        {
            dl->left->next->val=x;
            dl->left=dl->left->next;
            return 1;
        }
        else
        {
            printf("There is no space\n");
            return 0;
        }
    }
    else if(lst==1)
    {
        if(dl->right->prev!=NULL&&dl->right!=dl->left)
        {
            dl->right->prev->val=x;
            dl->right=dl->right->prev;
            return 1;
            
        }
        else
        {
            printf("There is no space\n");
            return 0;
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

int main()
{
    list myList;
    constructor(&myList);
    insertAfter(&myList, 0, stack);
    insertAfter(&myList, 0, heap);
    printList(&myList);
    nExtra(&myList, 5, -1);
    printList(&myList);
    deallocate(&myList);
    return 0;
}