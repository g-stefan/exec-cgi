# Reference

For people working on `exec-cgi` itself. Using the tool is covered in
[Command line](command-line.md) and [CGI environment](cgi-environment.md).

## fabricare project

```json
{
	"name" : "exec-cgi",
	"make" : "exe",
	"SPDX-License-Identifier": "MIT",
	"sourcePath" : "XYO/ExecCGI",
	"dependency" : [ "xyo-system" ]
}
```

One project, an executable built from every source in
`source/XYO/ExecCGI/`. `xyo-system` brings in `xyo-encoding`,
`xyo-multithreading`, `xyo-data-structures`, `xyo-managed-memory` and
`xyo-platform`. The version lives in `version.json` under the key
`exec-cgi`; `Version.rh` is generated from `Version.Template.rh` by
xyo-version (`fabricare version` bumps the build number). There is no test
project.

## Headers

| Header | Contents |
|--------|----------|
| `<XYO/ExecCGI/Dependency.hpp>` | includes `<XYO/System.hpp>`; `namespace XYO::ExecCGI { using namespace XYO::System; }` |
| `<XYO/ExecCGI/Application.hpp>` | `XYO::ExecCGI::Application` |
| `<XYO/ExecCGI/Copyright.hpp>`, `License.hpp`, `Version.hpp` | tool metadata |
| `Application.rh`, `Copyright.rh`, `Version.rh` | macros shared by the C++ code and the Windows resource script |

## Macros

| Macro | Meaning |
|-------|---------|
| `XYO_EXECCGI_LIBRARY` | if defined, `Application.cpp` does not define `main` (`XYO_APPLICATION_MAIN` is skipped), so the class can be compiled into another program; no fabricare project sets it |
| `XYO_EXECCGI_NO_VERSION` | `Version.rh` gives `0.0.0` / build `0` instead of the generated version |
| `XYO_EXECCGI_VERSION_ABCD`, `_STR`, `_STR_BUILD`, `_STR_DATETIME`, `_STR_WITH_BUILD` | version, from `version.json` |
| `XYO_EXECCGI_COPYRIGHT`, `_PUBLISHER`, `_COMPANY`, `_CONTACT` | copyright strings |

## class XYO::ExecCGI::Application

```cpp
class Application : public virtual IApplication {
		XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Application);

	public:
		inline Application(){};

		void showUsage();
		void showLicense();
		void showVersion();

		int main(int cmdN, char *cmdS[]);

		static void initMemory();
};

XYO_APPLICATION_MAIN(XYO::ExecCGI::Application);   // unless XYO_EXECCGI_LIBRARY
```

| Member | Does |
|--------|------|
| `showUsage()` | prints the title, `showVersion()`, the copyright, the usage and an example |
| `showLicense()` | prints `License::license()` |
| `showVersion()` | prints `version <version> build <build> [<datetime>]` |
| `initMemory()` | initializes the managed memory of `String`, `TDynamicArray<String>` and `TDoubleEndedQueue<String>` before `main` (called by `XYO_APPLICATION_MAIN` through `TIfHasInitMemory`) |
| `main(cmdN, cmdS)` | the tool; returns the process exit code |

`main`, step by step:

1. With exactly one argument equal to `--help` / `--usage` →
   `showUsage()`, `--license` → `showLicense()`, `--version` →
   `showVersion()`, then return `0` (exact `strcmp`, no `=value`).
2. With no argument: `showUsage()`, return `2`.
3. `request = cmdS[1]`; `request.split2("?", script, query)` — split at
   the first `?`; without one, `query` is empty.
4. `Shell::setenv` each CGI variable (`SERVER_SOFTWARE`, `SERVER_NAME`,
   `GATEWAY_INTERFACE`, `REDIRECT_STATUS`, `REMOTE_ADDR`, `REMOTE_HOST`,
   `REQUEST_METHOD`, `SERVER_REQUEST_URI`, `QUERY_STRING`, `PATH_INFO`,
   `SCRIPT_NAME`, `PATH_TRANSLATED`); return `1` if one fails.
