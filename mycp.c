#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char* argv[])
{
    if(argc <2){
        printf("mycp : 파일 명령어 누락 \n"); //인자 없거나, 하나인경우
    }else{
        int filedesR = open(argv[1], O_RDONLY);
        int filedesW;
        struct stat readFile_info;
        struct stat writeFile_info;
        
        char buf[1028];
        ssize_t nread;
        
        if(errno != 0){
            if(errno == 2){
                printf("%s 를 설명할 수 없음 : %s \n",argv[1], strerror(errno)); //파일이 존재하지 않을 때
                exit(EXIT_FAILURE);
            }else{
                printf("%s 다음에 대상 파일명령이 누락됨: %s \n",argv[1], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
       
        if(stat(argv[1] , &readFile_info) == -1){ //stat error
            perror("stat");
            exit(EXIT_FAILURE);
        }else{
            mode_t readFile_mode = readFile_info.st_mode;
            if(S_ISDIR(readFile_mode)){ // argv[1] : 디렉터리
                printf("mycp: %s 를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다 ", argv[1]);
                exit(EXIT_FAILURE);
            }
            if(S_ISREG(readFile_mode) || S_ISLNK(readFile_mode)){ //일반파일, 심볼릭
                filedesW = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, readFile_mode);
            }
        }

        while((nread = read(filedesR,buf,1028)) > 0){
            if(write(filedesW, buf, nread) <nread){
                break;
            }
        }
        close(filedesR);
        close(filedesW);
    }
    return 0;
}