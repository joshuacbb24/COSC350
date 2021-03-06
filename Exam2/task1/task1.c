#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>

#define READ 0  //In the file_pipes this one will be the read desc
#define WRITE 1 //In the file_pipes this one will be the write desc

int main(){
    int data_processed;
    int file_pipe1[2];//Parrent to Child
    int file_pipe2[2];//Child to Parrent
    const char child_message[] = "Hi mom\0";
    const char parrent_message[] = "I love you\0";
    //char buffer1[BUFSIZ+1];//Sends pipe1 file des
    //char buffer2[BUFSIZ+1];//Sends pipe2 file des
    char buffer[BUFSIZ+1];//Holds read in message
    pid_t fork_result;
    int in, out;

    //memset(buffer1, '\0', sizeof(buffer1));
    //memset(buffer2, '\0', sizeof(buffer2));
    memset(buffer, '\0', sizeof(buffer));


    if(pipe(file_pipe1) != 0){
        printf("Pipe1 failed\n");
        exit(EXIT_FAILURE);
    }
    if(pipe(file_pipe2) != 0){
        printf("Pipe2 failed\n");
        exit(EXIT_FAILURE);
    }
    
    fork_result = fork();
    if(fork_result == -1){
        fprintf(stderr, "Fork failure");
        exit(EXIT_FAILURE);
    }

    if(fork_result == 0){//Child
        close(file_pipe1[WRITE]);
        close(file_pipe2[READ]);
        //sprintf(buffer1, "%d", file_pipe1[READ]);
        //sprintf(buffer2, "%d", file_pipe2[WRITE]);
        //(void)execl("pipe4", "pipe4", buffer1, buffer2, "Hi, Mom\0", (char*)0);
        out = write(file_pipe2[WRITE], child_message, strlen(child_message));
        if(out == -1){
            printf("Error writing in child\n");
            exit(EXIT_FAILURE);
        }
        in = read(file_pipe1[READ], buffer, BUFSIZ);

        printf("My mom said: %s\n", buffer);

        close(file_pipe2[WRITE]);
        close(file_pipe1[READ]);
        exit(EXIT_FAILURE);
    }else{//Parrent
        close(file_pipe1[READ]);
        close(file_pipe2[WRITE]);
        
        data_processed = read(file_pipe2[READ], buffer, BUFSIZ);
        printf("My child said: %s\n", buffer);
        data_processed = write(file_pipe1[WRITE], parrent_message , strlen(parrent_message));
        wait(&fork_result);
    }


    exit(EXIT_SUCCESS);
}



