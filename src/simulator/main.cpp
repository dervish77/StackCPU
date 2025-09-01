/*****************************************************************************
 *
 *  StackSim main program.
 *
 *  Description:
 *      StackSim - StackCPU simulator
 *
 *	File:	main.cpp
 *
 *	Author: Brian Lingard
 *	Date:	08/17/2025
 *
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "uiSim.h"

#include "debug.h"


/*
 *  STATICS
 */
uiSim UI;


/*
 *  FUNCTIONS
 */

/*  main	- main program
 *
 *  Parameters:
 *	argc	- number of command line arguments
 *	argv	- pointer to command line argument array
 *
 *  Returns:
 *	none
 */
int main(int argc, char **argv)
{
	char filename[BUFFER_LEN];
	char debugname[BUFFER_LEN];
	int mode = MODE_HALT;
	int debug_enable = 0;
    int play = 1;
    int i;

	// setup default filename
	strcpy(filename, "file.bin");
	strcpy(debugname, "file.log");

    // if no command line arguments
    switch (argc)
    {
        case 1:
            play = 1;
            break;
        default:
            i = 1;
            while (i < argc)
            {
                if (argv[i][0] == '-')
                {
                    switch(argv[i][1])
                    {
                        case 'h':
                            PrintUsage();
                            play = 0;
                            break;
						case 'v':
                            PrintBanner();
                            play = 0;
                            break;
                        case 'm':
							mode = atoi(argv[i+1]);
							i++;
                            break;
                        case 'f':
							strcpy(filename, argv[i+1]);
							i++;
                            break;
                        case 'd':
							strcpy(debugname, argv[i+1]);
							debug_enable = 1;
							i++;
                            break;
                        default:
                            fprintf(stderr, "Unknown command line argument\n");
                            exit(0);
                    }	/* end of inner switch */
                }
                i++;	/* increment arg index */
            }
            break;
    }

    if (play)
    {
		PrintBanner();
		
		UI.RunCLI(filename, mode);
    }

    exit(0);
}

void PrintBanner()
{
    printf("******************************************************\n");
    printf("             Welcome to StackSim %s\n", VERSION);
    printf("******************************************************\n");
}

void PrintUsage()
{
    printf("Usage:  stacksim [options]\n");
    printf("\n");
	printf("options:\n");
    printf("   -d <file>    output debug to log file (default is off)\n");
    printf("   -f <file>    memory file to load\n");
    printf("   -m <mode>    start up mode\n");
    printf("   -h           display help\n");
    printf("   -v           display version\n");
    printf("\n");
}

/* end of main.c */
