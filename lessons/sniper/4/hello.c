#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	char buffer[64];
	printf("%p", buffer);
	gets(buffer);
	return 0;
}

