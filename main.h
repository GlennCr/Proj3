//main.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "syntax.h"

#define MAX_TOKEN_LENGTH			100
#define MAX_SYMTABLE_LENGTH			100
#define KEYWORDS 					14

#define RESERVED 37
#define VAR 1
#define BEGIN 2
#define END 3
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define BOOLEAN 13
#define TYPE 14
#define PLUS 15
#define MINUS 16
#define DIV 17
#define MULT 18
#define EQUAL 19
#define COLON 20 
#define COMMA 21
#define SEMICOLON 22
#define LBRAC 23
#define RBRAC 24
#define LPAREN 25
#define RPAREN 26
#define NOTEQUAL 27
#define GREATER 28
#define LESS 29
#define LTEQ 30
#define GTEQ 31
#define DOT 32
#define ID 33
#define NUM 34
#define ERROR 35
#define LBRACE 36
#define RBRACE 37

#define PRIMARY	0
#define EXPR 	1
#define NOOP 	2
#define GOTO 	3
#define ASSIGN 	4 
#define IF 		5
#define WHILE 	6 
