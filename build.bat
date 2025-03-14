echo OFF

@REM run env.bat before this

@REM clean
del /Q /F build

@REM build
cmake -B build -G "Ninja" ^
-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE ^
-DCMAKE_BUILD_TYPE=Debug

ninja -C build
