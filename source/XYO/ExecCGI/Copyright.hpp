// Exec CGI
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_EXECCGI_COPYRIGHT_HPP
#define XYO_EXECCGI_COPYRIGHT_HPP

#ifndef XYO_EXECCGI_DEPENDENCY_HPP
#	include <XYO/ExecCGI/Dependency.hpp>
#endif

namespace XYO::ExecCGI::Copyright {
	const char *copyright();
	const char *publisher();
	const char *company();
	const char *contact();
};

#endif
