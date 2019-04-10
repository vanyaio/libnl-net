#include<stdio.h>

#define NODE_ENTRY_FIELDS 3
struct node_entry{
  char* ip_addr;
  char* hostname;
  char* task;
};
struct conf_file{
  char* reaper;
  int node_cnt;
  struct node_entry* entries;
};

ssize_t getline_no_nl(char **lineptr, size_t *n, FILE *stream){
 ssize_t ret = getline(lineptr, n, stream);
 if ((*lineptr)[ret - 1] == '\n') {
     (*lineptr)[ret - 1] = '\0';
 }
 return ret;
}

int conf_alloc(struct conf_file** conf, FILE* fp){
  *conf = malloc(sizeof(struct conf_file));

  char* buff = NULL;
  size_t len = 0;
  getline_no_nl(&buff, &len, fp);
  int node_cnt = atoi(buff);
  free(buff);

  (*conf)->node_cnt = node_cnt;
  (*conf)->entries = malloc(sizeof(struct node_entry) * node_cnt);
  for (int i = 0; i < node_cnt; i++){
    (*conf)->entries[i].ip_addr = NULL;
    (*conf)->entries[i].hostname = NULL;
    (*conf)->entries[i].task = NULL;

    len = 0;
    getline_no_nl(&((*conf)->entries[i].ip_addr), &len, fp);

    len = 0;
    getline_no_nl(&((*conf)->entries[i].hostname), &len, fp);

    len = 0;
    getline_no_nl(&((*conf)->entries[i].task), &len, fp);
  }

  len = 0;
  (*conf)->reaper = NULL;
  getline_no_nl(&((*conf)->reaper), &len, fp);
  printf("kek\n");
}

void print_conf_file(struct conf_file* conf){
  printf("%d\n", conf->node_cnt);
  for (int i = 0; i < conf->node_cnt; i++){
    printf("%s\n", conf->entries[i].ip_addr);
    printf("%s\n", conf->entries[i].hostname);
    printf("%s\n", conf->entries[i].task);
  }
  printf("%s\n", conf->reaper);
}
int main(int argc, char* argv[]){

  /*
  FILE *fp = fopen(argv[1], "r");
  char* buff = NULL;
  size_t len = 0;
  while (getline_no_nl(&buff, &len, fp) > 0){
      printf("(%s)\n", buff);
      free(buff);
      len = 0;
  }
  */

  printf("(%s)\n", argv[1]);
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
    printf("wat\n");
  struct conf_file* conf = NULL;
  conf_alloc(&conf, fp);
  print_conf_file(conf);
}



























//gcc -g -O0 p1.c -o p1
