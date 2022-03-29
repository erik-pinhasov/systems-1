#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    int ord_number=0;
    char ordir[256];
    DIR *ord_ptr;
    if (argc != 2){
        perror("args error");exit(1);
    }
    sprintf(ordir,"./%s_Order",argv[1]);
    struct dirent *order_dir;
    ord_ptr = opendir(ordir);
    if (ord_ptr)
    {
        while ((order_dir = readdir(ord_ptr)) != NULL){
            if (strcmp(order_dir->d_name,".") != 0 && strcmp(order_dir->d_name,"..") !=0 )
                ord_number++;
        }
        closedir(ord_ptr);
    }
    if (ord_number > 0)
        printf("%d\n",ord_number);
    else
        printf("No orders\n");
    return 0;
}