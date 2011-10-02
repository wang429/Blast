//
//  Collisions.cpp
//  Game Framework
//
//  Created by Ben Menke on 5/11/11.
//  Copyright 2011 Menke. All rights reserved.

#include "Collisions.h"
#include "Graphics.h"
#include "FrameworkDefs.h"

using namespace Framework;

bool CollisionHull::collideRectToRect(RectangularHull* hull1, RectangularHull* hull2, CollisionPacket* packet)
{
	Vector p1=hull1->position;
	Vector p2=hull2->position;
	
	FRect bounds1=hull1->getRect();
	FRect bounds2=hull2->getRect();
	
	float left1=p1.x+bounds1.x;
	float right1=left1+bounds1.width;
	float bottom1=p1.y+bounds1.y;
	float top1=bottom1+bounds1.height;
	
	float left2=p2.x+bounds2.x;
	float right2=left2+bounds2.width;
	float bottom2=p2.y+bounds2.y;
	float top2=bottom2+bounds2.height;
	
	if(left1>right2) return false;
	if(left2>right1) return false;
	if(bottom1>top2) return false;
	if(bottom2>top1) return false;
	
	if(packet==NULL)
		return true;
	
	packet->object1=hull1->object;
	packet->object2=hull2->object;
    packet->hull1=hull1;
    packet->hull2=hull2;
	
	//seperating distances if hull 1 moves
	float left=right1-left2;
	float right=right2-left1;
	float bottom=top1-bottom2;
	float top=top2-bottom1;
	
	float distance=min(min(left,right),min(top,bottom));
	Vector normal;
	if(distance==left)
		normal=Vector(-1,0);
	if(distance==right)
		normal=Vector(1,0);
	if(distance==bottom)
		normal=Vector(0,-1);
	if(distance==top)
		normal=Vector(0,1);
	
	packet->normal=normal;
	packet->distance=distance;
	
	return true;
}

bool CollisionHull::collideCirleToCicle(CircularHull* hull1, CircularHull* hull2, CollisionPacket* packet)
{
	Vector p1=hull1->position;
	Vector p2=hull2->position;
	
	Vector sepAxis=p1-p2;
	float distance=sepAxis.length();
	float radiusSum=hull1->getRadius()+hull2->getRadius();
	if(distance>radiusSum)
		return false;
	if(packet!=NULL)
	{
		packet->object1=hull1->object;
		packet->object2=hull2->object;
        packet->hull1=hull1;
        packet->hull2=hull2;
		packet->normal=sepAxis.normalize();
		packet->distance=radiusSum-distance;
	}
	return true;
}

bool CollisionHull::collideLineToLine(LineHull* hull1, LineHull* hull2, CollisionPacket* packet)
{
	Vector position1=hull1->getPosition();
	Vector position2=hull2->getPosition();
	Vector ps1=hull1->getPoint1()+position1;
	Vector pe1=hull1->getPoint2()+position1;
	Vector ps2=hull2->getPoint1()+position2;
	Vector pe2=hull2->getPoint2()+position2;
	
	
	Vector axis1=pe1-ps1;
	double temp=axis1.x;
	axis1.x=-axis1.y;
	axis1.y=temp;
	
	Vector axis2=pe2-ps2;
	temp=axis2.x;
	axis2.x=-axis2.y;
	axis2.y=temp;
	
	axis1.normalize();
	axis2.normalize();
	
	float pl1=dot(axis1, ps1);
	float pr1=dot(axis1, pe1);
	
	float pl2=dot(axis1, ps2);
	float pr2=dot(axis1, pe2);
	
	if(pl2>pr2)
	{
		float t=pl2;
		pl2=pr2;
		pr2=t;
	}
	
	if(pl1<pl2||pl1>pr2) return false;
	
	float distance1=min(pl1-pl2,pr2-pl1);
	if(distance1==pl1-pl2)
		axis1=Vector(-axis1.x,-axis1.y);
    
	pl1=dot(axis2, ps1);
	pr1=dot(axis2, pe1);
	
	pl2=dot(axis2, ps2);
	pr2=dot(axis2, pe2);
	
	if(pl1>pr1)
	{
		float t=pl1;
		pl1=pr1;
		pr1=t;
	}
	
	if(pl2<pl1||pl2>pr1) return false;
	
	float distance2=min(pl2-pl1,pr1-pl2);
	if(distance2==pl2-pl1)
		axis2=Vector(-axis2.x,-axis2.y);
	
	if(packet!=NULL)
	{
		float distance=min(distance1,distance2);
		Vector normal;
		
		if(distance==distance1)
			normal=axis1;
		else
			normal=axis2;
        
		packet->normal=normal;
		packet->distance=distance;
		packet->object1=hull1->object;
		packet->object2=hull2->object;
        packet->hull1=hull1;
        packet->hull2=hull2;
	}
	return true;
}

