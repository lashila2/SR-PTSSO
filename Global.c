
#include "Global.h"
#include "string.h"
#include "stdio.h"


PublicParameter PP;

extern void Sig();//

char *param = "type a\n"
              "q 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\n"
              "h 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\n"
              "r 730750818665451621361119245571504901405976559617\n"
              "exp2 159\n"
              "exp1 107\n"
              "sign1 1\n"
              "sign0 1";

char *param80 = "type a\n"
                "q 102763100880691630550007492641178297811504126856595060243346329336699665671973957105208003302452445321724918610667872113010093995303019243806055072999061956342866100230266607997846897451040894143137654627262191349896018786483295426209082230659070902109329245977275861622959525370888746748436283965327650365527\n"
                "h 140626733841181092357897427722298544395888693761755997976346288046697973382327803142585509571953766697217331903746946000780642308524072548888409750201361676352634075568511730466316738503518403462510354126915540610842806998916523414013531816951531611936366043224\n"
                "r 730750818665451459101842416367364881864821047297\n"
                "exp2 159\n"
                "exp1 63\n"
                "sign1 1\n"
                "sign0 1";

char *param112 = "type a\n"
                 "q 42374727010286385486631308624107218721633807220492715261405830879329996429292646730852542507144543540116962301673739037729165941771802561432719441191003827024891825191830238309508676030734692423587251585683042842935530142670367556692352229221104146066445840432588831043644734514015769368188137882847332031377716707399607786140693586022951248259825873957331837594967800879351216101631231957769546838459532159786606132662526869868490154172499464116637130849195561446127870213359910900908942736686087931349704541501810634858959697126148666161406975179949741433675521295654775671730739248458355728671858501749547853305327\n"
                 "h 1571765980602546678696986510408322599482288963960088170064718955840352718417062180085495959291043100980063926911974801793696881201746675035932039104250901430262169258556009897021692152032131166364459453969458225830263637968613403434171571816018709226700645318792378321995503294929980044487516743900067522408725233823646810492646696371262876525051816146420710353991354401218987184553072315776470314954978858712121122989035509861124144379559524232399580068488842421703762983213069898527994363489866187616974611014564549073987368199134364950694816737808\n"
                 "r 26959946667150639794667015087019630673637144422540572481103610248191\n"
                 "exp2 224\n"
                 "exp1 10\n"
                 "sign1 -1\n"
                 "sign0 -1";

char *param128 = "type a\n"
                 "q 966767996962299188696873815247812097758444616140644054582832239160894661074169106962401395417850888547071823940214138823587976693383811219033225038153990478966705415145414079586847567094960570888162954093908451325346645646293073433117988280038349206327743535917662166892160831023860748461696162254157462750831593348322676956395697820499768571290854898841230391993640328123331695262294366356555279749903603634484348872575379942722068598474456253667607556939926907982678401676584786336184406697410138848946142671231572391346027432781761340353325064709830586157246318852021219972287335151048759786906988869306431444307938536897657115769224849938039487058376720274793284292043840940204750621674915393549897635059227380423262032313818627807062606429560383677630632143586717761812560755170861637771107317610398929759080572844923538572964716834665392417026710328988777027430069538142593095941033693324744112754340392236123073406343\n"
                 "h 16698342750874899630469142318187392660066989296564509345160041114635366293831830863781584662817809219641882830147808193258074032772699890036550010444638789298157171334292982320742512543325750247185844772751679483944250636567694789795216293622198620528676202566988522672486185788491368843104064947752925890114665939143482443686581081430687424492515972987298927600321581588645064237425469586863631187762318294496336135242456056765894245233200209542535535777935984007111357299757538153058248996897259695863868194040889856089330778220470807546790887651304454064679209024307930971437548395680646425729368179379053449352927421059840195192547367997254869459816609969638846447082729647596138681907660497310771998475456877195158283644544893070538280410576323211066823281790231308761882582081563650838177428709946679939887575786930904959179326506883498239368\n"
                 "r 57896044618658097711785492597880058715812779097856111313570905261936247570433\n"
                 "exp2 255\n"
                 "exp1 166\n"
                 "sign1 1\n"
                 "sign0 1";

int t = 7, n =10;

int num = 16;

