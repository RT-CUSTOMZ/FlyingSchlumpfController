#pragma once

#include <stdint.h>

enum class Type : uint32_t {
  invalid,
  buttonPressed,
  developDemoEvent,
};

typedef struct struct_message
{
  struct_message(Type type)
  : type(static_cast<uint32_t>(type))
  {
  }
  uint32_t type;
} struct_message;

struct ButtonMessage : struct_message
{
  ButtonMessage()
  : struct_message(Type::buttonPressed)
  {
  }
  uint32_t value;
};

struct DevelopmentDemoMessage : struct_message
{
  DevelopmentDemoMessage()
  : struct_message(Type::developDemoEvent)
  {
  }
  uint32_t value;
};