5. Build `php-cgi -q -n -d -f "<script>"`, plus `>"<cmdS[2]>"` when there
   is a second argument.
6. Return `Shell::system(cmdLine)`.

Two things in steps 4 and 6 must stay as they are:

- **`Shell::setenv`, not C `putenv`.** `Shell::setenv` (`_putenv_s` on
  Windows, `setenv(…, 1)` on Linux) copies the value and returns `true` on
  success. C `putenv` returns `0` on success and, on Linux, keeps the
  pointer it was given, so reusing one buffer for all variables leaves
  every variable pointing at the last value. Up to 4.9.0 build 8 the code
  used `putenv` into one `char env[16384]` buffer and tested
  `if (!putenv(env)) return 1;`, which returned `1` on the first success:
  the tool never ran PHP. The same version copied the script name with
  `strncpy` without a terminating `0` and built the command with `sprintf`
  into fixed buffers.
- **`Shell::system`, qualified.** `Shell` is a nested namespace and is not
  brought in by `using namespace XYO::System`, so a bare `system(cmdLine)`
  silently resolves to the C library `::system` (the `String` converts to
  `const char *`). On Windows both return the exit code of `cmd.exe`; on
  Linux `::system` returns a wait status (`exit code × 256`).
  `Shell::system` converts it the way a shell does: the exit code, or
  `128 + signal`.

## namespace XYO::ExecCGI::Version / Copyright / License

```cpp
const char *XYO::ExecCGI::Version::version();          // "4.9.0"
const char *XYO::ExecCGI::Version::build();            // "9"
const char *XYO::ExecCGI::Version::versionWithBuild(); // "4.9.0.9"
const char *XYO::ExecCGI::Version::datetime();         // "2026-10-04 18:55:35"

const char *XYO::ExecCGI::Copyright::copyright();
const char *XYO::ExecCGI::Copyright::publisher();
const char *XYO::ExecCGI::Copyright::company();
const char *XYO::ExecCGI::Copyright::contact();

std::string XYO::ExecCGI::License::license();      // MIT header + copyright + MIT text
std::string XYO::ExecCGI::License::shortLicense(); // copyright + short MIT notice
```

## Windows resources

`Application.rc` adds the icon (`Application.ico`), the version
information (`XYO_PLATFORM_VERSION_INFO`, file name `exec-cgi.exe`,
description "Execute php script as cgi") and `Application.manifest`, which
sets the active code page to **UTF-8**: on Windows 10 1903 and later the
arguments, the environment values and the command line passed to
`cmd.exe` are UTF-8, so non-ASCII script names and query strings survive.

## Testing a change

There is no test project. Build with `fabricare make` and try
`output/bin/exec-cgi` on Windows and on Linux (WSL; a small shell script
named `php-cgi` that prints `$*` and the environment is enough when PHP is
not installed there). Check at least:

| Case | Expect |
|------|--------|
| `exec-cgi --version` | version line, exit `0` |
| `exec-cgi` | usage, exit `2` |
| `exec-cgi "t.php?a=foo&b=5"` with `t.php` printing `$_GET` | headers, `a` and `b`, exit `0` |
| the same with an output file | the same text in the file |
| `exec-cgi "dir with space/t.php?a=1" "out dir/o.html"` | works |
| a script with `exit(3);` | exit `3` |
| a missing script | `No input file specified.`, non-zero exit |
| `php-cgi` not on the `PATH` | the shell's "not found", exit `1` / `127` |

## Command line summary

| Input | Result | Exit code |
|-------|--------|-----------|
| `--help`, `--usage` (only argument) | usage | `0` |
| `--license` | license | `0` |
| `--version` | `version X.Y.Z build N [date]` | `0` |
| nothing | usage | `2` |
| `script[?query]` | response on the console | `php-cgi`'s exit code |
| `script[?query] file` | response in `file` | `php-cgi`'s exit code |
