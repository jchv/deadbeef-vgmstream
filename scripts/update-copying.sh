#!/bin/sh
set -e

VGMSTREAM_ROOT=${VGMSTREAM_ROOT:-./vgmstream}
VGMSTREAM_COPYING="${VGMSTREAM_ROOT}/COPYING"

if [ ! -f "$VGMSTREAM_COPYING" ]; then
    echo "error: $VGMSTREAM_COPYING not found" >&2
    exit 1
fi

cat > COPYING << 'EOF'
deadbeef-vgmstream
--
Copyright (c) 2014 John Chadwick <john@jchw.io>

Licensed under the same terms as vgmstream (ISC license, see below.)

vgmstream
--
EOF
tr -d '\r' < "$VGMSTREAM_COPYING" >> COPYING

echo "Updated COPYING from $VGMSTREAM_COPYING"
