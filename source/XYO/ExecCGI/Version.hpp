// Exec CGI
// Copyright (c) 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_EXECCGI_VERSION_HPP
#define XYO_EXECCGI_VERSION_HPP

#ifndef XYO_EXECCGI_DEPENDENCY_HPP
#	include <XYO/ExecCGI/Dependency.hpp>
#endif

namespace XYO::ExecCGI::Version {

	const char *version();
	const char *build();
	const char *versionWithBuild();
	const char *datetime();

};

#endif
