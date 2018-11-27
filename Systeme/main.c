/*
 * extract MP3 tag frames
 * *
 * Author: Gilles MENEZ
 * Created: 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <arpa/inet.h>

#include "util.h"
#include "id3_tagheader.h"
#include "id3_frame.h"



char ** ls_mp3_inArray(){
    struct dirent *de;


     DIR *dr = opendir("./mp3");

     if (dr == NULL)
     {
         printf("Could not open current directory" );

     }
     int cpt=0;
     while ((de = readdir(dr)) != NULL){
       //printf("%s\n", de->d_name);
       cpt++;
     }

    rewinddir(dr);
     char ** mp3 = malloc(cpt*sizeof(char *));

     int i=0;
      while ((de = readdir(dr)) != NULL){

      mp3[i]=malloc(strlen(de->d_name)+1);
      strcpy(mp3[i],de->d_name);

    //  strcpy(mp3[i],da->d_name);
      //strcat(mp3[i],'\0');

       i++;
     }

     closedir(dr);

     return mp3;
     free(mp3);

}
/*====================================================*/
/* Pour comprendre comment parcourir un fichier mp3.

   On rend la position en fin de lecture : normalement
   on devrait etre en fin de tag.
*/
int mp3_get_frame_from_id(int fd, char * id, char * contenu){
    frame_header fh;
    int nb_lu;
    char * frameId=malloc(5*sizeof(char));
    /* Tag Header analysis ---------------------*/
    tag_header th; /* on va recuperer le header du tag
  		    car on en aura besoin pour parcourir les frames */
    nb_lu = id3_read_tagheader(fd, &th);
    if (nb_lu != 10) return nb_lu;

    do {
      nb_lu = id3_read_frame_header(fd, &fh, th.tailletag); //lit le header de la frame
      strcpy(frameId,fh.id); // copie l'id de la frame
      strcat(frameId,"\0");
      if(strcmp(frameId,id)==0){ //si id de la frame est égal à l'id recherché
          read_error(fd,contenu,fh.tailleframe,"Content de l'id concerné"); //on rajoute le body de la frame dans le contenu
        lseek(fd, fh.tailleframe, SEEK_CUR); //on saute le body de la frame si mauvais ID
          break;
      }
      else {
      }
    } while ( nb_lu > 0);

    return nb_lu;
  }

  char * getAlbum(int fd){
    

  char* id="TALB";
  char* contenu = malloc(sizeof(char)*10000);
  
  int cr=mp3_get_frame_from_id(fd,id,contenu);
  
  //strcat(contenu,'\0');
  if((cr!=-1) && (contenu[0]  !='\0')){
    
    return contenu;
  }
  else
  {
  return "pas trouvé" ;
  }
    
    //printf("%s\n",buff);
  }
off_t mp3_read(int fd){
  int nb_lu;
  int i;

  /* Tag Header analysis ---------------------*/
  tag_header th; /* on va recuperer le header du tag
		    car on en aura besoin pour parcourir les frames */
  nb_lu = id3_read_tagheader(fd, &th);
  if (nb_lu != 10) return nb_lu;

  /* Tag frames analysis ---------------------*/
  do {
    nb_lu = id3_read_frame(fd, th.tailletag);
  } while ( nb_lu > 0);

  /* Pour verifier ou on est a la fin de la lecture du tag ? -*/
  off_t fintag = lseek(fd, 0, SEEK_CUR);

#if DEBUG
  printf("\nFIN TAG : %d\n", fintag);
#endif

  return fintag;
}

/*=====================================================*/

int main(int argc, char *argv[]){
 FILE *f;
  char *filename;
  int nb_lu;
  int i;

  /* Verification de l'appel du programme ----*/
  if (argc != 2){ /* usage -- print usage message and exit */
    fprintf(stderr, "Usage: %s mp3file\n", argv[0]);
    exit(1);
  }
  else
    filename = argv[1];


if (! (f = fopen(filename, "r"))){
   perror(filename);
   exit(1); 
}

  int fd = fileno(f); /* Get descriptor from FILE * */
  char* id="TALB";
  char contenu[10000] ="";

char *test = malloc(strlen(contenu)+1);  
test = getAlbum(fd);
printf("%s\n",test);
/*int cr=mp3_get_frame_from_id(fd,id,contenu);
if((cr!=-1) && (contenu[0]  !='\0')){contenu[30] ='\0';
  printf("%s␣\n",contenu);}
else
{
printf("%s␣\n","pas␣trouvé") ;
}*/
  /* 1) Parcours d'un fichier mp3 */
  //  mp3_read(fd);*/
/*  char ** lf = ls_mp3_inArray();
  int j =0;
  while(lf[j]!=NULL){
    printf(" %s\n",lf[j]);
    j++;
  }*/
  printf("\n");
  sync();

  return 0;
}

/*=====================================================*/
