#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100000           /* Size of Queue */
int f=0,r=-1;       /* Global declarations */
typedef struct PRQ
{
    char *ele;
    int pr;
    int id; //0=if has to go into CPU queue 1= if it has to go to disk

}PriorityQ;

PriorityQ PQ[SIZE];
 
void PQinsert(char *elem, int pre, int id) //inserting into the priority queue
{
        int i;      
        i=r;
        ++r;

        while(PQ[i].pr >= pre && i >= f) //finding location fo new element
        {
            PQ[i+1]=PQ[i];//pushing all elements back one to find location of new element
            i--;        
        }
        PQ[i+1].ele=elem;//putting new element in open position
        PQ[i+1].pr=pre; 
        PQ[i+1].id =id;
    }

 
PriorityQ PQremove()
{                      /* Function for Delete operation */
    PriorityQ p;
    p=PQ[f];
    f=f+1;  //number of elements which have been removed, pointer to starting value in queue
    return(p);
}


void display()
{       
             /* Function to display status of Queue */
        int i;
        printf("PRIORITY QUEUE: Front->");
        for(i=f;i<=r;i++)
            printf("[%s,time:%d] ",PQ[i].ele,PQ[i].pr);
        printf("<-Rear\n");
    
}
 
