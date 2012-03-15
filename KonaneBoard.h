/*  KonaneBoard.h
    Interface for game logic for Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the KonaneBoard and Konane AI class interfaces.
    Implementation for KonaneBoard is found in KonaneBoard.cpp.
    Implementation for KonaneAI is found in KonaneAI.cpp */


#include <string>
#include <vector>

#ifndef __KONANE_BOARD_H_INCLUDED__
#define __KONANE_BOARD_H_INCLUDED__

#define MOVE_VECTOR vector<int>
#define MOVES_ARRAY vector< MOVE_VECTOR >

using namespace std;

class KonaneBoard; // Forward declaration

//  Some different functions that can be used for a specific AI's static_score_eval
double static_score_simple (KonaneBoard *board, int player, int depth);
double static_score_new (KonaneBoard *board, int player, int depth);

//  Some different functions that can be used for a specific AI's end_game_eval
double end_game_simple (KonaneBoard *board, int player, int depth);
double end_game_depthadj (KonaneBoard *board, int player, int depth);

class KonaneAI
{
    private:
        string AIName; // Could be useful later
        int timeframe; // How long the AI is allowed to make moves
    public:
        // Various initialization functions
        void KonaneAI_init0 (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timef, string n);
        void KonaneAI_init1 (double(*static_score)(KonaneBoard *board, int player, int depth),
                  double(*end_game)(KonaneBoard *board, int player, int depth),
                  int timeframe);
        void KonaneAI_init2 ();

        // Minimax the score for a specific move
        double recurse_score (KonaneBoard *board, int player, time_t stop_time, int depth);
    
        // Select the best move (or remove) possible, and put it in the movevec box.
        void best_move (KonaneBoard *board, MOVE_VECTOR *movevec);
        void best_remove (KonaneBoard *board, MOVE_VECTOR *movevec);
    
        // Function the AI uses to evaluate non-end states. Should return value between 0.01 and 1.99
        double (*static_score_eval)(KonaneBoard *board, int player, int depth);
        // Function the AI uses to evaluate end states. Should return <= 0 for a loss, >= 2 for a win.
        double (*end_game_eval)(KonaneBoard *board, int player, int depth);
};


class KonaneBoard
{
    private:
        int boardsize;
        vector< vector<int> > board;  // The board. 0 = empty, 1 = player1, 2 = player2
        int turn;
        bool AI[2];
        KonaneAI *AIPointers[2];
        vector<string> names;

    public:
        // Various initialization functions
        void KonaneBoard_init0 (int board_size, KonaneAI *AI_1, KonaneAI *AI_2, string p1name, string p2name);
        void KonaneBoard_init1 (int board_size, int which_AI, KonaneAI *AIPnt, string p1name, string p2name);
        void KonaneBoard_init2 (int board_size, string p1name, string p2name);
        void KonaneBoard_init3 ();
        void KonaneBoard_clear (int board_size);  // Empties the board without destroying it

        void remove (int x,int y);  // removes a piece from the board
        void jump (int x1,int y1,int x2,int y2);
        void unjump (int x1,int y1,int x2,int y2);
        void flip_turn ();  // Use with CAUTION in hypothetical explorations of the game tree.
                            // Should never be called on the actual game board.
    
        // Functions to generate a vector of possible moves (or removes) for the current player
        void generate_moves (MOVES_ARRAY *movebox);
        void generate_moves_piece (MOVES_ARRAY *movebox, int x, int y);
        
        void generate_removes (MOVES_ARRAY *removebox);
    
        // Converting move_vectors into a format that can be used by jump/unjump
        void move_vector_to_jump (MOVE_VECTOR *movevec);
        void move_vector_to_unjump (MOVE_VECTOR *movevec);
        void move_vector_to_remove (MOVE_VECTOR *movevec);

        // Handle the process of actually getting a move from the relevant player
        void move ();
        void get_move (MOVE_VECTOR *movevec);
        void get_move_AI (MOVE_VECTOR *movevec);
        void get_remove_AI (MOVE_VECTOR *removevec);

        int check_turn ();
        int size();
        bool check_AI (); // checks if current player is an AI
        int check_val (int x, int y); // safe access to the gameboard. Returns -1 if out of bounds
        string get_name (int player);
        bool game_over ();
        void display_board (); // Prints an ASCII representation of the board to stdout
};


#endif


