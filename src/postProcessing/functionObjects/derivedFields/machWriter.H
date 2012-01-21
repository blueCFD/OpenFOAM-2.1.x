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
    Foam::machWriter

Description
    Write Mach number field

SourceFiles
    machWriter.C

\*---------------------------------------------------------------------------*/

#ifndef machWriter_H
#define machWriter_H

#include "derivedFieldWriter.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
  class dictionary;


  /*---------------------------------------------------------------------------*\
    Class machWriter Declaration
  \*---------------------------------------------------------------------------*/

  class machWriter : public derivedFieldWriter
  {
  public:
    //- Runtime type information
    TypeName("machWriter");

    //- Field file name.
    static const word FIELD_NAME;

    // Constructors

    machWriter(const dictionary& dict);

    // Member Functions

    //- Write derived field.
    virtual void write(const objectRegistry& obr);
  };

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
