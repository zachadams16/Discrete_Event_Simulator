
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct FIFO
{
    char *ele;
    int time;
    int id;

};

typedef struct FIFO CPUQ;  //all FIFO structures
typedef struct FIFO DISK1Q;
typedef struct FIFO DISK2Q;

void D1insert();
void D2insert();
void PQinsert();

CPUQ CQ[SIZE];
int queEle=0; //number of elements in CPUQ
int rem = 0; //# of removed elements in CPU, also location of first element in queue
CPUQ CQremove();

DISK1Q D1Q[SIZE];
int disk1Ele=0; //number of elements in D1Q
int remD1 = 0; //# of removed elements in D1, also location of first element in queue
DISK1Q D1Qremove();

DISK2Q D2Q[SIZE];
int disk2Ele=0;// number of elements in D2Q
int remD2 = 0; //# of removed elements in D2, also location of first element in queue
DISK2Q D2Qremove();

void CQinsert(char *elem, int pre, int id)
{
        int i=0;       /* Function for Insert operation */
        while(CQ[i].ele != NULL) /* Find location for new elem */
        {
   			i++; 
        }
        CQ[i].ele=elem;//putting new element in open position
        CQ[i].time=pre;
        CQ[i].id = id;
        PQinsert(CQ[i].ele,CQ[i].time,1);
        queEle++; 
}

void handler_CPU(int job_time, int quitprob, int quit){
    CPUQ p; //creating a new queue element
	CQ[rem].time += job_time;
    p=CQremove(); //removing from the CPU
    CQ[rem].id = 1; //changing its ID to show its been in CPU
    if(quit<quitprob){
       CQ[0] = p; //if it quits
    }
}

void CQdisplay()
{                  /* Function to display status of Queue */
    int i;
    printf("CPU QUEUE : Front->");
    for(i=rem;i<queEle;i++)
        printf("[%s,time:%d] ",CQ[i].ele,CQ[i].time);
    printf("<-Rear\n");
    
}

CPUQ CQremove(){ //removing an element. pushes rem up so that it points to the location of the first element in the queue
    int i = 0;
    CPUQ p;
    p=CQ[rem];
    rem=rem+1;
    return(p);
}


void D1insert(char *elem, int pre, int id)
{
        int i=0;       
        while(D1Q[i].ele !=NULL) //Find location for new elem 
        {
            i++; 
        }
        D1Q[i].ele=elem;//putting new element in open position
        D1Q[i].time=pre;
        D1Q[i].id = id;
        PQinsert(D1Q[i].ele,D1Q[i].time,2);
        disk1Ele++; 
}

void handler_DISK1(int job_time){ //processes event as it enters the disk
    DISK1Q p;
    int jobT =job_time;
    D1Q[rem].time += jobT;
    p=D1Qremove();
    D1Q[rem].id = 0;
    PQinsert(p.ele,p.time,0);//id is set to 0 so it will go back through cpu
}

void D1Qdisplay()
{                  /* Function to display status of Queue */
    int i;
    printf("DISK1 QUEUE : Front->");
    for(i=remD1;i<disk1Ele;i++)
        printf("[%s,time:%d] ",D1Q[i].ele,D1Q[i].time);
    printf("<-Rear\n");
    
}

DISK1Q D1Qremove(){ //uses same remove method as CPU
    int i = 0;
    DISK1Q p;
    p=D1Q[remD1];
    remD1=remD1+1;
    return(p);
}


void D2insert(char *elem, int pre, int id) //uses same insert function as disk1
{
        int i=0;       
        while(D2Q[i].ele !=NULL) 
        {
            i++; 
        }
        D2Q[i].ele=elem;
        D2Q[i].time=pre;
        D2Q[i].id = id;
        PQinsert(D2Q[i].ele,D2Q[i].time,3);
        disk2Ele++; 
}

void handler_DISK2(int job_time){ //same as disk1
    DISK2Q p;
    int jobT =job_time;
    D2Q[remD2].time += jobT;
    p=D2Qremove();
    D2Q[rem].id = 0;
    PQinsert(p.ele,p.time,0);
}

void D2Qdisplay()
{                  /* Function to display status of Queue */
    int i;
    printf("DISK2 QUEUE : Front->");
    for(i=remD2;i<disk2Ele;i++)
        printf("[%s,time:%d] ",D2Q[i].ele,D2Q[i].time);
    printf("<-Rear\n");
    
}

DISK2Q D2Qremove(){ //same remove function
    int i = 0;
    DISK2Q p;
    p=D2Q[remD2];
    remD2=remD2+1;
    return(p);
}


