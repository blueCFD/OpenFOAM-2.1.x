@echo off

rem ------------------------------------------------------------------------------
rem  License
rem     This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
rem     For more information about these patches, visit:
rem         http://www.bluecape.com.pt/blueCFD
rem
rem     OpenFOAM's official site: http://www.openfoam.com
rem
rem     OpenFOAM is free software: you can redistribute it and/or modify it
rem     under the terms of the GNU General Public License as published by
rem     the Free Software Foundation, either version 3 of the License, or
rem     (at your option) any later version.
rem
rem     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
rem     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
rem     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
rem     for more details.
rem
rem     You should have received a copy of the GNU General Public License
rem     along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
rem 
rem  Script
rem      batchrc.bat
rem 
rem  Description
rem      This batch file initializes the OpenFOAM environment for running in the 
rem    Windows Command Line.
rem 
rem ------------------------------------------------------------------------------

%~d0
cd "%~dp0.."
call :SETHOME "%CD%"

rem BEGGINING of Default options --------------------------------------

rem - Compiler:
rem PICK ONE from these:
rem   i686-w64-mingw32  - using the 32bit compiler from mingw-w64 at sourceforge.net
rem   x86_64-w64-mingw32  - using the 64bit compiler from mingw-w64 at sourceforge.net
rem   mingw32, mingw-w32, mingw-w64  - using the custom build of mingw(-w64) cross-compiler
rem   i586-mingw32msvc, amd64-mingw32msvc  - attempt to use Ubuntu's pre-built mingw-w64 binaries, but the 64bit version only worked in Windows XP x64
set WM_COMPILER=mingw-w32

rem - MPI implementation:
rem PICK ONE: "", OPENMPI, MPICH or MSMPI
set WM_MPLIB=OPENMPI

rem - Precision:
rem PICK ONE: SP or DP
set WM_PRECISION_OPTION=DP

rem END of Default options --------------------------------------------

rem BEGGINING of Process summoned options ---------------------------
FOR %%A IN (%*) DO (
  set %%A
)
rem END of Process summoned options ---------------------------------

set USER=ofuser
set USERNAME=ofuser
set WM_PROJECT=OpenFOAM
set WM_PROJECT_VERSION=2.0
set FOAM_INST_DIR=%HOME%
rem set FOAM_INST_DIR=%HOME%\%WM_PROJECT%
set WM_PROJECT_INST_DIR=%FOAM_INST_DIR%
set WM_PROJECT_DIR=%FOAM_INST_DIR%\%WM_PROJECT%-%WM_PROJECT_VERSION%
set WM_PROJECT_USER_DIR=%HOME%\%USER%-%WM_PROJECT_VERSION%
set WM_THIRD_PARTY_DIR=%WM_PROJECT_INST_DIR%\ThirdParty-%WM_PROJECT_VERSION%

set WM_OS=MSwindows
set WM_ARCH=linux
set WM_COMPILE_OPTION=Opt

IF "%WM_COMPILER%"=="i686-w64-mingw32"    set WM_ARCH_OPTION=32
IF "%WM_COMPILER%"=="x86_64-w64-mingw32"  set WM_ARCH_OPTION=64
IF "%WM_COMPILER%"=="mingw32"             set WM_ARCH_OPTION=32
IF "%WM_COMPILER%"=="mingw-w32"           set WM_ARCH_OPTION=32
IF "%WM_COMPILER%"=="mingw-w64"           set WM_ARCH_OPTION=64
IF "%WM_COMPILER%"=="i586-mingw32msvc"    set WM_ARCH_OPTION=32
IF "%WM_COMPILER%"=="amd64-mingw32msvc"   set WM_ARCH_OPTION=64

rem - Floating-point signal handling:
rem     set or unset
set FOAM_SIGFPE=""

rem - memory initialisation:
rem     set or unset
rem set FOAM_SETNAN=""

set WM_COMPILER_ARCH=""
set WM_COMPILER_LIB_ARCH=""
set WM_CC=%WM_COMPILER%-gcc
set WM_CXX=%WM_COMPILER%-g++
set FOAM_JOB_DIR=%FOAM_INST_DIR%\jobControl

