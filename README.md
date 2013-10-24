##Description

PluginParameters is a [JUCE module](http://www.juce.com/documentation/juce-module-format) which manages automatically different types of plugin parameters. It intends to help automating things in the development of cross-platform plugins taking it from where JUCE leaves you in the wild. 

##License

The PluginParameters module is released under the [GPLv.2](http://www.gnu.org/licenses/gpl-2.0.html) *without the Preset Manager*. A Commercial License (available for 150eur) provides you additionally a Preset Manager and the possibility to use it in any kind of close-sourced or/and commercial projects.

If this project has saved hours of your precious time, please consider either buying a Commercial License, making a donation (donations can count towards buying a Commercial License, if you wish so) or simply contributing time and code back. This work has been exclusively supported with private earnings, time and energy and it doesn't include any advertising or any hidden source of income whatsoever; so it relies on your contributions.

For more details, send me a Personal Message at [this thread at the JUCE forum](http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters) (MarC).

## Installation

If you are new to [JUCE](http://www.juce.com), first [download it](http://www.juce.com/downloads) into a local folder. Next drag-and-drop the "modules" folder to your local JUCE folder. When you create a new project in JUCE, the [Introjucer](http://www.juce.com/documentation/introjucer) will detect automatically the PluginParameters module. Switch it on and you are ready to go!

###What it does

* Maps many common parameter types (boolean, integer, float, logarithmic, logarithmic with sign) to the host float range of [0,1] back and forth.
* Allows parameters to be registered at the host (automated by default) or to be left unregistered (non-automated) but saved/restored anyway internally at the end/beginning of each session..
* Allows parameters to be grouped as ParamGroups, which can be nested. This allows to create quite easily independent plugin building blocks.
* Provides common ParamGroups as Arrays and Matrices for each defined parameter type.
* Saves/loads selected parameter values to/from XML data. Methods are provided to save/load also entire ParamGroups directly to/from XML data.
* Should compile correctly in Visual Studio and Xcode.
* (only with a commercial license) It provides a Preset Manager:

The Presets manager is a class which provides utilities to load, save, rename, delete, reset preset files that store all parameter values of a ParamGroup recursively. It marks files that are loaded as read-only so that they cannot loaded twice or modified outside the plugin, supports preset "folder-files" (where the actual preset file is created inside a folder of the same name) and tracks unsaved changes with an "*" next to the preset file name.

A demo of a possible UI for this class is available in the [PluginParametersGroupsDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersGroupsDemo) example. Additionally, it includes commented lines of the code that would be necessary to setup the Preset Manager.

###What it doesn't do

* Parameter changes are not thread safe however this shouldn't be a problem in most cases, if you know what you are doing (See forum post: [What's best practice for GUI change notification?](http://www.rawmaterialsoftware.com/viewtopic.php?f=8&t=10531)).

##[Reference](https://github.com/4drX/pluginparameters/wiki)

This page provides detailed instructions to create your plugin from scratch using the PluginParameters module. Alternatively, you can build on the example plugins available in the "plugins" folder: 
* [PluginParametersDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersDemo) 
* [PluginParametersGroupsDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersGroupsDemo)

This two plugins are released independently from the PluginParameters module, with a MIT license.

##[Doxygen Documentation](http://4drx.github.io/pluginparameters/annotated.html)

This will probably help you understand the code quicker.

##[Forum and announcements](http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters)

Please submit any comments, bug reports, etc here.