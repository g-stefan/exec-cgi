// Exec CGI
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/ExecCGI/License.hpp>

namespace XYO::ExecCGI::License {

	static const char *licenseHeader_ =
	    "\r\n"
	    "Exec CGI\r\n"
	    "\r\n"
	    "MIT License (MIT)\r\n"
	    "\r\n"
	    "Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>\r\n"
	    "\r\n";

	static const char *shortLicense_ =
	    "\r\n"
	    "Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>\r\n"
	    "MIT License (MIT) <http://opensource.org/licenses/MIT>\r\n"
	    "\r\n";

	const char *licenseHeader() {
		return licenseHeader_;
	};

	const char *licenseBody() {
		return XYO::ManagedMemory::License::licenseBody();
	};

	const char *shortLicense() {
		return shortLicense_;
	};

};
