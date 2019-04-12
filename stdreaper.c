#include <sys/types.h>
#include <sys/wait.h>
//gcc stdreaper.c -o stdreaper
int main(int argc, char* argv[]){
  int status;
  while(wait(&status) != -1){
    if (status != 0)
      exit(1);
  }
  exit(0);
}
