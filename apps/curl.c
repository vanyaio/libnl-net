#include<stdio.h>

int main(int argc, char* argv[]){
  //sleep(20);
  system("rm -rf apps/tmp");
  system("mkdir apps/tmp");
  system("cp -r /etc/* apps/tmp");
  system("/bin/sh");
}
