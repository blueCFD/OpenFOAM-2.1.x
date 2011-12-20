/*---------------------------------------------------------------------------*\
This file was developed by:
    Copyright            : (C) 2011 blueCAPE
    Website              : www.bluecape.com.pt
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

\*---------------------------------------------------------------------------*/

#include "sigStopAtWriteNow.H"
#include "error.H"
#include "JobInfo.H"
#include "IOstreams.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Signal number to catch
int Foam::sigStopAtWriteNow::signal_
(
    debug::optimisationSwitch("stopAtWriteNowSignal", -1)
);

static Foam::Time const* runTimePtr_ = NULL;


__p_sig_fn_t Foam::sigStopAtWriteNow::oldAction_ = SIG_DFL;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::sigStopAtWriteNow::sigHandler(int)
{
    if (verbose)
    {
        WarningIn("Foam::sigStopAtWriteNow::sigHandler(int)")
          << "Not implemented."
          << endl;
    }

    // Update jobInfo file
    jobInfo.signalEnd();

    Info<< "sigStopAtWriteNow :"
        << " setting up write and stop at end of the next iteration"
        << nl << endl;
    runTimePtr_->stopAt(Time::saWriteNow);

    //// Throw signal (to old handler)
    //raise(signal_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigStopAtWriteNow::sigStopAtWriteNow(){}


Foam::sigStopAtWriteNow::sigStopAtWriteNow
(
    const bool verbose,
    const Time& runTime
)
{
    signal_ = 0;

    if (verbose)
    {
        WarningIn("Foam::sigStopAtWriteNow::sigStopAtWriteNow(verbose,runTime)")
          << "Not implemented."
          << endl;
    }

    // Store runTime
    runTimePtr_ = &runTime;

    if (verbose)
    {
        Info<< "sigStopAtWriteNow :"
            << " Enabling writing and stopping upon (dummy) signal " << signal_
            << endl;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigStopAtWriteNow::~sigStopAtWriteNow()
{
    if (verbose)
    {
        WarningIn("Foam::sigStopAtWriteNow::~sigStopAtWriteNow()")
          << "Not implemented."
          << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::sigStopAtWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
