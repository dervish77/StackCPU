//
// hex.h
//
// implementation of intel hex utility function
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hex.h"

#include "memSim.h"


#define DEBUG 0


/*  ConvertData	- copy binary data to appended file 
 *
 *  PARAMETERS:
 *	mem     	- input source
 *	out_file_p	- output file pointer
 *  start       - starting address
 *  bytes_rec   - bytes per record
 *
 *  RETURNS:
 *	count		- number of bytes copied
 */
int ConvertData(memSim *mem, FILE *out_file_p, int start, int bytes_rec) 
{
    int i;
	int src, end;
    int count = 0;
    int bytes = 0;
    int address = start;

    unsigned char sum = 0;
    unsigned char c;
    unsigned char checksum = 0;

    // hex format -- :bbaaaatt[data]cc
    // bb - bytes in record
    // aaaa - address
    // tt - record type
    // cc - checksum

    //fseek(in_file_p, 0L, SEEK_SET);
	
	src = MEM_PROG_START;
	end = MEM_DATA_END + 1;

    while (src < end) //(!feof(in_file_p)) 
	{
        // copy data for current record
        for (i = 0; i < bytes_rec; i++) 
		{
            c = (unsigned char) mem->Read( src ); //fgetc(in_file_p);
            if (src < end) //(!feof(in_file_p)) 
			{
                if (i == 0) 
				{
                    if (DEBUG) printf("start rec addr %d\n", address);
                    // start new record
                    fprintf(out_file_p, ":%02X", bytes_rec);
                    fprintf(out_file_p, "%04X", address);
                    fprintf(out_file_p, "%02X", TYPE_DAT);
                }

                fprintf(out_file_p, "%02X", c);
                sum += c;
                address++;
                count++;
				src++;
            }
        }

        if (src <= end) //(!feof(in_file_p)) 
		{
            // end of current record
            if (DEBUG) printf("..end rec count %d sum %02x\n", count, sum);
            checksum = (~sum) + 1; // calculate checksum as 2's complement
            fprintf(out_file_p, "%02X", checksum);
            fprintf(out_file_p, "\n");
            sum = 0;
        }
    }

    // end of file
    fprintf(out_file_p, ":%02X", 0);
    fprintf(out_file_p, "%04X", 0);
    fprintf(out_file_p, "%02X", TYPE_EOF);
    fprintf(out_file_p, "%02X", 255);
    fprintf(out_file_p, "\n");

    return (count);
}

/* end of bin2hex.c */