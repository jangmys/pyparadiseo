#include "abstract.h"
#include <pyeot.h>

template void export_abstract<PyEOT>(std::string s);
template void export_abstract<BinarySolution>(std::string s);
template void export_abstract<RealSolution>(std::string s);
