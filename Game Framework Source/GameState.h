#pragma once
//
//  GameStateFactory.h
//  Game Framework
//
//  Created by Ben Menke on 3/16/11.
//  Copyright 2011 Menke. All rights reserved.
//


//  Handles the creation and cleanup of various gamestates.
//  GameStates get updated and rendered once per frame and
//  handle any specific processes they need to.
//  Managed by the game object.

namespace Framework
{

    class Game;
    
    class GameState
    {
        friend class Game;
    public:
        virtual ~GameState() {};
        
    private:
        /*
         Creates the game state.
         */
        virtual void initialize(Game* game)=0;
        
        virtual void update(double deltaTime)=0;
        virtual void render(double deltaTime)=0;
        
        /*
         cleans up after its self
         */
        virtual void destroy()=0;
        
    };

}

