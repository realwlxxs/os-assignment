#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 1024

using namespace std;

void ipc_shm() {
  key_t key = ftok("./ipcid", 1);
  int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | 0666);
  char *shmaddr = static_cast<char *>(shmat(shmid, NULL, 0));
  printf("%s\n", shmaddr);
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  if (!strcmp(argv[1], "shm")) {
    ipc_shm();
  }
}
