#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define INDEX_FILE ".jump_index"

void crawl(char *path, int out_fd) {
  char *buf = malloc(512); 
  if(buf == 0) return; // Out of memory

  char *p;
  int fd;
  //directory
  struct dirent de;
  //file type
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    free(buf);
    return;
  }

  if (fstat(fd, &st) < 0 || st.type != T_DIR) {
    free(buf);
    close(fd);
    return;
  }

  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    // Reset buffer to current directory path
    strcpy(buf, path);
    int len = strlen(buf);
    p = buf + len;

    //handle slashes for the root directory
    if (len > 0 && buf[len - 1] != '/') {
      *p++ = '/';
    }

    // Copy filename and null-terminate
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if (stat(buf, &st) < 0) continue;

    if (st.type == T_FILE) {
      // if its a file, just add the path and move on
      printf(out_fd, "%s %s\n", de.name, path);
    } 
    else if (st.type == T_DIR) {
      // If a direcory, keep going
      crawl(buf, out_fd);
    }
  }

  free(buf);
  close(fd);
}

int main(void) {
  int out_fd = open(INDEX_FILE, O_CREATE | O_WRONLY);
  if (out_fd < 0) {
    printf(2, "crawler: cannot open index for writing\n");
    exit();
  }

  crawl("/", out_fd);

  close(out_fd);
  exit();
}