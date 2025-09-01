/*****************************************************************************
 *
 *  StackLd main program.
 *
 *  Description:
 *      StackLd - StackCPU linker
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




/*
 *  STATICS
 */


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
	int mode = 0;
    int play = 1;
    int i;

	// setup default filename
	strcpy(filename, "file.obj");

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
		
		//UI.RunCLI(filename, mode);
    }

    exit(0);
}

void PrintBanner()
{
    printf("******************************************************\n");
    printf("             Welcome to StackLd %s\n", VERSION);
    printf("******************************************************\n");
}

void PrintUsage()
{
    printf("Usage:  stackld [options] [lib files] [obj files]\n");
    printf("\n");
    printf("options:\n");
    printf("   -C <cfgfile>   linker config file\n");
    printf("   -L dir         lib file search path\n");
    printf("   -o <binfile>   output exe bin file\n");
    printf("   -m <mapfile>   output map file\n");
    printf("   -x <hexfile>   output hex file\n");
    printf("   -h             display help\n");
    printf("   -v             verbose mode\n");
    printf("   -V             display version\n");
    printf("\n");
}

/* end of main.cpp */
