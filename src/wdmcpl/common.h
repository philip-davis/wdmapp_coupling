#ifndef WDM_COUPLING_COMMON_H
#define WDM_COUPLING_COMMON_H
#include <redev.h>
#include <unordered_map>
#include "wdmcpl/transfer_field.h"
#include "wdmcpl/assert.h"
#include "wdmcpl/config.h"

#ifdef USE_APEX
#include <apex.h>
#define APEX_FUNC_TIMER_START(fn)                                              \
    apex_profiler_handle profiler0 = apex_start(APEX_FUNCTION_ADDRESS, &fn);
#define APEX_NAME_TIMER_START(num, name)                                       \
    apex_profiler_handle profiler##num = apex_start(APEX_NAME_STRING, name);
#define APEX_TIMER_STOP(num) apex_stop(profiler##num);
#else
#define APEX_FUNC_TIMER_START(fn) (void)0;
#define APEX_NAME_TIMER_START(num, name) (void)0;
#define APEX_TIMER_STOP(num) (void)0;
#endif

namespace wdmcpl
{
using ProcessType = redev::ProcessType;
class GatherOperation;
class ScatterOperation;

namespace detail
{
// helper function for dealing with field maps
template <typename T, typename U>
auto& find_or_error(const std::string& name,
                    const std::unordered_map<T, U>& map)
{
  auto it = map.find(name);
  WDMCPL_ALWAYS_ASSERT(it != map.end());
  return it->second;
}

template <typename T, typename U>
auto& find_or_error(const std::string& name, std::unordered_map<T, U>& map)
{
  auto it = map.find(name);
  WDMCPL_ALWAYS_ASSERT(it != map.end());
  return it->second;
}
template <typename T, typename U>
auto find_many_or_error(const std::vector<T>& keys,
                        const std::unordered_map<T, U>& map)
{

  std::vector<std::reference_wrapper<U>> results;
  results.reserve(keys.size());
  std::transform(keys.begin(), keys.end(), std::back_inserter(results),
                 [&map](const std::string& key) {
                   return std::ref(detail::find_or_error(key, map));
                 });
  return results;
}
template <typename T, typename U>
auto find_many_or_error(const std::vector<T>& keys,
                        std::unordered_map<T, U>& map)
{

  std::vector<std::reference_wrapper<U>> results;
  results.reserve(keys.size());
  std::transform(keys.begin(), keys.end(), std::back_inserter(results),
                 [&map](const std::string& key) {
                   return std::ref(detail::find_or_error(key, map));
                 });
  return results;
}
} // namespace detail

struct TransferOptions
{
  FieldTransferMethod transfer_method;
  FieldEvaluationMethod evaluation_method;
};
} // namespace wdmcpl

#endif // WDM_COUPLING_COMMON_H
