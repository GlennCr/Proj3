#include "main.h"
//to do

//Review requirements
//implement his get _token and stuff like that.

//work on plan for code.

char *_reserved[] = 
{	"",
	"VAR", 
	"BEGIN", 
	"END", 
	"ASSIGN", 
	"IF", 
	"WHILE", 
	"DO", 
	"THEN", 
	"print", 
    "INT",
	"REAL",
	"STRING",
	"BOOLEAN",
	"TYPE",
	"+",
	"-", 
	"/", 
	"*", 
	"=", 
	":", 
	",", 
	";", 
	"[", 
	"]", 
	"(", 
	")", 
	"<>", 
	">", 
	"<",
	"<=",
	">=",
    ".",
    "ID",
    "NUM",	
    "ERROR",
	"{",
	"}"
};

int printToken(int _ttype)
{ //prints a defined _token's associated string
   if (_ttype <= RESERVED)
   {   printf("%s\n",_reserved[_ttype]);
       return 1;
   } else
       return 0; 
}

void syntax_error(char* NT, int _line_no)
{
	printf("Syntax error while parsing %s line %d\n", NT, _line_no);
}
//---------------------------------------------------------

//---------------------------------------------------------
// Global Variables associated with the next input _token


//ADDED SYMBOL TABLE AND ENTRY STRUCT
const int MAX_MEM_POOL = 100;

char _token[MAX_TOKEN_LENGTH];      // _token string
int  _ttype;                        // _token type
int  _activeToken = FALSE;                  
int  _tokenLength;

int _mem_pool_alloc = 0;
char* _var_table[MAX_MEM_POOL];		//contains strings of ID's
int _var_val_table[MAX_MEM_POOL];	//contains values of ID's

int _do_var_dec = 0;

int _line_no = 1;

//----------------------------------------------------------
//mine

void push_var(char* newVar)
{
	if(_mem_pool_alloc < MAX_MEM_POOL)
	{
		for(int i = 0; (i < _mem_pool_alloc) & (_mem_pool_alloc > 0), i++ )
		{
			if( strcmp(_var_table[_mem_pool_alloc], newVar) == 0)
			{
				syntax_error("add var declaration, aka 'push_var()'. Tried to declare var more than once!", _line_no);
				exit(0);

			}

		}
		//did not find this var in the var table. Can add it to the var table.
		strcpy( _var_table[_mem_pool_alloc], newVar);
		_var_val_table[_mem_pool_alloc] = 0; //initial value of a var is 0.

	}
	else
	{
		syntax_error("add var declaration, aka 'push_var()'. Input tried to declare more values than MAX_MEM_POOL. Try increasing this value in main.cpp", line_no);
		exit(0);

	}
}

int skipSpace()
{
   char c;

   c = getchar(); 
   _line_no += (c == '\n');
   while (!feof(stdin) && isspace(c))
   {    c = getchar(); 
        _line_no += (c == '\n');
   }

   // return character to input buffer if eof is not reached
   if (!feof(stdin)) 
        ungetc(c,stdin);
}

int isKeyword(char *s)
{
     int i;

     for (i = 1; i <= KEYWORDS; i++)
	if (strcmp(_reserved[i],s) == 0)
	   return i;
     return FALSE;
}

/* ungetToken() simply sets a flag so that when getToken() is called
// the old _ttype is returned and the old _token is not overwritten 
// NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
// AT LEAST ONE CALL TO getToken()
// CALLING TWO ungetToken() WILL NOT UNGET TWO TOKENS 
*/
void ungetToken() //did you mean ungetc()? 
{
    _activeToken = TRUE;
}


