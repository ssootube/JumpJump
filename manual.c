//발판 객체
struct object{
	short _x,_y; //발판의 좌표
	unsigned char type; //발판의 타입
	};

enum {NORMAL,JUMP,WIDE,BREAKLESS,TRAP,WIDE_BREAKLESS,BLACKHOLE,L_SLIDE,R_SLIDE,ROCKET,MOP,
//발판의 타입은 위와 같은 종류이며, 아래는 좌/우로 움직임이 추가된 발판이다.
M_NORMAL,M_JUMP,M_WIDE,M_BREAKLESS,M_TRAP,M_WIDE_BREAKLESS,M_BLACKHOLE,M_L_SLIDE,M_R_SLIDE,M_ROCKET,M_MOP,
	last};//last는 그저 개수를 세기 위해서 패딩으로 들어간 항목이다.
#define NTYPE last //발판 타입의 총 개수를 정의했다.



//발판 정보 함수
unsigned char get_jump_power(unsigned char TYPE); //TYPE 발판의 탄성력을 리턴한다.
unsigned char get_ground_width(unsigned char TYPE);//TYPE 발판의 너비를 리턴한다.
#define Nground 10 //한 화면에 표시될 수 있는 발판의 총 개수
struct object ground[Nground]; //발판 객체 변수를 글로벌에 선언해두었다.
void draw_ground(void); //바로 윗 줄에서 선언한 글로벌 발판 객체를 이용해 발판을 그린다. 
void ground_regenerate(int i);//i번째 발판을 재 생성한다. 재생성되는 이유는 화면을 벗어났거나, 깨졌거나, 총알로 인해 공격당한 경우 등이 있을 수 있다. 필요한 경우에 가져가 사용하면 된다. 단, 이때 재 생성되는 발판의 종류는 ‘map_num’변수에 의해 제한된다. 이는 특정 맵에서 특정 발판만 생성되게 하기 위한 것이다.



//조작 함수
int player_x, player_y; //플레이어의 좌표
int boost=0;//부스트 게이지 값
int gun_x, gun_y;//날아가는 총알의 좌표
#define speed 2 //스피드 값
float dy =0;//가속도
void game_input(void);//가속도 센서가 좌우로 움직이면, player_x좌표를 speed값 만큼 더하거나 빼준다. 또한 ‘조이스틱 하’를 입력하면 부스트 게이지 값을 줄이면서 캐릭터의 가속도를 증가시켜준다. 물론 게이지 값이 0이면 더 이상 사용할 수 없다. 또한 ‘조이스틱 상’을 입력하면 총알을 발사하도록 한다.



//메뉴 함수
void draw_menu(short selected,char** str);//str의 텍스트를 한 줄씩 화면에 표시하며, selected번째줄 가장 왼쪽에, 선택되었다는 의미로 *을 표시한다.
enum _page{Default,Main,Main_Map,Main_Start,Main_Map_Move}; //페이지 번호
enum _page select_menu(enum _page page, short selected);//page의 selected번째 항목이 선택되었을 경우, 표시되어야 할 페이지의 번호를 리턴한다.
void showMenu();//위 두 함수를 이용해 메뉴를 표시한다.



//상호작용 함수
void screen_down();//캐릭터가 계속 올라가면서, 화면을 아래로 땡겨주는 함수이다.
void action(int i);// ground[i].TYPE에 해당하는 발판과 캐릭터가 부딪힌 경우 수행해야할 액션을 수행한다. 결국 해당 발판의 탄성력만큼 플레이어의 가속도를 증가시키거나, 함정을 밟았을 경우 게임오버시키거나, 너무 강하게 낙하한 경우 발판이 깨지거나 몬스터의 경우에는 닿으면 죽는 등의 액션을 수행한다.
void hit_test(void);//발판의 너비값을 이용해서 플레이어와 부딪힌 경우 위의 액션 함수를 실행해준다.



//초기화 함수
void game_init(void); //게임 변수들을 초기화한다.
void init_devices(void);
void Port_init(void);



//그리기 함수
void draw_player(void); //캐릭터를 그리고, 가속도에 따라 효과선도 그린다.
void draw_boost_bar();//부스트 게이지바를 화면에 그린다.



//소리 알고리즘
if(dy<-2)Sound(3,abs(dy)*20);
if(dy>2)Sound(3,1000/abs(dy));
//플레이어의 가속도에 따라 음낮이를 달리하여 소리가 나도록 조절하여 구현하였다.
//소리의 값은 몇번의 시행착오 끝에 게임과 자연스럽게 어울리도록 곱하고 나누며 상수를 조절하였다.
//결국 캐릭터가 빠르게 올라갈 때는 음도 빠르게 올라가는 듯한 느낌이 들게 만든다.

