#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#define CUSTOMERS_NUMBER 5
#define RESOURCES_NUMBER 3

int total[RESOURCES_NUMBER]={0};
int available[RESOURCES_NUMBER];
int maximum[CUSTOMERS_NUMBER][RESOURCES_NUMBER];
int allocation[CUSTOMERS_NUMBER][RESOURCES_NUMBER]={0};
int need[CUSTOMERS_NUMBER][RESOURCES_NUMBER];
int Bavailable[RESOURCES_NUMBER];
int Bmaximum[CUSTOMERS_NUMBER][RESOURCES_NUMBER];
int Ballocation[CUSTOMERS_NUMBER][RESOURCES_NUMBER];
int Bneed[CUSTOMERS_NUMBER][RESOURCES_NUMBER];
int Finish[CUSTOMERS_NUMBER]={0};

int release_resources(int cus_num);
int request_resources(int cus_num, int request[]);
void *thread_func(void* cus_numt);
int bankerAlgorithm(int cus_num,int request[]);
void printState();
pthread_mutex_t mutex1;
char string[RESOURCES_NUMBER*2]={0};
int safeSequence[CUSTOMERS_NUMBER]={0};


int main(int ar, const char * av[]) {
    
  //********************* initialize the matrices ****************
    for(int i=0 ;i<ar-1;i++){
        available[i]=atoi(av[i+1]);
        total[i]=available[i];      // in the begining available resources equal to total resources
    }
    for(int i=0;i< RESOURCES_NUMBER;i++){
        for(int j=0;j<CUSTOMERS_NUMBER;j++){
            maximum[j][i]=rand()%(total[i]+1);     //maximum should less than total
            need[j][i]=maximum[j][i];    // need=maximum-allocation  (allocation=0)
        
        }
    
    }
    //********************* initialize the matrices ****************
    
    //****************** print the state of the process *********************
    
    for (int i=0; i<RESOURCES_NUMBER; i++) {
        string[i*2]=i+'A';
        string[i*2+1]=' ';
    }
    printf("Total system resources are:\n");
    printf("%sn",string);
    for (int i=0; i<RESOURCES_NUMBER; i++) {
        printf("%d ",total[i]);
    }
    printf("nnProcesses (maximum resources):\n");
    printf("   %s\n",string);
    for(int i=0; i<CUSTOMERS_NUMBER;i++){
        printf("PROCESS%d ",i+1);
        for(int j=0;j<RESOURCES_NUMBER;j++){
            printf("%d ",maximum[i][j]);
        }
        printf("\n");
    }

    printState();
   //****************** print the state of the process *********************
    
    
   pthread_mutex_init(&mutex1, NULL);    //initialize mutex
   pthread_t a1,a2,a3,a4,a5;
   int a=0,b=1,c=2,d=3,e=4;
   pthread_create(&a1,NULL,thread_func,&a);
   pthread_create(&a2,NULL,thread_func,&b);
   pthread_create(&a3,NULL,thread_func,&c);
   pthread_create(&a4,NULL,thread_func,&d);
   pthread_create(&a5,NULL,thread_func,&e);     //create 5 threads
   char *returnV;
    
   pthread_join(a1,(void**)&returnV);
   pthread_join(a2,(void**)&returnV);
   pthread_join(a3,(void**)&returnV);
   pthread_join(a4,(void**)&returnV);
   pthread_join(a5,(void**)&returnV); // wait for all the 5 threads to terminate

   
    
    
    return 0;
}


void *thread_func(void* Tcus_num){
    
    
    int *c=(int*)Tcus_num;
    int cus_num= *c;
    
    int requestSum=0;
    
    while(!Finish[cus_num]){   // the whilie loop stops when the thread has finished and its need becomes zero
        requestSum=0;
        int request[RESOURCES_NUMBER]={0};
        
        for(int i=0;i<RESOURCES_NUMBER;i++){
           
            request[i]=rand()%(need[cus_num][i]+1); // generate a request below its need randomly
            requestSum=requestSum+request[i];
        }
        
        if(requestsum!=0)  // to make sure it doesn't requst for 0 reaources
            while(request_resources(cus_num,request)==-1); // only when the request has been granted succesfully
                                                                // will the loop terminates, otherwise it will keep
                                                                // making the same request.
        
        
    }
    
    
   
    
    return 0;
}



