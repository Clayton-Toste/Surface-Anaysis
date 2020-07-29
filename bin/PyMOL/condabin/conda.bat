@REM Copyright (C) 2012 Anaconda, Inc
@REM SPDX-License-Identifier: BSD-3-Clause

@REM echo _CE_CONDA is %_CE_CONDA%
@REM echo _CE_M is %_CE_M%
@REM echo CONDA_EXE is %CONDA_EXE%

@IF NOT DEFINED _CE_CONDA (
  @SET _CE_M=
  @SET "CONDA_EXE=%~dp0..\Scripts\conda.exe"
)
@IF [%1]==[activate]   "%~dp0_conda_activate" %*
@IF [%1]==[deactivate] "%~dp0_conda_activate" %*

@SETLOCAL EnableDelayedExpansion
@IF DEFINED _CE_CONDA (
  @REM when _CE_CONDA is defined, we're in develop mode.  CONDA_EXE is actually python.exe in the root of the dev env.
  FOR %%A IN ("%CONDA_EXE%") DO @SET _sysp=%%~dpA
) ELSE (
  @REM This is the standard user case.  This script is run in root\condabin.
  FOR %%A IN ("%~dp0.") DO @SET _sysp=%%~dpA
  IF NOT EXIST "!_sysp!\Scripts\conda.exe" @SET "_sysp=!_sysp!..\"
)
@SET _sysp=!_sysp:~0,-1!
@SET PATH=!_sysp!;!_sysp!\Library\mingw-w64\bin;!_sysp!\Library\usr\bin;!_sysp!\Library\bin;!_sysp!\Scripts;!_sysp!\bin;%PATH%
@SET CONDA_EXES="%CONDA_EXE%" %_CE_M% %_CE_CONDA%
@CALL %CONDA_EXES% %*
@ENDLOCAL

@IF %errorlevel% NEQ 0 EXIT /B %errorlevel%

@IF [%1]==[install]   "%~dp0_conda_activate" reactivate
@IF [%1]==[update]    "%~dp0_conda_activate" reactivate
@IF [%1]==[upgrade]   "%~dp0_conda_activate" reactivate
@IF [%1]==[remove]    "%~dp0_conda_activate" reactivate
@IF [%1]==[uninstall] "%~dp0_conda_activate" reactivate

@EXIT /B %errorlevel%
