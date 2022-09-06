/*****************************************************************************/
/*                                                                           */
/* Version:  3.0   Date:  06/09/2022   File: test_sol.c                      */
/* Author:  Marco Chiarandini                                                */
/* email: marco@imada.sdu.dk                                                 */
/*                                                                           */
/*****************************************************************************/

#include "test_sol.h"

#ifndef STR_ERR_UNKNOWN_LONG_OPT
# define STR_ERR_UNKNOWN_LONG_OPT   "%s: unrecognized option `--%s'\n"
#endif

#ifndef STR_ERR_LONG_OPT_AMBIGUOUS
# define STR_ERR_LONG_OPT_AMBIGUOUS "%s: option `--%s' is ambiguous\n"
#endif

#ifndef STR_ERR_MISSING_ARG_LONG
# define STR_ERR_MISSING_ARG_LONG   "%s: option `--%s' requires an argument\n"
#endif

#ifndef STR_ERR_UNEXPEC_ARG_LONG
# define STR_ERR_UNEXPEC_ARG_LONG   "%s: option `--%s' doesn't allow an argument\n"
#endif

#ifndef STR_ERR_UNKNOWN_SHORT_OPT
# define STR_ERR_UNKNOWN_SHORT_OPT  "%s: unrecognized option `-%c'\n"
#endif

#ifndef STR_ERR_MISSING_ARG_SHORT
# define STR_ERR_MISSING_ARG_SHORT  "%s: option `-%c' requires an argument\n"
#endif

#define STR_HELP_INSTANCE			\
  "  -i, --instance ARG    the instance file\n"

#define STR_HELP_SOLUTION			\
  "  -s, --solution ARG    the solution file\n"

#define STR_HELP_PROBLEM				\
  "  -p, --problem ARG     the problem -- see help\n"

#define STR_HELP_HELP						\
  "      --help            display this help text and exit\n"

#define STR_HELP				\
  STR_HELP_INSTANCE				\
  STR_HELP_SOLUTION				\
  STR_HELP_PROBLEM				\
  STR_HELP_HELP

/* Set to 1 if option --instance (-i) has been specified.  */
char opt_instance;

/* Set to 1 if option --solution (-s) has been specified.  */
char opt_solution;

/* Set to 1 if option --problem (-t) has been specified.  */
char opt_problem;

/* Set to 1 if option --help has been specified.  */
char opt_help;

/* Parse command line options.  Return index of first non-option argument,
   or -1 if an error is encountered.  */

/*************************************************************
 ************** utilities part ********************************
 ************************************************************/

void usage() {
  fprintf(stderr, "\nSolution Verifier for Graph Coloring, V2.6\n\n");
  fprintf(stderr,
	  "Usage: test_sol -i [INST_FILE] -s [SOL_FILE] -p [PROB_TYPE]\n");
  fprintf(stderr,
	  "--help            -h  \t help: prints this information\n\n");
  fprintf(stderr,
	  "Instance File Format: DIMACS file format. Nodes start from 1.\n");
  fprintf(stderr,
	  "Solution File Format: colors assigned to nodes wrote in column.\n");
  fprintf(stderr,
	  "After each entry the character \\n (new line) has to be printed.\n");
  fprintf(stderr,
	  "Colors start from 1 and the first color in the column represent\n");
  fprintf(stderr, "the color assigned to node 1. \n\n");
  fprintf(stderr,
	  "The Problem: coloring 1, list coloring 2, set coloring 3,\n");
  fprintf(stderr,
	  "             T-coloring 4, set T-coloring 5, interval-col 6\n");
  fprintf(stderr, "Report bugs to <marco@imada.sdu.dk>\n\n");
}

void checkOptions(const char * const program_name, const int argc,
		  char ** const argv) {
  int r = parse_options(program_name, argc, argv);

  if (r < 0) {
    fprintf(stderr,
	    "test_sol: You must specify input files and type of problem\n");
    fprintf(stderr, "Try `test_sol --help' for more information.\n");
    exit(1);
  }
  if (opt_help) {
    usage();
    exit(1);
  }
  if (!opt_instance || !opt_solution || !opt_problem) {
    fprintf(stderr,
	    "test_sol: You must specify input files and type of problem\n");
    fprintf(stderr, "Try `test_sol --help' for more information.\n");
    exit(1);
  }
}

