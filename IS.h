
#ifndef SIGLE_SIGN_IS_H
#define SIGLE_SIGN_IS_H

typedef struct {
    int fdorport;
    void *m;
}message;

int initIS(int port);

void* ISreceive(int listenfd,int connfd, void* m);

void* ISreceive_thread(void* args);

void* ISsend(int port,void *m);

int closeIS(int listenfd);

#endif //SIGLE_SIGN_IS_H
