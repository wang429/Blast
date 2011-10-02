//
//  App.mm
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2010 Menke. All rights reserved.
//

#import "Rect.h"
#import "Engine.h"
#import "Game.h"
#import "App.h"
#import "Keyboard.h"

using namespace Framework;

@implementation App

//overriden for custom event handling
- (void)sendEvent:(NSEvent *)event
{
    int flags=(int)[event modifierFlags];
    switch ([event type])
    {
        case NSFlagsChanged:
            
            if(flags & NSAlternateKeyMask)
                gEngine.onKeyPress('\0',KEY_ALT);
            else
                gEngine.onKeyRelease('\0', KEY_ALT);
            
            if(flags & NSControlKeyMask)
                gEngine.onKeyPress('\0',KEY_CONTROL);
            else
                gEngine.onKeyRelease('\0', KEY_CONTROL);
            
            if(flags & NSShiftKeyMask)
                gEngine.onKeyPress('\0',KEY_SHIFT);
            else
                gEngine.onKeyRelease('\0', KEY_SHIFT);
            
            if(flags & NSCommandKeyMask)
                gEngine.onKeyPress('\0',KEY_CMD);
            else
                gEngine.onKeyRelease('\0', KEY_CMD);
            
            break;
        
        case NSKeyDown:
            if([event isARepeat])
                gEngine.onKeyRepeat([[event characters] characterAtIndex:0], [event keyCode]);
            
            gEngine.onKeyPress([[event characters] characterAtIndex:0], [event keyCode]);
            break;
            
        case NSKeyUp:
            gEngine.onKeyRelease([[event characters] characterAtIndex:0], [event keyCode]);
            break;
            
        case NSLeftMouseDown:
            gEngine.onMousePress([event buttonNumber]);
            break;
            
        case NSLeftMouseUp:
            gEngine.onMouseRelease([event buttonNumber]);
            break;
            
        case NSRightMouseDown:
            gEngine.onMousePress([event buttonNumber]);
            break;
            
        case NSRightMouseUp:
            gEngine.onMouseRelease([event buttonNumber]);
            break;
            
        case NSOtherMouseDown:
            gEngine.onMousePress([event buttonNumber]);
            break;
            
        case NSOtherMouseUp:
            gEngine.onMouseRelease([event buttonNumber]);
            break;
            
        default:
            break;
    }
    [super sendEvent: event];
}

- (void)run
{
    
	NSRunLoop *  runLoop = [[NSRunLoop currentRunLoop] retain];
	
	[self finishLaunching];
	
	_running = 1;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	
	
	[window makeKeyAndOrderFront:self];
	
	gEngine.getGame()->init();
	while([self isRunning]) 
	{
		//manage cocoa memory manager
		[pool drain];
		pool = [[NSAutoreleasePool alloc] init];
		
		//get first cocoa event
		NSEvent * event = [self   nextEventMatchingMask: NSAnyEventMask untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES];
		
		//handle all events
		while(event != nil)
		{
			[self sendEvent: event];
			event = [self   nextEventMatchingMask: NSAnyEventMask untilDate: nil inMode: NSDefaultRunLoopMode dequeue: YES];
		}
		
		//update engine /*could go in its own thread*/
		gEngine.run();
		
		[[[window contentView] openGLContext] flushBuffer];
		
	}
	gEngine.getGame()->shutdown();
	
	[runLoop release];
	[pool drain];
}

@end

@implementation Window

//let the system know we do handle these events
- (void)keyDown:(NSEvent *)event
{
    
}

- (void)keyUp:(NSEvent *)event
{
    
}

- (void)mouseDown:(NSEvent *)event
{
    
}

- (void)mouseUp:(NSEvent *)event
{
    
}

- (void)rightMouseDown:(NSEvent *)event
{
    
}
- (void)rightMouseUp:(NSEvent *)event
{
    
}

- (void)otherMouseDown:(NSEvent *)event
{
    
}

- (void)otherMouseUp:(NSEvent *)event
{
    
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	gEngine.onFocusGained();
}

- (void)windowDidResignKey:(NSNotification *)notification
{
	gEngine.onFocusLost();
}

- (void)windowWillClose:(NSNotification *)notification
{
	gEngine.end();
}

@end

