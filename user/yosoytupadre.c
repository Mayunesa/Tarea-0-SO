#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Mi PID es: %d\n", getpid());
  printf("El PID de mi padre es: %d\n", getppid());
  
  // Solo para Parte II (si implementas getancestor)
  printf("Ancestro nivel 0: %d\n", getancestor(0));
  printf("Ancestro nivel 1: %d\n", getancestor(1)); 
  printf("Ancestro nivel 2: %d\n", getancestor(2));
  
  exit(0);
}