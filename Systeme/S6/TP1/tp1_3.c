#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char *argv[])
{
    int pfd[2]; /* creation du tube */
    int status;
    if (pipe(pfd) == -1)
        perror("impossible de creer le tube");

    if(argc < 2){
        printf("Usage : ./filename cmd (options)");
        exit(1);
    }
    if (fork() == 0)
    {
        close(STDOUT_FILENO);
        close(pfd[0]);               /* close the unused read side */
        dup2(pfd[1], STDOUT_FILENO); /* connect the write side with stdout */
        close(pfd[1]);
        execlp(argv[1], argv[1], argv[2], (char *) 0); 
    }

    if(fork()!=0)
    {
        close(STDIN_FILENO);
        close(pfd[1]); // close the unused write side
        char buf[1024];
        read(pfd[0],buf,1024); //lit et ecrit dans buf le contenu du pipe
        printf("%s\n",buf); //ecrit sur stdout le contenu de buf
        close(pfd[0]); //ferme le pipe
        wait(&status); //attend la fin du fils (no zombie)
       
     
       
    
    }


}
