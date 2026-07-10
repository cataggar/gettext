const std = @import("std");

// gettext-runtime/intl's LIBINTLSOURCES (see its Makefile.am), minus:
// - printf.c (and its vasnprintf/asnprintf/printf-args/printf-parse gnulib
//   dependency chain): only provides the optional libintl_printf/
//   libintl_fprintf convenience wrappers, not gettext()/dgettext()/
//   bindtextdomain()/textdomain() themselves, which is all consumers like
//   glib actually call (see Makefile.am's own comment on which files
//   "implement public API declared in libgnuintl.in.h").
// - setlocale.c: an internal helper (a portable libintl_setlocale
//   wrapper), not part of that public API surface, and pulls in a much
//   larger gnulib dependency chain (setlocale-fixes.h, localename.h,
//   stdcountof.h, ...) for locale-name normalization this build doesn't
//   need. log.c (optional untranslated-string debug logging) IS
//   included -- dcigettext.c calls into it unconditionally.
const intl_sources = [_][]const u8{
    "bindtextdom.c",
    "dcgettext.c",
    "dgettext.c",
    "gettext.c",
    "finddomain.c",
    "hash-string.c",
    "loadmsgcat.c",
    "localealias.c",
    "textdomain.c",
    "l10nflist.c",
    "explodename.c",
    "dcigettext.c",
    "dcngettext.c",
    "dngettext.c",
    "ngettext.c",
    "plural.c", // bison-generated from plural.y, see plural.c's own header comment
    "plural-exp.c",
    "langprefs.c",
    "log.c",
    "version.c",
    "osdep.c",
    "intl-compat.c",
    "compat.c",
    "localcharset.c", // vendored from libiconv's libcharset module (see comment there)
};

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });
    mod.addIncludePath(b.path("gettext-runtime/intl"));
    mod.addCMacro("HAVE_CONFIG_H", "1");
    mod.addCMacro("LOCALEDIR", "\"\"");
    mod.addCMacro("LOCALE_ALIAS_PATH", "\"\"");

    mod.addCSourceFiles(.{
        .root = b.path("gettext-runtime/intl"),
        .files = &intl_sources,
        .flags = &.{ "-std=gnu99", "-w" },
    });

    const lib = b.addLibrary(.{
        .name = "intl",
        .linkage = .static,
        .root_module = mod,
    });
    lib.installHeader(b.path("gettext-runtime/intl/libgnuintl.h"), "libintl.h");
    b.installArtifact(lib);
}
