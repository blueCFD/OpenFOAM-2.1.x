/*---------------------------------------------------------------------------*\
    Copyright            : (C) 2012 Symscape
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
    // Reset old handling
    const __p_sig_fn_t success = ::signal(signal_, oldAction_);
    oldAction_ = SIG_DFL;
    
    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "Foam::sigStopAtWriteNow::sigHandler(int)"
        )   << "Cannot reset " << signal_ << " trapping"
            << abort(FatalError);
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
    if (signal_ > 0)
    {
        // Check that the signal is different from the writeNowSignal
        if (sigWriteNow::signal_ == signal_)
        {
            FatalErrorIn
            (
                "Foam::sigStopAtWriteNow::sigStopAtWriteNow"
                "(const bool, const Time&)"
            )   << "stopAtWriteNowSignal : " << signal_
                << " cannot be the same as the writeNowSignal."
                << " Please change this in the controlDict ("
                << findEtcFile("controlDict", false) << ")."
                << exit(FatalError);
        }


        // Store runTime
        runTimePtr_ = &runTime;

	oldAction_ = ::signal(signal_, &Foam::sigWriteNow::sigHandler);        

	if (SIG_ERR == oldAction_)
        {
            FatalErrorIn
            (
                "Foam::sigStopAtWriteNow::sigStopAtWriteNow"
                "(const bool, const Time&)"
            )   << "Cannot set " << signal_ << " trapping"
                << abort(FatalError);
        }

        if (verbose)
        {
            Info<< "sigStopAtWriteNow :"
                << " Enabling writing and stopping upon signal " << signal_
                << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigStopAtWriteNow::~sigStopAtWriteNow()
{
    // Reset old handling
    if (signal_ > 0)
    {
        const __p_sig_fn_t success = ::signal(signal_, oldAction_);
	oldAction_ = SIG_DFL;

	if (SIG_ERR == success)
        {
            FatalErrorIn
            (
                "Foam::sigStopAtWriteNow::~sigStopAtWriteNow()"
            )   << "Cannot reset " << signal_ << " trapping"
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::sigStopAtWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
