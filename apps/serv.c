#include<string.h>
#include<stdio.h>

int main(int argc, char* argv[]){
  char buf[100];
  strcpy(buf, "php -S ");
  strcat(buf, argv[2]);
  strcat(buf, ":8080 &");
  system(buf);

  exit(0);
}
