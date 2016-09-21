/****************************************************************************

File Name:      reader.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of java pre defined types.

****************************************************************************/  

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define ASCII_NINE '9'
#define COUNT ((int) (sizeof (int) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1
#define SIZE 50     /* max size of decimal array */
#define maxBit 2147483647

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (int number, int base, FILE *stream);
void clrbuf (int);
int decin (void);
void decout (unsigned int, FILE *);
void digiterror (int, int *, const char *);
int getaline (char *, int);
void hexout (unsigned int, FILE *);
void newline (FILE *);
int writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static int debug_on = FALSE;

void baseout (int number, int base, FILE * stream) {
    int character;  /* variable to store converted number. */ 
    int counter;    /* counter to track number of digits converted to base. */
    char numberArray[10]; /* array to store converted number. */
    int index = 0;
        if( number < 0 ){
            number = number + maxBit;
            number++; 
        }   
        if( base == HEX ){  /* condition for when base to convert to is hex. */
            counter = 0;    /* initializing counter to start at 0. */ 
            
            while( number > 0 ){   /* loop to convert number. */ 
                character = number % HEX; /* converts last digit in number. */ 
                number = number / HEX; /* removes converted digit in number. */
                numberArray[counter] = digits[character]; /* stores converted*/
                counter++;  /* counter increases. */
            }
            /* loop to pad first index with a 0 */
            for( index = counter; index < COUNT; index++ ){ 
                /* finds first index; inserts */
                numberArray[index] = digits[0]; 
            }
             index--; 
            /* loop to print converted numbers stored in array */
            while( index >= 0 ){
                /* prints digit in array at counter position */   
                fputc( numberArray[index], stream );  
                index--; /* decreases counter to move index to next position */
            } 
        }
        /* loop to initialize if base to convert to is decimal */
        else if( base == DECIMAL ){
            counter = 0;    /* initializing counter to start at 0. */
            if( number == 0 ){  /* checks if number is 0.*/
                /* prints 0 at the first index when number is 0. */
                numberArray[0] = digits[0]; 
            }
            /* loop to initialize while number isnt 0. */
            while( number != 0 ){
                character = number % DECIMAL;   
                number = number / DECIMAL;
                numberArray[counter] = digits[character];
                counter++; 
            } 
            while( counter > 0 ){
                fputc( numberArray[counter-1], stream );
                counter--;
            }
        }
    }


/*-------------------------------------------------------------------------- 
Function Name:          clrbuf 
Purpose:                When called, this function will clear stdin. 
Description:            This function checks to see if the incoming 
                        parameter is already '\n' indicating that stdin 
                        is already clear.  If stdin is already clear, this 
                        function does nothing.  Otherwise, this function 
                        calls "fgetc" in a loop until stdin is clear. 
Input:                  character:  the most recent character received from a 
                                previous call to fgetc. 
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/  
void clrbuf (int character) {  
	/* removes character from stdin buffer */
    while( character != '\n' ){
            character = fgetc(stdin);
    }  
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
int decin (void) {
    int character = 0;
    int numSum = 0;
    int oldSum = 0;

        while( character != '\n' ){
            character = fgetc(stdin);
            /* check if character is a valid number */
            if( character >= ASCII_ZERO && character <= ASCII_NINE ){
                oldSum = numSum;
                numSum = numSum * DECIMAL + ( character - ASCII_ZERO );
            }
            if( character == '\n' ){;
                clrbuf(character);
                break;
            }
            if( character == EOF ){
                return EOF;
            }
            /* check if character is within the range of valid characters */
            if( character < ASCII_ZERO || character > ASCII_NINE  ){
                digiterror( character, &numSum, OUT_OF_RANGE );
                oldSum = 0;
                continue;
            }
            /* check to see if character causes overflow */
            if( numSum / DECIMAL != oldSum ){
                digiterror( character, &numSum, CAUSED_OVERFLOW );
                oldSum = 0;
            } 
        } 
        return numSum; 
    }


/*--------------------------------------------------------------------------
Function Name:         decout.
Purpose:               Delegates conversion of number to decimal to baseout and
                       prints it.
Description:           Given a positive number, decout takes the number; tells
                       basout to convert it and then displays it in DECIMAL. It
                       does this by delegating the task to baseout.
Input:                 Numeric value to be displayed (number) and where to 
                       display (stream).
Result:                Numeric value to be displayed (number) and where to 
                       display (stream). 
Return Value:          No return value.
--------------------------------------------------------------------------*/
void decout (unsigned int number, FILE * stream) {
    /* call to baseout to convert number to decimal. */
    baseout( number, DECIMAL, stream );
}


