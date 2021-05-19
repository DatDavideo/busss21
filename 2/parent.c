#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <spawn.h>

extern char **environ; // standard libc process environment

int main(void) {
  pid_t child1;
  char const* args[] = { "./child1", NULL };
  int ret = posix_spawn(&child1, *args, NULL, NULL, (char* const*)args, environ);
  if(ret != 0) {
    fprintf(stderr, "Could not start child process 1!\n");
    abort();
  }
  
  pid_t child2;
  char const* args2[] = { "./child2", NULL };
  int ret2 = posix_spawn(&child2, *args2, NULL, NULL, (char* const*)args2, environ);
  if(ret2 != 0) {
    fprintf(stderr, "Could not start child process 2!\n");
    abort();
  }

  while(true) {
    write(fileno(stdout), "B", 1);
  }
}
