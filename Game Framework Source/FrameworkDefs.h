//
//  FrameworkDefs.h
//  Game Framework
//
//  Created by Ben Menke on 3/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

namespace Framework 
{
    //bit scan reverse platform independence undefined if bits==0
    __inline unsigned char BSR(unsigned long bits)
    {
#ifdef __GNUC__
        int result=0;
        int bsr=__builtin_clz(bits);
        int size=sizeof(bits)*8;
        result=size-bsr-1;
        return result;
#elif _MSC_VER
        unsigned long result;
        _BitScanReverse(&result,bits);
        return result;
#else
        for(size_t i=sizeof(long)-1; i>=0; i--)
        {
            if(bits>>i==1)
                return i;
        }
        return 0;
#endif
    }
    
#ifndef min
    __inline double min(double num1, double num2)
    {
        return num1<num2 ? num1 : num2;
    }
#endif

#ifndef max
    __inline double max(double num1, double num2)
    {
        return num1>num2 ? num1 : num2;
    }
#endif
    
    
    
    __inline double interpolate(double startValue, double endValue, double startPos, double endPos, double currentPos)
    {
        return startValue + (endValue-startValue)*((startPos-currentPos)/(startPos-endPos));
    }
}