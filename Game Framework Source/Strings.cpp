/*
 -------------------------------------------------------------------------------
 lookup3.c, by Bob Jenkins, May 2006, Public Domain.
 */

#include <stdint.h>  
#include <string.h>
#include "Strings.h"

using namespace Framework;

#define hashsize(n) ((uint32_t)1<<(n))
#define hashmask(n) (hashsize(n)-1)
#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define mix(a,b,c) \
{ \
a -= c;  a ^= rot(c, 4);  c += b; \
b -= a;  b ^= rot(a, 6);  a += c; \
c -= b;  c ^= rot(b, 8);  b += a; \
a -= c;  a ^= rot(c,16);  c += b; \
b -= a;  b ^= rot(a,19);  a += c; \
c -= b;  c ^= rot(b, 4);  b += a; \
}

/*
 -------------------------------------------------------------------------------
 final -- final mixing of 3 32-bit values (a,b,c) into c
 
 Pairs of (a,b,c) values differing in only a few bits will usually
 produce values of c that look totally different.  This was tested for
 * pairs that differed by one bit, by two bits, in any combination
 of top bits of (a,b,c), or in any combination of bottom bits of
 (a,b,c).
 * "differ" is defined as +, -, ^, or ~^.  For + and -, I transformed
 the output delta to a Gray code (a^(a>>1)) so a string of 1's (as
 is commonly produced by subtraction) look like a single 1-bit
 difference.
 * the base values were pseudorandom, all zero but one bit set, or 
 all zero plus a counter that starts at zero.
 
 These constants passed:
 14 11 25 16 4 14 24
 12 14 25 16 4 14 24
 and these came close:
 4  8 15 26 3 22 24
 10  8 15 26 3 22 24
 11  8 15 26 3 22 24
 -------------------------------------------------------------------------------
 */
#define final(a,b,c) \
{ \
c ^= b; c -= rot(b,14); \
a ^= c; a -= rot(c,11); \
b ^= a; b -= rot(a,25); \
c ^= b; c -= rot(b,16); \
a ^= c; a -= rot(c,4);  \
b ^= a; b -= rot(a,14); \
c ^= b; c -= rot(b,24); \
}

/*
 --------------------------------------------------------------------
 This works on all machines.  To be useful, it requires
 -- that the key be an array of uint32_t's, and
 -- that the length be the number of uint32_t's in the key
 
 The function hashword() is identical to hashlittle() on little-endian
 machines, and identical to hashbig() on big-endian machines,
 except that the length has to be measured in uint32_ts rather than in
 bytes.  hashlittle() is more complicated than hashword() only because
 hashlittle() has to dance around fitting the key bytes into registers.
 --------------------------------------------------------------------
 */
unsigned int Framework::hashString(const char *string)
{
	uint32_t a,b,c;
	size_t length=strlen(string);
	/* Set up the internal state */
	a = b = c = 0xdeadbeef + (((uint32_t)length)<<2);
	
	/*------------------------------------------------- handle most of the key */
	while (length > 3)
	{
		a += string[0];
		b += string[1];
		c += string[2];
		mix(a,b,c);
		length -= 3;
		string += 3;
	}
	
	/*------------------------------------------- handle the last 3 uint32_t's */
	switch(length)                     /* all the case statements fall through */
	{ 
		case 3 : c+=string[2];
		case 2 : b+=string[1];
		case 1 : a+=string[0];
			final(a,b,c);
		case 0:     /* case 0: nothing left to add */
			break;
	}
	/*------------------------------------------------------ report the result */
	return c;
}

/*
 Other string functions by Ben Menke
 */

void concat(const char* string1, const char* string2, char* concat, int maxChars)
{
	int n=0; 
	for(int i=0; string1[i]!='\0'&&n<maxChars; i++, n++)
	{
		concat[i]=string1[i];
	}
	
	for(int i=0; string2[i]!='\0'&&n<maxChars; i++, n++)
	{
		concat[i]=string2[i];
	}
}


