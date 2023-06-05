/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2023
* Author: TO_DO
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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 ?Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
#define SET_BIT3	0x08
#define SET_BIT2	0x04
#define SET_BIT1	0x02
#define SET_BIT0	0x01

#define RESET_BIT3	0xF7
#define RESET_BIT2	0xFB
#define RESET_BIT1	0xFD
#define RESET_BIT0	0xFE

#define CHK_BIT3	0x08
#define CHK_BIT2	0x04
#define CHK_BIT1	0x02
#define CHK_BIT0	0x01

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct offset {
	bust_intg mark;			/* the offset to the mark position (in chars) */
	bust_intg read;			/* the offset to the get a char position (in chars) */
	bust_intg wrte;			/* the offset to the add chars (in chars) */
} Offset;

/* Buffer structure */
typedef struct bufferReader {
	bust_string	content;			/* pointer to the beginning of character array (character buffer) */
	bust_intg	size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	bust_intg	increment;			/* character array increment factor */
	bust_intg	mode;				/* operational mode indicator */
	bust_byte	flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Offset		offset;				/* Offset / position field */
	bust_intg	histogram[NCHAR];	/* Statistics of chars */
	bust_intg	numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(bust_intg, bust_intg, bust_intg);
ReaderPointer	readerAddChar		(ReaderPointer const, bust_char);
bust_boln		readerClear		    (ReaderPointer const);
bust_boln		readerFree		    (ReaderPointer const);
bust_boln		readerIsFull		(ReaderPointer const);
bust_boln		readerIsEmpty		(ReaderPointer const);
bust_boln		readerSetMark		(ReaderPointer const, bust_intg);
bust_intg		readerPrint		    (ReaderPointer const);
bust_intg		readerLoad			(ReaderPointer const, FILE* const);
bust_boln		readerRecover		(ReaderPointer const);
bust_boln		readerRetract		(ReaderPointer const);
bust_boln		readerRestore		(ReaderPointer const);
/* Getters */
bust_char		readerGetChar		(ReaderPointer const);
bust_string		readerGetContent	(ReaderPointer const, bust_intg);
bust_intg		readerGetPosRead	(ReaderPointer const);
bust_intg		readerGetPosWrte	(ReaderPointer const);
bust_intg		readerGetPosMark	(ReaderPointer const);
bust_intg		readerGetSize		(ReaderPointer const);
bust_intg		readerGetInc		(ReaderPointer const);
bust_intg		readerGetMode		(ReaderPointer const);
bust_byte		readerGetFlags		(ReaderPointer const);
bust_void		readerPrintStat		(ReaderPointer const);
bust_intg		readerNumErrors		(ReaderPointer const);

#endif
