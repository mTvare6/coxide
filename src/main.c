#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/syslimits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include "shell.h"


#define STATIC_DB 0


typedef struct {
  char *path;
  size_t freq;
} path_rate_t;

const char *easycat(const char *l, const char *r);

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
      if(data_home==NULL){perror("malloc() returned NULL");}
      strcpy(data_home, homedir);
      strcat(data_home, "/.local/share/coxide/");
    } else{
      if(realloc(data_home, strlen(data_home)+9)==NULL) {perror("realloc returned NULL");} // /coxide/\0 = 9
      strcat(data_home, "/coxide/");
    }
    const char* DBFILE = easycat(data_home, "main.db");
    const char* TMPDBFILE = easycat(data_home, "tmp.db");
#else
#define DBFILE "/Users/me/.local/share/coxide/main.db"
#define TMPDBFILE  "/Users/me/.local/share/coxide/tmp.db"
#endif

    mkdir(data_home, S_IRWXU | S_IRWXG | S_IRWXO);
    FILE *dbfp = fopen(DBFILE, "r");
    bool new_db=false;
    FILE *tmpdbfp = fopen(TMPDBFILE, "w");

    if(dbfp==NULL){
      new_db=true;
      dbfp=fopen(DBFILE, "w+");
    }
    if(tmpdbfp==NULL){
      tmpdbfp=fopen(TMPDBFILE, "w");
    }
    if(dbfp==NULL||tmpdbfp==NULL){
      fputs("Unable to open file", stderr);
      exit(2);
    }

    if(argv[1][0]=='-'){

      char *resolved_path = malloc(sizeof(char)*PATH_MAX);
      realpath(argv[2], resolved_path);
      if(new_db){
        fprintf(dbfp, "1,%s\n", resolved_path);
        return 0;
      }
      path_rate_t buf = {malloc(sizeof(char)*PATH_MAX), 1};
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
        return 0;
      }
      size_t i=0, li=0;
      path_rate_t most = {malloc(sizeof(char)*PATH_MAX), 0};
      path_rate_t buf = {malloc(sizeof(char)*PATH_MAX), 1};

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
      if(li!=0){
        puts(most.path);
      } else{
        puts(homedir);
      }
    }
    rename(TMPDBFILE, DBFILE);
#if(STATIC_DB==0)
    free((void *)DBFILE);
    free((void *)TMPDBFILE);
#endif
    fclose(dbfp);
  }
  return 0;
}

const char* easycat(const char *l, const char *h)
{
    char *comb = malloc(strlen(l) + strlen(h) + 1);
    if(comb==NULL){perror("malloc()");}
    strcpy(comb, l);
    strcat(comb, h);
    return comb;
}

