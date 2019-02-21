#include "EntityParam.h"
#include "stdafx.h"
#include <vector>


EntityBSplineSurf::EntityBSplineSurf(EntityTransformMat* entTM, int u, int v):
	EntityParam(128, "B - NURBS SRF"),numU(u),numV(v)
{
	memcpy(R, entTM->getR(), sizeof(R));
	memcpy(Tr, entTM->getT(), sizeof(Tr));
}





void EntityBSplineSurf::parseIGESStringInfo(std::vector<double> entityInfoValStorage)
{
	// 原始数据获得参数数据
	K1 = entityInfoValStorage[1]; K2 = entityInfoValStorage[2];
	M1 = entityInfoValStorage[3]; M2 = entityInfoValStorage[4];
	PROP1 = entityInfoValStorage[5];
	PROP2 = entityInfoValStorage[6];
	PROP3 = entityInfoValStorage[7];
	PROP4 = entityInfoValStorage[8];
	PROP5 = entityInfoValStorage[9];

	int A = 1 + K1 + M1;
	int B = 1 + K2 + M2;
	int C = (1 + K1) * (1 + K2);

	for (int i = 0; i <= A; i++)
		S.push_back(entityInfoValStorage[10 + i]);
	for (int i = 0; i <= B; i++)
		T.push_back(entityInfoValStorage[11 + A + i]);
	for (int i = 0; i < C; i++)
		W.push_back(entityInfoValStorage[12 + A + B + i]);

	for (int i = 0; i < 3 * C; i += 3)
		CtrlPointsXYZ.push_back({//隐式转换
			entityInfoValStorage[12 + A + B + C + i],
			entityInfoValStorage[13 + A + B + C + i],
			entityInfoValStorage[14 + A + B + C + i]});

	u[0] = entityInfoValStorage[12 + A + B + 4 * C];
	u[1] = entityInfoValStorage[13 + A + B + 4 * C];
	v[0] = entityInfoValStorage[14 + A + B + 4 * C];
	v[1] = entityInfoValStorage[15 + A + B + 4 * C];

	// 下面创建NURBS曲面
	NURBSSurf.setDim(4);// 齐次坐标

	// 应该再看一下初始化列表语法的复制构造函数
	NURBSSurf.setNumber({ K1 + 1, K2 + 1 });
	NURBSSurf.setKnots({S, T});
	NURBSSurf.setOrder({M1 + 1, M2 + 1});
	
	// 计算旋转平移后的控制点
	calTransRotCoefs();
	NURBSSurf.setCoefs(CtrlPointsXYZ);
	NURBSSurf.setWeight(W);

	// 调试输出
	showEntityInfo();

	// 计算点坐标
	NURBSSurf.getTransposeCoefsMat();
	NURBSSurf.getEvaluatePoints({ numU - 1, numV - 1 });

}



void EntityBSplineSurf::showEntityInfo()
{
	NURBSSurf.showNURBSParamInfo();
}



void EntityBSplineSurf::setRotTransMat(EntityTransformMat & entityTransMat)
{
	memcpy(R, entityTransMat.getR(), sizeof(R));
	memcpy(Tr, entityTransMat.getT(), sizeof(Tr));
}



void EntityBSplineSurf::calTransRotCoefs(void)
{
	for (int i = 0; i < CtrlPointsXYZ.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CtrlPointsXYZ[i][j] = Tr[j] + R[3 * j] * CtrlPointsXYZ[i][0] +
				R[3 * j + 1] * CtrlPointsXYZ[i][1] + R[3 * j + 2] * CtrlPointsXYZ[i][2];
		}
	}
	return;
}



unsigned int* EntityBSplineSurf::getTriangleIndex(void)
{
	// 三角组合
	int intu = numU-1;//参数域u区间个数
	int intv = numV-1;
	int mtri = 2 * intu * intv;
	int dims[2];
	dims[0] = mtri;
	dims[1] = 3;
	unsigned int* ptri;

	ptri = new unsigned int[3 * mtri];
	int myint, myint2, myint3;

	for (int j = 0; j < intv; j++) {
		myint = j * intu;	//前j行参数区间的网格数
		myint2 = j * numU;	//前j行参数区间的节点数
		for (int i = 0; i < intu; i++) {
			myint3 = 2 * (myint + i);

			ptri[myint3] = myint2 + numU + i + 1;
			ptri[myint3 + 1] = myint2 + i + 2;

			ptri[myint3 + mtri] = myint2 + i + 2;
			ptri[myint3 + 1 + mtri] = myint2 + numU + i + 1;

			ptri[myint3 + 2 * mtri] = myint2 + i + 1;
			ptri[myint3 + 1 + 2 * mtri] = myint2 + numU + i + 2;
		}
	}

	// 注意组合的格式
	for (int i = 0; i < 10; i++)
	{
		printf("[ %d, %d, %d ]\n", ptri[i],
			ptri[i + mtri], ptri[i + 2 * mtri]);
	}

	return ptri;
}



void EntityBSplineSurf::setnumUnumV(int u, int v)
{
	numU = u;
	numV = v;
}






