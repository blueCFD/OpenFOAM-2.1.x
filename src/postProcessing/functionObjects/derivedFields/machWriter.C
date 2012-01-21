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

#include "machWriter.H"
#include "volFields.H"
#include "dictionary.H"
#include "Time.H"
#include "basicPsiThermo.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(machWriter, 0);
  const word machWriter::FIELD_NAME("Ma");
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::machWriter::machWriter(const dictionary & dict)
  : derivedFieldWriter(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::machWriter::write(const objectRegistry& obr)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr);
  const Time& runTime = obr.time();

  volScalarField machNo
    (
     IOobject
     (
      FIELD_NAME,
      runTime.timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
      ),
     mesh,
     dimensionedScalar(FIELD_NAME, dimless, 0.0)
     );
  
  const word UName("U");
  const word TName("T");
  const word thermoPhysicalName("thermophysicalProperties");
  const word thermoName("thermodynamicProperties");

  if (obr.foundObject<volVectorField>(UName) && 
      obr.foundObject<volScalarField>(TName))
    {
      const volVectorField& U = obr.lookupObject<volVectorField>(UName);
      
      if (obr.foundObject<basicPsiThermo>(thermoPhysicalName))
        {
          const basicPsiThermo& thermo =
            obr.lookupObject<basicPsiThermo>(thermoPhysicalName);
      
          volScalarField Cp = thermo.Cp();
          volScalarField Cv = thermo.Cv();

          machNo = mag(U)/(sqrt((Cp/Cv)*(Cp - Cv)*thermo.T()));
        }
      else if (obr.foundObject<dictionary>(thermoName))
        {
          const volScalarField& T = obr.lookupObject<volScalarField>(TName);
          const dictionary& thermoProps =
            obr.lookupObject<dictionary>(thermoName);

          dimensionedScalar R(thermoProps.lookup("R"));
          dimensionedScalar Cv(thermoProps.lookup("Cv"));

          machNo = mag(U)/(sqrt(((Cv + R)/Cv)*R*T));
        }
    }

  machNo.write();
}


// ************************************************************************* //
