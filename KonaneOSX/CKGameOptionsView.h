//
//  CKGameOptionsView.h
//  KonaneOSX
//
//  Created by Andrew MacKie-Mason on 1/20/12.
//  University of Chicago, CMSC 16200
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

#import <Cocoa/Cocoa.h>

#ifndef __CK_GAME_OPTIONS_VIEW_H__INCLUDED__
#define __CK_GAME_OPTIONS_VIEW_H__INCLUDED__

/*  This class handles the window that allows the user to enter options
    and then start a game. */

@class CKBoard;

@interface CKGameOptionsView : NSView
{
    // Other objects to communicate with
    IBOutlet NSPanel *containingWindow;
    IBOutlet CKBoard *board;
    IBOutlet NSUserDefaultsController *userDefaults;
    
    // Player 1 information
    IBOutlet NSTextField *p1name;
    IBOutlet NSMatrix *p1AIMatrix;
    BOOL p1ai;
    
    // Player 2 information
    IBOutlet NSTextField *p2name;
    IBOutlet NSMatrix *p2AIMatrix;
    BOOL p2ai;
    
    // Other info
    IBOutlet NSSlider *boardSizeSlider;
}

//  Handle the start button being pressed.
//  Effects:    Hides the window
//              Tells the board to start playing with the correct info
-(IBAction)startGame:(id)sender;

@end

#endif