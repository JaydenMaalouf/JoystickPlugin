# Fails the build unless the built DLL exports only JSP_-prefixed SDL
# symbols (no stock SDL_x name leaked through) and the renamed API is
# actually present. Windows equivalent of verify-exports.sh (uses dumpbin
# instead of nm).
param(
  [Parameter(Mandatory = $true)][string]$DllPath
)

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$dumpbin = & $vswhere -latest -find '**\Hostx64\x64\dumpbin.exe' | Select-Object -First 1
$exports = & $dumpbin /exports $DllPath | Out-String

$stock = $exports -split "`n" | Select-String -Pattern '\sSDL_[A-Za-z0-9_]+\s*$'
if ($stock) {
  Write-Error "Unprefixed SDL symbols exported: $($stock | Select-Object -First 5)"
  exit 1
}
if ($exports -notmatch 'JSP_SDL_Init') {
  Write-Error 'JSP_SDL_Init not exported'
  exit 1
}

Write-Host 'Verified: only JSP_-prefixed SDL symbols exported'
