@echo off
call povars64.bat
porc main.rc
pocc main.c /Ze /Os /std:c23 
polink /nodefaultlib /entry:main /release /subsystem:windows /out:SleepSoundly.exe main.obj main.res kernel32.lib shell32.lib user32.lib winmm.lib
del /s /q *.res *.obj
pause
