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
    return 0;
}

int get_price(char *input){
    char *price = strrchr(input, ' ');
    price++;
    input[strcspn(input,price)] = 0;
    return atoi(price);
}

void write_item(int fd,char *buffer)
{
    int bsize = strlen(buffer);
    int price = get_price(buffer);
    char tmp[1024], pad_str[MAX_PAD];

    memset(pad_str,'.',MAX_PAD-bsize);

    sprintf(tmp,"  %s %.*s %dNIS\n",buffer,MAX_PAD - bsize,pad_str,price);
    write_file(fd, tmp);
}

void write_title(int fd,int i, char *dish_name)
{
    char buffer[256];
    sprintf(buffer," %c. %s",97+i,dish_name);
    write_file(fd,buffer);
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
    char input[256], *buffer;

    if (argc != 3){
        perror("Argument error");return (-1);
    }

    menu_fd = create_files(argv[1]);
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        int j = 1;
        printf("Insert Type Dish %c:\n", 97 + i);
        fgets(input, 256, stdin);
        write_title(menu_fd,i ,input);
        while (1){
            printf("Insert dish number %d:\n", j++);
            fgets(input, 256, stdin);
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