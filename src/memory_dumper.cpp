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
    _CrtMemCheckpoint(&s2);
    _CrtMemDifference( &s3, &s1, &s2 );

  #define _FREE_BLOCK      0
  #define _NORMAL_BLOCK    1
  #define _CRT_BLOCK       2
  #define _IGNORE_BLOCK    3
  #define _CLIENT_BLOCK    4

    auto is_not_empty = [this]( int block_type )
    {
      return (int)s3.lCounts[block_type] > 0 && (int)s3.lSizes[block_type] > 0;
    };

    bool have_diff = is_not_empty(_FREE_BLOCK) ||
      is_not_empty(_NORMAL_BLOCK) ||
      is_not_empty(_CRT_BLOCK) ||
      is_not_empty(_IGNORE_BLOCK) ||
      is_not_empty( _CLIENT_BLOCK );

    if ( have_diff )
    {
      std::cout << "========================================================\n";
      std::cout << "MemoryDumper: " << dumper_name_ << std::endl;
      std::cout << "========================================================\n";
      _CrtMemDumpStatistics( &s3 );
      std::cout << "========================================================\n\n";
    }
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