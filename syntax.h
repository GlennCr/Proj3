//syntax here
//GLENN CRAVER

/*-----------------------------------------------------------
  PARSE TREE TYPES
------------------------------------------------------------*/

struct symbol
{
	char id[100];
	int ival;
};

struct programNode {
       struct declNode* decl;
       struct bodyNode* body;
};

struct declNode {
	// A NULL field means that the section is empty  
	struct var_declNode* var_decl_section;      
};

struct var_declNode {
	struct id_listNode* id_list;
	struct primaryNode* var;
};

struct id_listNode {
	char * id;
	struct id_listNode* id_list;
};

struct bodyNode {
	struct stmtNode* stmt_list;
};  

struct stmtNode {
	int stmtType;           // WHILE, ASSIGN, IF, PRINT, NOOP, GOTO
	struct stmtNode* next;
	union {
		struct while_stmtNode* while_stmt;
		struct if_stmtNode* if_stmt;
		struct assign_stmtNode* assign_stmt;
		struct print_stmtNode* print_stmt;
		//struct goto_stmtNode* goto_stmt;
	};
};

struct* stmtNode stmt_noop();
struct* stmtNode stmt_goto();


struct while_stmtNode {
	struct conditionNode* condition;
	struct bodyNode* body;
};

struct if_stmtNode {
	struct conditionNode* condition;
	struct bodyNode* body;

};

struct conditionNode {
	struct primaryNode* left_operand;
	int relop;
	struct primaryNode* right_operand;
	struct stmtNode* trueBranch;
	struct stmtNode* falseBranch;
};

struct assign_stmtNode {
	char* id;
	struct exprNode* expr; //can't be null.
};

struct print_stmtNode {
	char* id;
};

struct exprNode {	
	struct primaryNode* primary;
	int binop;	// PLUS , MINUS, MULT, OR DIV
                    // or NO-OP
    int tag;        // PRIMARY or EXPR
    //EXPR must have an operator!
	struct primaryNode * left_operand; //could be null
	struct primaryNode * right_operand; //could be null
};

struct primaryNode {
	int tag;	// NUM or ID
	int ival;
	char id[100];
};


/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct symbol*					make_symbol();
struct programNode* 			make_programNode();
struct declNode*	 			make_declNode();
struct var_declNode* 			make_var_declNode();
struct id_listNode* 			make_id_listNode();
struct bodyNode* 				make_bodyNode();
struct stmtNode* 				make_stmtNode();
struct assign_stmtNode* 		make_assign_stmtNode();
struct while_stmtNode* 			make_while_stmtNode();
struct if_stmtNode* 			make_if_stmtNode();
struct print_stmtNode* 			make_print_stmtNode();
struct exprNode* 				make_exprNode();
struct primaryNode* 			make_primaryNode();
struct condition_node*			make_condition_node();

struct 	programNode* program_node();
struct	declNode* decl();
struct	var_declNode* var_decl();
struct	id_listNode* id_list();
struct	bodyNode* body();
struct	stmtNode* stmt();
struct	assign_stmtNode* assign_stmt();
struct	while_stmtNode* while_stmt();
struct	if_stmtNode*	if_stmt();
struct 	print_stmtNode* print_stmt();

struct	primaryNode* primary();
struct 	exprNode* expr();
struct	conditionNode* condition();

void appendNode(struct stmtNode*, struct stmtNode*);
int getVal(struct primaryNode*);
void appendNoop(struct stmtNode*, struct stmtNode*);
