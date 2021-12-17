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
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

    _CrtMemCheckpoint(&s1);
  }
  virtual ~MsvcMemoryDumper() override
  {
    std::cout << "========================================================\n";
    std::cout << "MemoryDumper: " << dumper_name_ << std::endl;
    std::cout << "========================================================\n";
    _CrtMemCheckpoint(&s2);
    _CrtMemDifference( &s3, &s1, &s2 );
    _CrtMemDumpStatistics( &s3 );
    std::cout << "========================================================\n\n";
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