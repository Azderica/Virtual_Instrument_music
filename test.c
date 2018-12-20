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
void set_screen_size();			// set screen size
void exception_handler();		// exception handler

/* drawing */
void drawing_start_img();		// drawing starting img

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
	int i = 0;
	int pid;
	int response = 1;
	char *name;	// = "wav/";
	char line[5] = ".wav";
	char c[2];
	char input[BUFSIZ];

	//playSound( argv[1] );
	/*************************************************
	 *                   Preference                  *
	 *************************************************/
	tty_mode(0);
	set_cr_noecho_mode();
	signal(SIGINT, exception_handler);
	signal(SIGQUIT, exception_handler);
	

	/*************************************************
	 *                   Draw Map                    *
	 *************************************************/
	initscr();
	clear();
	drawing_start_img();

	/*************************************************
	 *                     Start                     *
	 *************************************************/
	if(getchar()){
		clear();
		refresh();
		move(0,0);
	}

	/*************************************************
	 *                  Select Menu                  *
	 *************************************************/


        /*************************************************
         *                     Loop                      *
         *************************************************/
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

void set_screen_size(){
	
}

void exception_handler(){
	endwin();
	tty_mode(1);
	exit(1);
}

void drawing_start_img(){
	int i, j, x, y;
	char str[2][8][100];
	
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
			x = 8*i+j;
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
}
