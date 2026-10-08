---
name: exec-cgi
description: >-
  How to use exec-cgi, the XYO command line tool (C++, namespace
  XYO::ExecCGI, on top of xyo-system) that runs a PHP page through php-cgi
  as a CGI/1.1 GET request without a web server: exec-cgi
  "page.php?query" [output file] sets QUERY_STRING, REDIRECT_STATUS,
  PATH_TRANSLATED, ... and runs php-cgi through the system shell, so the
  page gets $_GET; used to generate static .html from .php templates in
  builds. Covers options, quoting the request, the CGI headers always at
  the top of the response and how to strip them, php.ini loaded only when
  the query has no =, PHP errors written into the output, getcwd() and
  $_SERVER quirks, exit codes, php-cgi needed on the PATH,
  REDIRECT_STATUS vs HTTP_REDIRECT_STATUS, the broken 4.9.0 build 8, and
  shell-injection limits. Use when running PHP pages from the command
  line or a build with exec-cgi, writing scripts that call it, or working
  inside the exec-cgi repository.
---

# exec-cgi

Runs a PHP page as a web server would for a `GET` request, without a web
server:

```
exec-cgi "page.php?lang=en&id=5" page.html
```

`page.php` gets `$_GET = ['lang' => 'en', 'id' => '5']`; the response goes
to `page.html` (or to the console without a second argument).

A tool, not a library: there is nothing to link. Built on `xyo-system`
(see the `xyo-system` skill when changing the source). Needs `php-cgi`
(not `php`) on the `PATH`.

Full documentation: `docs/` in the exec-cgi repository
(`X:\Storage\XYO\Gitea\CPP\exec-cgi\docs` on this machine): README,
getting-started, **command-line**, **cgi-environment**, reference. The
whole implementation is `source/XYO/ExecCGI/Application.cpp` (about 130
lines).

## Pick a form

| Want | Command |
|------|---------|
| Response on the console | `exec-cgi "page.php?a=1&b=2"` |
| Response in a file | `exec-cgi "page.php?a=1&b=2" page.html` |
| No parameters | `exec-cgi page.php [page.html]` |
| Arrays, spaces, special characters | URL syntax: `"list.php?tag[]=a&tag[]=b"`, `"s.php?q=hello+world"`, `%XX` |
| Paths with spaces | `exec-cgi "my dir/p.php?x=1" "out dir/p.html"` |
| Only the page, without the CGI headers | `exec-cgi "p.php?x=1" \| sed '1,/^\r\?$/d' > p.html` |
| Extra values for the page | environment variables, inherited: `MY_VAR=1 exec-cgi p.php` → `getenv('MY_VAR')` |

## Hard rules

1. **Quote the request.** Unquoted, `&` ends the command in every shell
   and `?` is a Linux wildcard. In `.cmd` files write `%%` for `%`.
2. **Split at the first `?`**: before it is the script **file path**
   (relative to the current directory, no document root), after it the
   query string, passed as is (PHP decodes `%XX`, `+`, `a[]=`).
3. **The response always starts with headers**
   (`X-Powered-By: PHP/x`, `Content-type: text/html; charset=UTF-8`, then
   an empty line; `\r\n` line ends), also in the output file — `php-cgi`
   ignores `-q` in CGI mode. Strip up to the first empty line if only the
   page is wanted. `header()` / `http_response_code()` add lines
   (`Status: 404 Not Found`).
4. **Only `GET`**: no POST body, cookies, `HTTP_*` headers, `REQUEST_URI`,
   `DOCUMENT_ROOT`, `HTTP_HOST`, `SERVER_PORT` (all `null` in `$_SERVER`).
   `SERVER_NAME` / `SERVER_SOFTWARE` = `exec-cgi`, `REMOTE_ADDR` =
   `127.0.0.1`, the whole argument is in `$_SERVER['SERVER_REQUEST_URI']`.
5. **Page context**: `getcwd()` is the script's folder (relative includes
   work from there); `PHP_SELF` is the script name twice (use
   `SCRIPT_NAME`); no `$argv`; `PHP_SAPI == 'cgi-fcgi'`.
