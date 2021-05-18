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
    fprintf(stderr, "Could not start child process!\n");
    abort();
  }

  while(true) {
    write(fileno(stdout), "B", 1);
  }
}
