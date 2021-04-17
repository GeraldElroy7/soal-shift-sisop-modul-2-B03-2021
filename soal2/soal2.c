#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>

char download[] = "/home/frozz/Downloads/pets.zip";
char loc[] = "/home/frozz/modul2/petshop";
char keterangan[100] = "";

void listFilesRecursively(char *Path){
  int status;
  char path[1000];
  pid_t pid3,pid4,pid5;
  DIR *dir = opendir(Path);
  struct dirent *dp;
  FILE *pointer;
 
  if(!dir) return;
 
  while ((dp = readdir(dir)) != NULL){
	  if(strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0){
    
  	  //2B
  	  //We need to split the file's name using delimiter
  	  char token[100]= "";
  	  strcpy(token,dp->d_name);
  	  char pfolder[100] = "/home/frozz/modul2/petshop/";
  	  strtok(token,";");
  	  strcat(pfolder,token);
  	  pid3 = fork();
  	  if(pid3 == 0){
    	  char *argv3[] = {"mkdir","-p",pfolder,NULL};
    	  execv("/usr/bin/mkdir",argv3);
  	  }
  	  else{
    	((wait(&status))>0);
  	  }
 	 
  	  //2C
  	  //Move each picture to the right folder
  	  char mfolder[100] = "/home/frozz/modul2/petshop/";
  	  strcat(mfolder,dp->d_name);
  	  char petfile[100] = "";
  	  strcpy(petfile,dp->d_name);
 	 
  	  //Use delimiter from each .jpg file
  	  char a[1] = ";";
  	  char b[1] = "_";
  	  char c[2] = ".j";
  	  if(!(strstr(dp->d_name,"_"))){
    	  pid4 = fork();
    	if(pid4 == 0){
      	char *argv4[] = {"mv","-f",mfolder,pfolder,NULL};
      	execv("/usr/bin/mv",argv4);
    	}
    	else{
      	((wait(&status))>0);
    	}
    	char nama_pemilik[] = "";
   	 
    	strcpy(keterangan,pfolder);
    	strcat(keterangan,"/keterangan.txt");
   	 
    	//use fopen "a" to append a text to keterangan.txt
    	int check = 2;
    	pointer = fopen(keterangan,"a");
    	for(int i=0; i<50; i++){
      	if(check == 2){
        	if(petfile[i] == a[0]){
          	check-=1;
          	fprintf(pointer,"nama : ");
        	}
        	continue;
      	}
     	 
      	if(check == 1){
        	if(petfile[i] == a[0]){
          	check-=1;
          	fprintf(pointer,"\numur : ");
          	continue;
        	}
        	fprintf(pointer, "%c", petfile[i]);
        	strncat(nama_pemilik, &petfile[i], 1);
        	continue;
      	}
     	 
      	if(petfile[i]==c[0] && petfile[i+1]==c[1]){
        	fprintf(pointer," tahun\n\n");
        	break;
      	}
      	fprintf(pointer, "%c", petfile[i]);
    	}
   	 
    	char rm[100]="";
    	strcpy(rm,pfolder);
    	strcat(rm,"/");
    	strcat(rm,nama_pemilik);
    	strcat(rm.".jpg");
    	strcat(pfolder,"/");
    	strcat(pfolder,dp->d_name);
   	 
    	pid5 = fork();
    	if(pid5 == 0){
      	char *argv5[] = {"mv",pfolder,rm,NULL};
      	execv("/usr/bin/mv",argv5);
    	}
    	else{
      	((wait(&status))>0);
    	}
  	}
 	 
  	strcpy(path,Path);
  	strcpy(path,"/");
  	strcat(path,dp->d_name);
 	 
  	listFilesRecursively(path);
	}
  }	 
 
  closedir(dir);  
} 	 
    
int main(){
 
  pid_t pid1,pid2;
  int status;
 
  pid1 = fork();
 
  if(pid1 < 0) {
	exit(EXIT_FAILURE);
  }
 
  if(pid1 == 0) {
	char *argv1[] = {"mkdir","-p",loc,NULL};
	execv("/usr/bin/mkdir",argv1);
  }
  else {
	while((wait(&status)) > 0);
	pid2 = fork();
	if(pid2 == 0) {
  	char *argv2[] = {"unzip",download,"-x","*/*","-d",loc,NULL};
  	execv("/usr/bin/unzip",argv2);
	}
	else {
  	((wait(&status)) > 0);
	}
	listFilesRecursively(loc);   	 
  }
 
  return 0;
}
