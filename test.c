#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<strings.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/ioctl.h>
#include	<termios.h>
#include	<pthread.h>
#include	<termios.h>
#include	<curses.h>

typedef struct record{
	char *name;
	long int interval_utime;
}record; 

/* set */
void set_cr_noecho_mode(void);		// no echo mode
void tty_mode(int);			// tty mode
void exception_handler();		// exception handler

/* drawing */
void drawing_start_img();		// drawing starting img
int drawing_select_img();		// drawing select img
void drawing_play_piano();		// drawing play piano
void drawing_record_piano();		// drawing record piano
void drawing_listen_piano();		// drawing listen piano

/* check */
int check_valid_input(char input);	// check valid key value

/* sheet */
void select_sheet(int i);		// select sheet
void cleer_sheet();			// clear sheet

/* clock */
void mclock_init();			// set initial time for record
long int uclock();			// return elapsed time after mclock_init

/* global variable */
int num = 0;				// control maximum input
char sheet[6][15][130];			// sheet papaer
record rec[1000];			// rec
struct timeval tp;
struct timeval __mclock_start;

int playSound( char *filename ) {	// play sound function
	char command[256];
	int status;
	num+=1;
	sprintf( command, "aplay -N -c 1 -q -t wav %s", filename );

	status = system( command );
	num-=1;
	return status;
}

int main() {
	pthread_t t;
	int i = 0, j, k, response = 1;
	char *name;	// = "wav/";
	char line[5] = ".wav", c[2];

	char *sheet_name, sheet_c[2];
	char txt[5] = ".txt";
	FILE *fp;

	/*************************************************/
	/*                   Preference                  */
	/*************************************************/
	tty_mode(0);
	set_cr_noecho_mode();
	signal(SIGINT, exception_handler);
	signal(SIGQUIT, exception_handler);
	resize_term(30, 130);

	initscr();
	clear();
	
	/*************************************************/
	/*                 set sheet data                */
	/*************************************************/
	for(i=0; i<6; i++)
		for(j=0; j<15; j++)
			for(k=0; k<130; k++)
				sheet[i][j][k] = 0;
	for(i=0; i<6; i++){
		sheet_name = (char*)malloc(sizeof(char)*130);
		strcpy(sheet_name, "Sheet/Sheet");
		sheet_c[0] = '0' + i;
		sheet_c[1] = '\0';
		strcat(sheet_name, sheet_c);
		strcat(sheet_name, txt);
		
		fp = fopen(sheet_name, "r");
		for(j=0; j<15; j++){
			fgets(sheet[i][j], 130, fp);
		}
		fclose(fp);
		free(sheet_name);
	}

	/*************************************************
	 *                   Start Logo                  *
	 *************************************************/
	drawing_start_img();
	clear();
	refresh();

        /*************************************************
         *                     Loop                      *
         *************************************************/
	while(response){

		/*************************************************
		 *                   Select Menu                 *
		 *************************************************/
		response = drawing_select_img();
		clear();
		refresh();
	
		if(response == 1){
			drawing_play_piano();
			i = 0;		// initial	
			while(1){
				select_sheet(i);
				name = (char*)malloc(sizeof(1000));
				c[0] = getchar();
				if(check_valid_input(c[0])){
					c[1] = '\0';
					strcpy(name, "wav/");
					strcat(name, c);
					strcat(name, line);
					if(num<30)
					{
						pthread_create(&t, NULL, playSound,(void *)name);
						usleep(10000);
					}
					free(name);
				}
				if(c[0] == '<' && i > 0)
					i--;
				if(c[0] == '>' && i < 5)
					i++;
				if(c[0] == '[' || c[0] == ']')
					break;
				//fflush(stdout);
				cleer_sheet();
			}
		}
		i=0;
		if(response == 2){
			drawing_record_piano();
			mclock_init();
			while(1){
				name = (char*)malloc(sizeof(1000));
				c[0] = getchar();
				if(check_valid_input(c[0])){
					c[1] = '\0';
					strcpy(name, "wav/");
					strcat(name, c);
					strcat(name, line);
					if(num<30)
					{
						pthread_create(&t, NULL, playSound,(void *)name);
						rec[i].name = name;
						rec[i].interval_utime = uclock();
						printf("%s, %ld\n", rec[i].name, rec[i].interval_utime);
						i++;
						usleep(10000);
					}
					free(name);
				}
				if(c[0] == '[' || c[0] == ']')
					break;
			}
		}

		if(response == 3){
			drawing_listen_piano();

			while(1){
				getchar();
			
			}
		}
		clear();
		refresh();
	}

	endwin();
	tty_mode(1);


	return 0;
}

