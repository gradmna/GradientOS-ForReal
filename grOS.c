#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "utils.h"
#include "treeCredit.c"
#define SSO(t) setSizeof(#t,sizeof(t))
#define CRASH(s, cd) crashed(((uint16_t)(s)<<12)|(uint16_t)(cd))

uint16_t crashCode=0;
uint32_t gradH; // if wisp only allows uint32_t ids, i'm using that too...
bool timePending = true;
uint32_t time = 0; // fix to uint64_t on 2159/11/21
uint32_t activeWin = 0;
WindowCollectionItem*windows = &((WindowCollectionItem){0,0,0,0,"unused",&(WindowCollectionItem){0,0,100,100,"unused",NULL}});

int daysB4Mo(int y, int m) {
	int res = ((int[]){0,31,59,90,120,151,181,212,243,273,304,334})[m];
	if ((m>2)&&((y%4==0)&&((y%100!=0)||(y%400==0)))) res++;
	return res;
}

int y2d(int y) {
	int count = 2023;
	int res = 0;
	while (count<y) {
		res += daysB4Mo(count,11)+31; // funky way of doing this
		count++;
	}
	return res;
}

void __renderCrash__(int);
void crashed(uint16_t code) {
	if((crashCode!=0)&&(code!=0)){
		alert("crah²...");
		setMainLoop(NULL);
		return;
	}
	if (code != 0) crashCode=code;
	c*strCode = calloc(5,sizeof(c));
	strCode[0] = h2c((crashCode>>12)&15);
	strCode[1] = h2c((crashCode>>8)&15);
	strCode[2] = h2c((crashCode>>4)&15);
	strCode[3] = h2c(crashCode&15);
	debug(strCode);
	free(strCode);
	setMainLoop(__renderCrash__);
}

void callShitTonOfCtxFuns(c**,size_t);
void __renderCrash__(int _) {
	(void)_;
	disownAllH__DANGEROUS();
	callCtxFun("createLinearGradient(0,0,0,cnv.height)",1); // this'll be 0.
	callShitTonOfCtxFuns((c*[]){
		"h[0].addColorStop(0,\"#cc0\")",
		"h[0].addColorStop(1,\"#0c6\")",
		"fillStyle=ctx.h[0]",
		"fillRect(0,0,cnv.width,cnv.height)"
	},4);
}

void callShitTonOfCtxFuns(c**funs,size_t size) {
	size_t count = 0;
	while (count < size) {
		if(callCtxFun(funs[count],0)==-2) {
			crashed(0);
			return;
		}
		count++;
	}
}

