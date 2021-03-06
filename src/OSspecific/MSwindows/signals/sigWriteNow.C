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

#include "sigWriteNow.H"
#include "error.H"
#include "JobInfo.H"
#include "IOstreams.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

// Signal number to catch
int Foam::sigWriteNow::signal_
(
    debug::optimisationSwitch("writeNowSignal", -1)
);

static Foam::Time* runTimePtr_ = NULL;


__p_sig_fn_t Foam::sigWriteNow::oldAction_ = SIG_DFL;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::sigWriteNow::sigHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(signal_, oldAction_);
    oldAction_ = SIG_DFL;
    
    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "Foam::sigWriteNow::sigHandler(int)"
        )   << "Cannot reset " << signal_ << " trapping"
            << abort(FatalError);
    }

    Info<< "sigWriteNow :"
        << " setting up write at end of the next iteration" << nl << endl;
    runTimePtr_->writeOnce();

    //// Throw signal (to old handler)
    //raise(signal_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigWriteNow::sigWriteNow()
{}


Foam::sigWriteNow::sigWriteNow(const bool verbose, Time& runTime)
{
    if (signal_ >= 0)
    {
        // Store runTime
        runTimePtr_ = &runTime;

	oldAction_ = ::signal(signal_, &Foam::sigWriteNow::sigHandler);        

	if (SIG_ERR == oldAction_)
	{
            FatalErrorIn
            (
                "Foam::sigWriteNow::sigWriteNow(const bool, const Time&)"
            )   << "Cannot set " << signal_ << " trapping"
                << abort(FatalError);
        }

        if (verbose)
        {
            Info<< "sigWriteNow :"
                << " Enabling writing upon signal " << signal_
                << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigWriteNow::~sigWriteNow()
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
                "Foam::sigWriteNow::~sigWriteNow()"
            )   << "Cannot reset " << signal_ << " trapping"
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::sigWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