bool CollisionHull::collideRectToLine(RectangularHull* rectHull, LineHull* lineHull, CollisionPacket* packet)
{
	Vector linePosition=lineHull->getPosition();
	Vector p1=lineHull->getPoint1()+linePosition;
	Vector p2=lineHull->getPoint2()+linePosition;
	
	Vector rectPosition=rectHull->getPosition();
	FRect bounds1=rectHull->getRect();
	
	float left1=rectPosition.x+bounds1.x;
	float right1=left1+bounds1.width;
	float bottom1=rectPosition.y+bounds1.y;
	float top1=bottom1+bounds1.height;
    
	//get left and right line points
	float left2=min(p1.x, p2.x);
	float right2=max(p1.x, p2.x);
	float bottom2=min(p1.y, p2.y);
	float top2=max(p1.y, p2.y);
	
	if(left1>right2) return false;
	if(left2>right1) return false;
	if(bottom1>top2) return false;
	if(bottom2>top1) return false;
	
	
	//create lineAxis
	Vector lineAxis=lineHull->getPoint2()-lineHull->getPoint1();
	double temp=lineAxis.x;
	lineAxis.x=-lineAxis.y;
	lineAxis.y=temp;
	lineAxis.normalize();
	
	//find min and max rect points on axis
	float rectP1;
	float rectP2;
    
	Vector rectPoint(left1,bottom1);
	rectP1=rectP2=dot(rectPoint, lineAxis);
	
	rectPoint=Vector(left1,top1);
	float dotp=dot(rectPoint, lineAxis);
	if(dotp<rectP1)
		rectP1=dotp;
	if(dotp>rectP2)
		rectP2=dotp;
	
	rectPoint=Vector(right1,top1);
	dotp=dot(rectPoint, lineAxis);
	if(dotp<rectP1)
		rectP1=dotp;
	if(dotp>rectP2)
		rectP2=dotp;
	
	rectPoint=Vector(right1,bottom1);
	dotp=dot(rectPoint, lineAxis);
	if(dotp<rectP1)
		rectP1=dotp;
	if(dotp>rectP2)
		rectP2=dotp;
	
	float point=dot(lineAxis,p1);
	
	if(rectP1>point) return false;
	if(rectP2<point) return false;
	
	if(packet!=NULL)
	{
		float distance1=right1-left2;
		float distance2=right2-left1;
		float distance3=top1-bottom2;
		float distance4=top2-bottom1;
		float distance5=point-rectP1;
		float distance6=rectP2-point;
        
		float distance=min(min(distance1, distance2),min(min(distance3, distance4), min(distance5, distance6)));
        Vector testPoint;
		Vector normal;
		if(distance==distance1)
			normal=Vector(-1,0);
		else if(distance==distance2)
			normal=Vector(1,0);
		else if(distance==distance3)
			normal=Vector(0,-1);
		else if(distance==distance4)
			normal=Vector(0,1);
		else if(distance==distance5)
			normal=lineAxis;
		else if(distance==distance6)
			normal=Vector(-lineAxis.x,-lineAxis.y);
		
		packet->normal=normal;
		packet->distance=distance;
		packet->object1=rectHull->object;
		packet->object2=lineHull->object;
        packet->hull1=rectHull;
        packet->hull2=lineHull;
		
	}
	
	
	
	
	return true;
}

