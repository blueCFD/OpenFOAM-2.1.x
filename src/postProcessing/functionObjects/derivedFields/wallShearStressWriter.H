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

Class
    Foam::wallShearStressWriter

Description
    Write wallShearStress field

SourceFiles
    wallShearStress.C

\*---------------------------------------------------------------------------*/

#ifndef wallShearStressWriter_H
#define wallShearStressWriter_H

#include "derivedFieldWriter.H"
#include "volFieldsFwd.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
  class dictionary;


  /*---------------------------------------------------------------------------*\
    Class wallShearStressWriter Declaration
  \*---------------------------------------------------------------------------*/

  class wallShearStressWriter : public derivedFieldWriter
  {
    // Private data

    //- Name of velocity field
    word UName_;

    //- Reference density needed for incompressible calculations
    scalar rhoRef_;
    
    // Private methods

    //- Return the effective viscous stress (laminar + turbulent).
    tmp<volSymmTensorField> devRhoReff(const objectRegistry& obr, 
                                       bool & foundField) const;

    //- Return rhoRef if the pressure field is dynamic, i.e. p/rho
    //  otherwise return 1
    scalar rho(const volScalarField& p) const;

  public:
    //- Runtime type information
    TypeName("wallShearStressWriter");

    //- Field file name.
    static const word FIELD_NAME;

    // Constructors

    wallShearStressWriter(const dictionary& dict);

    // Member Functions

    //- Write derived field.
    virtual void write(const objectRegistry& obr);
  };

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
