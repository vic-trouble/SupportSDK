#pragma once

namespace Network
  {

  enum class PacketType
    {
    PT_NO_PACKETS       = 1000,
    PT_HasConnection	  = 0,	//two-way, server sends random number, client responds with the same number
    PT_Connects			    = 1,	//server -> client: current map
    PT_Maps				      = 2,	//server -> client: list of maps
    PT_StartGame		    = 3,	//client -> server: selected map
    PT_ClientReady		  = 4, //client -> server: at loading time notify server about the finishing of loading resources
    PT_ServerReady		  = 5, //server -> client: at loading time notify client about finishing preparing level
    PT_Position			    = 5,	//two-way
    PT_GOState			    = 6,	//server -> client
    PT_ControllerState	= 7,	//server -> client
    PT_SpawnEntity		  = 8,	//server -> client
    PT_CameraMove		    = 9,	//two-way
    PT_EndGame			    = 10, //two-way
    PT_Command			    = 11, //client -> server: sends commands from GUI (first 4 bytes are PacketType, 4 bytes - Command id, rest bytes - command data)
    PT_Selection		    = 12, //two-way: (?client -> server)
    PT_Definitions      = 13,
    PT_Achived          = 14,
    PT_ContentNumber    = 15,
    PT_GovermentState   = 16,
    };

  } // Network