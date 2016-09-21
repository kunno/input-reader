/*
File Name:      reader.java
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of java pre defined types.

****************************************************************************/  
/**
 * The reader class is a direct port of reader.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
    private Character character;

    public MyLibCharacter (int ch) {
        character = new Character ( (char) ch );
    }

    public char charValue () {
        return character.charValue ();
    }
    public String toString () {
        return "" + character;
    }
}

public class reader {
	private static final int ASCII_ZERO = 48; // ASCII value of 0
    private static final int ASCII_NINE = 57; // ASCII value of 9
	private static final int COUNT = 8;		// # of hex digits
	private static final int CR = 13;		// Carriage Return
	private static final int MAXLENGTH = 80;	// Max string length

	private static final int EOF = -1;		// process End Of File

	private static final int DECIMAL = 10;		// to indicate base 10
	private static final int HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// for ASCII conversion
	     new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

	private static final String DEBUG_GETALINE = 
		"[*DEBUG:  The length of the string just entered is ";

	private static final String DIGIT_STRING = "digit ";
	private static final String REENTER_NUMBER ="\nPlease reenter number: ";
	private static final String OUT_OF_RANGE = " out of range!!!\n";
	private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
	private static final String DEBUG_WRITELINE =
		"\n[*DEBUG:  The length of the string displayed is ";

	private static Stack<MyLibCharacter> InStream =
		new Stack<MyLibCharacter>();

	private static boolean debug_on = false;
	private static int hexCounter = 0; // counter for the number hex digits

	/* Function Name: Baseout.
     * Purpose:Takes in a positive number and displays in a given base.
     * Decription: Given a positive number, baseout takes the number; converts
     * and displays it in both HEX and DECIMAL. It does this by taking the
     * number and checks to make sure it is non-zero, the mods and divides it 
     * by the appropriate base to get the conversion, stores it in an array and
     * displays it to the user.
     * Result: Displays number in both DECIMAL and HEX.
     * Input: Numeric value to be displayed (number), base to used to 
     * display (base), and where to display (stream).
     * Return value: No return value.
     */
    private static void baseout (int number, int base, PrintStream stream) {
        int character;  // variable to store converted number. 
        int counter;    // counter to track number of digits converted to base.
        char[] numberArray; // array to store converted number.
        int SIZE = 50;  // variable to set array size.
        int index = 0;
            
        if( base == HEX ){  // condition for when base to convert to is hex. 
            numberArray = new char[COUNT];  // set array size to equal count. 
            counter = 0;    // initializing counter to start at 0. 
            
            while( number > 0 ){   // loop to convert number. 
                character = number % HEX;   // converts last digit in number. 
                number = number / HEX;  // removes converted digit in number.
                numberArray[counter] = digits[character];   // stores converted
                counter++;  // counter increases.
            }
            // loop to pad first index with a 0
            for( index = counter; index < COUNT; index++ ){ 
                numberArray[index] = digits[0]; // finds first index; inserts 0
            }
            index--;
            // loop to print converted numbers stored in array
            while( index >= 0 ){
                // prints digit in array at counter position   
                fputc( numberArray[index], stream );  
                index--;  // decreases counter to move index to next position
            } 
        }
        // loop to initialize if base to convert to is decimal 
        else if( base == DECIMAL ){
            numberArray = new char[SIZE];   // setting array size to equal size 
            counter = 0;    // initializing counter to start at 0.
            if( number == 0 ){  // checks if number is 0.
                // prints 0 at the first index when number is 0.
                numberArray[0] = digits[0]; 
            }
            // loop to initialize while number isnt 0.
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


	/* Function Name: clrbuf.
     * Purpose: Removes any characters in the stdin buffer by repeatedly 
     * calling fgetc until a newline character is detected. 
     * Decription: The method is implemented by calling fgetc until the buffer 
     * is empty. The buffer is empty when the last character removed is a 
     * newline character '\n'.
     * Result: Displays number in both DECIMAL and HEX.
     * Input: character value in buffer (character).
     * Return value: No return value.
     */ 
    public static void clrbuf (int character) {
		// removes character system.in from buffer
        while( character != '\n' ){
            character = fgetc(System.in);

        }   
    }
    
	/* Function Name: Decin.
     * Purpose: Reads a decimal number from stdin and converts it from a 
     * sequence of ASCII characters into a decimal integer.
     * Decription: Given a positive number, decin takes the number; converts
     * and displays it in both HEX and DECIMAL. It does this by taking the
     * number and checks to make sure it is valid and non overflow before 
     * displaying it to the user. If non valid number or overflow is entered
     * error message is returned.
     * Result: Displays number in both DECIMAL and HEX.
     * Input: Numeric value to be displayed (number), base to used to 
     * display (base), and where to display (stream).
     * Return value: Returns converted value or EOF.
     */ 
    public static int decin() {
    int character = 0;
    int numSum = 0;
    int oldSum = 0;

        while( character != '\n' ){
            character = fgetc(System.in);
            // check if character is valid number
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
            // check if character is within range of valid characters
            if( character < ASCII_ZERO || character > ASCII_NINE  ){
                digiterror( character, OUT_OF_RANGE );
                numSum = 0;
                oldSum = 0;
            }
            // check if character would cause overflow 
            if( numSum / DECIMAL != oldSum ){
                digiterror( character, CAUSED_OVERFLOW );
                numSum = 0;
                oldSum = 0;
            }
        } 
        return numSum; 
    }
        


	/* Function Name: Decout.
     * Purpose: Delegates conversion of number to decimal to baseout and prints
     * it.
     * Decription: Given a positive number, decout takes the number; tells
     * basout to convert it and then displays it in DECIMAL. It does this by 
     * delegating the task to baseout.
     * Result: Displays number in DECIMAL.
     * Input: Numeric value to be displayed (number) and where to display
     * (stream).
     * Return Value: No return value.
     */
    public static void decout (int number, PrintStream stream) {
        // call to baseout
        baseout(number, DECIMAL, stream);
    }

	/*-------------------------------------------------------------------------
	Function Name:          digiterror
	Purpose:                This function handles erroneous user input.
	Description:            This function  displays and error message to the 
                            user, and asks for fresh input.
	Input:                  character:  The character that began the problem.
				message:  The message to display to the user.
	Result:                 The message is displayed to the user.
				The result in progress needs to be set to 0 in
				decin after the call to digiterror.
	-------------------------------------------------------------------------*/
	public static void digiterror (int character, String message) {

		/* handle error */
		clrbuf (character);

		/* output error message */
		writeline (DIGIT_STRING, System.err);
		fputc ( (char)character, System.err);
		writeline (message, System.err);

		writeline (REENTER_NUMBER, System.err);
	}


	/* Function Name: getaline.
     * Purpose: Reads characters from stdin or System.in and stores them into 
     * the message array.
     * Decription: Uses a simple loop reading characters one at a time from the
     * standard input using fgetc until it reads '\n', storing them in sequence
     * in the message buffer so that they can be accessed once the function 
     * completes.
     * Result: Displays string entered by user.
     * Input: Message to be displayed and the maxlength of the message.
     * Return Value: Returns string inputed by user.
     */ 
    public static int getaline( char message[], int maxlength ) {
        int character = 0; 
        int index = 0;

        // inserts null character in array for crlbuf to work properly 
        for( int i = 0; i < message.length; i++){
            message[i] = '\0';
        }

        while(character != '\n' ){
            character = fgetc(System.in); // gets character from user
            message[index] = (char)character; // stores character in array
            // condition to check if newline is reached
            if( character == '\n' ){
                message[index] = '\0';
                break;
            }
            // condition to check if EOF is reached
            if( character == EOF ){
                return EOF;
            }
            // condition to check if maxlength is reached
            if( index == maxlength - 1 ){
                clrbuf(character);
                message[index] = '\0';
                break;
            }
            index++;
        }
        // resets maxlength size to message size
        if( index < maxlength ){
            maxlength = index;
        }
        // debug message for user when in debug mode
        if( debug_on == true ){
            System.err.print( DEBUG_GETALINE + index + "]\n" );
        }
        return index;
    }
    


	/* Function Name: Hexout.
     * Purpose:Delegates conversion of number to hex to baseout and prints it.
     * Decription: Given a positive number, hexout tells baseout to convert it
     * and then displays it in HEX. It does this by taking the delegating the
     * task to baseout. Also calls on writeline function to print the prefix
     * required for all hex values.
     * Result: Displays number in HEX.
     * Input: Numeric value to be displayed (number) and where to display
     * (stream).
     * Return Value: No return value.
     */
    public static void hexout (int number, PrintStream stream) {

        // Output "0x" for hexidecimal.
        writeline ("0x", stream);
        baseout (number, HEX, stream); 
    }


    /**
     * Returns a character from the input stream.
     *
     * @return  <code>char</code> 
     */
    public static int fgetc(InputStream stream) {
        char ToRet = '\0';

        // Check our local input stream first.
        //   If it's empty read from System.in
        if (InStream.isEmpty ()) {
            try {
                // Java likes giving the user the
                // CR character too. Dumb, so just 
                // ignore it and read the next character
                // which should be the '\n'.                  
                ToRet = (char) stream.read ();
                if (ToRet == CR)
                    ToRet = (char) stream.read ();
                                
                // check for EOF
                if ((int) ToRet == 0xFFFF)
                    return EOF;
            }

            // Catch any errors in IO.
            catch (EOFException eof) {

                // Throw EOF back to caller to handle
                return EOF;
            }
            catch (IOException ioe) {

                writeline ("Unexpected IO Exception caught!\n",
                                                    System.out);
                writeline (ioe.toString (), System.out);
            }

        }

        // Else just pop it from the InStream.
        else
            ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
            return ToRet;
    }


    /**
     * Displays a single character.
     *
     * @param    Character to display.
     */
    public static void fputc(char CharToDisp, PrintStream stream) {

        // Print a single character.
        stream.print (CharToDisp);   

        // Flush the system.out buffer, now. 
        stream.flush ();
    }


	/* Function Name: Newline.
     * Purpose: Prints a new line to the stream.
     * Decription: Prints a new line to the stream by calling on writeline to 
     * print the new line.
     * Result: Sets a new line for stream to write on.
     * Input: Where to display (stream).
     * Return Value: No return value.
     */ 
    public static void newline ( PrintStream stream ) {
        // prints new line
        fputc ( '\n', stream ); 
    }

    /* Function Name: Writeline.
     * Purpose: Writes messages to the stream for display to the user.
     * Decription: Writes messages to the stream for display to the user by
     * storing each character of the message in an array and then using stream
     * to print the message.
     * Result: Writes characters to stream.
     * Input: Character values to be displayed (message) and where to 
     * display (stream).
     * Return Value: Returns message.
     */ 
    public static int writeline (String message, PrintStream stream) {  
		
        int msgLength = 0;  // counter to keep track of the message length. 
         int index = 0 ;
        // creates an array to store characters of the message. 
       // char[] messageToDisplay = new char[ message.length() ];

        // loop to insert characters of message into array
        for( ; index < message.length() && message.charAt(index) !='\0'; index++ ){
            // takes a character from message and puts it into array
           // messageToDisplay[index] = new Character( message.charAt( index ) ); 
            fputc( message.charAt(index), stream ); 
        }

        if( debug_on == true ){
            System.err.print( DEBUG_WRITELINE + index + "]\n" );
        }
          /*
        // while loop to print message
        while( msgLength < message.length() ){
            // sends message to stream
            if (messageToDisplay[msgLength]=='\0') break;
            fputc( messageToDisplay[msgLength], stream );
            // increases counter
            msgLength++;
        }   */
        return index; // returns counter till loop condition is false    
        
    }


	/**
	* Places back a character into the input stream buffer.
	*
	* @param    A character to putback into the input buffer stream.
	*/
	public static void ungetc (int ToPutBack) {

		// Push the char back on our local input stream buffer.
		InStream.push (new MyLibCharacter (ToPutBack));
	}


	public static void main( String[] args ) {

		char buffer[] = new char[MAXLENGTH];       /* to hold string */

		int number;                  /* to hold number entered */
		int strlen;                  /* length of string */

		/* initialize debug states */
		debug_on = false;

		/* check command line options for debug display */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				debug_on = true;
		} 

		/* infinite loop until user enters ^D */
		while (true) {
			writeline ("\nPlease enter a string:  ", System.out);

			strlen = getaline (buffer, MAXLENGTH);
			newline (System.out);

			/* check for end of input */
			if ( EOF == strlen )
				break;

			writeline ("The string is:  ", System.out);
			writeline ( new String(buffer), System.out);

			writeline ("\nIts length is ", System.out);
			decout (strlen, System.out);
			newline (System.out);

			writeline ("\nPlease enter a decimal number:  ", System.out);
			if ((number = decin ()) == EOF)
				break;

			writeline ("Number entered is:  ", System.out);
			decout (number, System.out);

			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number, System.out);

			writeline ("\nNumber entered multiplied by 8 is:  ", System.out);
			decout (number << 3, System.out);
			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number << 3, System.out);

			newline (System.out);
		}
	}
}