int request_resources(int cus_num, int request[]){
    
    
    
    int returnValue=-1;
    
    // since i want the process to print the result continuously and also run without race condition, i make the part
    // involing printing and accesing globale variable critical.
    //****************** critical section*********************
    pthread_mutex_lock(&mutex1);
    
    printf("\nPROCESS%d requests for ",cus_num+1);
    for(int i=0;i<RESOURCES_NUMBER;i++){
        printf("%d ",request[i]);
    }
    printf("\n");


        for(int i=0;i<RESOURCES_NUMBER;i++){   // to check whether request<= availabe
                                                  // if it is not, then it will return -1
            if(request[i]>available[i]){
                printf("PROCESS%d is waiting for the reaources...n",cus_num+1);

                pthread_mutex_unlock(&mutex1);   // before the thread return, it has to unlock the mutex
                return -1;
            }
            
        }
        
   

    returnValue=bankerAlgorithm(cus_num,request);  // execute banker's algorithm
    
    if(returnValue==0){ 
        int need_is_zero=1;
        printf("a safe sequence is found: ");
        for(int i=0;i<CUSTOMERS_NUMBER;i++){
            printf("PROCESS%d ",safeSequence[i]+1 );
        
        }
        printf("\n PROCESS%d's request has been granted",customer_num+1);
       
        for(int j=0;j<RESOURCES_NUMBER;j++){ // give the resources to the theread
            allocation[cus_num][j]=allocation[cus_num][j]+request[j];
            available[j]=available[j]-request[j];
            need[cus_num][j]=need[cus_num][j]-request[j];
            if(need[cus_num][j]!=0){// to check if need is zero
                need_is_zero=0;
            }
        
        }
        
        if(need_is_zero){
            
            Finish[cus_num]=1; // if need is zero, mark the thread "finish"
            release_resources(cus_num); // release resources when a thread finishes
            
        }
        
        printState();
        
    }
    else{
        printf("cannot find a safe sequencen");
    
    }
   
    pthread_mutex_unlock(&mutex1); // unlock the mutex
    return returnValue;
}


int release_resources(int cus_num){
    
    
    printf("PROCESS%d releases all the resources \n",cus_num+1);
    for(int j=0;j<RESOURCES_NUMBER;j++){
        available[j]=available[j]+allocation[cus_num][j]; // release the resources
        allocation[cus_num][j]=0;
        
        
    }
    
    
    
    return 0;
}
int bankerAlgorithm(int cus_num,int request[]){
    int finish[CUSTOMERS_NUMBER]={0};
    
    for(int i=0;i<RESOURCES_NUMBER;i++){ // copy the matrices
        Bavailable[i]=available[i];
        for(int j=0;j<CUSTOMERS_NUMBER;j++){
            Ballocation[j][i]=allocation[j][i];
            
            Bmaximum[j][i]=maximum[j][i];
         
            Bneed[j][i]=need[j][i];
        
        
        }
    }
    
        for(int i=0;i<RESOURCES_NUMBER;i++){ // pretend to give the resource to the thread
            Bavailable[i]=Bavailable[i]-request[i];
            Ballocation[cus_num][i]=Ballocation[cuS_num][i]+request[i];
            Bneed[cus_num][i]=Bneed[cus_num][i]-request[i];
        }
    
   

    
//*************************safety Algorithm***************************
    int count=0;
    while(1){
        
        int I=-1;
        
        for(int i=0;i<CUSTOMERS_NUMBER;i++){ // to find a thread that its need is less than or equal to available.
            int nLessThanA=1;
            for(int j=0; j<RESOURCES_NUMBER;j++){
                if(Bneed[i][j]>Bavailable[j] || finish[i]==1){
                    nLessThanA=0;
                    break;
                }
                
            }
            if(nLessThanA){ // if the thread is found, record its thread number
                I=i;
                break;
            }
            
        }
        
        if(I!=-1){
            safeSequence[count]=I; // record the sequence
            count++;
            finish[I]=1; // mark the thread "finish"
            for(int k=0;k<RESOURCES_NUMBER;k++){  // pretend to give the reaource to thread
                Bavailable[k]=Bavailable[k]+Ballocation[I][k];
            }
            
            
        }
        else{ // if can not find any thread that its need is less than or equal to available.
            
            for(int i=0;i<CUSTOMERS_NUMBER;i++){
                if(finish[i]==0){ // if there is any thread hasn't been found, that means it can't find a safe sequence
                    return -1;
                }
                
            }
            return 0;  // all the threads have been found
            
            
        }

    
    
    }
    
//*************************safety Algorithm***************************
    
    
}


void printState(){
    
    
    printf("Processes (currently allocated resources):\n");
    printf("   %s\n",string);
    
    for(int i=0; i<CUSTOMERS_NUMBER;i++){
        printf("P%d ",i+1);
        for(int j=0;j<RESOURCES_NUMBER;j++){
            printf("%d ",allocation[i][j]);
        }
        printf("\n");
    }
    printf("Processes (possibly needed resources):\n");
    printf("   %s\n",string);
    for(int i=0; i<CUSTOMERS_NUMBER;i++){
        printf("P%d ",i+1);
        for(int j=0;j<RESOURCES_NUMBER;j++){
            printf("%d ",need[i][j]);
        }
        
        printf("\n");
    }
    printf("Available system resources are:\n");
    printf("%s\n",string);
    
    for (int i=0; i<RESOURCES_NUMBER; i++) {
        printf("%d ",available[i]);
    }
    
    printf("\n");
    
    
    
}
