#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/list.hpp>
#include <boost/python/manage_new_object.hpp>
#include "adl_sdk.h"
#include "adl.h"

using namespace boost::python;
using boost::noncopyable;
using boost::shared_ptr;
using namespace std;

class Adapter {
public:
   Adapter(int iAdapterIndex) : index(iAdapterIndex) {}
   ADLTemperature* getTemperature(int iThermalControllerIndex) {
      ADLTemperature* lpTemperature = new ADLTemperature();
      ADL::Instance()->ADL_Overdrive5_Temperature_Get(index, iThermalControllerIndex, lpTemperature);
      return lpTemperature;
   }

   list getPerformanceLevels() {
      ADL* adl = ADL::Instance();
      int features = ADL::FEAT_GET_OD_PARAMETERS | ADL::FEAT_GET_OD_PERF_LEVELS;

      if ((adl->GetSupportedFeatures() & features) != features) {
         throw runtime_error("features missing from device");
      }

      ADLODParameters para;
      if (adl->ADL_Overdrive5_ODParameters_Get(index, &para) != ADL_OK) {
         throw runtime_error("could not fetch current parameters");
      }

      int perf_level_size = sizeof(ADLODPerformanceLevels) + sizeof(ADLODPerformanceLevel) * (para.iNumberOfPerformanceLevels - 1);
      ADLODPerformanceLevels* levels = (ADLODPerformanceLevels*)malloc(perf_level_size);
      levels->iSize = perf_level_size;
      if(adl->ADL_Overdrive5_ODPerformanceLevels_Get(index, 0, levels) != ADL_OK) {
         free(levels);
         throw runtime_error("could not fetch performance levels");
      }

      list levelsList;
      for (int ndx = 0; ndx < para.iNumberOfPerformanceLevels; ndx++) {
         levelsList.append(levels->aLevels[ndx]);
      }

      return levelsList;
   }

   void setPerformanceLevels(object levels) {
      ADL* adl = ADL::Instance();
      int features = ADL::FEAT_GET_OD_PARAMETERS | ADL::FEAT_GET_OD_PERF_LEVELS;

      if ((adl->GetSupportedFeatures() & features) != features) {
         throw runtime_error("features missing from device");
      }

      ADLODParameters para;
      if (adl->ADL_Overdrive5_ODParameters_Get(index, &para) != ADL_OK) {
         throw runtime_error("could not fetch current parameters");
      }

      int perf_level_size = sizeof(ADLODPerformanceLevels) + sizeof(ADLODPerformanceLevel) * (para.iNumberOfPerformanceLevels - 1);
      ADLODPerformanceLevels* newLevels = (ADLODPerformanceLevels*) malloc(perf_level_size);
      newLevels->iSize = perf_level_size;
      if(adl->ADL_Overdrive5_ODPerformanceLevels_Get(index, 0, newLevels) != ADL_OK) {
         free(newLevels);
         throw runtime_error("could not fetch performance levels");
      }

      // TODO check that len is correct
      for (int ndx = 0; ndx < len(levels); ndx++)
      {
         //adl->ADL_Overdrive5_ODPerformanceLevels_Get(index, newLevels)
         ADLODPerformanceLevel& lev = extract<ADLODPerformanceLevel&>(levels[ndx]);
         newLevels->aLevels[ndx] = lev;
      }

      if(adl->ADL_Overdrive5_ODPerformanceLevels_Set(index, newLevels) != ADL_OK) {
         free(newLevels);
         throw runtime_error("could not set values");
      }

      free(newLevels);
   }

protected:
   int index;
};
 
BOOST_PYTHON_MODULE(pyadl)
{
   class_<ADLTemperature>("Temperature", init<>())
      .def_readwrite("iSize", &ADLTemperature::iSize)
      .def_readwrite("iTemperature", &ADLTemperature::iTemperature)
   ;
   class_<ADLClockInfo>("ClockInfo", init<>())
      .def_readwrite("iCoreClock", &ADLClockInfo::iCoreClock)
      .def_readwrite("iMemoryClock", &ADLClockInfo::iMemoryClock)
   ;
   /*
   class_<ADLODPerformanceLevels>("ODPerformanceLevels", init<>())
      .def_readwrite("iSize", &ADLODPerformanceLevels::iSize)
      .def_readwrite("iReserved", &ADLODPerformanceLevels::iSize)
      .def_readwrite("aLevels", &ADLODPerformanceLevels::aLevels)
   ;
   */
   class_<ADLODPerformanceLevel>("ODPerformanceLevel", init<>())
      .def_readwrite("iEngineClock", &ADLODPerformanceLevel::iEngineClock)
      .def_readwrite("iMemoryClock", &ADLODPerformanceLevel::iMemoryClock)
      .def_readwrite("iVddc", &ADLODPerformanceLevel::iVddc)
   ;
   class_<ADL, noncopyable>("ADL", no_init)
      .def("Instance", ADL::Instance, return_value_policy<reference_existing_object>())
      .staticmethod("Instance")
      .def("IsATICardAndCatalystPresent", &ADL::IsATICardAndCatalystPresent)
   ;
   class_<Adapter>("Adapter", init<int>((boost::python::arg("index") = 0), "Select an adapter by supplying and index"))
      .def("getTemperature", &Adapter::getTemperature, return_value_policy<manage_new_object>(), "Get the temperature from a specified thermal controller", (
         boost::python::arg("sensorIndex") = 0
      ))
      .def("getPerformanceLevels", &Adapter::getPerformanceLevels, "Get the performance levels")
      .def("setPerformanceLevels", &Adapter::setPerformanceLevels, "Set the performance levels", (
         boost::python::arg("levels")
      ))
   ;
}

