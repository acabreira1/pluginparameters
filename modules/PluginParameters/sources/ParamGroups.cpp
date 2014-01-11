/*
  ==============================================================================

   This file is part of the PluginParameters module.
   Copyright 2012-13 by MarC

  ------------------------------------------------------------------------------

   PluginParameters can be redistributed and/or modified under the terms of the 
   GNU General Public License (Version 2), as published by the Free Software 
   Foundation. A copy of the license is included in the JUCE distribution, or 
   can be found online at www.gnu.org/licenses.

   PluginParameters is distributed in the hope that it will be useful, but 
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
   details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses PluginParameters, commercial 
   licenses are available. For more information, please send me a PM (Personal 
   Message) or reply to this thread at the JUCE forum: 
   http://www.juce.com/forum/topic/juce-module-automatically-handle-plugin-parameters

  ==============================================================================
*/

#include "ParamGroups.h"

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

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
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="String");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<StringParamArray *>(paramGroupList[index]);
}

StringParamMatrix *ParamGroup::getStringParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="String");
  /* Double check this ParamGroup. This might not be a ParamMatrix */   
  return dynamic_cast<StringParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addFloatParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addFloatParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

FloatParamArray *ParamGroup::getFloatParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Float");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<FloatParamArray *>(paramGroupList[index]);
}

FloatParamMatrix *ParamGroup::getFloatParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Float");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<FloatParamMatrix *>(paramGroupList[index]);
}


void ParamGroup::addLogParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogParamArray *ParamGroup::getLogParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Log");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogParamArray *>(paramGroupList[index]);
}

LogParamMatrix *ParamGroup::getLogParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Log");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addLogWith0ParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWith0ParamArray *ParamGroup::getLogWith0ParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWith0");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogWith0ParamArray *>(paramGroupList[index]);
}

LogWith0ParamMatrix *ParamGroup::getLogWith0ParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWith0");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogWith0ParamMatrix *>(paramGroupList[index]);
}


void ParamGroup::addLogWithSignParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,minAbsValue,factor,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,minAbsValue,factor,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWithSignParamArray *ParamGroup::getLogWithSignParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWithSign");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<LogWithSignParamArray *>(paramGroupList[index]);
}

LogWithSignParamMatrix *ParamGroup::getLogWithSignParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="LogWithSign");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<LogWithSignParamMatrix *>(paramGroupList[index]);
}

void ParamGroup::addIntParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginIntType* const values,int *const size,const int maxSize,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addIntParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, PluginParameters_PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

IntParamArray *ParamGroup::getIntParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Int");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<IntParamArray *>(paramGroupList[index]);
}

IntParamMatrix *ParamGroup::getIntParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Int");
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<IntParamMatrix *>(paramGroupList[index]);
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
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch an array with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Bool");   
  /* Double check this ParamGroup. This might not be a ParamArray */
  return dynamic_cast<BoolParamArray *>(paramGroupList[index]);
}

BoolParamMatrix *ParamGroup::getBoolParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  /* You are trying to fetch a matrix with elements of another type... */
  jassert(paramGroupList[index]->getParam(0)->getType()=="Bool");   
  /* Double check this ParamGroup. This might not be a ParamMatrix */
  return dynamic_cast<BoolParamMatrix *>(paramGroupList[index]);
}