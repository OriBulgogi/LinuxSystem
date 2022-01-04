#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

//헤더

typedef struct CMD {
    char *name;
    char *desc;
    int (*cmd)(int argc, char *argv[]);

}CMD;

int cmd_cd (int argc,char *argv[]);
int cmd_pwd (int argc, char *argv[]);
int cmd_exit (int argc, char *argv[]);
int cmd_help (int argc, char *argv[]);
int cmdProcessing (void);

CMD builtin[] = {
    {"cd", "작업 디렉터리 바꾸기", cmd_cd},
    {"pwd", "현재 작업 디렉터리는?", cmd_pwd},
    {"exit", "셸 실행을 종료합니다", cmd_exit},
    {"help", "도움말 보여 주기", cmd_help}
};
const int builtins = sizeof(builtin)/sizeof(CMD); //내장 명령의 수

int main(void)
{
    int isExit = 0;
    while (!isExit)
        
        isExit = cmdProcessing(); //명령 처리 루틴호출
    fputs("My shell을 종료합니다... \n", stdout);
    return 0;
}

#define STR_LEN 1024
#define MAX_TOKENS 128

int cmdProcessing(void)
{
    char cmdLine[STR_LEN]; //입력 명령 전체 저장하는 배열
    char *cmdTokens[MAX_TOKENS]; // 입력 명려을 공백으로 분리하는 배열
    char delim[] = " \t\n\r"; //토큰 구분자 - strtok에서 사용
    char *token; //하나의 토큰을 분리하는데 사용
    int tokenNum; // 입력 명령에 저장된 토큰 수
    struct sigaction act;
    act.sa_handler = SIG_IGN;

    int exitCode =0;        //종료 코드
    fputs ("[myshell] $", stdout); 
    fgets (cmdLine, STR_LEN, stdin); //한줄의 명령 입력
    
    tokenNum =0;
    token =strtok(cmdLine, delim); //입력 명령의 문자열 하나 분리

    while(token){
        cmdTokens[tokenNum++] = token; //분리된 문자열을 배열에 저장 
        token = strtok(NULL, delim);
    }
    cmdTokens[tokenNum] = NULL;
    if(tokenNum ==0)
        return exitCode;
    for(int i=0; i< builtins; ++i){ //내장명령인지 검사하여 명령이 있으면 해당함수 호출
        if(strcmp(cmdTokens[0], builtin[i].name)==0 )
            return builtin[i].cmd(tokenNum, cmdTokens);
    }
    
    pid_t  pid  = fork(); //외부 명령
    if(pid == 0){
        return execvp(cmdTokens[0], cmdTokens);
    }
    return exitCode;

}
void  printcwd(){
    char buf[1028];
    if(getcwd(buf, 1028) == NULL)
        exit(1);
    printf("%s\n", buf);

}
int cmd_cd(int argc, char *argv[])
{
    if(argc == 1){
        chdir(getenv("HOME")); //인자없이 cd -> home 
    }else if(argc ==2 ){
        if(chdir(argv[1]))
            printf("그런 디렉터리가 없습니다: %s", argv[1]);
    }else{
        // printcwd(); //test code    
    }
    return 0;
}
int cmd_pwd(int argc, char *argv[])
{
    printcwd();
    return 0;
}
int cmd_exit(int argc, char *argv[])
{

    return 1;
}
int cmd_help(int argc, char *argv[])
{
    if (argc == 1){
        for(int i=0; i<builtins; i++){
            printf("%-6s: %s \n",builtin[i].name, builtin[i].desc );
        }
    }else if(argc == 2){
        for(int i=0; i<builtins; i++){
            if(strcmp(builtin[i].name, argv[1]) == 0){
                printf("%-6s: %s \n",builtin[i].name, builtin[i].desc );
            }
        }
    }else{
        printf("특정명령어 or 인자가 없는 명령수행만 가능");
    }
    return 0;
}