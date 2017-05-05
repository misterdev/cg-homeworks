#include "evalSpline.h"

float roundf( float a )
{
	return ( a >= 0 ) ? floor( a + 0.5 ) : floor ( a - 0.5 ) ;
}

void computeBSplines( struct NodalPartition* nodes, float basis[MaxNumPts][MaxOrder+1][NumTotEvaluations] )
{
	int order = nodes->m;
	int k = nodes->k;

	int i;
	int m;
	float t;
	int param;
	float* pos = &(nodes->pos[0]);

	int a, b;
	float n1, n2, d1, d2;

	for( i=0 ; i< MaxNumPts ; i++ )
		for( m=0 ; m<=MaxOrder ; m++ )
			for( param=0 ; param<NumTotEvaluations ; param++ )
				N[i][m][param] = 0.0;

	for( m=1 ; m<=order ; m++ )
	{
		for( i=0 ; i < ((order+k) + (order-m)) ; i++ ) //triangular
		{
			//basis[i][m][t] e' non zero nell'intervallo [i;i+m]

			//nodi coincidenti ( la spline N[i][m][t] e' nulla )
			if( pos[i] == pos[i+m] )
				continue;

			a = roundf( (NumTotEvaluations-1) * pos[i] );
			b = roundf( (NumTotEvaluations-1) * pos[i+m] );
		
			d1 = (pos[i+m-1] - pos[i] );
			d2 = (pos[i+m] - pos[i+1]);

			for( param=a ; param<b ; param++ ) 
			{
				t = (float)param * evalStep;
				if( m == 1 )
				{
					basis[i][m][param] = 1.0;
				}
				else
				{
					if( d1 != 0 )
					{
						n1 = ( t - pos[i] );
						basis[i][m][param] += (n1/d1) * basis[i][m-1][param];
					}
					if( d2 != 0 )
					{
						n2 = (pos[i+m] - t);
						basis[i][m][param] += (n2/d2) * basis[i+1][m-1][param];
					}
				}
			}
		}
	}
}

int findIntervalFromPosition( NodalPartition* nodes, float pos )
{
	int i;
	int nn = nodes->numNodes-1;
	for( i=0 ; i<nn ; i++ )
		if( pos >= nodes->pos[i] && pos < nodes->pos[i+1] )
			return i;

	printf( "Interval not found... " );
	return -1;
}

/** this function moves all nodes of the partition at multiples of evalStep. This avoid numerical errors due to the discretization of the parameter space.
 *
 */ 
void snapPartitionNodes( NodalPartition* nodes )
{
	int i;
	float snappedPos;

	for( i=1 ; i< nodes->numNodes-1 ; i++ )
	{
		snappedPos = roundf( nodes->pos[i] / evalStep ) * evalStep;
		nodes->pos[i] = snappedPos;
	}

}

/** Loads a spline from a file named fName.  Store loaded spline into numCV, CV and nodes.
*/
int loadSpline( char* fName, int* numCV, float CV[][2], NodalPartition* nodes )
{
	FILE* file;
	int i; //,j;
	int res;
	char s[16];

	printf("Opening file %s\n", fName);
#ifdef WIN32
	if (( file = fopen( fName, "r")) == NULL)
#else
    if (( file = fopen( fName, "r")) == NULL)
#endif
	{
		printf("file non trovato\n");
		return 0;
	}

	res = fscanf( file,"%s %d",s, &nodes->m );
	res = fscanf( file,"%s %d",s, numCV );
	for( i=0 ; i<*numCV ; i++ )
		res = fscanf( file,"%f %f", &CV[i][0], &CV[i][1] );
	
	res = fscanf( file,"%s %d",s, &nodes->numNodes );
	for( i=0 ; i<nodes->numNodes ; i++ )
		res = fscanf( file,"%f", &nodes->pos[i] );

	fclose( file );

	nodes->k = *numCV - nodes->m; ///< num internal nodes
	nodes->a = nodes->m - 1;
	nodes->b = nodes->m + nodes->k;
	nodes->A = roundf( nodes->pos[nodes->a] / evalStep );
	nodes->B = roundf( nodes->pos[nodes->b] / evalStep );

	snapPartitionNodes( nodes );

	printNodes(nodes);

	printf("Spline loaded. Remeber to refresh BSpline basis and spline points.\n");
	return 1;
	
}

int saveSpline( char* fName, int numCV, float CV[][2], NodalPartition* nodes )
{
	FILE* file;
	int i; //,j;

	printf("Opening file %s\n", fName );
#ifdef WIN32
	if (( file = fopen( fName, "w")) == NULL)
#else
    if (( file = fopen( fName, "w")) == NULL)
#endif
	{
		printf("problems saving file\n");
		return 0;
	}

	fprintf( file,"Order %d\n",nodes->m );
	fprintf( file,"NumCV %d\n", numCV );
	for( i=0 ; i<numCV ; i++ )
		fprintf( file,"%f %f\n", CV[i][0], CV[i][1] );
	fprintf( file,"NumNodes %d\n", nodes->numNodes );
	for( i=0 ; i<nodes->numNodes ; i++ )
		fprintf( file,"%f\n", nodes->pos[i] );

	fclose( file );
	printf( "spline saved to %s\n", fName );
	return 1;
}

/** Recomputes the nodal partition based on the number of control vertices numCV */
void refreshNodes(NodalPartition* nodes, int numCV, int partitionType)
{
	int i;

	// If spline order has not been set manually, default to order 3;
	if( nodes->m == 0 )
		nodes->m = 3;

	nodes->k = numCV - nodes->m; ///< num internal nodes
	nodes->numNodes = (nodes->k + 2) + 2 * (nodes->m - 1);
	nodes->a = nodes->m - 1;
	nodes->b = nodes->m + nodes->k;

	if( partitionType == 2 )
	{
		for( i=0 ; i<nodes->m ; i++ )
			nodes->pos[i] = 0.0;
		for( i=0 ; i<nodes->m ; i++ )
			nodes->pos[(nodes->numNodes-1)-i] = 1.0;
		for( i=0 ; i<nodes->k ; i++ )
			nodes->pos[i+nodes->m] = (1.0 / (nodes->k+1)) * (float)(i+1);
	}
	else if( partitionType == 1 )
	{
		//nodi equispaziati in [0;1]
		for( i=0 ; i<nodes->numNodes ; i++ )
			nodes->pos[i] = (1.0 / (nodes->numNodes-1)) * (float)i;
	}

	nodes->A = roundf( nodes->pos[nodes->a] / evalStep );
	nodes->B = roundf( nodes->pos[nodes->b] / evalStep );

	snapPartitionNodes( nodes );

	printNodes(nodes);
}

void printNodes(NodalPartition* nodes)
{
	int i;
	printf( "\n\n=================================================\n\n" );
	printf( "  m -> order: %d\n", nodes->m );
	printf( "  k -> internal nodes: %d\n", nodes->k );
	printf( "  numNodes: %d\n", nodes->numNodes );
	printf( "  partition: " );
	for( i=0 ; i<nodes->numNodes ; i++ )
		printf( "%.2f ", nodes->pos[i] );
	printf( "\n" );
	printf( "  a = %d    b = %d\n", nodes->a, nodes->b );
	printf( "  A = %d    B = %d", nodes->A, nodes->B );
	printf( "\n=================================================\n\n" );
}

