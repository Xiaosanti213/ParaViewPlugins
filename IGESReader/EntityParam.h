#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <valarray>
#include <iostream>
#include "stdafx.h"

#include "NURBSParam.h"


class EntityParam
{
public:
	EntityParam(int type = 0, const std::string name = "UNINITIALIZED");// 只有声明指定默认参数

	EntityParam(const EntityParam&);

	EntityParam& operator=(const EntityParam&);

	virtual ~EntityParam() {};

	virtual void parseIGESStringInfo(std::vector<double>) = 0;

	virtual void showEntityInfo() {};

	const int getEntityType() const { return entityType; };

private:

	int entityType;
	std::string entityName;

};




// Type: 124 Transform Matrix
class EntityTransformMat : public EntityParam {

public:
	EntityTransformMat();

	// Reference:《C++ Primer Plus》Pg 516
	EntityTransformMat(const EntityTransformMat&);

	EntityTransformMat& operator=(const EntityTransformMat&);

	virtual ~EntityTransformMat();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	double* getR();

	double* getT();

private:

	double* R;
	double* T;

};




// Type: 128 Rational B-Spline Surface
class EntityBSplineSurf : public EntityParam {

public:
	EntityBSplineSurf(EntityTransformMat* entTM = new EntityTransformMat());

	//~EntityBSplineSurf();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	// TODO: refer const usage
	NURBSParam* getNURBSSurf() { return &NURBSSurf; };

	void setRotTransMat(EntityTransformMat&);

	void calTransRotCoefs(void);

private:
	//bool original;
	//bool superior;

	int K1, K2;
	int M1, M2;
	int PROP1, PROP2, PROP3;
	int PROP4, PROP5;

	// 变长使用vector 
	std::vector<double> S, T, W;
	std::vector<std::vector<double>> CtrlPointsXYZ;
	std::vector<double> u{ 0, 0 }, v{ 0, 0 };	

	double R[9], Tr[3];
	NURBSParam NURBSSurf;
};


// Type: 404 Drawing Entity
class EntityDrawing : public EntityParam {

public:
	EntityDrawing();

	virtual void parseIGESStringInfo(std::vector <double>);

	virtual void showEntityInfo();
};



// Type: 406 Property Entity
class EntityProperty : public EntityParam {

public:
	EntityProperty();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();
private:
};


// Type: 410 View Entity
class EntityView : public EntityParam {

public:
	EntityView();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

};