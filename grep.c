#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <regex.h>

#define DEFAULT_LINE_NUM 5

void do_grep(FILE *fp, regex_t *preg);
void do_igrep(FILE *fp, regex_t *preg);
void die(const char *s);
void usage();

int main(int argc, char *argv[])
{
	regex_t preg;
	int err;
    int opt;
	char ignore_case = 0;
	char invert_match = 0;

    struct option longopt[] = {
        {"ignore-case", required_argument, NULL, 'i'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    while((opt = getopt_long(argc, argv, "ivh", longopt, NULL)) != -1) {
        switch(opt) {
        case 'i':
			ignore_case = 1;
            break;
        case 'v':
			invert_match = 1;
            break;
        case 'h':
			usage();
            exit(0);
        case '?':
			usage();
            exit(1);
        }
    }

	if (argc < optind) {
		usage();
		exit(1);
	}

	if (ignore_case) {
		err = regcomp(&preg, argv[optind], REG_EXTENDED | REG_NOSUB | REG_NEWLINE | REG_ICASE);
	} else {
		err = regcomp(&preg, argv[optind], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);
	}
	if (err != 0) {
		char buf[1024];
		regerror(err, &preg, buf, sizeof buf);
		puts(buf);
		exit(1);
	}

    if (argc == optind) {
        do_grep(stdin, &preg);
    } else {
        int i;
        for (i = optind + 1; i < argc; ++i) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                die(argv[i]);
            }

			if (invert_match == 1) {
				do_igrep(fp, &preg);
			} else {
				do_grep(fp, &preg);
			}
            fclose(fp);
        }
    }
	regfree(&preg);
    exit(0);
}

void do_grep(FILE *fp, regex_t *preg)
{
	char buf[4048];
    while(fgets(buf, sizeof buf, fp) != NULL) {
		if (regexec(preg, buf, 0, NULL, 0) == 0) {
			fputs(buf, stdout);
		}
    }
}

void do_igrep(FILE *fp, regex_t *preg)
{
	char buf[4048];
    while(fgets(buf, sizeof buf, fp) != NULL) {
		if (regexec(preg, buf, 0, NULL, 0) != 0) {
			fputs(buf, stdout);
		}
    }
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

void usage()
{
	fprintf(stdout, "Usage: grep [-i|--ignore-case] [-v|--invert-match] PATTERN [FILE]\n");
}
