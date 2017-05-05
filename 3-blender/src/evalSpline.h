#ifndef _SPLINE_EVAL_H
#define _SPLINE_EVAL_H

#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>

#define NumTotEvaluations 500 ///< numero di valutazioni delle funzioni base su tutta la partizione nodale
static const float evalStep = 1.0 / (NumTotEvaluations-1); ///< 

#define MaxNumPts 32 ///< max Control Vertices
#define MaxOrder 5 ///< cubic splines
#define MaxK MaxNumPts-2 ///< max internal nodes == maxCV - minimum order
#define MaxNodes (MaxK + 2) + 2 * ( MaxOrder - 1 )

float N[MaxNumPts][MaxOrder+1][NumTotEvaluations];

float roundf( float a );

typedef struct NodalPartition NodalPartition;
struct NodalPartition
{
	float pos[MaxNodes]; ///< position of nodes
	int numNodes; ///< number of total nodes in the nodal partition
	int m; ///< spline order
	int k; ///< num internal nodes
	int a, b; ///< indice dei nodi a,b cioe' della partizione non estesa
	int A, B; ///< indice dei nodi a e b rispetto al parametro 

};

void computeBSplines( struct NodalPartition* nodes, float N[MaxNumPts][MaxOrder+1][NumTotEvaluations] );

int findIntervalFromPosition( NodalPartition* nodes, float pos );

void snapPartitionNodes( NodalPartition* nodes );

int loadSpline( char* fName, int* numCV, float CV[][2], NodalPartition* nodes );
int saveSpline( char* fName, int  numCV, float CV[][2], NodalPartition* nodes );

void refreshNodes(NodalPartition* nodes, int numCV, int partitionType);

void printNodes(NodalPartition* nodes);

#endif
