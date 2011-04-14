#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

char *get_path(char *pathname){
	char *path = malloc(PATH_MAX);
	if(chdir(pathname) != 0){
		perror("blad przy zmienianiu katalogu");
		exit(1);
	}
	if(!(getcwd(path,PATH_MAX))){
		perror("blad przy odczytywaniu sciezki katalogu");
		exit(2);
	}
	return path;
}


int main(int argc, char * argv[])
{
	int i;
	int verbose=0,own=0,sum=0,waiting=0,expand_path=1;
	DIR * dir;
	struct dirent * file;
	char * new_env;
	siginfo_t ex;
	int cp=0;
	for(i=1;i<argc; i++){
		if(!strcmp(argv[i],"-v"))
			verbose=1;
		if(!strcmp(argv[i],"-w"))
			waiting=1;
		if(!strcmp(argv[i],"-p")) //aby dodac do PATH odpowiedni folder
			expand_path=0;
	}
	char str[PATH_MAX]="";
	strcat(str,getenv("PATH_TO_BROWSE")&&strcmp("",getenv("PATH_TO_BROWSE"))?getenv("PATH_TO_BROWSE"):".");
	char str2[10]="";
	strcat(str2,getenv("EXTENSION_TO_BROWSE")&&strcmp("",getenv("EXTENSION_TO_BROWSE"))?getenv("EXTENSION_TO_BROWSE"):"");
	if(expand_path){
		char the_path[512]="";
		strcat(the_path,getenv("PATH"));
		strcat(the_path,":");
		char temp[128];
		strcpy(temp,argv[0]);
		int temp_len=strlen(temp);
		for(i=0;i<=6;i++)
			temp[temp_len-1-i]=0;
		strcat(the_path,get_path(temp));
		setenv("PATH",the_path,1);
	}
	dir=opendir(str);
	file=readdir(dir);
	while(file){
		if(file->d_type==DT_DIR && strcmp(file->d_name,".") && strcmp(file->d_name,"..")){
			cp++;
			if(!fork()){
				new_env=malloc(PATH_MAX*sizeof(char));
				strcat(new_env,str);
				strcat(new_env,"/");
				strcat(new_env,file->d_name);
				setenv("PATH_TO_BROWSE",new_env,1);
				if(verbose && waiting)
					execlp("policz","policz","-v","-w","-p",NULL);
				else if(verbose)
					execlp("policz","policz","-v","-p",NULL);
				else if(waiting)
					execlp("policz","policz","-w","-p",NULL);
				else
					execlp("policz","policz","-p",NULL);
			}
		} else if(file->d_type==DT_REG){
			int ext_len=strlen(str2);
			int file_len=strlen(file->d_name)-ext_len;
			int boolean=1;
			for(i=0;i<ext_len;i++)
				if(str2[i]!=file->d_name[file_len+i])
					boolean=0;
			if(!ext_len || boolean){
				own++;
				sum++;
			}
		}
		file=readdir(dir);
	}
	if(waiting)
		sleep(10);
	for(;cp;cp--){
		waitid(P_ALL,0,&ex,WEXITED);
		sum+=ex.si_status;
	}
	if(verbose)
		printf("%s: %d (%d w tym katalogu)\n",str,sum,own);
	_exit(sum);
}
