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

#include "vorticityWriter.H"
#include "volFields.H"
#include "dictionary.H"
#include "Time.H"
#include "fvc.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(vorticityWriter, 0);
  const word vorticityWriter::FIELD_NAME("vorticity");
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::vorticityWriter::vorticityWriter(const dictionary & dict)
  : derivedFieldWriter(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::vorticityWriter::write(const objectRegistry& obr)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr);
  const Time& runTime = obr.time();
  const volVectorField& U = obr.lookupObject<volVectorField>("U");

  volVectorField vorticity
    (
     IOobject
     (
      FIELD_NAME,
      runTime.timeName(),
      mesh,
      IOobject::NO_READ
      ),
     fvc::curl(U)
     );
  
  vorticity.write();
}


// ************************************************************************* //
