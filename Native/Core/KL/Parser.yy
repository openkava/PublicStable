%define api.pure
%locations
%lex-param { KL::Context &ctx }
%parse-param { KL::Context &ctx }
%name-prefix="kl_"
%defines
%error-verbose
%debug

// We're expecing one shift/reduce conflicts for the if/else clause.
%expect 1


%code top {
#define YYDEBUG 1
#define RTLOC (CG::Location( yyloc.first_line, yyloc.first_column ))
}

%code requires {
#include <string>
#include <Fabric/Core/AST/Alias.h>
#include <Fabric/Core/AST/AndOp.h>
#include <Fabric/Core/AST/ArgList.h>
#include <Fabric/Core/AST/AssignedVarDecl.h>
#include <Fabric/Core/AST/AssignOp.h>
#include <Fabric/Core/AST/AssignOpImpl.h>
#include <Fabric/Core/AST/BinOp.h>
#include <Fabric/Core/AST/BinOpImpl.h>
#include <Fabric/Core/AST/BreakStatement.h>
#include <Fabric/Core/AST/Call.h>
#include <Fabric/Core/AST/Case.h>
#include <Fabric/Core/AST/CaseList.h>
#include <Fabric/Core/AST/CastNode.h>
#include <Fabric/Core/AST/CompoundStatement.h>
#include <Fabric/Core/AST/ConditionalStatement.h>
#include <Fabric/Core/AST/ConstBoolean.h>
#include <Fabric/Core/AST/ConstDecl.h>
#include <Fabric/Core/AST/ConstDeclStatement.h>
#include <Fabric/Core/AST/ConstSize.h>
#include <Fabric/Core/AST/ConstSizeDecl.h>
#include <Fabric/Core/AST/ConstScalar.h>
#include <Fabric/Core/AST/ConstScalarDecl.h>
#include <Fabric/Core/AST/ConstString.h>
#include <Fabric/Core/AST/ContinueStatement.h>
#include <Fabric/Core/AST/CStyleLoop.h>
#include <Fabric/Core/AST/ExprStatement.h>
#include <Fabric/Core/AST/Function.h>
#include <Fabric/Core/AST/GlobalConstDecl.h>
#include <Fabric/Core/AST/GlobalList.h>
#include <Fabric/Core/AST/IndexOp.h>
#include <Fabric/Core/AST/InitializedVarDecl.h>
#include <Fabric/Core/AST/Initializer.h>
#include <Fabric/Core/AST/MethodOp.h>
#include <Fabric/Core/AST/MethodOpImpl.h>
#include <Fabric/Core/AST/NotOp.h>
#include <Fabric/Core/AST/Operator.h>
#include <Fabric/Core/AST/OrOp.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/AST/ParamList.h>
#include <Fabric/Core/AST/Report.h>
#include <Fabric/Core/AST/ReturnStatement.h>
#include <Fabric/Core/AST/StatementList.h>
#include <Fabric/Core/AST/StructDecl.h>
#include <Fabric/Core/AST/StructDeclMember.h>
#include <Fabric/Core/AST/StructDeclMemberList.h>
#include <Fabric/Core/AST/StructMemberOp.h>
#include <Fabric/Core/AST/SwitchStatement.h>
#include <Fabric/Core/AST/TempValue.h>
#include <Fabric/Core/AST/TernaryOp.h>
#include <Fabric/Core/AST/Var.h>
#include <Fabric/Core/AST/VarDecl.h>
#include <Fabric/Core/AST/UniOp.h>
#include <Fabric/Core/KL/Debug.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Core/CG/BooleanAdapter.h>
#include <Fabric/Core/CG/ByteAdapter.h>
#include <Fabric/Core/CG/IntegerAdapter.h>
#include <Fabric/Core/CG/ScalarAdapter.h>
#include <Fabric/Core/CG/StringAdapter.h>
#include <Fabric/Core/CG/StructAdapter.h>
#include <Fabric/Core/CG/FixedArrayAdapter.h>
#include <Fabric/Core/CG/VariableArrayAdapter.h>
using namespace Fabric;

}

%{
#include "Parser.hpp"
#include <Fabric/Core/KL/Context.h>
#include <Fabric/Core/KL/Debug.h>
#include <Fabric/Core/KL/Location.h>
#include <Fabric/Core/KL/Source.h>

#if defined(FABRIC_OS_WINDOWS)
#pragma warning( disable: 4065 )
#endif

void kl_error( YYLTYPE *yyl, KL::Context &ctx, const char *msg );
int kl_lex( YYSTYPE *yys, YYLTYPE *yyl, KL::Context &ctx );
%}

%union { std::string const *valueStringPtr; }
%destructor { delete $$; } <valueStringPtr>

%union { CG::Adapter const *cgAdapter; }
%destructor { } <cgAdapter>

%union { CG::UniOpType uniOpType; }
%destructor { } <uniOpType>

