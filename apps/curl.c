#include<stdio.h>

int main(int argc, char* argv[]){
  printf("   %s         \n\n\n\n\n", argv[3]);
  system("/bin/sh");
  exit(0);
}
