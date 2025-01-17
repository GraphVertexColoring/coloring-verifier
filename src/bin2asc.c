/* Dimacs graph format translator to and from a binary, more efficient
   format. Written by Tamas Badics (badics@rutcor.rutgers.edu), 
   using the technique of Marcus Peinado, Boston University. */
/*
   Corrected to number nodes from 1 to n (not 0 to n-1)
   MT
*/

#include "bin2asc.h"



char Bitmap[MAX_NR_VERTICES][MAX_NR_VERTICESdiv8];
char Preamble[MAX_PREAMBLE];


const char unsigned masks[ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };


//int get_params(void);
/* ====================================================== */

char get_edge(  int i,  int j )
{
	int byte, bit;
	char mask;
	
	bit  = 7-(j & 0x00000007);
	byte = j >> 3;
	
	mask = masks[bit];
	return( (Bitmap[i][byte] & mask)==mask );
}

/* ============================================= */
void write_graph_DIMACS_ascii(const char *file)
{
	int i,j;
	FILE *fp;

	if ( (fp=fopen(file,"w"))==NULL )
	  { printf("ERROR: Cannot open outfile\n"); exit(10); }

	fprintf(fp, "%s",Preamble);

	for ( i = 0; i<NUM_NODES; i++ )
	  {
		  for ( j=0; j<=i; j++ )
			if ( get_edge(i,j) ) fprintf(fp,"e %d %d\n",i+1,j+1 );
	  }

	fclose(fp);
}

void read_graph_DIMACS_bin(const char *file)
{

	int i, length = 0;
	FILE *fp;
	
	if ( (fp=fopen(file,"r"))==NULL )
	  { printf("ERROR: Cannot open infile\n"); exit(10); }

	if (!fscanf(fp, "%d\n", &length))
	  { printf("ERROR: Corrupted preamble.\n"); exit(10); }

	if(length >= MAX_PREAMBLE)
	  { printf("ERROR: Too long preamble.\n"); exit(10); }
		   
	fread(Preamble, 1, length, fp);
	Preamble[length] = '\0';
	
	if (!get_params())
		  { printf("ERROR: Corrupted preamble.\n"); exit(10); }

	for ( i = 0
		 ; i < NUM_NODES && fread(Bitmap[i], 1, (int)((i + 8)/8), fp)
		 ; i++ );

	fclose(fp);
}

int get_params(void)
                      /* getting Nr_vert and Nr_edge from the preamble string, 
						 containing Dimacs format "p ??? num num" */
{
	char c, *tmp;
	char * pp = Preamble;
	int stop = 0;
	tmp = (char *)calloc(100, sizeof(char));
	
	NUM_NODES = NUM_EDGES = 0;
	
	while (!stop && (c = *pp++) != '\0'){
		switch (c)
		  {
			case 'c':
			  while ((c = *pp++) != '\n' && c != '\0');
			  break;
			  
			case 'p':
			  sscanf(pp, "%s %ld %ld\n", tmp, &NUM_NODES, &NUM_EDGES);
			  stop = 1;
			  break;
			  
			default:
			  break;
		  }
	}
	
	free(tmp);
	
	if (NUM_NODES == 0 || NUM_EDGES == 0)
	  return 0;  /* error */
	else
	  return 1;
	
}
