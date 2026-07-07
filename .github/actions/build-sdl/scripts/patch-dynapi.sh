#!/usr/bin/env bash
# Two source patches needed for the prefixed build, applied to the freshly
# downloaded (pristine) SDL tarball:
#
# 1. The dynapi hard-codes stock symbol names and allows SDL_DYNAMIC_API
#    env-var redirection, so it must be off. SDL deliberately rejects
#    -DSDL_DYNAMIC_API=0 on the command line and requires editing the master
#    switch in src/dynapi/SDL_dynapi.h - this is that edit.
# 2. The ELF version script (src/dynapi/SDL_dynapi.sym) exports the API by
#    exact name and hides everything else, so its entries must carry the
#    same JSP_ prefix.
#
# Requires SDL_SOURCE_DIR in the environment.
set -euo pipefail

DYNAPI_HEADER="$SDL_SOURCE_DIR/src/dynapi/SDL_dynapi.h"
MATCHES=$(grep -c '^#define SDL_DYNAMIC_API 1$' "$DYNAPI_HEADER")
if [ "$MATCHES" -ne 1 ]; then
  echo "ERROR: expected exactly one '#define SDL_DYNAMIC_API 1' master switch, found $MATCHES" >&2
  exit 1
fi
sed -i.bak 's|^#define SDL_DYNAMIC_API 1$|#define SDL_DYNAMIC_API 0 // patched by build-sdl: dynapi is incompatible with the symbol-prefixed build|' "$DYNAPI_HEADER"

sed -i.bak 's|^\( *\)SDL_|\1JSP_SDL_|' "$SDL_SOURCE_DIR/src/dynapi/SDL_dynapi.sym"
