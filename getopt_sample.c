#include <stdio.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int opt;
	while((opt = getopt(argc, argv, "af:tx")) != -1) {
		switch(opt) {
		case 'a':
			printf("found arg 'a'\n");
			break;
		case 'f':
			printf("found arg 'f':%s\n", optarg);
			break;
		case 't':
			printf("found arg 't'\n");
			break;
		case 'x':
			printf("found arg 'x'\n");
			break;
		case '?':
			printf("invalid arg\n");
			break;
		}
	}
}