/*--------------------------------------------------------------------------
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, int * sum, const char * message) {

	/* handle error */
	clrbuf (character);

	/* reset sum */
	*sum = 0;

	/* output error message */
	writeline (DIGIT_STRING, stderr);
	fputc (character, stderr);
	writeline (message, stderr);

	writeline (REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
int getaline (char * message, int maxlength) {
    int character = 0; 
    int index = 0;
    int i;

    /* inserts null character in array for clrbuf to work properly */    
    for( i = 0; i < maxlength ; i++){
        message[i] = '\0';
    }

    while(character != '\n' ){
        character = fgetc(stdin); /* gets character from user */
        message[index] = (char)character; /* stores character in array */
        /* condition to check if newline is reached */
        if( character == '\n' ){
            message[index] = '\0';
            break;
        }
        /* condition to check if EOF is reached */
        if( character == EOF ){
            return EOF;
        }
        /* condtion to check if maxlength is reached */
        if( index == maxlength - 1 ){
            clrbuf(character);
            message[index] = '\0';
            break;
        }
        index++;
    }
    /* resets maxlength size to message size */
    if( index < maxlength ){
        maxlength = index;
    }
    /* debug messages to print for user when in debug mode */
    if( debug_on == TRUE ){
        fprintf( stderr, "%s %d]", DEBUG_GETALINE, index );
    }
    return index;
}


/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderr
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout (unsigned int number, FILE * stream) {

    /* Output "0x" for hexidecimal. */
    writeline ("0x", stream);
    /* call to baseout to convert number to hex. */
    baseout (number, HEX, stream);
}


/*--------------------------------------------------------------------------
Function Name:         newline.
Purpose:               Prints a new line to the stream.
Description:           Prints a new line to the stream by calling on writeline 
                       to print the new line.
Input:                 Where to display (stream).
Result:                Sets a new line for stream to write on.
return Value:          No return value.
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
    /* calls fputc to print new line in the stream. */
    fputc( '\n', stream );
}


/*--------------------------------------------------------------------------
Function Name:         Writeline.
Purpose:               Writes messages to the stream for display to the user.
Description:           Writes messages to the stream for display to the user by
                       storing each character of the message in an array and 
                       then using stream to print the message.
Input:                 Character values to be displayed (message) and where to
                       display (stream).
Result:                Writes characters to stream.
Return Value:          Returns message. 
--------------------------------------------------------------------------*/
int writeline (const char * message, FILE * stream) {
       
    int index = 0;

    /* condition to print message */
    while( message[index] != '\0'){
        fputc( message[index], stream);
        index++;
    }
    if( debug_on == TRUE ){
        fprintf(stderr, "%s %d]\n", DEBUG_WRITELINE, index );
    }
    return index;
}


