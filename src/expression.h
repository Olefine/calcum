/*
 * expression.h
 *
 *  Created on: 12 июня 2015 г.
 *      Author: egorgorodov
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "cli.h"

typedef struct {
	char *leftNode;
	char *rightNode;
} ExpressionAST;

typedef struct {
	char *container;
	int currentPos;
	int maxSize;
	int startPos;
	int endPos;
} StrNumContainer;

static const char *posibleOperators[] = { "+", "-", "*", "/" };

typedef struct {
	StrNumContainer *left;
	StrNumContainer *right;
} Node;

typedef struct {
	int currentPos;
	Node **nodes;
} Tree;

Tree *createTree();
void T_insert(Tree *tree, Node *nd);

Node *createNode();
int N_compare(Node *nd1, Node *nd2);

typedef int (*BinaryOperation)(int a, int b);

int plus(int left, int right);
int minus(int left, int right);

void buildAST(Expression *expr);
int isOperator(char exprChar);
void parseNumbers(char *str, int opPos, Node *node);
void parseLeft(char *str, int opPost, StrNumContainer *leftStrNumContainer);
void parseRight(char *str, int opPos, StrNumContainer *rightStrNumContainer);

static const int DEFAULT_AllOCATION_SIZE = 3;

StrNumContainer *newContainer();

// strNumContainer operations
int SNC_push(StrNumContainer *strNumContainer, char value);
int SNC_reverse(StrNumContainer *strNumContainer);
int SNC_eval(StrNumContainer *left, StrNumContainer *right, char strOperation);

#endif /* EXPRESSION_H_ */
