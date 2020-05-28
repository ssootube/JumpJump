#include <stdlib.h>
#define player_hight 2
#define player_width 2
#define Nground 10
#define jump_power 15
#define speed 2
#define speedlimit 20
#define boostMAX 20
#define MAXLINE 8

enum {NORMAL,JUMP,WIDE,BREAKLESS,TRAP,WIDE_BREAKLESS,BLACKHOLE,L_SLIDE,R_SLIDE,ROCKET,MOP,
	M_NORMAL,M_JUMP,M_WIDE,M_BREAKLESS,M_TRAP,M_WIDE_BREAKLESS,M_BLACKHOLE,M_L_SLIDE,M_R_SLIDE,M_ROCKET,M_MOP,
	last};
#define NTYPE last
#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "_main.h"
#include "_glcd.h"
#include "_adc.h"
#include "_buzzer.h"
struct object{
	short _x,_y;
	unsigned char type;
	};
struct object ground[Nground];
int player_x, player_y=10, boost=0, clock=0 , ground_move_tick=0, gun_x, gun_y;
float dy =0;//속도
unsigned int Data_ADC3 =0; //조이스틱 상하 데이터 값 저장용
unsigned int Data_ADC4 =0; //조이스틱 좌우 데이터 값 저장용
int score = 0;//점수
char gameover = 0, gun_shot = 0;
short map_num = 0;
short is_moving = 0;

unsigned char get_jump_power(unsigned char TYPE){
switch(TYPE){	
	case WIDE:	
	case M_WIDE:
	case M_NORMAL:	
	case M_TRAP:
	case TRAP:
	case MOP:
	case M_MOP:		
	case NORMAL:	return 7;
	case M_JUMP:
	case JUMP:		return 14;
	case M_BREAKLESS:
	case M_WIDE_BREAKLESS:
	case WIDE_BREAKLESS:
	case BREAKLESS: return 2*dy+3;
	case M_BLACKHOLE:
	case BLACKHOLE: 
	dy= -0.5;
	return 0;
	case M_L_SLIDE:
	case L_SLIDE:
	player_x += 10;
	return 7;
	case M_R_SLIDE:
	case R_SLIDE:
	player_x -= 10;
	return 7;
	case M_ROCKET:
	case ROCKET: return 2*dy+21;

}
return 0;
}

unsigned char get_ground_width(unsigned char TYPE){
switch(TYPE){
	case M_NORMAL:
	case NORMAL:
	case M_JUMP:
	case TRAP:
	case M_TRAP:
	case M_BREAKLESS:
	case BREAKLESS: 
	case BLACKHOLE:
	case M_BLACKHOLE:
	case M_L_SLIDE:
	case M_R_SLIDE:
	case M_ROCKET:
	case L_SLIDE:
	case R_SLIDE:
	case ROCKET:
	case MOP:
	case M_MOP:
	case JUMP:		return 7;
	case M_WIDE_BREAKLESS:
	case WIDE_BREAKLESS:
	case M_WIDE:
	case WIDE:		return 17;
	

}
return 0;
}

