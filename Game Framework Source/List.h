#pragma once
//
//  List.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2011 Menke. All rights reserved.
//

/*
Simple Efficient Linked list object
has a fast memory pool for them.
 */
#include <stddef.h>

namespace Framework
{
    //link used to determine free and used memory in a memory pool
    struct PoolLink
    {
    public:
        PoolLink* next;
        
        void* operator new(size_t size);
        void operator delete(void* p);
    };
    
    struct LinkMemPool
    {
        friend struct PoolLink;
    private:
        int location;
        int available;
        void* lastDelete;
        PoolLink* first;
        char buffer[120000];
        
        void PushLink(PoolLink* link);
        PoolLink* PopLink();
        
    public:
        LinkMemPool();
        void* GetMemory();
        void ReleaseMemory(void* object);
        
        //returns how many more links can be made
        int AvailableLinks();
    };
    extern LinkMemPool linkMemory;
    
    template<typename _Element>
    struct Link
    {
    public:
        Link<_Element>* next;
        Link<_Element>* prev;
        _Element* element;
        
        Link()
        {
            next=NULL;
            prev=NULL;
            element=NULL;
        }
        
        void* operator new(size_t size)
        {
            void* address=linkMemory.GetMemory();
            return address;
        }
        
        void operator delete(void* p)
        {
            linkMemory.ReleaseMemory(p);
        }
    };
    
    template<typename _Element>
    class List
    {
        //friend class List<_Element>;  used in visual studio
    private:
        Link<_Element>* first;
        Link<_Element> last;
        int numbElement;
        
    public:
        List()
        {
            first=&last;
            numbElement=0;
        }
        ~List()
        {
            while(pop()) {}
        }	
        
        int getSize()
        {
            return numbElement;
        }
        
        bool push(_Element* element)
        {
            Link<_Element>* link=new Link<_Element>;
            link->element=element;
            link->next=first;
            first->prev=link;
            first=link;
            numbElement+=1;
            return true;
        }
        bool pop()
        {
            if(first!=&last)
            {
                Link<_Element>* temp=first;
                temp->next->prev=NULL;
                first=temp->next;
                temp->next=NULL;
                temp->prev=NULL;
                numbElement-=1;
                delete temp;
                return true;
            }
            return false;
        }
        bool pushEnd(_Element* element)
        {
            Link<_Element>* link=new Link<_Element>;
            link->element=element;
            last->prev->next=link;
            link->prev=last->prev;
            link->next=last;
            last->prev=link;
            numbElement+=1;
            return true;
        }
        bool popEnd()
        {
            if(first!=last)
            {
                Link<_Element>* temp=last->prev;
                if(temp->prev!=NULL)
                {
                    temp->prev->next=temp->next;
                    temp->next->prev=temp->prev;
                }
                else
                {
                    first=last;
                }
                temp->next=NULL;
                temp->prev=NULL;
                numbElement-=1;
                delete temp;
                return true;
            }
            return false;
        }
        bool insert(_Element* element, Link<_Element>* location, bool before=false)
        {
            Link<_Element>* link=new Link<_Element>();
            link->element=element;
            
            if(before)
            {
                link->prev=location->prev;
                link->next=location;
                
                if(location->prev!=NULL)
                    location->prev->next=link;
                
                location->prev=link;
                
                if(location==first)
                    first=link;
            }
            else 
            {
                link->next=location->next;
                if(location->next!=NULL)
                    location->next->prev=link;
                
                location->next=link;
                link->prev=location;
				
            }
            
            numbElement+=1;
            return true;
        }
        bool remove(Link<_Element>* element)
        {
            if(element->next)
                element->next->prev=element->prev;
            
            if(element->prev)
                element->prev->next=element->next;
            
            if(element==first)
                first=element->next;
            
            element->next=NULL;
            element->prev=NULL;
            
            numbElement-=1;
            delete element;
            return true;
        }
        bool merge(List<_Element>* list, Link<_Element>* location)
        {
            if(list->last!=list->first)
            {
                list->last->prev->next=location->next;
                location->next->prev=list->last->prev;
                list->first->prev=location;
                location->next=list->first;
                list->first=list->last;
                numbElement+=list->numbElement;
                list->numbElement=0;
            }
            return true;
        }
        
        Link<_Element>* begin()
        {
            return first;
        }
        Link<_Element>* end()
        {
            return &last;
        }
    };
    
}
    
    
    
