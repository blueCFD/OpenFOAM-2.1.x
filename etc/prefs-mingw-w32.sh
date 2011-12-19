#----------------------------------*-sh-*--------------------------------------
# License
#     This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
#     For more information about these patches, visit:
#         http://www.bluecape.com.pt/blueCFD
#
#     OpenFOAM's official site: http://www.openfoam.com
#
#     OpenFOAM is free software: you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
#
# File
#     etc/prefs-mingw-w32.sh
#
# Description
#     Preset variables for the OpenFOAM configuration - POSIX shell syntax.
#
#     This file will only be sourced if set when calling bashrc. Example:
#       source $HOME/OpenFOAM/OpenFOAM-2.0/etc/bashrc prefs-mingw-w32.sh
#
#------------------------------------------------------------------------------

#- Operating System:
#    WM_OSTYPE = POSIX | MSwindows
export WM_OSTYPE=MSwindows

# Specify compiler mode: system or ThirdParty
# ~~~~~~~~~~~~~~~~~~~~~~~
foamCompiler=ThirdParty

# Specify openmpi
# ~~~~~~~~~~~~~~~~~~~~~~
export WM_MPLIB=OPENMPI

#- Cross-compilers: WM_COMPILER = 
#       i686-w64-mingw32  - using the 32bit compiler from mingw-w64 at sourceforge.net
#       x86_64-w64-mingw32  - using the 64bit compiler from mingw-w64 at sourceforge.net
#       mingw32, mingw-w32, mingw-w64  - using the custom build of mingw(-w64) cross-compiler
#       i586-mingw32msvc, amd64-mingw32msvc  - attempt to use Ubuntu's pre-built mingw-w64 binaries, but the 64bit version only worked in Windows XP x64
export WM_COMPILER=mingw-w32
unset WM_COMPILER_ARCH WM_COMPILER_LIB_ARCH

#- Architecture:
#    WM_ARCH_OPTION = 32 | 64
export WM_ARCH_OPTION=32

# ----------------------------------------------------------------- end-of-file
