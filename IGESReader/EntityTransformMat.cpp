#include "EntityParam.h"


EntityTransformMat::EntityTransformMat() :
	EntityParam(124, "TRANSFORMATION MATRIX")
{
	R = new double[9]{1,0,0,0,1,0,0,0,1};
	T = new double[3]{0};
}

EntityTransformMat::EntityTransformMat(const EntityTransformMat & entityTrans):
	EntityParam(entityTrans)
{
	R = new double[9];// Ã²ËÆ²»ÐèÒª
	T = new double[3];
	memcpy(R, entityTrans.R, sizeof(R));
	memcpy(T, entityTrans.T, sizeof(T));
}

EntityTransformMat & EntityTransformMat::operator=(const EntityTransformMat & entityTransMat)
{
	if (this == &entityTransMat)
		return *this;

	EntityParam::operator=(entityTransMat);
	delete[] R; delete[] T;
	memcpy(R, entityTransMat.R, sizeof(R));
	memcpy(T, entityTransMat.T, sizeof(T));

	return *this;
}

EntityTransformMat::~EntityTransformMat()
{
	delete[] R;
	delete[] T;
}

void EntityTransformMat::parseIGESStringInfo(std::vector<double> entityInfoValStorage)
{
	R[0] = entityInfoValStorage[1]; R[1] = entityInfoValStorage[2]; 
	R[2] = entityInfoValStorage[3]; T[0] = entityInfoValStorage[4];

	R[3] = entityInfoValStorage[5]; R[4] = entityInfoValStorage[6];
	R[5] = entityInfoValStorage[7]; T[1] = entityInfoValStorage[8];

	R[6] = entityInfoValStorage[9]; R[7] = entityInfoValStorage[10];
	R[8] = entityInfoValStorage[11]; T[2] = entityInfoValStorage[12];

	return;
}

void EntityTransformMat::showEntityInfo()
{
}

double * EntityTransformMat::getR()
{
	return R;
}

double * EntityTransformMat::getT()
{
	return T;
}








