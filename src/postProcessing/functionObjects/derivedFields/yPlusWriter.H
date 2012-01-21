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
    Foam::yPlusWriter

Description
    Write yPlus field

SourceFiles
    yPlus.C

\*---------------------------------------------------------------------------*/

#ifndef yPlusWriter_H
#define yPlusWriter_H

#include "derivedFieldWriter.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
  class dictionary;
  class fvMesh;


  /*---------------------------------------------------------------------------*\
    Class yPlusWriter Declaration
  \*---------------------------------------------------------------------------*/

  class yPlusWriter : public derivedFieldWriter
  {
    // Private data

    
    // Private methods

    //- Extract yPlus from LES models.
    template<class T_Model>
    bool 
    extractedFromLES(char const * const modelProperties,
                  const fvMesh& mesh,
                  volScalarField::GeometricBoundaryField& yPlusb);

    //- Extract yPlus from RAS wall function model.
    template<class T_Model, class T_WallFunctionPatchField>
    bool 
    extractedFromRAS(char const * const modelProperties,
                  const fvMesh& mesh,
                  volScalarField::GeometricBoundaryField& yPlusb);

    //- Extract yPlus from RAS wall function low-Re model.
    template<class T_Model>
    bool 
    extractedFromRAS(char const * const modelProperties,
                  const fvMesh& mesh,
                  volScalarField::GeometricBoundaryField& yPlusb);

  public:
    //- Runtime type information
    TypeName("yPlusWriter");

    //- Field file name.
    static const word FIELD_NAME;

    // Constructors

    yPlusWriter(const dictionary& dict);

    // Member Functions

    //- Write derived field.
    virtual void write(const objectRegistry& obr);
  };

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
