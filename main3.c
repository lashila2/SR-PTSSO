
#include "Global.h"
#include "string.h"
#include "time.h"



void SAGen(element_t* apk,element_t* ask, element_t *k_){
    element_random(ask);
    element_pow_zn(apk,PP.P,ask);
    element_random(k_);
}

void SAKeygen(element_t *ski,element_t *pki,element_t apk,element_t k,element_t* tao){
    element_t et;
    element_init_G1(et,PP.pairing);
    element_pow_zn(et,apk,tao);

    element_mul(et,et,k);
    element_from_hash(ski,et, sizeof(et));
    element_pow_zn(pki,PP.P,ski);
    element_set(tao,ski);
    element_clear(et);

}
void SAKeypre(element_t *ski1,element_t *pki1,element_t ask,element_t k,element_t pki){
    element_t et;
    element_init_G1(et,PP.pairing);
    element_pow_zn(et,ask,pki);

    element_mul(et,et,k);
    element_from_hash(ski1,et, sizeof(et));
    element_pow_zn(pki1,PP.P,ski1);
    element_clear(et);
}

void SAEnc(element_t* C1,element_t* C2,element_t pk,element_t M,element_t apk,element_t k,element_t *tao){
    element_t ri;
    element_t tmp;
    element_init_Zr(ri,PP.pairing);
    element_init_G1(tmp,PP.pairing);
    element_t et;
    element_init_G1(et,PP.pairing);
    element_pow_zn(et,apk,tao);
    element_mul(et,et,k);
    element_from_hash(ri,et, sizeof(et));
    element_pow_zn(C1,PP.P,ri);
//    element_printf("P = %B\n",PP.P);
    element_pow_zn(tmp,pk,ri);
//    element_printf("r * pk = %B\n",tmp);
//    element_printf("r = %B\n",r);
//    element_printf("pk = %B\n",pk);
    element_add(C2,M,tmp);
    element_set(tao,ri);
    element_clear(ri);
    element_clear(tmp);
}

void SAMsgext(element_t *M,element_t ask,element_t pk,element_t k, element_t C1,element_t C2 ,element_t C1_,element_t C2_){
    element_t et;
    element_init_G1(et,PP.pairing);
    element_pow_zn(et,C1_,ask);
    element_t ri;
    element_init_Zr(ri,PP.pairing);
    element_mul(et,et,k);
    element_from_hash(ri,et, sizeof(et));
    element_t temp;
    element_init_G1(temp,PP.pairing);
    element_mul_zn(temp,pk,ri);
    element_sub(M,C2,temp);
}

int main()
{

    time_t start,end;

    initPP(&PP,param);
    element_t tao;
    element_init_Zr(tao,PP.pairing);
    element_random(tao);

    element_t ask,apk,k;
    element_init_G1(apk,PP.pairing);
    element_init_Zr(ask,PP.pairing);
    element_init_Zr(k,PP.pairing);
    start = clock();
    SAGen(&ask,&apk,&k);
    end = clock();
    double totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "SAGen time：%f \n", totalTime);

    element_t sk1,pk1;
    element_init_Zr(sk1,PP.pairing);
    element_init_G1(pk1,PP.pairing);
    start = clock();
    SAKeygen(&sk1,&pk1,apk,k,&tao);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "SAKeygen time：%f \n", totalTime);

    element_t sk2,pk2;
    element_init_Zr(sk2,PP.pairing);
    element_init_G1(pk2,PP.pairing);
    start = clock();
    SAKeypre(&sk2,&pk2,ask,k,pk1);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "SAKeypre time：%f \n", totalTime);

    element_t M;
    element_init_G1(M,PP.pairing);
    element_from_hash(M,"lashila",strlen("lashila"));

    element_t C1,C2;
    element_init_G1(C1,PP.pairing);
    element_init_G1(C2,PP.pairing);
    start = clock();
    SAEnc(&C1,&C2,pk1,M,apk,k,&tao);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "SAEnc time：%f \n", totalTime);

    element_t C1_,C2_;
    element_init_G1(C1_,PP.pairing);
    element_init_G1(C2_,PP.pairing);
    SAEnc(&C1_,&C2_,pk2,M,apk,k,&tao);


    start = clock();
    SAMsgext(M,ask,pk2,k,C1_,C2_,C1,C2);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "SAMsgext time：%f \n", totalTime);

    element_t sk,pk;
    element_init_G1(pk,PP.pairing);
    element_init_Zr(sk,PP.pairing);
    start = clock();
    EKeygen(&pk,&sk);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "EKeygen time：%f \n", totalTime);


    start = clock();
    EEnc(C1,C2,pk,M);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "EEnc time：%f \n", totalTime);


}