@echo off
cls

:MONTA
masm %1.mep
if not errorlevel 1 goto LINKA
echo .
echo Erro de montagem
goto FIMERRO

:LINKA
link %1.obj
if not errorlevel 1 goto FIM
echo .
echo Erro de linkagem
goto FIMERRO

:FIM
echo .
echo Compilacao OK

:FIMERRO

@echo on