void set_cr_noecho_mode(void){
	struct termios ttystate;

	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how){
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);
}

void exception_handler(){
	endwin();
	tty_mode(1);
	exit(1);
}

void drawing_start_img(){
	//resize_term(30, 95);
	int i, j, x, y;
	char str[2][8][130];
	char input;
	
	strcpy(str[0][0], "    ___      ___ ___  ________  _________  ___  ___  ________  ___               ");
	strcpy(str[0][1], "   |\\  \\    /  /|\\  \\|\\   __  \\|\\___   ___\\\\  \\|\\  \\|\\   __  \\|\\  \\             ");
	strcpy(str[0][2], "   \\ \\  \\  /  / | \\  \\ \\  \\|\\  \\|___ \\  \\_\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\            ");
	strcpy(str[0][3], "    \\ \\  \\/  / / \\ \\  \\ \\   _  _\\   \\ \\  \\ \\ \\  \\\\\\  \\ \\   __  \\ \\  \\            ");
	strcpy(str[0][4], "     \\ \\    / /   \\ \\  \\ \\  \\\\  \\|   \\ \\  \\ \\ \\  \\\\\\  \\ \\  \\ \\  \\ \\  \\____        ");
	strcpy(str[0][5], "      \\ \\__/ /     \\ \\__\\ \\__\\\\ _\\    \\ \\__\\ \\ \\_______\\ \\__\\ \\__\\ \\_______\\       ");
	strcpy(str[0][6], "       \\|__|/       \\|__|\\|__|\\|__|    \\|__|  \\|_______|\\|__|\\|__|\\|_______|        ");
	strcpy(str[0][7], "    ");
	strcpy(str[1][0], "              ______ ______   ___  ___  ________  ___  ________      ");
	strcpy(str[1][1], "             |\\   _  \\ _   \\|\\  \\|\\  \\|\\   ____\\|\\  \\|\\   ____\\ ");
	strcpy(str[1][2], "             \\ \\  \\\\\\__\\ \\  \\ \\  \\\\\\  \\ \\  \\___|\\ \\  \\ \\  \\___| ");
	strcpy(str[1][3], "              \\ \\  \\|__|\\ \\  \\ \\  \\\\\\  \\ \\_____  \\ \\  \\ \\  \\  ");
	strcpy(str[1][4], "               \\ \\  \\    \\ \\  \\ \\  \\\\\\  \\|____|\\  \\ \\  \\ \\  \\____    ");
	strcpy(str[1][5], "                \\ \\__\\    \\ \\__\\ \\_______\\____\\_\\  \\ \\__\\ \\_______\\");
	strcpy(str[1][6], "                 \\|__|     \\|__|\\|_______|\\_________\\|__|\\|_______| ");
	strcpy(str[1][7], "                                         \\|_________|               ");


	for(i=0; i<2; i++){
		for(j=0; j<8; j++){
			x = 8*i+j+3;
			y = 5;
			move(x, y);
			addstr(str[i][j]);
		}
	}		

	move(++x, y);
	addstr("-----------------------------------------------------------------------------");
	move(++x, y);
	addstr("                                    Start                                    ");
	move(++x, y);
	addstr("-----------------------------------------------------------------------------");
	move(++x, y);
	addstr("                               [Press Any Key]                               ");

	refresh();

	while(1){
		input = getchar();
		if(input>='a' && input<='z')
			break;
	}
	move(0,0);
}

int drawing_select_img(){
	int x, y;
	char input;

	x = 4;
	y = 5;
	move(x, y);
	addstr("-----------------------------------------------------------------------------");
	move(++x, y);
	addstr("                                 Select Menu                                 ");
	move(++x, y);
	addstr("-----------------------------------------------------------------------------");
	move(x+4, y);
	addstr("                               1. Play Piano                                 ");
	move(x+8, y);
	addstr("                               2. Record Piano                               ");
	move(x+12, y);
	addstr("                               3. Listen Piano                               ");
	move(x+16, y);
	addstr("-----------------------------------------------------------------------------");
	move(x+17, y);
	addstr("            Key    -    u : up   d : down   y : select   n : exit            ");
	move(x+18, y);
	addstr("-----------------------------------------------------------------------------");

	x = 10;			// now 10
	y = 33;
	move(x,y);

	refresh();

	while(1){
		input = getchar();
		if(input == 'u' || input == 'U'){
			if(x > 10)
				x -= 4;
		}
		else if(input == 'd' || input == 'D'){
			if(x < 18)
				x += 4;
		}
		else if(input == 'y' || input == 'Y'){
			if(x == 10)
				return 1;
			else if(x == 14)
				return 2;
			else if(x == 18)
				return 3;
		}
		else if(input == 'n' || input == 'N')
			return 0;
		move(x, y);
		refresh();
	}
	return 0;
}

