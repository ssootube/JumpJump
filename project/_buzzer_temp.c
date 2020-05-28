#include <avr/io.h>
#include <avr/io.h>
//#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_buzzer.h"

#define    LDo        395
#define    LRe        354
#define    LMi        316
#define    LFa        298
#define    LSo        266
#define    LLa        237
#define    LSi        212

#define    BDo        200
#define    BRe        178
#define    BMi        159
#define    BFa        150
#define    BSo        134
#define    BLa        112
#define    BSi        107

#define    HDo        101
#define    HRe        90
#define    HMi        80
#define    HFa        76
#define    HSo        68
#define    HLa        61
#define    HSi        54

#define    hDo        51
#define    RB        50
#define    LB        65/2
#define    SB        30
#define    BB        35

#define     Buzzer_Port     PORTG
#define     Buzzer_Pin      4
#define     MelOn    SETBIT(Buzzer_Port, Buzzer_Pin)
#define     MelOff   CLEARBIT(Buzzer_Port, Buzzer_Pin)

void delay_us_Melody(int data)
{	int i;	for(i=0;i<data;i++)	{		_delay_us(1);	}}

void Sound(unsigned int ch, unsigned int time)
{
	unsigned int tim;
	tim=ch;
	tim=tim*1000/time;
	while(tim--)
	{
		MelOn;
		delay_us_Melody(time*5);
		MelOff;
		delay_us_Melody(time*5);
	}

}

void S_Good()
{	Sound(BB,BDo);
	Sound(BB,BSo);
}
void S_Push1()
{
	Sound(SB,HDo);
	Sound(SB,HMi);
}
void S_Start()
{
	Sound(LB,BDo);	
	Sound(LB,BSo);
	Sound(LB,BFa);
	Sound(LB,BFa);
	Sound(LB,BMi);
	Sound(LB,BMi);
	Sound(LB,BMi);
	Sound(LB,BFa);
	Sound(LB,BSo);
	Sound(LB,BSo);
	Sound(LB,HDo);
}
void S_Over()
{
	Sound(LB,HMi);
	Sound(LB,HDo);
	Sound(LB,BLa);
	Sound(LB,BFa);
	Sound(LB,BRe);
}
