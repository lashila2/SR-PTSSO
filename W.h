
#ifndef SIGLE_SIGN_W_H
#define SIGLE_SIGN_W_H

int initW(int port);

void* Wreceive(int listenfd, void* m);

void* Wsend(int port,void *m);

int closeW(int listenfd);

#endif //SIGLE_SIGN_W_H
