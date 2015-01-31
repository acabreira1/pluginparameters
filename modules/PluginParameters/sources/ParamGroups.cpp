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
  StringParamArray *pointer=dynamic_cast<StringParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

StringParamMatrix *ParamGroup::getStringParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  StringParamMatrix *pointer=dynamic_cast<StringParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

void ParamGroup::addFloatParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue,const float maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addFloatParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const float minValue,const float maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

FloatParamArray *ParamGroup::getFloatParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  FloatParamArray *pointer=dynamic_cast<FloatParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

FloatParamMatrix *ParamGroup::getFloatParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  FloatParamMatrix *pointer=dynamic_cast<FloatParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}


void ParamGroup::addLogParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue,const float maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue,const float maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogParamArray *ParamGroup::getLogParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogParamArray *pointer=dynamic_cast<LogParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

LogParamMatrix *ParamGroup::getLogParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogParamMatrix *pointer=dynamic_cast<LogParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

void ParamGroup::addLogWith0ParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue,const float maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue,const float maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWith0ParamArray *ParamGroup::getLogWith0ParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogWith0ParamArray *pointer=dynamic_cast<LogWith0ParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

LogWith0ParamMatrix *ParamGroup::getLogWith0ParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogWith0ParamMatrix *pointer=dynamic_cast<LogWith0ParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}


void ParamGroup::addLogWithSignParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float* const values,int *const size,const int maxSize,const float minValue,const float maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, float** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const float minValue,const float maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

LogWithSignParamArray *ParamGroup::getLogWithSignParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogWithSignParamArray *pointer=dynamic_cast<LogWithSignParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

LogWithSignParamMatrix *ParamGroup::getLogWithSignParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  LogWithSignParamMatrix *pointer=dynamic_cast<LogWithSignParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

void ParamGroup::addIntParamArray(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int* const values,int *const size,const int maxSize,const int minValue,const int maxValue, bool saveOnlySizedArrayFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamArray(name,registerAtHostFlag,loadSaveOptions,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addIntParamMatrix(const int paramIndex,const String &name, const bool registerAtHostFlag, const LoadSaveOptions loadSaveOptions, int** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const int minValue,const int maxValue, const bool saveOnlySizedMatrixFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamMatrix(name,registerAtHostFlag,loadSaveOptions,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

IntParamArray *ParamGroup::getIntParamArray(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  IntParamArray *pointer=dynamic_cast<IntParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

IntParamMatrix *ParamGroup::getIntParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  IntParamMatrix *pointer=dynamic_cast<IntParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
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
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  BoolParamArray *pointer=dynamic_cast<BoolParamArray *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}

BoolParamMatrix *ParamGroup::getBoolParamMatrix(const int index) const{
  /* wrong index */
  if (index<0 || index>=paramGroupList.size()) {jassertfalse; return nullptr;}
  BoolParamMatrix *pointer=dynamic_cast<BoolParamMatrix *>(paramGroupList[index]);
  if (pointer==nullptr) { jassertfalse; Logger::writeToLog(String(__FILE__)+":"+String(__LINE__)+"::"+"You are trying to fetch a ParamGroup of another kind..."); return nullptr;}
  return pointer;
}