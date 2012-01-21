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

#include "wallShearStressWriter.H"
#include "volFields.H"
#include "dictionary.H"
#include "Time.H"
#include "wallFvPatch.H"

#include "incompressible/singlePhaseTransportModel/singlePhaseTransportModel.H"
#include "incompressible/RAS/RASModel/RASModel.H"
#include "incompressible/LES/LESModel/LESModel.H"
#include "basicThermo.H"
#include "compressible/RAS/RASModel/RASModel.H"
#include "compressible/LES/LESModel/LESModel.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(wallShearStressWriter, 0);
  const word wallShearStressWriter::FIELD_NAME("wallShearStress");
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

Foam::tmp<Foam::volSymmTensorField> 
Foam::wallShearStressWriter::devRhoReff(const objectRegistry& obr, 
                                        bool & foundField) const
{
  if (obr.foundObject<incompressible::RASModel>("RASProperties"))
    {
      const incompressible::RASModel& ras
        = obr.lookupObject<incompressible::RASModel>("RASProperties");

      return rhoRef_*ras.devReff();
    }
  else if (obr.foundObject<compressible::RASModel>("RASProperties"))
    {
      const compressible::RASModel& ras
        = obr.lookupObject<compressible::RASModel>("RASProperties");

      return ras.devRhoReff();
    }
  else if (obr.foundObject<incompressible::LESModel>("LESProperties"))
    {
      const incompressible::LESModel& les
        = obr.lookupObject<incompressible::LESModel>("LESProperties");

      return rhoRef_*les.devBeff();
    }
  else if (obr.foundObject<compressible::LESModel>("LESProperties"))
    {
      const compressible::LESModel& les =
        obr.lookupObject<compressible::LESModel>("LESProperties");

      return les.devRhoBeff();
    }
  else  if (obr.foundObject<basicThermo>("thermophysicalProperties"))
    {
      const basicThermo& thermo =
        obr.lookupObject<basicThermo>("thermophysicalProperties");

      const volVectorField& U = obr.lookupObject<volVectorField>(UName_);

      return -thermo.mu()*dev(twoSymm(fvc::grad(U)));
    }
  else if
    (
     obr.foundObject<singlePhaseTransportModel>("transportProperties")
     )
    {
      const singlePhaseTransportModel& laminarT =
        obr.lookupObject<singlePhaseTransportModel>
        ("transportProperties");

      const volVectorField& U = obr.lookupObject<volVectorField>(UName_);

      return -rhoRef_*laminarT.nu()*dev(twoSymm(fvc::grad(U)));
    }
  else if (obr.foundObject<dictionary>("transportProperties"))
    {
      const dictionary& transportProperties =
        obr.lookupObject<dictionary>("transportProperties");

      dimensionedScalar nu(transportProperties.lookup("nu"));

      const volVectorField& U = obr.lookupObject<volVectorField>(UName_);

      return -rhoRef_*nu*dev(twoSymm(fvc::grad(U)));
    }
  else
    {
      foundField = false;
      return volSymmTensorField::null();
    }
}


Foam::scalar Foam::wallShearStressWriter::rho(const volScalarField& p) const
{
  if (p.dimensions() == dimPressure)
    {
      return 1.0;
    }
  else
    {
      return rhoRef_;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::wallShearStressWriter::wallShearStressWriter(const dictionary & dict)
  : derivedFieldWriter(dict),
    UName_("U"),
    rhoRef_(1.)
{
  // Optional entry U
  dict.readIfPresent("UName", UName_);

  // Reference density needed for incompressible calculations
  dict.readIfPresent("rhoInf", rhoRef_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::wallShearStressWriter::write(const objectRegistry& obr)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr);
  
  volVectorField wallShearStress
    (
     IOobject
     (
      FIELD_NAME,
      obr.time().timeName(),
      mesh,
      IOobject::NO_READ,
      IOobject::NO_WRITE
      ),
     mesh,
     dimensionedVector
     (
      FIELD_NAME,
      //tdevRhoReff().dimensions() / tmagSf().dimensions(),
      dimensionSet(0, 2, -2, 0, 0, 0, 0),
      vector::zero
      )
     );

  bool foundField = true;
  tmp<volSymmTensorField> tdevRhoReff = devRhoReff(obr, foundField);

  if (foundField) {
    const volSymmTensorField::GeometricBoundaryField& devRhoReffb =
      tdevRhoReff().boundaryField();

    tmp<surfaceScalarField> tmagSf = mesh.magSf();
    const surfaceScalarField::GeometricBoundaryField& magSfb =
      tmagSf().boundaryField();

    volVectorField::GeometricBoundaryField& wShearStressb =
      wallShearStress.boundaryField();
    const surfaceVectorField::GeometricBoundaryField& Sfb =
      mesh.Sf().boundaryField();
    const fvPatchList& patches = mesh.boundary();
    
    forAll(patches, patchi)
      {
        const fvPatch& currPatch = patches[patchi];
        
        if (isA<wallFvPatch>(currPatch))
          {
            vectorField& wssp = wShearStressb[patchi];
            wssp =
              (Sfb[patchi]/magSfb[patchi]) & devRhoReffb[patchi];
            
            writeStatistics(wssp, currPatch.name(), FIELD_NAME);
          }
      }
  }

  wallShearStress.write();
}


// ************************************************************************* //
