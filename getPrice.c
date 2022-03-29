#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void print_price(char *buffer){
    char *price;
    price = strtok_r(buffer,"NIS",&buffer);
    price = strrchr(price,' ');
    printf("%s NIS\n",++price);
}

char *dish_name(int num,char **args){
    int i;
    char *d_name = malloc(50*sizeof(char));
    for (i=2;i<num;i++){
        sprintf(d_name,"%s",args[i]);
        if (i != num-1)
            sprintf(d_name," ");
    }
    return d_name;
}
int main(int argc, char **argv)
{
    int menu_fd,rbytes;
    char menu_file[256],buffer[256],*item,*d_name;
    sprintf(menu_file,"%s.txt",argv[1]);
    if ((menu_fd = open(menu_file, O_RDONLY)) == -1){
        printf("Restaurant Not Found!\n");return (-1);
    }
    d_name = dish_name(argc,argv);
    do{
        rbytes = read(menu_fd,buffer,255);
        if ((item = strstr(buffer,d_name))){
            print_price(item);return 0;}
    }while(rbytes > 0);
    free(d_name);
    printf("dish Not Found!\n");
    close(menu_fd);
    return -1;
}