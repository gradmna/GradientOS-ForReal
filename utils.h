#ifndef __UTILS
#define __UTILS
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "gradientmalloc.h"
#define STR2(x) #x
#define STR(x) STR2(x)
#define h2c(h) 48+(39*((h)>9))+(h)
#define debug(s) __debug__((c*)__func__, __FILE__ ":" STR(__LINE__), s) // debugging shit
#define LEN1 250
#define LEN2 30
typedef char c;
typedef struct WindowCollectionItem {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	c*title;
	struct WindowCollectionItem*next;
} WindowCollectionItem;
extern uint32_t activeWin;
extern unsigned char treeCredit[];
extern unsigned int treeCredit_len;
bool compareStr(c*str1, c*str2);
c*concatStr(c*into, c*what);
void*memcpy(void*dest, void*src, size_t n);
void*memset(void*dest, int byte, size_t n);
bool sStarts(c*longer, c*shorter);
int parseInt(c*str, size_t limit);
c*i2s(int in, c*s);
c*d2s(double in, c*s, size_t limit);

// imports
__attribute__((import_module("env"), import_name("setMainLoop"))) void setMainLoop(void(*)(int)); // a copy of emscripten's emscripten_set_main_loop bcuz it sounds cool
__attribute__((import_module("env"), import_name("setSizeof"))) void setSizeof(c*,size_t);
__attribute__((import_module("env"), import_name("alert"))) void alert(c*); // for serious user-facing shit
__attribute__((import_module("env"), import_name("initCtx"))) bool initCtx(c*);
__attribute__((import_module("env"), import_name("callCtxFun"))) int callCtxFun(c*,bool); // if i have such a high-level api for canvas shit, why don't i just use *that*?
__attribute__((import_module("env"), import_name("ctxFun2Str"))) char*ctxFun2Str(c*);
__attribute__((import_module("env"), import_name("disownAllH"))) void disownAllH__DANGEROUS(void);
__attribute__((import_module("env"), import_name("disownHandle"))) void disownHandle(int);
__attribute__((import_module("env"), import_name("handles"))) int handleLen(void);
__attribute__((import_module("env"), import_name("netOpen"))) uint32_t netOpen(c*, uint16_t);
__attribute__((import_module("env"), import_name("netSend"))) bool netSend(uint32_t,c*); // if you give netSend a blank string (or NULL), it will act like an "is open" function
__attribute__((import_module("env"), import_name("netRecv"))) c*netRecv(uint32_t); // this functions uses calloc, so we have to free this func's output when we're done.
__attribute__((import_module("env"), import_name("netClose"))) void netClose(uint32_t);
__attribute__((import_module("env"), import_name("sendTime"))) void sendTime(c*,c*,c*);
__attribute__((import_module("env"), import_name("perfTime"))) double perfTime(void);
__attribute__((import_module("env"), import_name("cnvProp"))) int obtainProp(int idx);
__attribute__((import_module("env"), import_name("debug"))) void __debug__(c*, c*, c*); // debug() macro uses ts
__attribute__((import_module("env"), import_name("renderMiscTxt"))) void renderMiscTxt(unsigned char[]); // type of |treeCredit|.
// imports from mbedtls wrapper
__attribute__((import_module("tls"), import_name("tlsOpen"))) uint32_t tlsOpen(c*, uint16_t);
#endif
