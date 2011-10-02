#pragma once
//
//  Vector.h
//  Game Framework
//
//  Created by Ben Menke on 12/7/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include <Math.h>
#define PI 3.141592654

namespace Framework 
{
    struct Vector
    {
        double x;
        double y;
        
        Vector()
        {
            x=0;
            y=0;
        }
        Vector(double inX, double inY)
        {
            x=inX;
            y=inY;
        }
        
        __inline double length()
        {
            return sqrt(x*x+y*y);
        }
        __inline double lengthSquared()
        {
            return x*x+y*y;
        }
        
        __inline Vector normalize() 
        {
            double mag=sqrt(x*x+y*y);
			
			if(mag==0)
				x=1;
			else 
			{
				x/=mag;
				y/=mag;
			}

            return *this;
        }
        
        
        __inline double direction()
        {
            return atan2(y, x);
        }
        
        __inline Vector invert()
        {
            return Vector(-x,-y);
        }
        
        __inline Vector GetNormal()
        {
            return Vector(-y,x).normalize();
        }
    };
    
	__inline double distanceSquared(Vector v1, Vector v2)
	{
		return pow((v2.x-v1.x),2)+pow((v2.y-v1.y), 2);
	}
	
	__inline double distance(Vector v1, Vector v2)
	{
		return sqrt(distanceSquared(v1, v2));
	}
    
    __inline double dot(Vector v1, Vector v2)
    {
        return v1.x*v2.x+v1.y*v2.y;
    }
    __inline double cross(Vector v1, Vector v2)
    {
        return v1.x*v2.y-v1.y*v2.x; 
    }
    
    //Vector Operators
    __inline bool operator==(Vector const &o, Vector const &o2) 
    {
        return (o.x==o2.x&&o.y==o2.y);
    }
    __inline bool operator!=(Vector const &o, Vector const &o2)  
    {
        return (o.x!=o2.x||o.y!=o2.y);
    }
    
    __inline Vector operator+(Vector const &o, Vector const &o2)  
    {
        return Vector(o.x+o2.x,o.y+o2.y);
    }
    __inline Vector operator-(Vector const &o, Vector const &o2)  
    {
        return Vector(o.x-o2.x,o.y-o2.y);
    }
    __inline Vector operator*(Vector const &o, Vector const &o2)  
    {
        return Vector(o.x*o2.x,o.y*o2.y);
    }
    __inline Vector operator/(Vector const &o, Vector const &o2)  
    {
        return Vector(o.x/o2.x,o.y/o2.y);
    }
    __inline Vector operator*(Vector const &o, double const &o2)  
    {
        return Vector(o.x*o2,o.y*o2);
    }
    __inline Vector operator/(Vector const &o, double const &o2) 
    {
        return Vector(o.x/o2,o.y/o2);
    }
    
    __inline Vector operator+=(Vector &o, Vector const &o2)  
    {
        o.x+=o2.x;
        o.y+=o2.y;
        return o;
    }
    __inline Vector& operator-=(Vector &o, Vector const &o2)  
    {
        o.x-=o2.x;
        o.y-=o2.y;
        return o;
    }
    __inline Vector& operator*=(Vector &o, Vector const &o2) 
    {
        o.x*=o2.x;
        o.y*=o2.y;
        return o;
    }
    __inline Vector& operator/=(Vector &o, Vector const &o2)
    {
        o.x/=o2.x;
        o.y/=o2.y;
        return o;
    }	
    __inline Vector& operator*=(Vector &o, double const &o2) 
    {
        o.x*=o2;
        o.y*=o2;
        return o;
    }
    __inline Vector& operator/=(Vector &o, double const &o2)
    {
        
        o.x/=o2;
        o.y/=o2;
        return o;
    }
    
    __inline Vector normal(Vector& p1, Vector& p2)
    {
        Vector v=p2-p1;
        return v.GetNormal();
    }
    
	
    __inline Vector directionVec(Vector start, Vector end)
    {
        Vector v=end-start;
        return v.normalize();
    }
}

