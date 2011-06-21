#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include "adl_sdk.h"
#include "adl.h"

using namespace boost::python;
using boost::noncopyable;
 
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
   class_<ADL, noncopyable>("ADL", no_init)
      .def("Instance", ADL::Instance, return_value_policy<reference_existing_object>())
      .staticmethod("Instance")
      .def("IsATICardAndCatalystPresent", &ADL::IsATICardAndCatalystPresent)
   ;
}

