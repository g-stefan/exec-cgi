//
// Exec CGI
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef EXEC_CGI_VERSION_HPP
#define EXEC_CGI_VERSION_HPP

#define EXEC_CGI_VERSION_ABCD                1,2,0,4
#define EXEC_CGI_VERSION_STR                 "1.2.0"
#define EXEC_CGI_VERSION_STR_BUILD           "4"
#define EXEC_CGI_VERSION_STR_DATETIME        "2020-09-15 01:12:04"

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

