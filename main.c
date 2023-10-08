#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "string.h"
#include "Global.h"
#include "IS.h"
#include "user.h"
#include "W.h"



int main() {
    printf("Hello, World!\n");
    char buff[129];
    char buff1[129];
    memset(buff,0, sizeof(buff));
    initPP(&PP, param);
    element_printf("P = %B\n",PP.P);
    element_to_bytes(buff,PP.P);
    printf("%s\n",buff);
    FILE *fp = NULL;
    fp = fopen("P.txt", "wb+");
    fwrite(buff, sizeof(char),129,fp);
    fclose(fp);
    fp = fopen("P.txt", "rb+");
    fread(buff1, sizeof(char),129,fp);
    fclose(fp);
    printf("%s\n",buff1);
//    printf_buff(buff, strlen(buff));


    element_from_bytes(PP.P,buff1);
    element_printf("P = %B\n",PP.P);

    element_t secret;
    element_t *result;
    element_t s;
    result = (element_t*) malloc(sizeof(element_t)*n);
    element_init_Zr(secret,PP.pairing);
    element_set_si(secret,10086);
    element_printf("%B\n",secret);
    genshare(secret,n,t,result,PP.pairing);
//    recover_secret(&s,result,PP.pairing);
//
    element_t psk,ppk;
    element_t h_pw,kh;
    element_init_G1(ppk,PP.pairing);
    element_init_G1(h_pw,PP.pairing);
    element_init_G1(kh,PP.pairing);
    H2(h_pw,"lashila");
    element_pow_zn(kh,h_pw,secret);
    H1(&psk,kh,"lashila");
    element_printf("kh = %B\n",kh);
//
    element_pow_zn(ppk,PP.P,psk);
    element_printf("psk = %B\n",psk);
    element_printf("ppk = %B\n",ppk);

    Sharemessage sharemessage[n];
    Sharemessage sharemessage1[n];
    for (int i = 0; i < n; ++i) {
        memset(&sharemessage[i],0, sizeof(Sharemessage));
        memset(&sharemessage1[i],0, sizeof(Sharemessage));
        strcpy(sharemessage[i].ID,"lashila");
        element_to_bytes(sharemessage[i].ki,result[i]);
        element_to_bytes_compressed(sharemessage[i].ppk,ppk);
    }

    fp = fopen("sharemessageki.txt", "wb+");
    fwrite(sharemessage, sizeof(Sharemessage),n,fp);
    fclose(fp);

    fp = fopen("sharemessageki.txt", "rb+");
    fread(sharemessage1, sizeof(Sharemessage),n,fp);
    fclose(fp);


    for (int i = 0; i < n; ++i) {
        element_from_bytes(result[i],sharemessage1[i].ki);
    }
    recover_secret(&s,result,PP.pairing);

    Si si[n];
    Si si1[n];

    for (int i = 0; i < n; ++i) {
        memset(&si[i],0, sizeof(char)*128);
        memset(&si1[i],0, sizeof(char)*128);
        element_to_bytes(si[i].si,result[i]);
    }
    fp = fopen("si.txt", "wb+");
    fwrite(si, sizeof(char)*128,n,fp);
    fclose(fp);

    fp = fopen("si.txt", "rb+");
    fread(si1, sizeof(char)*128,n,fp);
    fclose(fp);

    element_t vi_result[n];
    Vi vi[n];
//    element_t vi_result[n];
    for (int i = 0; i < n; ++i) {
        memset(&vi[i],0, sizeof(char)*128);
        element_init_G1(vi_result[i],PP.pairing);
        element_pow_zn(vi_result[i],PP.P,result[i]);
        element_to_bytes_compressed(vi[i].vi,vi_result[i]);
//        element_printf("vi = %B\n",vi_result[i]);
    }
    fp = fopen("vi.txt", "wb+");
    fwrite(vi, sizeof(char)*128,n,fp);
    fclose(fp);

    Vi vi1[n];
    for (int i = 0; i < n; ++i) {
        memset(&vi1[i],0, sizeof(char)*128);
    }
    fp = fopen("vi.txt", "rb+");
    fread(vi1, sizeof(char)*128,n,fp);
    fclose(fp);



    element_t tmp;
    char* string;
    string = (char*)malloc(sizeof(char)*256);
    element_init_G1(tmp,PP.pairing);
    element_from_hash(tmp,"123\n",3);
//    element_to_bytes(string,tmp);
//    printf("%s",string);
    element_t tmp2;
    element_init_G1(tmp2,PP.pairing);

    element_t pk;
    element_t sk;
    EKeygen(&pk,&sk);

    element_t C1;
    element_t C2;
    EEnc(&C1,&C2,pk,tmp);
    EDec(&tmp2,sk,C1,C2);
    element_to_bytes(string,tmp2);
    element_printf("temp1 = %B\n",tmp);
    element_printf("temp2 = %B\n",tmp2);
    printf("%s\n",string);
    printf("%d,\n", sizeof(element_t));

//    int IS_listenfd[n];
//    for (int i = 0; i < n; ++i) {
//        IS_listenfd[i] = initIS(5100+i);
//        printf("%d\n",IS_listenfd[i]);
//    }
//    pthread_t pt[n];
//    message m[n];
//    for (int i = 0; i < n; ++i) {
//        m[i].fdorport = IS_listenfd[i];
//        pthread_create(&pt[i],NULL,ISreceive_thread,&m[i]);
//        sleep(1);
//    }
//    Usersend(8800+1,"hello\0");

//    usleep(5000);
//    for (int i = 0; i < n; ++i) {
//        Usersend(8800+i,"hello\0");
//    }






//    for (int i = 0; i < n; ++i) {
//        pthread_join(pt[i],NULL);
//        closeIS(5100+i);
//    }

    return 0;
}
