#include <stdio.h>
#include <stdlib.h>

struct node
{
int rows;
int columns;
int *grid;
struct node *next_step;
};

struct node *createInitialNode(FILE *input,int *numsteps)
{
    int rows;
    int cols;
    int x1;
    int y1;
    int i , j;
    int num=0;
    struct node *head = NULL;
    head = (struct node*)malloc(sizeof(struct node));
    fscanf(input, "%d", numsteps);
    fscanf(input, "%d %d", &rows, &cols);
    head->grid = (int*)malloc(cols * rows * sizeof(int));
    head->rows = rows;
    head-> columns = cols;
    head->next_step = NULL;
    // intializing the grid with all zeroes
    for(i = 0; i<rows; i++){
        for (j = 0; j<cols; j++){
            *(head->grid + cols*i+j) = 0;
        }
    }
    num = fscanf(input, "%d %d", &x1,&y1);
    while (num==2){
        *(head->grid + cols*x1+y1) = 1;
        num = fscanf(input, "%d %d", &x1,&y1);
    }
    return head;
    
}

void nextStep(struct node *begin){
    int sum;
    int a,b;
    int intialCols;
    intialCols = begin->columns;
    struct node* next = NULL;
    struct node *previous = begin;
    next = (struct node*)malloc(sizeof(struct node));
    next -> columns = begin->columns;
    next -> rows = begin->rows;
    next -> grid = (int*)malloc(next->rows*next->columns*sizeof(int));
    while(previous->next_step!=NULL){
        previous = previous->next_step;
    }
    previous->next_step = next;
     for (a = 0; a<begin->rows; a++){
         for (b = 0; b<begin->columns; b++){
             sum = neighbors(previous,a, b);
             if (*(previous->grid+intialCols*a+b)==1 && sum < 2)
             {
                 *(next->grid+intialCols*a+b) = 0;}
                 else if (*(previous->grid+intialCols*a+b)==1 && (sum == 2 || sum == 3 ))
                 {
                     *(next->grid+intialCols*a+b) = 1;
                 }
                 else if (*(previous->grid+intialCols*a+b) == 1 && (sum > 3) )
                 {
                     *(next->grid+intialCols*a+b) = 0;
                 }
                 else if (*(previous->grid+intialCols*a+b) == 0 && (sum ==3))
                 {
                     *(next->grid+intialCols*a+b) = 1;
                 }
         }
     }
     return ;
}
            
         
     


void printList(struct node *begin){
    while (begin!=NULL){
        printGrid(begin->grid, begin->rows, begin->columns);
        begin= begin->next_step;
        printf("\n");
    }
    return;
}

int neighbors(struct node *nodeptr,int a,int b){
    int life = 0;
    int rows;
    int cols;
    rows = nodeptr->rows;
    cols = nodeptr->columns;
    int size =0;
    size = rows*cols;
    int *Arr = nodeptr->grid;
    if (a-1>=0 && b-1>=0)
    {
        if (*(Arr+cols*(a-1)+ b-1) ==1)
        life++;
    }
    if (a-1>=0)
    {
       if (*(Arr+cols*(a-1)+ b) ==1)
       life++;
    }
    if (a-1>=0 && b+1 < cols)
    {
       if (*(Arr+cols*(a-1)+ b+1) ==1)
       life++;
    }
    if (b-1>=0)
    {
        if (*(Arr+cols*a+ b) ==1)
        life++;
    }
    if (a+1 < rows && b-1 >=0)
    {
        if (*(Arr+cols*(a+1)+ b-1) ==1)
        life++;
    }
    if (a+1 < rows)
    {
        if (*(Arr+cols*(a+1)+ b) ==1)
        life++;
    }
    if (a+1 < rows && b+1 < cols)
    {
        if (*(Arr+cols*(a+1)+ b+1) ==1)
        life++;
    }
    if (b+1 < cols )
    {
        if (*(Arr+cols*a+ b+1) ==1)
    life++;
    }
    return life;
}

void printGrid(int *anArray,int rows,int columns){
    for (int i = 0; i<rows; i++){
        for (int j=0; j<columns; j++){
            printf("%d ", *(anArray+columns*i+j));
        }
        printf("\n");
    }
    return;
}

int main(void)
{
     printf("Welcome to Conway's game of Life\nPlease enter the filename\n");
char fileName[30];
    scanf ("%[^\n]%*c", fileName); //Reading filename
FILE *inFile;
    inFile = fopen(fileName,"r"); //Opening file
if(inFile == NULL){
    printf("ERROR opening the given filename %s\n",fileName);
    return -1;
}
int numsteps;
struct node* head = createInitialNode(inFile, &numsteps);// passing numsteps address becouse we need in 65
for (int step = 1; step<=numsteps; step++)
   nextStep(head);
   printList(head);

return 0;
}
