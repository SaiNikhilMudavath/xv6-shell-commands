// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };

#define  MAX_SIZE 256
int
main(void)
{
  int pid, wpid;

  if(open("console", O_RDWR) < 0){
    mknod("console", 1, 1);
    open("console", O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){

    char username[MAX_SIZE];char password[MAX_SIZE];
    int success=0;
    for(int i=0;i<3;i++)
    {
      printf(1,"Enter Username: ");
      int n=read(0,username,sizeof(username)-1);
      if(n>0 && username[n-1]=='\n') username[n-1]='\0';
      if(strcmp(username,USERNAME)!=0) continue;
      printf(1,"Enter Password: ");
      n=read(0,password,sizeof(password)-1);
      if(n>0 && password[n-1]=='\n') password[n-1]='\0';
      if(strcmp(password,PASSWORD)==0)
      {
        // printf(1,"Login successful\n");
        success=1;break;
      }
    }
    if(success==0)
    {
      while(1) sleep(100);
    }



    // printf(1, "init: starting sh\n");
    
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }
}
