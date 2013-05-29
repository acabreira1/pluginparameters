# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../Source
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../Source
  TARGET := PluginParametersMidiFX.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../Source
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -Os
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vstsdk2.4 -I ../../JuceLibraryCode -I ../../Source
  TARGET := PluginParametersMidiFX.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

OBJECTS := \
  $(OBJDIR)/MyPluginProcessor_ce34cb34.o \
  $(OBJDIR)/MidiDelay_32b53b6.o \
  $(OBJDIR)/MidiDelayComponent_94ee9931.o \
  $(OBJDIR)/MidiNoteGain_11108f0.o \
  $(OBJDIR)/MidiNoteGainComponent_ae482537.o \
  $(OBJDIR)/MidiSustain_c7bcf6b6.o \
  $(OBJDIR)/MidiSustainComponent_26845631.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/juce_audio_basics_3e4c0f0c.o \
  $(OBJDIR)/juce_audio_devices_612fd0ca.o \
  $(OBJDIR)/juce_audio_formats_8fb11a6a.o \
  $(OBJDIR)/juce_audio_processors_30f71cc4.o \
  $(OBJDIR)/juce_audio_utils_976c5d8a.o \
  $(OBJDIR)/juce_core_688c2fee.o \
  $(OBJDIR)/juce_data_structures_396c5d2a.o \
  $(OBJDIR)/juce_events_388f562.o \
  $(OBJDIR)/juce_graphics_511053c6.o \
  $(OBJDIR)/juce_gui_basics_e3186842.o \
  $(OBJDIR)/juce_gui_extra_719bc24a.o \
  $(OBJDIR)/juce_PluginUtilities_31553fd6.o \
  $(OBJDIR)/juce_VST_Wrapper_e9df50df.o \
  $(OBJDIR)/PluginParameters_41d270aa.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking PluginParametersMidiFX
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning PluginParametersMidiFX
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping PluginParametersMidiFX
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/MyPluginProcessor_ce34cb34.o: ../../Source/MyPluginProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MyPluginProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiDelay_32b53b6.o: ../../Source/MidiDelay/MidiDelay.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiDelay.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiDelayComponent_94ee9931.o: ../../Source/MidiDelay/MidiDelayComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiDelayComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiNoteGain_11108f0.o: ../../Source/MidiNoteGain/MidiNoteGain.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiNoteGain.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiNoteGainComponent_ae482537.o: ../../Source/MidiNoteGain/MidiNoteGainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiNoteGainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiSustain_c7bcf6b6.o: ../../Source/MidiSustain/MidiSustain.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiSustain.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiSustainComponent_26845631.o: ../../Source/MidiSustain/MidiSustainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MidiSustainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_3e4c0f0c.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_612fd0ca.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_8fb11a6a.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_30f71cc4.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_976c5d8a.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_688c2fee.o: ../../../../../../../soft/libscpp/juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_396c5d2a.o: ../../../../../../../soft/libscpp/juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_388f562.o: ../../../../../../../soft/libscpp/juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_511053c6.o: ../../../../../../../soft/libscpp/juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_e3186842.o: ../../../../../../../soft/libscpp/juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_719bc24a.o: ../../../../../../../soft/libscpp/juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_31553fd6.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_e9df50df.o: ../../../../../../../soft/libscpp/juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginParameters_41d270aa.o: ../../../../../../../soft/libscpp/juce/modules/pluginparameters/PluginParameters.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginParameters.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