void drawing_play_piano(){
	int i, j, x, y;\
	y = 5;
	x = 18;
	move(x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	addstr("|                                 Play Piano                                |");
	move(++x, y);
	addstr("|                                                                           |");
	move(++x, y);
	addstr("|  |!|@| |$|%|^| |*|(| |Q|W|E| |T|Y| |I|O|P| |S|D| |G|H|J| |L|Z| |C|V|B|    |");
	move(++x, y);
	addstr("|  |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_|    |");
	move(++x, y);
	addstr("| |1|2|3|4|5|6|7|8|9|0|q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m| |");
	move(++x, y);
	addstr("| |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| |");
	move(++x, y);
	addstr("|                                                                           |");
	move(++x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	addstr("  change : '<' or '>'                                     exit : '[' or ']' ");
	move(++x, y);
	refresh();
}

void drawing_record_piano(){
	int i, j, x, y;
	x = 3;
	y = 5;
	
	x = 20;
	move(x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	addstr("|                                Record Piano                               |");
	move(++x, y);
	addstr("|                                                                           |");
	move(++x, y);
	addstr("|  |!|@| |$|%|^| |*|(| |Q|W|E| |T|Y| |I|O|P| |S|D| |G|H|J| |L|Z| |C|V|B|    |");
	move(++x, y);
	addstr("|  |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_|    |");
	move(++x, y);
	addstr("| |1|2|3|4|5|6|7|8|9|0|q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m| |");
	move(++x, y);
	addstr("| |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_| |");
	move(++x, y);
	addstr("|                                                                           |");
	move(++x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	addstr("                                                          exit : '[' or ']' ");
	move(++x, y);
	
	refresh();
}
void drawing_listen_piano(){
	int x, y;
	x = 3;
	y = 5;

	move(++x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	addstr("|                                Listen Piano                               |");
	move(++x, y);
	addstr(" ---------------------------------------------------------------------------");
	move(++x, y);
	
	refresh();
}

int check_valid_input(char input){
	if(input >= '0' && input <= '9')
		return 1;
	else if(input >= 'a' && input <= 'z')
		return 1;
	else if(input >= 'A' && input <= 'Z')
		return 1;
	else if(input == '!' || input == '@' || input == '$' || input == '%' || input == '^' || input == '*' || input == '(')
		return 1;
	else
		return 0;
}

void select_sheet(int i){
	int j, k, x=2, y=5;
	char *sheet_line;
	sheet_line = (char*)malloc(sizeof(char)*130);
	for(j=0; j<15; j++){
		if(sheet[i][j][0] == '\0')
			break;
		move(x++, y);
		for(k=0; k<130; k++);
			sheet_line[k] = ' ';
		strncpy(sheet_line, sheet[i][j], strlen(sheet[i][j])-2);
		sheet_line[99] = '\0';
		addstr(sheet_line);
	}
	free(sheet_line);
	move(0, 0);
	refresh();
}

void cleer_sheet(){
	int x;
	char clear_line[130] = "                                                                                        ";
	for(x=0; x<18; x++){
		move(x, 0);
		addstr(clear_line);
	}
	refresh();
}


void mclock_init(void)
{
	gettimeofday(&__mclock_start,NULL);
}

long int uclock(void)
{
	struct timeval timecurrent;
	struct timeval timeresult;
	gettimeofday(&timecurrent, NULL);
	timeresult.tv_sec = timecurrent.tv_sec - __mclock_start.tv_sec;
	timeresult.tv_usec = timecurrent.tv_usec - __mclock_start.tv_usec;
	
	if(timeresult.tv_usec < 0)
	{
		timeresult.tv_sec--;
		timeresult.tv_usec+= 1000000;
	}
	
	return timeresult.tv_sec * 1000000 + timeresult.tv_usec;
}
