/*  KonaneBoard.cpp
    Implementation of game logic for Konane

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

/*  This file contains the KonaneBoard class implementation. The interface
    can be found in KonaneBoard.h */

#include "KonaneBoard.h"

#include <cmath>
#include <string>
#include <iostream>

using namespace std;

/*  Various KonaneBoard class constructors, defined as functions rather than constructors
    to allow for initialization after declaration (in if/else constructs). */


void KonaneBoard::KonaneBoard_clear (int board_size)
{
    board.clear();
    
    boardsize = board_size;
    
    int x;
    for (x = 0; x < board_size; ++x)
    {
        vector<int> column;
        board.push_back(column);
        int y;
        for (y = 0; y < board_size; ++y)
        {
            board[x].push_back(0);    // Black (1) goes on even parity spots
        }
    }
}

void KonaneBoard::KonaneBoard_init0 (int board_size, KonaneAI *AI_1, KonaneAI *AI_2, string p1name, string p2name)
{
    AI[0] = true;
    AI[1] = true;
    AIPointers[0] = AI_1;
    AIPointers[1] = AI_2;
    names.clear();
    names.push_back(p1name);
    names.push_back(p2name);
    boardsize = board_size;

    turn = 1;
    
    board.clear();

    int x;
    for (x = 0; x < board_size; ++x)
    {
        vector<int> column;
        board.push_back(column);
        int y;
        for (y = 0; y < board_size; ++y)
        {
            board[x].push_back((x+y) % 2 + 1);    // Black (1) goes on even parity spots
        }
    }
}

void KonaneBoard::KonaneBoard_init1 (int board_size, int which_AI, KonaneAI *AIPnt, string p1name, string p2name)
{
    if (which_AI == 1 or which_AI == 2)
    {
        AI[which_AI-1] = true;
        AI[2-which_AI] = false;
        AIPointers[which_AI-1] = AIPnt;
        AIPointers[2-which_AI] = NULL;
    }
    else
    {
        AI[0] = true;
        AI[1] = false;
        AIPointers[0] = AIPnt;
        AIPointers[1] = NULL;
    }
    names.clear();
    names.push_back(p1name);
    names.push_back(p2name);
    boardsize = board_size;

    turn = 1;

    board.clear();
    
    int x;
    for (x = 0; x < board_size; ++x)
    {
        vector<int> column;
        board.push_back(column);
        int y;
        for (y = 0; y < board_size; ++y)
        {
            board[x].push_back((x+y) % 2 + 1);    // Black (1) goes on even parity spots
        }
    }
}

void KonaneBoard::KonaneBoard_init2 (int board_size, string p1name, string p2name)
{
    AI[0] = false;
    AI[1] = false;
    AIPointers[0] = NULL;
    AIPointers[1] = NULL;
    names.clear();
    names.push_back(p1name);
    names.push_back(p2name);
    boardsize = board_size;

    turn = 1;

    board.clear();
    
    int x;
    for (x = 0; x < board_size; ++x)
    {
        vector<int> column;
        board.push_back(column);
        int y;
        for (y = 0; y <= board_size; ++y)
        {
            board[x].push_back((x+y) % 2 + 1);    // Black (1) goes on even parity spots
        }
    }
}

void KonaneBoard::KonaneBoard_init3 ()
{
    AI[0] = false;
    AI[1] = false;
    AIPointers[0] = NULL;
    AIPointers[1] = NULL;
    names.clear();
    names.push_back("Player 1");
    names.push_back("Player 2");
    
    boardsize = 6;

    turn = 1;

    board.clear();
    
    int x;
    for (x = 0; x < boardsize; ++x)
    {
        vector<int> column;
        board.push_back(column);
        int y;
        for (y = 0; y < boardsize; ++y)
        {
            board[x].push_back((x+y) % 2 + 1);    // Black (1) goes on even parity spots
        }
    }
}

/*  remove
    
    Removes the piece that is at (x,y) */
