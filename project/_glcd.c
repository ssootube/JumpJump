#include "_main.h"
#include "_glcd.h"
#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>


typedef  unsigned char byte;
typedef  unsigned int  word;

#define  DISPON   0x3f
#define  DISPOFF  0x3e
word   d;

byte   xchar, ychar;

//폰트는 세로 출력을 위해 뒤집어 놓았다.
byte font[95][7] = {{0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x04,0x04,0x04,0x04,0x00,0x00,0x04},
{0x0a,0x0a,0x0a,0x00,0x00,0x00,0x00},
{0x0a,0x0a,0x1f,0x0a,0x1f,0x0a,0x0a},
{0x04,0x0f,0x14,0x0e,0x05,0x1e,0x04},
{0x18,0x19,0x02,0x04,0x08,0x13,0x03},
{0x0c,0x12,0x14,0x08,0x15,0x12,0x0d},
{0x0c,0x04,0x08,0x00,0x00,0x00,0x00},
{0x02,0x04,0x08,0x08,0x08,0x04,0x02},
{0x08,0x04,0x02,0x02,0x02,0x04,0x08},
{0x00,0x04,0x15,0x0e,0x15,0x04,0x00},
{0x00,0x04,0x04,0x1f,0x04,0x04,0x00},
{0x00,0x00,0x00,0x00,0x0c,0x04,0x08},
{0x00,0x00,0x00,0x1f,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x0c,0x0c},
{0x00,0x01,0x02,0x04,0x08,0x10,0x00},
{0x0e,0x11,0x13,0x15,0x19,0x11,0x0e},
{0x04,0x0c,0x04,0x04,0x04,0x04,0x0e},
{0x0e,0x11,0x01,0x02,0x04,0x08,0x1f},
{0x1f,0x02,0x04,0x02,0x01,0x11,0x0e},
{0x02,0x06,0x0a,0x12,0x1f,0x02,0x02},
{0x1f,0x10,0x1e,0x01,0x01,0x11,0x0e},
{0x06,0x08,0x10,0x1e,0x11,0x11,0x0e},
{0x1f,0x01,0x02,0x04,0x08,0x08,0x08},
{0x0e,0x11,0x11,0x0e,0x11,0x11,0x0e},
{0x0e,0x11,0x11,0x0f,0x01,0x02,0x0c},
{0x00,0x0c,0x0c,0x00,0x0c,0x0c,0x00},
{0x00,0x0c,0x0c,0x00,0x0c,0x04,0x08},
{0x02,0x04,0x08,0x10,0x08,0x04,0x02},
{0x00,0x00,0x1f,0x00,0x1f,0x00,0x00},
{0x08,0x04,0x02,0x01,0x02,0x04,0x08},
{0x0e,0x11,0x01,0x02,0x04,0x00,0x04},
{0x0e,0x11,0x01,0x0d,0x15,0x15,0x0e},
{0x0e,0x11,0x11,0x11,0x1f,0x11,0x11},
{0x1e,0x11,0x11,0x1e,0x11,0x11,0x1e},
{0x0e,0x11,0x10,0x10,0x10,0x11,0x0e},
{0x1c,0x12,0x11,0x11,0x11,0x12,0x1c},
{0x1f,0x10,0x10,0x1e,0x10,0x10,0x1f},
{0x1f,0x10,0x10,0x1e,0x10,0x10,0x10},
{0x0e,0x11,0x10,0x17,0x11,0x11,0x0f},
{0x11,0x11,0x11,0x1f,0x11,0x11,0x11},
{0x0e,0x04,0x04,0x04,0x04,0x04,0x0e},
{0x07,0x02,0x02,0x02,0x02,0x12,0x0c},
{0x11,0x12,0x14,0x18,0x14,0x12,0x11},
{0x10,0x10,0x10,0x10,0x10,0x10,0x1f},
{0x11,0x1b,0x15,0x15,0x11,0x11,0x11},
{0x11,0x11,0x19,0x15,0x13,0x11,0x11},
{0x0e,0x11,0x11,0x11,0x11,0x11,0x0e},
{0x1e,0x11,0x11,0x1e,0x10,0x10,0x10},
{0x0e,0x11,0x11,0x11,0x15,0x12,0x0d},
{0x1e,0x11,0x11,0x1e,0x14,0x12,0x11},
{0x0e,0x11,0x10,0x0e,0x01,0x11,0x0e},
{0x1f,0x04,0x04,0x04,0x04,0x04,0x04},
{0x11,0x11,0x11,0x11,0x11,0x11,0x0e},
{0x11,0x11,0x11,0x11,0x11,0x0a,0x04},
{0x11,0x11,0x11,0x15,0x15,0x15,0x0a},
{0x11,0x11,0x0a,0x04,0x0a,0x11,0x11},
{0x11,0x11,0x11,0x0a,0x04,0x04,0x04},
{0x1f,0x01,0x02,0x04,0x08,0x10,0x1f},
{0x0e,0x08,0x08,0x08,0x08,0x08,0x0e},
{0x00,0x10,0x08,0x04,0x02,0x01,0x00},
{0x0e,0x02,0x02,0x02,0x02,0x02,0x0e},
{0x04,0x0a,0x11,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x1f},
{0x08,0x04,0x02,0x00,0x00,0x00,0x00},
{0x00,0x00,0x0e,0x01,0x0f,0x11,0x0f},
{0x10,0x10,0x16,0x19,0x11,0x11,0x1e},
{0x00,0x00,0x0e,0x10,0x10,0x11,0x0e},
{0x01,0x01,0x0d,0x13,0x11,0x11,0x0f},
{0x00,0x00,0x0e,0x11,0x1f,0x10,0x0e},
{0x06,0x09,0x08,0x1c,0x08,0x08,0x08},
{0x00,0x0f,0x11,0x11,0x0f,0x01,0x0e},
{0x10,0x10,0x16,0x19,0x11,0x11,0x11},
{0x04,0x00,0x0c,0x04,0x04,0x04,0x04},
{0x02,0x00,0x06,0x02,0x02,0x12,0x0c},
{0x10,0x10,0x12,0x14,0x18,0x14,0x12},
{0x0c,0x04,0x04,0x04,0x04,0x04,0x0e},
{0x00,0x00,0x1b,0x15,0x15,0x11,0x11},
{0x00,0x00,0x16,0x19,0x11,0x11,0x11},
{0x00,0x00,0x0e,0x11,0x11,0x11,0x0e},
{0x00,0x00,0x1e,0x11,0x1e,0x10,0x10},
{0x00,0x00,0x0d,0x13,0x0f,0x01,0x01},
{0x00,0x00,0x16,0x19,0x10,0x10,0x10},
{0x00,0x00,0x0e,0x10,0x0e,0x01,0x1e},
{0x08,0x08,0x1c,0x08,0x08,0x09,0x06},
{0x00,0x00,0x11,0x11,0x11,0x13,0x0d},
{0x00,0x00,0x11,0x11,0x11,0x0a,0x04},
{0x00,0x00,0x11,0x11,0x15,0x15,0x0a},
{0x00,0x00,0x11,0x0a,0x04,0x0a,0x11},
{0x00,0x00,0x11,0x11,0x0f,0x01,0x0e},
{0x00,0x00,0x1f,0x02,0x04,0x08,0x1f},
{0x02,0x04,0x04,0x08,0x04,0x04,0x02},
{0x04,0x04,0x04,0x00,0x04,0x04,0x04},
{0x08,0x04,0x04,0x02,0x04,0x04,0x08},
{0x00,0x00,0x08,0x15,0x02,0x00,0x00}};

	void cmndl( byte cmd )	
	{
		SetBit(PORTE,PORTE5);	
		_delay_us(10);
		ClrBit(PORTE,PORTE4);
		ClrBit(PORTE,PORTE6);	
		SetBit(PORTE,PORTE7);	
		PORTA = cmd ;		
		_delay_us(10);
		ClrBit(PORTE,PORTE5);	
		
	}
	
	void cmndr( byte cmd )          
	{
		SetBit(PORTE,PORTE5);  
		_delay_us(10);		
		ClrBit(PORTE,PORTE4);  		
		SetBit(PORTE,PORTE6);	
		ClrBit(PORTE,PORTE7);		
		PORTA = cmd ; 		
		_delay_us(10);
		ClrBit(PORTE,PORTE5);			
	}
	
	void cmnda( byte cmd )         
	{
		SetBit(PORTE,PORTE5);  
		_delay_us(10);		
		ClrBit(PORTE,PORTE4); 
		SetBit(PORTE,PORTE6);
		SetBit(PORTE,PORTE7);		
		PORTA = cmd ; 		
		_delay_us(10);
		ClrBit(PORTE,PORTE5);
	}
	
	/* 1 문자 출력  */	
	void datal( byte dat )         
	{
		SetBit(PORTE,PORTE5); 
		_delay_us(10);		
		SetBit(PORTE,PORTE4);  
		ClrBit(PORTE,PORTE6);
		SetBit(PORTE,PORTE7);		
		PORTA = dat ;
		_delay_us(10);
		ClrBit(PORTE,PORTE5);
	}
	
	void datar( byte dat )     
	{
		SetBit(PORTE,PORTE5);
		_delay_us(10);		
		SetBit(PORTE,PORTE4);
		SetBit(PORTE,PORTE6);
		ClrBit(PORTE,PORTE7);
		PORTA = dat ;		
		_delay_us(10);
		ClrBit(PORTE,PORTE5);
	}
	
	void dataa( byte dat )       
	{
		SetBit(PORTE,PORTE5); 
		_delay_us(10);		
		SetBit(PORTE,PORTE4); 
		SetBit(PORTE,PORTE6);
		SetBit(PORTE,PORTE7);
		PORTA = dat ; 		
		_delay_us(10);
		ClrBit(PORTE,PORTE5);			
	}
	/* GLCD Clear */	
	void lcd_clear(void)               /* 그래픽 LCD의 화면을 모두 지우는 함수 */
	{
		byte i, j, x, y;
		x = 0xB8;                         /* X start address */
		y = 0x40;                         /* Y start address */
		for(i = 0; i <= 7; i++)
		{
			cmnda(x);    cmnda(y);
			for(j = 0; j <= 63; j++)
			dataa(0x00);                   /* clear CS1 and CS2 */
			x++;
		}
	}	
	/* GLCD Initialize */	
	void lcd_init(void)
	{
		cmnda( DISPON );
		cmnda( 0xc0 );
		cmnda( 0xb8 );
		cmnda( 0x40 );
	}
	
	
	/* 문자 위치 세트 */
	void  lcd_xy( byte x, byte y )
	{
		xchar = 7-x;
		ychar = y;
		cmnda(0xB8+xchar);                      /* X address */
		if(ychar <= 9)                          /* if y <= 9, CS1 Y address */
		cmndl(0x40+ychar*7+1);
		else                                    /* if y >= 10, CS2 Y address */
		cmndr(0x40+(ychar-9)*7);
	}

	/* 한 문자 출력 */
	void  lcd_char(byte character)
	{
		byte i;
		for(i = 0; i < 7; i++)
		{
			if(ychar < 9)                  /* if y < 9, CS1 */
			datal(font[character-0x20][i]);
			else                            /* if y >= 10, CS2 */
			datar(font[character-0x20][i]);
		}
		if(ychar <9)
		datal(0x00);                    /* last byte 0x00 */
		else
		datar(0x00);                    /* last byte 0x00 */
	}


	/* 문자열 출력 */
	void  lcd_string(byte x,byte y,char *string)
	{
		xchar = x;
		ychar = y;
		
		while(*string != '\0')
		{
			lcd_xy(x++,y);
			if(ychar ==9)                  /* change from CS1 to CS2 */
			cmndr(0x40);
		
			lcd_char(*string);               /* display a charcater */
			string++;                        /* 다음 문자 */
		}
	}

	void GLCD_Axis_xy(unsigned char x, unsigned char y)
	{
		cmnda( 0xB8 + x); // X address
		if(y <= 63)
		{
			cmndl( 0x40 + y); // CS1 Y address
		}
		else
		{
			cmndr( 0x40 + y - 64); // CS2 Y address
		}
	}

	unsigned char ScreenBuffer[8][128];           // screen buffer
	// draw a dot on GLCD
	// 점을 그립니다.
	void GLCD_Dot(unsigned char xx,unsigned char y)
	{
		unsigned char x, i;

		
		// 해상도 범위(128.64) 인지 검사합니다.
		if((xx > 63) || (y > 127)) return;
		x = xx / 8;                 // calculate x address
		i = xx % 8;
		if(i == 0)      { i = 0x01; }
		else if(i == 1) { i = 0x02; }
		else if(i == 2) { i = 0x04; }
		else if(i == 3) { i = 0x08; }
		else if(i == 4) { i = 0x10; }
		else if(i == 5) { i = 0x20; }
		else if(i == 6) { i = 0x40; }
		else            { i = 0x80; }
		
		ScreenBuffer[x][y] |= i;    // OR old data with new data
		GLCD_Axis_xy(x, y);          // draw dot on GLCD screen
		if(y <= 63) { datal(ScreenBuffer[x][y]);  }
		else        { datar(ScreenBuffer[x][y]);  }
	}

	void ScreenBuffer_clear(void)
	{
		unsigned char i, j;
		
		for(i=0; i<8; i++)
		{
			for(j=0; j < 128; j++)
			{
				ScreenBuffer[i][j] = 0x00;
			}
		}

	}

	void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		int x, y;
		if(y1 != y2)
		{
			if(y1 < y2)  
			{
				for(y = y1; y <= y2; y++)
				{
					x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
					GLCD_Dot(x,y);
				}
			}
			else
			{
				for(y = y1; y >= y2; y--)
				{
					x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
					GLCD_Dot(x,y);
				}
			}
		}
		else if(x1 != x2)
		{
			if(x1 < x2)  
			{
				for(x = x1; x <= x2; x++)
				{ y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
					GLCD_Dot(x,y);
				}
			}
			else
			{
				for(x = x1; x >= x2; x--)
				{
					y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
					GLCD_Dot(x,y);
				}
			}
		}
		else                
		{
			GLCD_Dot(x1,y1); 
		}
	}


	// draw a rectangle
	// 직사각형을 그립니다.
	void GLCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		GLCD_Line(x1,y1,x1,y2);    // horizontal line
		GLCD_Line(x2,y1,x2,y2);
		GLCD_Line(x1,y1,x2,y1);    // vertical line
		GLCD_Line(x1,y2,x2,y2);
	}

	void GLCD_Rectangle_black(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2) {
		int x, y;
		for(x = x1; x <= x2; x++) {
			for(y = y1; y <= y2; y++) {
				GLCD_Dot(x,y);
			}
		}
	}

	// draw a circle
	// 원을 그립니다.
	void GLCD_Circle(unsigned char x1,unsigned char y1,unsigned char r)
	{
		int x, y;
		float s;
		for(y = y1 - r*3/4; y <= y1 + r*3/4; y++) // draw with y variable
		{
			s = sqrt(r*r - (y-y1)*(y-y1)) + 0.5;
			x = x1 + (unsigned char)s;
			GLCD_Dot(x,y);
			x = x1 - (unsigned char)s;
			GLCD_Dot(x,y);
		}
		for(x = x1 - r*3/4; x <= x1 + r*3/4; x++) // draw with x variable
		{
			s = sqrt(r*r - (x-x1)*(x-x1)) + 0.5;
			y = y1 + (unsigned char)s;
			GLCD_Dot(x,y);
			y = y1 - (unsigned char)s;
			GLCD_Dot(x,y);
		}
	}

	void GLCD_Circle_black(unsigned char x1,unsigned char y1,unsigned char r) {
		int x, y;
		float s;
		for(y = y1 - r*3/4; y <= y1 + r*3/4; y++) { // draw with y variable
			s = sqrt(r*r - (y-y1)*(y-y1)) + 0.5;
			for(x = x1- (unsigned char)s; x <= x1 + (unsigned char)s; x++) {
				GLCD_Dot(x,y);
			}
		}
		for(x = x1 - r*3/4; x <= x1 + r*3/4; x++) { // draw with x variable
			s = sqrt(r*r - (x-x1)*(x-x1)) + 0.5;
			for(y = y1 - (unsigned char)s; y <= y1 + (unsigned char)s; y++) {
				GLCD_Dot(x,y);
			}
		}
	}

	// display 1-digit decimal number
	unsigned char GLCD_1DigitDecimal(unsigned char number, unsigned char flag)
	{
		number %= 10;               // 10^0
		
		if ((number == 0) && (flag == 0))
		{
			lcd_char(' ');
			return 0;
		}
		
		lcd_char(number + '0');
		return 1;
	}

	// display 2-digit decimal number
	void GLCD_2DigitDecimal(unsigned char number)
	{
		unsigned int i;
		unsigned char flag;
		
		flag = 0;
		number = number % 100;
		i = number/10;
		flag = GLCD_1DigitDecimal(i, flag); // 10^1
		i = number % 10;
		lcd_char(i + '0');              // 10^0
	}

	// display 3-digit decimal number
	void GLCD_3DigitDecimal(unsigned int number)
	{
		unsigned int i;
		unsigned char flag;
		flag = 0;
		number = number % 1000;
		i = number/100;
		flag = GLCD_1DigitDecimal(i, flag); // 10^2
		number = number % 100;
		i = number/10;
		flag = GLCD_1DigitDecimal(i, flag); // 10^1
		i = number % 10;
		lcd_char(i + '0');              // 10^0
	}

	// display 4-digit decimal number
	void GLCD_4DigitDecimal(unsigned int number,byte x, byte y)
	{
		unsigned int i;
		unsigned char flag;
		flag = 0;
		number = number % 10000;
		i = number/1000;
		lcd_xy(x,y);
		flag = GLCD_1DigitDecimal(i, flag);
		number = number % 1000;
		i = number/100;
		lcd_xy(x+1,y);
		flag = GLCD_1DigitDecimal(i, flag);
		number = number % 100;
		i = number/10;
		lcd_xy(x+2,y);
		flag = GLCD_1DigitDecimal(i, flag); 
		i = number % 10;
		lcd_xy(x+3,y);
		lcd_char(i + '0');           
	}

	/*-------------------------------------------------------------------------*/
