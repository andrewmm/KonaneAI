//
//  AppController.h
//  KonaneOSX
//
//  Created by Andrew Reid MacKie-Mason on 1/18/12.
//  University of Chicago, CMSC 16200
//
//  Project URL: http://brick.cs.uchicago.edu/Courses/CMSC-16200/2012/pmwiki/pmwiki.php/Student/KonaneAI

#import <Cocoa/Cocoa.h>

#ifndef __APP_CONTROLLER_H__INCLUDED__
#define __APP_CONTROLLER_H__INCLUDED__


@interface AppController : NSView {
    IBOutlet id gameBoard;
}

- (IBAction) newGame: (id)sender;

@end

#endif