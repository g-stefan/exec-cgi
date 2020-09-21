//
// Exec CGI
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "exec-cgi-version.hpp"

namespace ExecCGI {
	namespace Version {

		static const char *version_ = "1.3.0";
		static const char *build_ = "5";
		static const char *versionWithBuild_ = "1.3.0.5";
		static const char *datetime_ = "2020-09-21 17:40:06";

		const char *version() {
			return version_;
		};
		const char *build() {
			return build_;
		};
		const char *versionWithBuild() {
			return versionWithBuild_;
		};
		const char *datetime() {
			return datetime_;
		};

	};
};

