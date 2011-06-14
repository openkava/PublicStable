@echo off
if "%1" == "" (
  echo "%0 <directory>"
  exit /b 1
)

set ARCHIVE=%1-patch.tar.bz2

echo Creating %ARCHIVE%

if exist %ARCHIVE% del /q %ARCHIVE%

..\..\bin\windows\tar.exe -cf - %1 | ..\..\bin\windows\bzip2 > %ARCHIVE%
rmdir /s/q %1
