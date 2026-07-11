/* Minimal libintl_setlocale() implementation.
 *
 * Real gettext's setlocale.c wraps the platform setlocale() to also
 * respect the locale chosen by the user (via environment variables) on
 * platforms (Windows, macOS) whose own setlocale("", ...) doesn't already
 * do that -- but it pulls in a substantial extra gnulib dependency chain
 * (setlocale-fixes.h, localename.h, stdcountof.h, ...) this build
 * deliberately doesn't vendor (see build.zig's comment on intl_sources).
 *
 * libgnuintl.in.h redirects plain setlocale() calls to libintl_setlocale()
 * on Windows/macOS/Cygwin unconditionally (see its
 * "Support for the locale chosen by the user" section), so any Windows
 * consumer of this library that calls setlocale() -- including glib's
 * gdatetime.c era-lookup code -- needs this symbol to exist, even if
 * nothing yet relies on the extra locale-name normalization the real
 * implementation adds. This is a plain passthrough to the real, current
 * libc/mingw-w64 setlocale(): correct, just without that extra polish.
 *
 * Deliberately does NOT include libintl.h/libgnuintl.h: that header is
 * exactly what defines the setlocale->libintl_setlocale redirection, and
 * including it here would make the call below recurse into itself.
 */
#include <locale.h>

const char *libintl_setlocale(int category, const char *locale) {
    return setlocale(category, locale);
}
