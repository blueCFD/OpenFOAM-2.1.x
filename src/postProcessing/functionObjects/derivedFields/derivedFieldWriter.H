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
    Foam::derivedFieldWriter

Description
    Basis for writers used in derivedFields e.g., wallShearStressWriter

    \*---------------------------------------------------------------------------*/

#ifndef derivedFieldWriter_H
#define derivedFieldWriter_H

#include "dictionary.H"
#include "entry.H"
#include "Switch.H"
#include "Pstream.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

  // Forward declaration of classes
  class objectRegistry;

  /*---------------------------------------------------------------------------*\
    Class derivedFieldWriter Declaration
  \*---------------------------------------------------------------------------*/

  class derivedFieldWriter
  {
    // Private data
  
    //- Produce statistics
    Switch statistics_;

  public:
    // Destructor

    virtual ~derivedFieldWriter();

    // Member Functions

    //- Write derived field.
    virtual void write(const objectRegistry& obr) = 0;

    //- Create obj if requested.
    template<class T>
    static T* read(const dictionary& dict)
    {
      const bool nonRecursive = false;
      const bool noPatternMatch = false;
      T* obj = 0;
      const entry* entry = 
        dict.lookupEntryPtr(T::FIELD_NAME, nonRecursive, noPatternMatch);
    
      if (0 != entry) 
        {
          obj = new T(entry->dict());
        }

      return obj;
    }

  protected:
    // Constructors

    derivedFieldWriter(const dictionary& dict);

    //- Write min/max/average
    template<class T_Field>
    void writeStatistics(const T_Field& fieldp,
                         const word& patchName,
                         const word& fieldName)
    {
      if (statistics_ && Pstream::master()) 
        {   
            Info<< "Patch: " << patchName
                << ", Field: " << fieldName
                << ", min: " << min(fieldp) 
                << ", max: " << max(fieldp)
                << ", average: " << average(fieldp) << nl << endl;
        }
    }
  };

  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