bool CollisionHull::collideRectToCircle(RectangularHull* rectHull, CircularHull* circleHull, CollisionPacket* packet)
{
	Vector circleCenter=circleHull->getPosition();
	float radius=circleHull->getRadius();
	
	Vector rectPosition=rectHull->getPosition();
	FRect bounds1=rectHull->getRect();
	
	float left1=rectPosition.x+bounds1.x;
	float right1=left1+bounds1.width;
	float bottom1=rectPosition.y+bounds1.y;
	float top1=bottom1+bounds1.height;
	
	//check rect axis
	if(left1>circleCenter.x+radius) return false;
	if(right1<circleCenter.x-radius) return false;
	if(top1<circleCenter.y-radius) return false;
	if(bottom1>circleCenter.y+radius) return false;
	
	//find nearest corner to circle
	float distance[4];
	Vector points[4];
	
	points[0]=Vector(right1,top1);
	points[1]=Vector(left1,top1);
	points[2]=Vector(right1,bottom1);
	points[3]=Vector(left1,bottom1);
	
	distance[0]=Framework::distanceSquared(circleCenter,points[0]);
	distance[1]=Framework::distanceSquared(circleCenter,points[1]);
	distance[2]=Framework::distanceSquared(circleCenter,points[2]);
	distance[3]=Framework::distanceSquared(circleCenter,points[3]);
	float minDistance=distance[0];
	Vector point=points[0];
	for(int i=1; i<4; i++)
	{
		if(distance[i]<minDistance)
		{
			minDistance=distance[i];
			point=points[i];
		}
	}
	
	Vector axis=point-circleCenter;
	axis.normalize();
	float circleDot=dot(circleCenter, axis);
	
	float nearDot=dot(points[0], axis);
	for(int i=1; i<4; i++)
	{
		float tempDot=dot(points[i], axis);
		if(tempDot<nearDot)
			nearDot=tempDot;
	}
	if(nearDot>radius+circleDot) return false;
	
	if(packet!=NULL)
	{
		Vector sepAxis[5];
		sepAxis[0]=axis;
		sepAxis[1]=Vector(1,0);
		sepAxis[2]=Vector(-1,0);
		sepAxis[3]=Vector(0,-1);
		sepAxis[4]=Vector(0,1);
		
		float sepDistance[5];
		sepDistance[0]=radius+circleDot-nearDot;
		sepDistance[1]=(circleCenter.x+radius)-left1;
		sepDistance[2]=right1-(circleCenter.x-radius);
		sepDistance[3]=top1-(circleCenter.y-radius);
		sepDistance[4]=(circleCenter.y+radius)-bottom1;
		
		Vector finalAxis=sepAxis[0];
		float finalDistance=sepDistance[0];
		for(int i=1; i<5; i++)
		{
			if(sepDistance[i]<finalDistance)
			{
				finalDistance=sepDistance[i];
				finalAxis=sepAxis[i];
			}
		}
		
		packet->object1=rectHull->object;
		packet->object2=circleHull->object;
		packet->distance=finalDistance;
		packet->normal=finalAxis;
        packet->hull1=rectHull;
        packet->hull2=circleHull;
	}
	
	
	return true;
}

