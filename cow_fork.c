#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char * argv[])
{
  printf(1, "Before malloc: %d\n", freepages());
  char * x = malloc(500000);
  memset(x, '?', 500000);
  printf(1, "After malloc: %d %c\n", freepages(), x[100000]);

  if (fork() == 0) {
    printf(1, "Child: Before memset: %d\n", freepages());
    memset(x, 'C', 500000);
    printf(1, "Child: After  memset: %d %c\n", freepages(), x[100000]);
  } else {
    sleep(30);
    printf(1, "Parent: Before memset: %d\n", freepages());
    memset(x, 'P', 500000);
    printf(1, "Parent: After  memset: %d %c\n", freepages(), x[100000]);
    wait();
    printf(1, "Parent: After   child: %d\n", freepages());
  }
  exit();
}