int parse_options(const char * const program_name, const int argc,
		  char ** const argv) {
  static const char * const optstr__instance = "instance";
  static const char * const optstr__solution = "solution";
  static const char * const optstr__problem = "problem";
  static const char * const optstr__help = "help";
  int i = 0;
  opt_instance = 0;
  opt_solution = 0;
  opt_problem = 0;
  opt_help = 0;
  instance_name = 0;
  solution_name = 0;
  problem = 0;
  while (++i < argc) {
    const char *option = argv[i];
    if (*option != '-')
      return i;
    else if (*++option == '\0')
      return i;
    else if (*option == '-') {
      const char *argument;
      size_t option_len;
      ++option;
      if ((argument = strchr(option, '=')) == option)
	goto error_unknown_long_opt;
      else if (argument == 0)
	option_len = strlen(option);
      else
	option_len = argument++ - option;
      switch (*option) {
      case '\0':
	return i + 1;
      case 'h':
	if (strncmp(option + 1, optstr__help + 1, option_len - 1)
	    == 0) {
	  if (argument != 0) {
	    option = optstr__help;
	    goto error_unexpec_arg_long;
	  }
	  opt_help = 1;
	  return i + 1;
	}
	goto error_unknown_long_opt;
      case 'i':
	if (strncmp(option + 1, optstr__instance + 1, option_len - 1)
	    == 0) {
	  if (argument != 0)
	    instance_name = argument;
	  else if (++i < argc)
	    instance_name = argv[i];
	  else {
	    option = optstr__instance;
	    goto error_missing_arg_long;
	  }
	  opt_instance = 1;
	  break;
	}
	goto error_unknown_long_opt;
      case 'p':
	if (strncmp(option + 1, optstr__problem + 1, option_len - 1)
	    == 0) {
	  if (argument != 0)
	    problem = atoi(argument);
	  else if (++i < argc)
	    problem = atoi(argv[i]);
	  else {
	    option = optstr__problem;
	    goto error_missing_arg_long;
	  }
	  opt_problem = 1;
	  break;
	}
	goto error_unknown_long_opt;
      case 's':
	if (strncmp(option + 1, optstr__solution + 1, option_len - 1)
	    == 0) {
	  if (argument != 0)
	    solution_name = argument;
	  else if (++i < argc)
	    solution_name = argv[i];
	  else {
	    option = optstr__solution;
	    goto error_missing_arg_long;
	  }
	  opt_solution = 1;
	  break;
	}
      default:
      error_unknown_long_opt: fprintf(stderr,
				      STR_ERR_UNKNOWN_LONG_OPT, program_name, option);
	return -1;
      error_missing_arg_long: fprintf(stderr,
				      STR_ERR_MISSING_ARG_LONG, program_name, option);
	return -1;
      error_unexpec_arg_long: fprintf(stderr,
				      STR_ERR_UNEXPEC_ARG_LONG, program_name, option);
	return -1;
      }
    } else
      do {
	switch (*option) {
	case 'i':
	  if (option[1] != '\0')
	    instance_name = option + 1;
	  else if (++i < argc)
	    instance_name = argv[i];
	  else
	    goto error_missing_arg_short;
	  option = "\0";
	  opt_instance = 1;
	  break;
	case 's':
	  if (option[1] != '\0')
	    solution_name = option + 1;
	  else if (++i < argc)
	    solution_name = argv[i];
	  else
	    goto error_missing_arg_short;
	  option = "\0";
	  opt_solution = 1;
	  break;
	case 'p':
	  if (option[1] != '\0')
	    problem = atoi(option + 1);
	  else if (++i < argc)
	    problem = atoi(argv[i]);
	  else
	    goto error_missing_arg_short;
	  option = "\0";
	  opt_problem = 1;
	  break;
	default:
	  fprintf(stderr, STR_ERR_UNKNOWN_SHORT_OPT, program_name,
		  *option);
	  return -1;
	error_missing_arg_short: fprintf(stderr,
					 STR_ERR_MISSING_ARG_SHORT, program_name, *option);
	  return -1;
	}
      } while (*++option != '\0');
  }
  return i;
}