bool CollisionHull::collideLineToCircle(LineHull* lineHull, CircularHull* circleHull, CollisionPacket* packet)
{
	
	Vector lineP1=lineHull->getPoint1()+lineHull->getPosition();
	Vector lineP2=lineHull->getPoint2()+lineHull->getPosition();
	//create lineAxis
	Vector lineAxis=lineP2-lineP1;
	double temp=lineAxis.x;
	lineAxis.x=-lineAxis.y;
	lineAxis.y=temp;
	lineAxis.normalize();
	
	Vector circleCenter=circleHull->position;
	float radius=circleHull->getRadius();
	float circleDot=dot(lineAxis, circleCenter);
	float lineDot=dot(lineAxis,lineP1);
	//check line axis
	if(circleDot+radius<lineDot) return false;
	if(circleDot-radius>lineDot) return false;
	
	//find nearest point to circle center 
	float distance1=distanceSquared(circleCenter,lineP1);
	float distance2=distanceSquared(circleCenter,lineP2);
	Vector linePoint;
	if(distance1<distance2)
	{
		linePoint=lineP1;
	}
	else
	{
		linePoint=lineP2;
	}
    
	Vector axis=linePoint-circleCenter;
	axis.normalize();
	float circleCenterDot=dot(circleCenter, axis);
	
	float dot1=dot(lineP1,axis);
	float dot2=dot(lineP2,axis);
	float nearDot=min(dot1,dot2);
	
	if(nearDot>radius+circleCenterDot) return false;
	
	if(packet!=NULL)
	{
		float sepDistance1=lineDot-(circleDot-radius);
		float sepDistance2=(radius+circleCenterDot)-nearDot;
		
		Vector sepAxis;
		float sepDistance;
		if(sepDistance1<sepDistance2)
		{
			sepAxis=lineAxis;
			sepDistance=distance1;
		}
		else
		{
			sepAxis=axis;
			sepDistance=distance2;
		}
        
		packet->object1=lineHull->object;
		packet->object2=circleHull->object;
		packet->distance=sepDistance;
		packet->normal=sepAxis;
        packet->hull1=lineHull;
        packet->hull2=circleHull;
	}
	
	return true;
}

HullType CollisionHull::getType()
{
	return type;
}

bool CollisionHull::isEnabled()
{
	return active;
}

void CollisionHull::setActive(bool active)
{
	this->active=active;
}

void CollisionHull::setObject(void* data)
{
    object=data;
}

void* CollisionHull::getObject()
{
    return object;
}

std::vector<CollisionPacket> CollisionHull::getCollisions()
{
	int numCollisions=(int)packets.size();
    std::vector<CollisionPacket> collisions(numCollisions);
	for(int i=0; i<numCollisions; i++)
	{
		collisions[i]=packets[i];
	}
	
	return collisions;
}

void CollisionHull::destroy()
{
	dead=true;
}

void CollisionHull::setPosition(Vector position)
{
	moved=true;
	this->position=position;
}

Vector CollisionHull::getPosition()
{
	return position;
}
Vector CollisionHull::getCenter()
{
    return position+center;
}

bool CollisionHull::hasMoved()
{
	return moved;
}

bool CollisionHull::isDead()
{
	return dead;
}

float CircularHull::getRadius()
{
	return radius;
}

BoundingBox CircularHull::getBoundingBox()
{
	BoundingBox result;
	result.hull=this;
	result.bounds.x=-radius;
	result.bounds.y=-radius;
	result.bounds.width=radius*2;
	result.bounds.height=radius*2;
	
	return result;
}

bool CircularHull::collides(CollisionHull* other, CollisionPacket* packet)
{
	HullType type=other->getType();
	switch(type)
	{
		case circularHull:
			return collideCirleToCicle(this, static_cast<CircularHull*>(other), packet);
			break;
            
		case rectangularHull:
			if(collideRectToCircle(static_cast<RectangularHull*>(other), this, packet))
			{
				if(packet!=NULL)
				{
					void* temp=packet->object1;
					packet->object1=packet->object2;
					packet->object2=temp;
                    CollisionHull* tempHull=packet->hull1;
                    packet->hull1=packet->hull2;
                    packet->hull2=tempHull;
					packet->normal.x=-packet->normal.x;
					packet->normal.y=-packet->normal.y;
				}
				return true;
			}
			return false;
			break;
			
		case lineHull:
			if(collideLineToCircle(static_cast<LineHull*>(other), this, packet))
			{
				if(packet!=NULL)
				{
                    void* temp=packet->object1;
					packet->object1=packet->object2;
					packet->object2=temp;
                    CollisionHull* tempHull=packet->hull1;
                    packet->hull1=packet->hull2;
                    packet->hull2=tempHull;
					packet->normal.x=-packet->normal.x;
					packet->normal.y=-packet->normal.y;
				}
				return true;
			}
			return false;
			break;
	}
	return false;
}