void KonaneBoard::remove (int x,int y)
{
    bool legal;
    if (check_turn() == 1)
    {
        legal = ((x == 0 || x == boardsize-1) && (y == x || y == boardsize - 1 - x)) ||
                ((x == boardsize / 2 - 1 || x == boardsize / 2) && (y == boardsize / 2 - 1 || y == boardsize / 2));
    }
    else
    {
        legal = check_val(x,y+1) == 0 || check_val(x,y-1) == 0 || check_val(x-1,y) == 0 || check_val(x+1,y) == 0;
    }
    if (check_val(x,y) == check_turn() && legal)
    {
        board[x][y] = 0;
        turn = 3-turn;
    }
}

/*  jump

    If legal, jumps the piece at (x1,y1) to (x2,y2), removing intermediate pieces. */
void KonaneBoard::jump (int x1, int y1, int x2, int y2)
{
    if (board[x1][y1] == turn)
    {
        // Try to do something about the massive code duplication here
        if (x1==x2 and (y1-y2)%2==0 and y1 != y2)
        {
            bool valid_move = true;
            int dist = fabs(y1-y2);
            int dir = (y2-y1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2; ++i)
            {
                int y_temp = y1+2*i*dir;
                if (not ((check_val(x1, y_temp-dir) == 3-turn) and (check_val(x1, y_temp) == 0)))
                {
                    valid_move = false;
                }
            }
            if (valid_move)
            {
                for (i = 1; i <= dist/2; ++i)
                {
                    int y_temp = y1+2*i*dir;
                    board[x1][y_temp-2*dir] = 0;
                    board[x1][y_temp-dir] = 0;
                    board[x1][y_temp] = turn;
                }
                turn = 3-turn;
            }
        }
        else if (y1==y2 and (x1-x2)%2==0 and x1 != x2)
        {
            bool valid_move = true;
            int dist = fabs(x1-x2);
            int dir = (x2-x1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2; ++i)
            {
                int x_temp = x1+2*i*dir;
                if (not ((check_val(x_temp-dir,y1) == 3-turn) and (check_val(x_temp, y1) == 0)))
                {
                    valid_move = false;
                }
            }
            if (valid_move)
            {
                for (i = 1; i <= dist/2; ++i)
                {
                    int x_temp = x1+2*i*dir;
                    board[x_temp-2*dir][y1] = 0;
                    board[x_temp-dir][y1] = 0;
                    board[x_temp][y1] = turn;
                }
                turn = 3-turn;
            }
            
        }
    }      
}

/*  unjump

    Inverse function to jump, above.
    FIX: try to do something about code duplication with jump. */
void KonaneBoard::unjump (int x1, int y1, int x2, int y2)
{
    if (check_val(x1,y1) == 0)
    {
        // Try to do something about the massive code duplication here
        if (x1==x2 and (y1-y2)%2==0 and y1 != y2)
        {
            bool valid_move = true;
            int dist = fabs(y1-y2);
            int dir = (y2-y1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2 - 1; ++i)
            {
                int y_temp = y1+2*i*dir;
                if (not ((check_val(x1, y_temp-dir) == 0) and (check_val(x1, y_temp) == 0)))
                {
                    valid_move = false;
                }
            }

            if (not ((check_val(x1,y1+dist*dir-dir) == 0) and (check_val(x1,y1+dist*dir) == 3-turn)))
            {
                valid_move = false;
            }

            if (valid_move)
            {
                for (i = dist/2; i >= 1; --i)
                {
                    int y_temp = y1+2*i*dir;
                    board[x1][y_temp-2*dir] = 3-turn;
                    board[x1][y_temp-dir] = turn;
                    board[x1][y_temp] = 0;
                }
                turn = 3-turn;
            }
        }
        else if (y1==y2 and (x1-x2)%2==0 and x1 != x2)
        {
            bool valid_move = true;
            int dist = fabs(x1-x2);
            int dir = (x2-x1) / dist;       // make sure jumps in right direction

            int i;
            for (i = 1; i <= dist/2 - 1; ++i)
            {
                int x_temp = x1+2*i*dir;
                if (not ((check_val(x_temp-dir,y1) == 0) and (check_val(x_temp, y1) == 0)))
                {
                    valid_move = false;
                }
            }

            if (not ((check_val(x1+dist*dir-dir,y1) == 0) and (check_val(x1+dist*dir,y1) == 3-turn)))
            {
                valid_move = false;
            }

            if (valid_move)
            {
                for (i = dist/2; i >= 1; --i)
                {
                    int x_temp = x1+2*i*dir;
                    board[x_temp-2*dir][y1] = 3-turn;
                    board[x_temp-dir][y1] = turn;
                    board[x_temp][y1] = 0;
                }
                turn = 3-turn;
            }
            
        }
    }      
}

