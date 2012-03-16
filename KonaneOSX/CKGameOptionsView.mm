//
//  CKGameOptionsView.m
//  KonaneOSX
//
//  Created by Andrew MacKie-Mason on 1/20/12.
//  University of Chicago, CMSC 16200
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

#import "CKGameOptionsView.h"

#include "CKBoard.h"

#include <string>

@implementation CKGameOptionsView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {}
    return self;
}

- (void)awakeFromNib
{
    [containingWindow makeKeyAndOrderFront:self];
}

//  Handle Start Game button being pressed
//  Effects:    Hide the window
//              Gather the data and send it to the game board
-(IBAction)startGame:(id)sender
{
    p1ai = [[[p1AIMatrix selectedCell] title] isEqualToString:@"Computer"];
    p2ai = [[[p2AIMatrix selectedCell] title] isEqualToString:@"Computer"];
    
    // Hide the window
    [containingWindow orderOut:self];
    
    int numberOfAI = 0;
    if (p1ai) ++numberOfAI;
    if (p2ai) ++numberOfAI;

    int aiPos = 0;
    if (p1ai) aiPos = 1;
    if (p2ai) aiPos = 2; // if both are AI, this won't be used anyways
    
    // Convert names to C++ strings
    std::string p1nameString = [[p1name stringValue] cStringUsingEncoding:[NSString defaultCStringEncoding]];
    std::string p2nameString = [[p2name stringValue] cStringUsingEncoding:[NSString defaultCStringEncoding]];
    
    int boardSize = [boardSizeSlider intValue];
    
    [board initGame:boardSize withAI:numberOfAI aiPosition:aiPos withName:p1nameString andName:p2nameString];
}

@end
