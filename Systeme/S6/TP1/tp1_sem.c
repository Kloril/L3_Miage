/*--------------------------------------------------
   Version 0 d'un �change SHM System V entre deux fils.
    Les semaphores utilis�s sont POSIX !

    Author : GM
 ----------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <time.h>
#define BUF_SIZE 256

void make_message(int num, char *message){
  char buftime [26];
  time_t timer;
  struct tm* tm_info;
  time(&timer );
  tm_info = localtime(&timer );
  strftime(buftime , 26, "%Y-%m-%d %H:%M:%S" , tm_info ) ;
  sprintf(message , "%s %d at %s" , "Hello , I’m child number",num , buftime);
}
int main(int argc, char *argv[]){
  int i;
  char *virtualaddr;
  int shmid;
  sem_t *sGet;
  sem_t *sPut;
  key_t key;
  
  /*----- Attaching the shared mem to my address space  */
  key = ftok(argv[0],'R'); /* Generation de la key */
  shmid = shmget(key, 1024, 0644|IPC_CREAT); /* Creation du segment
                                                memoire : 1024 octets */
  if (0 > shmid){
    perror("Shared Mem creation error\n");
    exit(1);  
  }
  /* => virtualaddr will be available across fork ! */
  virtualaddr = shmat(shmid, (void*)0, 0); /* Attachement � l'espace mem du processus */
  sem_unlink("/put");
  sem_unlink("/get");
  /*--- Create POSIX Named Semaphores, and initialising with 1 */
  int init_sem_value = 0; /* Dijkstra sem */
  sPut = sem_open("/put", O_CREAT|O_RDWR, 0644, init_sem_value);
  sGet= sem_open("/get", O_CREAT|O_RDWR, 0644, init_sem_value);
  switch (fork()){ /*----- child 1 */
  case -1:
    printf("Error forking child 1!\n");  exit(1);
  case 0:
    {
      
         
      char buf[BUF_SIZE];

      /* Referring the semaphore */
      sPut = sem_open ("/put", O_RDWR);
      sGet  = sem_open("/get", O_RDWR);

      printf("\nChild 1 executing...\n");
      for(int i =0;i<50;i++){
      /*Child 1 writing in shared mem */
      //sem_wait(sGet);
      make_message(1,buf);
      sleep(1);
      strcpy (virtualaddr, buf);
      printf("Message sent by child1 : %s\n", buf);
      sem_post(sGet);

     // Child 1 reading from shared mem 
      //sem_wait(sGet);
      sem_wait(sPut);   
      strcpy (buf, virtualaddr);
      printf("Message received by child 1: %s\n", buf);
      //sem_post(sPut);
        }
      /*printf("Exiting child 1...\n"); */
      _exit(0);
    }
    break;
  default: break;
  }



  
  switch (fork()){ /*----- child 2 */
  case -1:
    printf("Error forking child 2!\n"); exit(1);
  case 0:
    {
    
    

      char buf[BUF_SIZE];

      /* Referring the semaphore */
      sGet = sem_open ("/get", O_RDONLY);
      sPut = sem_open ("/put", O_WRONLY);

      printf("\nChild 2 executing...\n");
    for(i=0;i<50;i++){
      /*Child 2 reading from shared memory*/
    //  sem_wait(sPut);    
      sem_wait(sGet);
      strcpy (buf, virtualaddr);
      printf("Message received by child 2: %s\n", buf);
  
     // sem_post(sGet);
      
      /*Child 2 writing in shared mem*/
        
      make_message(2,buf);
      sleep(1); /* La fabrication du message prend un peu de temps*/
      strcpy(virtualaddr, buf);
      printf("Message sent by child 2 : %s\n", buf);
      sem_post(sPut);

      /*printf("Exiting child 2...\n");*/
    }
      _exit(EXIT_SUCCESS);
    }
    break;  
  default:
    break;
  }



  
  
  printf("Parent waiting for children completion...\n");
  for(i=0;i<=1;i++){
    if (wait(NULL) == -1){
      printf("Error waiting.\n");
      exit(EXIT_FAILURE);
    }
  }
  printf("Parent finishing.\n");

  //Deleting semaphores..
  sem_unlink ("/put");
  sem_unlink ("/get");

  //Deleting Shared Memory.
  shmctl (shmid, IPC_RMID, NULL);
  exit(EXIT_SUCCESS);

}
