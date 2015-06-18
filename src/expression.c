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
			T_insert(tree, parseNumbers(expr->stringRepr, i));
		}
	}

	int acum = 0;
	acum = recursive_eval(tree->nodes, 0, tree->currentPos);

	printf("%d\n", acum);
}

int recursive_eval(Node **nodes, i, max) {
	if(i == max)
		return 0;
	else
		if(i == 0)
			return binaryOperationCall(SNC_eval(nodes[i]->left, nodes[i]->right, nodes[i]->operator), recursive_eval(nodes, i + 1, max), resolveOperation(nodes[i+1]->operator));
		else {
			return binaryOperationCall(atoi(nodes[i]->right->container), recursive_eval(nodes, i + 1, max), resolveOperation(nodes[i]->operator));
		}
}

int isOperator(char exprChar) {
	for(int i = 0; posibleOperators[i]; i++)
		if(posibleOperators[i][0] == exprChar)
			return 1;

	return 0;
}

Node *parseNumbers(char *str, int opPos) {
	Node *node = createNode();
	parseLeft(str, opPos, node->left);
	parseRight(str, opPos, node->right);
	node->operator = str[opPos];

	printf("%c\n", node->operator);

	return node;

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
	StrNumContainer *strContainer = malloc(sizeof(StrNumContainer *) + 4 * (sizeof(int)));
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
	int i;
	int startPos = -1, endPos = -1;

	for(i = opPos-1; i >= 0; i--) {
		if(isOperator(str[i])) {
			if(isspace(str[i-1]))
				startPos = i + 2;
			else
				startPos = i + 1;
			break;
		} else if(!(str[i] == ' ')) {
			if(endPos < 0)
				endPos = i;
			SNC_push(leftStrNumContainer, str[i]);
		}
	}

	if(startPos < 0)
		startPos = i+1;

	if(SNC_reverse(leftStrNumContainer))
		fprintf(stderr, "Can not reverse leftStrNumContainer\n");

	leftStrNumContainer->startPos = startPos;
	leftStrNumContainer->endPos = endPos;
}

void parseRight(char *str, int opPos, StrNumContainer *rightStrNumContainer) {
	int i;
	int startPos = -1, endPos = -1;

	for(i = opPos+1; i < strlen(str); i++) {
		if(isOperator(str[i])) {
			if(isspace(str[i-1]))
				endPos = i - 2;
			else
				endPos = i - 1;
			break;
		} else {
			if(!(str[i] == ' '))
				if(startPos < 0)
					startPos = i;
				SNC_push(rightStrNumContainer, str[i]);
		}
	}

	if(endPos < 0)
		endPos = i-1;

	rightStrNumContainer->startPos = startPos;
	rightStrNumContainer->endPos = endPos;
}

Tree *createTree() {
	Tree *tree = malloc(sizeof(Tree *));
	tree->currentPos = 0;
	//TODO dynamicly resize nodes
	tree->nodes = malloc(sizeof(Node *) + 20 * sizeof(Node *));

	return tree;
}

void T_insert(Tree *tree, Node *nd) {
	tree->nodes[tree->currentPos++] = nd;
}

Node *createNode() {
	//TODO dynamicly allocate int for operators(number_of_operator * sizeof(char))
	Node *nd = malloc(sizeof(Node *) + 10 * sizeof(char));
	nd->left = newContainer();
	nd->right = newContainer();
	nd->operator = ' ';

	return nd;
}

int N_compare(Node *nd1, Node *nd2) {
	return (nd1->right->endPos == nd2->left->endPos) && (nd1->right->startPos == nd2->left->startPos);
}

int plus(int left, int right) { return left + right; }
int minus(int left, int right) { return left - right; }

int binaryOperationCall(int left, int right, BinaryOperation fun) { return fun(left, right); }

BinaryOperation resolveOperation(char operator) {
	printf("op = %c\n", operator);
	if(operator == '+')
		return plus;
	else
		return minus;
}