6. **php.ini depends on the query**: if the query contains `=` (or is
   absent on Windows) `-n` applies → no `php.ini`, only built-in
   extensions; if it has no `=` (`?flag`, or empty on Linux) `php.ini` is
   loaded. Check `extension_loaded()` before relying on `mbstring`,
   `intl`, `gd`, PDO drivers.
7. **Errors go into the response**: without `php.ini`, `display_errors` is
   on, so warnings and fatal errors are written as HTML into the output
   file. Exit code: `php-cgi`'s — `0`, the script's `exit(N)` (low 8
   bits), `255` for parse error / uncaught exception / fatal error,
   non-zero for a missing script (`No input file specified.`). `php-cgi`
   not found: Windows `1`, Linux `127`. Output folder missing: Windows
   `1`, Linux `2` (the shell, PHP not run). No argument: usage, `2`.
8. **Options only as the sole argument**, exact: `--help`, `--usage`,
   `--license`, `--version` → exit `0`. `exec-cgi --version x` runs a
   script named `--version` into file `x`.
9. **Version**: 4.9.0 build 8 and earlier are broken — they exit `1`
   before running PHP (inverted `putenv` check) and set
   `HTTP_REDIRECT_STATUS`, which PHP ≥ 8.1.30 / 8.2.24 / 8.3.12 ignores
   (`Security Alert! The PHP CGI cannot be accessed directly.`). Use build
   9 or later; check with `exec-cgi --version`.
10. **Trusted input only**: script and output paths go into a shell
    command inside `"..."` (`%VAR%` on Windows, `$VAR` / `` ` `` /
    `$(...)` on Linux still expand; a `"` in a path breaks it). The query
    goes only through the environment. Never wire exec-cgi to a web
    server — it sets `REDIRECT_STATUS` on purpose.

## In scripts

```bat
rem Windows batch: one page per language
for %%l in (en ro de) do (
	exec-cgi "page.php?lang=%%l" "output\page.%%l.html"
	if errorlevel 1 exit /b 1
)
```

```bash
# Linux
for l in en ro de; do
	exec-cgi "page.php?lang=$l" "output/page.$l.html" || exit 1
	grep -q '<b>\(Warning\|Notice\|Deprecated\)</b>' "output/page.$l.html" && { echo "PHP warning in $l"; exit 1; }
done
```

```js
// fabricare script, exec-cgi installed in the SDK bin
exitIf(Shell.system("exec-cgi \"page.php?lang=en\" output/page.en.html"));
```

## Working inside this repository

- One fabricare project, `exec-cgi` (`make: exe`, source
  `source/XYO/ExecCGI`, depends on `xyo-system`). `fabricare make`,
  `fabricare install`, `fabricare release`; there is no test project:
  try changes with `output/bin/exec-cgi` on Windows (php-cgi is installed)
  and on Linux (WSL, see the `fabricare` skill; a fake `php-cgi` shell
  script printing `$*` and the environment is enough). The case list is
  in `docs/reference.md`.
- `Application::main`: options, `request.split2("?", script, query)`,
  `Shell::setenv` per variable, build `php-cgi -q -n -d -f "<script>"`
  [`>"<output>"`], `return Shell::system(cmdLine)`.
- Keep `Shell::setenv` (returns `true` on success, copies the value) — C
  `putenv` returns `0` on success and on Linux keeps the pointer. Keep the
  call qualified as `Shell::system(...)`: a bare `system(...)` compiles to
  C `::system`, whose Linux wait status is not an exit code.
- Keep `REDIRECT_STATUS` (not `HTTP_REDIRECT_STATUS`).
- `XYO_EXECCGI_LIBRARY` leaves out `main` (`XYO_APPLICATION_MAIN`).
  `Version.rh` is generated from `Version.Template.rh` and `version.json`.
- Keep `docs/` and this skill in step with `Application.cpp` when the
  behavior changes (the outputs in the docs are real tool output).
- Licensing follows REUSE: `source/`, `docs/` and `README.md` are MIT
  (source files also carry SPDX headers); config, `fabricare.json`,
  `version.json` and `.claude/` are Unlicense. Every new top level file or
  folder needs a `Files:` entry in `.reuse/dep5` (check with
  `python -m reuse lint`).