void initPP(PublicParameter *PP,char* param){
    pairing_init_set_buf(PP->pairing, param, strlen(param));
//    pbc_param_t par;
//    pbc_param_init_a_gen(par,224,2048);
//    pbc_param_out_str(stdout,par);
//    pairing_init_pbc_param(PP->pairing,par);
    element_init_G1(PP->P,PP->pairing);
    element_random(PP->P);

    return;
}
void genSHA256(unsigned char* output_sha256, unsigned char* msg) {
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, msg, strlen(msg));
    SHA256_Final(output_sha256, &ctx);
}
void print_hex(unsigned char *buffer, int size) {
    int i;
    printf("******************start hex code**********************************\n");
    for (i = 1; i <= size; i++) {
        printf("0x%02X ",buffer[i-1]);
        if(i % 16 == 0 && i < size) {
            printf("\n");
        }
    }
    printf("\n");
    printf("********************end hex code************************************\n");
}
void printf_buff(char *buff,int size) {
    int i=0;
    for (i=0;i<size;i++) {
        printf("%02X",(unsigned char )buff[i]);
//        if((i+1)%64==0)
//            putchar('\n');
    }
    printf("\n");
}
void sprintf_buff(char *buff1, char *buff, int size){
    int i=0;
    for (i=0;i<size;i++) {
        sprintf(buff1,"%s%02X",buff1,(unsigned char )buff[i]);
    }
}
char *padding_buf(char *buf,int size,int *final_size) {
    char *ret=NULL;
    int pidding_size=AES_BLOCK_SIZE-(size%AES_BLOCK_SIZE);
    int i;
    *final_size=size+pidding_size;
    ret=(char *)malloc(size+pidding_size);
    memcpy(ret,buf,size);
    if(pidding_size!=0) {
        for (i=size;i<(size+pidding_size);i++) {
            ret[i]=0;
        }
    }
    return ret;
}
unsigned char *str2hex(char *str) {
    unsigned char *ret=NULL;
    int str_len=strlen(str);
    int i=0;
    assert((str_len%2)==0);
    ret=(char *)malloc(str_len/2 +1);
    for (i=0;i<str_len;i+=2) {
        sscanf(str+i,"%2hhx",&ret[i/2]);
    }
    return ret;
}
void encrpyt_buf(char *raw_buf,char **encrpyt_buf,int len, unsigned char *key) {
    AES_KEY aes;
    unsigned char *iv=str2hex("667b02a85c61c786def4521b060265e8");
    AES_set_encrypt_key(key,128,&aes);
    AES_cbc_encrypt(raw_buf,*encrpyt_buf,len,&aes,iv,AES_ENCRYPT);
    free(iv);
}
void decrpyt_buf(char *raw_buf,char **encrypt_buf,int len, unsigned char *key) {
    AES_KEY aes;
    unsigned char *iv=str2hex("667b02a85c61c786def4521b060265e8");
    AES_set_decrypt_key(key,128,&aes);
    AES_cbc_encrypt(raw_buf,*encrypt_buf,len,&aes,iv,AES_DECRYPT);
    free(iv);
}

void e(element_t temp3,element_t temp2){

}
void SecretShare(element_t s,int n,int t,pairing_t pairing){
    element_t *Fx;
    element_t *si;
    element_t temp;
    Fx = (element_t*)malloc(sizeof(element_t)*t);
    si = (element_t*)malloc(sizeof(element_t)*(n+1));
    element_init_Zr(temp,pairing);
    element_init_Zr(Fx[0],pairing);
    element_set(Fx[0],s);
    for (int i = 1; i < t; ++i) {
        element_init_Zr(Fx[i],pairing);
        element_random(Fx[i]);
    }
    for (int i = 1; i < n+1; ++i) {
        element_set0(temp);
        for (int j = 0; j < t; ++j) {

        }

    }
}


void genshare(element_t secret,int n,int t,element_t *result,pairing_t pairing)
{

    element_t *a;//
    a = (element_t*)malloc(sizeof(element_t) * (t));
    element_t *share;//
    share = (element_t*)malloc(sizeof(element_t) * (n));

    //element_printf("secret in = %B\n",secret);

    element_init_Zr(a[0],pairing);//a0 = secret

    element_set(a[0],secret);
    //element_printf("a[0] = %B\n",a[0]);

    for(int i=1;i<t;i++)
    {
        element_init_Zr(a[i],pairing);
//        element_set1(a[i]);
        element_random(a[i]);
        //element_printf("a[%d] = %B\n",i,a[i]);
    }

    for(int i=0;i<n;i++)//
    {
        element_init_Zr(share[i],pairing);
        element_set(share[i],a[0]);
        //element_printf("share[%d] = %B\n",i,share[i]);
    }

    for(int i=1;i<=n;i++)//
    {
        element_t tmp;//
        element_t x;

        element_init_Zr(tmp,pairing);
        element_init_Zr(x,pairing);

        element_set_si(x,i);

        for(int j =1;j<t;j++)
        {
            element_t tmp1;//
            element_t xpowj;
            element_t jz;

            element_init_Zr(tmp1,pairing);
            element_init_Zr(xpowj,pairing);
            element_init_Zr(jz,pairing);

            element_set_si(jz,j);
            element_pow_zn(xpowj,x,jz);

            element_mul(tmp1,a[j],xpowj);
            element_add(share[i-1],share[i-1],tmp1);

            element_set0(tmp1);
        }
    }
    for(int i=0;i<n;i++)//
    {
        element_init_Zr(result[i],pairing);
        element_set(result[i],share[i]);
        element_printf("%B\n",result[i]);
    }
    free(a);
    free(share);


}

