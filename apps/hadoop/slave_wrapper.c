#include"../../conf.h"

int main(int argc, char const *argv[]) {
  char* buff[BUFF_SIZE];
  strcpy(buff, "python3 slave.py ");
  for (int i = 1; i < 5; i++){
    strcat(buff, argv[i]);
    strcat(buff, " ");
  }
  system(buff);
  return 0;
}
