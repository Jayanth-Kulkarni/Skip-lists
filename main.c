#include<stdio.h>
#include<stdlib.h>
#define MAXLEVEL 10

/***************************************************************************************
 *  From the pseudo code of search algorithm it can be deduced that list is a structure*
 *  which contains header and the current max level indicated by var level.            *
 *  Header is a pointer to a structure that contains,                                  *
 *  dynamically allocated array of pointers to pointers called forward.                *
 *  which intern points to a self referential structure called node, containing        *
 *  the forward pointer itself and the information stored in a variable data.          *
 ***************************************************************************************
 */

typedef struct node
{
   int data;
   struct node** forward; // its a pointer pointing to another pointer
}node;

typedef struct SL
{
    node* header;
    int level;
}SL;


/************************************************
 *Initialization block which allocates          *
 *memory for header and forward pointers which  *
 *point to the header                           *
 ************************************************
 */

SL *initialize(SL* list)
{
    list->level = 1;
    int i;
    node *head = (node*)malloc(sizeof(struct node));  //allocating memory for header..
    list->header = head;
    head->forward = (node**)malloc(sizeof(node*)*(MAXLEVEL+1)); // allocating forward pointers
    for(i=0;i<=MAXLEVEL;i++)
    {
        list->header->forward[i] = list->header;// all forward pointers of the header are pointing to NIL
    }
    return list;
}

int search_list(SL* list, int key)
{
    int i;
    node* temp;
    temp = list->header;
    for(i=list->level;i>=1;i--)
    {
        while((temp->forward[i]->data)<key)
        {
            temp = temp->forward[i];
        }
    }
    if(temp->forward[1]->data==key)
        return 1;
    return 0;
}
int randomlevel()
{
   float p = 5;
   int num_level=1;


   while(rand()%10>p&&num_level<MAXLEVEL)
    {
        num_level++;
    }
    return num_level;
}

int insert(SL* list, int val)
{
    node* NIL = list->header;
    int level_value;
    node* update[MAXLEVEL];
    node* temp;
    temp = list->header;
    int i;
    for(i=list->level;i>=1;i--)
    {
        while((temp->forward[i]->data)<val)
           temp = temp->forward[i];
        update[i] = temp; //holds the pointers of all the values lesser than
    }
    temp = temp->forward[1];
    if(temp->data == val) // The value is already present
        return 1;
    else
    {
        level_value = randomlevel();
        if(level_value>list->level)
        {
            for(i=list->level+1;i<=level_value;i++)
            {
                update[i]= NIL; // Assigning NIL values for the new pointers
            }
            list->level = level_value;
        }
 //creating new node to insert the data
temp = (node*)malloc(sizeof(node));
temp->data = val;
//allocating memory for forward pointers..
temp->forward = (node**)malloc(sizeof(node*)*(level_value+1));
for(i=1;i<=level_value;i++)
{
    temp->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = temp;
}
return 0;

}
}

int delete_node(SL* list,int key)
{

    node* NIL = list->header;
    node* update[MAXLEVEL];
    node* temp;
    temp = list->header;
    int i;
    for(i=list->level;i>=1;i--)
    {
        while(temp->forward[i]->data<key)
        {
            temp = temp->forward[i];
        }
        update[i] = temp;
    }
    temp = temp->forward[1];

    if(temp->data!=key) //element is not found
        return 1;

    /*correcting the forward pointers*/

    for(i=1;i<list->level;i++)
    {
        if(update[i]->forward[i]!=temp)
            break;
        update[i]->forward[i] = temp->forward[i];
    }
/*
    while(temp)
    {
        free(temp->forward);
        free(temp);
    }*/

    //Checking if the level needs to be decreased..
    while((list->level>1)&&(list->header->forward[list->level]==NIL))
        list->level--;

    return 0; // successfully deleted..

}

void display(SL* list)
{
    node *x = list->header;
            while (x && x->forward[1] != list->header)
            {
                printf("%d->",x->forward[1]->data);
                x = x->forward[1];
            }
            printf("NIL\n");
}


int main()
{
    FILE *myFile;
    int numbers[1000]={0},i=0;
    if ((myFile = fopen("randomnumbers.txt", "r"))==NULL)
    {
    printf("failed to open\n");
    return 1;
    }
    else
    while((fscanf(myFile,"%d",&numbers[i]))!=EOF) //scanf and check EOF
    i++;

   SL list;
   int ele;
   initialize(&list);
   for(i=0;i<1000;i++)
   {
       insert(&list,numbers[i]);
   }


    while(1)
    {
        int ch;
        printf("1.To Search an element\n2.To Insert an element\n3.To delete an element\n4.To display\n5.To exit\n");
        scanf("%d",&ch);
        switch(ch)
        {
        case 1:
            printf("Enter the element to be searched\n");
            scanf("%d",&ele);
            if(search_list(&list,ele))
                printf("Element found\n");
            else
                printf("Element not present\n");
            break;
        case 2:
            printf("Enter the element to be inserted\n");
            scanf("%d",&ele);
            if(insert(&list,ele))
                printf("Element already present\n");
            else
                printf("Element was inserted\n");
            break;
        case 3:
            printf("Enter the element to be deleted\n");
            scanf("%d",&ele);
            if(delete_node(&list,ele))
                 printf("Element not present\n");
            else
                 printf("Element found and deleted\n");
            break;
        case 4:
            display(&list);
            break;
        case 5:
            exit(0);
            break;
        }
    }

   return 0;
}

