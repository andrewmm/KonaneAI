/*  KonaneAI.h
    Implementation for functions used by Konane AI

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the implementation for the functions needed to operate the AI.
    Interface can be found in KonaneAI.h */

#include "KonaneBoard.h"
#include "KonaneUtility.h"

#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

// Implementation for specific static score evaluator functions


double static_score_simple (KonaneBoard *board, int player, int depth)
{
    return 1;
}

double static_score_new (KonaneBoard *board, int player, int depth)
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
        return 1 + (size1 - size2) / 10 - 1/(10 * (depth + 1));
    }
    else
    {
        return 1 + (size2 - size1) / 10 - 1/(10 * (depth + 1));
    }
}

//  Implementation for specific end game evaluator functions
double end_game_simple (KonaneBoard *board, int player, int depth)
{
    int thisturn = board->check_turn();
    return 2 * fabs(thisturn - player);
}

double end_game_depthadj (KonaneBoard *board, int player, int depth)
{
    int thisturn = board->check_turn();
    if (thisturn == player)
    {
        return (0 - 1/(10 * (depth + 1)));
    }
    else
    {
        return (2 + 1/(10 * (depth + 1)));
    }
}

//  Various KonaneAI class constructors

KonaneAI::KonaneAI (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef, string n)
{
    static_score_eval = static_score;
    end_game_eval = end_game;
    timeframe = timef;
    AIName = n;
}

KonaneAI::KonaneAI (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef)
{
    static_score_eval = static_score;
    end_game_eval = end_game;
    timeframe = timef;
    AIName = "Computer Bob";
}

KonaneAI::KonaneAI ()
{
    static_score_eval = &static_score_new;
    end_game_eval = &end_game_depthadj;
    timeframe = 10;
    AIName = "Computer Bob";
}


// Implementation for KonaneAI class functions


double KonaneAI::recurse_score (KonaneBoard *board, int player, time_t stop_time, int depth)
{
    MOVES_ARRAY movebox;
    generate_moves (board, &movebox);
    time_t current_time = time(NULL);
    int thisturn = board->check_turn();

    if (movebox.size() == 0)
    {
        return (*end_game_eval)(board, player, depth);
    }

    if (current_time >= stop_time)
    {
        double temp_score = (*static_score_eval)(board, player, depth);
        if (temp_score > 1.99)
        {
            return 1.99;
        }
        else if (temp_score < 0.01)
        {
            return 0.01;
        }
        else
        {
            return temp_score;
        }
    }

    int scoreacc;
    if (thisturn == player)
    {
        scoreacc = 0;
    }
    else
    {
        scoreacc = 2;
    }

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        move_vector_to_jump(board, &(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();

        int movescore = recurse_score (board, player, new_stop_time, depth+1);

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

void KonaneAI::best_move (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    MOVES_ARRAY movebox;
    generate_moves (board, &movebox);
    movevec->clear();

    time_t current_time = time(NULL);
    time_t stop_time = current_time + timeframe;

    vector<int> optimalmoves;
    int maxscore = 0;

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        KonaneBoard tempboard(*board);
        move_vector_to_jump(&tempboard, &(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();

        double movescore = recurse_score (&tempboard, board->check_turn(), new_stop_time, 1);

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

    if (maxscore == 0)
    {
        cout << "I'm gonna lose. :(" << endl;
    }

    srand (time (NULL));
    int selectedmove = rand() % optimalmoves.size();

    *movevec = movebox[optimalmoves[selectedmove]];
}
