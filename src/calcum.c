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

int main(int argc, char **argv) {
	Configuration config;
	config.confFlags = malloc(sizeof(char *));

	Expression expr;
	expr.stringRepr;

	parseCLIArgs(argc, argv, &config, &expr);
	buildAST(&expr);

	return 0;
}