long int **allocateMatrix(long int rows, long int cols) {
  long int i;
  long int **matrix;

  matrix = (long int **) malloc((rows + 1) * sizeof(long int *));
  if (matrix == NULL ) {
    fprintf(stderr, "Problems in memory allocation\n");
  }
  for (i = 0; i < (rows + 1); i++) {
    matrix[i] = (long int *) malloc((cols + 1) * sizeof(long int));
    if (matrix[i] == NULL ) {
      fprintf(stderr, "Problems in memory allocation\n");
    }
    memset(matrix[i], 0, (cols + 1) * sizeof(long int));
  }
  return matrix;
}

long int *allocateLongVector(long int size) {
  long int *vector;

  vector = (long int *) malloc((size + 1) * sizeof(long int));
  if (vector == NULL ) {
    fprintf(stderr, "Problems in memory allocation\n");
  }
  memset(vector, 0, (size + 1) * sizeof(long int));
  return vector;
}

int *allocateIntVector(int size) {
  int *vector;

  vector = (int *) malloc((size + 1) * sizeof(int));
  if (vector == NULL ) {
    fprintf(stderr, "Problems in memory allocation\n");
  }
  memset(vector, 0, (size + 1) * sizeof(int));
  return vector;
}

int int_compare(const void * a, const void * b) {
  return (*(int*) a - *(int*) b);
}

/*************************************************************
 **************** checker part ********************************
 ************************************************************/
void checkConstraints_bin() {
  long int v = 0, w = 0, col = 0;
  int assigned;
  int conflict = 0;
  long int *conflicting_nodes = NULL;

  int i, j;
  int dist, size;

  read_graph_DIMACS_bin(instance_name);
  //write_graph_DIMACS_ascii("tmp.col");
  conflicting_nodes = allocateLongVector(NUM_NODES);
  solution_sizes = allocateIntVector(NUM_NODES);
  solution_sets = (int **) malloc((NUM_NODES + 1) * sizeof(int *));
  assert(solution_sets != NULL);
  for (i = 0; i <= NUM_NODES; i++) {
    solution_sets[i] = (int *) malloc(sizeof(int));
  }

  readSolution();
  CONF_EDGES = 0;
  CONF_NODES = 0;
  NOT_IN_LIST = 0;


  for (v = 1; v <= NUM_NODES - 1; v++) {
    for (w = v + 1; w <= NUM_NODES; w++)
      if (get_edge(w-1, v-1)) {
	//printf("e %d %d\n",w+1,v+1 );
	conflict = 0;
	for (i = 0; i < solution_sizes[v]; i++) {
	  for (j = 0; j < solution_sizes[w]; j++) {
	    if (solution_sets[v][i] == solution_sets[w][j]) {
	      conflict++;
	      if (VERBOSE)
		printf("%ld %ld\n", v, w);
	    }
	  }
	}
	if (conflict > 0) {
	  for (i = 0; i < CONF_NODES; i++) {
	    if (conflicting_nodes[i] == v)
	      break;
	  }
	  if (i == CONF_NODES) {
	    conflicting_nodes[i] = v;
	    CONF_NODES++;
	  }
	  for (i = 0; i < CONF_NODES; i++) {
	    if (conflicting_nodes[i] == w)
	      break;
	  }
	  if (i == CONF_NODES) {
	    conflicting_nodes[i] = w;
	    CONF_NODES++;
	  }
	  CONF_EDGES++;
	}
      }
  }
  free(conflicting_nodes);
}

