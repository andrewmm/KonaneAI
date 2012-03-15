/*  KonaneAI.h
    Implementation for functions used by Konane AI

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the implementation for the functions needed to operate the AI.
    It shares an interface file with KonaneBoard.cpp */

#include "KonaneBoard.h"

#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

//  When not QUIET, AI will announce determined wins or loses to stdout
#define QUIET 1

//
//  Implementation for specific static score evaluator functions
//

//  static_score_simple treats any non-end game position as the same
double static_score_simple (KonaneBoard *board, int player, int depth)
{
    return 1;
}

//  static_score_new treats positions with more moves as better,
//  as well as favoring deeper lookaheads among otherwise identical trees.
double static_score_new (KonaneBoard *board, int player, int depth)
{
    MOVES_ARRAY movebox1,movebox2;

    board->generate_moves (&movebox1);
    board->flip_turn();
    board->generate_moves (&movebox2);
    board->flip_turn();
    
    double size1 = movebox1.size();
    double size2 = movebox2.size();

    double depthd = depth;
    
    if (board->check_turn() == player)
    {
        return 1 + (size1 - size2) / 10 - 1/(10 * (depthd + 1));
    }
    else
    {
        return 1 + (size2 - size1) / 10 - 1/(10 * (depthd + 1));
    }
}

//
//  Implementation for specific end game evaluator functions
//

//  end_game_simple merely returns 0 for loss and 2 for win.
double end_game_simple (KonaneBoard *board, int player, int depth)
{
    int thisturn = board->check_turn();
    return 2 * fabs(thisturn - player);
}

//  end_game_depthadj adjusts slightly by favoring quicker wins and slower loses
double end_game_depthadj (KonaneBoard *board, int player, int depth)
{
    int thisturn = board->check_turn();
    double depthd = depth;

    if (thisturn == player)
    {
        return 0 - 1/(10 * (depthd + 1));
    }
    else
    {
        return 2 + 1/(10 * (depthd + 1));
    }
}

//
//  Implementation for various AI initialization functions
//
//  In the absence of speficications, it defaults to static_score_new and end_game_simple
//  with a 10 second time frame.
//

void KonaneAI::KonaneAI_init0 (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef, string n)
{
    static_score_eval = static_score;
    end_game_eval = end_game;
    timeframe = timef;
    AIName = n;
}

void KonaneAI::KonaneAI_init1 (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef)
{
    static_score_eval = static_score;
    end_game_eval = end_game;
    timeframe = timef;
    AIName = "Computer Bob";
}

void KonaneAI::KonaneAI_init2 ()
{
    static_score_eval = &static_score_new;
    end_game_eval = &end_game_simple;
    timeframe = 10;
    AIName = "Computer Bob";
}


//
//  Implementation for KonaneAI class functions
//


//  Develops a score for the board state passed to it based on a recursive examination of
//  possible future states. It will stop by the absolute time stop_time, and divides its
//  remaining time equally among the various branches.
//
//  Returns:    The score for the current board position
double KonaneAI::recurse_score (KonaneBoard *board, int player, time_t stop_time, int depth)
{
    MOVES_ARRAY movebox;
    board->generate_moves (&movebox);
    time_t current_time = time(NULL);
    int thisturn = board->check_turn();

    // Have we gotten to an end state?
    if (movebox.size() == 0)
    {
        // Error handling if AI isn't initialized properly
        if (end_game_eval != &end_game_simple and end_game_eval != &end_game_depthadj)
        {
            cout << "ERROR: Forced to fix end_game" << endl;
            end_game_eval = &end_game_simple;
        }
        
        double temp_score = (*end_game_eval)(board, player, depth);
        
        // Fix scores if the end_game_eval function exceeds the bounds
        if (temp_score > 2.9)       return 2.9;
        else if (temp_score < -0.9) return -0.9;
        else                        return temp_score;
    }

    // Have we spent as much time as we're allowed to?
    if (current_time >= stop_time) 
    {
        // Error handling in case AI isn't initialized properly
        if (static_score_eval != &static_score_new and static_score_eval != &static_score_simple)
        {
            cout << "ERROR: Forced to change static_score" << endl;
            static_score_eval = &static_score_new;
        }
        
        double temp_score = (*static_score_eval)(board, player, depth);
        
        // Fix scores if the static_score_eval function exceeds the bounds
        if (temp_score > 1.99)      return 1.99;
        else if (temp_score < 0.01) return 0.01;
        else                        return temp_score;
    }

    // We want to initialize scoreacc way out of range (in correct dir) to make comparison work
    // That is, the first move needs to automatically be preferable to the initial scoreacc
    double scoreacc;
    if (thisturn == player)
    {
        scoreacc = -2;
    }
    else
    {
        scoreacc = 4;
    }

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        // Perform the hypothetical move
        board->move_vector_to_jump(&(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();
        double movescore = recurse_score (board, player, new_stop_time, depth+1);

        // Minimax those scores
        if (thisturn == player)
        {
            scoreacc = max(scoreacc, movescore);
        }
        else
        {
            scoreacc = min(scoreacc, movescore);
        }

        // Undo the hypothetical move
        board->move_vector_to_unjump(&(movebox[i]));
    }

    return scoreacc;
}


//  Given a board state, fill the movevec box with the best possible move for the current player
//  Should take less than or equal to the time allowed by timeframe.
//
//  Effects:    Fills movevec with the best possible move (supposedly!)
void KonaneAI::best_move (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    MOVES_ARRAY movebox;
    board->generate_moves (&movebox);
    movevec->clear();

    time_t current_time = time(NULL);
    
    // Error handling in case AI isn't initialized properly
    if (timeframe > 10 or timeframe < 1)
    {
        cout << "ERROR: forced to fix timeframe" << endl;
        timeframe = 10;
    }
    time_t stop_time = current_time + timeframe;
    
    // We'll collect all moves that have the max score, then randomly decide between them
    vector<int> optimalmoves;
    double maxscore = -10;

    int i;
    for (i = 0; i < movebox.size(); ++i)
    {
        // Don't risk damaging the current board!
        KonaneBoard tempboard(*board);
        tempboard.move_vector_to_jump(&(movebox[i]));

        time_t new_stop_time = current_time + (i+1) * (stop_time - current_time) / movebox.size();
        double movescore = recurse_score (&tempboard, board->check_turn(), new_stop_time, 1);

        // Is this move better than all the old ones?
        if (movescore > maxscore)
        {
            optimalmoves.clear();
            optimalmoves.push_back(i);
            maxscore = movescore;
        } // Or just as good?
        else if (movescore == maxscore)
        {
            optimalmoves.push_back(i);
        }
    }

    // Taunts and sadness
    if (QUIET == 0)
    {
        if (maxscore <= 0)
        {
            cout << "I'm gonna lose. :( " << maxscore << endl;
        }
        if (maxscore >= 2)
        {
            cout << "I'm gonna win! :) " << maxscore << endl;
        }
    }

    // If we have multiple equivalently good options, pick randomly.
    // This prevents simplistic AI's from favoring one part of the board
    // Over the others.
    srand (time (NULL));
    int selectedmove = rand() % optimalmoves.size();

    *movevec = movebox[optimalmoves[selectedmove]];
}


//  Given a board, at the start of the game, we have the AI remove a random piece that it's
//  allowed to remove.

void KonaneAI::best_remove(KonaneBoard *board, vector<int> *removevec)
{
    removevec->clear();
    vector< vector<int> > removevecBox;
    board->generate_removes (&removevecBox);
    srand(time(NULL));
    int selectedRemove = rand() % removevecBox.size();
    *removevec = removevecBox[selectedRemove];
}
