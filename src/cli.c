/*
 * cli.c
 *
 *  Created on: 11 июня 2015 г.
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>
#include "cli.h"
#include "string.h"

//static const int CONF_ARGS_MAX_COUNT = 2;

void parseCLIArgs(int argc, char **argv, Configuration *conf, Expression *expr) {
	int i, confIter, expIter;

	for(i = 1, confIter = 0; i < argc; i++) {
		if(startWith(argv[i], &CONF_PREFIX)) {
			int len = strlen(argv[i]);
			conf->confFlags[confIter] = malloc(len * sizeof(char));
			copyArgs(conf->confFlags[confIter], argv[i], 1);
			confIter++;
		} else {
			int len = strlen(argv[i]);
			expr->stringRepr = malloc(len * sizeof(char));
			copyArgs(expr->stringRepr, argv[i], 0);
		}
	}

	conf->flagsCount = confIter;
	expr->exprCount = expIter;
}

int startWith(char *arg, const char *prefix) {
	return *arg == *prefix ? 1 : 0;
}

void copyArgs(char *argv, char *arg, int offset) {
	register int i;
	for(i = 0; i < strlen(arg); i++) {
		argv[i] = arg[i+offset];
	}

	argv[i] = '\0';
}