void random_select(int* result)
{
    //
    for (int i = 0; i < n; i++) {
        result[i] = i;
    }
    // 
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }
    // 
    for (int i = 0; i < t; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
}

void get_lagrange_coefficient(int a,int *shareid,element_t *lagrange_co,pairing_t pairing)
{
    //printf("lagrang\n");
    int thisid = a+1;
    int idgroup[t];
    for(int i=0;i<t;i++)
    {
        idgroup[i]=shareid[i];
        //printf("%d\n",idgroup[i]);
        idgroup[i]++;
    }


    element_t l;
    element_t ll,lr;
    element_t lrn;

    element_init_Zr(l,pairing);
    element_init_Zr(lrn,pairing);

    element_init_Zr(ll,pairing);
    element_init_Zr(lr,pairing);

    signed long int lleft=1;
    for(int i=0;i<t;i++)
    {
        if(idgroup[i]==thisid)
        {
            continue;
        }
        lleft = lleft * (0-idgroup[i]);
    }

    signed long int lright=1;
    for(int i=0;i<t;i++)
    {
        if(idgroup[i]==thisid)
        {
            continue;
        }
        lright = lright * (idgroup[i]-thisid);
    }

    element_set_si(ll,lleft);
    element_set_si(lr,lright);
    element_invert(lrn,lr);

    element_mul(l,ll,lrn);
    element_init_Zr(lagrange_co,pairing);
    element_set(lagrange_co,l);
//    element_printf("lagl %B\n",ll);
//    element_printf("lagr %B\n",lr);
//    element_printf("lagrn %B\n",lrn);
}

void recover_secret(element_t* s,element_t *result,pairing_t pairing)
{
//    int shareid[3] = {3,2,1};
    int shareid[t];
//    shareid = (int*) malloc(sizeof(int)*t);
    printf("severs:\n",t);
    random_select(&shareid);
//    for(int i=0;i<t;i++)
//    {
//        IS[shareid[i]].chosen=1;
//    }
    // for(int i=0;i<n;i++)
    // {
    //     printf("%d\n",IS[i].chosen);
    // }

    element_t l[t];
//    l = (element_t*)malloc(sizeof(element_t) * (t));

    for(int i=0;i<t;i++)
    {
        element_init_Zr(l[i],pairing);
        get_lagrange_coefficient(shareid[i],shareid,&l[i],pairing);
        //element_printf("lagrange_co%d=%B\n",i,l[i]);
    }


    element_t *wl;
    wl = (element_t*)malloc(sizeof(element_t) * (t));
    for(int i=0;i<t;i++)
    {
        element_init_Zr(wl[i],pairing);
        element_mul(wl[i],result[shareid[i]],l[i]);
        element_printf("%B\n",wl[i]);
    }
//    element_add(wl[1],wl[0],wl[1]);
//    element_add(wl[2],wl[1],wl[2]);
//    element_printf("wl %B\n",wl[2]);


    element_t *w_compute;//w_c[0]存w1w2,w_c[1]存w1w2w3....w_c[t-2]存最终结果
    w_compute = (element_t*)malloc(sizeof(element_t) * (t-1));
    element_init_Zr(w_compute[0],pairing);
    element_add(w_compute[0],wl[0],wl[1]);
//    element_printf("%B\n",w_compute[0]);

    for(int i=1;i<t-1;i++)
    {
        element_init_Zr(w_compute[i],pairing);
        element_add(w_compute[i],w_compute[i-1],wl[i+1]);
//        element_printf("%B\n",w_compute[i]);
    }

    element_init_Zr(s,pairing);
    element_set(s,w_compute[t-2]);
    element_printf("\nrecover secret = %B\n",s);
//    printf("here1\n");

//    free(l);
    free(wl);
    free(w_compute);


}
void recover_secretG(element_t* s,element_t *result,pairing_t pairing)
{
    int shareid[n];
//    printf("severs:\n",t);
    random_select(&shareid);
    element_t *l;
    l = (element_t *) malloc(sizeof(element_t)*t);

    for(int i=0;i<t;i++)
    {
        element_init_Zr(l[i],PP.pairing);
        get_lagrange_coefficient(shareid[i],shareid,&l[i],PP.pairing);
    }


    element_t *wl;
    wl = (element_t*)malloc(sizeof(element_t) * (t));
    for(int i=0;i<t;i++)
    {
        element_init_G1(wl[i],PP.pairing);
//        element_printf("%B",result[i]);
        element_pow_zn(wl[i],result[shareid[i]],l[i]);
//        element_printf("%B\n",wl[i]);
    }
//    element_add(wl[1],wl[0],wl[1]);
//    element_add(wl[2],wl[1],wl[2]);
//    element_printf("wl %B\n",wl[2]);


    element_t *w_compute;//
    w_compute = (element_t*)malloc(sizeof(element_t) * (t-1));
    element_init_G1(w_compute[0],pairing);
    element_add(w_compute[0],wl[0],wl[1]);
//    element_printf("%B\n",w_compute[0]);

    for(int i=1;i<t-1;i++)
    {
        element_init_G1(w_compute[i],pairing);
        element_add(w_compute[i],w_compute[i-1],wl[i+1]);
//        element_printf("%B\n",w_compute[i]);
    }

    element_init_G1(s,pairing);
    element_set(s,w_compute[t-2]);
//    element_printf("\nrecover secret = %B\n",s);
//    printf("here1\n");

//    free(l);
    free(wl);
    free(w_compute);


}

