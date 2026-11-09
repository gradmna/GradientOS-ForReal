/*
	once upon a time, a FUNCTION was whispered into the GRAD MAN.
	it was a FUNCTION of ALLOCATION. it was a FUNCTION of RESOURCES CREATION.
	it was a FUNCTION of INNOVATION. it was a FUNCTION of GREATLY MANAGEMENT.
	this is the legend of GRADIENTMALLOC.

	for millenia, there was UNDEFINED BEHAVIOR and TODOS instead of MALLOC,
	harming all who dare use.
	with this, the sky would run black with terror
	and the coding land will crack with fear.
	then, her heart pounding...
	the C-EARTH will draw her final breath.

	only then, shining with hope...
	three VARIABLES appear at WORLDS' edge.
	a BYTE INDEX, an INDEX OFFSET, and an ARGUMENT FROM THE CALLER.

	only they can erase corruption
	to banish UNDEFINED BEHAVIOR and TODOS.
	only then will balance be restored,
	and the WORLD saved from uncertainty.

	today, the FOUNTAIN OF REUSABLE CODE-
	the geyser that gives this coding land form-
	stands tall at the center of the kingdom.

	but recently, another fountain has appeared on the horizon...

	and with it, the balance of INNOVATION and UNDEFINED BEHAVIOR begins to shift...
*/

#include <stddef.h>
#include <stdbool.h>
#include "gradientmalloc.h"
#include "utils.h"
#define ISINVALIDPTR(p) (((uint8_t*)(p)<(uint8_t*)heap)||((uint8_t*)(p)>(uint8_t*)(&__heap_end)))
#define heap (&__heap_base)

typedef struct __attribute((packed)) {
	size_t sz;
	uint16_t taken;
} GMallocHdr;
extern uint8_t __heap_base;
extern uint8_t __heap_end;

size_t taken = 0;

uint32_t roundUp(uint32_t a, uint32_t b) {
	return b*((a+b-1)/b);
}

void*malloc(size_t n) {
	size_t byteIdx=0;
	while (byteIdx<taken) {
		GMallocHdr h;
		memcpy(&h,heap+byteIdx,sizeof(GMallocHdr));
		h.sz = roundUp(h.sz,sizeof(GMallocHdr));
		if ((!(h.taken))&&(n<=h.sz)) break;
		byteIdx += h.sz+sizeof(GMallocHdr);
	}
	size_t actualN = roundUp(n,sizeof(GMallocHdr));
	if ((taken+sizeof(GMallocHdr)+actualN)>((&__heap_end)-heap)) return NULL;
	GMallocHdr new = {0};
	new.taken = false;
	new.sz = n;
	memcpy(heap+byteIdx, &new, sizeof(GMallocHdr));
	taken += actualN+sizeof(GMallocHdr);
	return(heap+byteIdx+sizeof(GMallocHdr));
}

void free(void*ptr) {
	if(ISINVALIDPTR(ptr))return; // i wish i could throw an error here...
	GMallocHdr*h = (GMallocHdr*)ptr-1;
	h->taken = false;
}

void*calloc(size_t a, size_t b) {
	void*out=malloc(a*b);
	if(out==NULL)return(out);
	memset(out, 0, a*b); // just to stay compliant...
	return(out);
}

void*realloc(void*ptr, size_t n) {
	if (ptr==NULL) return malloc(n);
	if (ISINVALIDPTR(ptr)) return NULL;
	bool bad = (n==0);
	GMallocHdr*h = NULL;
	void*moveTo = NULL;
	if (!(bad)) {
		h = (GMallocHdr*)ptr-1;
		void*moveTo=malloc(n);
		if(moveTo==NULL)return(NULL);
		memcpy(moveTo, ptr, (n>h->sz)?h->sz:n);
	}
	free(ptr);
	return(moveTo);
}
