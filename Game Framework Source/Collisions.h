#pragma once
//
//  Collisions.h
//  Game Framework
//
//  Created by Ben Menke on 5/11/11.
//  Copyright 2011 Menke. All rights reserved.
//

// Collision System

#include "Rect.h"
#include <Vector>
#include <list>

namespace Framework 
{

class CollisionHull;

struct BoundingBox
{
    FRect bounds;
    CollisionHull* hull;
};

enum HullType 
{
	circularHull,
	rectangularHull,
	lineHull
};

/*
 Packet of collision data.
 */
struct CollisionPacket
{
	Vector normal;
    CollisionHull* hull1;
    CollisionHull* hull2;
	void* object1;	//in hulls object1 is the holding hull's object
	void* object2;
	float distance; //depth of collision along normal
};

class CircularHull;
class RectangularHull;
class LineHull;

class CollisionHull
{
	friend class CollisionManager;
protected: 
	void* object; //object stored with hull
    Vector center;
	Vector position; //position in collision system. updated through positionRef every update
	std::vector<CollisionPacket> packets;
	HullType type;
	bool active;
	bool dead; //mark for deletion
	bool moved;
	
public:
    virtual ~CollisionHull(){};
    
	HullType getType();
	bool isEnabled();
	void setActive(bool active);
	std::vector<CollisionPacket> getCollisions(); 
	void destroy();
	void setPosition(Vector position);
	
    void* getObject();
    void setObject(void* data);
    
    Vector getCenter();
	Vector getPosition();
	bool hasMoved(); //return true if moved this frame
	bool isDead();
	
	//virtual methods
    virtual BoundingBox getBoundingBox()=0;
	virtual bool collides(CollisionHull* other, CollisionPacket* packet)=0; 
	
    virtual float distanceToPoint(Framework::Vector point)=0;
    
	virtual void debugDraw()=0;
	
	static bool collideRectToRect(RectangularHull* hull1, RectangularHull* hull2, CollisionPacket* packet);
	static bool collideCirleToCicle(CircularHull* hull1, CircularHull* hull2, CollisionPacket* packet);
	static bool collideLineToLine(LineHull* hull1, LineHull* hull2, CollisionPacket* packet);
	static bool collideRectToLine(RectangularHull* rectHull, LineHull* lineHull, CollisionPacket* packet); 
	static bool collideRectToCircle(RectangularHull* rectHull, CircularHull* circleHull, CollisionPacket* packet);
	static bool collideLineToCircle(LineHull* lineHull, CircularHull* circleHull, CollisionPacket* packet);
	
};

class CircularHull : public CollisionHull
{
	friend class CollisionManager;
private:
	float radius;
	
public:
	float getRadius();
	
	BoundingBox getBoundingBox();
	bool collides(CollisionHull* other, CollisionPacket* packet);
	float distanceToPoint(Framework::Vector point);
	void debugDraw();
};

class RectangularHull : public CollisionHull
{
	friend class CollisionManager;
private:
	FRect rect;
public:
	FRect getRect();
	
	BoundingBox getBoundingBox();
	bool collides(CollisionHull* other, CollisionPacket* packet);
	float distanceToPoint(Framework::Vector point);
	void debugDraw();
};

class LineHull : public CollisionHull
{
	friend class CollisionManager;
private:
	Vector p1;
	Vector p2;
public:
	Vector getPoint1();
	Vector getPoint2();
	
	BoundingBox getBoundingBox();
	bool collides(CollisionHull* other, CollisionPacket* packet);
	float distanceToPoint(Framework::Vector point);
	void debugDraw();
};

struct CollisionPair
{
    CollisionHull* hull1;
    CollisionHull* hull2;
};

class SweepLine
{
private:
    struct EndPoint
    {
	public:
		BoundingBox box;
        float pos;
        float endPoint; //other end of line
		float yPos;
    };
	
    std::vector<EndPoint> points;
	
	void radixSort();
	
public:
    SweepLine();
    SweepLine(CollisionHull* hulls, int numEntries);
    
    void insert(CollisionHull* hull); 
    
    void updateLine(); //updates bounding box positions and removes dead boxes
	std::vector<CollisionPair> processOverlaps();
    
    void debugDraw();
};

class CollisionManager
{
private:
	std::list<CollisionHull*> collisionHulls;
	std::vector<CollisionPacket> collisionCache;
	std::vector<CollisionPair> collisionPairs;
	SweepLine sweep;
	
	void prepHulls();
	void checkCollisions();
	void cleanHulls();
	void broadphase();
	void narrowphase();
	
public:
	CollisionManager();
	
	CollisionHull* createCircularHull(float radius,  void* object);
	CollisionHull* createRectangularHull(FRect bounds,  void* object);
	CollisionHull* createLineHull(Vector p1, Vector p2, void* object);
	
	void update();
	
	void debugDraw();
};

    
}
