/*---------------------------------------------------------------------------*\
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "ofgpu/ofgpuconfig.h"

#include "cudaGpu.H"
#include "addToRunTimeSelectionTable.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineNamedTemplateTypeNameAndDebug(cudaGpu, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        cudaGpu,
        dictionary
    );
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::cudaGpu::cudaGpu
(
 const word& name,
 const Time&,
 const dictionary& dict
)
  : 
  functionObject(name)
{
  const label device = dict.lookupOrDefault<label>("cudaDevice", 0);
  ofgpuConfig(device);
}

// ************************************************************************* //
