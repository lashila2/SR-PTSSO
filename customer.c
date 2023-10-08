#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

#define BUFFSIZE 1024
#define ERRORCODE -1

static void *thread_send(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(sd, buf, strlen(buf), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
    }
    return NULL;
}
static void *thread_recv(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(buf), 0);
        if (rv <= 0)
        {
            if(rv == 0) //server socket
            {
                printf("server have already full !\n");
                exit(0);//
            }
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        printf("%s", buf);//
    }
    return NULL;
}
int run_client(char *ip_str, int port)
{
    int client_sd;
    int con_rv;
    pthread_t thrd1, thrd2;
    struct sockaddr_in client_sockaddr; //
    client_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sd == -1)
    {
        printf("socket create error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    memset(&client_sockaddr, 0, sizeof(client_sockaddr));
    client_sockaddr.sin_port = htons(port);
    client_sockaddr.sin_family = AF_INET;
    client_sockaddr.sin_addr.s_addr = inet_addr(ip_str);
    con_rv = connect(client_sd, (struct sockaddr*) &client_sockaddr,
                     sizeof(client_sockaddr));

    if (con_rv == -1)
    {
        printf("connect error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    if (pthread_create(&thrd1, NULL, thread_send, &client_sd) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    if (pthread_create(&thrd2, NULL, thread_recv, &client_sd) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    pthread_join(thrd2, NULL);// 
    pthread_join(thrd1, NULL);
    close(client_sd);
    return 0;
}
int main(int argc, char *argv[])
{
//    if (argc < 3)
//    {
//        printf("Usage:ip port,example:127.0.0.1 8080 \n");
//        return ERRORCODE;
//    }
    int port = atoi("5001");
    char *ip_str = "127.0.0.1";
    run_client(ip_str,port);
    return 0;
}
