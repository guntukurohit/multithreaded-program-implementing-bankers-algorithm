#include <stdio.h>
int x;
bool y=false; 
int current[8][8], max_claim[8][8], available[8], need[8][8],pro[8];
int allocation[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int max_resources[8], running[8];
int counter = 0, i, j, exec, resources, processes;
 //...................
 
int main()
{
	printf("\nEnter number of processes: ");
    	scanf("%d", &processes);
 
    	for (i = 0; i < processes; i++) 
	{
        	running[i] = 1;
        	counter++;
    	}
    	x=counter; //....................................
    	for(i=0;i<x;i++)
			{
    		pro[i]=false;
			}
 
    	printf("\nEnter number of resources: ");
    	scanf("%d", &resources);
 
    	printf("\nEnter Claim Vector:");
    	for (i = 0; i < resources; i++) 
	{ 
	        scanf("%d", &max_resources[i]);
    	}
 
   	printf("\nEnter Allocated Resource Table:\n");
    	for (i = 0; i < processes; i++) 
	{
	        for(j = 0; j < resources; j++) 
		{
  			scanf("%d", &current[i][j]);
        	}
    	}
 
    	printf("\nEnter Maximum Claim Table:\n");
    	for (i = 0; i < processes; i++) 
	{
        	for(j = 0; j < resources; j++) 
		{
            		scanf("%d", &max_claim[i][j]);
        	}
    	}
 
	printf("\nThe Claim Vector is: ");
    	for (i = 0; i < resources; i++) 
	{
	        printf("\t%d", max_resources[i]);
	}
 
    	printf("\nThe Allocated Resource Table:\n");
    	for (i = 0; i < processes; i++) 
	{
	        for (j = 0; j < resources; j++) 
		{
            		printf("\t%d", current[i][j]);
        	}
		printf("\n");
    	}
 
    	printf("\nThe Maximum Claim Table:\n");
    	for (i = 0; i < processes; i++) 
	{
        	for (j = 0; j < resources; j++) 
		{
		        printf("\t%d", max_claim[i][j]);
        	}
        	printf("\n");
    	}
 
    	for (i = 0; i < processes; i++) 
	{
        	for (j = 0; j < resources; j++) 
		{
            		allocation[j] += current[i][j];
        	}
    	}
 
    	printf("\nAllocated resources:");
    	for (i = 0; i < resources; i++) 
	{
        	printf("\t%d", allocation[i]);
    	}
 
    	for (i = 0; i < resources; i++) 
	{
	        available[i] = max_resources[i] - allocation[i];
	}
 
    	printf("\nAvailable resources:");
    	for (i = 0; i < resources; i++) 
	{
        	printf("\t%d", available[i]);
    	}
    	printf("\n");
 
    	while (x != 0) 
	{  
        	for (i = 0; i < processes; i++) 
		{
            		if (running[i]) 
			{
                		exec = 1;
                		for (j = 0; j < resources; j++) 
				{
					need[i][j]=max_claim[i][j] - current[i][j];
                    			if ( need[i][j] > available[j]) 
					{
                        			exec = 0;
                        			break;
                    			}
                		}
                		if (exec) 
				{
                    			printf("\nProcess%d is executing\n", i + 1);
                    			running[i] = 0;
                    			x--; 
                    			//pro[i]=true;
													y=true;
 
                    			for (j = 0; j < resources; j++) 
					{
                        			available[j] += current[i][j];
                    			}
			                break;
                		}
            		}
        	}
        	if (y!=true)
		{
            		printf("\nThe processes are in unsafe state.\n");
            		break;
        	} 
		else 
		{
            		printf("\nThe process is in safe state");
            		printf("\nAvailablility:");
 
            		for (i = 0; i < resources; i++) 
			{
                		printf("\t%d", available[i]);
            		}
 
		        printf("\n");
        	}
    	}
    	return 0;
}