int scan_number()
{
	char c;
	
	c = getchar();
	if (isdigit(c))
	{	// First collect leading digits before dot
		// 0 is a NUM by itself
		if (c == '0')	      
		{	_token[_tokenLength] = c;
			_tokenLength++;
			_token[_tokenLength] = '\0';
		} else
		{	while (isdigit(c))
			{	_token[_tokenLength] = c;
				_tokenLength++;;
				c = getchar();
			}
			ungetc(c,stdin);
			_token[_tokenLength] = '\0';
		}

		// Check if leading digits are integer part of a REALNUM
		c = getchar();
		if (c == '.')
		{	c = getchar();
			if (isdigit(c))
			{	_token[_tokenLength] = '.';
				_tokenLength++;
				while (isdigit(c))
				{	_token[_tokenLength] = c;
					_tokenLength++;
					c = getchar();
				}
				_token[_tokenLength] = '\0';
				if (!feof(stdin)) 
					ungetc(c,stdin);
					syntax_error("scan_number. Digit is of type REAL.", _line_no);
				exit(0);
			} else
			{	ungetc(c, stdin);    // note that ungetc returns characters on a stack, so we first
				c = '.';             // return the second character and set c to '.' and return c again
				ungetc(c,stdin);		                                 
				return  NUM;         
                        }
		} else
		{	ungetc(c, stdin);
			return NUM;
		}
	} else
		return ERROR;   
}


int scan_id_or_keyword()
{
	int _ttype;
	char c;

	c = getchar();
	if (isalpha(c))
	{	while (isalnum(c))
		{	_token[_tokenLength] = c;
			_tokenLength++;
			c = getchar();
		}
		if (!feof(stdin)) 
			ungetc(c,stdin); 
             
		_token[_tokenLength] = '\0';		                
		_ttype = isKeyword(_token); 
		if (_ttype == 0) 
			_ttype = ID;
		return _ttype;
	} else
		return ERROR;
}                            
int getToken()
{	char c;
 
       if (_activeToken)
       { _activeToken = FALSE;
         return _ttype;
       }   // we do not need an else because the function returns in the body 
           // of the if
   
       skipSpace();   
       _tokenLength = 0;
       c = getchar();
       switch (c)
       {   	case '.': return DOT;
	       	case '+': return PLUS;
	       	case '-': return MINUS;
	       	case '/': return DIV;
	       	case '*': return MULT;
	       	case '=': return EQUAL;
	       	case ':': return COLON;
	       	case ',': return COMMA;
	       	case ';': return SEMICOLON;
	       	case '[': return LBRAC;
	       	case ']': return RBRAC;
	       	case '(': return LPAREN;
	       	case ')': return RPAREN;
	   		case '{': return LBRACE;
	   		case '}': return RBRACE;
	       	case '<':
                      c = getchar();
                       if (c == '=')
                          return LTEQ;
                       else
                       if (c == '>')
                          return NOTEQUAL;
                       else
                       {
                          ungetc(c,stdin);
                          return LESS;
                       }
           case '>': 
                        c = getchar();
                        if (c == '=')
                           return GTEQ;
                        else
                        {
                           ungetc(c, stdin);
                           return GREATER;
                        }
           
           default :
			if (isdigit(c))
			{	ungetc(c,stdin);
				return scan_number();
			}
			else
			if (isalpha(c))
			{	ungetc(c,stdin);
				return scan_id_or_keyword();
			}
			else
			if (c == EOF)
				return EOF;
			else
				return ERROR;
	}
}



/*----------------------------------------------------------------------------
SYNTAX ANALYSIS SECTION
----------------------------------------------------------------------------*/
//NEED TO EXPAND?
#define PRIMARY 0
#define EXPR 1



/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/

struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct declNode* make_declNode()
{
	return (struct declNode*) malloc(sizeof(struct declNode));
}

struct var_decl_sectionNode* make_var_decl_sectionNode()
{
	return (struct var_decl_sectionNode*) malloc(sizeof(struct var_decl_sectionNode));
}

struct var_decl_listNode* make_var_decl_listNode()
{
	return (struct var_decl_listNode*) malloc(sizeof(struct var_decl_listNode));
}

struct var_declNode* make_var_declNode()
{
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}

struct id_listNode* make_id_listNode()
{
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}

struct bodyNode* make_bodyNode()
{
	return (struct bodyNode*) malloc(sizeof(struct bodyNode));
}

struct stmt_listNode* make_stmt_listNode()
{
	return (struct stmt_listNode*) malloc(sizeof(struct stmt_listNode));
}

struct stmtNode* make_stmtNode()
{
	return (struct stmtNode*) malloc(sizeof(struct stmtNode));
}

struct while_stmtNode* make_while_stmtNode()
{
	return (struct while_stmtNode*) malloc(sizeof(struct while_stmtNode));
}

