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

extern FILE *stderr;

void buildAST(Expression *expr) {


	for(int i = 0; expr->stringRepr[i]; i++) {
		if(isOperator(expr->stringRepr[i])) {
			StrNumContainer *leftStrNumContainer = newContainer();
			StrNumContainer *rightStrNumContainer = newContainer();

			parseNumbers(expr->stringRepr, i, leftStrNumContainer, rightStrNumContainer);
		}
	}
}

int isOperator(char exprChar) {
	for(int i = 0; posibleOperators[i]; i++)
		if(posibleOperators[i][0] == exprChar)
			return 1;

	return 0;
}

void parseNumbers(char *str, int opPos, StrNumContainer *leftStrNumContainer, StrNumContainer *rightStrNumContainer) {
	//left node
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

	//right node
	//NOTICE for right node SNC_reverse do not need
	for(int i = opPos+1; i <= strlen(str) ; i++) {
		if(isOperator(str[i])) {
			break;
		} else {
			if(!(str[i] == ' '))
				SNC_push(rightStrNumContainer, str[i]);
		}
	}

	int result = SNC_eval(leftStrNumContainer, rightStrNumContainer, str[opPos]);

	printf("%s = %d\n", str, result);

	free(leftStrNumContainer->container);
	free(leftStrNumContainer);

	free(rightStrNumContainer->container);
	free(rightStrNumContainer);
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
	strContainer->container = (char *)malloc(DEFAULT_AllOCATION_SIZE * sizeof(char));
	strContainer->maxSize = DEFAULT_AllOCATION_SIZE;
	strContainer->currentPos = 0;

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
	//FIXME realize inplace reverse in the feature
	int bytesCountForAllocate = strNumContainer->currentPos - 1;
	char *buff = (char *) malloc(bytesCountForAllocate * sizeof(char));

	for(int i = bytesCountForAllocate, j = 0; i >= 0; i--, j++)
		buff[j] = strNumContainer->container[i];

	strcpy(strNumContainer->container, buff);

	free(buff);
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

int plus(int left, int right) { return left + right; }
int minus(int left, int right) { return left - right; }

int binaryOperationCall(int left, int right, BinaryOperation fun) { return fun(left, right); }