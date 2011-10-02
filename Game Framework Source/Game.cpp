//
//  Game.cpp
//  Game Framework
//
//  Created by Ben Menke on 11/24/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "Engine.h"
#include "Game.h"
#include "Graphics.h"
#include "Image.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ImageLoader.h"
#include "Font.h"

using namespace Framework;

Game::Game()
{
}

Font testFont;

void Game::init()
{
}

void Game::shutdown()
{

}

void Game::quit()
{
	hasQuit=true;
	gEngine.end();
}

void Game::update(double deltaTime)
{
    //emergency shutdown
    if(gEngine.getKeyboard()->keyPressed(KEY_ESC))
        gEngine.end();
	
	//////////////////////////////
	//start of main processes
	//////////////////////////////
	
    //build and destroy game states
    for(Link<Link<GameState> >* iter=cleanUp.begin(); iter!=cleanUp.end(); )
    {
        Link<Link<GameState> >* temp=iter;
        iter=iter->next;
        
        temp->element->element->destroy();
        stateStack.remove(temp->element);
        cleanUp.remove(temp);
    }
    for(Link<GameState>* iter=create.begin(); iter!=create.end(); )
    {
        Link<GameState>* temp=iter;
        iter=iter->next;
        
        temp->element->initialize(this);
        stateStack.push(temp->element);
        create.remove(temp);
    }
	
	////////////////////
	//begin game update
	////////////////////
    
    //update gamestates
    for(Link<GameState>* iter=stateStack.begin(); iter!=stateStack.end(); iter=iter->next)
    {
        iter->element->update(deltaTime);
    }
	
	gGraphics.loadIdentity();
	gGraphics.clear();
    
	gGraphics.beginDraw();
	render(deltaTime);
	
	gGraphics.endDraw();
	gGraphics.flush();
}

void Game::render(double deltaTime)
{
    //render gamestates
    for(Link<GameState>* iter=stateStack.begin(); iter!=stateStack.end(); iter=iter->next)
    {
        iter->element->render(deltaTime);
    }
}

bool Game::isQuitting()
{
	return hasQuit;
}

void Game::pushState(GameState* state)
{
    create.push(state);
}

void Game::popState(GameState* state)
{
    for(Link<GameState>* iter=stateStack.end()->prev; iter!=NULL; /* iterates within loop */)
    {
        Link<GameState>* temp=iter;
        iter=iter->prev;
        GameState* state2=temp->element;
        cleanUp.push(temp);
        
        if(state2==state)
            break;
    }
}


