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

#include "ofgpu/pcg.h"
#include "ofgpu/sparsematrixargs.h"

#include "PCGgpu.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(PCGgpu, 0);

    lduMatrix::solver::addsymMatrixConstructorToTable<PCGgpu>
        addPCGgpuAsymMatrixConstructorToTable_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::PCGgpu::PCGgpu
(
    const word& fieldName,
    const lduMatrix& matrix,
    const FieldField<Field, scalar>& interfaceBouCoeffs,
    const FieldField<Field, scalar>& interfaceIntCoeffs,
    const lduInterfaceFieldPtrsList& interfaces,
    const dictionary& solverControls
)
:
    PCG
    (
        fieldName,
        matrix,
        interfaceBouCoeffs,
        interfaceIntCoeffs,
        interfaces,
        solverControls
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::lduMatrix::solverPerformance Foam::PCGgpu::solve
(
    scalarField& psi,
    const scalarField& source,
    const direction cmpt
) const
{
    scalar initialResidual(0.);
    scalar finalResidual(0.);
    label iterationsPerformed(0);
    bool converged(false);

    lduAddressing const & lduAddr = matrix_.lduAddr();
    word const preconditionerName = lduMatrix::preconditioner::getName(controlDict_);

    ofgpuPCGsolve(ofgpu::SparseMatrixArgs(preconditionerName.c_str(),
					  psi.size(), lduAddr.lowerAddr().size(),
					  lduAddr.losortStartAddr().begin(), lduAddr.losortAddr().begin(), lduAddr.lowerAddr().begin(), matrix_.lower().begin(), 
					  matrix_.diag().begin(),
					  lduAddr.ownerStartAddr().begin(), lduAddr.upperAddr().begin(), matrix_.upper().begin(),
					  psi.begin(),
					  source.begin(),
					  maxIter_, tolerance_, relTol_,
					  initialResidual, finalResidual, iterationsPerformed, converged));

    // --- Setup class containing solver performance data
    lduMatrix::solverPerformance solverPerf
    (
        preconditionerName + typeName,
        fieldName_,
	initialResidual,
	finalResidual,
	iterationsPerformed,
	converged
    );

    return solverPerf;
}

// ************************************************************************* //
