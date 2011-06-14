@echo off

set KLCMD=..\..\build\Windows\x86\Release\Fabric\Tools\kl.exe
rem set KLCMD=..\..\build\Windows\x86\Debug\Fabric\Tools\kl.exe
rem set KLCMD=..\..\Fabric_Win32\build\Win32\Debug\bin\kl.exe
rem set KLCMD=..\..\Fabric_Win32\build\Win32\Release\bin\kl.exe

echo Using command: %KLCMD%

if "%1"=="" (
  for %%F in (*.kl) do call :validate %%~nF
) else (
  call :validate %~n1
)

exit /B %ERRORLEVEL%

:validate

set OUT=%TEMP%\kl.out

%KLCMD% --run %1.kl 2>&1 > %OUT%
if %ERRORLEVEL% NEQ 0 (
  echo FAIL %1
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

