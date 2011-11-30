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

char _token[ MAX_TOKEN_LENGTH ];      // _token string
int  _ttype;                        // _token type
int  _activeToken = false;                  
int  _tokenLength;

int _var_count = 0;
struct symbol* _var_table[ MAX_SYMTABLE_LENGTH ] ;		//contains table of vars

int _line_no = 1;

//----------------------------------------------------------
//mine

void push_symbol(char* varID)
{
	if(_var_count < MAX_SYMTABLE_LENGTH)
	{
		int i;
		for( i = 0; (i < _var_count) & (_var_count > 0); i++ )
		{
			if( strcmp(_var_table[_var_count]->id, varID) == 0)
			{
				syntax_error("add var declaration, aka 'push_symbol()'. Tried to declare var more than once!", _line_no);
				exit(0);
			}
		}
		//did not find this var in the var table. Can add it to the var table.
		struct symbol* newvar;
		newvar = make_symbol();
		_var_table[_var_count] = newvar;

		strcpy( _var_table[_var_count]->id, varID);
		_var_table[_var_count]->ival = 0; //initial value of a var is 0.

		_var_count++;

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
void ungetToken() //did you mean ungetc()? 
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
		printf("!!Read %s while parsing PRIMARY on line %d\n", _token, _line_no);
		//primar->tag = _ttype;
		if(_ttype == NUM)
		{	primar->ival = atoi(_token);
			strcpy(primar->id, "NUM");
		}
		else if(_ttype == ID)
		{	
			strcpy(primar->id, _token);
		}
		else
		{
			syntax_error("primary(). Expected id or NUM!", _line_no);
			exit(0);
		}

	}
	
	printf("!!Done parsing PRIMARY on line %d\n", _line_no);
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
		ungetToken();
		cond->left_operand = primary();
		
		_ttype = getToken();
		if((_ttype == NOTEQUAL) | (_ttype == GREATER) | (_ttype == LESS) | (_ttype == LTEQ) | (_ttype == GTEQ) )
		{	//has relop, infers there should be a primary next.
			cond->relop = _ttype;
			cond->right_operand = primary();
					}
		else
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
		
		printf("!!Reading LOP of EXP on line %d\n", _line_no);
		expr->left_operand = primary(); //get left operand.

		_ttype = getToken();
		if ( (_ttype == PLUS) | (_ttype == MINUS) | (_ttype == MULT) | (_ttype == DIV) )
		{	
			printf("!!Read BINOP on line %d\n", _line_no);
		
			expr->binop = _ttype;

			_ttype = getToken();
			if ((_ttype == ID)| (_ttype == NUM))
			{	
				printf("!!Reading ROP of EXP on line %d\n", _line_no);
		
				expr->tag = EXPR;
				ungetToken();
				expr->right_operand = primary(); //get right operand.

				_ttype = getToken();
				if(_ttype == SEMICOLON)
				{
					return expr;
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
		} else	
		if (_ttype == SEMICOLON)
		{	expr->tag = PRIMARY;
			//ungetToken();
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

// Looks good
struct assign_stmtNode* assign_stmt()
{	struct assign_stmtNode* assignStmt;

	assignStmt = make_assign_stmtNode();
	assignStmt->id = (char *) malloc((_tokenLength+1)*sizeof(char));
	strcpy(assignStmt->id,_token);
	
	_ttype = getToken();
	if (_ttype == EQUAL)
	{	
		printf("!!Parsing rigghts hand expr on line %d\n", _line_no);
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
}

// looks good
struct if_stmtNode* if_stmt()
{
	struct if_stmtNode* if_stm;
	if_stm = make_if_stmtNode();

	if_stm->condition = condition();
	if_stm->body = body();
	if_stm->condition->trueBranch = if_stm->body->stmt_list;

}

// looks good
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
			syntax_error("stmtNode(). Expected semicolon.", _line_no);
			exit(0);
		}
	} else
	{
		syntax_error("stmtNode(). Expected ID.", _line_no);
		exit(0);
	}
	
}

// looks good
struct stmtNode* stmt()
{
	printf("!!Parsing STMT on line %d\n", _line_no);
	struct stmtNode* stm;
	stm = make_stmtNode();
	
	_ttype = getToken();
	if (_ttype == ID) // assign_stmt
	{	
		printf("!!Parsing as ASSIGN on line %d\n", _line_no);
		stm->assign_stmt = assign_stmt();
		stm->stmtType = ASSIGN;
		//_ttype = getToken();
	} else
	if (_ttype == WHILE) // while_stmt
	{	
		printf("!!Parsing as WHILE on line %d\n", _line_no);
		//struct stmtNode* gt;
		//gt = stmt_goto();
		//gt->next = stm;
		stm->while_stmt = while_stmt();
		stm->stmtType = WHILE;
	} else
	if (_ttype == IF)
	{	
		printf("!!Parsing as IF on line %d\n", _line_no);
		stm->if_stmt = if_stmt();
		stm->stmtType = IF;
	} else
	if (_ttype == PRINT)
	{	
		printf("!!Parsing as PRINT on line %d\n", _line_no);
		stm->print_stmt = print_stmt();
		stm->stmtType = PRINT;
	} else // syntax error
	{
		syntax_error("stmt. Statement expected", _line_no);
		exit(0);
	}	

	return stm;
}

struct stmtNode* stmt_noop()
{
	struct stmtNode* noop;
	noop = make_stmtNode();
	noop->stmtType = NOOP;
	noop->next = NULL;
	return noop;

}

struct stmtNode* stmt_goto()
{
	struct stmtNode* gt;
	gt = make_stmtNode();
	gt->stmtType = GOTO;
	gt->next = NULL;
	return gt;

}

//looks good
//What it Does:
//Creates a statement node, the actual node we'll keep.
//Sets this statement's next value and sets up its unions values etc.

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
		
		stL = make_stmtNode();
		stL = stmt_list();

		if(st->stmtType == IF)
		{
			printf("!!Linking IF on line %d\n", _line_no);
			struct stmtNode* noop;
			noop = stmt_noop();
			noop->next = stL;
			
			st->next = noop; //appending noop to st
			st->if_stmt->condition->falseBranch = noop; //set false branch to noop.
			appendNode(st->if_stmt->condition->trueBranch, noop); //append noop to trueBranch of condition.

			return st;
			
		} else
		if(st->stmtType == WHILE)
		{
			printf("!!Linking WHILE on line %d\n", _line_no);
			struct stmtNode* noop;
			noop = stmt_noop();
			noop->next = stL;

			struct stmtNode* gt;
			gt = stmt_goto();
			gt->next = st;

			st->next = noop; //appending noop to st
			st->if_stmt->condition->falseBranch = noop; //set false branch to noop of st.
			appendNode(st->if_stmt->condition->trueBranch, gt); //append goto to trueBranch of conditoion.

			return st;
		}
	} else
	{
		ungetToken();
		return st;
	}
	
}