%union { CG::BinOpType binOpType; }
%destructor { } <binOpType>

%union { CG::AssignOpType assignOpType; }
%destructor { } <assignOpType>

%union { Fabric::AST::Param *astParamPtr; }
%union { Fabric::AST::ParamList *astParamListPtr; }
%union { Fabric::AST::Global *astGlobalPtr; }
%union { Fabric::AST::GlobalList *astGlobalListPtr; }
%union { Fabric::AST::StructDecl *astStructDecl; }
%union { Fabric::AST::StructDeclMember *astStructDeclMember; }
%union { Fabric::AST::StructDeclMemberList *astStructDeclMemberList; }
%union { Fabric::AST::Statement *astStatementPtr; }
%union { Fabric::AST::StatementList *astStatementListPtr; }
%union { Fabric::AST::CompoundStatement *astCompoundStatementPtr; }
%union { Fabric::AST::ArgList *astArgListPtr; }
%union { Fabric::AST::Expr *astExprPtr; }
%union { Fabric::AST::ConstDecl *astConstDeclPtr; }
%union { Fabric::AST::Case *astCasePtr; }
%union { Fabric::AST::CaseList *astCaseListPtr; }
%destructor { $$->release(); } <*>

%token TOKEN_END 0 "end of file"

%token <valueStringPtr> TK_WHITESPACE "whitespace"

%token <valueStringPtr> TK_COMMENT_BLOCK "block comment"
%token <valueStringPtr> TK_COMMENT_LINE "line comment"

%token TK_IF "if"
%token TK_IN "in"
%token TK_IO "io"
%token TK_DO "do"
%token TK_FOR "for"
%token TK_VAR "var"
%token TK_CASE "case"
%token TK_ELSE "else"
%token TK_TRUE "true"
%token TK_ALIAS "alias"
%token TK_BREAK "break"
%token TK_CONST "const"
%token TK_FALSE "false"
%token TK_WHILE "while"
%token TK_REPORT "report"
%token TK_RETURN "return"
%token TK_STRUCT "struct"
%token TK_SWITCH "switch"
%token TK_DEFAULT "default"
%token TK_CONTINUE "continue"
%token TK_FUNCTION "function"
%token TK_OPERATOR "operator"

%token TK_LBRACE "{"
%token TK_RBRACE "}"
%token TK_LPAREN "("
%token TK_RPAREN ")"
%token TK_LBRACKET "["
%token TK_RBRACKET "]"
%token TK_COMMA ","
%token TK_SEMICOLON ";"
%token TK_DOT "."
%token TK_COLON ":"
%token TK_QUESTION_MARK "?"

%token TK_PLUS "+"
%token TK_PLUS_PLUS "++"
%token TK_MINUS "-"
%token TK_MINUS_MINUS "--"

%token TK_AST "*"
%token TK_FSLASH "/"
%token TK_PERCENT "%"

%token TK_EXCL "!"
%token TK_AMP_AMP "&&"
%token TK_PIPE_PIPE "||"
%token TK_CARET_CARET "^^"

%token TK_TILDE "~"
%token TK_AMP "&"
%token TK_PIPE "|"
%token TK_CARET "^"

%token TK_EQUALS "="
%token TK_PLUS_EQUALS "+="
%token TK_MINUS_EQUALS "-="
%token TK_AST_EQUALS "*="
%token TK_FSLASH_EQUALS "/="
%token TK_PERCENT_EQUALS "%="
%token TK_AMP_AMP_EQUALS "&&="
%token TK_PIPE_PIPE_EQUALS "||="
%token TK_CARET_CARET_EQUALS "^^="
%token TK_AMP_EQUALS "&="
%token TK_PIPE_EQUALS "|="
%token TK_CARET_EQUALS "^="

%token TK_EQUALS_EQUALS "=="
%token TK_EXCL_EQUALS "!="
%token TK_LANGLE "<"
%token TK_LANGLE_EQUALS "<="
%token TK_RANGLE ">"
%token TK_RANGLE_EQUALS ">="

%token <valueStringPtr> TK_CONST_UI "integer constant"
%token <valueStringPtr> TK_CONST_FP "floating point constant"
%token <valueStringPtr> TK_CONST_STRING_SQUOT "single-quoted string constant"
%token <valueStringPtr> TK_CONST_STRING_DQUOT "double-quoted string constant"

%token <cgAdapter> TK_REGISTERED_TYPE "registered type"
%token <valueStringPtr> TK_IDENTIFIER "identifier"

