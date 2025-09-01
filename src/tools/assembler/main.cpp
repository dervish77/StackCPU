/*****************************************************************************
 *
 *  StackAsm main program.
 *
 *  Description:
 *      StackAsm - StackCPU assembler
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
	strcpy(filename, "file.asm");

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
    printf("             Welcome to StackAsm %s\n", VERSION);
    printf("******************************************************\n");
}

void PrintUsage()
{
    printf("Usage:  stackasm [options] [source file]\n");
    printf("\n");
    printf("options:\n");
    printf("   -g             add debug info to obj file\n");
    printf("   -D name[=val]  define a symbol\n");
    printf("   -W n           set warning level n\n");
    printf("   -I dir         include dir search path\n");
    printf("   -b <binfile>   output binary exe (bypass obj output)\n");
    printf("   -o <objfile>   output obj file\n");
    printf("   -h             display help\n");
    printf("   -v             verbose mode\n");
    printf("   -V             display version\n");
    printf("\n");
}

/* end of main.cpp */