void draw_ground(void){
	if(ground_move_tick > 640) ground_move_tick = - 640;
	ground_move_tick++;
	
	int width  = 0;
	int temp;
	for(int i = 0 ; i <Nground; ++i){
		width =get_ground_width(ground[i].type);
		switch (ground[i].type){
			
			case M_NORMAL: ground[i]._x = (abs(ground_move_tick/10)+i*7)%(64-width)+width;
			case NORMAL: GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+2);
				break;
			case M_JUMP:ground[i]._x = (abs(ground_move_tick/10)+i*11)%(64-width)+width;
			case JUMP: 	GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+4);
				break;
			case M_WIDE:ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case WIDE: GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+2);
				break;
			case M_WIDE_BREAKLESS:
			case M_BREAKLESS:ground[i]._x = (abs(ground_move_tick/10)+i*5)%(64-width)+width;
			case WIDE_BREAKLESS:
			case BREAKLESS: GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+1);
				break;
			case M_TRAP: ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case TRAP: width = get_ground_width((ground[i].type)); GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+2);
				for(int j = 1; j < width ; j+=2){ GLCD_Dot(ground[i]._x-j,ground[i]._y-1); GLCD_Dot(ground[i]._x-j,ground[i]._y-2);	}
				break;
			case M_BLACKHOLE: ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case BLACKHOLE: width = get_ground_width((ground[i].type)); 
				GLCD_Line(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y);
				for(int j = 1; j < width ; j+=3){
				temp = (j+abs(ground_move_tick/5))%width;				
				 GLCD_Dot(ground[i]._x-temp,ground[i]._y+1); GLCD_Dot(ground[i]._x-temp-1,ground[i]._y+1);	
				 }
				break;
			case M_R_SLIDE:ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case R_SLIDE:GLCD_Line(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y);
			GLCD_Dot(ground[i]._x-((ground_move_tick)%width),ground[i]._y+1);	
			GLCD_Dot(ground[i]._x-1-((ground_move_tick)%width),ground[i]._y+2);	
			GLCD_Dot(ground[i]._x-((ground_move_tick)%width),ground[i]._y+3);	
			break;
			case M_L_SLIDE:ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case L_SLIDE:GLCD_Line(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y);
			GLCD_Dot(ground[i]._x-width+((ground_move_tick)%width),ground[i]._y+1);
			GLCD_Dot(ground[i]._x-width+1+((ground_move_tick)%width),ground[i]._y+2);
			GLCD_Dot(ground[i]._x-width+((ground_move_tick)%width),ground[i]._y+3);
			break;
			case M_ROCKET:ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case ROCKET:GLCD_Rectangle(ground[i]._x,ground[i]._y,ground[i]._x-width,ground[i]._y+4);
			GLCD_Line(ground[i]._x,ground[i]._y-(ground_move_tick%4)+4,ground[i]._x-width,ground[i]._y-(ground_move_tick%4)+4);
			break;
			case M_MOP: ground[i]._x = (abs(ground_move_tick/10)+i*3)%(64-width)+width;
			case MOP:
			GLCD_Circle_black(ground[i]._x,ground[i]._y,2);	GLCD_Circle_black(ground[i]._x+2,ground[i]._y,2);GLCD_Circle_black(ground[i]._x+4,ground[i]._y,2);
			break;
	
		}
	}
}
void game_input(void)
{
	Data_ADC3 = Read_Adc_Data(3)/14;
	Data_ADC4 = Read_Adc_Data(6);
	if(Data_ADC4 > 444) Data_ADC4 = 444;
	else if(Data_ADC4 < 268) Data_ADC4 = 268;
	//상하: 메뉴이동, 좌우: 메뉴선택
	if(Data_ADC3 < 20)//up
	{
		if(gun_shot == 0 ){
			Sound(15,30);
			gun_shot = 1;
			gun_x = player_x;
			gun_y = player_y;
		}		
	}
	else if(Data_ADC3 > 50)//down
	{
	if(boost > 0){
			boost--;
			dy -= 3;
	}	
		
	}
	else if(Data_ADC4 > 424)//right double
	{
	player_x += 2*speed;
	
	}
	else if(Data_ADC4 > 384)//right 
	{
		player_x += speed;
		
	}
	else if(Data_ADC4 < 288)//left double
	{
		player_x -= 2*speed;
		
		}
	else if(Data_ADC4 < 348)//left double
		{
			player_x -= speed;
				
		}
	if(player_x<0) player_x = 0;
	else if(player_x > 64) player_x = 64;
}

