/*---------------------------------------------------------------------------*\
This file was developed by:
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com
-------------------------------------------------------------------------------

License
    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://www.bluecape.com.pt/blueCFD

    OpenFOAM's official site: http://www.openfoam.com

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
    sigInt

Description
    Signal handler for INT interrupt.  The standard interupt handler is
    overridden to ensure that the runningJob file is removed.

SourceFiles
    sigInt.C

\*---------------------------------------------------------------------------*/

#include "OSspecific.H"

#include <signal.h>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class sigInt Declaration
\*---------------------------------------------------------------------------*/

class sigInt
{
    // Private data

        //- Saved old signal trapping setting
        static __p_sig_fn_t oldAction_;


    // Private Member Functions

        static void sigIntHandler(int);


public:

    // Constructors

        sigInt();


    // Destructor

        ~sigInt();


    // Member functions

        void set(const bool verbose);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
