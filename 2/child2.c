#include <stdbool.h>
#include <stdio.h>

int main(void) {
  while(true) {
    fprintf(stdout, "%s", "C");
    fflush(stdout);
  }
}
