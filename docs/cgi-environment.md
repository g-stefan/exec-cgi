# CGI environment

A CGI program gets its request from **environment variables** (CGI/1.1,
RFC 3875). `exec-cgi` sets them in its own process, then runs `php-cgi`
through the system shell; `php-cgi` inherits them and turns them into
`$_SERVER` and `$_GET`.

## What runs

For `exec-cgi "<script>?<query>" <output>`:

```
php-cgi -q -n -d -f "<script>">"<output>"
```

Without an output file, the `>"<output>"` part is left out. The line is
run by `XYO::System::Shell::system`: `cmd.exe /c` on Windows, `/bin/sh -c`
on Linux. `php-cgi` is searched on the `PATH`.

## Variables

| Variable | Value | Used by PHP for |
|----------|-------|-----------------|
| `GATEWAY_INTERFACE` | `CGI/1.1` | switches `php-cgi` to CGI mode |
| `REDIRECT_STATUS` | `200` | allows `php-cgi` to run (see [below](#redirect_status)) |
| `REQUEST_METHOD` | `GET` | `$_SERVER['REQUEST_METHOD']` |
| `QUERY_STRING` | `<query>`, the text after the first `?` | `$_GET`, `$_REQUEST`, `$_SERVER['QUERY_STRING']` |
| `PATH_TRANSLATED` | `<script>` | **the file that is run**; also `$_SERVER['SCRIPT_FILENAME']` |
| `SCRIPT_NAME` | `<script>` | `$_SERVER['SCRIPT_NAME']` |
| `PATH_INFO` | `<script>` | `$_SERVER['PATH_INFO']` |
| `SERVER_REQUEST_URI` | the whole first argument | `$_SERVER['SERVER_REQUEST_URI']` (not a standard name) |
| `SERVER_SOFTWARE` | `exec-cgi` | `$_SERVER['SERVER_SOFTWARE']` |
| `SERVER_NAME` | `exec-cgi` | `$_SERVER['SERVER_NAME']` |
| `REMOTE_ADDR` | `127.0.0.1` | `$_SERVER['REMOTE_ADDR']` |
| `REMOTE_HOST` | `localhost` | `$_SERVER['REMOTE_HOST']` |

Every other variable of the calling environment is passed on as well, so
a page can also be given values with environment variables
(`MY_VAR=1 exec-cgi page.php` on Linux, `set MY_VAR=1` first on Windows →
`$_SERVER['MY_VAR']`, `getenv('MY_VAR')`).

**Not set:** `REQUEST_URI`, `DOCUMENT_ROOT`, `HTTP_HOST` and every other
`HTTP_*` request header, `SERVER_PORT`, `SERVER_PROTOCOL`, `HTTPS`,
`CONTENT_TYPE`, `CONTENT_LENGTH`. A page that builds links from
`$_SERVER['HTTP_HOST']` or `$_SERVER['REQUEST_URI']` gets `null` for them.

On Windows, an empty value removes a variable: with no query string,
`QUERY_STRING` is not set at all; on Linux it is set to an empty string.
PHP gives an empty `$_GET` in both cases.

### Side effects worth knowing

| What the page sees | Why |
|--------------------|-----|
| `getcwd()` is the **folder of the script**, not the folder `exec-cgi` was started in | `php-cgi` changes into the script's folder, as under a web server; relative `include` and file paths in the page work from there |
| `$_SERVER['PHP_SELF']` is the script name twice (`page.phppage.php`) | PHP builds it as `SCRIPT_NAME` + `PATH_INFO`, and both are the script; use `SCRIPT_NAME` instead |
| `$_SERVER['argv']` is not set | in CGI mode command line arguments are not passed to the page |
| `PHP_SAPI` is `cgi-fcgi` | the same `php-cgi` serves CGI and FastCGI |

## REDIRECT_STATUS

`php-cgi` is built with `cgi.force_redirect = 1`: it refuses to run a
script unless the web server says the request was redirected to it, by
setting `REDIRECT_STATUS`. Without it the output is a page that begins
with `<b>Security Alert!</b> The PHP CGI cannot be accessed directly.`

PHP used to accept `HTTP_REDIRECT_STATUS` as well. Since PHP 8.1.30,
8.2.24 and 8.3.12 (CVE-2024-8927: any HTTP client could set it with a
`Redirect-Status:` header) only `REDIRECT_STATUS` is accepted. `exec-cgi`
sets `REDIRECT_STATUS=200`, which works with every PHP version; up to
4.9.0 build 8 it set `HTTP_REDIRECT_STATUS=On`.

## Command line options of php-cgi

| Option | Effect in CGI mode |
|--------|--------------------|
| `-n` | do not load `php.ini`: built-in defaults, no extensions from `php.ini` |
| `-d -f` | `-d` takes `-f` as its argument (defines an ini entry named `-f`); harmless |
| `-q` | **ignored**: headers are always written in CGI mode |
| the script name | **ignored**: the script comes from `PATH_TRANSLATED` |

`php-cgi` reads `-n` and `-d` only when the query string contains an `=`
or is unset (a protection against CVE-2012-1823, where a query such as
`?-s` became a command line option). So:

| Request | `php.ini` | Extensions |
|---------|-----------|------------|
| `page.php?a=1`, `page.php` (Windows) | not loaded | only the ones built into `php-cgi` |
| `page.php?flag` (no `=`), `page.php` (Linux, empty `QUERY_STRING`) | loaded | the ones enabled in `php.ini` |

A page that needs an extension (`mbstring`, `intl`, `gd`, `pdo_sqlite`,
...) may work for one query and fail with `Call to undefined function` for
another. Check with `extension_loaded('mbstring')`, or keep pages to the
built-in functions. `php -m` lists what `php.ini` enables;
`exec-cgi "x.php?a=1"` with `<?php print_r(get_loaded_extensions());`
lists what `-n` leaves.

## Output

`php-cgi` writes a CGI response: header lines, an empty line, then the
page. Lines of the header end with `\r\n`:

```
X-Powered-By: PHP/8.5.7
Content-type: text/html; charset=UTF-8

<!DOCTYPE html>
...
```

- `header()` calls in the page change or add header lines
  (`header('Content-Type: text/plain')`).
- `http_response_code(404)` or a missing script adds a `Status: 404 Not
  Found` line.
- The headers are **always** written, to the console and to the output
  file. To keep only the page, drop everything up to the first empty line:
  `exec-cgi "page.php" | sed '1,/^\r\?$/d' > page.html`.
- The page body is written byte for byte; `exec-cgi` does not change line
  endings or encoding.

## Errors

Without `php.ini`, `display_errors` is on: warnings, notices and fatal
errors are written **into the response**, as HTML, and so into the output
file:

```
X-Powered-By: PHP/8.5.7
Content-type: text/html; charset=UTF-8

<br />
<b>Parse error</b>:  syntax error, unexpected identifier &quot;error&quot; in <b>C:\site\bad.php</b> on line <b>1</b><br />
```

A parse error, an uncaught exception or a fatal error ends `php-cgi` with
exit code `255`; a missing script gives `Status: 404 Not Found` and
`No input file specified.` with a non-zero exit code. Test the exit code
in build scripts; to also catch warnings, search the generated file for
`<b>Warning</b>` / `<b>Notice</b>` / `<b>Deprecated</b>`.

## Security

`exec-cgi` is meant for running **your own** pages on **your own**
machine, from a build or by hand. It is not a web server and not a safe
gateway:

- The script path and the output path are inserted into a **shell command
  line**, inside double quotes. A `"` ends the quoting. Windows `cmd.exe`
  still expands `%VAR%` inside quotes; Linux `sh` expands `$VAR`,
  `$(...)` and backticks. Never build these arguments from untrusted
  input.
- The query string is passed only through the environment, never through
  the shell, so it cannot run commands. It is still input to the page.
- `REDIRECT_STATUS` is set deliberately: it switches off the protection
  that stops `php-cgi` from running a file that was not routed to it by a
  web server. Do not install `exec-cgi` as a CGI handler of a web server.
- Any PHP file the arguments point at is run, with the rights of the user
  who runs `exec-cgi`.
