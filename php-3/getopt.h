/* Borrowed from Apache NT Port */
#include "parser.h"

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

extern int getopt(int argc, char* const *argv, const char *optstr);