/*
	int minAppH = 16;
	int maxAppH = 24;
	int h = obtainProp(1);
	int rows = 3; // least rows
	while (true) {
		int curr = h/rows;
		if ((curr>=minAppH)&&(curr<=maxAppH)) break;
		rows++;
	}
*/
void renderWindows(void) {
	// active window colors: #ccc & #eee
	// inactive window colors: #888 & #9f9f9f
	c str[LEN1] = {0};
	c str2[LEN2] = {0};
	c*comma = ",";
	c*aBd = "#ccc";
	c*aHl = "#eee";
	c*iBd = "#888";
	c*iHl = "#9f9f9f";
	uint32_t idx = 0;
	WindowCollectionItem*curr = windows;
	while (curr!=NULL) {
		c*bd = ((idx==0)||(idx==activeWin))?aBd:iBd;
		c*hl = ((idx==0)||(idx==activeWin))?aHl:iHl;
		uint16_t x = curr->x;
		uint16_t y = curr->y;
		uint16_t w = curr->w;
		uint16_t h = curr->h;
		if (idx==0) { // first window is always apps picker, which gets its own xy, dimensions and colors. no questions asked.
			x = y = obtainProp(1)*0.25;
			h = obtainProp(1)*0.5;
			w = obtainProp(0)*0.5-h;
			bd=aBd;
			hl=aHl;
		}
		concatStr(str, "createLinearGradient(");
		concatStr(str, i2s(x, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(y, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(x, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(y+h, str2));
		concatStr(str, ")");
		int gIdx = callCtxFun(str,true);
		memset(str,0,LEN1);
		memset(str2,0,LEN2);
		concatStr(str, "h[");
		concatStr(str, i2s(gIdx, str2));
		concatStr(str,"].addColorStop(0,\"");
		concatStr(str,hl);
		concatStr(str,"\")");
		callCtxFun(str,false);
		memset(str,0,LEN1);
		concatStr(str, "h[");
		concatStr(str, str2);
		concatStr(str,"].addColorStop(1,\"");
		concatStr(str,bd);
		concatStr(str,"\")");
		callCtxFun(str,false);
		memset(str,0,LEN1);
		concatStr(str,"fillStyle=ctx.h[");
		concatStr(str,str2);
		concatStr(str,"]");
		callCtxFun(str,false);
		memset(str,0,LEN1);
		memset(str2,0,LEN2);
		concatStr(str,"roundRect(");
		concatStr(str, i2s(x, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(y, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(w, str2));
		concatStr(str, comma);
		memset(str2,0,LEN2);
		concatStr(str, i2s(h, str2));
		if (idx==0) concatStr(str,",8)");
		if (idx!=0) concatStr(str,",[8,8,0,0])");
		callShitTonOfCtxFuns((c*[]){
			"beginPath()",
			str,
			"fill()"
		},3);
		memset(str,0,LEN1);
		memset(str2,0,LEN2);
		disownHandle(gIdx);
		curr = curr->next;
		idx++;
	}
}

void renderClock(void) {
	int sinceLoad = perfTime()/1000;
	int since2023 = time+sinceLoad;
	c str[LEN1] = {0};
	c str2[LEN2] = {0};
	c*bd = "#ccc";
	c*hl = "#eee"; // inch resting
	int gIdx = callCtxFun("createLinearGradient(cnv.width-150,0,cnv.width-132,0)",true);
	concatStr(str, "h[");
	concatStr(str, i2s(gIdx, str2));
	concatStr(str,"].addColorStop(0,\"");
	concatStr(str,hl);
	concatStr(str,"\"),ctx.h[");
	concatStr(str, str2);
	concatStr(str,"].addColorStop(1,\"");
	concatStr(str,bd);
	concatStr(str,"\"),ctx.fillStyle=ctx.h[");
	concatStr(str, str2);
	concatStr(str, "]");
	// for some reason i decided to add all the color stops and set the phil in one string, guess it's one of the many Reasons Why Grad is Funky and Unique™!
	callShitTonOfCtxFuns((c*[]){
		str,
		"fillRect(cnv.width-150,0,150,150)"
	},2);
	memset(str,0,LEN1);
	memset(str2,0,LEN2);
	disownHandle(gIdx);
	gIdx = callCtxFun("createRadialGradient(cnv.width-75,75,0,cnv.width-75,75,51)",true);
	concatStr(str, "h[");
	concatStr(str, i2s(gIdx, str2));
	concatStr(str,"].addColorStop(1,\"#ddd\"),ctx.h[");
	concatStr(str, str2);
	concatStr(str,"].addColorStop((51-8)/51,\"#999\"),ctx.h[");
	concatStr(str, str2);
	concatStr(str,"].addColorStop((51-8)/51-1e-2,\"#ddd\"),ctx.h[");
	concatStr(str, str2);
	concatStr(str, "].addColorStop((51-16)/51,\"#fff\"),ctx.fillStyle=ctx.h[");
	concatStr(str, str2);
	concatStr(str, "]");
	// well here i also made the frame go around a white body, might add another gradient in the body too
	callShitTonOfCtxFuns((c*[]){
		str,
		"beginPath()",
		"ellipse(cnv.width-75,75,51,51,0,0,Math.PI*2)",
		"fill()",
		"strokeStyle=\"#000\"", // we blackin ts alright
		"strokeWidth=1",
		// (deg*Math.PI)/180
		"h,Array.from({length:24},function(_,idx,rad,baseX,baseY){[rad=(idx*Math.PI)/12,baseX=Math.sin(rad),baseY=0-Math.cos(rad),ctx.beginPath(),ctx.moveTo(cnv.width-75+(baseX*41),75+(baseY*41)),ctx.lineTo(cnv.width-75+(baseX*(37-(idx%2==0)*4)),75+(baseY*(37-(idx%2==0)*4))),ctx.stroke()]})"
	},7);
	memset(str,0,LEN1);
	memset(str2,0,LEN2);
	disownHandle(gIdx);
	int sinceMdnt = since2023%86400;
	double hr = (sinceMdnt/3600.0);
	double m = (hr-(int)hr)*60;
	double s = (m-(int)m)*60;
	concatStr(str,"h,(function(hr,m,s,radH,radM,radS,baseHX,baseMX,baseSX,baseHY,baseMY,baseSY){[radH=(hr%12)*(Math.PI/6),radM=(m*Math.PI)/30,radS=(s*Math.PI)/30,baseHX=Math.sin(radH),baseMX=Math.sin(radM),baseSX=Math.sin(radS),baseHY=0-Math.cos(radH),baseMY=0-Math.cos(radM),baseSY=0-Math.cos(radS),ctx.beginPath(),ctx.moveTo(cnv.width-75+(baseMX*28),75+(baseMY*28)),ctx.lineTo(cnv.width-75,75),ctx.lineTo(cnv.width-75+(baseHX*16),75+(baseHY*16)),ctx.stroke(),ctx.strokeStyle=\"#f00\",ctx.beginPath(),ctx.moveTo(cnv.width-75,75),ctx.lineTo(cnv.width-75+(baseSX*32),75+(baseSY*32)),ctx.stroke()]})(");
	concatStr(str,d2s(hr, str2, LEN2));
	concatStr(str,",");
	memset(str2,0,LEN2);
	concatStr(str,d2s(m, str2, LEN2));
	concatStr(str,",");
	memset(str2,0,LEN2);
	concatStr(str,d2s(s, str2, LEN2));
	concatStr(str,")");
	callCtxFun(str,false);
}

__attribute__((used)) void render(int frame) { // dont make the ref undefined
	c str[LEN1] = {0};
	if(crashCode!=0) {
		crashed(0);
		return;
	}
	c*m=netRecv(gradH);
	if ((m==NULL)&&(timePending)) return;
	if (m!=NULL) {
		if (timePending) { // GradientOS epoch: 2023/10/16
			timePending = false;
			uint8_t*date = (uint8_t*)m;
			while (!(sStarts((c*)date,"Date: "))) date++;
			date += 6;
			int year = parseInt((c*)date+12,4);
			int mo = 0;
			c**months = (c*[]){"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
			while (!(sStarts((c*)date+8,months[mo]))) mo++;
			int days = daysB4Mo(year,mo)+y2d(year)+parseInt((c*)date+5,2)-1-288; // 288 - days since 2023 jan until 2023 october 16th
			// date[17] && date[18] => hours
			// date[20] && date[21] => mins
			// date[23] && date[24] => seconds
			int hours = parseInt((c*)date+17,2);
			int minutes = parseInt((c*)date+20,2);
			time = parseInt((c*)date+23,2);
			time += ((((days*24)+hours)*60)+minutes)*60;
			concatStr(str, "time: ");
			i2s((unsigned int)time, str+6);
			debug(str);
			memset(str,0,LEN1);
		}
		if (netSend(gradH,NULL)) netClose(gradH);
		free(m);
	}
	callCtxFun("clearRect(0,0,cnv.width,cnv.height)",false); // clear
	renderMiscTxt(treeCredit);
	renderWindows();
	renderClock();
}

void __main__(void) {
	#include "sizeofs.c" // generated in |build|
	sendTime(__DATE__, __TIME__, __OFFSET__);
	debug("Welcome to GradientOS!");
	gradH = netOpen("gradientos.neocities.org",80); // peak self advertisement
	netSend(gradH, "GET / HTTP/1.1\r\nHost: gradientos.neocities.org\r\nConnection: close\r\n\r\n");
	if (!(initCtx("2d"))) {
		alert("initialising context failed!");
		return;
	}
	setMainLoop(render);
}
