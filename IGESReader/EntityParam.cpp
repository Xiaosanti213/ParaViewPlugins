#include "stdafx.h"
#include "EntityParam.h"
#include <string>
#include <vector>
#include <sstream>


EntityParam::EntityParam(int type, const std::string name):
entityType(type), entityName(name)
{
}

EntityParam::EntityParam(const EntityParam &)
{
}

EntityParam & EntityParam::operator=(const EntityParam & entityParam)
{
	entityType = entityParam.entityType;
	entityName = entityParam.entityName;
	return *this;
}




void EntityParam::parseIGESStringInfo(std::vector<double> entityInfoValStorage)
{
}




