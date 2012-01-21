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

#include "yPlusWriter.H"
#include "dictionary.H"
#include "Time.H"
#include "wallFvPatch.H"
#include "nearWallDist.H"

#include "incompressible/RAS/RASModel/RASModel.H"
#include "nutkWallFunction/nutkWallFunctionFvPatchScalarField.H"
#include "compressible/RAS/RASModel/RASModel.H"
#include "mutkWallFunction/mutkWallFunctionFvPatchScalarField.H"
#include "incompressible/LES/LESModel/LESModel.H"
#include "compressible/LES/LESModel/LESModel.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(yPlusWriter, 0);
  const word yPlusWriter::FIELD_NAME("yPlus");
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //




inline
static const Foam::volScalarField nutORmut(const Foam::incompressible::RASModel & model)
{
  return model.nut()();
}


inline
static const Foam::volScalarField nutORmut(const Foam::compressible::RASModel & model)
{
  return model.mut()();
}


template<class T_Model, class T_WallFunctionPatchField>
bool 
Foam::yPlusWriter::extractedFromRAS(char const * const modelProperties,
                                    const fvMesh& mesh,
                                    volScalarField::GeometricBoundaryField& yPlusb)
{
  int count = 0;

  if (mesh.foundObject<T_Model>(modelProperties)) 
    {
      const T_Model & model
        = mesh.lookupObject<T_Model>(modelProperties);
    
      const volScalarField::GeometricBoundaryField patches =
        nutORmut(model).boundaryField();
      
      forAll(patches, patchI)
        {
          const fvPatchField<scalar>& currPatch = patches[patchI];
          
          if (isA<T_WallFunctionPatchField>(currPatch))
            {
              const T_WallFunctionPatchField& wP =
                dynamic_cast<const T_WallFunctionPatchField&>(currPatch);
              scalarField& yp = yPlusb[patchI];
              yp = wP.yPlus();
              ++count;

              writeStatistics(yp, currPatch.patch().name(), FIELD_NAME);
            }
        }
    }

  return (0 < count);
}


template<class T_Model>
bool 
Foam::yPlusWriter::extractedFromRAS(char const * const modelProperties,
                                    const fvMesh& mesh,
                                    volScalarField::GeometricBoundaryField& yPlusb)
{
  int count = 0;

  if (mesh.foundObject<T_Model>(modelProperties)) 
    {
      const scalar Cmu = 0.09;
      const T_Model & model
        = mesh.lookupObject<T_Model>(modelProperties);

      const fvPatchList& patches = mesh.boundary();
      
      forAll(patches, patchI)
        {
          const fvPatch& currPatch = patches[patchI];
          
          if (isA<wallFvPatch>(currPatch))
            {
              scalarField& yp = yPlusb[patchI];
              yp = model.yPlus(patchI, Cmu);
              ++count;

              writeStatistics(yp, currPatch.patch().name(), FIELD_NAME);
            }
        }
    }

  return (0 < count);
}


inline
static Foam::tmp<Foam::volScalarField> nuEffFor(const Foam::incompressible::LESModel & model)
{
  return model.nuEff();
}


inline
static Foam::tmp<Foam::volScalarField> nuEffFor(const Foam::compressible::LESModel & model)
{
  return model.muEff() / model.rho();
}


inline
static Foam::tmp<Foam::volScalarField> nuFor(const Foam::incompressible::LESModel & model)
{
  return model.nu();
}


inline
static Foam::tmp<Foam::volScalarField> nuFor(const Foam::compressible::LESModel & model)
{
  return model.mu() / model.rho();
}


template<class T_Model>
bool 
Foam::yPlusWriter::extractedFromLES(char const * const modelProperties,
                                    const fvMesh& mesh,
                                    volScalarField::GeometricBoundaryField& yPlusb)
{
  int count = 0;

  if (mesh.foundObject<T_Model>(modelProperties)) 
    {
      const fvPatchList& patches = mesh.boundary();

      const T_Model & model
        = mesh.lookupObject<T_Model>(modelProperties);

      const volScalarField::GeometricBoundaryField d = 
        nearWallDist(mesh).y();
      tmp<volScalarField> nuEff = nuEffFor(model);
      const volScalarField::GeometricBoundaryField& nuEffb = 
        nuEff().boundaryField();
      tmp<volScalarField> nu = nuFor(model);
      const volScalarField::GeometricBoundaryField& nub = 
        nu().boundaryField();
      const volVectorField::GeometricBoundaryField& Ub = 
        model.U().boundaryField();
      
      forAll(patches, patchI)
        {
          const fvPatch& currPatch = patches[patchI];
          
          if (isA<wallFvPatch>(currPatch))
            {
              scalarField& yp = yPlusb[patchI];
              yp = d[patchI] *
                sqrt
                (
                 nuEffb[patchI]
                 *mag(Ub[patchI].snGrad())
                 )
                /nub[patchI];

              ++count;
              writeStatistics(yp, currPatch.name(), FIELD_NAME);
           }
        }
    }

  return (0 < count);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::yPlusWriter::yPlusWriter(const dictionary & dict)
  : derivedFieldWriter(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void Foam::yPlusWriter::write(const objectRegistry& obr)
{
  const fvMesh& mesh = refCast<const fvMesh>(obr);
  
  volScalarField yPlus
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
     dimensionedScalar(FIELD_NAME, dimless, 0.0)
     );

  volScalarField::GeometricBoundaryField& yPlusb =
    yPlus.boundaryField();

  if (!extractedFromRAS<incompressible::RASModel,
      incompressible::RASModels::nutkWallFunctionFvPatchScalarField>
      ("RASProperties", mesh, yPlusb) &&
      !extractedFromRAS<compressible::RASModel,
      compressible::RASModels::mutkWallFunctionFvPatchScalarField>
      ("RASProperties", mesh, yPlusb) &&
      !extractedFromRAS<incompressible::RASModel>
      ("RASProperties", mesh, yPlusb) &&
      !extractedFromRAS<compressible::RASModel>
      ("RASProperties", mesh, yPlusb) && 
      !extractedFromLES<incompressible::LESModel>
      ("LESProperties", mesh, yPlusb)) {
    extractedFromLES<compressible::LESModel>
      ("LESProperties", mesh, yPlusb);
  }
  
  yPlus.write();
}


// ************************************************************************* //
