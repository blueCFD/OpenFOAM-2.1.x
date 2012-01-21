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
    Foam::PCGgpu

Description
    CPU-based preconditioned conjugate gradient solver for symmetric lduMatrices
    using a run-time selectable preconditiioner, identical to PCG.

SourceFiles
    PCGgpu.C

\*---------------------------------------------------------------------------*/

#ifndef PCGgpu_H
#define PCGgpu_H

#include "PCG.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class PCGgpu Declaration
\*---------------------------------------------------------------------------*/

class PCGgpu
:
    public PCG
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        PCGgpu(const PCGgpu&);

        //- Disallow default bitwise assignment
        void operator=(const PCGgpu&);


public:

    //- Runtime type information
    TypeName("PCGgpu");


    // Constructors

        //- Construct from matrix components and solver data stream
        PCGgpu
        (
            const word& fieldName,
            const lduMatrix& matrix,
            const FieldField<Field, scalar>& interfaceBouCoeffs,
            const FieldField<Field, scalar>& interfaceIntCoeffs,
            const lduInterfaceFieldPtrsList& interfaces,
            const dictionary& solverControls
        );


    // Member Functions

        //- Solve the matrix with this solver
        virtual lduMatrix::solverPerformance solve
        (
            scalarField& psi,
            const scalarField& source,
            const direction cmpt=0
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