float CircularHull::distanceToPoint(Framework::Vector point)
{
    return distance(point, position);
}

void CircularHull::debugDraw()
{
	Color color=colorWhite;
	if(packets.size()>0)
	{
		color.r=255;
		color.g=0;
		color.b=0;
		color.a=.8*255;
	}
	Vertex verts[]={radius*cos(0.0),radius*sin(0.0),0,color,0,0,radius*cos(36*PI/180),radius*sin(36*PI/180),0,color,0,0,radius*cos(72*PI/180),radius*sin(72*PI/180),0,color,0,0,radius*cos(108*PI/180),radius*sin(108*PI/180),0,color,0,0,radius*cos(144*PI/180),radius*sin(144*PI/180),0,color,0,0,radius*cos(180*PI/180),radius*sin(180*PI/180),0,color,0,0,radius*cos(216*PI/180),radius*sin(216*PI/180),0,color,0,0,radius*cos(252*PI/180),radius*sin(252*PI/180),0,color,0,0,radius*cos(288*PI/180),radius*sin(288*PI/180),0,color,0,0,radius*cos(324*PI/180),radius*sin(324*PI/180),0,color,0,0};
    
	gGraphics.pushMatrix();
	gGraphics.loadIdentity();
	gGraphics.setTranslate(position.x, position.y);
	gGraphics.setDrawMode(TRIANGLE_FAN);
	gGraphics.setImage(NULL);
	gGraphics.drawShape(verts, 10);
	gGraphics.popMatrix();
}

FRect RectangularHull::getRect()
{
	return rect;
}

BoundingBox RectangularHull::getBoundingBox()
{
	BoundingBox result;
	result.hull=this;
	result.bounds=rect;
	
	return result;
}

bool RectangularHull::collides(CollisionHull* other, CollisionPacket* packet)
{
	
	HullType type=other->getType();
	switch(type)
	{
		case circularHull:
			return collideRectToCircle(this, static_cast<CircularHull*>(other), packet);
			break;
			
		case rectangularHull:
			return collideRectToRect(this, static_cast<RectangularHull*>(other), packet);
			break;
			
		case lineHull:
			return collideRectToLine(this, static_cast<LineHull*>(other), packet);
			break;
	}
	return false;
}

float RectangularHull::distanceToPoint(Framework::Vector point)
{
	float left=rect.x+position.x;
	float right=left+rect.width;
	float bottom=rect.y+position.y;
	float top=bottom+rect.height;
    
    if(point.x>left&&point.x<right)
    {
        return min(fabs(point.y-top), fabs(point.y-bottom));
    }
    else if(point.y>bottom&&point.x<top)
    {
        return min(fabs(point.x-right), fabs(point.x-left));
    }
    else
    {
        return min(min(distance(point, Vector(left,bottom)), distance(point, Vector(left,top))),min(distance(point, Vector(right,bottom)), distance(point, Vector(right,top))));
    }
}

void RectangularHull::debugDraw()
{
	Color color=colorWhite;
	if(packets.size()>0)
	{
		color.r=255;
		color.g=0;
		color.b=0;
		color.a=.8*255;
	}
	
	Vertex verts[]={rect.x,rect.y,0,color,0,0,rect.x+rect.width,rect.y,0,color,0,0,rect.x+rect.width,rect.y+rect.height,0,color,0,0,rect.x,rect.y+rect.height,0,color,0,0};
    
	gGraphics.pushMatrix();
	gGraphics.loadIdentity();
	gGraphics.setTranslate(position.x, position.y);
	gGraphics.setDrawMode(TRIANGLE_FAN);
	gGraphics.setImage(NULL);
	gGraphics.drawShape(verts, 4);
	gGraphics.popMatrix();
}

