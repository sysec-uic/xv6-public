#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

#define TARGET_BLOCKS 16523
#define TESTFILE "bigfile.data"

static void
fail(char *msg)
{
  printf(1, "FAIL: %s\n", msg);
  exit();
}

static void
fill_block(char *buf, int blockno)
{
  int i;

  for(i = 0; i < BSIZE; i++)
    buf[i] = blockno + i;
}

static int
check_block(char *buf, int blockno)
{
  int i;

  for(i = 0; i < BSIZE; i++){
    if((uchar)buf[i] != (uchar)(blockno + i))
      return -1;
  }
  return 0;
}

static void
write_and_verify_round(int round)
{
  int fd, i, cc;
  char buf[BSIZE];

  unlink(TESTFILE);

  fd = open(TESTFILE, O_CREATE | O_RDWR);
  if(fd < 0)
    fail("cannot create test file");

  for(i = 0; i < TARGET_BLOCKS; i++){
    fill_block(buf, i);
    cc = write(fd, buf, sizeof(buf));
    if(cc != sizeof(buf)){
      printf(1, "FAIL: wrote %d blocks, expected %d (round %d)\n",
             i, TARGET_BLOCKS, round);
      close(fd);
      unlink(TESTFILE);
      exit();
    }
  }

  close(fd);

  fd = open(TESTFILE, O_RDONLY);
  if(fd < 0)
    fail("cannot reopen test file");

  for(i = 0; i < TARGET_BLOCKS; i++){
    cc = read(fd, buf, sizeof(buf));
    if(cc != sizeof(buf)){
      printf(1, "FAIL: short read at block %d in round %d\n", i, round);
      close(fd);
      unlink(TESTFILE);
      exit();
    }
    if(check_block(buf, i) < 0){
      printf(1, "FAIL: wrong data at block %d in round %d\n", i, round);
      close(fd);
      unlink(TESTFILE);
      exit();
    }
  }

  close(fd);
  if(unlink(TESTFILE) < 0)
    fail("cannot unlink test file");
}

int
main(int argc, char *argv[])
{
  printf(1, "bigfile: writing %d blocks\n", TARGET_BLOCKS);

  write_and_verify_round(1);
  write_and_verify_round(2);

  printf(1, "PASS\n");
  exit();
}
