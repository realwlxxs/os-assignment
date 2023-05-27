#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

void ipc_fifo() {
  mkfifo("./fifo", 0666);
  ofstream fifo;
  fifo.open("./fifo", ios::out);
  string s;
  while (getline(cin, s)) {
    fifo << s << endl;
  }
}

void ipc_socket() {
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  sockaddr server_addr;
  server_addr.sa_family = AF_UNIX;
  strcpy(server_addr.sa_data, "\0./socket");
  bind(sockfd, &server_addr, sizeof(server_addr));
  listen(sockfd, 1);
  int connect_fd = accept(sockfd, NULL, NULL);
  string s;
  while (getline(cin, s)) {
    write(connect_fd, s.c_str(), s.size());
  }
  close(sockfd);
  close(connect_fd);
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
