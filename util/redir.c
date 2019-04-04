#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int redir_io(const char* path, int out){
  int fd = open(path, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
  close(out);
  return dup2(fd, out);
}

/*
int main(){
  printf("hello\n");

  if (fork() == 0){
    redir_io("out", 1);
    system("echo \"lol\n\"");

    while (1);
    exit(0);
  }

  sleep(2);
  printf("kek\n");
  printf("kyk\n");
  return 0;
}
*/
