/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2023
* Author: Boyu_Li, Golden Zhang
* Professors: Paulo Sousa
************************************************************
###################################################
#                                                 #
#    ALGONQUIN         @@@@@@@         COLLEGE    #
#                  @@-----------@@                #
#               @@@@|  M O L D  |@@@@             #
#            @@@@@@@@-----------@@@@@@@@          #
#         @@@@@@@@@@@@@  @@@@@@@   @@@@@@@        #
#       @@@@@@@@@@@@@      @@@       @@@@@@       #
#     @@@@@@@    @@@@@    @@@@       @@@@@@@@     #
#    @@@@@@@       @@@@@ @@@@@@@    @@@@@@@@@@    #
#   @@@@@@@        @@@@@ @@@@@ @@@@@@    @@@@@@   #
#  @@@@@@@@@@    @@             @@@@      @@@@@@  #
#  @@@@@@@@@@@@@@@  @@@@@  @@@@  @@@@   @@    @@  #
# @@@@@@@@@@@@@@@   @@@@@ @@@@@   @@@@@@@@@    @@ #
# @@@@@      @@@@   @@@ @@@ @@@   @@@@    @@@@@@@ #
# @@@@        @@@@  @@@ @@@ @@@   @@@      @@@@@@ #
#  @@@@     @@@@@@@              @@@@@    @@@@@@  #
#  @@@@@@@@@@@     @@@  @@@   @@@    @@@@@@@@@@   #
#   @@@@@@@@@@@   @@@ @@@@@@ @@@@@    @@@@@@@@@   #
#    @@@@@@@@@@@@@@@ @@@@@@    @@@@@@@@@@@@@@@    #
#     @@@@@@@@@       @@@        @@@@@@@@@@@      #
#       @@@@@@         @@         @@@@@@@@@       #
#         @@@@@       @@@@@     @@@@@@@@@         #
#            @@@@@@@@@@@@@@@@@@@@@@@@@            #
#               @@@@@@@@@@@@@@@@@@@               #
#  COMPILERS        @@@@@@@@@@@        2023-S     #
#                                                 #
###################################################
*/

/*
************************************************************
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
 * to suppress the warnings about using "unsafe" functions like fopen()
 * and standard sting library functions defined in string.h.
 * The define does not have any effect in Borland compiler projects.
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_

#include "Compilers.h"

#endif

#ifndef BUFFER_H_

#include "Reader.h"

#endif

#ifndef SCANNER_H_

#include "Scanner.h"

#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern ReaderPointer stringLiteralTable;    /* String literal table */
i64 line;                                /* Current line number of the source code */
extern i64 errorNumber;                /* Defined in platy_st.c - run-time error number */

extern i64 stateType[NUM_STATES];
extern String keywordTable[KWT_SIZE];
extern PTR_ACCFUN finalStateTable[NUM_STATES];
extern i64 transitionTable[NUM_STATES][CHAR_CLASSES];

/* Local(file) global objects - variables */
static ReaderPointer lexemeBuffer;            /* Pointer to temporary lexeme buffer */
static ReaderPointer sourceBuffer;            /* Pointer to input source buffer */


/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
/* TO_DO: Follow the standard and adjust datatypes */

