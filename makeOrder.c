#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

void date(char *dt) //Save date in pointer 'dt' (using time.h) 
{
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  sprintf(dt, "\n\n%d/%d/%d",tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void write_file(int fd, char *buffer) //Write to order txt file
{
    int writed;
    writed = write(fd, buffer, strlen(buffer));
    if (writed != strlen(buffer) || writed == -1){
        perror("Writing error"); exit(-1);
    }
}/

int confirm(char *order, int sum){
    //Check if approve an order: return 1 for 'Confirm', else 0
    //Save total price and format sentence on 'order'
    char conf[20], tmp[30];
    sprintf(tmp, "Total price: %d NIS", sum);
    strcat(order,tmp);
    printf("%s (Confirm to approve/else cancel)\n",tmp);
    fgets(conf,10,stdin);   
    if (strcmp(conf,"Confirm\n") != 0){
        printf("Canceled"); return 0;
    }
    return 1;
}

int price(int fd_from, char *dish){
    //Find price for dish from file, calculate and return total price, for error return -1
    
    char *pr = "\0", *temp, buffer[256], *tmp2;
    int price = 0, rbytes, quant;
    temp = malloc(50*(sizeof(char)+1));
    strcpy(temp,dish);
    quant = atoi(strrchr(dish, ' '));
    temp[strcspn(temp," ")] = 0;
    do{  //Do until it find the dish in text, save price in 'pr' 
        rbytes = read(fd_from,buffer,255);
        if ((tmp2 = strstr(buffer, temp)) == NULL)
            continue;
        pr = strtok_r(tmp2,"NIS",&tmp2);
        pr = strrchr(pr,' ');
    }while (rbytes > 0);
    free(temp);
    price = atoi(pr);
    lseek(fd_from, 0, SEEK_SET);
    if (price <= 0)
        return -1;
    return price*quant;
}

int open_file(int mod, mode_t cmd, char *argv1, char *argv2, char *ext){
    //Open file with given permissions, return fd for success, exit for error 
    char rest[100];
    int fd;
    sprintf(rest,"%s%s%s.txt",argv1, ext, argv2);
    if ((fd = open(rest, cmd, mod)) == -1){
        perror("file from"); exit (1);
    }
    return fd;
}

int main(int argc,char **argv){
    int fd_from, fd_to, sum = 0, tmp = 0;
    char input[256], dt[15], path[30], order[1024] = "\0";
    if (argc != 3){ //exit for wrong input
        perror("arguments"); exit (1);
    }

    fd_from = open_file(O_RDONLY, 0, argv[1], "", ""); //open rest menu for reading
    sprintf(order,"%s Order\n\n",argv[1]);
    printf("Insert your order (Finish to finish):\n");
    while (1){ //Insert dishes until receive 'Finish' 
        fgets(input, 255, stdin);
        input[strcspn(input,"\n")] = 0;
        if (strcmp(input, "Finish") == 0)
            break;
        if ((tmp = price(fd_from, input)) == -1){ //Check if price wrong
            printf("Error, enter other:\n"); continue;
        }
        sum += tmp;
        strcat(input,"\n");
        strcat(order,input);
    }
    fd_to = open_file(0664, O_WRONLY | O_CREAT, argv[1], argv[2], "_Order/"); //Open txt for order in rest folder
    if (confirm(order,sum)){ //Write to file if confirmed
        date(dt);
        strcat(order,dt);
        write_file(fd_to,order);
    }
    sprintf(path,"%s_Order/%s.txt", argv[1], argv[2]);
    chmod(path, O_RDONLY); //Change file permmisions: reading only
    close(fd_from); close(fd_to);
    return 0;
}