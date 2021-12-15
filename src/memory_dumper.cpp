#include "memory_dumper.h"
#include <iostream>

#if defined(_WIN32)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdio.h>
#endif


namespace MemoryLeaksDetection
{
MemoryDumper::MemoryDumper( const std::string& dumper_name )
  : dumper_name_( dumper_name )
{

}

#if defined(_WIN32)
class MsvcMemoryDumper : public MemoryDumper
{
public:
  MsvcMemoryDumper( const std::string& dumper_name )
    : MemoryDumper( dumper_name )
  {
    _CrtMemCheckpoint(&s1);
  }
  virtual ~MsvcMemoryDumper() override
  {
    _CrtMemCheckpoint(&s2);
    _CrtMemDifference( &s3, &s1, &s2 );
    _CrtMemDumpStatistics( &s3 );
  }
private:
  _CrtMemState s1 = {}, s2 = {}, s3 = {};
};
#endif


MemoryDumperPtr makeMemoryDumper( const std::string& dumper_name )
{
#if defined(_WIN32)
  return std::make_shared<MsvcMemoryDumper>( dumper_name );
#endif

  return nullptr;
}
} // namespace MemoryLeaksDetection