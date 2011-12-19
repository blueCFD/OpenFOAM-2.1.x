
#if defined(WIN32) || defined(WIN64)
//LIBS_TO_LOAD - introduced by bruno.santos at bluecape dot com dot pt February 2010
//This allows to forcefully load libraries crucial to the application to run on Windows
#ifdef LIBS_TO_LOAD
  //example of LIBS_TO_LOAD: "OpenFOAM incompressibleRASModels Pstream"
  Foam::string libs_to_load(LIBS_TO_LOAD);
  Foam::dlOpen(libs_to_load);
#endif
#endif
