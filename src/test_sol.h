/*****************************************************************************/
/*                                                                           */
/* Version:  3.0    Date:  19/02/2004-06/09/2022   File: test_sol.h          */
/* Author:  Marco Chiarandini                                                */
/* email: machud@intellektik.informatik.tu-darmstadt.de                      */
/*                                                                           */
/*****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define LINE_BUF_LEN     2000

#define VERBOSE 1

#ifdef NOGETOPT
static char name_buf[LINE_BUF_LEN];
static char sol_buf[LINE_BUF_LEN];
//static int     opt;
#endif

#define TRUE 1
#define FALSE 0

#define ASCII_FORMAT	1
#define BIN_FORMAT	2

#define MAX_PREAMBLE 10000

/* If you change MAX_NR_VERTICES, change MAX_NR_VERTICESdiv8 to be
the 1/8th of it */
//#define NMAX 10000	/* maximum number of vertices handles */
#define MAX_NR_VERTICES		5000	/* = NMAX */
#define MAX_NR_VERTICESdiv8	625 	/* = NMAX/8 */

extern int **solution_sets;
extern int *solution_sizes;

extern  long int NUM_COL;
extern  long int MAX_COL;
extern  long int MIN_COL;
extern  long int NUM_EDGES;
extern  long int NUM_NODES;
extern float DENSITY;
extern long int COLORED_NODES;
extern  long int CONF_EDGES;
extern  long int CONF_NODES;
extern  long int NOT_IN_LIST;
extern  long int SET_SIZE;
extern  long int SUM_COL;

extern const char *instance_name;
extern const char *solution_name;
extern int problem;


char Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
char Preamble[MAX_PREAMBLE];

void usage();
void checkOptions(const char *const program_name, const int argc, char **const argv);
int parse_options (const char *const program_name, const int argc, char **const argv);

int get_params(void);
char get_edge(  int i,  int j );
void read_graph_DIMACS_bin(const char *file);
void write_graph_DIMACS_ascii(const char *file);


char checkConstraints();
void checkConstraints_ascii();
void checkConstraints_bin();
void readSolution();

