#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_PAD 30

char *write_item(char *buffer)
{
    char *price = strrchr(buffer, ' ');
    int buff_size = strlen(buffer) - strlen(price) + 1;
    int pad = MAX_PAD - buff_size;
    char tmp[256], pad_str[MAX_PAD];

    
    memset(pad_str, '.', pad);
    strcpy(tmp, "  ");
    strncat(tmp, buffer, buff_size);
    strncat(tmp, pad_str,pad);
    strcat(tmp, price);
    strcat(tmp, "NIS\n");
    strcpy(buffer, tmp);
    return buffer;
}

char *write_title(int i, char *dish_name)
{
    char buff[256] = " ";
    char abc[2] = {97 + i, '\0'};
    if (i > 26)
    {
        perror("To many dishes");
        exit(-1);
    }
    strcat(buff, abc);
    strcat(buff, ". ");
    strcat(buff, dish_name);
    strcpy(dish_name, buff);
    return dish_name;
}
int write_file(int fd, char *buffer)
{
    int writed;
    if ((writed = write(fd, buffer, strlen(buffer))) == -1)
    {
        perror("Writing error");
        exit(-1);
    }
    if (writed != strlen(buffer))
    {
        perror("Writing error");
        exit(-1);
    }
    return 0;
}
void create_ord(char *res_name){
    char ordir[50];
    strcpy(ordir, res_name);
    strcat(ordir, "_Order");
    mkdir(ordir, 0755); 
    
}
int create_file(char *res_name){   
    int fd;
    char file_name[50];
    strcpy(file_name, res_name);
    strcat(file_name, ".txt");
    if ((fd = open(file_name, O_WRONLY | O_CREAT, 0664)) == -1){
        perror("create file error");exit(-1);
    }
    return fd;
}

int main(int argc, char **argv)
{
    int menu_fd;
    char menu_file[50], input[256];
    char *buffer;

    if (argc != 3)
    {
        perror("Argument error");
        return -1;
    }
    create_ord(argv[1]);
    menu_fd = create_file(argv[1]);
    write_file(menu_fd, argv[1]);
    write_file(menu_fd, "_Menu\n\n");
    for (int i = 0; i < atoi(argv[2]); i++)
    {
        int j = 1;
        printf("Insert Type Dish %c:\n", 97 + i);
        fgets(input, 256, stdin);
        buffer = write_title(i, input);
        write_file(menu_fd, buffer);
        while (1)
        {
            printf("Insert dish number %d:\n", j++);
            fgets(input, 256, stdin);
            input[strcspn(input, "\n")] = 0;
            if (strcmp(input, "stop") == 0)
                break;
            buffer = write_item(input);
            write_file(menu_fd, buffer);
        }
        write_file(menu_fd, "              ");
        write_file(menu_fd, "Bon Apetit");
        printf("Successfully created\n");
    }
    return 0;
}