Vector LineHull::getPoint1()
{
	return p1;
}

Vector LineHull::getPoint2()
{
	return p2;
}

BoundingBox LineHull::getBoundingBox()
{
	BoundingBox result;
	result.hull=this;
	result.bounds.x=min(p1.x, p2.x);
	result.bounds.y=min(p1.y, p2.y);
	result.bounds.width=abs(p2.x-p1.x);
	result.bounds.height=abs(p2.y-p1.y);
	
	return result;
}

bool LineHull::collides(CollisionHull* other, CollisionPacket* packet)
{
	
	HullType type=other->getType();
	switch(type)
	{
		case circularHull:
			return collideLineToCircle(this, static_cast<CircularHull*>(other), packet);
			break;
			
		case rectangularHull:
			if(collideRectToLine(static_cast<RectangularHull*>(other), this, packet))
			{
				if(packet!=NULL)
				{
					void* temp=packet->object1;
					packet->object1=packet->object2;
					packet->object2=temp;
                    CollisionHull* tempHull=packet->hull1;
                    packet->hull1=packet->hull2;
                    packet->hull2=tempHull;
					packet->normal.x=-packet->normal.x;
					packet->normal.y=-packet->normal.y;
				}
				return true;
			}
			return false;
			break;
			
		case lineHull:
			return collideLineToLine(this, static_cast<LineHull*>(other), packet);
			break;
	}
	return false;
}

float LineHull::distanceToPoint(Framework::Vector point)
{
    Vector direction=p2-p1;
    direction.normalize();
    Vector vec2=point-p1;
    
    float point1=dot(direction, p1);
    float point2=dot(direction, p2);
    float point3=dot(direction, point);
    
    if(point3>point2||point3<point1)
        return min(distance(point, p1), distance(point, p2));
    
    return fabs(cross(direction, vec2));
}

void LineHull::debugDraw()
{	
	Color color=colorWhite;
	if(packets.size()>0)
	{
		color.r=255;
		color.g=0;
		color.b=0;
		color.a=.8*255;
	}
	
	Vertex verts[]={p1.x,p1.y,0,color,0,0,p2.x,p2.y,0,color,0,0};
	
	gGraphics.pushMatrix();
	gGraphics.loadIdentity();
	gGraphics.setTranslate(position.x, position.y);
	gGraphics.setDrawMode(LINES);
	gGraphics.setImage(NULL);
	gGraphics.drawShape(verts, 2);
	gGraphics.popMatrix();
}
CollisionManager::CollisionManager()
{
	
}

void CollisionManager::broadphase()
{
	sweep.updateLine();
	collisionPairs=sweep.processOverlaps();
}

void CollisionManager::narrowphase()
{
	for(std::list<CollisionHull*>::iterator iter=collisionHulls.begin(); iter!=collisionHulls.end(); iter++)
    {
		(*iter)->packets.clear();
	}
	
	for(int i=0; i<collisionPairs.size(); i++)
	{
		CollisionPair pair=collisionPairs[i];
		if(!pair.hull1->isEnabled())
			continue;
		if(!pair.hull2->isEnabled())
			continue;
		
		CollisionPacket packet;
		if(pair.hull1->collides(pair.hull2,&packet))
		{
			pair.hull1->packets.push_back(packet);
			void* temp=packet.object1;
			packet.object1=packet.object2;
			packet.object2=temp;
			packet.normal.x=-packet.normal.x;
			packet.normal.y=-packet.normal.y;
			pair.hull2->packets.push_back(packet);
		}
	}
}

CollisionHull* CollisionManager::createCircularHull(float radius, void* object)
{
	CircularHull* hull=new CircularHull();
	hull->radius=radius;
	hull->object=object;
	hull->type=circularHull;
	hull->active=true;
	hull->dead=false;
	hull->moved=true;
    hull->center=Vector(0,0);
	
	collisionHulls.push_back(hull);
	sweep.insert(hull);
	
	return hull;
}