/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
	char buffer[MAXLENGTH];      /* to hold string */
	int number;                  /* to hold number entered */
	int strlen;                  /* length of string */

	int array[10];               /* to show user where memory is allocated */
	int * ap = array;		/* to show user about addresses in memory */
	int ** app = &ap;		/* to show user about addresses in memory */
	int * apx = &array[0];	/* to show user about addresses in memory */
	char option;                 /* the command line option */

	/* initialize debug states */
	debug_on = FALSE;

	/* check command line options for debug display */
	while ((option = getopt (argc, argv, "x")) != EOF) {
		switch (option) {
			case 'x': debug_on = TRUE; break;
		}
	}

	/* infinite loop until user enters ^D */
	while (1) {
		writeline ("\nPlease enter a string:  ", stdout);
		strlen = getaline (buffer, MAXLENGTH);
		newline (stdout);

		/* check for end of input */
		if (strlen == EOF)
			break;

		writeline ("The string is:  ", stdout);
		writeline (buffer, stdout);

		writeline ("\nIts length is ", stdout);
		decout (strlen, stdout);
		newline (stdout);

		writeline ("\nPlease enter a decimal number:  ", stdout);
		if ((number = decin ()) == EOF)
			break;

		writeline ("Number entered is:  ", stdout);
		decout (number, stdout);

		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number, stdout);

		writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
		decout (number << 3, stdout);
		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number << 3, stdout);

		newline (stdout);
	}

	writeline ("\nThe value of ap is:  ", stdout);
	decout ((int) ap, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((int) ap, stdout);
	newline (stdout);

	writeline ("The value of app is:  ", stdout);
	decout ((int) app, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((int) app, stdout);
	newline (stdout);

	writeline ("The value of apx is:  ", stdout);
	decout ((int) apx, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((int) apx, stdout);
	newline (stdout);

	writeline ("The value of ap + 1 is:  ", stdout);
	decout ((int) (ap+1), stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((int) (ap+1), stdout);
	newline (stdout);

	writeline ("The address of array[0] is:  ", stdout);
	decout ((int) &array[0], stdout);
	newline (stdout);

	writeline ("The address of array[1] is:  ", stdout);
	decout ((int) &array[1], stdout);
	newline (stdout);

	writeline ("The size of a float is:  ", stdout);
	decout (sizeof (float), stdout);
	newline (stdout);

	writeline ("The size of a double is:  ", stdout);
	decout (sizeof (double), stdout);
	newline (stdout);

	writeline ("The size of a long double is:  ", stdout);
	decout (sizeof (long double), stdout);
	newline (stdout);

	writeline ("The size of a char is:  ", stdout);
	decout (sizeof (char), stdout);
	newline (stdout);

	writeline ("The size of an int is:  ", stdout);
	decout (sizeof (int), stdout);
	newline (stdout);

	writeline ("The size of a short is:  ", stdout);
	decout (sizeof (short), stdout);
	newline (stdout);

	writeline ("The size of a short int is:  ", stdout);
	decout (sizeof (short int), stdout);
	newline (stdout);

	writeline ("The size of a long is:  ", stdout);
	decout (sizeof (long), stdout);
	newline (stdout);

	writeline ("The size of a long int is:  ", stdout);
	decout (sizeof (long int), stdout);
	newline (stdout);

	writeline ("The size of a long long is:  ", stdout);
	decout (sizeof (long long), stdout);
	newline (stdout);

	writeline ("The size of a signed is:  ", stdout);
	decout (sizeof (signed), stdout);
	newline (stdout);

	writeline ("The size of a signed char is:  ", stdout);
	decout (sizeof (signed char), stdout);
	newline (stdout);

	writeline ("The size of a signed short is:  ", stdout);
	decout (sizeof (signed short), stdout);
	newline (stdout);

	writeline ("The size of a signed short int is:  ", stdout);
	decout (sizeof (signed short int), stdout);
	newline (stdout);

	writeline ("The size of a signed int is:  ", stdout);
	decout (sizeof (signed int), stdout);
	newline (stdout);

	writeline ("The size of a signed long is:  ", stdout);
	decout (sizeof (signed long), stdout);
	newline (stdout);

	writeline ("The size of a signed long int is:  ", stdout);
	decout (sizeof (signed long int), stdout);
	newline (stdout);

	writeline ("The size of a signed long long is:  ", stdout);
	decout (sizeof (signed long long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned is:  ", stdout);
	decout (sizeof (unsigned), stdout);
	newline (stdout);

	writeline ("The size of an unsigned char is:  ", stdout);
	decout (sizeof (unsigned char), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short is:  ", stdout);
	decout (sizeof (unsigned short), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short int is:  ", stdout);
	decout (sizeof (unsigned short int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned int is:  ", stdout);
	decout (sizeof (unsigned int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long is:  ", stdout);
	decout (sizeof (unsigned long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long int is:  ", stdout);
	decout (sizeof (unsigned long int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long long is:  ", stdout);
	decout (sizeof (unsigned long long), stdout);
	newline (stdout);

	writeline ("The size of a void pointer is:  ", stdout);
	decout (sizeof (void *), stdout);
	newline (stdout);

	writeline ("The size of a character pointer is:  ", stdout);
	decout (sizeof (char *), stdout);
	newline (stdout);

	writeline ("The size of an int pointer is:  ", stdout);
	decout (sizeof (int *), stdout);
	newline (stdout);

	writeline ("The size of a long pointer is:  ", stdout);
	decout (sizeof (long *), stdout);
	newline (stdout);

	writeline ("The size of a float pointer is:  ", stdout);
	decout (sizeof (float *), stdout);
	newline (stdout);

	writeline ("The size of a double pointer is:  ", stdout);
	decout (sizeof (double *), stdout);
	newline (stdout);

	writeline ("The size of a long double pointer is:  ", stdout);
	decout (sizeof (long double *), stdout);
	newline (stdout);

	newline (stdout);

	return 0;
}
