@echo off
setlocal EnableDelayedExpansion

set "CX=C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8"
set "FLASH_TOOL=flash\stm8flash.exe"
set "MCU=stm8s103f3"
set "LKF_FILE=temp.lkf"

del /S *.o
del /Q /F *.o
del /Q /F *.hex
del /Q /F *.elf
del /Q /F *.out

if not exist "%CX%\cxstm8.exe" (
  echo ERROR: cxstm8.exe not found!
  exit /b 1
)

echo Compiling all .c files(realiz) in root folder...
set COMPILE_OK=1

for %%f in (*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)

for %%f in (api\src\*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)

for %%f in (drivers\src\*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)

if %COMPILE_OK%==0 (
  echo ERROR: One or more files failed to compile!
  exit /b 1
)

if not exist "%LKF_FILE%" (
  echo ERROR: Linker file %LKF_FILE% not found!
  exit /b 1
)

echo Linking...
"%CX%\clnk.exe" -o main.sm8 "%LKF_FILE%" || exit /b 1

echo Generating HEX...
"%CX%\chex.exe" -fi -o main.hex main.sm8 || exit /b 1

if not exist main.hex (
  echo ERROR: main.hex not found!
  exit /b 1
)

del /Q /F *.o
del /Q /F *.sm8

echo Compiling all .c files (debug) in root folder...
set COMPILE_OK=1

for %%f in (*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" +debug -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)
for %%f in (api\src\*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" +debug -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)

for %%f in (drivers\src\*.c) do (
  echo Compiling %%f...
  "%CX%\cxstm8.exe" +debug -i"%~dp0api\inc" -i"%~dp0drivers\inc" "%%f" || set COMPILE_OK=0
)

if !COMPILE_OK! EQU 0 (
  echo ERROR: One or more files failed to compile!
  exit /b 1
)

if not exist "%LKF_FILE%" (
  echo ERROR: Linker file %LKF_FILE% not found!
  exit /b 1
)

echo Linking...
"%CX%\clnk.exe" -o main.out "%LKF_FILE%" || (
  echo Linking failed!
  exit /b 1
)

echo Converting OMF (.out) to ELF...
"%CX%\cvdwarf.exe" -o main.elf main.out || (
  echo Conversion to ELF failed!
  exit /b 1
)

echo Build(debug) complete.

if not exist "%FLASH_TOOL%" (
  echo ERROR: stm8flash.exe not found in 'flash\' folder!
  exit /b 1
)

echo Flashing to STM8...
"%FLASH_TOOL%" -c stlinkv2 -p %MCU% -w main.hex

if errorlevel 1 (
  echo ERROR: Flashing failed!
  exit /b 1
) else (
  echo Flashing successful!
)

exit /b 0