#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if(argc != 3){
    printf(2, "Usage: move old new\n");
    exit();
  }
  if(link(argv[1], argv[2]) < 0)
    printf(2, "move: link failed\n");
  if(unlink(argv[1]) < 0)
    printf(2, "move: unlink failed\n");
  exit();
}