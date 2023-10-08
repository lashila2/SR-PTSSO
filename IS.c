
#include "IS.h"
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
#include "Global.h"
#define MAXCONN 10
#define ERRORCODE -1
#define BUFFSIZE 2048
int count_connect = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct pthread_socket
{
    int socket_d;
    pthread_t thrd;
    Message message;
};
static void *thread_send(void *arg)
{
    char buf[BUFFSIZE];
    int sd = *(int *) arg;
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "hello,welcome to you! \n");
    if (send(sd, buf, strlen(buf), 0) == -1)
    {
        printf("send error:%s \n", strerror(errno));
        return NULL;
    }
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
static void *thread_send1(void *arg)
{
    char buf[BUFFSIZE];
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    while (1)
    {
//        memset(buf, 0, sizeof(buf));
//        read(STDIN_FILENO, buf, sizeof(buf));
        if (send(pt->socket_d, (char*)&(pt->message), sizeof(pt->message), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
        break;
    }
//    close(pt->socket_d);
    return NULL;
}
static void* thread_recv(void *arg)
{
    char buf[BUFFSIZE];
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    int sd = pt->socket_d;
    pthread_t thrd = pt->thrd;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(buf), 0); //
        if (rv < 0)
        {
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        if (rv == 0) // 
        {
            break;
        }
        printf("%s", buf); //
    }
    pthread_cancel(thrd);
    pthread_mutex_lock(&mutex);
    count_connect--;
    pthread_mutex_unlock(&mutex);
    close(sd);
    return NULL;
}
static void* thread_recv1(void *arg)
{
    char buf[BUFFSIZE];
    Message message1;
    struct pthread_socket *pt = (struct pthread_socket *) arg;
    int sd = pt->socket_d;
    pthread_t thrd = pt->thrd;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(sd, buf, sizeof(message1), 0); //
        memcpy(&message1,buf, sizeof(message1));
        if (rv < 0)
        {
            printf("recv error:%s \n", strerror(errno));
            break;
        }
        if (rv == 0) //
        {
            break;
        }
        printf("%s\n", message1.req.ID); //
//        element_printf("%B\n",message.req.pw_);
        break;
    }
    memcpy(&(pt->message),buf, sizeof(message1));
    pthread_mutex_lock(&mutex);
    count_connect--;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

static int create_listen(int port)
{

    int listen_st;
    struct sockaddr_in sockaddr; //
    int on = 1;
    listen_st = socket(AF_INET, SOCK_STREAM, 0); //
    if (listen_st == -1)
    {
        printf("socket create error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    if (setsockopt(listen_st, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) //
    {
        printf("setsockopt error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    sockaddr.sin_port = htons(port); //
    sockaddr.sin_family = AF_INET;    //
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);    //
    //
    if (bind(listen_st, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) == -1)
    {
        printf("bind error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    if (listen(listen_st, 5) == -1) //     
    {
        printf("listen error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    return listen_st;
}

int accept_socket(int listen_st)
{
    int accept_st;
    struct sockaddr_in accept_sockaddr; //
    socklen_t addrlen = sizeof(accept_sockaddr);
    memset(&accept_sockaddr, 0, addrlen);
    accept_st = accept(listen_st, (struct sockaddr*) &accept_sockaddr,&addrlen);
    //
    //
    if (accept_st == -1)
    {
        printf("accept error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    printf("accpet ip:%s \n", inet_ntoa(accept_sockaddr.sin_addr));
    return accept_st;
}
int run_server(int port)
{
    time_t start,end;
    double totalTime;
    Sharemessage sharemessage1[n];
    Si si1[n];
    for (int i = 0; i < n; ++i) {
        memset(&sharemessage1[i],0, sizeof(Sharemessage));
        memset(&si1[i],0, sizeof(Si));
    }
    FILE *fp = fopen("sharemessageki.txt", "rb+");
    fread(sharemessage1, sizeof(Sharemessage),n,fp);
    fclose(fp);
    fp = fopen("si.txt", "rb+");
    fread(si1, sizeof(char)*128,n,fp);
    fclose(fp);
    int listen_st = create_listen(port);    //
    pthread_t send_thrd, recv_thrd;
    struct pthread_socket ps;
    int accept_st;
    if (listen_st == -1)
    {
        return ERRORCODE;
    }
    printf("server start \n");
    while (1)
    {
        accept_st = accept_socket(listen_st); //
        if (accept_st == -1)
        {
            return ERRORCODE;
        }
        if (count_connect >= MAXCONN)
        {
            printf("connect have already be full! \n");
            close(accept_st);
            continue;
        }
        pthread_mutex_lock(&mutex);
        count_connect++;
        pthread_mutex_unlock(&mutex);
        ps.socket_d = accept_st;
        ps.thrd = send_thrd;
        if (pthread_create(&recv_thrd, NULL, thread_recv1, &ps) != 0)//
        {
            printf("create thread error:%s \n", strerror(errno));
            break;
        }
//        pthread_detach(recv_thrd); //
        pthread_join(recv_thrd,NULL);

        start = clock();
        element_t tpk,pw_;
        element_init_G1(tpk,PP.pairing);
        element_init_G1(pw_,PP.pairing);
        element_from_bytes_compressed(tpk,ps.message.req.tpk);
        element_from_bytes_compressed(pw_,ps.message.req.pw_);
//        element_printf("tpk* = %B\n",tpk);
//        element_printf("pw* = %B\n",pw_);

        element_t ki;
        element_init_Zr(ki,PP.pairing);
        element_from_bytes(ki,sharemessage1[ps.message.sd].ki);

        element_t ppk;
        element_init_G1(ppk,PP.pairing);
        element_from_bytes_compressed(ppk,sharemessage1[ps.message.sd].ppk);
//        element_printf("ppk = %B\n",ppk);

        element_t si;
        element_init_Zr(si,PP.pairing);
        element_from_bytes(si,si1[ps.message.sd].si);
//        element_printf("si = %B\n",si);

        element_t sigmai_;
        element_init_G1(sigmai_,PP.pairing);
        element_pow_zn(sigmai_,pw_,ki);

        char buff[256] = "0";
        strcpy(buff,ps.message.req.tpk);
        strcat(buff,ps.message.req.pld);
        element_t hash;
        element_init_G1(hash,PP.pairing);
        H2(&hash,buff);
//        element_printf("H2 = %B\n",hash);
        element_t tkni;
        element_init_G1(tkni,PP.pairing);
        element_pow_zn(tkni,hash,si);
//        element_printf("tkni = %B\n",tkni);
//        element_t pk
        element_t C1,C2;
//        element_printf("ppk1 = %B\n",ppk);
        element_init_G1(C1,PP.pairing);
        element_init_G1(C2,PP.pairing);
        EEnc(&C1,&C2,ppk,tkni);
//        element_printf("ppk2 = %B\n",ppk);
//        element_printf("tkni = %B\n",tkni);

//        element_printf("C1 = %B\n",C1);
//        element_printf("C2 = %B\n",C2);

        element_t psk;
        element_init_Zr(psk,PP.pairing);
        char buff1[129] = "0";
        fp = fopen("psk.txt","rb+");
        fread(buff1, sizeof(char),129,fp);
        element_from_bytes(psk,buff1);
        fclose(fp);

        EDec(&tkni,psk,C1,C2);
        element_pow_zn(ppk,PP.P,psk);
//        element_printf("psk = %B\n",psk);
//        element_printf("ppk = %B\n",ppk);
//        element_printf("tkni = %B\n",tkni);
//        EEnc(&C1,&C2,ppk,tkni);
//
//        element_printf("C1 = %B\n",C1);
//        element_printf("C2 = %B\n",C2);

        Res res;
        memset(&res,0, sizeof(Res));
        element_to_bytes_compressed(res.sigmai,sigmai_);
        element_to_bytes_compressed(res.C1,C1);
        element_to_bytes_compressed(res.C2,C2);

        ps.message.res = res;
        end = clock();
        totalTime = (double)(end-start)/CLOCKS_PER_SEC;
        printf( "IS time：%f \n", totalTime*1000);


        if (pthread_create(&send_thrd, NULL, thread_send1, &ps) != 0) //
        {
            printf("create thread error:%s \n", strerror(errno));
            break;

        }
//        pthread_detach(send_thrd);        //
        pthread_join(send_thrd,NULL);
        close(accept_st);
    }
    close(accept_st);
    close(listen_st);
    return 0;
}
static void* pthread_runIS(void *arg){
    int port = *(int *)arg;
    run_server(port);
}
//server main
int main(int argc, char *argv[])
{
//    if (argc < 2)
//    {
//        printf("Usage:port,example:8080 \n");
//        return -1;
//    }
    char buff[129];
    initPP(&PP,param);
    FILE *fp = fopen("P_origin.txt", "rb+");
    fread(buff, sizeof(char),129,fp);
    fclose(fp);
    element_from_bytes_compressed(PP.P,buff);
    element_printf("P = %B\n",PP.P);
    int port = atoi("5003");
    if (port == 0)
    {
        printf("port error! \n");
    }
    else
    {
        run_server(port);
    }
    return 0;
}
