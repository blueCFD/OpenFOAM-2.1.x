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
    Foam::derivedFields

Description
    Provide derived fields such as wallShearStress and yPlus

    Optionally provides statistics: min, max & average

    Member function derivedFields::write() writes
    derivedFields into the directory \<timeDir\>, e.g.

    @verbatim
    functions
    (
      derivedFields1
      {
        // Type of functionObject
        type derivedFields;

        // Where to load it from (if not already in solver)
        functionObjectLibs ("derivedFields");

        // Flow region
        // Optional default = defaultRegion
        // region someWhere;

        // Dedicated dictionary
        // Optional defualt = ""
        dictionary "";

        // Write interval
        outputControl timeStep;
        outputInterval 1;
        
        // functionObject will be called.
        // Optional default = on
        enabled on;

        // Write fields on first start up (e.g., time zero)
        // Optional default = off
        writeOnStart on;

        wallShearStress 
        {
          // min, max & average to Info (optional default = off)
          statistics on;

          // Incompressible density reference (optional default = 1.)
          rhoInf 1.;
        }

        yPlus
        {
          statistics on;
        }               
      }
   );
   @endverbatim

SourceFiles
    derivedFields.C

\*---------------------------------------------------------------------------*/

#ifndef derivedFields_H
#define derivedFields_H

#include "pointFieldFwd.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

  // Forward declaration of classes
  template<class T> class PtrList;
  class objectRegistry;
  class dictionary;
  class derivedFieldWriter;
  class mapPolyMesh;

  /*---------------------------------------------------------------------------*\
    Class derivedFields Declaration
  \*---------------------------------------------------------------------------*/

  class derivedFields
  {
    // Private data

    //- Active writers, if any.
    PtrList<derivedFieldWriter>* const writers_;

    //- Name of this set of derivedFields,
    word name_;

    const objectRegistry& obr_;

    // Private Member Functions

    //- Add writer to active list.
    void addWriter(derivedFieldWriter* writer);

    //- Disallow default bitwise copy construct
    derivedFields(const derivedFields&);

    //- Disallow default bitwise assignment
    void operator=(const derivedFields&);

  public:

    //- Runtime type information
    TypeName("derivedFields");

    // Constructors

    //- Construct for given objectRegistry and dictionary.
    //  Allow the possibility to load fields from files
    derivedFields
    (
     const word& name,
     const objectRegistry&,
     const dictionary&,
     const bool loadFromFiles = false
     );

    // Destructor
    virtual ~derivedFields();

    // Member Functions

    //- Return name of the set of derivedFields
    const word& name() const
    {
      return name_;
    }

    //- Read the derivedFields data
    void read(const dictionary& dict);

    //- Calculate the derivedFields and write
    void write();

    //- Satisfy OutputFilterFunctionObject
    void execute()
    {}

    //- Satisfy OutputFilterFunctionObject
    void end()
    {}

    //- Update for changes of mesh
    void updateMesh(const mapPolyMesh&)
    {}

    //- Update for changes of mesh
    void movePoints(const pointField&)
    {}
  };


  // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
