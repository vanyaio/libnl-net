#include"../../conf.h"

int main(int argc, char const *argv[]) {
  char* buff[BUFF_SIZE];
  strcpy(buff, "python3 master.py ");
  for (int i = 1; i < 4; i++){
    strcat(buff, argv[i]);
    strcat(buff, " ");
  }
  system(buff);
  //fork wait...
  return 0;
}
