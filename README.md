# Exec CGI

Execute php script as cgi
- `exec-cgi "page.php?a=foo&b=5" page.html` runs `page.php` through
`php-cgi` as a CGI `GET` request: the page gets `$_GET['a']` and
`$_GET['b']`, and its response is written to `page.html` (or to the
console without a second argument).
- No web server needed: generate static pages from PHP templates in a
build, or test a page from the command line.
- Same tool on Windows and Linux; needs `php-cgi` on the `PATH`; the exit
code of `php-cgi` is passed through.

Built on `xyo-system`.

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - download or build, install PHP, first runs, use in builds
- [Command line](docs/command-line.md) - options, the request, the output file, quoting, exit codes
- [CGI environment](docs/cgi-environment.md) - variables, `php.ini`, output format, errors, security
- [Reference](docs/reference.md) - source layout, `Application` class, macros, metadata

A Claude Code skill for this tool is in
[.claude/skills/exec-cgi](.claude/skills/exec-cgi/SKILL.md).

## License

Copyright (c) 2014-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