%type <astParamPtr> parameter
%type <astParamPtr> input_parameter
%type <astParamListPtr> parameter_list
%type <astGlobalPtr> global
%type <astGlobalPtr> function
%type <astGlobalPtr> prototype
%type <astGlobalPtr> alias
%type <astGlobalPtr> struct
%type <astGlobalPtr> global_const_decl
%type <astGlobalListPtr> global_list
%type <astStructDeclMember> struct_member
%type <astStructDeclMemberList> struct_member_list
%type <astConstDeclPtr> const_decl
%type <astStatementPtr> const_decl_statement
%type <astStatementPtr> statement
%type <astStatementPtr> loop_start_statement
%type <cgAdapter> array_modifier
%type <astStatementPtr> var_decl_statement
%type <astStatementPtr> var_decl
%type <astStatementPtr> loop_statement
%type <astStatementPtr> switch_statement
%type <astCasePtr> case
%type <astCaseListPtr> case_list
%type <astStatementListPtr> var_decl_list
%type <astStatementListPtr> statement_list
%type <astCompoundStatementPtr> compound_statement
%type <astArgListPtr> argument_expression_list
%type <cgAdapter> compound_type "type"

%type <uniOpType> prefix_unary_operator
%type <uniOpType> postfix_unary_operator
%type <binOpType> binary_operator
%type <binOpType> additive_operator
%type <binOpType> multiplicative_operator
%type <binOpType> equality_operator
%type <binOpType> relational_operator
%type <assignOpType> assignment_operator

//%type <astExprPtr> constant_expression
%type <astExprPtr> optional_expression
%type <astExprPtr> expression
%type <astExprPtr> assignment_expression
%type <astExprPtr> conditional_expression
%type <astExprPtr> logical_or_expression
%type <astExprPtr> logical_and_expression
%type <astExprPtr> inclusive_or_expression
%type <astExprPtr> exclusive_or_expression
%type <astExprPtr> and_expression
%type <astExprPtr> equality_expression
%type <astExprPtr> relational_expression
%type <astExprPtr> shift_expression
%type <astExprPtr> additive_expression
%type <astExprPtr> multiplicative_expression
%type <astExprPtr> cast_expression
%type <astExprPtr> prefix_unary_expression
%type <astExprPtr> postfix_expression
%type <astExprPtr> primary_expression

%%

start
  : global_list
  {
    ctx.m_resultGlobalList = $1;
    $1->release();
  }
;

global_list :
  global global_list
  {
    $$ = AST::GlobalList::Create( RTLOC, $1, $2 ).take();
    $1->release();
    $2->release();
  }
  | /* empty */
  {
    $$ = AST::GlobalList::Create( RTLOC ).take();
  }

binary_operator
  : TK_PLUS
  {
    $$ = CG::BIN_OP_ADD;
  }
  | TK_MINUS
  {
    $$ = CG::BIN_OP_SUB;
  }
  | TK_AST
  {
    $$ = CG::BIN_OP_MUL;
  }
  | TK_FSLASH
  {
    $$ = CG::BIN_OP_DIV;
  }
  | TK_PERCENT
  {
    $$ = CG::BIN_OP_MOD;
  }
;

global_const_decl
  : const_decl
  {
    $$ = AST::GlobalConstDecl::Create( RTLOC, $1 ).take();
    $1->release();
  }
;

global
  : function
  {
    $$ = $1;
  }
  | prototype
  {
    $$ = $1;
  }
  | alias
  {
    $$ = $1;
  }
  | struct
  {
    $$ = $1;
  }
  | global_const_decl
  {
    $$ = $1;
  }
;

function
  : TK_FUNCTION compound_type TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::Function::Create( RTLOC, *$3, CG::ExprType( $2, CG::USAGE_RVALUE ), $5, $7 ).take();
    $2->release();
    delete $3;
    $5->release();
    $7->release();
  }
  | TK_FUNCTION TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::Function::Create( RTLOC, *$2, CG::ExprType(), $4, $6 ).take();
    delete $2;
    $4->release();
    $6->release();
  }
  | TK_OPERATOR TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::Operator::Create( RTLOC, *$2, $4, $6 ).take();
    delete $2;
    $4->release();
    $6->release();
  }
  | TK_FUNCTION compound_type compound_type TK_DOT TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::MethodOpImpl::Create( RTLOC, CG::ExprType( $2, CG::USAGE_RVALUE ), $3, *$5, $7, $9 ).take();
    $2->release();
    $3->release();
    delete $5;
    $7->release();
    $9->release();
  }
  | TK_FUNCTION compound_type TK_DOT TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::MethodOpImpl::Create( RTLOC, CG::ExprType(), $2, *$4, $6, $8 ).take();
    $2->release();
    delete $4;
    $6->release();
    $8->release();
  }
  | TK_FUNCTION compound_type binary_operator TK_LPAREN input_parameter TK_COMMA input_parameter TK_RPAREN compound_statement
  {
    $$ = AST::BinOpImpl::Create( RTLOC, CG::ExprType( $2, CG::USAGE_RVALUE ), $3, $5, $7, $9 ).take();
    $2->release();
    $5->release();
    $7->release();
    $9->release();
  }
  | TK_FUNCTION compound_type TK_DOT assignment_operator TK_LPAREN input_parameter TK_RPAREN compound_statement
  {
    $$ = AST::AssignOpImpl::Create( RTLOC, $2, $4, $6, $8 ).take();
    $2->release();
    $6->release();
    $8->release();
  }
  | TK_FUNCTION compound_type TK_LPAREN parameter_list TK_RPAREN compound_statement
  {
    $$ = AST::Initializer::Create( RTLOC, $2, $4, $6 ).take();
    $2->release();
    $4->release();
    $6->release();
  }
