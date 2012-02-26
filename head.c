#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define DEFAULT_LINE_NUM 5

void do_head(FILE *fp, int n);
void die(const char *s);

int main(int argc, char *argv[])
{
	int opt;
	struct option longopt[] = {
		{"lines", required_argument, NULL, 'n'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};
	int nLines = DEFAULT_LINE_NUM;

	while((opt = getopt_long(argc, argv, "hn:", longopt, NULL)) != -1) {
		switch(opt) {
		case 'n':
			nLines = atoi(optarg);
			break;
		case 'h':
			fprintf(stdout, "Usage: %s [-n|--lines LINES] [FILE ...]\n", argv[0]);
			exit(0);
		case '?':
			fprintf(stdout, "Usage: %s [-n|--lines LINES] [FILE ...]\n", argv[0]);
			exit(1);
		}
	}

	if (optind == argc) {
		do_head(stdin, nLines);
	} else {
		int i;
		int nFiles = argc - optind;
		for (i = optind; i < argc; ++i) {
			FILE *fp = fopen(argv[i], "r");
			if (fp == NULL) {
				die(argv[i]);
			}
			if (nFiles > 1) {
				if (i > optind) {
					putchar('\n');
				}
				fprintf(stdout, "==> %s <==\n", argv[i]);
			}
			do_head(fp, nLines);
			fclose(fp);
		}
	}
	exit(0);
}

void do_head(FILE *fp, int n)
{
	int c;

	if (n <= 0) return;

	while((c = getc(fp)) != EOF) {
		if (putchar(c) < 0) exit(1);
		if (c == '\n') {
			--n;
			if(n == 0) return;
		}
	}
}

void die(const char *s)
{
	perror(s);
	exit(1);
}
