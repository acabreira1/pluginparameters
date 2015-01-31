/*
  ==============================================================================

   This file is part of the PluginParameters module.
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters is provided WITHOUT ANY WARRANTY; without even the implied 
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   To release a closed-source product which uses PluginParameters, commercial 
   licenses are available. For more information, please send me a PM (Personal 
   Message) or reply to this thread at the JUCE forum: 
   http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

  ==============================================================================
*/

// Your project must contain an AppConfig.h file with your project-specific settings in it,
// and your header search path must make it accessible to the module's files.
#include "AppConfig.h"

#ifndef PluginParameters_Epsilon
  #define PluginParameters_Epsilon (PluginParameters_HostFloatType)1e-6
#endif

#ifndef PluginParameters_ErrorLogger
  #define PluginParameters_ErrorLogger 0
#endif

#ifndef DONT_SET_USING_PLUGINPARAMETERS_NAMESPACE
  #define DONT_SET_USING_PLUGINPARAMETERS_NAMESPACE 0
#endif

#ifndef DEFINE_DEFAULT_HOST_TYPES
  typedef float PluginParameters_HostFloatType;
#endif

#ifndef PLUGINPARAMETERS
#define PLUGINPARAMETERS

#include "../juce_audio_basics/juce_audio_basics.h"
#include "../juce_audio_plugin_client/juce_audio_plugin_client.h"
#include "../juce_core/juce_core.h"
#include "../juce_data_structures/juce_data_structures.h"
#include "../juce_events/juce_events.h"
#include "../juce_audio_processors/juce_audio_processors.h"

namespace PluginParameters {  
  #ifndef __PLUGINPARAMETERS_PARAMS_HEADER__
  #include "sources/Params.h"
  #endif
  #ifndef __PLUGINPARAMETERS_PARAMGROUPS_HEADER__
  #include "sources/ParamGroups.h"
  #endif
  #ifndef __PLUGINPARAMETERS_PLUGINPROCESSOR_HEADER__
  #include "sources/PluginProcessor.h"
  #endif  
}

#if ! DONT_SET_USING_PLUGINPARAMETERS_NAMESPACE
  using namespace PluginParameters;
#endif

#endif