// looks good
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

//looks good
struct id_listNode* id_list()
{
	printf("!!making an id list\n");
	struct id_listNode* idList;
	idList = make_id_listNode();

	_ttype = getToken();
	if (_ttype == ID)
	{	
		idList->id = (char*) malloc(_tokenLength+1);
		strcpy(idList->id, _token);
		
		push_symbol(idList->id); //will throw error on it's own.
		
		_ttype = getToken();
		if (_ttype == COMMA)
		{
			idList->id_list = id_list();
		} else
		{ //if not a comma, put it back
			ungetToken();
		}
	} else
	{	syntax_error("id_list. ID expected", _line_no);
		exit(0);
	}

	return idList;

}

// looks good
struct var_declNode* var_decl()
{
	printf("!!making a var_declNode\n");
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

// looks good
struct declNode* decl()
{	
	printf("!!Making a decl node\n");
	struct declNode* dec;
	dec = make_declNode();

	_ttype = getToken();
	printf("!!got %s\n", _token);
	if (_ttype == VAR)
		{	//var_decl section should start with VAR
		_ttype = getToken();
		printf("!!got %s\n", _token);
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
struct programNode* program_node()
{	struct programNode* prog;

	prog = make_programNode();
	_ttype = getToken();
	if ((_ttype == VAR) | (_ttype == LBRACE))
	{	ungetToken();  
		prog->decl = decl();
		printf("!!Done parsing declarations\n\n");
		prog->body = body();
		return prog;
	} else
	{	syntax_error("program. TYPE or VAR or LBRACE expected", _line_no);
		exit(0);		// stop after first syntax error
	}
}

//Appends a node to a statement list.
void appendNode(struct stmtNode* stmt_list, struct stmtNode* node)
{
	if (stmt_list->next == NULL)
	{
		stmt_list->next = node; //hit end of list, set NOOP and return.
	} else
	{
		appendNode(stmt_list->next, node); //not end of list, continue.
	}
}

struct symbol* getSymbol(char* id)
{
	//search the var table for this id.
	//return that node if found
	//error out if not found.
	int i;
	for(i = 0; i < _var_count; i++)
	{
		if(strcmp(_var_table[i]->id, id) == 0)
			return _var_table[i]; 
	}
	
	printf("%s not found in symbol table.\n", id);
	exit(0);	
}

int getVal(struct primaryNode* prim)
{
	//return the value of a primary.
	//easy if it's a num
	//needs a func call to getVar if not.
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
			if(lop < rop)
				return true;
			break;
		case LTEQ:
			if(lop <= rop)
				return true;
			break;
		case GREATER:
			if(lop > rop)
				return true;
			break;
		case GTEQ:
			if(lop >= rop)
				return true;
			break;
		case NOTEQUAL:
			if(lop != rop)
				return true;
			break;
		case EQUAL:
			if(lop == rop)
				return true;
			break;
	}
	return false;
}

void execute(struct programNode* program)
{
	struct stmtNode* pc;
	struct stmtNode* node;
	pc = program->body->stmt_list; //set program counter to first node of the program's body.

	while(pc != NULL)
	{
		
		node = pc;

		switch(pc->stmtType) 
		{
			case ASSIGN:
			{ 
							
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
							break;
						case MINUS:
							leftOp->ival = exp_lop - exp_rop;
							break;
						case MULT:
							leftOp->ival = exp_lop * exp_rop;
							break;
						case DIV:
							leftOp->ival = (int) (exp_lop / exp_rop) - ((exp_lop / exp_rop)% 1) ;
							break;
						default:
							syntax_error("execute assign. Expected operator!", _line_no);
							exit(0);
					}
				}

				pc = node->next;
					break;
			}
				
			case IF:
			{
				//code here
				struct conditionNode* condition;
				condition = node->if_stmt->condition;

				if( evalCondition(condition) )
					pc = condition->trueBranch;
				else
					pc = condition->falseBranch;
				break;
			}
			case WHILE:
			{
				struct conditionNode* condition;
				condition = node->while_stmt->condition;

				if( evalCondition(condition) )
					pc = condition->trueBranch;
				else
					pc = condition->falseBranch;
				break;
			}
			case PRINT:
			{
				//code here
				int leftOp = getSymbol(node->print_stmt->id)->ival;
				printf("%d\n", leftOp);
				pc = node->next;
				break;
			}
			case GOTO:
				pc = node->next;
				break;
			case NOOP:
				pc = node->next;
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

	execute(prog_node);
	return 0;
}
