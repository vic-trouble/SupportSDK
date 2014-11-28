#ifndef __CONVERSIONSUTILITIES_H__
#define __CONVERSIONSUTILITIES_H__

#include "UtilitiesAPI.h"

#include "TypeDefines.h"

namespace SDK
  {

  namespace Utilities
    {
    template <typename TypeFrom>
    size_t ConvertToChar(uchar* op_buffer, const TypeFrom&& i_value);

    template <typename TypeFrom>
    size_t ConvertToChar(uchar* op_buffer, const TypeFrom& i_value);

    UTILITIES_API size_t ConvertToChar(uchar* op_buffer, const void* ip_data, size_t i_bytes_number);

    template <typename TypeTo>
    TypeTo ConvertFromChar(uchar* ip_buffer);

    //////////////////////////////////////////////////////

    template <typename TypeFrom>
    size_t ConvertToChar(uchar* op_buffer, const TypeFrom&& i_value)
      {
      const size_t bytes_to_copy = sizeof(TypeFrom);
      const uchar* source_bytes = static_cast<uchar*>(&i_value);
      for (size_t i = 0; i < bytes_to_copy; ++i)
        op_buffer[i] = source_bytes[i];
      return bytes_to_copy;
      }

    template <typename TypeFrom>
    size_t ConvertToChar(uchar* op_buffer, const TypeFrom& i_value)
      {
      const size_t bytes_to_copy = sizeof(TypeFrom);
      const uchar* source_bytes = reinterpret_cast<const uchar*>(&i_value);
      for (size_t i = 0; i < bytes_to_copy; ++i)
        op_buffer[i] = source_bytes[i];
      return bytes_to_copy;
      }
    
    template <typename TypeTo>
    TypeTo ConvertFromChar(uchar* ip_buffer)
      {
      TypeTo result_value;

      const size_t bytes_to_copy = sizeof(TypeTo);
      uchar* destination_bytes = reinterpret_cast<uchar*>(&result_value);
      for (size_t i = 0; i < bytes_to_copy; ++i)
        destination_bytes[i] = ip_buffer[i];
      return result_value;
      }

    } // Utilities

  } // SDK

#endif