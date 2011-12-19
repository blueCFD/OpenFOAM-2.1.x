/*---------------------------------------------------------------------------*\

    This file is based on the original version for POSIX:
        OpenFOAM/src/OSspecific/POSIX/

License
    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://www.bluecape.com.pt/blueCFD

    OpenFOAM's official site: http://www.openfoam.com

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "OSspecific.H"
#include "stack_trace.h"
#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void Foam::error::printStack(Ostream& os)
{
  if (env("FOAM_STACKTRACE_DISABLE"))
  {
    os << "Stack tracing is disabled (FOAM_STACKTRACE_DISABLE is set), so print stack will not be provided."
       << endl;
    return;
  }

  std::stringstream callstacktext(std::stringstream::in | std::stringstream::out);
  StackTrace *traceUs = new StackTrace();

  if(traceUs!=NULL)
  {
    traceUs->OutputToStream(&callstacktext);

    delete traceUs;
    traceUs=NULL;
    
    os << callstacktext.str().data();
  }
  else
  {
    os << "We're sorry, but the application crashed and stack tracing isn't working."
       << endl;
  }
}

// ************************************************************************* //

