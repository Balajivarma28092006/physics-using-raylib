
$ErrorActionPreference = "Stop"


$PROJECT_NAME = "game"
$RAYLIB_DIR = "C:/raylib/raylib/src"
$BUILD_DIR = "build"

if (!(Test-Path "$RAYLIB_DIR/raylib.h")) {
    Write-Error "raylib.h not found at $RAYLIB_DIR"
}

if (!(Test-Path "$RAYLIB_DIR/libraylib.a")) {
    Write-Error "libraylib.a not found at $RAYLIB_DIR"
}

if (!(Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Error "g++ not found. MinGW is required."
}


if (Test-Path $BUILD_DIR) {
    Remove-Item -Recurse -Force $BUILD_DIR
}
New-Item -ItemType Directory $BUILD_DIR | Out-Null


cmake `
  -S . `
  -B $BUILD_DIR `
  -G "MinGW Makefiles" `
  -DCMAKE_C_COMPILER=gcc `
  -DCMAKE_CXX_COMPILER=g++ `
  -DRAYLIB_DIR=$RAYLIB_DIR

# ---- BUILD ----
cmake --build $BUILD_DIR

Write-Host "`--> n BUILD SUCCESSFUL"
Write-Host "--> Run with: .\$BUILD_DIR\$PROJECT_NAME.exe"
