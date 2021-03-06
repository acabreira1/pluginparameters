##Description

PluginParameters is a [JUCE module](http://www.juce.com/documentation/juce-module-format) which manages automatically different types of plugin parameters. It intends to help automating things in the development of cross-platform plugins taking it from where JUCE leaves you in the wild. 

##License

This version of the PluginParameters module is released under the [GPLv.2](http://www.gnu.org/licenses/gpl-2.0.html). Please consider a symbolic donation if you wish to see it updated or sponsor a feature (they will be deduced from the Commercial License, if you consider upgrading later). Alternatively, a Commercial License (available for 149eur) allows you to use this module in closed-source projects and provides you full compatibility with the latest release of JUCE, support for XCode and Visual Studio and extra features.

For more details, send me a Personal Message at [the JUCE forum](http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters) (MarC).

## Installation

If you are new to [JUCE](http://www.juce.com), first [download it](http://www.juce.com/downloads) into a local folder. Next drag-and-drop the "modules" folder to your local JUCE folder. When you create a new project in JUCE, the [Introjucer](http://www.juce.com/documentation/introjucer) will detect automatically the PluginParameters module. Switch it on and you are ready to go!

###What it does

* It can be used to build a VST/AU/RTAS plugin or built-in plugins (AudioPluginInstance's).
* Includes methods to map any integer, float or bool parameter types to the host float range of [0,1] back and forth linearly, logarithmically (w/out a 0, w/out a sign). Custom mappings can also be defined.
* Parameters can be grouped as ParamGroups, which may be nested. This allows to create quite easily independent plugin building blocks.
* Arrays and Matrices are defined for each defined parameter type (they are both a subclass of ParamGroup).
* Allows parameters to be registered at the host (automated by default) or to be left unregistered (non-automated) but saved/restored anyway internally at the end/beginning of each session.
* Provides methods to load/save selected parameters from/to the host session automatically.
* Should compile correctly in Visual Studio and Xcode.

###Only with a commercial license

* Preset Manager.
* Includes Undo/Redo's from parameter changes made in the UI.

The Preset manager is a class which provides utilities to load, save, rename, delete, reset XML preset files that store all parameter values of a ParamGroup recursively. It marks files that are loaded as read-only so that they cannot loaded twice or modified outside the plugin, supports preset "folder-files" (where the actual preset file is created inside a folder of the same name) and tracks unsaved changes with an "*" next to the preset file name.

A demo of a possible UI for this class is available in the [PluginParametersGroupsDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersGroupsDemo) example. Additionally, it includes commented lines of the code that would be necessary to setup the Preset Manager.

Please refer to the last section of [Reference](https://github.com/4drX/pluginparameters/wiki#undosredos-only-with-a-commercial-license) (Advanced Usages), to see how Undos/Redos, custom float parameter mappings and parameters with other float and int types are supported.

##[Reference](https://github.com/4drX/pluginparameters/wiki)

This page provides detailed instructions to create your plugin from scratch using the PluginParameters module. Alternatively, you can build on the example plugins available in the "plugins" folder: 
* [PluginParametersDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersDemo) 
* [PluginParametersGroupsDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersGroupsDemo)

These two plugins are released independently from the PluginParameters module, with a MIT license.

##[Doxygen Documentation](http://4drx.github.io/pluginparameters/annotated.html)

This will probably help you understand the code quicker.

##[Forum and announcements](http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters)

Tune-in to the JUCE forum to submit comments, bug reports, etc.
