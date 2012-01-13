/*  KonaneAI.h
    Implementation for functions used by Konane AI

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the implementation for the functions needed to operate the AI.
    Interface can be found in KonaneAI.h */

#include "KonaneAI.h"
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>


double static_score_simple (KonaneBoard *board, int player)
{
    return 1;
}

double static_score_new (KonaneBoard *board, int player)
{
    MOVES_ARRAY movebox1,movebox2;

    generate_moves (board, &movebox1);
    board->flip_turn();
    generate_moves (board, &movebox2);
    board->flip_turn();

    double size1 = movebox1.size();
    double size2 = movebox2.size();

    if (board->check_turn() == player)
    {
        return (size1 - size2) / 10;
    }
    else
    {
        return (size2 - size1) / 10;
    }
}


double recurse_score (KonaneBoard *board, int player, time_t stop_time, double(*static_score)(KonaneBoard *, int))
{
    MOVES_ARRAY movebox;
    generate_moves (board, &movebox);

    time_t current_time = time(NULL);

    int thisturn = board->check_turn();

    if (movebox.size() == 0)
    {
        return 2 * fabs(thisturn - player);
    }

    if (current_time >= stop_time)
    {
        double temp_score = (*static_score)(board, player);
        if (temp_score > 1.9)
        {
            return 1.9;
        }
        else if (temp_score < 0.1)
        {
            return 0.1;
        }
        else
        {
            return temp_score;
        }
    }

    int i;

    int scoreacc;
    if (thisturn == player)
    {
        scoreacc = 0;
    }
    else
    {
        scoreacc = 2;
    }

    for (i = 0; i < movebox.size(); ++i)
    {
        move_vector_to_jump(board, &(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();

        int movescore = recurse_score (board, player, new_stop_time, static_score);

        if (thisturn == player)
        {
            scoreacc = max(scoreacc, movescore);
        }
        else
        {
            scoreacc = min(scoreacc, movescore);
        }

        move_vector_to_unjump(board, &(movebox[i]));
    }

    return scoreacc;
}

void best_move (KonaneBoard *board, MOVE_VECTOR *movevec, int time_frame, double(*static_score)(KonaneBoard *, int))
{
    MOVES_ARRAY movebox;
    generate_moves (board, &movebox);
    movevec->clear();

    time_t current_time = time(NULL);
    time_t stop_time = current_time + time_frame;

    vector<int> optimalmoves;
    int maxscore = 0;

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        KonaneBoard tempboard(*board);
        move_vector_to_jump(&tempboard, &(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();

        double movescore = recurse_score (&tempboard, board->check_turn(), new_stop_time, static_score);

        if (movescore > maxscore)
        {
            optimalmoves.clear();
            optimalmoves.push_back(i);
            maxscore = movescore;
        }
        else if (movescore == maxscore)
        {
            optimalmoves.push_back(i);
        }
    }

    srand (time (NULL));
    int selectedmove = rand() % optimalmoves.size();

    *movevec = movebox[optimalmoves[selectedmove]];
}

// Efficiency Tester, NOT REAL FUNCTION:

void recurse (KonaneBoard *board, int depth, int maxdepth, int *endgames, int *continues, int *mostmoves)
{
    MOVES_ARRAY movebox;
    generate_moves (board, &movebox);

    if (movebox.size() > *mostmoves)
    {
        *mostmoves = movebox.size();
    }

    if (movebox.size() == 0)
    {
        ++(*endgames);
    }
    else
    {
        ++(*continues);
    }

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        move_vector_to_jump(board, &(movebox[i]));
        if (depth + 1 <= maxdepth)
        {
            recurse (board, depth+1, maxdepth, endgames, continues, mostmoves);
        }
        move_vector_to_unjump(board, &(movebox[i]));
    }
}