void EKeygen(element_t* pk,element_t* sk){
    element_random(sk);
    element_pow_zn(pk,PP.P,sk);
//    element_printf("sk = %B; pk = %B\n",sk,pk);
}

void EEnc(element_t* C1,element_t* C2,element_t pk,element_t M){
    element_t r;
    element_t tmp;
    element_init_Zr(r,PP.pairing);
    element_init_G1(tmp,PP.pairing);
    element_random(r);
    element_pow_zn(C1,PP.P,r);
//    element_printf("P = %B\n",PP.P);
    element_pow_zn(tmp,pk,r);
//    element_printf("r * pk = %B\n",tmp);
//    element_printf("r = %B\n",r);
//    element_printf("pk = %B\n",pk);
    element_add(C2,M,tmp);
    element_clear(r);
    element_clear(tmp);
}

void EDec(element_t *M,element_t sk,element_t C1, element_t C2){
    element_t tmp;
    element_init_G1(tmp,PP.pairing);
    element_init_G1(tmp,PP.pairing);
    element_pow_zn(tmp,C1,sk);
//    element_printf("sk * C1 = %B\n",tmp);
//    element_printf("sk = %B\n",sk);
    element_sub(M,C2,tmp);
    element_clear(tmp);
}

void Ererand(element_t* C1_,element_t* C2_,element_t C1,element_t C2,element_t pk){

    element_t tmp1;
    element_t tmp2;
    element_t  r_;
    element_init_G1(tmp1,PP.pairing);
    element_init_G1(tmp2,PP.pairing);
    element_init_Zr(r_,PP.pairing);
    element_random(r_);
    element_mul_zn(tmp1,PP.P,r_);
    element_mul_zn(tmp2,pk,r_);
    element_add(C1_,C1,tmp1);
    element_add(C2_,C2,tmp2);
//    element_set(C1_,C1);
//    element_set(C2_,C2);

    element_clear(tmp1);
    element_clear(tmp2);
}

void EKM(element_t *pk_,element_t pk,element_t r){
    element_t tmp;
    element_init_G1(tmp,PP.pairing);
    element_pow_zn(tmp,PP.P,r);
    element_add(pk_,pk,tmp);
}

void H1(element_t *e,element_t g,char *m){
    element_t tmp;
    char buff[128];
    memset(buff,0, sizeof(buff));
    element_init_Zr(e,PP.pairing);
    element_init_Zr(tmp,PP.pairing);
    element_to_bytes_compressed(buff,g);
    strcat(buff,m);
    element_from_hash(e,buff, strlen(buff));
    element_clear(tmp);
}
void H2(element_t *e,char* m){
    element_init_G1(e,PP.pairing);
    element_from_hash(e,m, strlen(m));
}
void H3(unsigned char* h,unsigned char* m){
    genSHA256(h,m);
}
