#!/usr/bin/env bash
# Fails the build unless the built library exports only JSP_-prefixed SDL
# symbols (no stock SDL_x name leaked through) and the renamed API is
# actually present. Used for the Mac and Linux builds (Windows uses
# verify-exports.ps1, since it needs dumpbin instead of nm).
#
# Usage: verify-exports.sh <library-path> <mach-o-underscore: "_" or ""> <nm-args...>
set -euo pipefail
LIBRARY="$1"
UNDERSCORE="$2"
shift 2

EXPORTS=$(nm "$@" "$LIBRARY" | awk '{print $3}')

STOCK=$(grep "^${UNDERSCORE}SDL_" <<< "$EXPORTS" || true)
if [ -n "$STOCK" ]; then
  echo "ERROR: unprefixed SDL symbols exported:" >&2
  echo "$STOCK" | head >&2
  exit 1
fi

grep -q "^${UNDERSCORE}JSP_SDL_Init$" <<< "$EXPORTS" || {
  echo "ERROR: JSP_SDL_Init not exported" >&2
  exit 1
}

echo "Verified: only JSP_-prefixed SDL symbols exported"
