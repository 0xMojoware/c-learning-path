#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


void win()
{
  printf("code flow successfully changed\n");
}

int main(void)
{
  char buffer[64];

  gets(buffer);
}
