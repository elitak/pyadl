#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include "adl_sdk.h"

using namespace boost::python;
 
BOOST_PYTHON_MODULE(pyadl)
{
   class_<ADLClockInfo>("ADLClockInfo", init<>())
      .def_readwrite("iCoreClock", &ADLClockInfo::iCoreClock)
      .def_readwrite("iMemoryClock", &ADLClockInfo::iMemoryClock)
   ;
   class_<ADLODPerformanceLevel>("ADLODPerformanceLevel", init<>())
      .def_readwrite("iEngineClock", &ADLODPerformanceLevel::iEngineClock)
      .def_readwrite("iMemoryClock", &ADLODPerformanceLevel::iMemoryClock)
      .def_readwrite("iVddc", &ADLODPerformanceLevel::iVddc)
   ;

}

