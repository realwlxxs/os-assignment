#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

using namespace std;

void ipc_shm() {
  key_t key = ftok("./ipcid", 1);
  int shmid = shmget(key, BUFFER_SIZE, IPC_CREAT | 0666);
  char *shmaddr = static_cast<char *>(shmat(shmid, NULL, 0));
  printf("%s\n", shmaddr);
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
  msgrcv(msgid, &m, sizeof(m) - sizeof(m.type), m.type, 0);
  printf("%s\n", m.text);
}

void ipc_fifo() {
  ifstream fifo;
  fifo.open("./fifo", ios::in);
  string s;
  while (fifo.good()) {
    getline(fifo, s);
    cout << s << endl;
  }
}

void ipc_socket() {
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  sockaddr server_addr;
  server_addr.sa_family = AF_UNIX;
  strcpy(server_addr.sa_data, "\0./socket");
  connect(sockfd, &server_addr, sizeof(server_addr));
  char buf[BUFFER_SIZE];
  read(sockfd, buf, sizeof(buf));
  cout << buf;
  close(sockfd);
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  if (!strcmp(argv[1], "shm")) {
    ipc_shm();
  } else if (!strcmp(argv[1], "mq")) {
    ipc_mq();
  } else if (!strcmp(argv[1], "fifo")) {
    ipc_fifo();
  } else if (!strcmp(argv[1], "socket")) {
    ipc_socket();
  }
}