rem wmake configuration
set WM_DIR=%WM_PROJECT_DIR%\wmake
set WM_LINK_LANGUAGE=c++
set WM_OPTIONS=%WM_ARCH%%WM_COMPILER%%WM_PRECISION_OPTION%%WM_COMPILE_OPTION%

rem base executables/libraries
set FOAM_APPBIN=%WM_PROJECT_DIR%\platforms\%WM_OPTIONS%\bin
set FOAM_LIBBIN=%WM_PROJECT_DIR%\platforms\%WM_OPTIONS%\lib

rem external (ThirdParty) libraries
set FOAM_EXT_LIBBIN=%WM_THIRD_PARTY_DIR%\platforms\%WM_OPTIONS%\lib

rem user executables/libraries
set FOAM_USER_APPBIN=%WM_PROJECT_USER_DIR%\platforms\%WM_OPTIONS%\bin
set FOAM_USER_LIBBIN=%WM_PROJECT_USER_DIR%\platforms\%WM_OPTIONS%\lib

rem convenience
set FOAM_APP=%WM_PROJECT_DIR%\applications
set FOAM_SRC=%WM_PROJECT_DIR%\src
set FOAM_TUTORIALS=%WM_PROJECT_DIR%\tutorials
set FOAM_UTILITIES=%FOAM_APP%\utilities
set FOAM_SOLVERS=%FOAM_APP%\solvers
set FOAM_RUN=%WM_PROJECT_USER_DIR%\run

IF "%WM_MPLIB%"=="""" set FOAM_MPI=dummy
IF "%WM_MPLIB%"=="OPENMPI" set FOAM_MPI=openmpi-1.5.3
IF "%WM_MPLIB%"=="MPICH" set FOAM_MPI=mpich2-1.4.1p1
IF "%WM_MPLIB%"=="MSMPI" set FOAM_MPI=msmpi-2008R2

set MPI_HOME=%WM_THIRD_PARTY_DIR%\%FOAM_MPI%
set MPI_ARCH_PATH=%WM_THIRD_PARTY_DIR%\platforms\%WM_ARCH%%WM_COMPILER%\%FOAM_MPI%

IF "%WM_MPLIB%"=="OPENMPI" set OPAL_PKGDATADIR=%MPI_ARCH_PATH%\share\openmpi
IF "%WM_MPLIB%"=="MPICH" set MPICH_ROOT=%MPI_ARCH_PATH%

set FOAM_MPI_LIBBIN=%FOAM_LIBBIN%\%FOAM_MPI%
set MPI_BUFFER_SIZE=20000000

set ParaView_VERSION=3.12.0

set ParaView_INST_DIR=%WM_THIRD_PARTY_DIR%\paraview-%ParaView_VERSION%
set ParaView_DIR=%WM_THIRD_PARTY_DIR%\platforms\%WM_ARCH%%WM_COMPILER%\paraview-%ParaView_VERSION%

set PATH=%PATH%;%WM_DIR%;%MPI_ARCH_PATH%\lib;%MPI_ARCH_PATH%\bin;%FOAM_MPI_LIBBIN%;%FOAM_USER_APPBIN%;%FOAM_USER_LIBBIN%;%FOAM_APPBIN%;%FOAM_LIBBIN%;%FOAM_EXT_LIBBIN%;%WM_DIR%;%WM_PROJECT_DIR%\bin;%ParaView_DIR%\bin

rem add (non-dummy) MPI implementation
rem dummy MPI already added to LD_LIBRARY_PATH and has no external libraries
IF NOT "%FOAM_MPI%"=="dummy" set PATH=%PATH%;%FOAM_EXT_LIBBIN%\%FOAM_MPI%

rem Source all *.bat files present at "%WM_PROJECT_DIR%\etc\config.d"
for %%A in ("%WM_PROJECT_DIR%\etc\config.d\*.bat") DO CALL "%%A"
GOTO END

:SETHOME
set HOME=%~dp1
set HOME=%HOME:~0,-1%
GOTO :EOF

:END
cd "%WM_PROJECT_INST_DIR%"
