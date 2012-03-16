//
//  KonaneOSXAppDelegate.h
//  KonaneOSX
//
//  Created by Andrew Reid MacKie-Mason on 1/18/12.
//  University of Chicago, CMSC 16200
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

#import <Cocoa/Cocoa.h>

@interface KonaneOSXAppDelegate : NSObject <NSApplicationDelegate> {
    IBOutlet NSWindow *window;
    IBOutlet NSPanel *newGamePanel; 
}

@property (assign) IBOutlet NSWindow *window;

@end
