#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define BUFFER_SIZE 1024

using namespace std;

void ipc_shm() {
  key_t key = ftok("./ipcid", 1);
  int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | 0666);
  char *shmaddr = static_cast<char *>(shmat(shmid, NULL, 0));
  string s;
  while (getline(cin, s)) {
    strcpy(shmaddr, s.c_str());
  }
}

struct msg {
  long type;
  char text[BUFFER_SIZE];
};

void ipc_mq() {
  key_t key = ftok("./ipcid", 2);
  int msgid = msgget(key, IPC_CREAT | 0666);
  msg m;
  m.type = 1;
  string s;
  while (getline(cin, s)) {
    strcpy(m.text, s.c_str());
    msgsnd(msgid, &m, sizeof(m) - sizeof(m.type), 0);
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  if (!strcmp(argv[1], "shm")) {
    ipc_shm();
  } else if (!strcmp(argv[1], "mq")) {
    ipc_mq();
  }
}