void draw_menu(short selected,char** str){
		ScreenBuffer_clear();
		lcd_clear();
		lcd_string(0,0,"<Menu>");
		lcd_string(0,selected+1,"*");
		for(int i = 1 ; str[i-1] != NULL  || i == MAXLINE ; ++i){
			lcd_string(1,i,str[i-1]);
		}
}
enum _page{Default,Main,Main_Map,Main_Start,Main_Map_Move};
enum _page select_menu(enum _page page, short selected){
	switch(page){
		case Main:
		switch(selected){
			case 0:
			return Main_Map;
			case 1:
			return Main_Start;
			default:
			return page;						
		}
		case Main_Map:
			switch(selected){
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				map_num = selected;
				return Main_Map_Move;
				default:
				return page;
			}
		case Main_Start:
		case Main_Map_Move:
			switch(selected){
				case 0:
				is_moving = 0;
				return Main;
				case 1:
				is_moving = 1;
				return Main;
				default:
				return page;
			}
		case Default:
			return page;		
		break;
	}
	return Default;
}
void game_init(void);
void showMenu(){
	short selected = 0;
	enum _page page = Main;
	char* _main[] = {">Map",">Start",NULL};
	char* map[] = {">first",">basic",">normal",">rocket",">mixed",NULL};
	char* move[] = {">Yes",">No",NULL};
	while(1){
		Data_ADC3 = Read_Adc_Data(3)/14;
		Data_ADC4 = Read_Adc_Data(4)/8;
		
		switch(page){
			case Main:			
			draw_menu(selected,_main);			
			break;
			case Main_Map:
			draw_menu(selected,map);
			break;
			case Main_Start:
			game_init();		
			return;
			case Main_Map_Move:
			ScreenBuffer_clear();
			lcd_clear();
			lcd_string(0,4,"Do you");
			lcd_string(0,5,"want to");
			lcd_string(0,6,"turn on");
			lcd_string(0,7,"moving?");
			lcd_string(0,0,"<Menu>");
			lcd_string(0,selected+1,"*");
			for(int i = 1 ; move[i-1] != NULL  || i == MAXLINE ; ++i){
				lcd_string(1,i,move[i-1]);
			}			
			break;
			case Default:
			return;
		}			
	
		if(Data_ADC3 < 20){//up
			selected = (selected-1)%MAXLINE;
			S_Good();
		}
		else if(Data_ADC3 > 50){//down
			selected = (MAXLINE+selected+1)%MAXLINE;
			S_Good();
		}
	   else if(Data_ADC4 > 90)//right
	   {
		    page = select_menu(page,selected);
			S_Push1();		
	   }

	   	_delay_ms(300);		
	}
}
void Port_init(void){
	PORTA = 0x00; DDRA = 0xff;
	PORTB = 0xfe; DDRB = 0b11111110;
	PORTC=0x00;DDRC=0xf0;
	PORTD=0x80;DDRD=0b10000000;
	PORTE=0x00;DDRE=0xff;
	PORTF=0x00;DDRF=0x00;
}
void init_devices(void){
	cli();
	Port_init();
	lcd_init();
	Adc_init();
	sei();
}
void ground_regenerate(int i){
	unsigned short temp_random;
	switch(map_num){
		case 0: //first
		temp_random = rand()%(2-is_moving);
		if(temp_random == 0) ground[i].type = WIDE_BREAKLESS;
		else if(temp_random == 1) ground[i].type = M_WIDE_BREAKLESS;
		break;
		case 1: // basic
		temp_random = rand()%(2*(2-is_moving));
		if(temp_random == 0) ground[i].type = WIDE_BREAKLESS;
		else if(temp_random == 1) ground[i].type = BREAKLESS;
		else if(temp_random == 2) ground[i].type = M_WIDE_BREAKLESS;
		else if(temp_random == 3) ground[i].type = M_BREAKLESS;
		break;
		case 2: //normal
		temp_random = rand()%(4*(2-is_moving));
		if(temp_random == 0) ground[i].type = BREAKLESS;
		else if(temp_random == 1) ground[i].type = WIDE_BREAKLESS;
		else if(temp_random == 2) ground[i].type = NORMAL;
		else if(temp_random == 3) ground[i].type = WIDE;
		else if(temp_random == 4) ground[i].type = M_BREAKLESS;
		else if(temp_random == 5) ground[i].type = M_WIDE_BREAKLESS;
		else if(temp_random == 6) ground[i].type = M_NORMAL;
		else if(temp_random == 7) ground[i].type = M_WIDE;		
		break;
		case 3://rocket
		temp_random = rand()%(2*(2-is_moving));
		if(temp_random == 0) ground[i].type = ROCKET;
		else if(temp_random == 1) ground[i].type = JUMP;
		else if(temp_random == 2) ground[i].type = M_ROCKET;
		else if(temp_random == 3) ground[i].type = M_JUMP;		
		break;
		case 4://mixed
		ground[i].type = rand()%((NTYPE/2)*(2-is_moving));
		break;
		
	}
	ground[i]._y = rand()%3;
	ground[i]._x = rand()%57+8;
	if(ground[i]._x-get_ground_width(ground[i].type)<0) ground[i]._x += get_ground_width(ground[i].type);
}
void action(int i){
	if(dy >0){
	if(ground[i].type == TRAP||ground[i].type == M_TRAP) gameover = 1;
	dy -= get_jump_power(ground[i].type);	
	if(dy > -0.5){//블록 깨짐
		GLCD_Dot(ground[i]._x,ground[i]._y); GLCD_Dot(ground[i]._x+3,ground[i]._y); ;  GLCD_Dot(ground[i]._x+5,ground[i]._y);
		ground_regenerate(i);
	}
	}
	else if(dy <= 0){
		if(ground[i].type == MOP || ground[i].type == M_MOP )  gameover = 1;
	}
}