CollisionHull* CollisionManager::createRectangularHull(FRect bounds, void* object)
{
	RectangularHull* hull=new RectangularHull();
	hull->rect=bounds;
	hull->object=object;
	hull->type=rectangularHull;
	hull->active=true;
	hull->dead=false;
	hull->moved=true;
    hull->center=Vector(bounds.width/2.0-bounds.x,bounds.height/2.0-bounds.y);
	
    collisionHulls.push_back(hull);
	sweep.insert(hull);
	return hull;
}

CollisionHull* CollisionManager::createLineHull(Vector p1, Vector p2, void* object)
{
	LineHull* hull=new LineHull();
	hull->p1=p1;
	hull->p2=p2;
	hull->object=object;
	hull->type=lineHull;
	hull->active=true;
	hull->dead=false;
	hull->moved=true;
    hull->center=Vector((p2.x-p1.x)/2.0,(p2.y-p1.x)/2.0);
	
	collisionHulls.push_back(hull);
	sweep.insert(hull);
	return hull;
}

void CollisionManager::prepHulls()
{
	for(std::list<CollisionHull*>::iterator iter=collisionHulls.begin(); iter!=collisionHulls.end(); iter++)
    {
		(*iter)->packets.clear();
	}
}

void CollisionManager::checkCollisions()
{
	broadphase();
	narrowphase();
}

void CollisionManager::cleanHulls()
{
	for(std::list<CollisionHull*>::iterator iter=collisionHulls.begin(); iter!=collisionHulls.end(); iter++)
    {
		if((*iter)->isDead())
		{
            collisionHulls.erase(iter);
		}
	}
}

void CollisionManager::update()
{
	prepHulls();
	checkCollisions();
	cleanHulls();
}

void CollisionManager::debugDraw()
{	
	for(std::list<CollisionHull*>::iterator iter=collisionHulls.begin(); iter!=collisionHulls.end(); iter++)
    {
		(*iter)->debugDraw();
	}
	sweep.debugDraw();
}

SweepLine::SweepLine()
{
}

SweepLine::SweepLine(CollisionHull* hulls, int numEntries)
{
	points.resize(numEntries*3);
	
	for(int i=0; i<numEntries; i++)
	{
		BoundingBox box=hulls[i].getBoundingBox();
		points[i].box=box;
		points[i].pos=hulls->getPosition().x+box.bounds.x;
		points[i].yPos=hulls->getPosition().y+box.bounds.y;
		points[i].endPoint=points[i].pos+box.bounds.width;
	}
}

void SweepLine::radixSort()
{
	std::vector<EndPoint> buffer;
	buffer.resize(points.size());
	
	//build counters and check if already sorted
	int counters[4][256];
	memset(counters, 0, sizeof(int)*256*4);
	bool sorted=true;
	for(int i=0; i<points.size(); i++)
	{
		unsigned int posInt=(unsigned int&)points[i].pos;
		unsigned char radix1=(posInt) & 0xFF;
		unsigned char radix2=(posInt>>8) & 0xFF;
		unsigned char radix3=(posInt>>16) & 0xFF;
		unsigned char radix4=(posInt>>24) & 0xFF;
		counters[0][radix1]++;
		counters[1][radix2]++;
		counters[2][radix3]++;
		counters[3][radix4]++;
		
		if(sorted&&i<points.size()-1&&points[i].pos>=points[i+1].pos)
			sorted=false;
	}
	
	//negatives are in last 128 of counter 4
	int numNegative=0;
	for(int i=128; i<256; i++)
	{
		numNegative+=counters[3][i];
	}
	
	for(int pass=0; pass<4; pass++)
	{
		unsigned int offsets[256];
		offsets[0]=pass==3 ? numNegative : 0;						 
		for(int i=1; i<256; i++)
		{
			offsets[i]=offsets[i-1]+counters[pass][i-1];
		}
		
		//fix negative offsets
		if(pass==3&&numNegative>0)
		{
			offsets[255]=0;
			for(int i=255; i>128; i--)
			{
				offsets[i-1]=offsets[i]+counters[pass][i];
			}
		}
		
		std::vector<EndPoint>* destBuffer= pass%2==0 ? &buffer : &points;
		std::vector<EndPoint>* srcBuffer= pass%2==0 ? &points : &buffer;
		for(int i=0; i<srcBuffer->size(); i++)
		{
			unsigned int posInt=(unsigned int&)(*srcBuffer)[i].pos;
			unsigned char radix=(posInt>>(pass<<3)) & 0xFF;
			
			posInt=offsets[radix];
			offsets[radix]++;
			
			(*destBuffer)[posInt]=(*srcBuffer)[i];
		}
	}
}

