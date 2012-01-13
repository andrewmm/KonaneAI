/*  KonaneAI.h
    Interface for functions used by Konane AI

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the interface for the functions needed to operate the AI.
    Implementation can be found in KonaneAI.cpp */


#include "KonaneUtility.h"


#ifndef __KONANE_AI_H_INCLUDED__
#define __KONANE_AI_H_INCLUDED__

double static_score_simple (KonaneBoard *, int );
double static_score_new (KonaneBoard *, int );

double recurse_score (KonaneBoard *, int, time_t, double(*static_score)(KonaneBoard *, int));

void best_move (KonaneBoard *, MOVE_VECTOR *, int, double(*static_score)(KonaneBoard *, int));

/*  Temporary testing function. */

void recurse (KonaneBoard *, int, int, int *, int *, int *);

#endif