void checkConstraints_ascii() {
  FILE *col_file;
  char buf[LINE_BUF_LEN];
  char *p, *tmp;
  long int from = 0, to = 0, col = 0;
  int assigned;
  int conflict = 0;
  long int *conflicting_nodes = NULL;

  int i, j;
  int dist, size;


  col_file = fopen(instance_name, "r");
  assert(col_file != NULL);

  while (!feof(col_file)) {

    buf[0] = '\0';
    fgets((char*) &buf, LINE_BUF_LEN, col_file);
    if ((p = strstr((char*) (&buf), "c ")) != NULL ) {
      /* If a c is found in a line, it's part of header */
      printf("%s", buf);
    } else if ((p = strstr((char*) (&buf), "p ")) != NULL ) {
      /* This is the line with the problem dimension */
      printf("%s", buf);
      /*	    printf("reading parameter line ...\n");*/
      if (strstr((char*) &buf, "edge") != NULL
	  || (strstr((char*) &buf, "col") != NULL)
	  || (strstr((char*) &buf, "edges") != NULL)
	  || strstr((char*) &buf, "band") != NULL) {
	p++;
	while (*p != '\0' && *p != '\n') {
	  while (*p == ' ')
	    p++;
	  while (*p != ' ')
	    p++;
	  while (*p == ' ')
	    p++;
	  NUM_NODES = atol(p);
	  
	  while (*p != ' ') p++;
	  while (*p == ' ')
	    p++;
	  NUM_EDGES = atol(p);
	  conflicting_nodes = allocateLongVector(NUM_NODES);

	  solution_sizes = allocateIntVector(NUM_NODES);
	  solution_sets = (int **) malloc(
					  (NUM_NODES + 1) * sizeof(int *));
	  assert(solution_sets != NULL);
	  for (i = 0; i <= NUM_NODES; i++) {
	    solution_sets[i] = (int *) malloc(sizeof(int));
	  }

	  readSolution();
	  CONF_EDGES = 0;
	  CONF_NODES = 0;
	  NOT_IN_LIST = 0;
	  while (*p != ' ')
	    p++;
	  while (*p == ' ')
	    p++;
	  *p = '\0';
	}
      }
    } else if (buf[0] == 'e') /*((p=strstr((char*)(&buf), "e"))!=NULL) */
      { /* This is a line with an edge */

	tmp = strtok(buf, " ");

	tmp = strtok(0, " ");

	from = atoi(tmp);
	tmp = strtok(0, " ");

	to = atoi(tmp);

	tmp = strtok(0, " ");

	if (tmp != NULL ) {
	  dist = atoi(tmp);
	} else
	  dist = 1;
	
	assert((1 <= from) && (from <= NUM_NODES));
	assert((1 <= to) && (to <= NUM_NODES));
	conflict = 0;
	if (from == to) {
	  if (problem == 5)
	    for (i = 0; i < solution_sizes[from] - 1; i++)
	      for (j = i + 1; j < solution_sizes[to]; j++)
		if (abs(
			solution_sets[from][i]
			- solution_sets[to][j]) < dist) {
		  conflict++;
		  if (VERBOSE)
		    printf("%ld %d %d %d %d %d\n", from, i, j,
			   solution_sets[from][i],
			   solution_sets[from][j], dist);
		}
	} else {

	  for (i = 0; i < solution_sizes[from]; i++)
	    for (j = 0; j < solution_sizes[to]; j++)
	      if (problem == 4 || problem == 5) {
		if (abs(
			solution_sets[from][i]
			- solution_sets[to][j]) < dist) {
		  conflict++;
		  if (VERBOSE)
		    printf("%d %d (%ld,%d) (%ld,%d) %d\n", i, j,
			   from, solution_sets[from][i], to,
			   solution_sets[to][j], dist);
		}
	      } else {
		if (solution_sets[from][i]
		    == solution_sets[to][j]) {
		  conflict++;
		  if (VERBOSE)
		    printf("%ld %ld\n", from, to);
		}
	      }
	}
	if (conflict > 0) {
	  for (i = 0; i < CONF_NODES; i++) {
	    if (conflicting_nodes[i] == from)
	      break;
	  }
	  if (i == CONF_NODES) {
	    conflicting_nodes[i] = from;
	    CONF_NODES++;
	  }
	  for (i = 0; i < CONF_NODES; i++) {
	    if (conflicting_nodes[i] == to)
	      break;
	  }
	  if (i == CONF_NODES) {
	    conflicting_nodes[i] = to;
	    CONF_NODES++;
	  }
	  CONF_EDGES++;
	}
      } else if (buf[0] == 'n'
		 && (problem == 3 || problem == 5 || problem == 6)) {
      tmp = strtok(buf, " ");
      tmp = strtok(0, " ");
      from = atoi(tmp);
      tmp = strtok(0, " ");
      size = atoi(tmp);
      if (solution_sizes[from] != size) {
	SET_SIZE++;
      }
      if (problem == 6) {
	qsort(solution_sets[from], solution_sizes[from], sizeof(int),
	      int_compare);
	for (i = 0; i < solution_sizes[from]; i++)
	  printf("%d ", solution_sets[from][i]);
	printf("\n");
	for (i = 0; i < solution_sizes[from] - 1; i++)
	  if (solution_sets[from][i + 1] - solution_sets[from][i]
	      != 1) {
	    conflict++;
	    if (VERBOSE)
	      printf(
		     "non consecutive integers to vertex %ld: (%d %d) of ",
		     from, solution_sets[from][i + 1],
		     solution_sets[from][i]);
	    for (i = 0; i < solution_sizes[from]; i++)
	      printf("%d ", solution_sets[from][i]);
	    printf("\n");
	  }

      }
    } else if (buf[0] == 'f' && problem == 2) {
      tmp = strtok(buf, " ");
      tmp = strtok(0, " ");

      from = atoi(tmp);
      tmp = strtok(0, " ");
      assigned = 0;

      while (tmp != NULL ) {
	col = atoi(tmp);

	for (i = 0; i < solution_sizes[from]; i++)
	  if (solution_sets[from][i] == col) {
	    assigned = 1;
	  }
	tmp = strtok(0, " ");
      }
      if (assigned != 1) {
	NOT_IN_LIST++;
	if (VERBOSE)
	  printf("<---%ld %ld\n", from, col);
      }
    }

  }
  fclose(col_file);
  free(conflicting_nodes);
}

