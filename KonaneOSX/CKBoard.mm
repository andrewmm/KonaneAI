//
//  CKBoard.m
//  KonaneOSX
//
//  Created by Andrew Reid MacKie-Mason on 1/18/12.
//  CMSC 16200, University of Chicago
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI
//
//  This code borrows heavily from http://macdevcenter.com/pub/a/mac/2006/12/19/building-a-game-engine-with-cocoa.html

#import "CKBoard.h"
#include "KonaneBoard.h"
#include "AppController.h"

#include <string>
#include <iostream>




@implementation CKBoard


+ (void)initialize
{
    // Expose bindings
    [self exposeBinding:@"backgroundColor"];
    [self exposeBinding:@"cellColor"];
    [self exposeBinding:@"highlightColor"];
    
    [[NSUserDefaultsController sharedUserDefaultsController] setInitialValues:[NSDictionary dictionaryWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Defaults" ofType:@"plist"]]];
}


@synthesize backgroundColor = _backgroundColor;
@synthesize cellColor = _cellColor;
@synthesize highlightColor = _highlightColor;

// Custom setters
- (void)setBackgroundColor:(NSColor *)backgroundColor
{
    [self willChangeValueForKey:@"backgroundColor"];
    // Based on automatic garbage collection
    _backgroundColor = backgroundColor;
    [self didChangeValueForKey:@"backgroundColor"];
    [self setNeedsDisplay:YES];
    [_backgroundColor retain];
}

- (void)setCellColor:(NSColor *)cellColor
{
    [self willChangeValueForKey:@"cellColor"];
    // Based on automatic garbage collection
    _cellColor = cellColor;
    [self didChangeValueForKey:@"cellColor"];
    [self setNeedsDisplay:YES];
    [_cellColor retain];
}

- (void)setHighlightColor:(NSColor *)highlightColor
{
    [self willChangeValueForKey:@"cellColor"];
    // Based on automatic garbage collection
    _highlightColor = highlightColor;
    [self didChangeValueForKey:@"cellColor"];
    [self setNeedsDisplay:YES];
    [_highlightColor retain];
}


//
//
//  Window management functions
//
//

- (id)initWithFrame:(NSRect)frame
{    
    self = [super initWithFrame:frame];
    if (self) {
//        [gameOptionsWindow makeKeyAndOrderFront:self];  
        if([gameOptionsWindow canBecomeKeyWindow])
        {
            NSLog(@"Yes");
        }
        
        // Initialize the KonaneBoard
        board = new KonaneBoard;
        ai[0] = new KonaneAI;
        ai[1] = new KonaneAI;
        ai[0]->KonaneAI_init2();
        ai[1]->KonaneAI_init2();
        movevec = new vector<int>;
        threading = NO;
        numRemoved = 0;
        
        [self setBackgroundColor:[NSColor whiteColor]];
        [self setCellColor:[NSColor brownColor]];
        [self setHighlightColor:[NSColor yellowColor]];
        
        [statusField setStringValue:@""];
        [self emptyBoard];
        
       
        
        // set up player colors
        playerColors[0] = [NSColor blackColor];
        playerColors[1] = [NSColor whiteColor];
        
        piecesPaths.clear();
        
        // set up the piece paths as nil pointers
        int i,j;
        for (i=0; i < board->size(); ++i)
        {
            vector<NSBezierPath *> column;
            piecesPaths.push_back(column);
            for (j = 0; j < board->size(); ++j)
            {
                piecesPaths[i].push_back(nil);
            }
        }
        
        // nothing is hovered or selected yet
        hoveredCoord = NIL_POINT;
        selectedCoord = NIL_POINT;
        lastSourceCoord = NIL_POINT;
        lastDestCoord = NIL_POINT;
        
        // current dimension of a square on the board
        squareDimension = frame.size.width/board->size();
    }
    return self;
}

// Clean up memory
- (void)dealloc
{
    // Things created with new
    delete board;
    delete ai[0];
    delete ai[1];
    delete movevec;
    
    int i; int j;
    for (i=0; i< board->size(); i++)
    {
        for (j=0; j< board->size(); j++)
        {
            if (piecesPaths[i][j] != nil)
            {
                [piecesPaths[i][j] release];
            }
        }
    }
    
    [super dealloc];
}

// Maintain square aspect ratio
- (void)awakeFromNib
{
    [self bind:@"backgroundColor" toObject:userDefaults withKeyPath:@"values.backgroundColor"
        options:[NSDictionary dictionaryWithObject:NSUnarchiveFromDataTransformerName forKey:NSValueTransformerNameBindingOption]];
    [self bind:@"cellColor" toObject:userDefaults withKeyPath:@"values.cellColor"
       options:[NSDictionary dictionaryWithObject:NSUnarchiveFromDataTransformerName forKey:NSValueTransformerNameBindingOption]];
    [self bind:@"highlightColor" toObject:userDefaults withKeyPath:@"values.highlightColor"
       options:[NSDictionary dictionaryWithObject:NSUnarchiveFromDataTransformerName forKey:NSValueTransformerNameBindingOption]];
    
    
    // The window should always be a square
    [[self window] setContentAspectRatio:NSMakeSize(1.0, 1.0)];
    
    // We do not want to accept mouse events
    [[self window] setAcceptsMouseMovedEvents:NO];
}


// for receiving mouse events
- (BOOL) acceptsFirstResponder
{
    return YES;
}

//  When the window is resized, we also want to reset
//  the size of the little squares.
- (void) setFrame: (NSRect) frame
{
    [super setFrame: frame];
    squareDimension = frame.size.width/board->size();
}



//
//
//  Mouse event functions
//
//

// Handle it when the mouse moves
- (void)mouseMoved:(NSEvent *)event
{
    // Get mouse location
    NSPoint mouse;
    mouse = [self convertPoint: [event locationInWindow] fromView: nil];
    
    // FIXXXX: Fill in the rest of this when we're doing mouse-over highlighting.
}


// Handle mouse-clicking!
- (void)mouseDown:(NSEvent *)event
{
    if (gameStarted && !board->check_AI())
    {
        if (numRemoved >= 2)
        {
            // Get mouse location
            NSPoint mouse;
            mouse = [self convertPoint: [event locationInWindow] fromView: nil];
        
            // Save previous click position
            NSPoint previousPoint = selectedCoord;
        
            // Save new click position
            selectedCoord = [self boardCoordForMousePoint:mouse];
        
            if (previousPoint.x != NIL_POINT.x && previousPoint.y != NIL_POINT.y)
            {
                [self jumpFromCoord:previousPoint toCoord:selectedCoord];
                selectedCoord = NIL_POINT;
                [self checkStatus];
            }
            [self setNeedsDisplay:YES];
        }
        else
        {
            // Get mouse location
            NSPoint mouse;
            mouse = [self convertPoint: [event locationInWindow] fromView: nil];
            NSPoint removeCoord = [self boardCoordForMousePoint:mouse];
            
            [self removeFromCoord:removeCoord];
            [self checkStatus];
            [self setNeedsDisplay:YES];
        }
    }
}


//
//
//  Drawing functions
//
//

// Main drawing function
- (void)drawRect:(NSRect)dirtyRect
{
    // Draw the board
    [self drawBoardBackgroundInRect:dirtyRect];
    
    int i,j;
    for (i=0; i < board->size(); ++i)
    {
        for (j=0; j < board->size(); ++j)
        {
            int piece = board->check_val(i, j);
            
            if ( piece == 1 || piece == 2)
            {
                if (piecesPaths[i][j] != nil)
                {
                    [piecesPaths[i][j] release];
                }
                piecesPaths[i][j] = [[self drawPieceForCoord:NSMakePoint(i, j) andPlayer:piece] retain];
            }
        }
    }
}

// Index through displaying each of the spots on the board, highlighting
// if the spot is the selected one
- (void)drawBoardBackgroundInRect:(NSRect)rect
{
    [[self backgroundColor] set];
    [NSBezierPath fillRect:rect];
    int i,j;
    for (i=0; i < board->size(); ++i)
    {
        for (j=0; j < board->size(); ++j)
        {
            NSPoint coord = NSMakePoint(i, j);
            BOOL highlight = (coord.x == selectedCoord.x && coord.y == selectedCoord.y) ||
                             (coord.x == lastSourceCoord.x && coord.y == lastSourceCoord.y) ||
                             (coord.x == lastDestCoord.x && coord.y == lastDestCoord.y);
            [self drawRectForBoardCoord:coord andHighlight:highlight];
        }
    }
}

// If we don't give it a highlighting option, default to NO
- (void) drawRectForBoardCoord:(NSPoint)p
{
    [self drawRectForBoardCoord:p andHighlight:NO];
}

// given a board coordinate such as (1,1), draw that board square and optionally highlight it
- (void)drawRectForBoardCoord:(NSPoint)p andHighlight:(BOOL)h
{
    // FIXXXX: Make this better later
    [[self cellColor] set];
    [[NSBezierPath bezierPathWithRect:[self rectForBoardCoord:p]] fill];
    if (h)
    {
        [[[self highlightColor] colorWithAlphaComponent:0.3f] set];
        [[NSBezierPath bezierPathWithRect:[self rectForBoardCoord:p]] fill];
    }
    //[[NSBezierPath bezierPathWithOvalInRect:[self rectForBoardCoord:p]] fill];
}

// Draw a piece in a given location. Return the NSBezierPath for its boundary.
- (NSBezierPath*)drawPieceForCoord:(NSPoint)p andPlayer:(int)player
{
    // Get the containing square
    NSRect rect = [self rectForBoardCoord:p];
    
    // Shrink it a bit to give us some margin
    float margin = rect.size.width*0.2;
    rect.origin.x += margin;
    rect.origin.y += margin;
    rect.size.width -= 2*margin;
    rect.size.height -= 2*margin;
    
    NSBezierPath *pieceBoundary;
    pieceBoundary = [NSBezierPath bezierPathWithOvalInRect:rect];
    
    [playerColors[player-1] set];
    [pieceBoundary fill];
    // FIXXXX add piece hilighting (only for correct player)
    
    return pieceBoundary;
}



//
//
//  Utility functions
//
//

// Get the rectangle to fill in for each place on the board. Will be FIXXXXed
- (NSRect)rectForBoardCoord:(NSPoint)p
{
    NSRect rect = [self bounds];
    
    NSRect r;
    // Make drawing rectangle smaller to leave a boarder
    r.origin = NSMakePoint(rect.origin.x+ (p.x+0.1)*squareDimension, rect.origin.y+ (p.y+0.1)*squareDimension);
    r.size = NSMakeSize(squareDimension * 0.8, squareDimension * 0.8);
    
    return r;
}

// Given a mouse coordinate, find the board coordinate
- (NSPoint)boardCoordForMousePoint:(NSPoint)p
{
    int x = -1;
    int y = -1;
    
    while (p.x > 0)
    {
        p.x -= squareDimension;
        ++x;
    }
    while (p.y > 0)
    {
        p.y -= squareDimension;
        ++y;
    }
    
    return NSMakePoint(x,y);
}


//
//
//  Gameplay management functions
//
//

- (void)newGame
{
    [self stopGame];
    [gameOptionsWindow makeKeyAndOrderFront:self];
}

// Just wrap this into new game?
- (void)stopGame
{
    //[self emptyBoard];
    gameStarted = NO;
    [self setNeedsDisplay:YES];
}

// Gameplay functions
- (void)initGame:(int)boardSize withAI:(int)numberOfAI aiPosition:(int)aiPos withName:(string)p1name andName:(string)p2name
{
    gameStarted = YES;
    [self fillBoard:boardSize withAI:numberOfAI aiPosition:aiPos withName:p1name andName:p2name];
    NSString *p1nameNS = [NSString stringWithCString:p1name.c_str() 
                                            encoding:[NSString defaultCStringEncoding]];
    NSString *p2nameNS = [NSString stringWithCString:p2name.c_str() 
                                            encoding:[NSString defaultCStringEncoding]];
    [self display];
    [player1Display setStringValue:p1nameNS];
    [player2Display setStringValue:p2nameNS];
    [[self window] setAcceptsMouseMovedEvents:YES];
    [self checkStatus];
}

// Set the board up as empty
- (void)emptyBoard
{
    board->KonaneBoard_clear(6);
    gameStarted = NO; // Don't accept mouse input
    
    // current dimension of a square on the board
    squareDimension = [self bounds].size.width/board->size();
    
    [self setNeedsDisplay:YES];
}


// Fill up the board
- (void)fillBoard:(int)boardSize withAI:(int)numberOfAI aiPosition:(int)aiPos withName:(string)p1name andName:(string)p2name
{
    if (numberOfAI == 1)
    {
        board->KonaneBoard_init1(boardSize, aiPos, ai[aiPos-1], p1name, p2name);
    }
    else if (numberOfAI == 2)
    {
        board->KonaneBoard_init0(boardSize, ai[0], ai[1], p1name, p2name);
    }
    else
    {
        board->KonaneBoard_init2(boardSize, p1name, p2name);
    }
    
    numRemoved = 0;
    
    piecesPaths.clear();
    
    // set up the piece paths as nil pointers
    int i,j;
    for (i=0; i < board->size(); ++i)
    {
        vector<NSBezierPath *> column;
        piecesPaths.push_back(column);
        for (j = 0; j < board->size(); ++j)
        {
            piecesPaths[i].push_back(nil);
        }
    }
    
    selectedCoord = NIL_POINT;
    lastSourceCoord = NIL_POINT;
    lastDestCoord = NIL_POINT;
    
    squareDimension = [self bounds].size.width/board->size();
    
    [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn to remove a piece from the corners or center.",(board->get_name(board->check_turn())).c_str()]];
}

// Check if AI should move or if the game is over
- (void)checkStatus
{
    if (!threading)
    {
        if(board->game_over() && numRemoved >= 2)
        {
            [statusField setStringValue:[NSString stringWithFormat:@"Game over! %s wins!",(board->get_name(3-board->check_turn())).c_str()]];
            [controller newGame:self];
        }
        
        if(board->check_AI() && (!board->game_over() || numRemoved < 2))
        {
            if (numRemoved >= 2)
            {
                [statusField setStringValue:[NSString stringWithFormat:@"%s is thinking...",(board->get_name(board->check_turn())).c_str()]];
                [statusField display];
                movevec->clear();
            
                // Disable things that shouldn't work while the AI is thinking
                threading = YES;
                [newGameMenu setEnabled:NO];
            
                [NSThread detachNewThreadSelector:@selector(getMoveAI) toTarget:self withObject:nil];
            }
            else
            {
                [statusField setStringValue:[NSString stringWithFormat:@"%s is thinking about what to remove...",(board->get_name(board->check_turn())).c_str()]];
                [statusField display];
                movevec->clear();
                
                // Disable things that shouldn't work while the AI is thinking
                threading = YES;
                [newGameMenu setEnabled:NO];
                
                [NSThread detachNewThreadSelector:@selector(getRemoveAI) toTarget:self withObject:nil];
            }
        }
    }
}

// Run the board's jump procedure
- (void)jumpFromCoord:(NSPoint)p1 toCoord:(NSPoint)p2
{
    int currentTurn = board->check_turn();
    board->jump(p1.x, p1.y, p2.x, p2.y);
    if (currentTurn == board->check_turn())
    {
        [statusField setStringValue:@"Illegal move."];
    }
    else
    {
        [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn.",(board->get_name(board->check_turn())).c_str()]];
        lastSourceCoord = p1;
        lastDestCoord = p2;
    }
    [self display];
}

- (void)removeFromCoord:(NSPoint)p
{
    int currentTurn = board->check_turn();
    board->remove(p.x,p.y);
    if (currentTurn == board->check_turn())
    {
        [statusField setStringValue:@"Illegal remove."];
    }
    else
    {
        ++numRemoved;
        if (numRemoved >= 2)
        {
            [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn.",(board->get_name(board->check_turn())).c_str()]];
        }
        else
        {
            [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn to remove a piece adjacent to this one.",(board->get_name(board->check_turn())).c_str()]];
        }
    }
    [self display];
}

- (void)performMove
{
    board->move_vector_to_jump(movevec);
    [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn.",(board->get_name(board->check_turn())).c_str()]];
    lastSourceCoord.x = (*movevec)[0];
    lastSourceCoord.y = (*movevec)[1];
    lastDestCoord.x = (*movevec)[2];
    lastDestCoord.y = (*movevec)[3];
    [self display];
    [self checkStatus];
}

- (void)performRemove
{
    int currentTurn = board->check_turn();
    board->move_vector_to_remove(movevec);
    if (board->check_turn() == currentTurn)
    {
        NSLog(@"Error, illegal remove");
    }
    else
    {
        ++numRemoved;
    }
    if (numRemoved >= 2)
    {
        [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn.",(board->get_name(board->check_turn())).c_str()]];
    }
    else
    {
        [statusField setStringValue:[NSString stringWithFormat:@"It is %s's turn to remove a piece adjacent to this one.",(board->get_name(board->check_turn())).c_str()]];
    }
    [self display];
    [self checkStatus];
}

// These should be threaded out
- (void)getMoveAI
{
    board->get_move_AI(movevec);
    [self performSelectorOnMainThread:@selector(performMove) withObject:nil waitUntilDone:NO];
    
    // Enable everything again
    threading = NO;
    [newGameMenu setEnabled:YES];
}

- (void)getRemoveAI
{
    board->get_remove_AI(movevec);
    [self performSelectorOnMainThread:@selector(performRemove) withObject:nil waitUntilDone:NO];
    
    // Enable everything again
    threading = NO;
    [newGameMenu setEnabled:YES];
}
                             


@end
