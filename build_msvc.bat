@echo off
call c:\BuildTools\devcmd.bat
rc /nologo main.rc
cl /nologo /GS- /GF /Os /O1 /DNDEBUG /Tc main.c main.res kernel32.lib shell32.lib user32.lib winmm.lib /link /nodefaultlib /entry:main /Fe:sleep_soundly.exe
pause
