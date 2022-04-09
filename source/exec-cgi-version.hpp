//
// Exec CGI
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef EXEC_CGI_VERSION_HPP
#define EXEC_CGI_VERSION_HPP

#define EXEC_CGI_VERSION_ABCD 1,13,0,34
#define EXEC_CGI_VERSION_STR "1.13.0"
#define EXEC_CGI_VERSION_STR_BUILD "34"
#define EXEC_CGI_VERSION_STR_DATETIME "2022-04-04 01:44:51"

#ifndef XYO_RC

namespace ExecCGI {
	namespace Version {
		const char *version();
		const char *build();
		const char *versionWithBuild();
		const char *datetime();
	};
};

#endif
#endif
