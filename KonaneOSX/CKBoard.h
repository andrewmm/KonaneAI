//
//  CKBoard.h
//  KonaneOSX
//
//  Created by Andrew Reid MacKie-Mason on 1/18/12.
//  CMSC 16200, University of Chicago
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI
//
//  This code borrows heavily from http://macdevcenter.com/pub/a/mac/2006/12/19/building-a-game-engine-with-cocoa.html?

#ifndef __CKBOARD_H_INCLUDED__
#define __CKBOARD_H_INCLUDED__

#import <Cocoa/Cocoa.h>
#include "KonaneBoard.h"

#define NIL_POINT NSMakePoint(-1,-1)

@interface CKBoard : NSView {
    // Other things to communicate with
    IBOutlet id controller;
    IBOutlet id userDefaults;
    
    IBOutlet id player1Display;
    IBOutlet id player2Display;
    
    IBOutlet NSPanel *gameOptionsWindow;
    
    IBOutlet NSTextField *statusField;
    IBOutlet NSMenuItem *newGameMenu;
    
    // The KonaneBoard object will have the backend data, as will the AI
    KonaneBoard *board;
    KonaneAI *ai[2];
    
    vector<int> *movevec;
    
    // These can be used to check if a piece is hovered over.
    vector< vector<NSBezierPath *> > piecesPaths;
    
    // Is the game going?
    BOOL gameStarted;
    int numRemoved;
    
    // Are we threading out the AI's search?
    BOOL threading;
    
    // Keep track of some mouse locations
    NSPoint hoveredCoord;
    NSPoint selectedCoord;
    NSPoint lastSourceCoord;
    NSPoint lastDestCoord;
    
    // What piece are we holding?
    int holdingPiece;
    
    // Attributes of the displayed board.
    float squareDimension;
    NSColor *playerColors[2]; 
    
    NSColor *_backgroundColor;
    NSColor *_cellColor;
    NSColor *_highlightColor;
}
// attributes to be bound
@property (readonly, retain) NSColor *backgroundColor;
@property (readonly, retain) NSColor *cellColor;
@property (readonly, retain) NSColor *highlightColor;

// Superclass methods to override
- (id)initWithFrame:(NSRect)frame;
- (void)dealloc;
- (void)awakeFromNib;
- (BOOL)acceptsFirstResponder;
- (void)setFrame:(NSRect)frame;

- (void)mouseMoved:(NSEvent *)event;
- (void)mouseDown:(NSEvent *)event;

- (void)drawRect:(NSRect)dirtyRect;

// Drawing functions
- (void)drawBoardBackgroundInRect:(NSRect)rect;
- (void)drawRectForBoardCoord:(NSPoint)p;
- (void)drawRectForBoardCoord:(NSPoint)p andHighlight:(BOOL)h;
- (NSBezierPath*)drawPieceForCoord:(NSPoint)p andPlayer:(int)player;


// Other utility functions
- (NSRect)rectForBoardCoord:(NSPoint)p;
- (NSPoint)boardCoordForMousePoint:(NSPoint)p;

// Game management functions
- (void)newGame;
- (void)stopGame;
- (void)initGame:(int)numberOfAI aiPosition:(int)aiPos withName:(string)p1name andName:(string)p2name;
- (void)emptyBoard;
- (void)fillBoard:(int)boardSize withAI:(int)numberOfAI aiPosition:(int)aiPos withName:(string)p1name andName:(string)p2name;
- (void)checkStatus;
- (void)jumpFromCoord:(NSPoint)p1 toCoord:(NSPoint)p2;
- (void)removeFromCoord:(NSPoint)p;
- (void)performMove;
- (void)performRemove;
- (void)getMoveAI;
- (void)getRemoveAI;

@end


// Forward declarations from KonaneUtility.h

bool game_over (KonaneBoard *);
void move_vector_to_jump (KonaneBoard *, vector<int> *);

#endif