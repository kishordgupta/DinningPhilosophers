#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#define NO_OF_PHILOSOPHERS 5
#define MAX_SIZE 80
#define EAT_TIME 9
 // 0 = thinking 1 = waiting 2= eating 3=done
 void printstatus(int id, int status)
 {
 printf(" |");
 int i=0;
    time_t mytime;
    mytime = time(NULL);
    char *p;
int len;


p = ctime(&mytime);
     printf("%.*s", (int)strlen(p)-1, p);
  
 for(i=0;i<id;i++)
 {
   printf("| -------- |");
 }
  if(status==0)
 {
 printf("| -THINK-- |");
 }
 else if(status==1)
 {
 printf("| -WAIT--- |");
 }
  else if(status==2)
 {
 printf("| -EAT---- |");
 }
  else if(status==3)
 {
 printf("| -DONE--- |");
 }
 
 for(i=id+1;i<NO_OF_PHILOSOPHERS;i++)
 {
   printf("| -------- |");
 }
  printf("\n");
 }
 
 
     void chopsticksserver(int a)
       { 
          printf ("Started choptick  %d\n", a);
          chopstickserverstart(a);
        }  
     void philosofersclient(int a)
       {  
	         printf ("Started philo  %d\n", a);
           philoclient(a);
        }  
int main (int argc, char **argv) {
          pid_t pidserver[NO_OF_PHILOSOPHERS];
          pid_t pidsphilo[NO_OF_PHILOSOPHERS];
          int i;
          int n = NO_OF_PHILOSOPHERS;

/* Start chopstick. */
for (i = 0; i < n; ++i) {
  if ((pidserver[i] = fork()) < 0) {
    perror("fork");
    abort();
  } else if (pidserver[i] == 0) {
    chopsticksserver(i);
    exit(0);
  }
}
sleep(2);
/* Start philo. */
for (i = 0; i < n; ++i) {
  if ((pidsphilo[i] = fork()) < 0) {
    perror("fork");
    abort();
  } else if (pidsphilo[i] == 0) {
    philosofersclient(i);
    exit(0);
  }
}

/* Wait for children to exit. */
int status;
 n=NO_OF_PHILOSOPHERS*2;
pid_t pid;
while (n > 0) {
  pid = wait(&status);
  //printf("%d Child with PID %ld exited with status 0x%x.\n", n,(long)pid, status);
  --n;  // TODO(pts): Remove pid from the pids array.
}
	
   
}
// port   5004 x0x 5000  x1x  5001  x2x  5002  x3x  5003  x4x 5004
// 0 = thinking 1 = waiting 2= eating 
int philoclient(int portno){
    int status = 0;
         int chopstickport1 ;
         int chopstickport2;
        if(portno>0){
                    chopstickport1 = 5000+portno;
                     chopstickport2=chopstickport1-1;
                    }
        if(portno==0)
            {chopstickport1 = 5004;
              chopstickport2=5000;
           }
          
           int thinkingtime = rand() % 9;
           printstatus(portno,status) ;
           sleep(thinkingtime);
           status = 1;
          printstatus(portno,status) ;
   int timing =0;        
while(1){

timing++;
int right =  philoclientconnectfork(chopstickport2,portno , 0);
int left =   philoclientconnectfork(chopstickport1,portno,0);

if(right==0&&left==0)
{
         status =2; 
          printstatus(portno,status) ;
           sleep(EAT_TIME);
          status=3;
           printstatus(portno,status) ;
     
             philoclientconnectfork(chopstickport2,portno , 2);
             philoclientconnectfork(chopstickport1,portno,2);
             break;
}
else if(right==0&&left==1)
{ status =1;
 printstatus(portno,status) ;
 philoclientconnectfork(chopstickport2,portno , 1);
}
else if(right==1&&left==0)
{   status =1;
  printstatus(portno,status) ;
  philoclientconnectfork(chopstickport1,portno , 1);
}
else
{
 status =1;
 printstatus(portno,status) ;
}
 int thinkingtime = rand() % 4;
 sleep(thinkingtime);
if(timing>10)
{
//if(thinkingtime%2==0)
// sleep(10);
 printstatus(portno,status) ;
 timing=0;
}
}
  return 0;
}
int philoclientconnectfork(int portno,int id , int status)
{
    int returnstatus =0;
  int clientSocket;
  char buffer[18];
  char buffercv[18];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portno);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  addr_size = sizeof serverAddr;
   /* open a TCP socket */
  if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("can't open stream socket");
     exit(1);
  }
  if(status==0){
  sprintf(buffer,"%s", "ACQUIRE");
  }
  if(status==1)
  {
  sprintf(buffer, "%s", "RELEASE");
  }
   if(status==2)
  {
  sprintf(buffer, "%s", "DONE");
  }
  /* connect to the server */    
  while(1){
  if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0) {
     
      perror(buffer);
    sleep(1);
  }
  else
  break;
}
 
  /* write a message to the server */
  write(clientSocket, buffer, sizeof(buffer));
    if(status==0){
   int len;
  /*---- Read the message from the server into the buffer ----*/
   len =  recv(clientSocket, buffercv, 1024, 0);
buffercv[len]=0;
  /*---- Print the received message ----*/
 
 
 if(strstr(buffercv, "Y") != NULL)
 { 
 returnstatus =0;
 }
 if(strstr(buffercv, "N") != NULL)
 {
 returnstatus =1;
 }}
 close(clientSocket);
 return returnstatus;
}
    
int chopstickserverstart(int portno)
{

      char busystatus[2] ={'N','\0'};
       char freestatus[2] ={'Y','\0'};
char status[2];
  sprintf(status, "%s", "Y");
 strcpy(status, freestatus);
   int sockfd, newsockfd, clilen;
  struct sockaddr_in cli_addr, serv_addr;
  int port;
  char string[MAX_SIZE];
  int len;
  

     port = portno+5000;
  
  /* open a TCP socket (an Internet stream socket) */
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
     perror("can't open stream socket");
     exit(1);
  }
  	
  /* bind the local address, so that the cliend can send to server */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  
  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
     perror("can't bind local address");
     exit(1);
  }
  else
  { perror("chopsticks initialize ");
  }
	
  /* listen to the socket */
  listen(sockfd, 5);
int i=0;
  for(;;) {
   if(i==2){//printf("\n %d  closing \n" , port);
   break;}
     /* wait for a connection from a client; this is an iterative server */
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
     if(newsockfd < 0) {
        perror("can't bind local address");
     }
   
     /* read a message from the client */
     len = read(newsockfd, string, MAX_SIZE); 
    
     string[len] = 0;
     if(strstr(string, "RELEASE") != NULL)
     { //printf("releaseing %d", portno);
      sprintf(status, "%s", "Y");
      
     }
    else if(strstr(string, "DONE") != NULL)
     { //printf("done %d", portno);
      sprintf(status, "%s", "Y");
    
       i++;
     }
     else{
     send(newsockfd,status, strlen(status),0);
      sprintf(status, "%s", "N");}
     
   
     close(newsockfd);
  }  
   close(sockfd);
}


