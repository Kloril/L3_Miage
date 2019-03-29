/* Exemple de tube/pipe :
   Le pere �crit un message � son fils.

   Author : GM
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

pid_t pid;
pid_t pid2;

int main(int argc, char *argv[]){

  /* CREATION DU pipe => deux extremites : lire/�crire */
  int fdAller[2];
  int fdRetour[2];
  if (pipe(fdAller) == -1) {
    fprintf(stderr,"Pipe failed");
    return 1;
  }

  if (pipe(fdRetour) == -1) {
    fprintf(stderr,"Pipe failed");
    return 1;
  }

  /* fork a child process child 1*/ 
  
  switch(pid2=fork()){
      case -1:
      {
        fprintf(stderr, "Fork Failed"); 
         return 1;
         break;
      }
         
      case 0:
      {
         close(fdRetour[WRITE_END]);
         close(fdAller[READ_END]);

        for(int i =0;i<5;i++){
        int status;
        char msg[BUFFER_SIZE] = "Bienvenu fils 2 !";
        
        
        /* close the unused end of the pipe */
       
        
        /* write to the pipe */
        write(fdAller[WRITE_END], msg, strlen(msg)+1);
        
        printf("le fils  1 écrit : %s\n", msg);
       // fflush(stdout);
        
        /* close the write end of the pipe */
        

      //  wait(&pid2); 
       /* Non zombie ! */
    
  /* child process : READ */
        char msgRecu[BUFFER_SIZE];
        
        /* close the unused end of the pipe */
       

        /* read from the pipe */
        read(fdRetour[READ_END], msgRecu, BUFFER_SIZE);
        printf("Le fils 1 lit : %s\n",msgRecu);
      //  fflush(stdout);
        
        /* close the write end of the pipe */
        
        }
        close(fdAller[WRITE_END]);
        close(fdRetour[READ_END]);
         exit(0);
        break;
      }
        

        default:
            break;
    }
  
    switch (pid=fork())
    {
        case -1:{
            fprintf(stderr, "Fork Failed"); exit(1);
            break;
        }
            
        case 0 :
        {
            close(fdAller[WRITE_END]);
            close(fdRetour[READ_END]);

            for(int i =0;i<5;i++){

            
                char msgRecu[BUFFER_SIZE];
        
        /* close the unused end of the pipe */
        

        /* read from the pipe */
        read(fdAller[READ_END], msgRecu, BUFFER_SIZE);
        printf("Le fils 2 lit : %s\n",msgRecu);
      //  fflush(stdout);
        
        /* close the write end of the pipe */
       
            
                int statusChild;
            char msg[BUFFER_SIZE] = "Bienvenu fils 1!";
            
            /* close the unused end of the pipe */
            close(fdRetour[READ_END]);
            
            /* write to the pipe */
            write(fdRetour[WRITE_END], msg, strlen(msg)+1);
            
            printf("le fils  2 écrit : %s\n", msg);
         //   fflush(stdout);
            
            /* close the write end of the pipe */
           
             /* Non zombie ! */
        }
          close(fdRetour[WRITE_END]);
         close(fdAller[READ_END]);
  /* child process : READ */
        exit(0);
        break;

        }
             
        default:
            break;
    }
     
  //close()
  return 0;
}

