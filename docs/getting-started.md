# Getting started

## 1. Get the executable

### From a release

Each release has these archives (`release/` after `fabricare release`, or
the GitHub releases page):

| Archive | Contents | Needs |
|---------|----------|-------|
| `xyo.exec-cgi.vX.Y.Z.win64-msvc-2026.static.bin.zip` | `exec-cgi.exe`, static build | nothing, copy it anywhere on the `PATH` |
| `xyo.exec-cgi.vX.Y.Z.win64-msvc-2026.bin.zip` | `exec-cgi.exe` | `xyo-platform.dll`, `xyo-managed-memory.dll`, `xyo-system.dll` (and the layers they use) next to it or on the `PATH` — the XYO SDK |
| `xyo.exec-cgi.vX.Y.Z.ubuntu-24.04.bin.zip`, `ubuntu-26.04` | `exec-cgi` | `xyo-system.so` and the layers below on the library path (`~/.fabricare/<platform>/bin` or `LD_LIBRARY_PATH`) |
| `xyo.exec-cgi.vX.Y.Z.sha512.json` | SHA-512 of every archive | |

On a machine without the XYO SDK, use the static Windows build. The Linux
build started without its libraries fails with
`error while loading shared libraries: xyo-system.so`, exit code `127`.

**Use 4.9.0 build 9 or later.** 4.9.0 build 8 and earlier exit with code
`1` without running PHP (see [Command line](command-line.md#exit-codes)).

### Build it

`exec-cgi` is built with [fabricare](https://github.com/g-stefan/fabricare).
`xyo-platform`, `xyo-managed-memory`, `xyo-data-structures`,
`xyo-multithreading`, `xyo-encoding` and `xyo-system` must be installed to
the SDK first. From the repository root:

```bash
fabricare make       # build into output/  (output/bin/exec-cgi[.exe])
fabricare install    # copy output/bin to ~/.fabricare/<platform>/bin
fabricare clean      # remove output/ and temp/
```

After `install`, `exec-cgi` is in `~/.fabricare/<platform>/bin`, which is
on the `PATH` of a fabricare build (and usually of the developer's shell).
fabricare sets up the compiler environment (MSVC `vcvarsall.bat`) by
itself. `fabricare release` packs `output/` into the `release/` archives
listed above.

## 2. Get php-cgi

`exec-cgi` starts the program `php-cgi`, which must be on the `PATH`:

| System | Where `php-cgi` comes from |
|--------|----------------------------|
| Windows | the PHP zip from [windows.php.net](https://windows.php.net/download/) has `php-cgi.exe` next to `php.exe`; add that folder to the `PATH` |
| Debian / Ubuntu | `sudo apt install php-cgi` (`/usr/bin/php-cgi`) |

Check it:

```
php-cgi -v
```

```
PHP 8.5.7 (cgi-fcgi) (built: Jun  2 2026 21:13:51) (ZTS Visual C++ 2022 x64)
```

The `php` command line program (`php.exe`, `php-cli`) is **not** enough:
it does not read the CGI environment.

## 3. First runs

```
exec-cgi --version
```

```
version 4.9.0 build 9 [2026-10-04 18:55:35]
```

A page, `hello.php`:

```php
<?php
echo "Hello, " . htmlspecialchars($_GET['name'] ?? 'world') . "!\n";
```

Run it with a query string — quote the whole request:

```
exec-cgi "hello.php?name=Ana"
```

```
X-Powered-By: PHP/8.5.7
Content-type: text/html; charset=UTF-8

Hello, Ana!
```

The first lines are the HTTP headers `php-cgi` writes before the page; a
web server would send them to the browser. They are always there
([why](cgi-environment.md#output)).

Write the response to a file instead of the console:

```
exec-cgi "hello.php?name=Ana" hello.html
```

Without a query string, `$_GET` is empty:

```
exec-cgi hello.php
```

```
X-Powered-By: PHP/8.5.7
Content-type: text/html; charset=UTF-8

Hello, world!
```

## 4. Generate pages in a build

One page per language, from a Windows batch file:

```bat
for %%l in (en ro de) do (
	exec-cgi "page.php?lang=%%l" "output\page.%%l.html"
	if errorlevel 1 exit /b 1
)
```

From a shell script on Linux:

```bash
for l in en ro de; do
	exec-cgi "page.php?lang=$l" "output/page.$l.html" || exit 1
done
```

From a fabricare script (`fabricare/*.js`):

```js
exitIf(Shell.system("exec-cgi \"page.php?lang=en\" output/page.en.html"));
```

To keep only the page, drop everything up to and including the first
empty line (the headers end with `\r\n\r\n`):

```bash
exec-cgi "page.php?lang=en" | sed '1,/^\r\?$/d' > page.en.html
```

(`sed` from Git Bash or WSL on Windows.)
