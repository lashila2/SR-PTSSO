#include "user.h"
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

#define BUFFSIZE 2048
#define ERRORCODE -1
#define IDe "lashila"
#define pw "lashila"

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
static void *thread_send1(void *arg)
{
    char buf[BUFFSIZE];
    Message message = *(Message *) arg;
    while (1)
    {
//        memset(buf, 0, sizeof(buf));
//        read(STDIN_FILENO, buf, sizeof(buf));

        if (send(message.sd, (char*)&message, sizeof(message), 0) == -1)
        {
            printf("send error:%s \n", strerror(errno));
            break;
        }
        break;
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
            if(rv == 0) //
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
static void *thread_recv1(void *arg)
{
    char buf[BUFFSIZE];
     Message *message = (Message *) arg;
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        int rv = recv(message->sd, buf, sizeof(Message), 0);
        memcpy(message,buf, sizeof(Message));
        if (rv <= 0)
        {
            if(rv == 0) //
            {
                printf("server have already full !\n");
                exit(0);//
            }
            printf("recv error:%s \n", strerror(errno));
            break;
        }
//        printf("%s\n", message->req.pld);//
//        element_printf("%B\n",message.req.pw_);
        break;
    }
    return NULL;
}
int run_client(char *ip_str, int port,int port2)
{
    time_t start,end;
    double totalTime1,totalTime2;
    start = clock();
    element_t tpk,tsk;
    element_init_G1(tpk,PP.pairing);
    element_init_Zr(tsk,PP.pairing);
    EKeygen(&tpk,&tsk);
    element_t r;
    element_init_Zr(r,PP.pairing);
    element_random(r);
    element_t pw_;
    element_init_G1(pw_,PP.pairing);
    element_t hash;
    H2(hash,pw);
    element_pow_zn(pw_, hash,r);
    element_clear(hash);
//    element_printf("tpk = %B\n",tpk);
    //构造消息结构体
    Req req;
    memset(req.ID,0, 64* sizeof(char));
    memset(req.pld,0, 64* sizeof(char));
    memset(req.tpk,0,256*sizeof(char));
    memset(req.pw_,0,256*sizeof(char));
    strcpy(req.ID,IDe);
    element_to_bytes_compressed(req.pw_,pw_);
    strcpy(req.pld,"hello\0");
    element_to_bytes_compressed(req.tpk,tpk);
    element_from_bytes_compressed(tpk,req.tpk);
//    element_printf("tpk = %B\n",tpk);

    Vi vi1[n];
    for (int i = 0; i < n; ++i) {
        memset(&vi1[i],0, sizeof(char)*128);
    }
    FILE *fp = fopen("vi.txt", "rb+");
    fread(vi1, sizeof(char)*128,n,fp);
    fclose(fp);
    element_t vi[n];
    for (int i = 0; i < n; ++i) {
        element_init_G1(vi[i],PP.pairing);
        element_from_bytes_compressed(vi[i],vi1[i].vi);
//        element_printf("vi = %B\n",vi[i]);
    }

//初始化用户
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
    client_sockaddr.sin_port = htons(port); //
    client_sockaddr.sin_family = AF_INET; //
    client_sockaddr.sin_addr.s_addr = inet_addr(ip_str);//
    con_rv = connect(client_sd, (struct sockaddr*) &client_sockaddr,
                     sizeof(client_sockaddr));
    //
    if (con_rv == -1)
    {
        printf("connect error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    Message message;
    message.sd = client_sd;
    message.req = req;
    end = clock();
    totalTime1 = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "u time：%f \n", totalTime1*1000);

    if (pthread_create(&thrd1, NULL, thread_send1, &message) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }

    if (pthread_create(&thrd2, NULL, thread_recv1, &message) != 0)
    {
        printf("thread error:%s \n", strerror(errno));
        return ERRORCODE;
    }
    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);//

    close(client_sd);

    element_t C1[n];
    element_t C2[n];
    element_t sigmai[n];
    Message message2;

    for (int i = 0; i < n; ++i) {
        int client_sd2;
        int con_rv2;
        pthread_t thrd1, thrd2;
        struct sockaddr_in client_sockaddr2; //
        client_sd2 = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sd2 == -1)
        {
            printf("socket create error:%s \n", strerror(errno));
            return ERRORCODE;
        }
        memset(&client_sockaddr2, 0, sizeof(client_sockaddr2));
        client_sockaddr2.sin_port = htons(port2); //
        client_sockaddr2.sin_family = AF_INET; //
        client_sockaddr2.sin_addr.s_addr = inet_addr(ip_str);//
        con_rv2 = connect(client_sd2, (struct sockaddr*) &client_sockaddr2,
                          sizeof(client_sockaddr2));
        //
        if (con_rv2 == -1)
        {
            printf("connect error:%s \n", strerror(errno));
            return ERRORCODE;
        }

        memset(&message2,0, sizeof(Message));
        message2.sd = client_sd2;

        if (pthread_create(&thrd2, NULL, thread_recv1, &message2) != 0)
        {
            printf("thread error:%s \n", strerror(errno));
            return ERRORCODE;
        }
        pthread_join(thrd2, NULL);
        close(client_sd2);

        element_init_G1(C1[i],PP.pairing);
        element_init_G2(C2[i],PP.pairing);
        element_init_G1(sigmai[i],PP.pairing);
        element_from_bytes_compressed(C1[i],message2.res.C1);
        element_from_bytes_compressed(C2[i],message2.res.C2);
        element_from_bytes_compressed(sigmai[i],message2.res.sigmai);
    }
    start = clock();
    element_t y;
    element_init_Zr(y,PP.pairing);
    element_from_bytes(y,message2.req.pld);
    element_t tsk_,tpk_;
    element_init_Zr(tsk_,PP.pairing);
    element_init_G1(tpk_,PP.pairing);
    element_add(tsk_,tsk,y);
    element_pow_zn(tpk_,PP.P,tsk_);

    element_t sigma;
    recover_secretG(&sigma,sigmai,PP.pairing);

    element_t temp;
    element_t r_invert;
    element_init_Zr(r_invert,PP.pairing);
    element_init_G1(temp,PP.pairing);
    element_invert(r_invert,r);
    element_pow_zn(temp,sigma,r_invert);
//    element_printf("rsigma = %B\n",temp);

    element_t psk;
    element_init_Zr(psk,PP.pairing);
    H1(&psk,temp,pw);
    element_clear(temp);
//    element_printf("psk = %B\n",psk);

//    char buff1[129] = "0";
//    element_to_bytes(buff1,psk);
//    fp = fopen("psk.txt","wb+");
//    fwrite(buff1, sizeof(char),129,fp);
//    fclose(fp);

    element_t tkni[n];
    element_init_G1(temp,PP.pairing);
    char buff[256] = "0";
    char tpk_string[256] = "0";
    element_to_bytes_compressed(tpk_string,tpk_);
//    element_printf(" y= %B\n",y);
//    element_printf("tpk* = %B\n",tpk_);
    strcpy(buff,tpk_string);
    strcat(buff,"hello\0");
    H2(&temp,buff);
//    element_printf("H2 = %B\n",temp);
    for (int i = 0; i < n; ++i) {
        element_t temp1, temp2;
        element_init_GT(temp1, PP.pairing);
        element_init_GT(temp2, PP.pairing);
        element_init_G1(tkni[i],PP.pairing);
        EDec(&tkni[i],psk,C1[i],C2[i]);
//        element_printf("tkni = %B\n",tkni[i]);
//        element_printf("C1 = %B\n",C1[i]);
//        element_printf("C2 = %B\n",C2[i]);
        pairing_apply(temp1, tkni[i], PP.P, PP.pairing);
        pairing_apply(temp2, temp, vi[i], PP.pairing);
//        if (!element_cmp(temp1, temp2)) {
//            printf("signature verifies\n");
//        } else {
//            printf("signature does not verify\n");
//        }

    }
    element_t tkn;
    recover_secretG(&tkn,tkni,PP.pairing);

    Atkn atkn;
    memset(&atkn,0, sizeof(atkn));
    strcpy(atkn.pld,"hello\0");
    element_to_bytes_compressed(atkn.tpk_,tpk_);
    element_to_bytes_compressed(atkn.tkn,tkn);

    fp = fopen("Atkn.txt","wb+");
    fwrite(&atkn, sizeof(Atkn),1,fp);
    fclose(fp);

    char tsk_string[256];
    memset(tsk_string,0, sizeof(tsk_string));
    element_to_bytes(tsk_string,tsk_);

    fp = fopen("tsk*.txt","wb+");
    fwrite(tsk_string, sizeof(tsk_string),1,fp);
    fclose(fp);

    end = clock();
    totalTime2 = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "u time：%f \n", totalTime2*1000);

    totalTime2 = totalTime1+totalTime2;
    printf( "u time：%f \n", totalTime2*1000);



    return 0;
}
int main(int argc, char *argv[])
{
    char buff[129];
    initPP(&PP,param);
    FILE *fp = fopen("P_origin.txt", "rb+");
    fread(buff, sizeof(char),129,fp);
    fclose(fp);
    element_from_bytes_compressed(PP.P,buff);
    element_printf("P = %B\n",PP.P);

    int port = atoi("5001");
    int port2 = atoi("5011");
    char *ip_str = "127.0.0.1";
    run_client(ip_str,port,port2);
    return 0;
}