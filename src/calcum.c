/*
 ============================================================================
 Name        : calcum.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "cli.h"
#include "expression.h"

/*
 * +, - => '1'
 * *, / => '2'
 */

int main(int argc, char **argv) {
	Configuration config;
	config.confFlags = malloc(sizeof(char *));

	Expression expr;
	expr.stringRepr;

	parseCLIArgs(argc, argv, &config, &expr);
	int result = buildAndEvalAST(&expr);

	printf("result = %d\n", result);

	return 0;
}