;

prototype
  : TK_FUNCTION compound_type TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN TK_SEMICOLON
  {
    $$ = AST::Function::Create( RTLOC, *$3, CG::ExprType( $2, CG::USAGE_RVALUE ), $5, 0 ).take();
    $2->release();
    delete $3;
    $5->release();
  }
  | TK_FUNCTION TK_IDENTIFIER TK_LPAREN parameter_list TK_RPAREN TK_SEMICOLON
  {
    $$ = AST::Function::Create( RTLOC, *$2, CG::ExprType(), $4, 0 ).take();
    delete $2;
    $4->release();
  }

alias
  : TK_ALIAS compound_type
      {
        ctx.m_varType = $2;
      }
    TK_IDENTIFIER array_modifier TK_SEMICOLON
      {
        ctx.m_cgManager->registerAlias( *$4, $5 );
        $$ = AST::Alias::Create( RTLOC, *$4, $5 ).take();
        delete $4;
        $5->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }
  | TK_ALIAS compound_type
      {
        ctx.m_varType = $2;
      }
    TK_REGISTERED_TYPE array_modifier TK_SEMICOLON
      {
        try
        {
          ctx.m_cgManager->registerAlias( $4->getUserName(), $5 );
        }
        catch ( Exception e )
        {
          ctx.m_diagnostics.addError( RTLOC, e.getDesc() );
        }
        $$ = AST::Alias::Create( RTLOC, $4->getUserName(), $5 ).take();
        $4->release();
        $5->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }

;

struct
  : TK_STRUCT TK_IDENTIFIER TK_LBRACE struct_member_list TK_RBRACE TK_SEMICOLON
  {
    RT::StructMemberInfoVector structMemberInfoVector;
    $4->appenedToStructMemberInfoVector( structMemberInfoVector );
    ctx.m_cgManager->registerStruct( *$2, structMemberInfoVector );
    
    $$ = AST::StructDecl::Create( RTLOC, *$2, $4 ).take();
    delete $2;
    $4->release();
  }
  | TK_STRUCT TK_REGISTERED_TYPE TK_LBRACE struct_member_list TK_RBRACE TK_SEMICOLON
  {
    if( !RT::isStruct( $2->getType() ) )
    {
      ctx.m_diagnostics.addError( RTLOC, "Redefining non-struct as struct" );
    }
    else
    {
      RT::StructMemberInfoVector structMemberInfoVector;
      $4->appenedToStructMemberInfoVector( structMemberInfoVector );
      try
      {
        // This will fail.
        ctx.m_cgManager->registerStruct( $2->getUserName(), structMemberInfoVector );
      }
      catch ( Exception e )
      {
        ctx.m_diagnostics.addError( RTLOC, e.getDesc() );
      }
    }
    
    $$ = AST::StructDecl::Create( RTLOC, $2->getUserName(), $4 ).take();
    $2->release();
    $4->release();
  }
;

struct_member_list
  : struct_member struct_member_list
  {
    $$ = AST::StructDeclMemberList::Create( RTLOC, $1, $2 ).take();
    $1->release();
    $2->release();
  }
  | /* empty */
  {
    $$ = AST::StructDeclMemberList::Create( RTLOC ).take();
  }
;

struct_member
  : compound_type
      {
        ctx.m_varType = $1;
      }
    TK_IDENTIFIER array_modifier TK_SEMICOLON
      {
        $$ = AST::StructDeclMember::Create( RTLOC, *$3, $4 ).take();
        delete $3;
        $4->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }
;

