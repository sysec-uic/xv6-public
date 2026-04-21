#include "types.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) {
  int i, fd;

  if(argc < 2){
    printf(2, "Usage: touch files...\n");
    exit();
  }

  for(i = 1; i < argc; i++){
    // O_CREATE: Create the file if it doesn't exist
    // O_WRONLY: Open for writing
    if((fd = open(argv[i], O_CREATE | O_WRONLY)) < 0){
      printf(2, "touch: cannot touch %s\n", argv[i]);
      continue;
    }
    close(fd);
  }
  exit();
}