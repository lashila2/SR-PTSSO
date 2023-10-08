
#ifndef DEDUPLICATION_GLOBLE_H
#define DEDUPLICATION_GLOBLE_H
#include "pbc/pbc.h"
#include "openssl/sha.h"
#include "openssl/aes.h"
#include "assert.h"
#include "math.h"
#include "time.h"


typedef struct{
    element_t P;
    pairing_t pairing;//pairing中包含了G和GT
} PublicParameter;

typedef struct {
    char si[128];
}Si;

typedef struct {
    char vi[128];
}Vi;

typedef struct {
    char ID[64];
    char ki[128];
    char ppk[128];
}Sharemessage;

typedef struct {
    char ID[64];
    char pw_[256];
    char tpk[256];
    char pld[64];
}Req;

typedef struct {
    char sigmai[256];
    char C1[256];
    char C2[256];
}Res;

typedef struct {
    int sd;
    Req req;
    Res res;
}Message;

typedef struct {
    char pld[64];
    char tpk_[256];
    char tkn[256];
}Atkn;


extern PublicParameter PP;

//extern int t = 3 ,n = 5;
extern char* param;

extern int t,n;
// e h h_1 H 也由pairing实现
extern int q_E;

extern int num;//存放拥有相同文件user的个数


extern void Sig();//签名

void initPP(PublicParameter *PP,char* param);

void genSHA256(unsigned char* output_sha256, unsigned char* msg);

void print_hex(unsigned char *buffer, int size);

void printf_buff(char *buff,int size);

void sprintf_buff(char *buff1, char *buff, int size);

char *padding_buf(char *buf,int size,int *final_size);

unsigned char *str2hex(char *str);

void encrpyt_buf(char *raw_buf,char **encrpyt_buf,int len, unsigned char *key);

void decrpyt_buf(char *raw_buf,char **encrypt_buf,int len, unsigned char *key);

void genshare(element_t secret,int n,int t,element_t *result,pairing_t pairing);

void random_select(int* result);

void get_lagrange_coefficient(int a,int *shareid,element_t *lagrange_co,pairing_t pairing);

void recover_secret(element_t* s,element_t *result,pairing_t pairing);

void recover_secretG(element_t* s,element_t *result,pairing_t pairing);

void EKeygen(element_t* pk,element_t* sk);

void EEnc(element_t* C1,element_t* C2,element_t pk,element_t M);

void EDec(element_t *M,element_t sk,element_t C1, element_t C2);

void Ererand(element_t* C1_,element_t* C2_,element_t C1,element_t C2,element_t pk);

void EKM(element_t *pk_,element_t pk,element_t r);

void H1(element_t *e,element_t g,char *m);

void H2(element_t *e,char* m);

void H3(unsigned char* h,unsigned char* m);


#endif //DEDUPLICATION_GLOBLE_H
