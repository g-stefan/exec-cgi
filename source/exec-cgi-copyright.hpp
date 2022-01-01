//
// Exec CGI
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef EXEC_CGI_COPYRIGHT_HPP
#define EXEC_CGI_COPYRIGHT_HPP

#define EXEC_CGI_COPYRIGHT            "Copyright (c) Grigore Stefan"
#define EXEC_CGI_PUBLISHER            "Grigore Stefan"
#define EXEC_CGI_COMPANY              EXEC_CGI_PUBLISHER
#define EXEC_CGI_CONTACT              "g_stefan@yahoo.com"
#define EXEC_CGI_FULL_COPYRIGHT       EXEC_CGI_COPYRIGHT " <" EXEC_CGI_CONTACT ">"

#ifndef XYO_RC

namespace ExecCGI {
	namespace Copyright {
		const char *copyright();
		const char *publisher();
		const char *company();
		const char *contact();
		const char *fullCopyright();
	};
};

#endif
#endif
