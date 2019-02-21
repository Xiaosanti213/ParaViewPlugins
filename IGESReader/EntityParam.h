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



// Type: 102 Composite Curve
class EntityCompositeCurve : public EntityParam {

public:
	EntityCompositeCurve();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual ~EntityCompositeCurve();
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





// Type: 126 Rational B-Spline Curve
class EntityBSplineCurve : public EntityParam {

public:
	EntityBSplineCurve(int u = 40);

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	bool isPlane() { return PROP1 == 1; };// 0: 非平面 1: 平面
	bool isClose() { return PROP2 == 1; };// 0: 开 1: 闭
	bool isPoly() { return PROP3 == 1; }; // 0: 有理 1: 多项式
	bool isPerio() { return PROP4 == 1; };// 0：非周期 1: 周期的 

	NURBSParam* getNURBSCurve() { return &NURBSCurve; };

private:
	int K, M;
	int PROP1, PROP2, 
		PROP3, PROP4;

	std::vector<double> T, W;
	std::vector<std::vector<double>> CtrlPntsXYZ;
	std::vector<double> u{ 0, 0 };

	double XYZNorm[3];

	NURBSParam NURBSCurve;

	// 参数域点个数
	int numU;
};







// Type: 128 Rational B-Spline Surface
class EntityBSplineSurf : public EntityParam {

public:
	EntityBSplineSurf(EntityTransformMat* entTM = new EntityTransformMat(),
		int u = 0, int v = 0);

	//~EntityBSplineSurf();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	// TODO: refer const usage
	NURBSParam* getNURBSSurf() { return &NURBSSurf; };

	void setRotTransMat(EntityTransformMat&);

	void calTransRotCoefs(void);

	unsigned int* getTriangleIndex(void);

	void setnumUnumV(int, int);
	void getnumUnumV(int* get) { get[0] = numU; get[1] = numV; }

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

	// 参数域的点的个数
	int numU, numV;
};




// Type: 142 Curve on Parametric Surface
class EntityCurveOnParamSurf : public EntityParam
{
public:
	EntityCurveOnParamSurf();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	virtual ~EntityCurveOnParamSurf();

};


// Type: 144 Clipping parametric surface 
class EntityClipParamSurf : public EntityParam
{
public:
	EntityClipParamSurf();

	virtual void parseIGESStringInfo(std::vector<double>);

	virtual void showEntityInfo();

	virtual ~EntityClipParamSurf();
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