char checkConstraints() {
  int i;

  NUM_EDGES = 0;
  CONF_EDGES = 0;
  CONF_NODES = 0;
  NOT_IN_LIST = 0;
  SET_SIZE = 0;

  printf("\nreading coloring-problem-file %s ... \n\n", instance_name);

  int format = 0;

  if (strstr(instance_name, ".b\0") != NULL )
    format = BIN_FORMAT;
  else
    format = ASCII_FORMAT;

  if (format == BIN_FORMAT) {
    printf("...read input in bin format...\n");
    checkConstraints_bin();
  } else if (format == ASCII_FORMAT) {
    printf("...read input in ASCII format...");
    checkConstraints_ascii();
  }

  free(solution_sizes);
  for (i = 0; i < NUM_NODES + 1; i++)
    free(solution_sets[i]);
  free(solution_sets);

  DENSITY = ((float) (NUM_EDGES)) / ((NUM_NODES / 2) * (NUM_NODES - 1));

  if (COLORED_NODES==NUM_NODES && CONF_EDGES == 0 && NOT_IN_LIST == 0 && SET_SIZE == 0)
    return TRUE;
  else
    return FALSE;
}

void readSolution() {
  FILE *sol_file;
  long int node, count;
  long int col;
  char *p;
  long int *colors = allocateLongVector(NUM_NODES * 20);
  long int i;
  char buf[LINE_BUF_LEN];
  sol_file = fopen(solution_name, "r");
  assert(sol_file != NULL);

  node = 0;
  COLORED_NODES=0;
  NUM_COL = 0;
  SUM_COL = 0;
  MIN_COL = INT_MAX;
  MAX_COL = 0;
  char new_line;
  
  
  while (fgets((char*) &buf, LINE_BUF_LEN, sol_file)!=NULL) {
	 //!feof(sol_file)) {
    count = 0;
    new_line=TRUE;
    p = strtok(buf, " ");

    do {

      if (*p != '\n') {
	if (new_line) {
	  COLORED_NODES++;
	  node++;
	  new_line=FALSE;
	}
	col = atoi(p);
	solution_sets[node] = realloc(solution_sets[node],
				      (count + 1) * sizeof(int));
	solution_sets[node][count++] = col;
	for (i = 0; i < NUM_COL; i++) {
	  if (colors[i] == col)
	    break;
	}
	if (i == NUM_COL) {
	  colors[i] = col;
	  NUM_COL++;
	}
	if (col < MIN_COL)
	  MIN_COL = col;
	if (col > MAX_COL)
	  MAX_COL = col;
	SUM_COL += col;
      }
      p = strtok(0, " ");
    } while (p != NULL );
    solution_sizes[node] = count;
    if (count < 1)
      SET_SIZE++;

    
    //printf("%d %d %d %d\n",NUM_NODES,node,COLORED_NODES, col);
    //    fgets((char*) &buf, LINE_BUF_LEN, sol_file);
    //printf("%s",buf);
  }

  fclose(sol_file);
}

