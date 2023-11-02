#pragma once
#include <string>
#include "JsonObj.h"

bool isFileExist(const std::string& name);

std::string& readStringFromFile(const std::string& fileName, std::string& resieve);

JsonPrimitive* readJsonFromFile(const std::string& fileName);