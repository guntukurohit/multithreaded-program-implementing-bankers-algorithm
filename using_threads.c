#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>


int no_of_process ,no_of_resources;
int p_number= 0;
void*process(void *arg);
pthread_mutex_t lock;              //to lock the threads
pthread_cond_t condition;


int main()
{ 

	// input for no.of process from the user
	printf("Enter total no.of processes - ");
	scanf("%d",&no_of_process); //taking input of process number
	
	// input for no.of resources from the users
	printf("Enter no.of resources - ");
	scanf("%d",&no_of_resources);  // input of no of resources
	
	// input for the  availability of instances witheach resource;
	int current_resources[no_of_resources];
	printf("Enter available instances with each resource  - ");
	for(int i = 0;i<no_of_resources;i++)
	{
		scanf("%d",&current_resources[i]);
	}
	
	int allocated_resources[no_of_process][no_of_resources];
	// input for allocated resources for each process
	for(int i = 0;i<no_of_process;i++)
	{
        printf("Enter allocated resources for process %d",(i+1)," - ");
		for(int j = 0;j<no_of_resources;j++)
		{
			scanf("%d",&allocated_resources[i][j]);
		}
		printf("\n");
	}
	 
	printf("\n\n\n\n\n");
	
	int max_req[no_of_process][no_of_resources];
	//input for max_requirement of resources for each process
	for(int i = 0;i<no_of_process;i++)
	{
		printf("Enter maximum resources for process ",(i+1)," - ");
		for(int j = 0;j<no_of_resources;j++)
		{
			scanf("%d",&max_req[i][j]);
		}
		printf("\n");
	}
	
	int remaining_need[no_of_process][no_of_resources]; //matrix for remaining need of resources for each process;
	
	for(int i = 0;i<no_of_process;i++)
	{
		for(int j = 0;j < no_of_resources;j++)
		{
			remaining_need[i][j] = max_req[i][j] - allocated_resources[i][j];
		}
	}
	
	//array to store safe sequence
	int safe_seq[no_of_process];
	for(int i = 0;i<no_of_process;i++)
	{
		safe_seq[i] = -1;
	}
	bool s;
	//cout<<"\nFINAL OUTPUT - "<<s
	
	int temporary_resources[no_of_resources];
	for(int i = 0;i<no_of_resources;i++)
	{
		temporary_resources[i] = current_resources[i];
	}
	
	bool finish[no_of_process]; //to store finish status of processes;
	for(int i = 0;i<no_of_process;i++)
	{
		finish[i] = false;  //initially all processes are not finished
	}
	
	int no_finished = 0; //no_of finished processes are zero initially
	
	while(no_finished <no_of_process)
	{
		bool safe = false;
		
		for(int i = 0;i<no_of_process;i++)   //running loop for each process to check for finish status
		{
			if(!finish[i])
			{
				bool possible = true;
				for(int j = 0;j<no_of_resources;j++)
				{
					if(remaining_need[i][j] > temporary_resources[j])  //checking need and availability of resources for each process
					{
						possible = false;
						break;
					}
			    }
			    
			    if(possible)
			    {
			    	for(int j = 0;j<no_of_resources;j++)
			    	{
			    		temporary_resources[j] += allocated_resources[i][j];
					}
					safe_seq[no_finished] = i;
			    	finish[i] = true;              //updating the finish status
			    	no_finished++;
			    	safe = true;
				}
			}
		}
		
		if(!safe)
		{
			for(int j = 0;j<no_of_process;j++)
			{
				safe_seq[j] = -1;            
				s = false;      //no safe sequence found after all process checks.
				printf("SAFE SEQUENCE NOT FOUND");
				sleep(3);
				exit(0);
			}
		}
	}
	s = true;
	
	if(s == true){
	printf("SAFE SEQUENCE IS : ");
	for(int i = 0;i<no_of_process;i++)
	{
		
		printf("%d",safe_seq[i] + 1,"\n\n");
	}
}

    //creating and running threads
    pthread_t processes[no_of_process];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

	int processNumber[no_of_process];
	for(int i=0; i<no_of_process; i++) processNumber[i] = i;{

        for(int i=0; i<no_of_process; i++)
               { pthread_create(&processes[i], &attr, process,(void *)(&processNumber[i]));
                 
			   }

        for(int i=0; i<no_of_process; i++)
               { pthread_join(processes[i], NULL);}
               
        for(int i = 0;i<no_of_process;i++)
		{
			printf("\n\n\n\n");
			printf("----------PROCESS %d ",safe_seq[i]+1,"\n\n");
			int k = safe_seq[i];
			printf("Maximum requirement - ");
			for(int j = 0;j < no_of_resources;j++)
			{
			printf("%d",max_req[k][j]," ");
			}
			printf("\n");
			printf("Allocated resources        - ");
			for(int j = 0;j < no_of_resources;j++)
			{
			printf("%d",allocated_resources[k][j]," ");
			}
			printf("\n\n");
			printf("Needed              - ");
			for(int j = 0;j < no_of_resources;j++)
			{
			printf("%d",remaining_need[k][j]," ");
			}
			printf("\n");
			printf("\n\n\n");
			
			printf("Available for allocation\n");
			sleep(2);
			printf("Resource Allocated\n");
			sleep(2);
			printf("Process started\n");
			sleep(1);
			printf("Process terminated and resources released");
			printf("\nCurrent Available Resources - ");
			
			for(int j = 0;j < no_of_resources;j++)
			{
				current_resources[j] =  current_resources[j] + allocated_resources[k][j];
				printf("%d",current_resources[j]," ");
			}
			printf("\n\n\n\n");
			
			
		   }   
}
	return 0;
}
//thread process to be performed to use mutex lock
void *process(void*arg)
{
	int p = *((int *) arg);
	//resources locked
	pthread_mutex_lock(&lock);
	
	printf("\nThread created successfully\n");
	
}


