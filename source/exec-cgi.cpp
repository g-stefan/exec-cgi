//
// Exec CGI
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec-cgi-version.hpp"
#include "exec-cgi-copyright.hpp"
#include "exec-cgi-license.hpp"

char script[8192];
char php[16384];
char env[32000];
char *query;
FILE *in;
FILE *out;

int main(int cmd, char *cmds[]) {
	int k;

	if(cmd < 3) {

		if(cmd == 2) {
			if(strcmp(cmds[1], "--license") == 0) {
				printf("%s", ExecCGI::License::content());
				return 0;
			};
		};

	};

	if(cmd < 2) {
		printf("exec-cgi - execute php script as cgi\n");
		printf("version %s build %s [%s]\n", ExecCGI::Version::version(), ExecCGI::Version::build(), ExecCGI::Version::datetime());
		printf("%s\n\n", ExecCGI::Copyright::fullCopyright());
		printf("\n");

		printf("Usage:\n\n");
		printf("\texec-cgi --license\n");
		printf("\texec-cgi query output_file\n");
		printf("\texec-cgi query\n\n");
		printf("example:\n\n");
		printf("\texec-cgi \"foo.php?a=foo&b=5\" foo.html\n");
		return 2;
	};

	for(k = 0; cmds[1][k] != 0; ++k) {
		if(cmds[1][k] == '?') {
			break;
		}
	};
	strncpy(script, cmds[1], k);
	if(cmds[1][k] == '?') {
		query = &cmds[1][k + 1];
	} else {
		query = &cmds[1][k];
	}

	strcpy(env, "SERVER_SOFTWARE=exec-cgi");
	putenv(env);
	strcpy(env, "SERVER_NAME=exec-cgi");
	putenv(env);
	strcpy(env, "GATEWAY_INTERFACE=CGI/1.1");
	putenv(env);
	strcpy(env, "HTTP_REDIRECT_STATUS=On");
	putenv(env);
	strcpy(env, "REMOTE_ADDR=127.0.0.1");
	putenv(env);
	strcpy(env, "REMOTE_HOST=localhost");
	putenv(env);
	strcpy(env, "REQUEST_METHOD=GET");
	putenv(env);
	sprintf(env, "SERVER_REQUEST_URI=%s", cmds[1]);
	putenv(env);
	sprintf(env, "QUERY_STRING=%s", query);
	putenv(env);
	sprintf(env, "PATH_INFO=%s", script);
	putenv(env);
	sprintf(env, "SCRIPT_NAME=%s", script);
	putenv(env);
	sprintf(env, "PATH_TRANSLATED=%s", script);
	putenv(env);
	if(cmd >= 3) {
		sprintf(php, "php-cgi -q -n -d -f %s>%s", script, cmds[2]);
	} else {
		sprintf(php, "php-cgi -q -n -d -f %s", script);
	};
	return system(php);
};