parameter_list
  : /* empty */
  {
    $$ = AST::ParamList::Create( RTLOC ).take();
  }
  | parameter
  {
    $$ = AST::ParamList::Create( RTLOC, $1 ).take();
    $1->release();
  }
  | parameter TK_COMMA parameter_list
  {
    $$ = AST::ParamList::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

const_decl
  : TK_CONST compound_type TK_IDENTIFIER TK_EQUALS TK_CONST_UI TK_SEMICOLON
  {
    $$ = AST::ConstSizeDecl::Create( RTLOC, *$3, $2, *$5 ).take();
    $2->release();
    delete $3;
    delete $5;
  }
  | TK_CONST compound_type TK_IDENTIFIER TK_EQUALS TK_CONST_FP TK_SEMICOLON
  {
    $$ = AST::ConstScalarDecl::Create( RTLOC, *$3, $2, *$5 ).take();
    $2->release();
    delete $3;
    delete $5;
  }
;

const_decl_statement
  : const_decl
  {
    $$ = AST::ConstDeclStatement::Create( RTLOC, $1 ).take();
    $1->release();
  }
;

array_modifier
  : TK_LBRACKET TK_RBRACKET array_modifier
  {
    $$ = ctx.m_cgManager->getVariableArrayOf( $3 ).take();
    $3->release();
  }
  | TK_LBRACKET TK_CONST_UI TK_RBRACKET array_modifier
  {
    unsigned length;
    sscanf( $2->c_str(), "%u", &length );
    delete $2;

    if ( length == 0 )
    {
      $4->release();
      ctx.m_varType->release();
      ctx.m_varType = 0;
      yyerror( &yyloc, ctx, "fixed array size must be greater than zero" );
      YYERROR;
    }
    else
    {
      $$ = ctx.m_cgManager->getFixedArrayOf( $4, length ).take();
      $4->release();
    }
  }
  | /* empty */
  {
    $$ = ctx.m_varType;
    $$->retain();
  }
;

var_decl
  : TK_IDENTIFIER array_modifier TK_EQUALS assignment_expression
  {
    $$ = AST::AssignedVarDecl::Create( RTLOC, *$1, $2, $4 ).take();
    delete $1;
    $2->release();
    $4->release();
  }
  | TK_IDENTIFIER array_modifier TK_LPAREN argument_expression_list TK_RPAREN
  {
    $$ = AST::InitializedVarDecl::Create( RTLOC, *$1, $2, $4 ).take();
    delete $1;
    $2->release();
    $4->release();
  }
  | TK_IDENTIFIER array_modifier 
  {
    $$ = AST::VarDecl::Create( RTLOC, *$1, $2 ).take();
    delete $1;
    $2->release();
  }
;

var_decl_list
  : var_decl
  {
    $$ = AST::StatementList::Create( RTLOC, $1 ).take();
    $1->release();
  }
  | var_decl TK_COMMA var_decl_list
  {
    $$ = AST::StatementList::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
  ;

input_parameter
  : compound_type
      {
        ctx.m_varType = $1;
      }
    TK_IDENTIFIER array_modifier
      {
        $$ = AST::Param::Create( RTLOC, *$3, $4, CG::USAGE_RVALUE ).take();
        delete $3;
        $4->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }
  | TK_IN compound_type
      {
        ctx.m_varType = $2;
      }
    TK_IDENTIFIER array_modifier
      {
        $$ = AST::Param::Create( RTLOC, *$4, $5, CG::USAGE_RVALUE ).take();
        delete $4;
        $5->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }
;

parameter
  : input_parameter
  {
    $$ = $1;
  }
  | TK_IO compound_type
      {
        ctx.m_varType = $2;
      }
    TK_IDENTIFIER array_modifier
      {
        $$ = AST::Param::Create( RTLOC, *$4, $5, CG::USAGE_LVALUE ).take();
        delete $4;
        $5->release();
        ctx.m_varType->release();
        ctx.m_varType = 0;
      }
;

compound_statement
  : TK_LBRACE statement_list TK_RBRACE
  {
    $$ = AST::CompoundStatement::Create( RTLOC, $2 ).take();
    $2->release();
  }
;

statement_list
  : statement statement_list
  {
    if ( $1 )
    {
      $$ = AST::StatementList::Create( RTLOC, $1, $2 ).take();
      $1->release();
      $2->release();
    }
    else $$ = $2;
  }
  | /* empty */
  {
    $$ = AST::StatementList::Create( RTLOC ).take();
  }

compound_type
  : TK_REGISTERED_TYPE
    {
      ctx.m_varType = $1;
    }
    array_modifier
    {
      $$ = $3;
      ctx.m_varType->release();
      ctx.m_varType = 0;
    }
;

loop_start_statement
  : expression
  {
    $$ = AST::ExprStatement::Create( RTLOC, $1 ).take();
    $1->release();
  }
  | TK_VAR compound_type TK_IDENTIFIER TK_EQUALS assignment_expression 
  {
    $$ = AST::AssignedVarDecl::Create( RTLOC, *$3, $2, $5 ).take();
    $2->release();
    delete $3;
    $5->release();
  }
  | compound_type TK_IDENTIFIER TK_EQUALS assignment_expression 
  {
    $$ = AST::AssignedVarDecl::Create( RTLOC, *$2, $1, $4 ).take();
    $1->release();
    delete $2;
    $4->release();
  }
  | /* empty */
  {
    $$ = 0;
  }
;

var_decl_statement
  : TK_VAR compound_type 
      {
        ctx.m_varType = $2;
      }
    var_decl_list TK_SEMICOLON 
      {
        $$ = $4;
        ctx.m_varType = 0;
        $2->release();
      }
  | compound_type 
      {
        ctx.m_varType = $1;
      }
    var_decl_list TK_SEMICOLON 
      {
        $$ = $3;
        ctx.m_varType = 0;
        $1->release();
      }
;

loop_statement
  : TK_FOR TK_LPAREN loop_start_statement TK_SEMICOLON optional_expression TK_SEMICOLON optional_expression TK_RPAREN statement
  {
    $$ = AST::CStyleLoop::Create( RTLOC, $3, $5, $7, 0, $9 ).take();
    if ( $3 )
      $3->release();
    if ( $5 )
      $5->release();
    if ( $7 )
      $7->release();
    if ( $9 )
      $9->release();
  }
  | TK_WHILE TK_LPAREN optional_expression TK_RPAREN statement
  {
    $$ = AST::CStyleLoop::Create( RTLOC, 0, $3, 0, 0, $5 ).take();
    if ( $3 )
      $3->release();
    $5->release();
  }
  | TK_DO statement TK_WHILE TK_LPAREN optional_expression TK_RPAREN TK_SEMICOLON
  {
    $$ = AST::CStyleLoop::Create( RTLOC, 0, 0, 0, $5, $2 ).take();
    $2->release();
    if ( $5 )
      $5->release();
  }
;

switch_statement
  : TK_SWITCH TK_LPAREN expression TK_RPAREN TK_LBRACE case_list TK_RBRACE
  {
    $$ = AST::SwitchStatement::Create( RTLOC, $3, $6 ).take();
    $3->release();
    $6->release();
  }
;

case_list
  : case case_list
  {
    $$ = AST::CaseList::Create( RTLOC, $1, $2 ).take();
    $1->release();
    $2->release();
  }
  | /* empty */
  {
    $$ = AST::CaseList::Create( RTLOC ).take();
  }
;

case
  : TK_CASE expression TK_COLON statement_list
  {
    $$ = AST::Case::Create( RTLOC, $2, $4 ).take();
    $2->release();
    $4->release();
  }
  | TK_DEFAULT TK_COLON statement_list
  {
    $$ = AST::Case::Create( RTLOC, 0, $3 ).take();
    $3->release();
  }
;

statement
  : TK_SEMICOLON
  {
    $$ = 0;
  }
  | TK_RETURN expression TK_SEMICOLON
  {
    $$ = AST::ReturnStatement::Create( RTLOC, $2 ).take();
    $2->release();
  }
  | TK_RETURN TK_SEMICOLON
  {
    $$ = AST::ReturnStatement::Create( RTLOC ).take();
  }
  | const_decl_statement
  | var_decl_statement
  | expression TK_SEMICOLON
  {
    $$ = AST::ExprStatement::Create( RTLOC, $1 ).take();
    $1->release();
  }
  | TK_IF TK_LPAREN expression TK_RPAREN statement TK_ELSE statement
  {
    $$ = AST::ConditionalStatement::Create( RTLOC, $3, $5, $7 ).take();
    $3->release();
    $5->release();
    $7->release();
  }
  | TK_IF TK_LPAREN expression TK_RPAREN statement
  {
    $$ = AST::ConditionalStatement::Create( RTLOC, $3, $5 ).take();
    $3->release();
    $5->release();
  }
  | loop_statement
  | TK_REPORT expression TK_SEMICOLON
  {
    $$ = AST::Report::Create( RTLOC, $2 ).take();
    $2->release();
  }
  | TK_BREAK TK_SEMICOLON
  {
    $$ = AST::BreakStatement::Create( RTLOC ).take();
  }
  | TK_CONTINUE TK_SEMICOLON
  {
    $$ = AST::ContinueStatement::Create( RTLOC ).take();
  }
  | switch_statement
  | compound_statement
  {
    $$ = $1;
  }
;

/*
constant_expression
	: conditional_expression
  {
    $$ = $1;
  }
;
*/

optional_expression
  : expression
  | /* empty */
  {
    $$ = 0;
  }
;

expression
	: assignment_expression
  {
    $$ = $1;
  }
  /*
	| expression ',' assignment_expression
  */
;

assignment_operator
	: TK_EQUALS
  {
    $$ = CG::ASSIGN_OP;
  }
  | TK_PLUS_EQUALS
  {
    $$ = CG::ASSIGN_OP_ADD;
  }
  | TK_MINUS_EQUALS
  {
    $$ = CG::ASSIGN_OP_SUB;
  }
  | TK_AST_EQUALS
  {
    $$ = CG::ASSIGN_OP_MUL;
  }
  | TK_FSLASH_EQUALS
  {
    $$ = CG::ASSIGN_OP_DIV;
  }
  | TK_PERCENT_EQUALS
  {
    $$ = CG::ASSIGN_OP_MOD;
  }
  /*
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
  */
;

assignment_expression
	: conditional_expression
  {
    $$ = $1;
  }
	| postfix_expression assignment_operator assignment_expression
  {
    $$ = AST::AssignOp::Create( RTLOC, $2, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

conditional_expression
	: logical_or_expression
  {
    $$ = $1;
  }
	| logical_or_expression TK_QUESTION_MARK expression TK_COLON conditional_expression
  {
    $$ = AST::TernaryOp::Create( RTLOC, CG::TERNARY_OP_COND, $1, $3, $5 ).take();
    $1->release();
    $3->release();
    $5->release();
  }
;

logical_or_expression
	: logical_and_expression
  {
    $$ = $1;
  }
	| logical_or_expression TK_PIPE_PIPE logical_and_expression
  {
    $$ = AST::OrOp::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

logical_and_expression
	: inclusive_or_expression
  {
    $$ = $1;
  }
	| logical_and_expression TK_AMP_AMP inclusive_or_expression
  {
    $$ = AST::AndOp::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

inclusive_or_expression
	: exclusive_or_expression
  {
    $$ = $1;
  }
	| inclusive_or_expression TK_PIPE exclusive_or_expression
  {
    $$ = AST::BinOp::Create( RTLOC, CG::BIN_OP_BIT_OR, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

exclusive_or_expression
	: and_expression
  {
    $$ = $1;
  }
	| exclusive_or_expression TK_CARET and_expression
  {
    $$ = AST::BinOp::Create( RTLOC, CG::BIN_OP_BIT_XOR, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

and_expression
	: equality_expression
  {
    $$ = $1;
  }
	| and_expression TK_AMP equality_expression
  {
    $$ = AST::BinOp::Create( RTLOC, CG::BIN_OP_BIT_AND, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

equality_operator
  : TK_EQUALS_EQUALS
  {
    $$ = CG::BIN_OP_EQ;
  }
  | TK_EXCL_EQUALS
  {
    $$ = CG::BIN_OP_NE;
  }
;

equality_expression
	: relational_expression
  {
    $$ = $1;
  }
	| equality_expression equality_operator relational_expression
  {
    $$ = AST::BinOp::Create( RTLOC, $2, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

relational_operator
  : TK_LANGLE
  {
    $$ = CG::BIN_OP_LT;
  }
  | TK_LANGLE_EQUALS
  {
    $$ = CG::BIN_OP_LE;
  }
  | TK_RANGLE
  {
    $$ = CG::BIN_OP_GT;
  }
  | TK_RANGLE_EQUALS
  {
    $$ = CG::BIN_OP_GE;
  }
;

relational_expression
	: shift_expression
  {
    $$ = $1;
  }
	| relational_expression relational_operator shift_expression
  {
    $$ = AST::BinOp::Create( RTLOC, $2, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

shift_expression
	: additive_expression
  {
    $$ = $1;
  }
  /*
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
  */
;

additive_operator
  : TK_PLUS
  {
    $$ = CG::BIN_OP_ADD;
  }
  | TK_MINUS
  {
    $$ = CG::BIN_OP_SUB;
  }
;

additive_expression
	: multiplicative_expression
  {
    $$ = $1;
  }
	| additive_expression additive_operator multiplicative_expression
  {
    $$ = AST::BinOp::Create( RTLOC, $2, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

multiplicative_operator
  : TK_AST
  {
    $$ = CG::BIN_OP_MUL;
  }
  | TK_FSLASH
  {
    $$ = CG::BIN_OP_DIV;
  }
  | TK_PERCENT
  {
    $$ = CG::BIN_OP_MOD;
  }
;

multiplicative_expression
	: cast_expression
  {
    $$ = $1;
  }
	| multiplicative_expression multiplicative_operator cast_expression
  {
    $$ = AST::BinOp::Create( RTLOC, $2, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

cast_expression
	: prefix_unary_expression
  {
    $$ = $1;
  }
	| TK_LPAREN compound_type TK_RPAREN cast_expression
  {
    $$ = AST::CastNode::Create( RTLOC, $2, $4 ).take();
    $2->release();
    $4->release();
  }
;

prefix_unary_operator
	: TK_PLUS
  {
    $$ = CG::UNI_OP_POS;
  }
	| TK_MINUS
  {
    $$ = CG::UNI_OP_NEG;
  }
	| TK_TILDE
  {
    $$ = CG::UNI_OP_BIT_NOT;
  }
  | TK_PLUS_PLUS
  {
    $$ = CG::UNI_OP_PRE_INC;
  }
  | TK_MINUS_MINUS
  {
    $$ = CG::UNI_OP_PRE_DEC;
  }
;

prefix_unary_expression
	: postfix_expression
  {
    $$ = $1;
  }
	| prefix_unary_operator cast_expression
  {
    $$ = AST::UniOp::Create( RTLOC, $1, $2 ).take();
    $2->release();
  }
  | TK_EXCL cast_expression
  {
    $$ = AST::NotOp::Create( RTLOC, $2 ).take();
    $2->release();
  }
  /*
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
  */
;

argument_expression_list
	: /* empty */
  {
    $$ = AST::ArgList::Create( RTLOC ).take();
  }
  | assignment_expression
  {
    $$ = AST::ArgList::Create( RTLOC, $1 ).take();
    $1->release();
  }
	| assignment_expression TK_COMMA argument_expression_list
  {
    $$ = AST::ArgList::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
;

postfix_unary_operator
  : TK_PLUS_PLUS
  {
    $$ = CG::UNI_OP_POST_INC;
  }
  | TK_MINUS_MINUS
  {
    $$ = CG::UNI_OP_POST_DEC;
  }
;

postfix_expression
	: primary_expression
  {
    $$ = $1;
  }
	| postfix_expression TK_LBRACKET expression TK_RBRACKET
  {
    $$ = AST::IndexOp::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
	| postfix_expression TK_DOT TK_IDENTIFIER
  {
    $$ = AST::StructMemberOp::Create( RTLOC, $1, *$3 ).take();
    $1->release();
    delete $3;
  }
  | postfix_expression TK_DOT TK_IDENTIFIER TK_LPAREN argument_expression_list TK_RPAREN
  {
    $$ = AST::MethodOp::Create( RTLOC, *$3, $1, $5 ).take();
    $1->release();
    delete $3;
    $5->release();
  }
  /*
	| postfix_expression PTR_OP IDENTIFIER
  */
	| postfix_expression postfix_unary_operator
  {
    $$ = AST::UniOp::Create( RTLOC, $2, $1 ).take();
    $1->release();
  }
;

primary_expression
  : TK_TRUE
  {
    $$ = AST::ConstBoolean::Create( RTLOC, true ).take();
  }
  | TK_FALSE
  {
    $$ = AST::ConstBoolean::Create( RTLOC, false ).take();
  }
  | TK_CONST_UI
  {
    $$ = AST::ConstSize::Create( RTLOC, *$1 ).take();
    delete $1;
  }
  | TK_CONST_FP
  {
    $$ = AST::ConstScalar::Create( RTLOC, *$1 ).take();
    delete $1;
  }
  | TK_CONST_STRING_SQUOT
  {
    try
    {
      $$ = AST::ConstString::Create( RTLOC, *$1, true ).take();
    }
    catch ( Exception e )
    {
      ctx.m_diagnostics.addError( RTLOC, e.getDesc() );
    }
    delete $1;
  }
  | TK_CONST_STRING_DQUOT
  {
    try
    {
      $$ = AST::ConstString::Create( RTLOC, *$1, true ).take();
    }
    catch ( Exception e )
    {
      ctx.m_diagnostics.addError( RTLOC, e.getDesc() );
    }
    delete $1;
  }
  | TK_IDENTIFIER TK_LPAREN argument_expression_list TK_RPAREN
  {
    $$ = AST::Call::Create( RTLOC, *$1, $3 ).take();
    delete $1;
    $3->release();
  }
  | compound_type TK_LPAREN argument_expression_list TK_RPAREN
  {
    $$ = AST::TempValue::Create( RTLOC, $1, $3 ).take();
    $1->release();
    $3->release();
  }
  | TK_IDENTIFIER
  {
    $$ = AST::Var::Create( RTLOC, *$1 ).take();
    delete $1;
  }
  | TK_LPAREN expression TK_RPAREN
  {
    $$ = $2;
  }
;

%%

void kl_error( YYLTYPE *yyl, KL::Context &ctx, const char *msg )
{
  ctx.m_diagnostics.addError( CG::Location( yyl->first_line, yyl->first_column ), msg );
}

int kl_lex( YYSTYPE *yys, YYLTYPE *yyl, KL::Context &ctx )
{
  FABRIC_KL_TRACE( "kl_lex( %p, %p )", yys, yyl );

  KL::Location startLocation;
  KL::Token token;
  std::string text;
  KL::Location endLocation;
  while ( true )
  {
    startLocation = ctx.m_scanner->source().locationForStart();
    try
    {
      token = ctx.m_scanner->nextToken( yys );
    }
    catch ( Exception e )
    {
      ctx.m_diagnostics.addError( CG::Location( yyl->first_line, yyl->first_column ), e.getDesc() );
    }
    if ( token != TK_WHITESPACE
      && token != TK_COMMENT_LINE
      && token != TK_COMMENT_BLOCK
      )
    {
      endLocation = ctx.m_scanner->source().locationForEnd();
      break;
    }
    delete yys->valueStringPtr;
    yys->valueStringPtr = 0;
  }

  yyl->first_line = startLocation.line();
  yyl->first_column = startLocation.column();
  yyl->last_line = endLocation.line();
  yyl->last_column = endLocation.column();
  
  int result = int(token);
  if ( kl_debug )
    fprintf( stderr, "Returning token %d\n", result );
  return result;
}