/*  flip_turn
    Takes no actions, but flips the turn.
    MAKE SURE TO ALWAYS UNDO. */
void KonaneBoard::flip_turn ()
{
    turn = 3 - turn;
}

/*  check_turn
    Access function for turn */
int KonaneBoard::check_turn ()
{
    return turn;
}

/*  size
    Access function for boardsize */
int KonaneBoard::size ()
{
    return boardsize;
}

/*  check_AI
    Returns true if the current player is an AI, false otherwise. */
bool KonaneBoard::check_AI ()
{
    return (AI[turn-1]);
}

/*  check_val

    Safe access to the board array. Returns -1 if passed an invalid position. */
int KonaneBoard::check_val (int x, int y)
{
    if (0 <= x and x < boardsize and 0 <= y and y < boardsize)    // Safe access
    {
        return board[x][y];
    }
    else
    {
        return -1;
    }
}



/*  get_move_AI
    Declared in KonaneBoard.h */
void KonaneBoard::get_move_AI (MOVE_VECTOR *movevec)
{
    (AIPointers[check_turn() - 1])->best_move(this, movevec);
}

void KonaneBoard::get_remove_AI (MOVE_VECTOR *removevec)
{
    (AIPointers[check_turn() - 1])->best_remove(this, removevec);
}


string KonaneBoard::get_name (int player)
{
    return names[player-1];
}

