typedef enum {
  false, true } boolean;

#include <stdio.h>

int main()
{
  _Bool i;
  //printf("%ld\n", sizeof(boolean));
  i=0;
  printf("%d\n", i);
  i=1;
  printf("%d\n", i);
  printf("%ld\n", sizeof(i));
}
