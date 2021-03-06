/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "jplotGraph.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(Foam::jplotGraph, 0);
const Foam::word Foam::jplotGraph::ext_("dat");

namespace Foam
{
    typedef graph::writer graphWriter;
    addToRunTimeSelectionTable(graphWriter, jplotGraph, word);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::jplotGraph::write(const graph& g, Ostream& os) const
{
    os  << "# JPlot file" << nl
        << "# column 1: " << g.xName() << endl;

    label fieldI = 0;

    forAllConstIter(graph, g, iter)
    {
        os  << "# column " << fieldI + 2 << ": " << (*iter()).name() << endl;
        fieldI++;
    }

    g.writeTable(os);
}


// ************************************************************************* //
