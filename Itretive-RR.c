#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

#define NEW 0
#define READY 1
#define WAITING 2
#define RUNNING 3
#define EXIT 4

int presenttime=0,time_in_exec=0,EXITED=0,TQ,Context_Switch_Permitted=1,n;
int  itr=1,ps=0,noe=0;
struct process 
{
	int pid;
	int state;
	int timeleft;
	int at;
	int wt,tat,ct,execution_time;
	struct process *prev;
	struct process *next;
} *process_array;
int chpp;
struct Queue
{
	struct process *front ,*rear;
}*Ready;

void enqueue(struct process *p)
{
       if(Ready->front==NULL)
      { 
         p->prev=NULL;

         p->next=NULL;
         Ready->front=p;           
         Ready->rear=p;
         return;
      }
     p->prev=Ready->rear;
     Ready->rear->next=p;
     Ready->rear=p;
}
struct process *  deQueue()
{
     if(Ready->front==NULL)
     {
	     return NULL;
     }
     struct process * temp=Ready->front;
     Ready->front=Ready->front->next;
     temp->next=NULL;
     if(Ready->front==NULL)
     {
	     Ready->rear=NULL;
     }
     return temp;
}
void checkqueue()
{
int count=0;
if(itr==1)
   {
        noe=0;
    }
           for(int i=0;i<n;i++)
	   {
                   
		   if(process_array[i].state == NEW && presenttime>=process_array[i].at)
		   {      
			   
			   enqueue(&process_array[i]);
			   process_array[i].state=READY;
			   
                    }
                  if(process_array[i].state==EXIT)
                  {
                    count++;
                     if(itr==1)
                     {
                          noe++;
                      }
			}
	   }
	   if(count==n)
	   {
		  EXITED=1; 
		  
	   }

}

int main()
{


Ready =(struct Queue*) malloc(sizeof(struct Queue));
	printf("Please enter No of processes to schedule");
	scanf("%d",&n);
	process_array=(struct process *)malloc(sizeof(struct process)*n);
	for(int i=0;i<n;i++)
	{
             printf("\n\n Enter Process Id For %d Process",(i+1));
	     scanf("%d",&(process_array[i].pid));
	     printf("\n Enter arrival time For %d Process",(i+1));
	     scanf("%d",&(process_array[i].at));
	     printf("\n Enter Execution time For %d Process",(i+1));
	     scanf("%d",&(process_array[i].timeleft)); 
	     
            process_array[i].execution_time=process_array[i].timeleft;
	    process_array[i].state=NEW;
	        
	}
 struct process key; 
    int i,j;
    for (i = 1; i < n; i++) { 
        key = process_array[i]; 
        j = i - 1; 
  
        while (j >= 0 && process_array[j].at > key.at) { 
            process_array[j + 1] = process_array[j]; 
            j = j - 1; 
        } 
        process_array[j + 1] = key; 
    } 

struct process *runningprocess;
while(1)
{
        checkqueue();
        if(EXITED==1 || itr>2)
        {

                    break;      
        }
        if(itr==1)
       {
            TQ=6;
        }
        if(itr==2)
      {
            TQ=10;
       }
        if(itr==1&&ps==n&&Context_Switch_Permitted==1)
       {  

               itr++;
               ps=0;
        }
        if(itr==2&&ps==(n-noe)&&Context_Switch_Permitted==1)
        {
               itr++;
               ps=0;
                break; 
         }
        if(Ready->front!=NULL && Context_Switch_Permitted==1)
	{
             time_in_exec=1;
             ps++;
	     runningprocess=deQueue();
             runningprocess->state=RUNNING;
             runningprocess->timeleft--;
	     presenttime++;
             if(time_in_exec==runningprocess->execution_time)
             {
                  Context_Switch_Permitted=1;
                  runningprocess->state=EXIT;
                  runningprocess->ct=presenttime;
                  runningprocess->tat=presenttime-runningprocess->at;
                  runningprocess->wt=runningprocess->tat-runningprocess->execution_time;
             }
             else
            {
               Context_Switch_Permitted=0;
              }
	}
        else if(Context_Switch_Permitted==0&&runningprocess!=NULL && runningprocess->state==RUNNING )
	{
             if(runningprocess->timeleft==0)
             {
                  Context_Switch_Permitted=1;
                  runningprocess->state=EXIT;
                  runningprocess->ct=presenttime;
                  runningprocess->tat=presenttime-(runningprocess->at);
                  runningprocess->wt=(runningprocess->tat)-(runningprocess->execution_time);
                  continue;
             }
             else if(time_in_exec==TQ)
             {
                runningprocess->state=READY;
                enqueue(runningprocess);
                Context_Switch_Permitted=1;
                continue;
              }      
             time_in_exec++;
             runningprocess->timeleft--;
            presenttime++;

             if(runningprocess->timeleft==0)
             {
                  Context_Switch_Permitted=1;
                  runningprocess->state=EXIT;
                  runningprocess->ct=presenttime;
                  runningprocess->tat=presenttime-(runningprocess->at);
                  runningprocess->wt=(runningprocess->tat)-(runningprocess->execution_time);
             }            
             else if(time_in_exec==TQ)
             {
                runningprocess->state=READY;
                enqueue(runningprocess);
                Context_Switch_Permitted=1;
              }
            else
          { 
              Context_Switch_Permitted=0;
	  }
    
          
             
          
	}
else
{
presenttime++;
}

}
int sumwt=0,sumtat=0,n1=0;
for(int i=0;i<n;i++)
	{ if(process_array[i].state==EXIT)
          {
           printf("\n\nprocess pid=%d\nct=%d\ntat=%d\nwt=%d",process_array[i].pid,process_array[i].ct,process_array[i].tat,process_array[i].wt);
           sumwt+=process_array[i].wt;
           sumtat+=process_array[i].tat;
           n1++;
          }
          else
         {
              printf("\n\nprocess pid=%d requires %d more time to complete\n",process_array[i].pid,process_array[i].timeleft);    
           }           
	}
printf("\n\n Avergae TAT=%f \n Average WT=%f",(sumtat/(n1*1.0)),(sumwt/(n1*1.0)));
}
