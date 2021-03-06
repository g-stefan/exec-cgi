//
// Exec CGI
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef EXEC_CGI_VERSION_HPP
#define EXEC_CGI_VERSION_HPP

#define EXEC_CGI_VERSION_ABCD                1,7,0,9
#define EXEC_CGI_VERSION_STR                 "1.7.0"
#define EXEC_CGI_VERSION_STR_BUILD           "9"
#define EXEC_CGI_VERSION_STR_DATETIME        "2021-07-06 15:11:46"

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

