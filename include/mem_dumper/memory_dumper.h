#pragma once
#include <memory>
#include <string>

namespace MemoryLeaksDetection
{
class MemoryDumper
{
public:
  MemoryDumper( const std::string& dumper_name );
  virtual ~MemoryDumper() = default;
protected:
  std::string dumper_name_;
};
using MemoryDumperPtr = std::shared_ptr<MemoryDumper>;

MemoryDumperPtr makeMemoryDumper( const std::string& dumper_name );
} // namespace MemoryLeaksDetection