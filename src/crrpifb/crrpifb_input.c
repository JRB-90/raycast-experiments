#include "crrpifb_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>

const static char* EVNT_PATH = "/dev/input/event0";

typedef struct input_event input_event;

int evfd;
input_event events[64];
bool lctrl;
struct termios old;
struct termios new;

int InitInputDevice()
{
	lctrl = false;

	evfd = open(EVNT_PATH, O_RDONLY | O_NONBLOCK);
	if (evfd == -1)
	{
		fprintf(stderr, "Failed to open event fd\n");
		return -1;
	}

	int version;
	if (ioctl(evfd, EVIOCGVERSION, &version))
	{
		fprintf(stderr, "Failed to get input version\n");
		return -1;
	}

	printf(
		"Input driver version: %d.%d.%d\n",
		version >> 16,
		(version >> 8) & 0xff,
		version & 0xff
	);

	unsigned short id[4];
	if (ioctl(evfd, EVIOCGID, id))
	{
		fprintf(stderr, "Failed to get input id\n");
		return -1;
	}

	printf(
		"Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS],
		id[ID_VENDOR],
		id[ID_PRODUCT],
		id[ID_VERSION]
	);

	if (tcgetattr(STDIN_FILENO, &old))
	{
		fprintf(stderr, "Failed to get termios attribute\n");
		return -1;
	}
	
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new))
	{
		fprintf(stderr, "Failed to set termios attribute\n");
		return -1;
	}

	return 0;
}

int DestroyInputDevice()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	tcflush(STDIN_FILENO, TCIOFLUSH);
	close(evfd);

	return 0;
}

int UpdateInputState(InputState* const state)
{
	state->toggleDebug = false;
	state->toggleRenderMode = false;

	int bytesRead = read(evfd, events, sizeof(input_event) * 64);
	if (bytesRead < (int)sizeof(input_event))
	{
		fprintf(stderr, "Failed to read keyboard\n");
		return -1;
	}

	for (int i = 0; i < (bytesRead / sizeof(input_event)); i++)
	{
		if (events[i].type == EV_KEY)
		{
			if (events[i].value == 1)
			{
				switch (events[i].code)
				{
				case KEY_ESC:
					state->quit = true;
					break;
				case KEY_V:
					state->toggleDebug = true;
					break;
				case KEY_B:
					state->toggleRenderMode = true;
					break;
				case KEY_W:
					state->forwards = true;
					break;
				case KEY_S:
					state->backwards = true;
					break;
				case KEY_D:
					if (lctrl)
					{
						state->rotRight = false;
						state->right = true;
					}
					else
					{
						state->rotRight = true;
						state->right = false;
					}
					break;
				case KEY_A:
					if (lctrl)
					{
						state->rotLeft = false;
						state->left = true;
					}
					else
					{
						state->rotLeft = true;
						state->left = false;
					}
					break;
				case KEY_LEFTCTRL:
					lctrl = true;
					break;
				}
			}
			else if (events[i].value == 0)
			{
				switch (events[i].code)
				{
				case KEY_W:
					state->forwards = false;
					break;
				case KEY_S:
					state->backwards = false;
					break;
				case KEY_D:
					state->rotRight = false;
					state->right = false;
					break;
				case KEY_A:
					state->rotLeft = false;
					state->left = false;
					break;
				case KEY_LEFTCTRL:
					lctrl = false;
					break;
				}
			}
		}
	}

	return 0;
}
