//
// binUtils.h
//
// implementation of binary utility functions
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "binUtils.h"

#include "memSim.h"


#define DEBUG 0


// static globals used by DumpBlock()
static BOOLEAN_T	suppress_ascii;
static int			base;
static int          count;


/*  _DumpBlock	- read a block of size bytes from input source
 *		and send to output file
 *
 *  PARAMETERS:
 *	mem     	- input source
 *  offset      - offset address in memSim (of next block)
 *  end         - ending address in memSim
 *	size		- size of block in bytes
 *	bytes_per_row	- bytes per row
 *	out_file_p	- output file pointer
 *
 *  RETURNS:
 *	eof		- TRUE if at EOF
 */
BOOLEAN_T _DumpBlock(memSim *mem, int offset, int end, int size, int bytes_per_row, FILE *out_file_p)
{
   int r, b;
   int num_rows;
   int actual_bytes_in_row;  // tracks bytes read, in case less than a full row

   unsigned char row_array[DEFAULT_BYTES_PER_ROW];
   int c;

   int at_eof;

   int address = offset;

   num_rows = size / bytes_per_row;

   fprintf(out_file_p, "\nByte        Data");

   if (suppress_ascii)
      fprintf(out_file_p, "\n");
   else
   {
      for (b = 2; b < bytes_per_row; b++)
      {
         fprintf(out_file_p, "  ");
         if ( ((b+1) % 4) == 0 )
            fprintf(out_file_p, " ");
      }

      fprintf(out_file_p, "    Ascii\n");
   }

   at_eof = FALSE;

   for (r = 0; r < num_rows; r++)
   {
      fprintf(out_file_p, "%08d    ", ((r * bytes_per_row) + base));

      actual_bytes_in_row = 0;

      for (b = 0; b < bytes_per_row; b++)
      {
		 if (address <= end) // prevent reading past end of memory
		 {
			c = mem->Read( address );
		 }
         if (address > end) // are we done reading?
         {
            at_eof = TRUE;  // yes, mark "eof" and bail out of loop
            break;
         }
		 count++;      // increment count of bytes dumped
		 address++;    // increment address to next byte in memory

         row_array[b] = (unsigned char) c;

         fprintf(out_file_p, "%02X", row_array[b]);

         if ( ((b+1) % 4) == 0 )
            fprintf(out_file_p, " ");

         actual_bytes_in_row++;
      }

      if (!suppress_ascii)
      {
         if (actual_bytes_in_row < bytes_per_row)
         {
            for (b = actual_bytes_in_row; b < bytes_per_row; b++)
            {
               fprintf(out_file_p, "  ");
               if ( ((b+1) % 4) == 0 )
                  fprintf(out_file_p, " ");
            }
         }

         fprintf(out_file_p, "    ");

         for (b = 0; b < actual_bytes_in_row; b++)
         {
            if ( isprint( row_array[b] ) )
               fprintf(out_file_p, "%c", row_array[b]);
            else
               fprintf(out_file_p, ".");
         }
      }

      fprintf(out_file_p, "\n");

      if (at_eof)
         return(TRUE);
   }

   base += size;

   return(FALSE);
}

/*  DumpMemory	- dump bytes from input source and send to terminal
 *
 *  PARAMETERS:
 *	mem     	- input source
 *  start       - start address in memSim (of next block)
 *  end         - ending address in memSim
 *  suppress    - suppress ascii output if true
 *
 *  RETURNS:
 *	count		- number of bytes dumped
 */
int DumpMemory(memSim *mem, int start, int end, int suppress)
{
   BOOLEAN_T done = FALSE;
      
   int block_size = DEFAULT_BLOCK_SIZE;
   int bytes_per_row = DEFAULT_BYTES_PER_ROW;

   char response[20]; /* response buffer */

   int offset = start;

   // init static globals
   count = 0;
   base = start; // init to start address of dump request
   suppress_ascii = suppress;

   // dump the memory from start to end address
   while ( !done )
   {
      /* if _DumpBlock returns EOF, set done flag */
      if ( _DumpBlock( mem, offset, end, block_size, bytes_per_row, stdout) )
      {
         done = TRUE;
      }

      /* else, prompt for next block */
      else
      {
		 offset += block_size;
		 
         printf("\nDump next block (Y/n)? ");

         fgets( &response[0], sizeof(response), stdin );

         if ( (response[0] == 'n') || (response[0] == 'N'))
         {
            done = TRUE;
         }
      }
   }
   
   return(count);
}



/*  ConvertDataToHex	- convert binary data to intel hex format
 *
 *  PARAMETERS:
 *	mem     	- input source
 *  start       - starting address in memSim
 *  end         - ending address in memSim
 *	out_file_p	- output file pointer
 *  addr        - starting address for hex file
 *  bytes_rec   - bytes per record
 *
 *  RETURNS:
 *	count		- number of bytes converted
 */
int ConvertDataToHex(memSim *mem, int start, int end, FILE *out_file_p, int addr, int bytes_rec) 
{
    int i;
	int src, limit;
    int count = 0;
    int address = addr;

    unsigned char sum = 0;
    unsigned char c;
    unsigned char checksum = 0;

    // hex format -- :bbaaaatt[data]cc
    // bb - bytes in record
    // aaaa - address
    // tt - record type
    // cc - checksum

    //fseek(in_file_p, 0L, SEEK_SET);
	
	src = start;
	limit = end + 1;

    while (src < limit) //(!feof(in_file_p)) 
	{
        // copy data for current record
        for (i = 0; i < bytes_rec; i++) 
		{
            c = (unsigned char) mem->Read( src ); //fgetc(in_file_p);
            if (src < limit) //(!feof(in_file_p)) 
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

        if (src <= limit) //(!feof(in_file_p)) 
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

    return(count);
}

/* end of binUtils.cpp */
