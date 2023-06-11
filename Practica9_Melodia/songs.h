#include <inttypes.h>
#include <stdio.h>

#define c_note      261
#define d_note      294
#define e_note      329
#define f_note      349
#define g_note      391
#define gS_note     415
#define a_note      440
#define aS_note     466
#define b_note      494
#define aa_note     493
#define cH_note     523
#define cSH_note    554
#define dH_note     587
#define dSH_note    622
#define eH_note     659
#define fH_note     698
#define fSH_note    740
#define gH_note     784
#define gSH_note    830
#define aH_note     880
#define hH_note     987

#define bL_note     247
#define fS_note     370
#define cS_note     277
#define aL_note     220

#define gL_note     196
#define dS_note     311
#define gSL_note    208
#define cL_note     131
#define eL_note     165
#define aSL_note    233

#define TEMPO	    500
#define TEMPO_500   500
#define TEMPO_250   250
#define TEMPO_600   600


struct note ImperialMarch[] =
{
    //for the sheet music see:
    //http://www.musicnotes.com/sheetmusic/mtd.asp?ppn=MN0016254
    //this is just a translation of said sheet music to frequencies / time in ms
    //used TEMPO ms for a quart note
    {a_note, TEMPO},
    {a_note, TEMPO},
    {a_note, TEMPO},
    {f_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},

    {a_note, TEMPO},
    {f_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},
    {a_note, TEMPO*2},
    //first bit

    {eH_note, TEMPO},
    {eH_note, TEMPO},
    {eH_note, TEMPO},
    {fH_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},

    {gS_note, TEMPO},
    {f_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},
    {a_note, TEMPO*2},
    //second bit...

    {aH_note, TEMPO},
    {a_note, TEMPO*3/4},
    {a_note, TEMPO*1/4},
    {aH_note, TEMPO},
    {gSH_note, TEMPO/2},
    {gH_note, TEMPO/2},

    {fSH_note, TEMPO*1/4},
    {fH_note, TEMPO*1/4},
    {fSH_note, TEMPO/2},
    {0,TEMPO/2},
    {aS_note, TEMPO/2},
    {dSH_note, TEMPO},
    {dH_note, TEMPO/2},
    {cSH_note, TEMPO/2},
    //start of the interesting bit

    {cH_note, TEMPO*1/4},
    {b_note, TEMPO*1/4},
    {cH_note, TEMPO/2},
    {0,TEMPO/2},
    {f_note, TEMPO*1/4},
    {gS_note, TEMPO},
    {f_note, TEMPO*3/4},
    {a_note, TEMPO*1/4},

    {cH_note, TEMPO},
    {a_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},
    {eH_note, TEMPO*2},
    //more interesting stuff (this doesn't quite get it right somehow)

    {aH_note, TEMPO},
    {a_note, TEMPO*3/4},
    {a_note, TEMPO*1/4},
    {aH_note, TEMPO},
    {gSH_note, TEMPO/2},
    {gH_note, TEMPO/2},

    {fSH_note, TEMPO*1/4},
    {fH_note, TEMPO*1/4},
    {fSH_note, TEMPO/2},
    {0,TEMPO/2},
    {aS_note, TEMPO/2},
    {dSH_note, TEMPO},
    {dH_note, TEMPO/2},
    {cSH_note, TEMPO/2},
    //repeat... repeat

    {cH_note, TEMPO*1/4},
    {b_note, TEMPO*1/4},
    {cH_note, TEMPO/2},
    {0,TEMPO/2},
    {f_note, TEMPO/2},
    {gS_note, TEMPO},
    {f_note, TEMPO*3/4},
    {cH_note, TEMPO*1/4},

    {a_note, TEMPO},
    {f_note, TEMPO*3/4},
    {c_note, TEMPO*1/4},
    {a_note, TEMPO*2}
    //and we're done
};


//Author:Jose Manuel Gallegos Lopez 
struct note AngelesAzules[]={
	{bL_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{b_note, TEMPO_500*5/4},

	{a_note, TEMPO_500*3/8},
	{gS_note, TEMPO_500*3/4},
	{fS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/4},
	{bL_note, TEMPO_500*3/2},
	//first bit

	{bL_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{b_note, TEMPO_500*5/4},

	{a_note, TEMPO_500*3/8},
	{gS_note, TEMPO_500*3/4},
	{fS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/4},
	{bL_note, TEMPO_500*3/2},
	//first bit

	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{cS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*5/8},
	{aL_note, TEMPO_500*5/4},
	
	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{cS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*5/8},
	{aL_note, TEMPO_500*5/8},

//****************

	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{e_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*5/8},
	{cS_note, TEMPO_500*5/4},

	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{e_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*5/8},
	{cS_note, TEMPO_500*5/8},

//*****************

	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{e_note, TEMPO_500*1/2},
	{d_note, TEMPO_500*1/2},
	{cS_note, TEMPO_500*3},

	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{e_note, TEMPO_500*1/2},
	{d_note, TEMPO_500*1/2},
	{cS_note, TEMPO_500*3},

//Se repite todo*****

	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{cS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*5/8},
	{aL_note, TEMPO_500*5/4},
	
	{d_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*3/8},
	{cS_note, TEMPO_500*3/8},
	{d_note, TEMPO_500*5/8},
	{aL_note, TEMPO_500*5/8},

//****************

	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{e_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*5/8},
	{cS_note, TEMPO_500*5/4},

	{fS_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*3/8},
	{e_note, TEMPO_500*3/8},
	{fS_note, TEMPO_500*5/8},
	{cS_note, TEMPO_500*5/8},

//*****************

	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{e_note, TEMPO_500*1/2},
	{d_note, TEMPO_500*1/2},
	{cS_note, TEMPO_500*3},

	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{g_note, TEMPO_500*1/2},
	{e_note, TEMPO_500*1/2},
	{d_note, TEMPO_500*1/2},
	{cS_note, TEMPO_500*3}
	//and we're done
};



//Canciones
struct note *arraySongs[2][100] = {{ImperialMarch}, {AngelesAzules}};
uint16_t arrayLengthSongs[] = {sizeof(ImperialMarch) / sizeof(struct note), sizeof(AngelesAzules) / sizeof(struct note)};
