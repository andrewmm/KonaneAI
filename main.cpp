/*  main.cpp
    Main code for Konane game

    Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

    Andrew MacKie-Mason
    University of Chicago, CMSC 16200 */

#include "KonaneBoard.h"
#include "KonaneUtility.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>

#define QUIET 0

using namespace std;

int play_game(int p1ai, string p1name, int p2ai, string p2name)
{
    KonaneAI ai1;
    KonaneAI ai2;
    string input = "";

    if (p1ai == 1)
    {
        char p1complex;
        int p1timeframe;


        cout << "Should " << p1name << " play with a complex strategy? (y/n) ";
        getline (cin, input);
        p1complex = input[0];

        while (true)
        {
            cout << "What is " << p1name << "'s time frame? (4-10) ";
            getline (cin, input);
            if (stringstream(input) >> p1timeframe) break;
        }
        
        if (p1complex == 'y')
        {
            ai1.KonaneAI_init0(&static_score_new, &end_game_depthadj, p1timeframe, p1name);
        }
        else
        {
            ai1.KonaneAI_init0(&static_score_simple, &end_game_simple, p1timeframe, p1name);
        }
    }

    if (p2ai == 1)
    {
        char p2complex;
        int p2timeframe;

        cout << "Should " << p2name << " play with a complex strategy? (y/n) ";
        getline (cin, input);
        p2complex = input[0];

        while (true)
        {
            cout << "What is " << p2name << "'s time frame? (4-10) ";
            getline (cin, input);
            stringstream conversion(input);
            if (conversion >> p2timeframe) break;
        }

        if (p2complex == 'y')
        {
            ai2.KonaneAI_init0(&static_score_new, &end_game_depthadj, p2timeframe, p2name);
        }
        else
        {
            ai2.KonaneAI_init0(&static_score_simple, &end_game_simple, p2timeframe, p2name);
        }
    }

    KonaneBoard gameboard;

    if (p1ai == 1 and p2ai == 1)
    {
        gameboard.KonaneBoard_init0(&ai1, &ai2, p1name, p2name);
    }
    else if (p1ai == 1)
    {
        gameboard.KonaneBoard_init1(1, &ai1, p1name, p2name);
    }
    else if (p2ai == 1)
    {
        gameboard.KonaneBoard_init1(2, &ai2, p1name, p2name);
    }
    else
    {
        gameboard.KonaneBoard_init2(p1name, p2name);
    }

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
            cout << "It is " << gameboard.get_name(gameboard.check_turn()) << "'s turn." << endl;
        }
        move(&gameboard);
        if (QUIET == 0)
        {
            cout << "Board:" << endl;
            display_board(&gameboard);
        }
    }

    cout << "Game over. " << gameboard.get_name(3-gameboard.check_turn()) << " wins!" << endl;

    return 3-gameboard.check_turn();
}

int main()
{
    char p1hum, p2hum;
    int p1ai, p2ai;
    string p1name, p2name;
    string input;

    cout << "Welcome to Konane." << endl << "Is player 1 a human? (y/n) ";
    getline (cin, input);
    p1hum = input[0];
    
    cout << "What is player 1's name? ";
    getline (cin, p1name);
    if (p1hum == 'n')
    {
        p1ai = 1;
    }
    else
    {
        p1ai = 0;
    }

    cout << "Is player 2 a human? (y/n) ";
    getline (cin, input);
    p2hum = input[0];

    cout << "What is player 2's name? ";
    getline (cin, p2name);
    if (p2hum == 'n')
    {
        p2ai = 1;
    }
    else
    {
        p2ai = 0;
    }

    play_game(p1ai, p1name, p2ai, p2name);
    return 0;
}
