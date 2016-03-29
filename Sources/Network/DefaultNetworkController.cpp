#include "stdafx.h"

#include "DefaultNetworkController.h"

#include "Utilities.h"

namespace Network
  {

  DefaultNetworkController::~DefaultNetworkController()
    {}

  bool DefaultNetworkController::Initialize()
    {
    return Utilities::InitializeSockets();
    }

  void DefaultNetworkController::Reset()
    {
    Utilities::ShutdownSockets();
    }

  } // Network