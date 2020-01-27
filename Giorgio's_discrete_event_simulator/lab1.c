#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "PQ.h"
#include "FIFO.h"

	//globals
    int SEED, INIT_TIME,FIN_TIME,ARRIVE_MIN,ARRIVE_MAX,CPU_MIN,CPU_MAX,QUIT_PROB,DISK1_MIN,DISK1_MAX,DISK2_MIN,DISK2_MAX;
int main()/* Main Program */
{       

    //reading in a text file
	FILE *file;
	file = fopen("values.txt","r");
	char test[50];
	fscanf(file,"%s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d", test,&SEED,test,&INIT_TIME,test,&FIN_TIME,test, &ARRIVE_MIN, test, &ARRIVE_MAX,test,&QUIT_PROB,test,&CPU_MIN,test,&CPU_MAX,test,&DISK1_MIN,test,&DISK1_MAX,test,&DISK2_MIN,test,&DISK2_MAX);

    FILE *f;
    f = fopen("lab1.log", "a+"); //reading to a log file
    if(f==NULL){
        printf("something is wrong");
    }

    int i=1;

    srand(SEED);
    int currentT = INIT_TIME;

    int CPUt = 0; //total time for each server
    int DISK1t= 0;
    int DISK2t = 0;

    int cpuqele = 0; //keeps number of elements in each queue
    int d1qele = 0;
    int d2qele = 0;

    float thptCPU; 
	float thptD1;
	float thptD2;

	float utilCPU;
	float utilD1;
	float utilD2;

    int CPUj = 0;
    int d1j = 0;
    int d2j = 0; //number of jobs compleated for each component 

    int pqele_number=2;//number of elements in the priority queue
    int opn = 2;
    int job_time;

    int ttlruns = 0;  //used to fnd the average
    int ttlrunscpu = 0;
    int ttlrunsd1 = 0;
    int ttlrunsd2 = 0;

    int ttld1ele = 0;//for finding total of each to get average queue size
    int ttld2ele = 0;
    int ttlcpuele = 0;

    int maxcpuq = 0;
    int maxd1q = 0;
    int maxd2q = 0;

    float avecpuq;
    float aved1q;
    float aved2q;

    PriorityQ p;    //creating the queues
    CPUQ p1;
    DISK1Q p2;
    DISK2Q p3;

    int cputemp;
    int d1temp;
    int d2temp;

    int cpurespt;
    int d1respt;
    int d2respt;

    int cpumaxresp = 0; //max response time
    int d1maxresp = 0;
    int d2maxresp = 0;

    int ttldifcpu=0;
    int ttldifd1=0;
    int ttldifd2=0;

    int job_number = 2;

    fprintf(f, "SIMULATION HAS STARTED AT TIME: %d\n\n",currentT);
    PQinsert("job1",INIT_TIME,0); //adding starting elements
    fprintf(f, "job1 arrives at time: %d\n", INIT_TIME);

    PQinsert("simulation finished",FIN_TIME,0);

    while((pqele_number != 0) && (currentT<FIN_TIME)){
        switch(opn)
        {
        case 1:
            p.ele=(char *)malloc(sizeof(p.ele)); //creating memory for new element in the priority queue
            sprintf(p.ele,"Job%d",job_number); //create new job name
            p.pr = rand()%(ARRIVE_MAX-ARRIVE_MIN +1) + ARRIVE_MIN;
            currentT += p.pr;
            p.pr = currentT;
            PQinsert(p.ele,p.pr,0); //inserting into the priority queue
            fprintf(f, "%s arrives at time %d\n", p.ele, currentT); //writing to log file
            job_number++; 
            //display();
            opn = 2;
            break;
        case 2: 
            p=PQremove();
            switch(p.id){  //to see where to process event
                case 0: //id=0 the enters the cpu queue
                    p1.ele = (char *)malloc(sizeof(p1.ele)); //creating space for the element
                    p1.ele = p.ele;     //moving typr PQ into tupe FIFO
                    p1.time = p.pr;
                    p1.id = p.id;
                    CQinsert(p1.ele,p1.time,p1.id); //inserting into the CPUQ
                    fprintf(f, "%s arrives at the CPU at time %d\n", p.ele, currentT);
                    cputemp = currentT;
                    cpuqele++; 
                    break;
                case 1: //processing an event in the cpu
                    //CQdisplay();
                    job_time =CPU_MIN+rand()%(CPU_MAX-CPU_MIN +1);//creating a random job time
                    int quit = rand()%100;  //seeing if the job will quit after it leaves cpu
                    handler_CPU(job_time,QUIT_PROB,quit); //has the handler process the job
                    --cpuqele;
                    CPUj++;
                    CPUt += job_time;
                    currentT += job_time;  
                    p.pr += job_time;
                    fprintf(f, "%s finishes at the CPU at time %d\n", p.ele,currentT); //wrting to log file
                    ttldifcpu+=(currentT-cputemp);
                    cpurespt = (currentT-cputemp);
                    ttlrunscpu++;
                    if(quit<=QUIT_PROB){
                        fprintf(f, "%s quits after the CPU at time %d\n", p.ele,currentT);  //writing to log file
                        break;
                    }
                    	if(d1qele <= d2qele){
                        	p2.ele = (char *)malloc(sizeof(p2.ele)); //creating space for new element in the disk queue
                        	p2.ele = p.ele;
                        	p2.time = p.pr;
                        	p2.id = p.id;
                        	D1insert(p2.ele,p2.time,p2.id); //inserting into the disk
                        	d1qele++;
                        	fprintf(f, "%s arrives at the DISK1 queue at time %d\n", p.ele ,currentT); //writing to log file
                            d1temp = currentT;
                        	break;
                    	}else{
                        	p3.ele = (char *)malloc(sizeof(p3.ele));
                        	p3.ele = p.ele;
                        	p3.time = p.pr;
                        	p3.id = p.id;
                        	D2insert(p3.ele,p3.time,p3.id);
                        	d2qele++;
                        	fprintf(f, "%s arrives at the DISK2 queue at time %d\n", p.ele, currentT); //writing to log file
                            d2temp = currentT;
                        	break;
                    }

               	case 2: //processing an event in the disk1
                        job_time =DISK1_MIN+rand()%(DISK1_MAX-DISK1_MIN +1);
                        DISK1t+= job_time;
                        currentT += job_time;
                        p.pr += job_time;
                        handler_DISK1(job_time);
                        cpuqele++; 
                        --d1qele;
                        d1j++;
                        fprintf(f, "%s finishes at the DISK1 at time %d\n", p.ele,currentT); //writing to log file
                        ttldifd1+=(currentT-d1temp);
                        d1respt = (currentT-d1temp);
                        ttlrunsd1++;
                        break;

                case 3://processing an event in the disk2
                        job_time =DISK2_MIN+rand()%(DISK2_MAX-DISK2_MIN +1);
                        DISK2t += job_time;
                        currentT += job_time;
                        p.pr += job_time;
                        handler_DISK2(job_time);
                        cpuqele++; 
                        --d2qele;
                        d2j++;
                        fprintf(f, "%s finishes at the DISK2 at time %d\n", p.ele,currentT); //writing to log file
                        ttldifd2+=(currentT-d2temp);
                        d2respt = (currentT-d2temp);
                        ttlrunsd2++;
                        break;
            }
            opn = 1;
            ttlruns++;
            ttlcpuele += cpuqele;
  			ttld1ele += d1qele;
  			ttld2ele += d2qele;
  			if(maxcpuq<cpuqele)
  				maxcpuq = cpuqele;
  			if(maxd1q<d1qele)
  				maxd1q = d1qele;
  			if(maxd2q<d2qele) 
  				maxd2q = d2qele;
            if(cpumaxresp<cpurespt)
                cpumaxresp = cpurespt;
            if(d1maxresp<d1respt)
                d1maxresp = d1respt;
            if(d2maxresp<d2respt) 
                d2maxresp = d2respt;
            break;
    }
}
	fprintf(f, "simulations finish at time: %d\n", FIN_TIME);

	thptCPU = ((float)CPUj/CPUt)*100;
	thptD1 = ((float)d1j/DISK1t)*100;
	thptD2 = ((float)d2j/DISK2t)*100;

	avecpuq = (float) ttlcpuele / ttlruns;
    aved1q = (float) ttld1ele / ttlruns;
    aved2q = (float) ttld2ele / ttlruns;

	utilCPU = (float) CPUt /(FIN_TIME-INIT_TIME);
	utilD1 = (float) DISK1t /(FIN_TIME-INIT_TIME);
	utilD2 = (float) DISK2t /(FIN_TIME-INIT_TIME);

    float cpuaveresp = (float)ttldifcpu/ttlrunscpu;
    float d1averesp  = (float)ttldifd1/ttlrunsd1;
    float d2averesp = (float)ttldifd2/ttlrunsd2;

	fprintf(f," \n Utilization of Each Server Component \n CPU - %f\n Disk1 - %f\n Disk2 - %f\n",utilCPU,utilD1,utilD2);
	fprintf(f,"\n Max Queue sizes for Servers:\n\n CPU Max Queue Size - %d \n Disk1 Max Queue Size - %d\n Disk2 Max Queue Size - %d\n",maxcpuq,maxd1q,maxd2q);
	fprintf(f,"\n Average Queue sizes for Servers:\n\n CPU Average Queue Size - %f \n Disk1 Average Queue Size - %f\n Disk2 Average Queue Size - %f\n",avecpuq,aved1q,aved2q);
	fprintf(f,"\n Max Response Time for Each Server\n CPU - %d\n Disk1 - %d\n Disk2 - %d\n",cpumaxresp,d1maxresp,d2maxresp);
    fprintf(f,"\n Average Response Time for Each Server\n CPU - %f\n Disk1 - %f\n Disk2 - %f\n",cpuaveresp,d1averesp,d2averesp);
    fprintf(f," \n Throughput of each server\n CPU - %f %%\n Disk1 - %f %%\n Disk2 - %f %%\n",thptCPU,thptD1,thptD2);
    fclose(f);
    printf("\n CURRENT TIME: %d\n\n SIMULATION IS FINISHED \n\n terminating...\n\n to open log file type make openlog into terminal\n\n", currentT);

    return 0;
}

