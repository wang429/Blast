
//
//  List.cpp
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include "List.h"


using namespace Framework;

LinkMemPool Framework::linkMemory;

void* PoolLink::operator new(size_t size)
{
	void* value;
	//handles initial creation of links
	if(linkMemory.location<120000)
	{
		value=&linkMemory.buffer[linkMemory.location];
		
	}
	//handles recreation of links when objects are released
	else
	{
		value=linkMemory.lastDelete;
	}
	return value;
}

void PoolLink::operator delete(void* p)
{
	
}

LinkMemPool::LinkMemPool()
{
	first=NULL;
	lastDelete=NULL;
	location=0;
	available=40000/sizeof(char*);
	for(int i=0; i<available; i++)
	{
		PushLink(new PoolLink());
		location+=sizeof(char*)*3;
	}
}

void LinkMemPool::PushLink(PoolLink* link)
{	
	link->next=first;
	first=link;
}

PoolLink* LinkMemPool::PopLink()
{
	PoolLink* temp=first;
	if(first!=NULL)
		first=first->next;
	
	return temp;
}

void* LinkMemPool::GetMemory()
{
	if(available>0)
	{
		void* address=PopLink();
		available-=1;
		return address;
	}
	return NULL;
}

void LinkMemPool::ReleaseMemory(void *object)
{
	lastDelete=object;
	PushLink(new PoolLink);
	lastDelete=NULL;
	available+=1;
}

int LinkMemPool::AvailableLinks()
{
	return available;
}