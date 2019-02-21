#include "EntityParam.h"




EntityBSplineCurve::EntityBSplineCurve(int u):numU(u)
{}



void EntityBSplineCurve::parseIGESStringInfo(std::vector<double> entityInfoValStorage)
{
	K = entityInfoValStorage[1]; M = entityInfoValStorage[2];

	PROP1 = entityInfoValStorage[3];
	PROP2 = entityInfoValStorage[4];
	PROP3 = entityInfoValStorage[5];
	PROP4 = entityInfoValStorage[6];

	int A = 1 + K + M;

	for (int i = 0; i <= A; i++)
		T.push_back(entityInfoValStorage[7 + i]);
	for (int i = 0; i <= K; i++)
		W.push_back(entityInfoValStorage[8 + A + i]);

	for (int i = 0; i <= K; i++)
		CtrlPntsXYZ.push_back({
				entityInfoValStorage[9 + A + K + 3 * i],
				entityInfoValStorage[10 + A + K + 3 * i],
				entityInfoValStorage[11 + A + K + 3 * i]});

	u[0] = entityInfoValStorage[12 + A + 4 * K];
	u[1] = entityInfoValStorage[13 + A + 4 * K];

	for (int i = 0; i < 3; i++)
		XYZNorm[i] = entityInfoValStorage[14 + A + 4 * K + i];

	// 下面创建NURBS曲线
	//if(isPlane()) NURBSCurve.setDim(3);
	//else 
	NURBSCurve.setDim(4);

	NURBSCurve.setNumber({ K + 1 });
	NURBSCurve.setKnots({ T });
	NURBSCurve.setOrder({ M + 1 });
	NURBSCurve.setCoefs(CtrlPntsXYZ);
	NURBSCurve.setWeight(W);

	// 计算点坐标
	// NURBSCurve.getTransposeCoefsMat();
	NURBSCurve.getEvaluatePoints({ numU - 1 });

	NURBSCurve.showNURBSParamInfo();
}





void EntityBSplineCurve::showEntityInfo()
{
	NURBSCurve.showNURBSParamInfo();
}



