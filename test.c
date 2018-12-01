
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<termios.h>

void set_cr_noecho_mode(void);
void tty_mode(int);

int playSound( char *filename ) {
	char command[256];
	int status;

	sprintf( command, "aplay -c 1 -q -t wav %s", filename );

	status = system( command );

	return status;
}

int main() {
	int pid;
	int response = 1;
	char *name;	// = "wav/";
	char line[5] = ".wav";
	char c[2];
	char input[BUFSIZ];

	//playSound( argv[1] );

	tty_mode(0);
	set_cr_noecho_mode();
	while(1){
		name = (char*)malloc(sizeof(1000));
		c[0] = getchar();
		c[1] = '\0';

		strcpy(name, "wav/");

		strcat(name, c);
		strcat(name, line);
		playSound(name);

		free(name);
	}
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