void KonaneBoard::display_board ()
{
    int x,y;
    cout << "    0 1 2 3 4 5" << endl << "    -----------" << endl;
    for (y=0; y < boardsize; ++y)
    {
        cout << y << " | ";
        for (x=0; x < boardsize; ++x)
        {
            int disp = check_val(x,y);
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

void KonaneBoard::generate_moves (MOVES_ARRAY *movebox)
{
    movebox->clear();
    
    int x;
    for (x = 0; x < boardsize; ++x)
    {
        int y;
        for (y = 0; y < boardsize; ++y)
        {
            if(check_val(x, y) == check_turn())
            {
                generate_moves_piece (movebox, x, y);
            }
        }
    }
}

void KonaneBoard::generate_moves_piece (MOVES_ARRAY *movebox, int x, int y)
{
    int dir;
    for (dir=1; dir != -3; dir=dir-2)   // loop will run for dir = 1, -1
    {
        int x_temp = x+2*dir;
        int y_temp = y+2*dir;
        
        int i = 1;
        while (check_val(x,y_temp) == 0 and check_val(x,y_temp-dir) == 3-check_turn())
        {
            int themove_ar[] = {x,y,x,y_temp};
            MOVE_VECTOR themove(themove_ar,themove_ar+4);
            movebox->push_back(themove);
            ++i;
            y_temp = y+2*i*dir;
        }
        
        i = 1;
        while (check_val(x_temp,y) == 0 and check_val(x_temp-dir,y) == 3-check_turn())
        {
            int themove_ar[] = {x,y,x_temp,y};
            MOVE_VECTOR themove(themove_ar,themove_ar+4);
            movebox->push_back(themove);
            ++i;
            x_temp = x+2*i*dir;
        }
    }
}

//  Generate an array of possible removes (each is a pair of integers in a vector)
//  And put it in the provided box. There are two possibilities: either this is the
//  first player to remove, or the second player to remove.
void KonaneBoard::generate_removes(vector< vector<int> > *removeBox)
{
    if (check_turn() == 1)
    {
        // Corner spots
        int c1[2] = {0,0};
        vector<int> vc1(c1,c1+2);
        int c2[2] = {boardsize-1,boardsize-1};
        vector<int> vc2(c2,c2+2);
        
        // Middle spots
        int m1[2] = {boardsize/2 - 1,boardsize/2 - 1};
        vector<int> vm1(m1,m1+2);
        int m2[2] = {boardsize/2, boardsize/2};
        vector<int> vm2(m2,m2+2);
        
        removeBox->push_back(vc1);
        removeBox->push_back(vc2);
        removeBox->push_back(vm1);
        removeBox->push_back(vm2);
    }
    else
    {
        // Find the piece that was removed
        int x = -1;
        int y = -1;
        int i;
        for (i = 0; i < boardsize; ++i)
        {
            int j;
            for (j = 0; j < boardsize; ++j)
            {
                if (check_val(i,j) == 0)
                {
                    x = i;
                    y = j;
                }
            }
        }
        if (x != -1 && y != -1)
        {
            int up[2] = {x,y+1};
            vector<int> vup(up,up+2);
            int down[2] = {x,y-1};
            vector<int> vdown(down,down+2);
            int left[2] = {x-1,y};
            vector<int> vleft(left,left+2);
            int right[2] = {x+1,y};
            vector<int> vright(right,right+2);
            
            removeBox->push_back(vup);
            removeBox->push_back(vdown);
            removeBox->push_back(vleft);
            removeBox->push_back(vright);
        }
    }
}

void KonaneBoard::move ()
{
    MOVE_VECTOR movevec;
    int curturn = check_turn();
    
    if(check_AI())
    {
        get_move_AI(&movevec);
    }
    else
    {
        get_move(&movevec);
    }
    
    move_vector_to_jump(&movevec);
    
    if (check_turn() == curturn and check_AI())
    {
        move ();
    }
    else if (check_turn() == curturn)
    {
        cout << "Illegal move." << endl;
        move ();
    }
}

void KonaneBoard::get_move (MOVE_VECTOR *movevec)
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


/*  game_over
 Parameters:     board, a pointer to a KonaneBoard object.
 Return:         true if there are no more available moves on the board,
 false otherwise. */
bool KonaneBoard::game_over ()
{
    MOVES_ARRAY movebox;
    generate_moves(&movebox);
    return (movebox.empty());
}


/*  move_vector_to_jump
 Declared in KonaneBoard.h */
void KonaneBoard::move_vector_to_jump (MOVE_VECTOR *movevec)
{
    if (movevec->size() == 4)
    {
        jump((*movevec)[0],(*movevec)[1],(*movevec)[2],(*movevec)[3]);
    }
    else
    {
        cout << "Weird error: movevec the wrong size in move_vector_to_jump." << endl;
    }
}


/*  move_vector_to_unjump
 Declared in KonaneBoard.h */
void KonaneBoard::move_vector_to_unjump (MOVE_VECTOR *movevec)
{
    if (movevec->size() == 4)
    {
        unjump((*movevec)[0],(*movevec)[1],(*movevec)[2],(*movevec)[3]);
    }
    else
    {
        cout << "Weird error: movevec the wrong size in move_vector_to_unjump." << endl;
    }
}


void KonaneBoard::move_vector_to_remove(MOVE_VECTOR *movevec)
{
    if (movevec->size() == 2)
    {
        remove((*movevec)[0],(*movevec)[1]);
    }
    else
    {
        cout << "Weird error: movevec the wrong size in move_vector_to_remove." << endl;
    }
}