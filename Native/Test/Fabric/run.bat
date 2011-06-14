@echo off

set FABRICCMD=..\..\build\Windows\x86\Release\Fabric\CLI\fabric.exe
rem set FABRICCMD=..\..\build\Windows\x86\Debug\Fabric\CLI\fabric.exe
rem set FABRICCMD=..\..\Fabric_Win32\build\Win32\Debug\bin\fabric.exe
rem  set FABRICCMD=..\..\Fabric_Win32\build\Win32\Release\bin\fabric.exe

echo Using command: %KLCMD%

if "%1"=="" (
  for %%F in (*.js) do call :validate %%~nF
) else (
  call :validate %~n1
)

exit /B %ERRORLEVEL%

:validate

set OUT=%TEMP%\fabric.out

%FABRICCMD% %1.js 2>&1 > %OUT%
if %ERRORLEVEL% NEQ 0 (
  echo FAIL %1
  echo Unexpected failure:
  type %OUT%
  exit /B 1
)

fc %OUT% %1.out 2>&1 > NUL:
if %ERRORLEVEL% NEQ 0 (
  echo FAIL %1
  echo Expected output:
  type %1.out
  echo Actual output:
  type %OUT%
  exit /B 1
)

echo PASS %1

goto :EOF

