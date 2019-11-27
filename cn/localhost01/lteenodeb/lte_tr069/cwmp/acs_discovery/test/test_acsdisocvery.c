#include <stdio.h>
#include <stdlib.h>


int main()
{
	char *URL = (char *)acs_discovery('s');
	ENB_PRINTF ("THE URL IS %s\n", URL);
	return 0;
}
