/*
==============================================================================

This file is part of the PluginParameters module.
Copyright 2012-15 by 4drX

Permission is granted to use this software under the terms of the
GPL v2 (or any later version).

Details of this license can be found at: www.gnu.org/licenses

------------------------------------------------------------------------------

PluginParameters is provided WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

To release a closed-source product which uses PluginParameters, you will
need to purchase a commercial license. For more information, please send me
a PM (Personal Message) or reply to this thread at the JUCE forum:
http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

==============================================================================
*/

#include "ParamGroups.h"

void ParamGroup::addStringParamArray(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addStringParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

StringParamArray *ParamGroup::getStringParamArray(const int index) const{
  /* Wrong index? */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  StringParamArray *pointer=dynamic_cast<StringParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another type..."); return nullptr;}
  return pointer;
}

StringParamMatrix *ParamGroup::getStringParamMatrix(const int index) const{
  /* Wrong index? */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  StringParamMatrix *pointer=dynamic_cast<StringParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another type..."); return nullptr;}
  return pointer;
}

void ParamGroup::addBoolParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addBoolParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

BoolParamArray *ParamGroup::getBoolParamArray(const int index) const{
  /* Wrong index? */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  BoolParamArray *pointer=dynamic_cast<BoolParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another type..."); return nullptr;}
  return pointer;
}

BoolParamMatrix *ParamGroup::getBoolParamMatrix(const int index) const{
  /* Wrong index? */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  BoolParamMatrix *pointer=dynamic_cast<BoolParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another type..."); return nullptr;}
  return pointer;
}