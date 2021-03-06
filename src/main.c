#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#if defined(__linux__)
#include <linux/limits.h>
#else
#include <sys/syslimits.h>
#endif
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include "shell.h"


#define STATIC_DB 0
#define PATH_ALLOC_SIZE PATH_MAX*2 // PATH_MAX is a standard implmented by the kernel but isn't imposed??
#define MALLOC_FAILED EXIT_FAILURE

typedef struct {
  char *path;
  size_t freq;
} path_rate_t;

const char *easycat(const char *l, const char *r, size_t hlen);

int main (int argc, char *argv[]){
  if(argc>1){
    char *data_home = getenv("XDG_DATA_HOME");

    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
      homedir = getpwuid(getuid())->pw_dir;
    }
#if(STATIC_DB==0)
    if(data_home==NULL){
      data_home=malloc(strlen(homedir)+23); // /.local/share/coxide/\0 = 23
      if(data_home==NULL){
        perror("malloc() returned NULL");
        exit(MALLOC_FAILED);
      }
      strcpy(data_home, homedir);
      strcat(data_home, "/.local/share/coxide/");
    } else{
      if(realloc(data_home, strlen(data_home)+9)==NULL) {
        perror("realloc() returned NULL");
        exit(MALLOC_FAILED);
      } // /coxide/\0 = 9
      strcat(data_home, "/coxide/");
    }
    size_t data_home_len = strlen(data_home);
    const char* dbfile = easycat(data_home, "main.csv", data_home_len);
    const char* TMPDBFILE = easycat(data_home, "tmp.csv", data_home_len);
#else
const char *dbfile = "/Users/me/.local/share/coxide/main.csv";
const char *tmpdbfile =  "/Users/me/.local/share/coxide/tmp.csv";
#endif

    mkdir(data_home, S_IRWXU | S_IRWXG | S_IRWXO);
    FILE *dbfp = fopen(dbfile, "r");
    bool new_db=false;
    FILE *tmpdbfp = fopen(TMPDBFILE, "w");

    if(dbfp==NULL){
      new_db=true;
      dbfp=fopen(dbfile, "w+");
    }
    if(tmpdbfp==NULL){
      tmpdbfp=fopen(TMPDBFILE, "w");
    }
    if(dbfp==NULL){
      perror("Cannot open dbfile");
      exit(2);
    }
    if(tmpdbfp==NULL){
      perror("Cannot open tmpdbfile");
      exit(2);
    }

    if(argv[1][0]=='-'){

      char *resolved_path = malloc(sizeof(char)*PATH_ALLOC_SIZE);
      realpath(argv[2], resolved_path);
      if(new_db){
        fprintf(dbfp, "1,%s\n", resolved_path);
        return EXIT_SUCCESS;
      }
      path_rate_t buf = {malloc(sizeof(char)*PATH_ALLOC_SIZE), 1};
      bool prev_exist=false;
      while(fscanf(dbfp, "%zu,%s", &buf.freq, buf.path)==2){
        if(strcmp(buf.path, resolved_path)==0 || buf.path[0]=='\0'){
          fprintf(tmpdbfp, "%zu,%s\n", buf.freq+1, buf.path);
          prev_exist=true;
        }else{
          fprintf(tmpdbfp, "%zu,%s\n", buf.freq, buf.path);
        }
      }
      if(!prev_exist){
        fprintf(tmpdbfp, "1,%s\n", resolved_path);
      }
      rename(TMPDBFILE, dbfile);
    } else if(strncmp(argv[1], "shell", 5)==0){
      if(argc>2){
        if(strncmp(argv[2], "zsh", 3)==0){
          posixsh();
        } else if(strncmp(argv[2], "sh", 2)==0){
          posixsh();
        } else if(strncmp(argv[2], "bash", 4)==0){
          posixsh();
        } else if(strncmp(argv[2], "bash", 4)==0){
          posixsh();
        } else if(strncmp(argv[2], "elvish", 6)){
          elvish();
        } else if(strncmp(argv[2], "xonsh", 5)){
          xonsh();
        } else if(strncmp(argv[2], "fish", 4)){
          fish();
        }
      }
    }
    else{
      if(new_db){
        fputs("DBFILE was empty, use valid paths to cache directories", stderr);
        return EXIT_FAILURE;
      }
      size_t i=0, li=0;
      path_rate_t most = {malloc(sizeof(char)*PATH_ALLOC_SIZE), 0};
      path_rate_t buf = {malloc(sizeof(char)*PATH_ALLOC_SIZE), 1};

      while(fscanf(dbfp, "%zu,%s", &buf.freq, buf.path)==2){
        i++;
        if(buf.freq>most.freq){
          if(strstr(buf.path, argv[1])){
            //NOTE: dest is as large as source, do I need to use stpncpy
            stpcpy(most.path, buf.path);
            most.freq=buf.freq;
            li=i;
          }
        }
      }
      if(li!=0){
        i=0;
        fseek(dbfp, i, SEEK_SET);
        while(fscanf(dbfp, "%zu,%s", &buf.freq, buf.path)==2){
          i++;
          if(li==i){
            fprintf(tmpdbfp, "%zu,%s\n", most.freq+1, most.path);
          } else {
            fprintf(tmpdbfp, "%zu,%s\n", buf.freq, buf.path);
          }
        }
        rename(TMPDBFILE, dbfile);
        puts(most.path);
      } else{
        puts(homedir);
      }
    }
#if(STATIC_DB==0)
    free((void *)dbfile);
    free((void *)TMPDBFILE);
#endif
    fclose(dbfp);
  }
  return EXIT_SUCCESS;
}

const char* easycat(const char *l, const char *h, size_t hlen)
{
    char *comb = malloc(hlen + strlen(h) + 1);
    if(comb==NULL){
      perror("malloc()");
      exit(MALLOC_FAILED);
    }
    strcpy(comb, l);
    strcat(comb, h);
    return comb;
}

