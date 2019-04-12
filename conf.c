#include "conf.h"

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

  return 0;
}

char** conf_reaper_arg(struct conf_file* conf, pid_t* node_pids){
  char** argv;
  argv = malloc((conf->node_cnt + 2) * sizeof(char*));
  argv[0] = conf->reaper;

  char* buff[conf->node_cnt];
  for (int i = 1; i <= conf->node_cnt; i++){
    buff[i-1] = malloc(BUFF_SIZE * sizeof(char));
    sprintf(buff[i-1], "%d", node_pids[i-1]);
    argv[i] = buff[i-1];
    //
    //printf("%s\n", argv[i]);
    //
  }

  argv[conf->node_cnt + 1] = NULL;
  return argv;
}

char* conf_node_task_arg(struct node_entry* entry){
  char** argv;
  argv = malloc(4 * sizeof(char*));
  argv[0] = entry->task;
  argv[1] = entry->ip_addr;
  argv[2] = entry->hostname;
  argv[3] = NULL;
  return argv;
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

ssize_t getline_no_nl(char **lineptr, size_t *n, FILE *stream){
  ssize_t ret = getline(lineptr, n, stream);
  if ((*lineptr)[ret - 1] == '\n') {
     (*lineptr)[ret - 1] = '\0';
  }
  return ret;
}

int conf_set_etc_hosts(struct conf_file* conf){
  system("rm -rf tmp");
  system("mkdir tmp");
  system("cp -r /etc/* tmp");
  system("mount --bind tmp /etc");

  system("echo \"127.0.0.1 localhost\" > /etc/hosts");
  for (int i = 0; i < conf->node_cnt; i++){
    char buff[BUFF_SIZE];
    strcpy(buff, "echo \"");
    char* addr_no_mask = del_mask(conf->entries[i].ip_addr);

    strcat(buff, addr_no_mask);
    strcat(buff, " ");
    strcat(buff, conf->entries[i].hostname);
    strcat(buff, "\" >> /etc/hosts");
    system(buff);

    free(addr_no_mask);
  }

  return 1;
}

char* del_mask(char* addr){
  char* buff = malloc(BUFF_SIZE * sizeof(char));
  char* start = buff;
  while ((*addr) != '/'){
    *buff = *addr;
    buff++;
    addr++;
  }
  *buff = '\0';
  return start;
}




















//
