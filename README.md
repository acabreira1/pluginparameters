##Description

PluginParameters is a JUCE module which manages different types of plugin parameters automatically in plugins developed with [JUCE](http://rawmaterialsoftware.com/juce.php). It intends to help automating things in the development of closs-platform plugins taking it from where JUCE leaves you in the wild. 

## Installation

If you are new to JUCE, first download [JUCE](http://www.juce.com/downloads) into a local folder. Next drag-and-drop the "modules" folder to your local JUCE folder. When you create a new project in JUCE, the Introjucer will detect automatically the PluginParameters module. Switch it on and you are ready to go!

###What it does

* Automatically handles automated (synchronized with the host) parameters and non-automated (independent) parameters.
* Maps many common parameter types (boolean, integer, float, logarithmic, logarithmic with sign) to the host float range of [0,1] back and forth. Custom parameter types can also be setup by inheriting from the class Param.
* Allows parameters to be grouped as ParamGroups, which can be nested. This allows to create quite easily independent plugin building blocks.
* Provides common ParamGroups as Arrays and Matrices for each defined parameter type.
* Saves/loads selected parameter values to/from XML data. Methods are provided to save/load also entire ParamGroups directly to/from XML data.
* Should compile correctly in Windows and (if somebody helps me double check) in OSX.

###What it doesn't do

* Parameters changes are not thread safe however this shouldn't be a problem in most cases, if you know what you are doing (See forum post: [What's best practice for GUI change notification?](http://www.rawmaterialsoftware.com/viewtopic.php?f=8&t=10531)).

##[Reference](https://github.com/4drX/pluginparameters/wiki)

This page provides detailed instructions to create your plugin from scratch using the PluginParameters module. Alternatively, you can build on the example plugins available in the "plugins" folder: 
* [PluginParametersDemo](https://github.com/4drX/pluginparameters/tree/master/plugins/PluginParametersDemo) 
* [MidiFX](https://github.com/4drX/pluginparameters/tree/master/plugins/MidiFX)

##[Doxygen Documentation](http://4drx.github.io/pluginparameters/annotated.html)

This will probably help you understand the code quicker.

##[Forum and announcements](http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=11122&p=63732)

Please submit any comments, bug reports, etc here.

##License

This library is released under the [GPLv.2](http://www.gnu.org/licenses/gpl-2.0.html) but if you are interested in purchasing a commercial license to use it in proprietary code, please contact me with a PM at the [JUCE forum](http://www.rawmaterialsoftware.com/memberlist.php?mode=viewprofile&u=347). The cost of a commercial license is 120eur.