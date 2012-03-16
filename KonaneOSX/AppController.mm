//
//  AppController.m
//  KonaneOSX
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI
//
//  Created by Andrew Reid MacKie-Mason on 1/18/12.
//  University of Chicago, CMSC 16200
//

#import "AppController.h"
#include "CKBoard.h"


@implementation AppController

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame]; 
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

- (IBAction)newGame:(id)sender
{
    [gameBoard newGame];
}

@end
