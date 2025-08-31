//
// parse.h
//
// definition of parse string utility function
//

#ifndef __parse_h
#define __parse_h

//
// usage:
//
//   int arg_count;
//   char *buffer, *arg_array[MAX_ARGS];
//   buffer = &some_string[0];
//   arg_count = ParseString( buffer, arg_array );
//


// prototypes

int ParseString(char *string, char *args[]);


#endif // __parse_h
