#include "stdafx.h"

#include "ConversionsUtilities.h"

namespace SDK
  {

  namespace Utilities
    {

    size_t ConvertToChar(uchar* op_buffer, const void* ip_data, size_t i_bytes_number)
      {
      memcpy(op_buffer, ip_data, i_bytes_number);
      return i_bytes_number;
      }

    } // Utilities

  } // SDK