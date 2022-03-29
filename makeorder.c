#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

char *get_odir(int fd_to, char *input){
    char *ordir = malloc(sizeof(char)*50);
    if(!ordir){
        perror("malloc");
        exit(1);
    }
    sprintf(ordir,"%s_Order",rest);
    return ordir;
}

int price(int fd_from, char *dish){
    char *price, *temp, buffer[256], *quant;
    int rbytes;
    quant = strrchr(dish, ' ');
    do{
        rbytes = read(fd_from,buffer,255);
        if ((temp = strstr(buffer, dish) == NULL))
            continue;
        price = strtok_r(temp,"NIS",&temp);
        price = strrchr(price,' ');
    }while (rbytes > 0);
    return atoi(price);
}

int main(int argc,char **argv){
    int totalp = 0, fd_from, fd_to, wbytes;
    char rest[50], input[256];
    if (argc != 3){
        perror("argument error");
        exit (1);
    }
    sprintf(rest,"%s.txt",argv[1]);
    if (fd_from = open(rest, O_RDONLY) == -1){
            perror("file from");
            exit (1);
    }
    sprintf(rest,"%s_Order/%s.txt",argv[1],argv[2]);
    if ((fd_to = open(rest, O_WRONLY | O_CREAT, 0664)) == -1){
            perror("file to");
            exit (1);
    }
    while (1){
        printf("Insert your order (Finish to finish):\n");
        fgets(input, 255, stdin);
        input[strcspn(input,"\n")] = 0;
        if (strcmp(input, "Finish") == 0)
            break;
        wbytes = write(fd_to, input, strlen(input));
        if (wbytes != strlen(input)){
            perror("write bytes");
            exit (1);
        }

    }

    }
    return 0;
}