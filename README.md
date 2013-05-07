##Description

PluginParameters is a library to automatically handle plugin parameters (in plugins developed with [JUCE](http://rawmaterialsoftware.com/juce.php)). It intends to help automating things in the development of closs-platform plugins taking it from where JUCE leaves you in the wild. It should be simple to use with only a few lines doing the job and it provides two plugin templates which are convenient to create plugins from scratch. 

###What it does

* Automatically handles automated (synchronized with the host) parameters and non-automated (independent) parameters.
* Maps all kinds of parameter ranges (boolean, integer, float, logarithmic, logarithmic with sign) to the host float range of [0,1] back and forth.
* Saves/loads selected parameter values to/from XML data.
* Allows parameters to be grouped as ParamGroups, which can be nested. This allows to create quite easily independent plugin building blocks.
* Provides common ParamGroups as Arrays and Matrices for each parameter type.
* Should compile correctly in Windows and (if somebody helps me double check) in OSX.

###What it doesn't do

* Parameters changes are not thread safe however this shouldn't be a problem in most cases, if you know what you are doing (See forum post: [What's best practice for GUI change notification?](http://www.rawmaterialsoftware.com/viewtopic.php?f=8&t=10531)).

##[Reference](https://github.com/4drX/pluginparameters/wiki)

##[Forum and announcements](http://www.rawmaterialsoftware.com/viewtopic.php?f=6&t=11122&p=63732)

##License

This library is released under the [GPLv.2](http://www.gnu.org/licenses/gpl-2.0.html) but if you are interested in purchasing a commercial license to use it in proprietary code, please contact me with a PM at the [JUCE forum](http://www.rawmaterialsoftware.com/memberlist.php?mode=viewprofile&u=347).