struct if_stmtNode* make_if_stmtNode()
{
	return (struct if_stmtNode*) malloc(sizeof(struct if_stmtNode));
}

struct assign_stmtNode* make_assign_stmtNode()
{
	return (struct assign_stmtNode*) malloc(sizeof(struct assign_stmtNode));
}

struct print_stmtNode* make_print_stmtNode()
{
	return (struct print_stmtNode*) malloc(sizeof(struct print_stmtNode));
}

struct conditionNode* make_conditionNode()
{
	return (struct conditionNode*) malloc(sizeof(struct conditionNode));
}

struct exprNode* make_exprNode()
{
	return (struct exprNode*) malloc(sizeof(struct exprNode));
}

struct primaryNode* make_primaryNode()
{
	return (struct primaryNode*) malloc(sizeof(struct primaryNode));
}

/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
  PARSING AND BUILDING PARSE TREE
---------------------------------------------------------------------*/

struct primaryNode* primary()
{
	struct primaryNode* primar;
	
	primar = make_primaryNode();

	_ttype = getToken();
	if((_ttype == NUM) | (_ttype == ID))
	{
		//primar->tag = _ttype;
		if(_ttype == NUM)
		{	primar->ival = atoi(_token);
			strcpy(primar->id, "NUM");
		}
		if(_ttype == ID)
		{	
			strcpy(primar->id, _token);
		}
		else
		{
			syntax_error("primary(). Expected id or NUM!", _line_no);
			exit(0);
		}

	}
	return primar;
}

//ok in syntax.h
struct conditionNode* condition()
{	//has two primary values
	struct conditionNode* cond;
	cond = make_conditionNode();

	_ttype = getToken();
	if((_ttype == NUM) | (_ttype == ID))
	{
		char lop_token[100]; //save _token just in case
		strcpy(lop_token, _token);
		
		int lop_ind = checksym(lop_token);
		if(lop_ind == 0)
		{
			syntax_error("condition(). Left Operand not declared!")
		}
		
		ungetToken();
		cond->left_operand = primary();
		_ttype = getToken();

		if((_ttype == NOTEQUAL) | (_ttype == GREATER) | (_ttype == LESS) | (_ttype == LTEQ) | (_ttype == GTEQ) )
		{	//has relop, infers there should be a primary next.
			cond->relop = _ttype;
			cond->right_operand = primary();

			char rop_token[101]; //save _token just in case
			strcpy(rop_token, _token);	
			
			int rop_ind = checksym(rop_token);
			if(rop_ind == 0)
			{
				syntax_error("condition(). Right operand is undeclared!");
				exit(0);
			}

		}
		else
		{	//no relop, ungetToken for further processing.
			syntax_error("condition(). No rel op in condition!");
			exit(0);
		}
	}

		return cond;

}

struct exprNode* expr()
{ //SHOULD NOT BE RECURSIVE
	struct exprNode* exp;
	
	_ttype = getToken();
	if ((_ttype == ID)| (_ttype == NUM))
	{	
		exp = make_exprNode();
		ungetToken();
		exp->leftOperand = primary(); //get left operand.

		_ttype = getToken();
		if ( (_ttype == PLUS) | (_ttype == MINUS) | (_ttype == MULT) | (_ttype == DIV) )
		{	
			exp->operator = _ttype;

			_ttype = getToken();
			if ((_ttype == ID)| (_ttype == NUM))
			{	
				exp->tag = EXPR;
				ungetToken();
				exp->rightOperand = primary(); //get right operand.

				_ttype = getToken();
				if(_ttype == SEMICOLON)
				{
					return exp;
				}
				else
				{
					syntax_error("expr. Expr expected SEMICOLON at end", _line_no);
					exit(0);		
				}
			}
			else
			{
				syntax_error("expr. Primary expected for right operand!", _line_no);
				exit(0);
			}
		} 
		else	
		if (_ttype == SEMICOLON)
		{	exp->tag = PRIMARY;
			ungetToken();
			return expr;
		} 
		else
		{	syntax_error("expr. PLUS, MINUS, DIV, MULT, or SEMICOLON expected", _line_no);
			exit(0);
		}
	} else
	{	syntax_error("expr. Primary expected for left operand!", _line_no);
		exit(0);
	}
}

