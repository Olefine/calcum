/*
 * expression.c
 *
 *  Created on: 12 июня 2015 г.
 *      Author: egorgorodov
 */

#include "expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern FILE *stderr;

void buildAST(Expression *expr) {
	Tree *tree = createTree();

	for(int i = 0; expr->stringRepr[i]; i++) {
		if(isOperator(expr->stringRepr[i])) {
			Node *node = createNode();
			T_insert(tree, node);

			parseNumbers(expr->stringRepr, i, node);
		}
	}
}

int isOperator(char exprChar) {
	for(int i = 0; posibleOperators[i]; i++)
		if(posibleOperators[i][0] == exprChar)
			return 1;

	return 0;
}

void parseNumbers(char *str, int opPos, Node *node) {
	parseLeft(str, opPos, node->left);
	parseRight(str, opPos, node->right);


	int result = SNC_eval(node->left, node->right, str[opPos]);

	printf("%s = %d\n", str, result);


	//need free nodes after all work with
//	free(leftStrNumContainer->container);
//	free(leftStrNumContainer);
//
//	free(rightStrNumContainer->container);
//	free(rightStrNumContainer);
}


/*
 * strNumContainer is reversed number container
 * all functions return 0 if operation successed, and 1 otherwise
 * posibile functions for manage it:
 * 	  push(char* value)
 * 	  reverse(char *strNumContainer) // "003" -> "300"
 * 	  eval(char *reversedContainer) // "300" -> 300 with dynamic type resolver
 *
 */

StrNumContainer *newContainer() {
	StrNumContainer *strContainer = malloc(sizeof(StrNumContainer *));
	strContainer->container = (char *)malloc(DEFAULT_AllOCATION_SIZE * sizeof(char *));
	strContainer->maxSize = DEFAULT_AllOCATION_SIZE;
	strContainer->currentPos = 0;
	strContainer->startPos = 0;
	strContainer->endPos = 0;

	return strContainer;
}

int SNC_push(StrNumContainer *strNumContainer, char value) {
	if(strNumContainer->currentPos < strNumContainer->maxSize) {
		strNumContainer->container[strNumContainer->currentPos++] = value;
	} else if(strNumContainer->currentPos == strNumContainer->maxSize) {
		strNumContainer->container = (char *) realloc(strNumContainer->container, strNumContainer->currentPos + DEFAULT_AllOCATION_SIZE);
		strNumContainer->maxSize = strNumContainer->maxSize + DEFAULT_AllOCATION_SIZE;
		strNumContainer->container[strNumContainer->currentPos++] = value;
	} else {
		return 1;
	}
	return 0;
}

int SNC_reverse(StrNumContainer *strNumContainer) {
	int bytesCountForAllocate = strNumContainer->currentPos;
	int halfLen = bytesCountForAllocate / 2;

	for(int i = 0, j = 1; i < halfLen; i++, j++) {
		char tmp = strNumContainer->container[i];
		strNumContainer->container[i] = strNumContainer->container[bytesCountForAllocate - j];
		strNumContainer->container[bytesCountForAllocate - j] = tmp;
	}

	return 0;
}

int SNC_eval(StrNumContainer *left, StrNumContainer *right, char strOperation) {
	int leftValue = atoi(left->container);
	int rightValue = atoi(right->container);

	int result = 0;

	if(strOperation == '+')
		result = binaryOperationCall(leftValue, rightValue, plus);
	else
		result = binaryOperationCall(leftValue, rightValue, minus);

	return result;
}

void parseLeft(char *str, int opPos, StrNumContainer *leftStrNumContainer) {
	for(int i = opPos-1; i >= 0 ; i--) {
		if(isOperator(str[i])) {
			break;
		} else {
			if(!(str[i] == ' '))
				SNC_push(leftStrNumContainer, str[i]);
		}
	}

	if(SNC_reverse(leftStrNumContainer))
		fprintf(stderr, "Can not reverse leftStrNumContainer\n");
}

void parseRight(char *str, int opPos, StrNumContainer *rightStrNumContainer) {
	for(int i = opPos+1; i <= strlen(str) ; i++) {
			if(isOperator(str[i])) {
				break;
				//StrNumContainer *rrightStrNumContainer = newContainer();
				//parseRight(str, i, rrightStrNumContainer);
				//printf("%s\n", rrightStrNumContainer->container);
			} else {
				if(!(str[i] == ' '))
					SNC_push(rightStrNumContainer, str[i]);
			}
		}
}

Tree *createTree() {
	Tree *tree = malloc(sizeof(Tree *));
	tree->currentPos = 0;
	tree->nodes = malloc(sizeof(Node *));

	return tree;
}

void T_insert(Tree *tree, Node *nd) {
	tree->nodes[tree->currentPos] = nd;
}

Node *createNode() {
	Node *nd = malloc(sizeof(Node *));
	nd->left = newContainer();
	nd->right = newContainer();

	return nd;
}

int N_compare(Node *nd1, Node *nd2) {
	//need implement compare node positions
	return 1;
}

int plus(int left, int right) { return left + right; }
int minus(int left, int right) { return left - right; }

int binaryOperationCall(int left, int right, BinaryOperation fun) { return fun(left, right); }
