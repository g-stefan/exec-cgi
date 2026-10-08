# Command line

```
exec-cgi <script>[?<query>] [<output file>]
exec-cgi --help | --usage | --license | --version
```

## Options

| Option | Effect |
|--------|--------|
| `--help`, `--usage` | print usage, exit `0` |
| `--license` | print the MIT license, exit `0` |
| `--version` | print `version X.Y.Z build N [date time]`, exit `0` |

An option is recognized only when it is the **only** argument and is
spelled exactly as above. Anything else is taken as a request:
`exec-cgi --version extra` runs a script named `--version` and writes the
result to `extra` (`php-cgi` answers `No input file specified.`).

With no argument at all, `exec-cgi` prints the usage and exits `2`.

## The request

The first argument is split at the **first** `?`:

| Argument | Script | Query string |
|----------|--------|--------------|
| `page.php?lang=en&id=5` | `page.php` | `lang=en&id=5` |
| `page.php` | `page.php` | empty |
| `page.php?` | `page.php` | empty |
| `page.php?a=1?b=2` | `page.php` | `a=1?b=2` |
| `dir/page.php?x=a%20b` | `dir/page.php` | `x=a%20b` (PHP decodes it: `$_GET['x'] == 'a b'`) |

- **Script** is a file path, relative to the current directory or
  absolute. It is not a URL path: there is no document root. Use `/` or
  `\` as the system allows.
- **Query string** is passed to PHP as it is. Write it the way it would
  appear in a URL: `&` between parameters, `%XX` or `+` for special
  characters and spaces. PHP decodes it into `$_GET` (`a[]=1&a[]=2` gives an
  array, as on a web server).

The script, the query string and the whole argument become CGI environment
variables; see [CGI environment](cgi-environment.md).

## The output file

| Command | The response goes to |
|---------|----------------------|
| `exec-cgi "page.php?x=1"` | the console (standard output) |
| `exec-cgi "page.php?x=1" page.html` | `page.html`, created or overwritten |

The output file is opened by the system shell (`> "<output file>"`), so:

- the folder must exist; `exec-cgi` does not create it, the shell reports
  the error and PHP is not run;
- the file is created (empty) even when PHP fails, and then holds the
  error page;
- only standard output goes to the file; messages from the shell itself
  (for example `'php-cgi' is not recognized ...`) still appear on the
  console.

A third and later arguments are ignored.

## Quoting

Always quote the request. Without quotes the shell you type in sees the
special characters first:

| Typed | What happens |
|-------|--------------|
| `exec-cgi page.php?a=1&b=2` | Windows `cmd.exe` and Linux shells cut the line at `&`: `page.php?a=1` is run, then `b=2` as a separate command (on Linux, `exec-cgi` is put in the background) |
| `exec-cgi page.php?a=1` (Linux) | `?` is a wildcard: if a file `page.phpXa=1` exists, its name is passed instead |
| `exec-cgi "page.php?a=1&b=2"` | correct, on every system |

Single quotes work in Linux shells and PowerShell (`'page.php?a=1&b=2'`),
not in `cmd.exe`.

In a `.cmd` / `.bat` file, `%` starts a variable: write `%%` for a literal
`%` in a query (`"page.php?x=a%%20b"`).

Paths with spaces work for both the script and the output file:

```
exec-cgi "my pages/index.php?lang=en" "out dir/index.html"
```

`exec-cgi` puts each of them in double quotes on the command line it runs
(`php-cgi -q -n -d -f "my pages/index.php">"out dir/index.html"`). A path
that itself contains `"` cannot be used.

## Exit codes

| Case | Exit code |
|------|-----------|
| `--help`, `--usage`, `--license`, `--version` | `0` |
| no argument | `2` (and the usage is printed) |
| the page ran | `0`, or the value of `exit(N)` in the script |
| PHP parse error, uncaught exception, fatal error | `255` |
| script not found (`No input file specified.`) | non-zero (`127` with PHP 8.5 on Windows) |
| `php-cgi` not on the `PATH` | Windows `1` (from `cmd.exe`), Linux `127` (from `sh`) |
| the folder of the output file does not exist | Windows `1` (`The system cannot find the path specified.`), Linux `2` (from `sh`) |
| a CGI variable could not be set | `1` |

The exit code is the one of `php-cgi`, returned through
`XYO::System::Shell::system`: on Linux it is converted from the wait status
like a shell does (killed by signal `S` → `128 + S`). PHP keeps only the
low 8 bits of `exit(N)` on every system: `exit(300)` → `44`.
So `exec-cgi "page.php" page.html && next-step` and `if errorlevel 1` work
as expected.

**exec-cgi 4.9.0 build 8 and earlier** (the 4.9.0 release archives) do not
work: they exit `1` before `php-cgi` is started, on every system, because
the result of `putenv()` was tested the wrong way round. They also set
`HTTP_REDIRECT_STATUS` instead of `REDIRECT_STATUS`, which current PHP
versions refuse ([details](cgi-environment.md#redirect_status)).

## Examples

```
exec-cgi "index.php" index.html
exec-cgi "index.php?lang=ro" index.ro.html
exec-cgi "report.php?from=2026-01-01&to=2026-06-30" report.html
exec-cgi "list.php?tag[]=cpp&tag[]=php"
exec-cgi "C:\www\site\page.php?id=7" C:\temp\page.html
exec-cgi "search.php?q=hello+world"
```