//TO FIX
struct assign_stmtNode* assign_stmt()
{	struct assign_stmtNode* assignStmt;

	_ttype = getToken();
	if (_ttype == ID) 
	{	assignStmt = make_assign_stmtNode();
		assignStmt->id = (char *) malloc((_tokenLength+1)*sizeof(char));
		strcpy(assignStmt->id,_token);
		
		//_implicit definition code
		//not used
		if(checksym(_token) == 0)
		{
			syntax_error("assign_stmt. _token not defined.", _line_no)
			exit(0);
		}
		
		_ttype = getToken();
		if (_ttype == EQUAL)
		{	assignStmt->expr = expr();
			return assignStmt;
		} else
		{	syntax_error("assign_stmt. EQUAL expected", _line_no);
			exit(0);
		}
	} else
	{
		syntax_error("assign_stmt. ID expected", _line_no);
		exit(0);
	}
}

struct while_stmtNode* while_stmt()
{	//has condition and a body. Condition must be evaluated 'true' to execute body.
	struct while_stmtNode* wle_stmt;
	wle_stmt = make_while_stmtNode();
	
	_ttype = getToken();
	if (_ttype == WHILE)
	{
		wle_stmt->condition = condition();
		wle_stmt->body = body();
	} else
	{
		syntax_error("while_stmt(). Failed to get _ttype of WHILE!", _line_no);
		exit(0);
	}
}

//need make_if_stmtNode()
struct if_stmtNode* if_stmt()
{
	struct if_stmtNode* if_stm;
	if_stm = make_if_stmtNode();

	_ttype = getToken();
	if(_ttype = IF)
	{
		if_stm->condition = condition();
		if_stm->body = body();

	} else
	{
		syntax_error("if_stmt(). Failed to get _ttype of IF!", _line_no);
		exit(0);
	}

}

//need make_print_stmtNode()
struct print_stmtNode print_stmt() 
{
	struct print_stmtNode* print_stm;
	print_stm = make_print_stmtNode();

	_ttype = getToken();
	if(_ttype = PRINT)
}

struct stmtNode* stmt()
{
	struct stmtNode* stm;
	_ttype = getToken();
	stm = make_stmtNode();

	if (_ttype == ID) // assign_stmt
	{	ungetToken();
		stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		_ttype = getToken();
		if (_ttype == SEMICOLON)
		{	return stm;
		}
		else
		{	syntax_error("stmt. SEMICOLON expected", _line_no);
			exit(0);
		}
	} else
	if (_ttype == WHILE) // while_stmt
	{	ungetToken();
		stm->while_stmt = while_stmt();
		stm->stmtType = WHILE;
	}
	else
	if (_ttype == IF)
	{	ungetToken();
		stm->if_stmt = if_stmt();
		stm->stmtType = IF;
		
	} else
	if (_ttype == PRINT)
	{	
		ungetToken();
		stm->print_stmt = print_stmt();
		stm->stmtType = PRINT;
	} else // syntax error
	{
		syntax_error("stmt. ID or WHILE expected", _line_no);
		exit(0);
	}	
}

struct stmt_listNode* stmt_list()
{
	struct stmt_listNode* stmtList;

	_ttype = getToken();


	if ((_ttype == ID)|(_ttype == WHILE))
	{	ungetToken();
		stmtList = make_stmt_listNode();
		stmtList->stmt = stmt();
		_ttype = getToken();
		if ((_ttype == ID) | (_ttype == WHILE))
		{	ungetToken();
			stmtList->stmt_list = stmt_list();
			return stmtList;
		} else	// If the next _token is not in FOLLOW(stmt_list), 
			// let the caller handle it. 
		{	ungetToken();
			return stmtList;
		}
	} else
	{
		syntax_error("stmt_list. ID or WHILE expected", _line_no);
		exit(0);
	}
	
}

struct bodyNode* body()
{	struct bodyNode* bod;

	_ttype = getToken();
	if (_ttype == LBRACE)
	{	bod = make_bodyNode();
		bod->stmt_list = stmt_list();
		_ttype = getToken();
		if (_ttype == RBRACE)
			return bod;
		else
		{	syntax_error("body. RBRACE expected", _line_no);
			exit(0); 
		}
	} else
	{	syntax_error("body. LBRACE expected", _line_no);
		exit(0); 
	}
}

