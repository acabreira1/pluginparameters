#include "ParamGroups.h"

/*
Only methods who implicate a circular reference and therefore
can't be written in the header file are placed here.
*/

void ParamGroup::addStringParamArray(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addStringParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, String** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new StringParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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

void ParamGroup::addFloatParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addFloatParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new FloatParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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


void ParamGroup::addLogParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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

void ParamGroup::addLogWith0ParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWith0ParamMatrix(const int paramIndex, const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWith0ParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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


void ParamGroup::addLogWithSignParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType* const values,int *const size,const int maxSize,const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,minAbsValue,factor,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addLogWithSignParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginFloatType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const PluginParameters_PluginFloatType minValue,const PluginParameters_PluginFloatType maxValue, const PluginParameters_PluginFloatType minAbsValue, const PluginParameters_PluginFloatType factor, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new LogWithSignParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,minAbsValue,factor,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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

void ParamGroup::addIntParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType* const values,int *const size,const int maxSize,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, bool saveOnlySizedArrayFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,minValue,maxValue,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addIntParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, PluginParameters_PluginIntType** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols,const PluginParameters_PluginIntType minValue,const PluginParameters_PluginIntType maxValue, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new IntParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,minValue,maxValue,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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

void ParamGroup::addBoolParamArray(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool* const values,int *const size,const int maxSize,bool saveOnlySizedArrayFlag,bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamArray(name,automationFlag,loadSaveXmlFlag,values,size,maxSize,saveOnlySizedArrayFlag,saveOnlyNonDefaultValuesFlag));
  paramGroupsToUnallocateAtDestructor.add(paramGroup);
}

void ParamGroup::addBoolParamMatrix(const int paramIndex,const String &name, const bool automationFlag, const bool loadSaveXmlFlag, bool** const values,int *const numRows, int *const numCols,const int maxRows, const int maxCols, const bool saveOnlySizedMatrixFlag, bool saveOnlyNonDefaultValuesFlag){
  ParamGroup *paramGroup;
  addParamGroup(paramIndex,paramGroup=new BoolParamMatrix(name,automationFlag,loadSaveXmlFlag,values,numRows,numCols,maxRows,maxCols,saveOnlySizedMatrixFlag,saveOnlyNonDefaultValuesFlag));
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