void hit_test(void){
	for(int i = 0; i < Nground ; ++i){
		if((player_x-player_width < ground[i]._x)&&(player_x+player_width > ground[i]._x-get_ground_width(ground[i].type))
		&&(player_y+player_hight > ground[i]._y)&&(player_y+player_hight < ground[i]._y+7 )){
			action(i);
		}
		if(ground[i].type == MOP || ground[i].type == M_MOP ){
			if(gun_shot == 1 && (gun_x-ground[i]._x)*(gun_x-ground[i]._x)+(gun_y-ground[i]._y)*(gun_y-ground[i]._y) < get_ground_width(i)*get_ground_width(i)){
				 ground_regenerate(i);
				 	Sound(15,150);
			}
		}
		}
}
void screen_down(){
		if(player_y < player_hight+35){
			player_y = player_hight+35;
			score++;
			for(int i = 0 ; i <Nground ; ++i){
				ground[i]._y -= dy;
				if(ground[i]._y > 127) ground_regenerate(i);
				
			}
			if(gun_shot == 1) gun_y -= dy;
		}
		else if(player_y > 127)	gameover = 1; //게임 오버			
}
void game_init(void){
	srand(Read_Adc_Data(2)+Read_Adc_Data(5)+Read_Adc_Data(6)+Read_Adc_Data(7));//조도 센서 및 가속도 센서를 이용해 랜덤 시드를 준다.
	score = 0;
	player_x=31;
	player_y=10;
	boost=0;
	clock=0;
	ground_move_tick=0;
	dy =0;
	gameover = 0;
	for(int i = 0 ; i<Nground;++i){
		ground[i].type = WIDE_BREAKLESS;
		ground[i]._y = rand()%124+1;
		ground[i]._x = rand()%57+8;
		if(ground[i]._x-get_ground_width(ground[i].type)<0) ground[i]._x += get_ground_width(ground[i].type);
	}
}
void draw_player(void){
	GLCD_Circle(player_x,player_y,player_hight);
	if(gun_shot == 1){ GLCD_Dot(gun_x,gun_y);GLCD_Dot(gun_x,gun_y-1); GLCD_Dot(gun_x-1,gun_y+1);  GLCD_Dot(gun_x+1,gun_y+1);}
	if(dy < -15){
	GLCD_Line(player_x+2,player_y+player_hight,player_x+2,player_y+player_hight+4);
	GLCD_Line(player_x-2,player_y+player_hight,player_x-2,player_y+player_hight+4);
	 GLCD_Line(player_x,player_y+player_hight,player_x,player_y+player_hight+6);
	}
	else if(dy < -10){
		GLCD_Line(player_x+2,player_y+player_hight,player_x+2,player_y+player_hight+2);
		GLCD_Line(player_x-2,player_y+player_hight,player_x-2,player_y+player_hight+2);
		GLCD_Line(player_x,player_y+player_hight,player_x,player_y+player_hight+4);
	}
	else if(dy < -8){
		GLCD_Line(player_x+2,player_y+player_hight,player_x+2,player_y+player_hight+1);
		GLCD_Line(player_x-2,player_y+player_hight,player_x-2,player_y+player_hight+1);
		GLCD_Line(player_x,player_y+player_hight,player_x,player_y+player_hight+2);
	}
	else if(dy > 6){
			GLCD_Line(player_x+2,player_y-player_hight,player_x+2,player_y-player_hight-4);
			GLCD_Line(player_x-2,player_y-player_hight,player_x-2,player_y-player_hight-4);
			GLCD_Line(player_x,player_y-player_hight,player_x,player_y-player_hight-6);
		}
		else if(dy > 4){
		GLCD_Line(player_x+2,player_y-player_hight,player_x+2,player_y-player_hight-2);
		GLCD_Line(player_x-2,player_y-player_hight,player_x-2,player_y-player_hight-2);
		GLCD_Line(player_x,player_y-player_hight,player_x,player_y-player_hight-4);
		}
		else if(dy > 2){
			GLCD_Line(player_x+2,player_y-player_hight,player_x+2,player_y-player_hight-1);
			GLCD_Line(player_x-2,player_y-player_hight,player_x-2,player_y-player_hight-1);
			GLCD_Line(player_x,player_y-player_hight,player_x,player_y-player_hight-2);
		}
}
void draw_boost_bar(){
	 GLCD_Rectangle(63,124,63-boost*3,127);
}
int main(void)
{
	init_devices();
	lcd_clear();
	game_init();
//게임 인트로
	ScreenBuffer_clear();
	lcd_clear();
	lcd_string(0,0,"JumpJump");
	S_Start();
	_delay_ms(1500);
	showMenu();
	
    while (1) 
    {
		if(gameover == 1){
				ScreenBuffer_clear();
				lcd_clear();
				lcd_string(0,0,"GameOver");
				lcd_string(0,1,"<Score>");
				GLCD_4DigitDecimal(score,0,2);
				lcd_string(0,3,"Push the");
				lcd_string(0,4,"JoyStick");
				lcd_string(0,5,"Up to");
				lcd_string(0,6,"Restart!");
				S_Over();
				while(1){			
				Data_ADC3 = Read_Adc_Data(3)/14;
				if(Data_ADC3 < 20){
				showMenu();							
				break;
				}				
				}
				
		}
		clock++;
		if(clock >10+score/100){
			clock = 0; boost++;
			if(boost >boostMAX) boost = boostMAX;
		}
		ScreenBuffer_clear();
		lcd_clear();
		game_input();		
		gun_y -= 7;
		if(gun_y <= 0) gun_shot = 0;
		GLCD_4DigitDecimal(score,0,0);
		hit_test();
		screen_down();	
		dy += 0.5;
		if(dy>speedlimit) dy = speedlimit;
		if(dy<-2)Sound(3,abs(dy)*20);
		if(dy>2)Sound(3,1000/abs(dy));
		player_y+= dy;
		draw_boost_bar();
		draw_player();
		draw_ground();
		_delay_ms(30);
    }

	}
