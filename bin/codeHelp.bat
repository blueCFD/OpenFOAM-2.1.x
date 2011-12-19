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
rem      codeHelp.bat
rem 
rem  Description
rem      This batch file launches the OpenFOAM code C++ documentation, using 
rem    Qt Assistant.
rem 
rem ------------------------------------------------------------------------------

PUSHD %WM_PROJECT_DIR%\doc\Doxygen\html
start assistant -collectionFile openfoam-code.qhc
POPD
