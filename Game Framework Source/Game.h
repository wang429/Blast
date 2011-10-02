#pragma once
//
//  Game.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "List.h"
#include "GameState.h"

namespace Framework
{
    
    class Process;
    
    class Game
    {
    private:
        bool hasQuit;
        
        List<GameState> create;
        List<GameState> stateStack;
        List<Link<GameState> > cleanUp;
        
    public:
        
        ///////////////////////////////
        //Initialization and Shutdown
        ///////////////////////////////
        Game();
        virtual ~Game() {}
        
        /*
         Called by engine at startup
         */
        virtual void init();
        
        /*
         Called by engine after it exits from the main loop
         */
        virtual void shutdown();
        
        /*
         Quits the game
         */
        void quit();
        
        //update /* Update function called at interval specified in the engine */
        virtual void update(double deltaTime);
        /*render must be called by your update function it is not called by the engine */
        virtual void render(double deltaTime);
        
        //get game state
        bool isQuitting();
        
        void pushState(GameState* state);
        void popState(GameState* state); //removes all states to the given state. Null clears the stack.;
        
    };

}