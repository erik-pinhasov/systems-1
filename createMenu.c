#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PAD 25

int write_file(int fd, char *buffer)
{
    int writed;
    writed = write(fd, buffer, strlen(buffer));
    if (writed != strlen(buffer) || writed == -1){
        perror("Writing error");exit(-1);
    }
    memset(buffer,0,strlen(buffer));
    return 0;
}

int get_price(char **input){
    int price;
    char *tmp = strrchr(*input, ' ');
    price = atoi(tmp);
    input[strcspn(*input,tmp)] = 0;
    return price;
}

void write_item(int fd,char *input)
{
    int price = get_price(&input);
    int bsize = strlen(input);
    char buffer[1024], pad_str[MAX_PAD];

    memset(pad_str,'.',MAX_PAD-bsize);

    sprintf(buffer,"  %s %.*s %dNIS\n",input,MAX_PAD - bsize,pad_str,price);
    write_file(fd, buffer);
}


int create_files(char *res_name){   
    int fd;
    char file_name[50],ordir[50];
    sprintf(ordir,"%s_Order",res_name);
    sprintf(file_name,"%s.txt",res_name);

    mkdir(ordir, 0755); 
    if ((fd = open(file_name, O_WRONLY | O_CREAT, 0664)) == -1){
        perror("Create restraunt file error");exit(-1);
    }

    file_name[strcspn(file_name,".")] = 0;
    sprintf(file_name,"%s_Menu\n",res_name);
    write_file(fd,file_name);
    return fd;
}

int main(int argc, char **argv)
{
    int menu_fd;
    char input[256], buffer[1024];

    if (argc != 3 || atoi(argv[2]) > 26){
        perror("Argument error");return (-1);}

    menu_fd = create_files(argv[1]);
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        int j = 1;
        printf("Insert Type Dish %c:\n", 97 + i);
        fgets(input, 254, stdin);
        sprintf(buffer," %c. %s",97+i,input);
        write_file(menu_fd,buffer);
        while (1){
            printf("Insert dish number %d:\n", j++);
            fgets(input, 254, stdin);
            input[strcspn(input, "\n")] = 0;
            if (strcmp(input, "stop") == 0)
                break;
            write_item(menu_fd,input);
        }
    }
    write_file(menu_fd, "               Bon Apetit");
    close(menu_fd);
    printf("Successfully created\n");
    return 0;
}