struct id_listNode* id_list()
{
	struct id_listNode* idList;
	idList = make_id_listNode();
	_ttype = getToken();

	if (_ttype == ID)
	{	
		idList->id = (char*) malloc(_tokenLength+1);
		strcpy(idList->id, _token);
		
		int id_ind;
		if(_do_var_dec==1) //if we're doing variables, create variable symbol table entries. (have is_type set to false)
			id_ind = add_symbol_var(_token);
		else
			id_ind = add_symbol(_token);
		
		if(id_ind < 0)
		{
			if(_do_var_dec == 1) //if we're doing variables, check variable related rules
			{
				int varind = checksym(_token);
				if(symtable[varind]->is_type)
				{
					printf("ERROR CODE %d", 1);
					exit(0);
				}
				printf("ERROR CODE %d", 2);
				exit(0);
			}
			else
			printf("ERROR CODE %d", 0);
			exit(0);
		}
		else
		{
			symtable[id_ind]->_type_no = 1000; //I set this to 1000 because anything in an id_list is guaranteed to have a type_name, but we don't know it YET.
		}
		
		_ttype = getToken();
		if (_ttype == COMMA)
		{
			idList->id_list = id_list();
			return idList;
			
		} else
		if (_ttype == COLON)
		{	ungetToken();
			idList->id_list = NULL;
			return idList;
		} else
		{	syntax_error("id_list. COMMA or COLON expected", _line_no);
			exit(0);
		}
	} else
	{	syntax_error("id_list. ID expected", _line_no);
		exit(0);
	}

}

// looks good
struct var_declNode* var_decl()
{
	struct var_declNode* varDecl;
	varDecl = make_var_declNode();

	varDecl->id_list = id_list();

	_ttype = getToken();
	if (_ttype == SEMICOLON)
	{				
		return varDecl;
	}
	else
	{	syntax_error("var_decl. SEMICOLON expected", _line_no);
		exit(0);
	}
}
	
/*
//not needed?
//VAR_DECL_LIST
struct var_decl_listNode* var_decl_list()
{
	struct var_decl_listNode* varDeclList;
	varDeclList = make_var_decl_listNode();

	_ttype = getToken();
	if (_ttype == ID)
	{	ungetToken();
		varDeclList->var_decl = var_decl();
		_ttype = getToken();
		if (_ttype == ID)
		{	ungetToken();
			varDeclList->var_decl_list = var_decl_list();
			return varDeclList;
		}  else	
		{	ungetToken();
			return varDeclList;
		} 
	} else
	{	syntax_error("var_decl_list. ID expected", _line_no);
		exit(0);
	} 
}
//not needed?
struct var_decl_sectionNode* var_decl_section()
{
	struct var_decl_sectionNode *varDeclSection;
	varDeclSection = make_var_decl_sectionNode();

	_ttype = getToken();
	if (_ttype == VAR)
	{	// no need to ungetToken() 
		varDeclSection->var_decl_list = var_decl_list();  
		return varDeclSection;
	} else
	{	syntax_error("var_decl_section. VAR expected", _line_no);
		exit(0);
	}
}*/

// looks good
struct declNode* decl()
{	
	struct declNode* dec;
	dec = make_declNode();


	_ttype = getToken();
	if (_ttype == VAR)
		{	//var_decl section should start with VAR
		getToken();
		if(_ttype == ID)
   		{ //VAR should be followed immediately by an ID, or nothing. (no ID's)
   			ungetToken();
   			dec->var_decl_section = var_decl();
		}
	} else
	{	ungetToken();
		dec->var_decl_section = NULL;
	}
	return dec;
}

//looks good
struct programNode* program()
{	struct programNode* prog;

	prog = make_programNode();
	_ttype = getToken();
	if ((_ttype == VAR) | (_ttype == LBRACE))
	{	ungetToken();  
		prog->decl = decl();
		prog->body = body();
		return prog;
	} else
	{	syntax_error("program. TYPE or VAR or LBRACE expected", _line_no);
		exit(0);		// stop after first syntax error
	}
}

//
////
//
public int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}