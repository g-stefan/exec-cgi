// Exec CGI
// Copyright (c) 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2014-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_EXECCGI_APPLICATION_HPP
#define XYO_EXECCGI_APPLICATION_HPP

#ifndef XYO_EXECCGI_DEPENDENCY_HPP
#	include <XYO/ExecCGI/Dependency.hpp>
#endif

namespace XYO::ExecCGI {

	class Application : public virtual IApplication {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(Application);

		public:
			inline Application(){};

			void showUsage();
			void showLicense();
			void showVersion();

			int main(int cmdN, char *cmdS[]);

			static void initMemory();
	};

};

#endif