i64 startScanner(ReaderPointer psc_buf) {
    /* TO_DO: Start histogram */
    /* Basic scanner initialization */
    /* in case the buffer has been read previously  */
    readerRecover(psc_buf);
    readerClear(stringLiteralTable);
    line = 1;
    sourceBuffer = psc_buf;
    for (int i = 0; i < NUM_TOKENS -1; i++){
        scData.scanHistogram[i] = 0;
    }
    return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(bust_void) {

    /* TO_DO: Follow the standard and adjust datatypes */

    Token currentToken = {0}; /* token to return after pattern recognition. Set all structure members to 0 */
    LanguageAttributes indentation = {0};
    bust_char c;    /* input symbol */
    i64 state = 0;        /* initial state of the FSM */
    i64 lexStart;        /* start offset of a lexeme in the input char buffer (array) */
    i64 lexEnd;        /* end offset of a lexeme in the input char buffer (array)*/

    i64 lexLength;        /* token length */
    i64 i;                /* counter */
    /*
    bust_char newc;			// new char
    */

    while (1) { /* endless loop broken by token returns it will generate a warning */
        c = readerGetChar(sourceBuffer);

        /* ------------------------------------------------------------------------
            Part 1: Implementation of token driven scanner.
            Every token is possessed by its own dedicated code
            -----------------------------------------------------------------------
        */

        /* TO_DO: All patterns that do not require accepting functions */
        switch (c) {

            /* Cases for spaces */
            case ' ':
                break;
            case '\t':
                indentation.indentationCurrentPos++;
            case '\f':
                break;
            case ',':
            case '\n':
                line++;
                break;

                /* Cases for symbols */
            case ';':
                currentToken.code = EOS_T;
                return currentToken;
            case '(':
                currentToken.code = LPR_T;
                return currentToken;
            case ')':
                currentToken.code = RPR_T;
                return currentToken;
            case '{':
                currentToken.code = LBR_T;
                return currentToken;
            case '}':
                currentToken.code = RBR_T;
                return currentToken;
            case '.':
                currentToken.code = PER_T;
                return currentToken;
            case '+':
                currentToken.code = ARI_T;
                currentToken.attribute.arithmeticOperator = OP_ADD;
                return currentToken;
            case '-':
                currentToken.code = ARI_T;
                currentToken.attribute.arithmeticOperator = OP_SUB; 
                return currentToken;
            case '*':
                currentToken.code = ARI_T;
                currentToken.attribute.arithmeticOperator = OP_MUL;
                return currentToken;
            case '%':
                currentToken.code = ARI_T;
                currentToken.attribute.arithmeticOperator = OP_DIV;
                return currentToken;
            case '^':
                currentToken.code = ARI_T;
                currentToken.attribute.arithmeticOperator = OP_POW;
                return currentToken;
            case '_':
                currentToken.code = UND_T;
                return currentToken;
            case '<':
                currentToken.code = REL_T;
                currentToken.attribute.relationalOperator = OP_LT;
                return currentToken;
            case '>':
                currentToken.code = REL_T;
                currentToken.attribute.relationalOperator = OP_GT;
                return currentToken;
            case '=':
                if (readerGetChar(sourceBuffer)=='='){
                    currentToken.code = REL_T;
                    currentToken.attribute.relationalOperator = OP_EQ;
                    return currentToken;
                }else {
                    readerRetract(sourceBuffer);
                    currentToken.code = ASS_T;
                    currentToken.attribute.assignmentOperator = OP_ASS;
                    return currentToken;
                }
            case '!':
                if (readerGetChar(sourceBuffer)=='='){
                    currentToken.code = REL_T;
                    currentToken.attribute.relationalOperator = OP_NE;
                    return currentToken;
                }else {
                    readerRetract(sourceBuffer);
                    currentToken.code = LOG_T;
                    currentToken.attribute.assignmentOperator = OP_NOT;
                    return currentToken;
                }
            case '|':
                currentToken.code = LOG_T;
                currentToken.attribute.logicalOperator = OP_OR;
                return currentToken;
            case '&':
                currentToken.code = LOG_T;
                currentToken.attribute.logicalOperator = OP_AND;
                return currentToken;
                /* Cases for END OF FILE */
            case CHARSEOF0:
                currentToken.code = SEOF_T;
                currentToken.attribute.seofType = SEOF_0;
                return currentToken;
            case CHARSEOF255:
                currentToken.code = SEOF_T;
                currentToken.attribute.seofType = SEOF_255;
                return currentToken;

                /* ------------------------------------------------------------------------
                    Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
                    Note: Part 2 must follow Part 1 to catch the illegal symbols
                    -----------------------------------------------------------------------
                */

                /* TO_DO: Adjust / check the logic for your language */

            default: // general case
                state = nextState(state, c);
                lexStart = readerGetPosRead(sourceBuffer) - 1;
                readerSetMark(sourceBuffer, lexStart);
                int pos = 0;
                while (stateType[state] == NOFS) {
                    c = readerGetChar(sourceBuffer);
                    state = nextState(state, c);
                    pos++;
                }
                if (stateType[state] == FSWR)
                    readerRetract(sourceBuffer);
                lexEnd = readerGetPosRead(sourceBuffer);
                lexLength = lexEnd - lexStart;
                lexemeBuffer = readerCreate((i64) lexLength + 2, 0, MODE_FIXED);
                if (!lexemeBuffer) {
                    fprintf(stderr, "Scanner error: Can not create buffer\n");
                    exit(1);
                }
                readerRestore(sourceBuffer);
                for (i = 0; i < lexLength; i++)
                    readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
                readerAddChar(lexemeBuffer, READER_TERMINATOR);
                currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
                readerRestore(lexemeBuffer); //xxx
                return currentToken;
        } // switch

    } //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
/* TO_DO: Just change the datatypes */

i64 nextState(i64 state, bust_char c) {
    i64 col;
    i64 next;
    col = nextClass(c);
    next = transitionTable[state][col];
    if (DEBUG)
        printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
    assert(next != FS);
    if (DEBUG)
        if (next == FS) {
            printf("Scanner Error: Illegal state:\n");
            printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
            exit(1);
        }
    return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

i64 nextClass(bust_char c) {
    i64 val = -1;
    switch (c) {
        case CHRCOL2:
            val = 2;
            break;
        case CHRCOL3:
            val = 3;
            break;
        case CHRCOL5:
            val = 5;
            break;
        case CHRCOL6:
            val = 6;
            break;
        case CHRCOL8:
            val = 8;
            break;
        case COMM_SYM:
            val = 4;
            break;
        case CHARSEOF0:
        case CHARSEOF255:
            val = 7;
            break;
        default:
            if (isalpha(c))
                val = 0;
            else if (isdigit(c))
                val = 1;
            else
                val = 7;
    }
    return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
/* TO_DO: Adjust the function for IL */

Token funcCMT(String lexeme) {
    Token currentToken = {0};
    i64 i = 0, len = (i64) strlen(lexeme);
    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
    for (i = 1; i < len - 1; i++) {
        if (lexeme[i] == '\n')
            line++;
    }
    currentToken.code = CMT_T;
    return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ***********************************************************
 */
/* TO_DO: Adjust the function for IL */

Token funcIL(String lexeme) {
    Token currentToken = {0};
    bust_long tlong;
    if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) {
        currentToken = (*finalStateTable[ESNR])(lexeme);
    } else {
        tlong = atol(lexeme);
        if (tlong >= 0 && tlong <= SHRT_MAX) {
            currentToken.code = INL_T;
            currentToken.attribute.intValue = (i64) tlong;
        } else {
            currentToken = (*finalStateTable[ESNR])(lexeme);
        }
    }
    return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
/* TO_DO: Adjust the function for ID */

Token funcID(String lexeme) {
    Token currentToken = {0};
    size_t length = strlen(lexeme);
    bust_char lastch = lexeme[length - 1];
    i64 isID = BUST_FALSE;
    switch (lastch) {
        case MNID_SUF:
            currentToken.code = MNID_T;
            isID = BUST_TRUE;
            break;
        default:
            // Test Keyword
            currentToken = funcKEY(lexeme);
            break;
    }
    if (isID == BUST_TRUE) {
        strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
        currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
    }
    return currentToken;
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */

Token funcSL(String lexeme) {
    Token currentToken = {0};
    i64 i = 0, len = (i64) strlen(lexeme);
    currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
    for (i = 1; i < len - 1; i++) {
        if (lexeme[i] == '\n')
            line++;
        if (!readerAddChar(stringLiteralTable, lexeme[i])) {
            currentToken.code = RTE_T;
            strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
            errorNumber = RTE_CODE;
            return currentToken;
        }
    }
    if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
        currentToken.code = RTE_T;
        strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
        errorNumber = RTE_CODE;
        return currentToken;
    }
    currentToken.code = STR_T;
    return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
/* TO_DO: Adjust the function for Keywords */

Token funcKEY(String lexeme) {
    Token currentToken = {0};
    i64 kwindex = -1, j = 0;
    for (j = 0; j < KWT_SIZE; j++)
        if (!strcmp(lexeme, &keywordTable[j][0]))
            kwindex = j;
    if (kwindex != -1) {
        currentToken.code = KW_T;
        currentToken.attribute.codeType = kwindex;
    } else {
        currentToken = funcVAR(lexeme);
    }
    return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for Errors */

Token funcErr(String lexeme) {
    Token currentToken = {0};
    i64 i = 0, len = (i64) strlen(lexeme);
    if (len > ERR_LEN) {
        strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
        currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
        strcat(currentToken.attribute.errLexeme, "...");
    } else {
        strcpy(currentToken.attribute.errLexeme, lexeme);
    }
    for (i = 0; i < len; i++)
        if (lexeme[i] == '\n')
            line++;
    currentToken.code = ERR_T;
    return currentToken;
}


/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

bust_void printToken(Token t) {
    extern String keywordTable[]; /* link to keyword table in */
    switch (t.code) {
        case RTE_T:
            printf("RTE_T\t\t%s", t.attribute.errLexeme);
            /* Call here run-time error handling component */
            if (errorNumber) {
                printf("%d", errorNumber);
                exit(errorNumber);
            }
            scData.scanHistogram[10]++;
            printf("\n");
            break;
        case ERR_T:
            printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
            scData.scanHistogram[0]++;
            break;
        case SEOF_T:
            printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
            scData.scanHistogram[11]++;
            break;
        case MNID_T:
            printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
            scData.scanHistogram[1]++;
            break;
        case STR_T:
            printf("STR_T\t\t%s\n", readerGetContent(stringLiteralTable, (i64)t.attribute.codeType));
            scData.scanHistogram[3]++;
            break;
        case LPR_T:
            printf("LPR_T\n");
            scData.scanHistogram[4]++;
            break;
        case RPR_T:
            printf("RPR_T\n");
            scData.scanHistogram[5]++;
            break;
        case LBR_T:
            scData.scanHistogram[6]++;
            printf("LBR_T\n");
            break;
        case RBR_T:
            printf("RBR_T\n");
            scData.scanHistogram[7]++;
            break;
        case KW_T:
            printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
            scData.scanHistogram[8]++;
            break;
        case CMT_T:
            printf("CMT_T\n");
            scData.scanHistogram[12]++;
            break;
        case EOS_T:
            printf("EOS_T\n");
            scData.scanHistogram[9]++;
            break;
        case VAR_T:
            printf("VAR_T\t\t%s\n",t.attribute.idLexeme);
            scData.scanHistogram[14]++;
            break;
        case INL_T:
            printf("INL_T\t\t%d\n",t.attribute.intValue);
            scData.scanHistogram[2]++;
            break;
        case ARI_T:
            printf("ARI_T\t\t%d\n",t.attribute.arithmeticOperator);
            scData.scanHistogram[17]++;
            break;
        case FLL_T:
            printf("FLL_T\t\t%.4f\n",t.attribute.floatValue);
            scData.scanHistogram[13]++;
            break;
        case PER_T:
            printf("PER_T\n");
            scData.scanHistogram[15]++;
            break;
        case REL_T:
            printf("REL_T\t\t%d\n", t.attribute.relationalOperator);
            scData.scanHistogram[16]++;
            break;
        case LOG_T:
            printf("LOG_T\n");
            scData.scanHistogram[20]++;
            break;
        case UND_T:
            printf("UND_T\n");
            scData.scanHistogram[19]++;
            break;
        case ASS_T:
            printf("ASS_T\n");
            scData.scanHistogram[18]++;
            break;
        default:
            printf("Scanner error: invalid token code: %d\n", t.code);
    }
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
bust_void printScannerData(ScannerData scData) {
    for (int i = 0; i < NUM_TOKENS; i++) {
        printf("The tokens %s appears %d times in the file\n",tokenStrTable[i],scData.scanHistogram[i]);
    }
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/

/*
************************************************************
 * Acceptance State Function FL
 *		Function responsible to identify FL (float literals).
 ***********************************************************
 */

Token funcFL(String lexeme)
{
    Token currentToken = { 0 };
    bust_doub treal;
    if (lexeme[strlen(lexeme) - 1] == '\0') {
        currentToken = funcErr(lexeme);
    }
    else {
        treal = atof(lexeme);
        if (treal >= 0.0 && treal <= FLT_MAX) {
            currentToken.code = FLL_T;
            currentToken.attribute.floatValue = (f64)treal;
        }
        else {
            currentToken = funcErr(lexeme);
        }
    }

    return currentToken;
}

/*
************************************************************
 * Acceptance State Function VAR
 *		Function responsible to identify VAR (Variable).
 ***********************************************************
 */
Token funcVAR(String lexeme) {
    Token currentToken = { 0 };
    currentToken.code = VAR_T;
    strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
    currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
    return currentToken;
}