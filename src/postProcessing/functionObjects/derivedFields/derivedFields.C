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

#include "derivedFields.H"
#include "machWriter.H"
#include "yPlusWriter.H"
#include "vorticityWriter.H"
#include "wallShearStressWriter.H"
#include "dictionary.H"
#include "Time.H"
#include "PtrList.H"
#include "fvMesh.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(derivedFields, 0);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline
void Foam::derivedFields::addWriter(derivedFieldWriter* writer)
{
  if (0 != writer) 
    {
      const label id = writers_->size();
      
      writers_->setSize(id + 1);
      writers_->set(id, writer);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::derivedFields::derivedFields(const word& name,
                                   const objectRegistry& obr,
                                   const dictionary& dict,
                                   const bool loadFromFiles)
  : writers_(new PtrList<derivedFieldWriter>),
    name_(name),
    obr_(obr)
{
  read(dict);
}


Foam::derivedFields::~derivedFields()
{
  delete writers_;
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void Foam::derivedFields::read(const dictionary& dict)
{
  writers_->clear();

  // Check if the available mesh is an fvMesh otherise deactivate
  if (!isA<fvMesh>(obr_))
    {
      WarningIn
        (
         "derivedFields::derivedFields(const objectRegistry& obr, const dictionary& dict)"
         )   << "No fvMesh available, deactivating."
             << endl;
    }
  else
    {
      addWriter(derivedFieldWriter::read<wallShearStressWriter>(dict));
      addWriter(derivedFieldWriter::read<yPlusWriter>(dict));
      addWriter(derivedFieldWriter::read<machWriter>(dict));
      addWriter(derivedFieldWriter::read<vorticityWriter>(dict));

      Switch writeOnStart(dict.lookupOrDefault<Switch>("writeOnStart", false));

      if (writeOnStart) 
        {
          write();
        }
    }
}


void Foam::derivedFields::write()
{
  forAll(*writers_, i)
    {
      (*writers_)[i].write(obr_);
    }
}

// ************************************************************************* //
