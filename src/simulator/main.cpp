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
	FILE *in_file_p;
	char filename[BUFFER_LEN];
	int mode = MODE_EXIT;
    int play = 1;
    int i;

	// setup default filename
	strcpy(filename, "file.bin");

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
		
		// open memory load file
		if (strlen(filename) != 0)
		{
			in_file_p = fopen(filename, "r");
			if (in_file_p == (FILE *)NULL)
			{
				fprintf(stderr, "Couldn't open %s as input\n", filename);
				exit(0);
			}

			// call UI sim load memory 
			printf("\nLoading %s into memory\n", filename);
			UI.Load(filename, in_file_p);
		}
		
		switch (mode)
		{
			case MODE_EXIT:
			    printf("\nSim is EXITED\n");
				break;
			case MODE_HALT:
			    printf("\nSim is HALTED\n");
				break;
			case MODE_RUN:
			    printf("\nSim is RUNNING\n");
				break;
			case MODE_SSTEP:
			    printf("\nSim is SINGLE STEP\n");
				break;
			default:
				fprintf(stderr, "Unknown mode %d specified\n", mode);
				exit(0);
				break;
		}
		
		// call UI sim start
		if (mode != MODE_EXIT)
		{
			UI.Start(mode);
		}
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
    printf("Usage:  stacksim [-f file] [-m mode] [-h] [-v]\n");
    printf("\n");
    printf("   -h           display help\n");
    printf("   -v           display version\n");
    printf("\n");
    printf("   -f <file>    memory file to load\n");
    printf("   -m <mode>    start up mode\n");
    printf("\n");
}

/* end of main.c */
