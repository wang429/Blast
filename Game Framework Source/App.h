//
//  App.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2010 Menke. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/*
 App controls interface with the system */

@interface App : NSApplication 
{
	@public NSWindow* window;
}	

- (void)sendEvent:(NSEvent *)event;

- (void)run;

@end

/*
 Window controls input interfacing and is its own delegate
 */
@interface Window : NSWindow <NSWindowDelegate>
{
}
//window functions
- (void)keyDown:(NSEvent *)event;

- (void)keyUp:(NSEvent *)event;

- (void)mouseDown:(NSEvent *)event;

- (void)mouseUp:(NSEvent *)event;

- (void)rightMouseDown:(NSEvent *)event;

- (void)rightMouseUp:(NSEvent *)event;

- (void)otherMouseDown:(NSEvent *)event;

- (void)otherMouseUp:(NSEvent *)event;

//delegate functions
- (void)windowDidBecomeKey:(NSNotification *)notification;

- (void)windowDidResignKey:(NSNotification *)notification;

- (void)windowWillClose:(NSNotification *)notification;

@end
