#include "main.h"

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
	"}",
	"NOOP",
};

int printToken(int _ttype)
{ 
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

char _token[ MAX_TOKEN_LENGTH ];      // _token string
int  _ttype;                        // _token type
int  _activeToken = false;                  
int  _tokenLength;
int _depth = 0;

int _sym_count = 0;
struct symbol* _sym_table[ 100 ];//MAX_SYMTABLE_LENGTH) ] ;		//contains table of vars

int _line_no = 1;

//----------------------------------------------------------
//mine

void print_depth(int depth)
{
	int i;
	for(i = 0; i < depth; i++)
	{
		//printf("!  ");
	}
}

void push_symbol(char* varID)
{
	if(_sym_count < 100)//MAX_SYMTABLE_LENGTH)
	{
		int i;
		for( i = 0; (i < _sym_count) & (_sym_count > 0); i++ )
		{
			if( strcmp(_sym_table[i]->id, varID) == 0)
			{
				syntax_error("add var declaration, aka 'push_symbol()'. Tried to declare var more than once!", _line_no);
				exit(0);
			}
		}
		//did not find this var in the var table. Can add it to the var table.
		struct symbol* newsym;
		newsym = make_symbol();
		strcpy( newsym->id, varID);
		newsym->ival = 0; //initial value of a var is 0.

		_sym_table[_sym_count] = newsym;

		_sym_count++;

	}
	else
	{
		syntax_error("add var declaration, aka 'push_symbol()'. Input tried to declare more values than MAX_SYMTABLE_LENGTH. Try increasing this value in main.cpp", _line_no);
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
     return false;
}

/* ungetToken() simply sets a flag so that when getToken() is called
// the old _ttype is returned and the old _token is not overwritten 
// NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
// AT LEAST ONE CALL TO getToken()
// CALLING TWO ungetToken() WILL NOT UNGET TWO TOKENS 
*/
void ungetToken()
{
    _activeToken = true;
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
       { _activeToken = false;
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
---------------------------------------------------------------------

/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/

struct symbol* make_symbol()
{
	return (struct symbol*) malloc(sizeof(struct symbol));
}

struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct declNode* make_declNode()
{
	return (struct declNode*) malloc(sizeof(struct declNode));
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

// struct stmt_listNode* make_stmt_listNode()
// {
// 	return (struct stmt_listNode*) malloc(sizeof(struct stmt_listNode));
// }

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
		if(_ttype == NUM)
		{	primar->ival = atoi(_token);
			primar->tag = NUM;
			strcpy(primar->id, "NUM");			
		}
		else if(_ttype == ID)
		{	
			primar->tag = ID;
			strcpy(primar->id, _token);
		}
		return primar;
	}
	else
	{
		syntax_error("primary(). Expected id or NUM!", _line_no);
		exit(0);
	}
}

//ok in syntax.h
struct conditionNode* condition()
{	//has two primary values
	struct conditionNode* cond;
	cond = make_conditionNode();

	_ttype = getToken();
	
	if((_ttype == NUM) | (_ttype == ID))
	{		
		ungetToken();
		
		cond->left_operand = primary();

		_ttype = getToken();
		if((_ttype == NOTEQUAL) | (_ttype == GREATER) | (_ttype == LESS) | (_ttype == LTEQ) | (_ttype == GTEQ) )
		{	//has relop, infers there should be a primary next.

			cond->relop = _ttype;
			cond->right_operand = primary();
			
		} else
		{	//no relop, ungetToken for further processing.
			syntax_error("condition(). No rel op in condition!", _line_no);
			exit(0);
		}
	}

	return cond;

}

// looks good
struct exprNode* expr()
{ //SHOULD NOT BE RECURSIVE
	struct exprNode* expr;
	
	_ttype = getToken();
	if ((_ttype == ID)| (_ttype == NUM))
	{	
		expr = make_exprNode();
		
		ungetToken();
		
		expr->left_operand = primary(); //get left operand.

		_ttype = getToken();
		
		
		if ( (_ttype == PLUS) | (_ttype == MINUS) | (_ttype == MULT) | (_ttype == DIV) )
		{	
			expr->binop = _ttype;

			_ttype = getToken();
			
	
			if ((_ttype == ID)| (_ttype == NUM))
			{	
				expr->tag = EXPR;

				ungetToken();
				expr->right_operand = primary(); //get right operand.
				
				return expr; // we don't check for semi colon here. It's assign_stmt's job, not expr's
			} else
			{ //saw binop but no primary after it!
				syntax_error("expr. Primary expected for right operand!", _line_no);
				exit(0);
			}
		} else	
		{
			expr->tag = PRIMARY;
			ungetToken(); //dont remove! Needed to test if the expression ends here!
			
			return expr;
		} 
	} else
	{	//should be impossible to hit this, but just in case!
		syntax_error("expr. Primary expected for left operand!", _line_no);
		exit(0);
	}
}

// Looks good
struct assign_stmtNode* assign_stmt()
{	struct assign_stmtNode* assignStmt;

	assignStmt = make_assign_stmtNode();
	
	assignStmt->id = (char *) malloc((_tokenLength+1)*sizeof(char));
	strcpy(assignStmt->id,_token);
	
	_ttype = getToken();
	if (_ttype == EQUAL)
	{	
		
		assignStmt->expr = expr();

		_ttype = getToken();
		
		if(_ttype == SEMICOLON)
		{
			
			return assignStmt;
		} else
		{	syntax_error("assign_stmt. SEMICOLON expected", _line_no);
			exit(0);
		}
	} else
	{	syntax_error("assign_stmt. EQUAL expected", _line_no);
		exit(0);
	}
}

// looks good
struct while_stmtNode* while_stmt()
{	//has condition and a body. Condition must be evaluated 'true' to execute body.
	struct while_stmtNode* wle_stmt;
	wle_stmt = make_while_stmtNode();

	wle_stmt->condition = condition();			
	wle_stmt->body = body();

	wle_stmt->condition->trueBranch = wle_stmt->body->stmt_list;
	
	return wle_stmt;
}

struct if_stmtNode* if_stmt()
{
	struct if_stmtNode* if_stm;
	if_stm = make_if_stmtNode();

	if_stm->condition = condition();
	if_stm->body = body();

	if_stm->condition->trueBranch = if_stm->body->stmt_list;
	
	return if_stm;
}

struct print_stmtNode* print_stmt() 
{
	struct print_stmtNode* print_stm;
	print_stm = make_print_stmtNode();

	_ttype = getToken();
	if(_ttype == ID)
	{	
	
		print_stm->id = (char*) malloc(_tokenLength+1);
		strcpy(print_stm->id, _token);

		_ttype = getToken();
		if(_ttype == SEMICOLON)
		{
			return print_stm;
		} else
		{
			syntax_error("print_stmt(). Expected semicolon.", _line_no);
			exit(0);
		}
	} else
	{
		syntax_error("print_stmt(). Expected ID.", _line_no);
		exit(0);
	}
	
}

struct stmtNode* stmt()
{
	struct stmtNode* stm;
	stm = make_stmtNode();
	
	_ttype = getToken();
	
	if (_ttype == ID) // assign_stmt
	{	stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmtType, _line_no);	
		//_ttype = getToken();
	} else
	if (_ttype == WHILE) // while_stmt
	{	
		stm->stmtType = WHILE;
		//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmtType, _line_no);
		stm->while_stmt = while_stmt();
		
		// create noop
		struct stmtNode* noop;
		noop = make_stmtNode();
		noop->stmtType = NOOP;
		noop->next = NULL;
		stm->next = noop;// point stm to noop
		stm->next = noop;
		
		// create goto
		struct stmtNode* gto;
		gto = make_stmtNode();
		gto->stmtType = GOTO;
		// point goto to stm
		gto->next = stm;

		// point false branch to noop
		stm->while_stmt->condition->falseBranch = noop;

		// set the end of stmt_list(true branch) of while_stmt to point to goto
		struct stmtNode* while_stmtlist = stm->while_stmt->condition->trueBranch;
		//print_depth(_depth);printf("Setting trueBranch stmtlist to NOOP\n");
		while((while_stmtlist->next != NULL))
		{
			//print_depth(_depth);//printf("!Next stmt is %d\n", while_stmtlist->stmtType);
			while_stmtlist = while_stmtlist->next;
		}
		//print_depth(_depth);//printf("!Next stmt is %d, it's next is NULL\n", while_stmtlist->stmtType);
		while_stmtlist->next = gto;
		//print_depth(_depth);//printf("!set NULL to GOTO\n");

	} else
	if (_ttype == IF)
	{	
		stm->stmtType = IF;
		//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmtType, _line_no);
		stm->if_stmt = if_stmt();
		
		// if stmt ->next, set to noop
		struct stmtNode* noop;
		noop = make_stmtNode();
		noop->stmtType = NOOP;
		noop->next = NULL;

		stm->next = noop;
		
		// set false branch of if_stmts false branch to noop
		stm->if_stmt->condition->falseBranch = noop;
		// set the end of stmt_list(true branch) of if_stmt to point to noop
		struct stmtNode* if_stmtlist = stm->if_stmt->condition->trueBranch;

		//print_depth(_depth);//printf("!Setting trueBranch stmtlist to NOOP\n");
		while((if_stmtlist->next != NULL))
		{
			//print_depth(_depth);printf("Next stmt is %d\n", if_stmtlist->stmtType);
			if_stmtlist = if_stmtlist->next;
		}

		//print_depth(_depth);//printf("!Next stmt is %d, it's next is NULL\n", if_stmtlist->stmtType);
		if_stmtlist->next = noop;
		//print_depth(_depth);//printf("!set NULL to NOOP\n");

	} else
	if (_ttype == PRINT)
	{	
		stm->print_stmt = print_stmt();
		stm->stmtType = PRINT;
		//print_depth(_depth); //printf("!!stmt is %d at line%d\n", stm->stmtType, _line_no);
	} else // syntax error
	{
		syntax_error("stmt. Statement expected", _line_no);
		exit(0);
	}	

	return stm;
}

struct stmtNode* stmt_list()
{	
	
	struct stmtNode* st;
	struct stmtNode* stL;

	st = make_stmtNode();
	st->next = NULL;
	
	st = stmt();

	_ttype = getToken();
	if ((_ttype == ID)|(_ttype == WHILE) | (_ttype == IF) | (_ttype == PRINT))
	{	ungetToken();
		_depth++;	

		stL = make_stmtNode();
		stL->next = NULL;
		stL = stmt_list();

		if((st->stmtType == WHILE) | (st->stmtType == IF))
		{	
			st->next->next = stL; //setting stL in to st's noop
			//print_depth(_depth); //printf("!IFs noop is set to stmt_list %d\n", stL->stmtType);
		} else
		{
			//print_depth(_depth); //printf("!stmt %d next is set to stmt_list %d\n", st->stmtType, stL->stmtType);
			st->next = stL;	//non-condition statements do not have a noop. Just set it's next.
		}
		_depth--;

		//ungetToken();
		return st;	
	} else
	{
		
		//print_depth(_depth); //printf("!stmt %d next is set to NULL\n", st->stmtType);
		_depth--;
		ungetToken();
		return st;
	}
}

// looks good
struct bodyNode* body()
{	struct bodyNode* bod;

	_ttype = getToken();
	if (_ttype == LBRACE)
	{	
		//print_depth(_depth); //printf("!{{\n");
		
		bod = make_bodyNode();
		_depth++;
		bod->stmt_list = stmt_list();
		
		_ttype = getToken();
		
		
		if (_ttype == RBRACE)
		{	
			//print_depth(_depth); //printf("!}}\n");
			_depth--;
			//print_depth(_depth); //printf("!Returning bod\n");
			
			return bod;
		}
		else
		{	syntax_error("body. RBRACE expected", _line_no);
			exit(0); 
		}
	} else
	{	syntax_error("body. LBRACE expected", _line_no);
		exit(0); 
	}
}

//looks good
struct id_listNode* id_list()
{
	
	struct id_listNode* idLst;
	idLst = make_id_listNode();

	_ttype = getToken();
	if (_ttype == ID)
	{	
		idLst->id = (char*) malloc(_tokenLength+1);
		strcpy(idLst->id, _token);

		
			push_symbol(idLst->id); //will throw error on it's own.
		
		_ttype = getToken();
		if (_ttype == COMMA)
		{
			
			idLst->id_list = id_list();
		} else
		{ //if not a comma, put it back
			ungetToken();
		}
	} else
	{	syntax_error("id_list. ID expected", _line_no);
		exit(0);
	}

	
	return idLst;

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

struct declNode* decl()
{	
	
	struct declNode* dec;
	dec = make_declNode();

	_ttype = getToken();
	if (_ttype == VAR)
	{	//var_decl section should start with VAR
		
		_ttype = getToken();
		if(_ttype == ID)
   		{ //VAR should be followed immediately by an ID, or nothing. (no ID's)
   			ungetToken();
   			dec->var_decl_section = var_decl();
		}
		else
		{
			syntax_error("decl(). No ID found after VAR section token.", _line_no);
		}
	} else
	{	ungetToken();
		dec->var_decl_section = NULL;
	}
	return dec;
}

//looks good
struct programNode* program_node()
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

	//search the var table for this id.
	//return that node if found
	//error out if not found.
struct symbol* getSymbol(char* id)
{
	
	int i;
	for(i = 0; i < _sym_count; i++)
	{
		if(strcmp(_sym_table[i]->id, id) == 0)
		{
			
			return _sym_table[i]; 
		}
	}
	
	//printf("#%s not found in symbol table.\n", id);
	exit(0);
}

	//return the value of a primary.
	//easy if it's a num
	//needs a func call to getVar if not.
int getVal(struct primaryNode* prim)
{
	if(prim->tag == NUM)
	{
		
			
		return prim->ival;
	} else
	{
		struct symbol* out;
			
		out = getSymbol(prim->id);
		return out->ival;

	}
	return 0;
}

bool evalCondition(struct conditionNode* cond)
{
	int lop, rop;

	lop = getVal(cond->left_operand);
	rop = getVal(cond->right_operand);

	switch(cond->relop)
	{
		case LESS:	
			
			//printf("# ( %s < %s ", cond->left_operand->id, cond->right_operand->id);
			if(lop < rop)
			{	//printf("# is true\n");
				return true;
			}
			break;
		case LTEQ:
		
			//printf("# ( %s <= %s ", cond->left_operand->id, cond->right_operand->id);
			if(lop <= rop)
			{	//printf("# is true\n");
				return true;
			}
			break;
		case GREATER:
		
			//printf("# ( %s > %s ) ", cond->left_operand->id, cond->right_operand->id);
			if(lop > rop)
			{
				//printf("# is true\n");
				return true;
			}
			break;
		case GTEQ:
		
			//printf("# ( %s >= %s ", cond->left_operand->id, cond->right_operand->id);
			if(lop >= rop)
			{	//printf("# is true\n");
				return true;
			}
			break;
		case NOTEQUAL:
		
			//printf("# ( %s != %s ", cond->left_operand->id, cond->right_operand->id);
			if(lop != rop)
			{
				//printf("# is true\n");
				return true;
			}
			break;
		case EQUAL:
		
			//printf("#%s == %s ", cond->left_operand->id, cond->right_operand->id);
			if(lop == rop)
			{	//printf("# is true\n");
				return true;
			}
			break;
	}

	//printf("# is false\n");
	return false;
}

void execute(struct programNode* program)
{
	//printf("#starting execution\n");
	struct stmtNode* pc;
	struct stmtNode* node;

	pc = program->body->stmt_list; //set program counter to first node of the program's body.
	//printf("#Got top stmt_list\n");
	while(pc != NULL)
	{
		//printf("#Looping\n");
		node = pc;

		switch(pc->stmtType) 
		{
			case ASSIGN:
			{ 
				//printf("#Executing ASSIGN - ");
				
				struct assign_stmtNode* assign_stmt;
				assign_stmt = pc->assign_stmt;
				
				struct symbol* leftOp;
				leftOp = getSymbol(assign_stmt->id); //get value to assign
				
				struct exprNode* expr;
				expr = assign_stmt->expr;

				if(expr->tag == PRIMARY)
				{
					int valta = getVal(expr->left_operand);
					
					leftOp->ival = valta;
					//printf("#%s set to %d", leftOp->id, leftOp->ival);
							
				} else
				if(expr->tag == EXPR)
				{
					int exp_lop = getVal(expr->left_operand);
					int exp_rop = getVal(expr->right_operand);
					int exp_binop = expr->binop;
						
					switch(exp_binop)
					{
						case PLUS:
							leftOp->ival = exp_lop + exp_rop;
							//printf("#%s set to %d", leftOp->id, leftOp->ival);
							
							break;
						case MINUS:
							leftOp->ival = exp_lop - exp_rop;
							//printf("#%s set to %d", leftOp->id, leftOp->ival);
							
							break;
						case MULT:
							leftOp->ival = exp_lop * exp_rop;
							//printf("#%s set to %d", leftOp->id, leftOp->ival);
							
							break;
						case DIV:
							leftOp->ival = (int) (exp_lop / exp_rop) - ((exp_lop / exp_rop)% 1) ;
							//printf("#%s set to %d", leftOp->id, leftOp->ival);
							
							break;
						default:
							syntax_error("execute assign. Expected operator!\n", _line_no);
							exit(0);
					}
				}
					
				pc = node->next;
				
			}
				break;
			case IF:
			{
				//printf("#Executing IF\n");
				struct conditionNode* condition;
				condition = node->if_stmt->condition;
				
				//printf("#checking condition\n");
				if( evalCondition(condition) )
				{
					pc = condition->trueBranch;
				}
				else
				{
					pc = condition->falseBranch;
				}
				break;
			}
			case WHILE:
			{
				//printf("#Executing WHILE\n");
				struct conditionNode* condition;
				condition = node->while_stmt->condition;

				if( evalCondition(condition) )
				{
					pc = condition->trueBranch;
				}
				else
				{
					pc = condition->falseBranch;
				}
				break;
			}
			case PRINT:
			{
				//printf("#Executing PRINT\n");
				int leftOp = getSymbol(node->print_stmt->id)->ival;
				
				printf("%d\n", leftOp);
				pc = node->next;
			
				break;
			}
			case GOTO:
				//printf("#hit GOTO\n");
				pc = node->next;
				break;
			case NOOP:
				//printf("#hit NOOP\n");
				pc = node->next;
				break;

			default:
				//printf("#Nothing to run!");
				break;
		}

	}
}

//
////
//
int main(int argc, char const *argv[])
{
	struct programNode* prog_node;
	prog_node = program_node();
	//printf("!Program Node complete.\n");

	execute(prog_node);
	return 0;
}
