#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
void display_list(struct dirent **namelist, int n){
    if(n == -1){
        perror("scandir");
        exit(EXIT_FAILURE);
    }
    while (n--){
        printf("%s \n", namelist[n]->d_name);
        free(namelist[n]);
    }
}


int main(int argc, char* argv[])
{
    struct dirent **namelist;
    struct stat dirInfo;
    int n;

    if(argc == 1 ){
        n = scandir(".", &namelist, NULL, NULL);
    }else{
        for(int i=1; i<argc; i++){
            if(stat(argv[i] , &dirInfo) == -1){
                perror("stat");
                exit(EXIT_FAILURE);
            }else{
                if(S_ISDIR(dirInfo.st_mode)){
                n = scandir(argv[i], &namelist,NULL,NULL);
                }else{
                    printf("%s 는 디렉터리가아님 \n",argv[i]);
                }
            }
        }
    }
    display_list(namelist, n);
    
    free(namelist);
    exit(EXIT_FAILURE);
}

