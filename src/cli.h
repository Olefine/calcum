/*
 * cli.h
 *
 *  Created on: 11 июня 2015 г.
 *      Author: root
 */

#ifndef CLI_H_
#define CLI_H_

static const char CONF_PREFIX = '-';

typedef struct {
	char **confFlags;
	int flagsCount;
} Configuration;

typedef struct {
	char *stringRepr;
	int exprCount;
} Expression;

int startWith(char *arg, const char *prefix);
void parseCLIArgs(int argc, char **argv, Configuration *conf, Expression *expr);
void copyArgs(char *argv, char *arg, int offset);

#endif /* CLI_H_ */
