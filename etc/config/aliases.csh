#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# License
#     This file is part of OpenFOAM.
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
#     etc/config/aliases.csh
#
# Description
#     Aliases for working with OpenFOAM
#     Sourced from OpenFOAM-<VERSION>/etc/cshrc and/or ~/.cshrc
#
#------------------------------------------------------------------------------

# Change compiled version aliases
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmSET 'source $WM_PROJECT_DIR/etc/cshrc'
alias wm64 'wmSET WM_ARCH_OPTION=64'
alias wm32 'wmSET WM_ARCH_OPTION=32'
alias wmSP 'wmSET WM_PRECISION_OPTION=SP'
alias wmDP 'wmSET WM_PRECISION_OPTION=DP'

# clear env
alias wmUNSET='source $WM_PROJECT_DIR/etc/config/unset.csh'

# Toggle wmakeScheduler on/off
#  - also need to set WM_HOSTS
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmSchedON 'setenv WM_SCHEDULER $WM_PROJECT_DIR/wmake/wmakeScheduler'
alias wmSchedOFF 'unsetenv WM_SCHEDULER'

# Toggle Multi-Core On/Off
# Proposed the following bug report: http://www.openfoam.com/mantisbt/view.php?id=211
#wmSC - single core machine
#wmMC - multi-core machine
alias wmSC='unsetenv WM_NCOMPPROCS && echo "Building enabled for a single core"'
alias wmMC='setenv WM_NCOMPPROCS 1; test -r /proc/cpuinfo && setenv WM_NCOMPPROCS $(egrep "^processor" /proc/cpuinfo | wc -l) && echo "Building enabled for $WM_NCOMPPROCS cores"'

# Toggle WM_CONTINUE_ON_ERROR on/off
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmNONSTOP 'setenv WM_CONTINUE_ON_ERROR 1'
alias wmSTOPON1st 'unsetenv WM_CONTINUE_ON_ERROR'

# Change ParaView version
# ~~~~~~~~~~~~~~~~~~~~~~~
alias foamPV 'source `$WM_PROJECT_DIR/etc/config/paraview.csh` ParaView_VERSION=\!*; echo paraview-$ParaView_VERSION'


# Change directory aliases
# ~~~~~~~~~~~~~~~~~~~~~~~~
alias src 'cd $FOAM_SRC'
alias lib 'cd $FOAM_LIBBIN'
alias run 'cd $FOAM_RUN'
alias foam 'cd $WM_PROJECT_DIR'
alias foamsrc 'cd $FOAM_SRC/$WM_PROJECT'
alias foamfv 'cd $FOAM_SRC/finiteVolume'
alias app 'cd $FOAM_APP'
alias util 'cd $FOAM_UTILITIES'
alias sol 'cd $FOAM_SOLVERS'
alias tut 'cd $FOAM_TUTORIALS'

alias foamApps 'cd $FOAM_APP'
alias foamSol 'cd $FOAM_SOLVERS'
alias foamTuts 'cd $FOAM_TUTORIALS'
alias foamUtils 'cd $FOAM_UTILITIES'
alias foam3rdParty 'cd $WM_THIRD_PARTY_DIR'

if ( $?WM_PROJECT_SITE ) then
    alias foamSite 'cd $WM_PROJECT_SITE'
else
    alias foamSite 'cd $WM_PROJECT_INST_DIR/site'
endif

# -----------------------------------------------------------------------------
