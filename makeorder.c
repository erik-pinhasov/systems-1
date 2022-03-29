#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int write_file(int fd, char *buffer)
{
    int writed;
    writed = write(fd, buffer, strlen(buffer));
    if (writed != strlen(buffer) || writed == -1){
        perror("Writing error");exit(-1);
    }
    return 0;
}

int price(int fd_from, char *dish){
    char *price, *temp, buffer[256], input[256],*tmp2;
    int rbytes, quant;
    temp = malloc(50*(sizeof(char)+1));
    strcpy(temp,dish);
    quant = atoi(strrchr(dish, ' '));
    temp[strcspn(temp," ")] = 0;
    do{
        rbytes = read(fd_from,buffer,255);
        if ((tmp2 = strstr(buffer, temp)) == NULL)
            continue;
        price = strtok_r(tmp2,"NIS",&tmp2);
        price = strrchr(price,' ');
    }while (rbytes > 0);
    free(temp);
    return (atoi(price)*(quant));
}

int main(int argc,char **argv){
    int totalp = 0, fd_from, fd_to, wbytes, sum = 0;
    char rest[50], input[256], total[256], order[1024] = "\0", conf[10];
    if (argc != 3){
        perror("argument error");
        exit (1);
    }
    sprintf(rest,"%s.txt",argv[1]);
    fd_from = open(rest, O_RDONLY);
    if (fd_from == -1){
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
        sum += price(fd_from, input);
        strcat(input,"\n");
        strcat(order,input);
    }
    sprintf(total, "\nTotal price: %d NIS", sum);
    printf("(Confirm to approve/else cancle)\n");
    fgets(conf,10,stdin);   
    if (strcmp(conf,"Confirm\n") != 0)
        exit(1);
    strcat(order,total);
    write_file(fd_to,order);
    return 0;
}