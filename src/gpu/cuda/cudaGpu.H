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

Typedef
    Foam::cudaGpuFunctionObject

Description
    FunctionObject to initialize device and enable GPU linear solvers.

@verbatim
    functions
    (
      cudaGpu
      {
        // Type of functionObject
        type cudaGpu;

        // Where to load it from (if not already in solver)
        functionObjectLibs ("gpu");

	// CUDA Device Id
        // Optional default = 0
        // cudaDevice 2;            
      }
   );
   @endverbatim

SourceFiles
    cudaGpu.C

\*---------------------------------------------------------------------------*/

#ifndef cudaGpu_H
#define cudaGpu_H

#include "functionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
  class cudaGpu : public functionObject
  {
    // Private Member Functions
    
    //- Disallow default bitwise copy construct
    cudaGpu(const cudaGpu&);
    
    //- Disallow default bitwise assignment
    void operator=(const cudaGpu&);

  public:
    //- Runtime type information
    TypeName("cudaGpu");

    // Constructors

    //- Construct from components
    cudaGpu
    (
     const word& name,
     const Time&,
     const dictionary&
     );

   // Member Functions

    //- Called at the start of the time-loop
    virtual bool start()
    { return true; }

    //- Called at each ++ or += of the time-loop
    virtual bool execute(const bool forceWrite)
    { return true; }

    //- Read and set the function object if its data have changed
    virtual bool read(const dictionary&)
    { return true; }
  };
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
