
#ifndef SIGLE_SIGN_USER_H
#define SIGLE_SIGN_USER_H
#include "Global.h"

int initUser(int port);

void* Userreceive(int listenfd, void* m);

void* Usersend(int port,void *m);

int closeUser(int listenfd);


#endif //SIGLE_SIGN_USER_H
