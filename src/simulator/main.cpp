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
	char cmdstring[BUFFER_LEN];
	
	int mode = MODE_HALT;
	int debug_enable = 0;
	int unit_test = 0;
    int play = 1;
    int i;

	// setup default filename
	strcpy(filename, "file.bin");
	strcpy(debugname, "file.log");
	strcpy(cmdstring, " ");

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
                        case 'u':
                            unit_test = 1;
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
						case 'c':
							strcpy(cmdstring, argv[i+1]);
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
	
	//printf("cmdstring = %s\n", cmdstring);
	
	UNUSED(debug_enable);

    if (play)
    {
		PrintBanner();
		
		UI.RunCLI(filename, mode, unit_test);
    }

    exit(0);
}

void PrintBanner()
{
    printf("******************************************************\n");
    printf("                    StackSim %s\n", VERSION);
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
	printf("   -u           enable unit testing\n");
	printf("   -c <string>  pass command 'string' to CLI\n");
    printf("   -h           display help\n");
    printf("   -v           display version\n");
    printf("\n");
}

/* end of main.c */
