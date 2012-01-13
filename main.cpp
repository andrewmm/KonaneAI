/*  main.cpp
    Main code for Konane game

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

#include "KonaneBoard.h"
#include "KonaneUtility.h"
#include "KonaneAI.h"
#include <iostream>

#define QUIET 0

using namespace std;

int play_game(int AI_1, int AI_2)
{
    KonaneBoard gameboard(AI_1, AI_2);
    gameboard.remove(2,2);
    gameboard.remove(3,2);

    if (QUIET == 0)
    {
        cout << "Board:" << endl;
        display_board(&gameboard);
    }

    time_t ltime;

    while (not game_over(&gameboard))
    {
        time(&ltime);
        if (QUIET == 0)
        {
            cout << asctime(localtime(&ltime));
            cout << "It is Player " << gameboard.check_turn() << "'s turn." << endl;
        }
        move(&gameboard);
        if (QUIET == 0)
        {
            cout << "Board:" << endl;
            display_board(&gameboard);
        }
    }

    cout << "Game over. Player " << 3-gameboard.check_turn() << " wins!" << endl;

    return 3-gameboard.check_turn();
}

int main()
{
    play_game(1,1);
/*    int wins[] = {0,0};
    int i;
    for (i = 0; i < 5; ++i)
    {
        wins[play_game(1,1) - 1]++;
    }
    cout << "Player 1 (simple) won " << wins[0] << " times." << endl;
    cout << "Player 2 (advanced) won " << wins[1] << " times." << endl; */
    return 0;
}

/*
int main()
{
    cout << "Application beginning.\n";
    KonaneBoard gameboard(0,1);
    cout << "Gameboard initialized.\n";

    int xr,yr;
    for (xr = 2; xr <= 3; ++xr)
    {
        for (yr = 2; yr ==2; ++yr)
        {
            gameboard.remove(xr,yr);
        }
    }

    cout << "Board:" << endl;
    int x,y;
    for (y=0; y <= 5; ++y)
    {
        for (x=0; x <= 5; ++x)
        {
            cout << gameboard.check_val(x,y) << " ";
        }
        cout << "\n";
    }

    cout << "It's player " << gameboard.check_turn() << "'s turn." << endl;

    MOVES_ARRAY somemoves;
    generate_moves(&gameboard, &somemoves);
    cout << "Number of moves: " << somemoves.size() << endl;

    int i;
    for (i=0; i < somemoves.size(); ++i)
    {
        int j;
        cout << "<";
        for (j = 0; j < (somemoves[i]).size(); ++j)
        {
            cout << somemoves[i][j] << ",";
        }
        cout << ">, ";
    }
    cout << endl;

    best_move(&gameboard, 6);

    cout << "Board:" << endl;
    for (y=0; y <= 5; ++y)
    {
        for (x=0; x <= 5; ++x)
        {
            cout << gameboard.check_val(x,y) << " ";
        }
        cout << "\n";
    }
}

*/


//    eval_moves(&gameboard, 6);

/*    int endgames = 0;
    int continues = 0;
    int mostmoves = 0;

    time_t ltime;
    time(&ltime);
    cout << endl << endl  << asctime(localtime(&ltime)) << "Begin recursion!" << endl << endl;
    recurse(&gameboard, 0, 8, &endgames, &continues, &mostmoves);
    time(&ltime);
    cout << asctime(localtime(&ltime)) << "End recursion!" << endl << "Endgames: " << endgames << endl << "Total: " << endgames + continues << endl << "Most moves: " << mostmoves << endl; */
