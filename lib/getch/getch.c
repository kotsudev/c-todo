#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// Function to capture a single character input without echoing it to the console
char getch()
{
	struct termios oldt, newt;
	char ch;
	tcgetattr(STDIN_FILENO, &oldt); // Get current terminal attributes
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Set new attributes
	ch = getchar(); // Read a character
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old attributes
	return ch;
}
