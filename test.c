#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/ioctl.h>
#include	<termios.h>
#include	<pthread.h>
#include	<termios.h>
#include	<curses.h>

/* set */
void set_cr_noecho_mode(void);		// no echo mode
void tty_mode(int);			// tty mode
void exception_handler();		// exception handler

/* drawing */
void drawing_start_img();		// drawing starting img
int drawing_select_img();		// drawing select img
void drawing_play_piano();		// drawing play piano
void drawing_record_piano();		// drawing record piano
void drawing_listen_paino();		// drawing listen piano

int num = 0;

int playSound( char *filename ) {	// play sound function
	char command[256];
	int status;
	num+=1;
	sprintf( command, "aplay -N -c 1 -q -t wav %s", filename );

	status = system( command );
	printf("num = %d\n", num);
	num-=1;
	return status;
}

int main() {
	pthread_t t;
	int i = 0, response = 0;
	char *name;	// = "wav/";
	char line[5] = ".wav", c[2];

	//playSound( argv[1] );
	/*************************************************
	 *                   Preference                  *
	 *************************************************/
	tty_mode(0);
	set_cr_noecho_mode();
	signal(SIGINT, exception_handler);
	signal(SIGQUIT, exception_handler);
	initscr();
	clear();

	/*************************************************
	 *                   Start Logo                  *
	 *************************************************/
	drawing_start_img();
	clear();
	refresh();

	/*************************************************
	 *                   Select Menu                 *
	 *************************************************/
	response = drawing_select_img();
	clear();
	refresh();

        /*************************************************
         *                     Loop                      *
         *************************************************/
	//if(response == 1)
	drawing_play_piano();

	while(1){
		name = (char*)malloc(sizeof(1000));
		c[0] = getchar();
			
		c[1] = '\0';
		strcpy(name, "wav/");

		strcat(name, c);
		strcat(name, line);
		if(num<30)
		{
			pthread_create(&t, NULL, playSound,(void *)name);
			usleep(10000);
		}
		else
		{
			
		}
		
		free(name);
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
	resize_term(25, 95);
	int i, j, x, y;
	char str[2][8][100];
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
	resize_term(25, 95);
	int i, j, x, y;
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
	addstr("            Key       - u : up   d : down   y : select   n : exit            ");
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
	int i, j, x, y;
	x = 3;
	y = 5;
	move(++x, y);
	addstr("---------------------------------------------------------------------------");
	x++;
	move(++x, y);
	addstr("  |!|@| |$|%|^| |*|(| |Q|W|E| |T|Y| |I|O|P| |S|D| |G|H|J| |L|Z| |C|V|B| ");
	move(++x, y);
	addstr("  |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| |_|_| |_|_|_| ");
	move(++x, y);
	addstr(" |1|2|3|4|5|6|7|8|9|0|q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m|");
	move(++x, y);
	addstr(" |_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|");
	move(++x, y);
	addstr("---------------------------------------------------------------------------");
	
	refresh();
}
void drawing_record_piano();		// drawing record piano
void drawing_listen_paino();		// drawing listen piano
