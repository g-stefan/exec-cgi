//
// Exec CGI
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyo.hpp"

#include "exec-cgi.hpp"
#include "exec-cgi-copyright.hpp"
#include "exec-cgi-license.hpp"
#ifndef EXEC_CGI_NO_VERSION
#	include "exec-cgi-version.hpp"
#endif

using namespace XYO;

namespace ExecCGI {

	void Application::showUsage() {
		printf("exec-cgi - execute php script as cgi\n");
		showVersion();
		printf("%s\n\n", ExecCGI::Copyright::fullCopyright());
		printf("\n");

		printf("Usage:\n\n");
		printf("\texec-cgi --usage\n");
		printf("\texec-cgi --license\n");
		printf("\texec-cgi --version\n");
		printf("\texec-cgi query output_file\n");
		printf("\texec-cgi query\n\n");
		printf("example:\n\n");
		printf("\texec-cgi \"foo.php?a=foo&b=5\" foo.html\n");
	};

	void Application::showLicense() {
		printf("%s", ExecCGI::License::content());
	};

	void Application::showVersion() {
#ifndef EXEC_CGI_NO_VERSION
		printf("version %s build %s [%s]\n", ExecCGI::Version::version(), ExecCGI::Version::build(), ExecCGI::Version::datetime());
#endif
	};

	int Application::main(int cmdN, char *cmdS[]) {
		char script[16384];
		char php[16384];
		char env[16384];
		char *query;
		FILE *in;
		FILE *out;

		int k;

		if (cmdN < 3) {

			if (cmdN == 2) {
				if (strcmp(cmdS[1], "--usage") == 0) {
					showUsage();
					return 0;
				};
				if (strcmp(cmdS[1], "--license") == 0) {
					showLicense();
					return 0;
				};
				if (strcmp(cmdS[1], "--version") == 0) {
					showVersion();
					return 0;
				};
			};
		};

		if (cmdN < 2) {
			showUsage();
			return 2;
		};

		for (k = 0; cmdS[1][k] != 0; ++k) {
			if (cmdS[1][k] == '?') {
				break;
			};
		};
		strncpy(script, cmdS[1], k);
		if (cmdS[1][k] == '?') {
			query = &cmdS[1][k + 1];
		} else {
			query = &cmdS[1][k];
		};

		strcpy(env, "SERVER_SOFTWARE=exec-cgi");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "SERVER_NAME=exec-cgi");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "GATEWAY_INTERFACE=CGI/1.1");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "HTTP_REDIRECT_STATUS=On");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "REMOTE_ADDR=127.0.0.1");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "REMOTE_HOST=localhost");
		if (!putenv(env)) {
			return 1;
		};
		strcpy(env, "REQUEST_METHOD=GET");
		if (!putenv(env)) {
			return 1;
		};
		sprintf(env, "SERVER_REQUEST_URI=%s", cmdS[1]);
		if (!putenv(env)) {
			return 1;
		};
		sprintf(env, "QUERY_STRING=%s", query);
		if (!putenv(env)) {
			return 1;
		};
		sprintf(env, "PATH_INFO=%s", script);
		if (!putenv(env)) {
			return 1;
		};
		sprintf(env, "SCRIPT_NAME=%s", script);
		if (!putenv(env)) {
			return 1;
		};
		sprintf(env, "PATH_TRANSLATED=%s", script);
		if (!putenv(env)) {
			return 1;
		};
		if (cmdN >= 3) {
			sprintf(php, "php-cgi -q -n -d -f %s>%s", script, cmdS[2]);
		} else {
			sprintf(php, "php-cgi -q -n -d -f %s", script);
		};
		return system(php);
	};

};

#ifndef EXEC_CGI_LIBRARY
XYO_APPLICATION_MAIN_STD(ExecCGI::Application);
#endif
