//Armando Silva, asilva3

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <utime.h>
#include <errno.h>

int main(int argc, char **argv){   
struct stat info;
struct tm *ftime;
time_t mod_time;
char rtime[80];
int exit_status = EXIT_SUCCESS;
char *progname = argv[0];
int epoch_time= 15552000;
time_t curr_time;
char buf[80];

if(argc == 1){
 //Runs through the file that is called
   lstat(".", &info);
   time(&curr_time);
   mod_time = info.st_mtime;
   ftime = localtime(&mod_time);

//Checks the time, month, year   
   if(mod_time > curr_time + epoch_time ||
             mod_time < curr_time - epoch_time)
      strftime(rtime, sizeof(rtime), "%b %e %Y", ftime);
   else{
      strftime(rtime, sizeof(rtime), "%b %e %R", ftime);
      printf("%6o %9d %s\n", info.st_mode, (int)info.st_size, rtime);   
   }

}else{
 //Runs through each of the files
   for(int i= 1; i<argc; ++i){
      char *filename = argv[i];
 //Runs to check a symbolic link
      if(lstat(filename, &info) == -1){
         fprintf(stderr, "%s: %s: %s\n", progname, filename,
            strerror(errno));
         exit_status = EXIT_FAILURE;
         continue;
      }if(readlink (filename, buf, sizeof(buf)) != -1)
         printf("%6o %9d %s %s\n", info.st_mode, (int) info.st_size,
               rtime, buf);
      
      printf("%6o %9d %s %s\n" , info.st_mode, (int) info.st_size, 
               rtime, filename);
      time(&curr_time);
      mod_time = info.st_mtime;
      ftime = localtime(&mod_time);
 //Checks the time, month ,year 
      if(mod_time > curr_time + epoch_time || 
            mod_time < curr_time - epoch_time)
         strftime(rtime, sizeof(rtime), "%b %e %Y", ftime);
      else
         strftime(rtime, sizeof(rtime), "%b %e %R", ftime);
   }
 }return EXIT_SUCCESS;
}

