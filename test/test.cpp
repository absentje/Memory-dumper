#include <mem_dumper/memory_dumper.h>

int f()
{
  int* a = new int( 100 );
  return *a;
}

int main()
{
  {
    auto mem_dumper = MemoryLeaksDetection::makeMemoryDumper( "mem_dumper" );
    for ( int i = 0; i != 10; ++i )
    {
      f();
    }
  }
  return 0;
}