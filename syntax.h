//syntax here
//GLENN CRAVER

/*-----------------------------------------------------------
  PARSE TREE TYPES
------------------------------------------------------------*/

struct symbol_entry
{   bool is_type;
	int type_no;
	char symname[100]; //hurgh, this does not sit well with me....
	int can_print;
};

struct programNode {
       struct declNode* decl;
       struct bodyNode* body;
};

struct declNode {
	// A NULL field means that the section is empty  
	struct var_decl_sectionNode* var_decl_section;      
};

// not needed?
/*struct var_decl_sectionNode {
	struct var_decl_listNode* var_decl_list;          
};
// not needed?
struct  var_decl_listNode {
	struct var_declNode * var_decl;
        struct var_decl_listNode* var_decl_list;
};
*/
struct var_declNode {
	struct id_listNode* id_list;
	struct primaryNode* var;
};

struct id_listNode {
	char * id;
	struct id_listNode* id_list;
};

struct bodyNode {
	struct stmt_listNode* stmt_list;
};

struct stmt_listNode {
	struct stmtNode* stmt;
	struct stmt_listNode * stmt_list;
};

struct stmtNode {
	int stmtType;           // WHILE, ASSIGN, IF, PRINT
	union {
		struct while_stmtNode* while_stmt;
		struct if_stmtNode* assign_stmt;
		struct assign_stmtNode* assign_stmt;
		struct print_stmtNode* assign_stmt;
	};
};     

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
	int operator;	// PLUS , MINUS, MULT, OR DIV
                    // or NO-OP
    int tag;        // PRIMARY or EXPR
    //EXPR must have an operator!
	struct primaryNode * leftOperand; //could be null
	struct primaryNode * rightOperand; //could be null
};

struct primaryNode {
	int tag;	// NUM or ID
	int type_no;	//>=10
	int ival;
	char id[100];
};


/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* 			make_programNode();
struct declNode* 				make_declNode();
struct var_decl_sectionNode* 	make_var_decl_sectionNode();
struct var_decl_listNode* 		make_var_decl_listNode();
struct var_declNode* 			make_var_declNode();
struct id_listNode* 			make_id_listNode();

struct bodyNode* 				make_bodyNode();
struct stmt_listNode* 			make_stmt_listNode();
struct stmtNode* 				make_stmtNode();
struct while_stmtNode* 			make_while_stmtNode();
struct if_stmtNode* 			make_if_stmtNode();
struct assign_stmtNode* 		make_assign_stmtNode();
struct print_stmtNode* 			make_print_stmtNode();
struct condition_node*			make_condition_node();
struct exprNode* 				make_exprNode();
struct primaryNode* 			make_primaryNode();

/*------------------------------------------------------------------------
  PARSING FUNCTIONS
--------------------------------------------------------------------------*/
//struct 
struct programNode* program();
struct	declNode*  decl();
struct	var_decl_sectionNode* var_decl_section();
struct		var_decl_listNode* var_decl_list();
struct			var_declNode* var_decl();
struct					id_listNode* id_list();
struct	bodyNode* body();
struct		stmt_listNode* stmt_list();
struct			stmtNode* stmt();
struct				//stmt_listNode* stmt_list();
struct				assign_stmtNode* assign_stmt();
struct				while_stmtNode* while_stmt();
struct				if_stmtNode*	if_stmt();
struct					conditionNode* condition();
struct					exprNode* expr();
struct					primaryNode* primary();
