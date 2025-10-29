#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
  int n = 10;

  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      int tickets = 50 * (i + 1);
      settickets(tickets);
      for (int j = 0; j < 100000000; j++);  // carga
      exit(0);
    }
  }

  for (int i = 0; i < n; i++) {
    wait(0);
  }

  exit(0);
}
