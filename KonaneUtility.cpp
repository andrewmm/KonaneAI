/*  KonaneUtility.cpp
    Interface for utility functions used in Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the implementation for various utility functions.
    Interface can be found in KonaneUtility.h */

#include <iostream>
#include "KonaneUtility.h"
#include "KonaneAI.h"



#define MOVE_VECTOR vector<int>
#define MOVES_ARRAY vector< MOVE_VECTOR >


using namespace std;

/*  display_board
    Declared in KonaneUtility.h */
void display_board (KonaneBoard *board)
{
    int x,y;
    cout << "    0 1 2 3 4 5" << endl << "    -----------" << endl;
    for (y=0; y <= 5; ++y)
    {
        cout << y << " | ";
        for (x=0; x <= 5; ++x)
        {
            int disp = board->check_val(x,y);
            if (disp == 0)
            {
                cout << "  ";
            }
            else
            {
                cout << disp << " ";
            }
        }
        cout << endl;
    }
}

/*  game_over
    Parameters:     board, a pointer to a KonaneBoard object.
    Return:         true if there are no more available moves on the board,
                    false otherwise. */
bool game_over (KonaneBoard *board)
{
    MOVES_ARRAY movebox;
    generate_moves(board, &movebox);
    return (movebox.empty());
}



/*  generate_moves
    Parameters:     board, a pointer to a KonaneBoard object.
                    movebox, a pointer to the container object to put the moves into
    Actions:        Fills the movebox with the possible moves (represented in
                    <x1,y1,x2,y2> form) that can be taken by the player who's turn
                    it is in the game represented by board. */
void generate_moves (KonaneBoard *board, MOVES_ARRAY *movebox)
{
    movebox->clear();

    int x;
    for (x = 0; x <= 5; ++x)
    {
        int y;
        for (y = 0; y <= 5; ++y)
        {
            if(board->check_val(x, y) == board->check_turn())
            {
                generate_moves_piece (board, movebox, x, y);
            }
        }
    }
}



/*  generate_moves_piece
    Parameters:     board, a pointer to a KonaneBoard object
                    movebox, a pointer to the container object to put the moves into
                    (x,y), the location to derive moves for
    Action:         Finds all the moves originating at (x,y) and adds them to the end of
                    movebox. */
void generate_moves_piece (KonaneBoard *board, MOVES_ARRAY *movebox, int x, int y)
{
    int dir;
    for (dir=1; dir != -3; dir=dir-2)   // loop will run for dir = 1, -1
    {
        int x_temp = x+2*dir;
        int y_temp = y+2*dir;

        int i = 1;
        while (board->check_val(x,y_temp) == 0 and board->check_val(x,y_temp-dir) == 3-board->check_turn())
        {
            int themove_ar[] = {x,y,x,y_temp};
            MOVE_VECTOR themove(themove_ar,themove_ar+4);
            movebox->push_back(themove);
            ++i;
            y_temp = y+2*i*dir;
        }

        i = 1;
        while (board->check_val(x_temp,y) == 0 and board->check_val(x_temp-dir,y) == 3-board->check_turn())
        {
            int themove_ar[] = {x,y,x_temp,y};
            MOVE_VECTOR themove(themove_ar,themove_ar+4);
            movebox->push_back(themove);
            ++i;
            x_temp = x+2*i*dir;
        }
    }
}



/*  move_vector_to_jump
    Declared in KonaneUtility.h */
void move_vector_to_jump (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    if (movevec->size() == 4)
    {
        board->jump((*movevec)[0],(*movevec)[1],(*movevec)[2],(*movevec)[3]);
    }
    else
    {
        cout << "Weird error: movevec the wrong size in move_vector_to_jump." << endl;
    }
}



/*  move_vector_to_unjump
    Declared in KonaneUtility.h */
void move_vector_to_unjump (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    if (movevec->size() == 4)
    {
        board->unjump((*movevec)[0],(*movevec)[1],(*movevec)[2],(*movevec)[3]);
    }
    else
    {
        cout << "Weird error: movevec the wrong size in move_vector_to_jump." << endl;
    }
}



/*  move
    Parameters:     board, a pointer to a KonaneBoard object.
    Actions:        Handles the process of requesting and running a move
                    given by the current player. Outsources getting a move
                    to get_move if the player is human and get_move_AI if
                    the player is AI. */
void move (KonaneBoard *board)
{
    MOVE_VECTOR movevec;
    int curturn = board->check_turn();

    if(board->check_AI())
    {
        get_move_AI(board, &movevec);
    }
    else
    {
        get_move(board, &movevec);
    }

    move_vector_to_jump(board, &movevec);

    if (board->check_turn() == curturn and board->check_AI())
    {
        move (board);
    }
    else if (board->check_turn() == curturn)
    {
        cout << "Illegal move." << endl;
        move (board);
    }
}



/*  get_move
    Parameters:     board, a pointer to a KonaneBoard object.
    Actions:        Requests a move from the (human) player. */
void get_move (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    int x1,y1,x2,y2;
    movevec->clear();

    cout << "x1 = ";
    cin >> x1;
    cout << "y1 = ";
    cin >> y1;
    cout << "x2 = ";
    cin >> x2;
    cout << "y2 = ";
    cin >> y2;

    movevec->push_back(x1); movevec->push_back(y1); movevec->push_back(x2); movevec->push_back(y2);
}



/*  get_move_AI
    Declared in KonaneUtility.h */
void get_move_AI (KonaneBoard *board, MOVE_VECTOR *movevec)
{
    if (board->check_turn() == 1)
    {
        best_move(board,movevec,10,&(static_score_new));
    }
    else
    {
        best_move(board,movevec,10,&(static_score_new));
    }
}
