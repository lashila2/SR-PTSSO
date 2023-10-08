
#include "Global.h"
#include "string.h"

int main(){

    time_t start,end;
    double totalTime;

    char buff[129];
    initPP(&PP,param);
    FILE *fp = fopen("P_origin.txt", "rb+");
    fread(buff, sizeof(char),129,fp);
    fclose(fp);
    element_from_bytes_compressed(PP.P,buff);
    element_printf("P = %B\n",PP.P);
//U
    start = clock();
    Atkn atkn;
    fp = fopen("Atkn.txt","rb+");
    fread(&atkn, sizeof(Atkn),1,fp);
    fclose(fp);
    element_t pk1,sk1;
    element_init_G1(pk1,PP.pairing);
    element_init_Zr(sk1,PP.pairing);
    EKeygen(pk1,sk1);
//    element_printf("pk1 = %B\n",pk1);

    element_t vAS,sAS;
    element_init_G1(vAS,PP.pairing);
    element_init_Zr(sAS,PP.pairing);
    EKeygen(vAS,sAS);

    element_t C11,C12;
    element_init_G1(C11,PP.pairing);
    element_init_G1(C12,PP.pairing);
    EEnc(C11,C12,vAS,pk1);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "user time：%f \n", totalTime);

//W
    start = clock();
    element_t C11_,C12_;
    element_init_G1(C11_,PP.pairing);
    element_init_G1(C12_,PP.pairing);
    Ererand(C11_,C12_,C11,C12,vAS);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "Wu time：%f \n", totalTime);

//AS
    start = clock();
    element_t tkn;
    element_init_G1(tkn,PP.pairing);
    element_from_bytes_compressed(tkn,atkn.tkn);
    element_t tpk_;
    element_init_G1(tpk_,PP.pairing);
    element_from_bytes_compressed(tpk_,atkn.tpk_);

    char buff1[256] = "0";
    strcpy(buff1,atkn.tpk_);
    strcat(buff1,"hello\0");
    element_t temp;
    element_init_G1(temp,PP.pairing);
    H2(&temp,buff1);

    element_t s,V;
    element_init_Zr(s,PP.pairing);
    element_set_si(s,10086);
    element_init_G1(V,PP.pairing);
    element_pow_zn(V,PP.P,s);
//    element_printf("tkn = %B\n",tkn);
//    element_printf("H2 = %B\n",temp);
    element_t temp1, temp2;
    element_init_GT(temp1, PP.pairing);
    element_init_GT(temp2, PP.pairing);
    pairing_apply(temp1, tkn, PP.P, PP.pairing);
    pairing_apply(temp2, temp, V, PP.pairing);
    if (!element_cmp(temp1, temp2)) {
        printf("signature verifies\n");
    } else {
        printf("signature does not verify\n");
    }

    EDec(&pk1,sAS,C11_,C12_);
//    element_printf("pk1 = %B\n",pk1);

    element_t pk2,sk2;
    element_init_G1(pk2,PP.pairing);
    element_init_Zr(sk2,PP.pairing);
    EKeygen(&pk2,&sk2);

    element_t SK_AS;
    element_init_G1(SK_AS,PP.pairing);
    element_pow_zn(SK_AS,pk1,sk2);

    element_t C21,C22;
    element_init_G1(C21,PP.pairing);
    element_init_G1(C22,PP.pairing);
    EEnc(&C21,&C22,tpk_,pk2);

    char tao2[2048] = "0";
    char vAS_string[256]= "0";
    char pk2_string[256]= "0";
    char pk1_string[256]= "0";
    char skAS_string[256]= "0";
    element_to_bytes_compressed(vAS_string,vAS);
    element_to_bytes_compressed(pk2_string,pk2);
    element_to_bytes_compressed(pk1_string,pk1);
    element_to_bytes_compressed(skAS_string,SK_AS);
    strcpy(tao2,vAS_string);
    strcat(tao2,atkn.tpk_);
    strcat(tao2,pk2_string);
    strcat(tao2,pk1_string);
    strcat(tao2,skAS_string);
    strcat(tao2,atkn.tkn);
    unsigned char hash_tao2[64]="0";
    H3(hash_tao2,tao2);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "AS time：%f \n", totalTime);

//WAS
    start = clock();
    element_t C21_,C22_;
    element_init_G1(C21_,PP.pairing);
    element_init_G1(C22_,PP.pairing);
    Ererand(&C21_,&C22_,C21,C22,tpk_);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "WAS time：%f \n", totalTime);


//u
    start = clock();
    char tsk_string[256];
    memset(tsk_string,0, sizeof(tsk_string));

    fp = fopen("tsk*.txt","rb+");
    fread(tsk_string, sizeof(tsk_string),1,fp);
    fclose(fp);
    element_t tsk_;
    element_init_Zr(tsk_,PP.pairing);
    element_from_bytes(tsk_,tsk_string);


    element_t pk2_;
    element_init_G1(pk2_,PP.pairing);
    EDec(&pk2_,tsk_,C21_,C22_);
//    element_printf("pk2 = %B\n",pk2);
//    element_printf("pk2* = %B\n",pk2_);

    element_t SK_u;
    element_init_G1(SK_u,PP.pairing);
    element_pow_zn(SK_u,pk2_,sk1);
    char SKu_string[256]= "0";
    element_to_bytes_compressed(SKu_string,SK_u);

    char tao2_[2048]= "0";
    strcpy(tao2_,vAS_string);
    strcat(tao2_,atkn.tpk_);
    strcat(tao2_,pk2_string);
    strcat(tao2_,pk1_string);
    strcat(tao2_,SKu_string);
    strcat(tao2_,atkn.tkn);;
//    printf("tao2 = %s\n",tao2);
//    printf("tao2* = %s\n",tao2_);
    unsigned char hash_tao2_[64]="0";
    H3(hash_tao2_,tao2_);
    if (!strcmp(hash_tao2_,hash_tao2)){
        printf("tao2 same\n");
    } else{
        printf("tao2 diff\n");
        exit(0);
    }

    char tao1[2048] = "0";
    strcpy(tao1,atkn.tpk_);
    strcat(tao1,vAS_string);
    strcat(tao1,pk1_string);
    strcat(tao1,pk2_string);
    strcat(tao1,SKu_string);
    strcat(tao1,atkn.tkn);
    unsigned char hash_tao1[64] = "0";
    H3(hash_tao1,tao1);
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "u time：%f \n", totalTime);

//AS
    start = clock();
    char tao1_[2048] = "0";
    strcpy(tao1_,atkn.tpk_);
    strcat(tao1_,vAS_string);
    strcat(tao1_,pk1_string);
    strcat(tao1_,pk2_string);
    strcat(tao1_,skAS_string);
    strcat(tao1_,atkn.tkn);
    unsigned char hash_tao1_[64] = "0";
    H3(hash_tao1_,tao1_);
    if (!strcmp(hash_tao1_,hash_tao1)){
        printf("tao1 same\n");
    } else{
        printf("tao1 diff\n");
        exit(0);
    }
    end = clock();
    totalTime = (double)(end-start)/CLOCKS_PER_SEC;
    printf( "AS time：%f \n", totalTime);

}