void SweepLine::insert(CollisionHull* hull)
{
	BoundingBox box=hull->getBoundingBox();
	EndPoint p1;
	p1.pos=hull->getPosition().x+box.bounds.x;
	p1.endPoint=p1.pos+box.bounds.width;
	p1.box=box;
	p1.yPos=hull->getPosition().y+box.bounds.y;
	points.push_back(p1);
}

void SweepLine::updateLine()
{
	for(int i=0; i<points.size(); i++)
	{
		BoundingBox box=points[i].box;
		CollisionHull* hull=box.hull;
		
		if(hull->isDead())
		{
			points[i]=points[points.size()-1];
			points.pop_back();
			i--;
			continue;
		}
		
		if(hull->hasMoved())
		{
			Vector pos=hull->getPosition();
			
			points[i].pos=box.bounds.x+pos.x;
			points[i].endPoint=points[i].pos+box.bounds.width;
			points[i].yPos=box.bounds.y+pos.y;
		}
		
	}
	
	radixSort();
}

std::vector<CollisionPair> SweepLine::processOverlaps()
{
	std::vector<CollisionPair> pairs;
	std::vector<EndPoint> currentPoints;
	for(int i=0; i<points.size(); i++)
	{
		float pos=points[i].pos;
        
        //clear points we have past
        for(int j=0; j<currentPoints.size(); j++)
        {
			if(currentPoints[j].endPoint<pos)
			{
				currentPoints[j]=currentPoints[currentPoints.size()-1];
				currentPoints.pop_back();
				j--;
				continue;
			}
        }
		currentPoints.push_back(points[i]);
        
		for(int j=0; j<currentPoints.size()-1; j++)
		{
			EndPoint p1=currentPoints[j];
			EndPoint p2=currentPoints[currentPoints.size()-1];
			
			float top1=p1.yPos+p1.box.bounds.height;
			float top2=p2.yPos+p2.box.bounds.height;
			float bottom1=p1.yPos;
			float bottom2=p2.yPos;
			if(top1<bottom2) continue;
			if(top2<bottom1) continue;
			
			CollisionPair pair;
			pair.hull1=p1.box.hull;
			pair.hull2=p2.box.hull;
			pairs.push_back(pair);
		}
	}
	
	return pairs;
}

void SweepLine::debugDraw()
{
	gGraphics.pushMatrix();
	gGraphics.loadIdentity();
	
	gGraphics.setDrawMode(LINES);
	gGraphics.setImage(NULL);
	Vertex line[]={0,32,0,0,1,1,1,1,640,32,0,0,1,1,1,1};
	
	gGraphics.drawShape(line, 2);
	
	line[0].y=48;
	line[1].y=16;
	for(int i=0; i<points.size(); i++)
	{
		line[0].x=points[i].pos;
		line[1].x=points[i].pos;
		gGraphics.drawShape(line, 2);
		
		line[0].x=points[i].endPoint;
		line[1].x=points[i].endPoint;
		gGraphics.drawShape(line, 2);
		
	}
	
	gGraphics.popMatrix();
}