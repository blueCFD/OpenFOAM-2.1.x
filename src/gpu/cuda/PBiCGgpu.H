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
    Foam::PBiCGgpu

Description
    CPU-based preconditioned bi-conjugate gradient solver for asymmetric lduMatrices
    using a run-time selectable preconditiioner, identical to PBiCG.

SourceFiles
    PBiCGgpu.C

\*---------------------------------------------------------------------------*/

#ifndef PBiCGgpu_H
#define PBiCGgpu_H

#include "PBiCG.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class PBiCGgpu Declaration
\*---------------------------------------------------------------------------*/

class PBiCGgpu
:
    public PBiCG
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        PBiCGgpu(const PBiCGgpu&);

        //- Disallow default bitwise assignment
        void operator=(const PBiCGgpu&);


public:

    //- Runtime type information
    TypeName("PBiCGgpu");


    // Constructors

        //- Construct from matrix components and solver data stream
        PBiCGgpu
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
