#!/bin/sh
set -e

awk '
BEGIN {
    copying = "COPYING"
    n = 0
    while ((getline line < copying) > 0) {
        n++
        gsub(/\\/, "\\\\", line)
        gsub(/"/, "\\\"", line)
        lines[n] = line
    }
    close(copying)

    # Build the macro definition text
    macro = "#define COPYRIGHT_STR \\\n"

    for (i = 1; i <= n; i++) {
        content = "  \"" lines[i] "\\n\""
        if (i < n) {
            macro = macro content " \\\n"
        } else {
            macro = macro content "\n"
        }
    }

    replacing = 0
}

/^#define COPYRIGHT_STR/ {
    printf "%s", macro
    replacing = 1
    next
}

replacing {
    if (/\\[[:space:]]*$/) {
        next
    } else {
        replacing = 0
        next
    }
}

{ print }
' vgm.c > vgm.c.tmp && mv vgm.c.tmp vgm.c

# ...And run clang-format, just to get its behavior for line wrapping...
if command -v clang-format >/dev/null 2>&1; then
    clang-format -i vgm.c
else
    >&2 echo "Warning: skipping clang-format. This is OK during packaging."
fi

>&2 echo "Updated COPYRIGHT_STR from COPYING"
