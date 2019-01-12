// ConsoleApplication2.c : 定义控制台应用程序的入口点。
// 测试

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../../forTest.h"


#if !defined(MAX)
#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define	MIN(A, B)	((A) < (B) ? (A) : (B))
#endif

#define lengthKnotsU 15
#define lengthKnotsV 13

static void NURBSsurfaceRegularEval(
	int, int, double*, int, int, double*, double*, double, double, 
		unsigned int, double, double, unsigned int, double*);


struct nurbsParamSurf {
	char form[sizeof("B-NURBS")];//常量
	int dim;
	int number[2];
	double coefs[4*99];
	int knots[2][15];
	int order[2];
} nurbs;


int main()
{
	// 实参定义
	double knotsU[lengthKnotsU] = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8 };
	double knotsV[lengthKnotsV] = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 6, 6, 6 };

	memcpy(nurbs.form, "B-NURBS", sizeof("B-NURBS"));
	nurbs.dim = 4;
	nurbs.number[0] = 11; nurbs.number[1] = 9;
	memcpy(nurbs.knots[0], knotsU, sizeof(knotsU));
	memcpy(nurbs.knots[1], knotsV, sizeof(knotsV));
	nurbs.order[0] = nurbs.order[1] = 4;

	// 控制点坐标赋值 这个貌似不是在IGES File中解析的
	double ctrlPnts[] = { 
	-1.5160, -1.5160, -1.5160, -1.5088, -1.2099, -0.9730, -0.5888, -0.6746, -0.7925, -0.7925, -0.7925,
	1.7910,  1.7910,  1.7910,  1.9137,  1.2564,  0.5287,  -0.7837, -0.7406, -0.5283, -0.5283, -0.5283,
	2.4550,  2.4550,  2.4550,  3.0384,  3.8781,  3.7037,  2.8829,  2.0504,  1.5283,  1.5283,  1.5283,
	1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,

	-1.5242, -1.5242, -1.5242, -1.3538, -0.7121, -0.4343, -0.1319, -0.4788, -0.7843, -0.7843, -0.7843,
	1.7882,  1.7882,   1.7882, 1.9673,  1.4286,  0.7150,  -0.6257, -0.6728, -0.5255, -0.5255, -0.5255,
	2.4557,  2.4557,   2.4557, 3.0252,  3.8357,  3.6578,  2.8440,  2.0337,  1.5276,  1.5276,  1.5276,
	1.0000,  1.0000,   1.0000, 1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,  1.0000,

	-1.5386,   -1.5386,   -1.5386,   -1.0806,    0.1657,    0.5155,    0.6738,   -0.1333,   -0.7699,   -0.7699,   -0.7699,
	1.7792,    1.7792,    1.7792,    2.1373,    1.9747,    1.3059,   -0.1244,   -0.4579,   -0.5165,   -0.5165,   -0.5165,
	2.4663,    2.4663,    2.4663,    2.8234,    3.1872,    2.9561,    2.2487,    1.7785,    1.5170,    1.5170,    1.5170,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.5328,   -1.5328,   -1.5328,   -1.1906,   -0.1876,    0.1332,    0.3495,   -0.2724,   -0.7757,   -0.7757,   -0.7757,
	1.7693,    1.7693,    1.7693,    2.3256,    2.5798,    1.9605,    0.4310,   -0.2198,   -0.5066,   -0.5066,   -0.5066,
	2.4941,    2.4941,    2.4941,    2.2970,    1.4960,    1.1262,    0.6964,    1.1130,    1.4892,    1.4892,    1.4892,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.5054,   -1.5054,   -1.5054,   -1.7104,   -1.8576,   -1.6738,   -1.1834,   -0.9295,   -0.8032,   -0.8032,   -0.8032,
	1.7728,    1.7728,    1.7728,    2.2590,    2.3656,    1.7289,    0.2344,   -0.3041,   -0.5101,   -0.5101,   -0.5101,
	2.5058,    2.5058,    2.5058,    2.0734,    0.7776,    0.3488,    0.0370,    0.8303,    1.4774,    1.4774,    1.4774,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.4838,   -1.4838,   -1.4838,   -2.1202,   -3.1743,   -3.0985,   -2.3919,   -1.4477,   -0.8248,   -0.8248,   -0.8248,
	1.7862,    1.7862,    1.7862,    2.0040,    1.5465,    0.8425,   -0.5175,   -0.6264,   -0.5236,   -0.5236,   -0.5236,
	2.4899,    2.4899,    2.4899,    2.3761,    1.7503,    1.4014,    0.9298,    1.2131,    1.4933,    1.4933,    1.4933,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.4896,   -1.4896,   -1.4896,   -2.0103,   -2.8210,   -2.7163,   -2.0677,   -1.3086,   -0.8190,   -0.8190,   -0.8190,
	1.7962,    1.7962,    1.7962,    1.8157,    0.9414,    0.1879,   -1.0728,   -0.8645,   -0.5335,   -0.5335,   -0.5335,
	2.4622,    2.4622,    2.4622,    2.9025,    3.4415,    3.2313,    2.4821,    1.8786,    1.5211,    1.5211,    1.5211,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.5078,   -1.5078,   -1.5078,   -1.6637,   -1.7077,   -1.5116,   -1.0457,   -0.8705,   -0.8007,   -0.8007,   -0.8007,
	1.7938,    1.7938,    1.7938,    1.8601,    1.0842,    0.3423,   -0.9418,   -0.8083,   -0.5312,   -0.5312,   -0.5312,
	2.4543,    2.4543,    2.4543,    3.0516,    3.9205,    3.7495,    2.9218,    2.0671,    1.5290,    1.5290,    1.5290,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,

	-1.5160,   -1.5160,   -1.5160,   -1.5088,   -1.2099,   -0.9730,   -0.5888,   -0.6746,   -0.7925,   -0.7925,   -0.7925,
	1.7910,    1.7910,    1.7910,    1.9137,    1.2564,    0.5287,   -0.7837,   -0.7406,   -0.5283,   -0.5283,   -0.5283,
	2.4550,    2.4550,    2.4550,    3.0384,    3.8781,    3.7037,    2.8829,    2.0504,    1.5283,    1.5283,    1.5283,
	1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000,    1.0000 };
	
	memcpy(nurbs.coefs, ctrlPnts, sizeof(ctrlPnts));
	// 循环coefs转换成控制点四维坐标连续形式
	for (int i = 0; i < nurbs.number[1]; i++)
		for (int j = 0; j < nurbs.dim; j++)
			for (int k = 0; k < nurbs.number[0]; k++)
				nurbs.coefs[i * nurbs.dim * nurbs.number[0] + k * nurbs.dim + j] =
					ctrlPnts[i * nurbs.dim * nurbs.number[0] + j * nurbs.number[0] + k];


	// NURBSsurfaceRegularEval函数参数含义
	/*
		orderU: U方向阶数
		orderV: V方向阶数
		cp:		控制点坐标
		ncp:	U方向控制点个数
		kcp:	V方向控制点个数
		knotU:  U方向节点向量
		knotV:  V方向节点向量
		u0:		U方向节点起点
		deltau: U方向evalPnts间隔
		numu:	U方向evalPnts点的个数
		v0:		V方向节点起点
		deltav: V方向evalPnts间隔
		numv:   V方向evalPnts点的个数
		ep:     evalPnts点坐标
	*/

	const int evalPntsNum = 40000;
	double evalPnts[40000*3] = {0};

	int orderU = nurbs.order[0], orderV = nurbs.order[1];
	
	double u0 = knotsU[0 + orderU - 1], v0 = knotsV[0 + orderV - 1];
	int numu = 200; int numv = 200;
	double deltau = (knotsU[14 - orderU + 1] - u0) / (numu - 1);
	double deltav = (knotsV[12 - orderV + 1] - v0) / (numv - 1);

	// 计算evalPnts
	NURBSsurfaceRegularEval(orderU, orderV, nurbs.coefs,
		nurbs.number[0], nurbs.number[1], knotsU, knotsV, 
			u0, deltau, numu, v0, deltav, numv, evalPnts);


	// 三角组合
	int intu = numu - 1;//参数域区间个数
	int intv = numv - 1;
	int mtri = 2 * intu * intv;
	int dims[2];
	dims[0] = mtri;
	dims[1] = 3;
	unsigned int* ptri;

	ptri = malloc(3 * mtri * sizeof(unsigned int));
	int myint, myint2, myint3;

	for (int j = 0; j < intv; j++) {
		myint = j * intu;	//前j行参数区间的网格数
		myint2 = j * numu;	//前j行参数区间的节点数
		for (int i = 0; i < intu; i++) {
			myint3 = 2 * (myint + i);

			ptri[myint3] = myint2 + numu + i + 1;
			ptri[myint3 + 1] = myint2 + i + 2;

			ptri[myint3 + mtri] = myint2 + i + 2;
			ptri[myint3 + 1 + mtri] = myint2 + numu + i + 1;

			ptri[myint3 + 2 * mtri] = myint2 + i + 1;
			ptri[myint3 + 1 + 2 * mtri] = myint2 + numu + i + 2;
		}
	}
	
	// 注意组合的格式
	for (int i = 0; i < 10; i++)
	{
		printf("[ %d, %d, %d ]\n", ptri[i], 
					ptri[i + mtri], ptri[i + 2 * mtri]);
	}




	return 0;
}




static void NURBSsurfaceRegularEval(int orderU, int orderV, double *cp, int ncp, int kcp, double *knotU, double *knotV, double u0, double deltau, unsigned int numu, double v0, double deltav, unsigned int numv, double *ep) {

	/* Evaluates a NURBS surface at given regular parameter values */

	/* NURBSsurfaceRegularEval( orderU - order of NURBS in u, orderV - order of NURBS in v, cp - pointer to control points, ncp - number of control points in u, kcp - number of control points in v, knotU - knot sequence in u, knotV - knot sequence in v, u0 - first u, deltau - spacing in u, numu - number of u, v0 - first v, deltav - spacing in v, numv - number of v, ep - pointer to evaluated points ) */

	unsigned short int i, j, ii, jj, indi, indj, indi2, indj2, myint, myint2;
	unsigned short int numuMinusOne, numvMinusOne, ncpMinusOne, kcpMinusOne, fourncp, fourncpMinusFour;
	unsigned short int degU, degV, *spanArray, sdU, spanScalar, sdV;
	unsigned int epind, ind;
	double *left, *right, *N, *Npoly;
	double utmp, vtmp, wgh, NN;
	char uBounds, vBounds;

	numuMinusOne = numu - 1;
	numvMinusOne = numv - 1;

	ncpMinusOne = ncp - 1;
	kcpMinusOne = kcp - 1;

	degU = orderU - 1;
	degV = orderV - 1;

	if (u0<(knotU[degU] + 1e-12) && (u0 + numuMinusOne*deltau)>(knotU[ncp] - 1e-12)) {//u0<uk&&u(n+k+1)>u(n+1)
		uBounds = 1;
	}
	else {
		uBounds = 0;
	}

	if (v0<(knotV[degV] + 1e-12) && (v0 + numvMinusOne*deltav)>(knotV[kcp] - 1e-12)) {
		vBounds = 1;
	}
	else {
		vBounds = 0;
	}

	myint = MAX(orderU, orderV);
	left = (double*)malloc(myint * sizeof(double) + 1);//malloc用于分配内存：myint个double单元+1（返回分配的内存区域指针）
	right = (double*)malloc(myint * sizeof(double) + 1);

	fourncp = 4 * ncp;
	fourncpMinusFour = fourncp - 4;

	if ((numu*orderU * sizeof(double) + numu * sizeof(int) + orderV * sizeof(double))<(numv*orderV * sizeof(double) + numv * sizeof(int) + orderU * sizeof(double))) {

		N = (double*)malloc(orderV * sizeof(double) + 1);

		spanArray = (unsigned short *)malloc(numu * sizeof(unsigned short) + 1);//u向节点序列内存指针
		Npoly = (double*)malloc(numu*orderU * sizeof(double) + 1);//基函数内存？每一个u值对应k+1个递推基函数？


		if (uBounds && vBounds) {

			spanArray[0] = degU;//次数（参数域下标初始化？）u的起始下标,从uk开始？？
			sdU = 0;

			ep[0] = cp[0] / cp[3];//第一个控制点的笛卡尔坐标
			ep[1] = cp[1] / cp[3];
			ep[2] = cp[2] / cp[3];

			utmp = u0;

			for (ii = 1; ii < numuMinusOne; ii++) {//ii从1取到n+k???去掉首末端点的参数域？

				epind = 3 * ii;
				indi2 = ii*orderU;

				utmp += deltau;
				spanArray[ii] = FindSpanIncSeq(ncp, utmp, knotU, spanArray[ii - 1]);//knot为u向节点序列的指针
																					//递推得到第i个u的下标（二分法）？？？？为什么spanArray为short型而非int型？？？？？
				BasisFuns(spanArray[ii], utmp, degU, knotU, &Npoly[indi2], left, right);
				//返回基函数N
				spanArray[ii - 1] = sdU;
				sdU = spanArray[ii] - degU;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (sdU + j);
					indj2 = indi2 + j;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numu - 2] = sdU;

			epind = 3 * numuMinusOne;

			indj = fourncp - 1;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

			spanScalar = degV;
			vtmp = v0;

			for (jj = 1; jj < numvMinusOne; jj++) {

				vtmp += deltav;
				spanScalar = FindSpanIncSeq(kcp, vtmp, knotV, spanScalar);
				BasisFuns(spanScalar, vtmp, degV, knotV, N, left, right);
				sdV = spanScalar - degV;

				ind = jj*numu;
				epind = 3 * ind;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*(sdV + i);
					ep[epind] += N[i] * cp[indj];
					ep[epind + 1] += N[i] * cp[indj + 1];
					ep[epind + 2] += N[i] * cp[indj + 2];
					wgh += N[i] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

				myint2 = sdV*ncp;

				for (ii = 1; ii < numuMinusOne; ii++) {

					epind = 3 * (ii + ind);
					indi2 = ii*orderU;
					myint = myint2 + spanArray[ii];

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = N[i] * Npoly[indi2 + j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

				epind = 3 * (numuMinusOne + ind);
				indi = fourncp*sdV + fourncpMinusFour;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*i + indi;
					ep[epind] += N[i] * cp[indj];
					ep[epind + 1] += N[i] * cp[indj + 1];
					ep[epind + 2] += N[i] * cp[indj + 2];
					wgh += N[i] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

			ind = numvMinusOne*numu;
			epind = 3 * ind;
			indj = kcpMinusOne*fourncp + 3;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

			myint = kcpMinusOne*ncp;

			for (ii = 1; ii < numuMinusOne; ii++) {

				epind = 3 * (ii + ind);
				indi2 = ii*orderU;

				wgh = 0.0;
				indi = myint + spanArray[ii];
				for (j = 0; j <= degU; j++) {
					indj = 4 * (indi + j);
					indj2 = indi2 + j;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

			epind = 3 * (numu*numv - 1);
			indj = fourncp*kcp - 1;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

		}
		else if (vBounds) {

			spanArray[0] = FindSpan(ncp, degU, u0, knotU);
			BasisFuns(spanArray[0], u0, degU, knotU, Npoly, left, right);
			sdU = spanArray[0] - degU;

			wgh = 0.0;
			for (j = 0; j <= degU; j++) {
				indj = 4 * (sdU + j);
				ep[0] += Npoly[j] * cp[indj];
				ep[1] += Npoly[j] * cp[indj + 1];
				ep[2] += Npoly[j] * cp[indj + 2];
				wgh += Npoly[j] * cp[indj + 3];
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			utmp = u0;

			for (ii = 1; ii < numu; ii++) {

				epind = 3 * ii;
				indi2 = ii*orderU;

				utmp += deltau;
				spanArray[ii] = FindSpanIncSeq(ncp, utmp, knotU, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], utmp, degU, knotU, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdU;
				sdU = spanArray[ii] - degU;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (sdU + j);
					indj2 = indi2 + j;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numuMinusOne] = sdU;

			spanScalar = degV;
			vtmp = v0;

			for (jj = 1; jj < numvMinusOne; jj++) {

				vtmp += deltav;
				spanScalar = FindSpanIncSeq(kcp, vtmp, knotV, spanScalar);
				BasisFuns(spanScalar, vtmp, degV, knotV, N, left, right);
				sdV = spanScalar - degV;
				ind = jj*numu;

				myint2 = sdV*ncp;

				for (ii = 0; ii < numu; ii++) {

					epind = 3 * (ii + ind);
					indi2 = ii*orderU;
					myint = myint2 + spanArray[ii];

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = N[i] * Npoly[indi2 + j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

			}

			ind = numvMinusOne*numu;
			myint = kcpMinusOne*ncp;

			for (ii = 0; ii < numu; ii++) {

				epind = 3 * (ii + ind);
				indi2 = ii*orderU;

				wgh = 0.0;
				indi = myint + spanArray[ii];
				for (j = 0; j <= degU; j++) {
					indj = 4 * (indi + j);
					indj2 = indi2 + j;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

		}
		else if (uBounds) {

			spanArray[0] = degU;
			sdU = 0;

			spanScalar = FindSpan(kcp, degV, v0, knotV);
			BasisFuns(spanScalar, v0, degV, knotV, N, left, right);
			sdV = spanScalar - degV;

			myint2 = sdV*ncp;

			wgh = 0.0;
			for (i = 0; i <= degV; i++) {
				indj = fourncp*(sdV + i);
				ep[0] += N[i] * cp[indj];
				ep[1] += N[i] * cp[indj + 1];
				ep[2] += N[i] * cp[indj + 2];
				wgh += N[i] * cp[indj + 3];
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			utmp = u0;

			for (ii = 1; ii < numuMinusOne; ii++) {

				epind = 3 * ii;
				indi2 = ii*orderU;

				utmp += deltau;
				spanArray[ii] = FindSpanIncSeq(ncp, utmp, knotU, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], utmp, degU, knotU, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdU;
				sdU = spanArray[ii] - degU;

				myint = myint2 + sdU;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indi = ncp*i + myint;
					for (j = 0; j <= degU; j++) {
						indj = 4 * (indi + j);
						NN = N[i] * Npoly[indi2 + j];
						ep[epind] += NN * cp[indj];
						ep[epind + 1] += NN * cp[indj + 1];
						ep[epind + 2] += NN * cp[indj + 2];
						wgh += NN * cp[indj + 3];
					}
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numu - 2] = sdU;

			epind = 3 * numuMinusOne;
			indi = fourncp*sdV + fourncpMinusFour;

			wgh = 0.0;
			for (i = 0; i <= degV; i++) {
				indj = fourncp*i + indi;
				ep[epind] += N[i] * cp[indj];
				ep[epind + 1] += N[i] * cp[indj + 1];
				ep[epind + 2] += N[i] * cp[indj + 2];
				wgh += N[i] * cp[indj + 3];
			}
			ep[epind] = ep[epind] / wgh;
			ep[epind + 1] = ep[epind + 1] / wgh;
			ep[epind + 2] = ep[epind + 2] / wgh;

			vtmp = v0;

			for (jj = 1; jj < numv; jj++) {

				vtmp += deltav;
				spanScalar = FindSpanIncSeq(kcp, vtmp, knotV, spanScalar);
				BasisFuns(spanScalar, vtmp, degV, knotV, N, left, right);
				sdV = spanScalar - degV;
				ind = jj*numu;

				epind = 3 * ind;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*(sdV + i);
					ep[epind] += N[i] * cp[indj];
					ep[epind + 1] += N[i] * cp[indj + 1];
					ep[epind + 2] += N[i] * cp[indj + 2];
					wgh += N[i] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

				myint2 = sdV*ncp;

				for (ii = 1; ii < numuMinusOne; ii++) {

					epind = 3 * (ii + ind);
					indi2 = ii*orderU;
					myint = myint2 + spanArray[ii];

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = N[i] * Npoly[indi2 + j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

				epind = 3 * (numuMinusOne + ind);
				indi = fourncp*sdV + fourncpMinusFour;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*i + indi;
					ep[epind] += N[i] * cp[indj];
					ep[epind + 1] += N[i] * cp[indj + 1];
					ep[epind + 2] += N[i] * cp[indj + 2];
					wgh += N[i] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

		}
		else {

			spanArray[0] = FindSpan(ncp, degU, u0, knotU);
			BasisFuns(spanArray[0], u0, degU, knotU, Npoly, left, right);
			sdU = spanArray[0] - degU;

			spanScalar = FindSpan(kcp, degV, v0, knotV);
			BasisFuns(spanScalar, v0, degV, knotV, N, left, right);
			sdV = spanScalar - degV;

			myint2 = sdV*ncp;
			myint = myint2 + sdU;

			wgh = 0.0;
			for (i = 0; i <= degV; i++) {
				indi = ncp*i + myint;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (indi + j);
					NN = N[i] * Npoly[j];
					ep[0] += NN * cp[indj];
					ep[1] += NN * cp[indj + 1];
					ep[2] += NN * cp[indj + 2];
					wgh += NN * cp[indj + 3];
				}
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			utmp = u0;

			for (ii = 1; ii < numu; ii++) {

				epind = 3 * ii;
				indi2 = ii*orderU;

				utmp += deltau;
				spanArray[ii] = FindSpanIncSeq(ncp, utmp, knotU, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], utmp, degU, knotU, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdU;
				sdU = spanArray[ii] - degU;

				myint = myint2 + sdU;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indi = ncp*i + myint;
					for (j = 0; j <= degU; j++) {
						indj = 4 * (indi + j);
						NN = N[i] * Npoly[indi2 + j];
						ep[epind] += NN * cp[indj];
						ep[epind + 1] += NN * cp[indj + 1];
						ep[epind + 2] += NN * cp[indj + 2];
						wgh += NN * cp[indj + 3];
					}
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numuMinusOne] = sdU;

			vtmp = v0;

			for (jj = 1; jj < numv; jj++) {

				vtmp += deltav;
				spanScalar = FindSpanIncSeq(kcp, vtmp, knotV, spanScalar);
				BasisFuns(spanScalar, vtmp, degV, knotV, N, left, right);
				sdV = spanScalar - degV;
				ind = jj*numu;

				myint2 = sdV*ncp;

				for (ii = 0; ii < numu; ii++) {

					epind = 3 * (ii + ind);
					indi2 = ii*orderU;
					myint = myint2 + spanArray[ii];

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = N[i] * Npoly[indi2 + j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

			}

		}

	}
	else {

		N = (double*)malloc(orderU * sizeof(double) + 1);

		spanArray = (unsigned short *)malloc(numv * sizeof(unsigned short) + 1);
		Npoly = (double*)malloc(numv*orderV * sizeof(double) + 1);

		myint2 = 3 * numu;

		if (uBounds && vBounds) {

			spanArray[0] = degV;
			sdV = 0;

			ep[0] = cp[0] / cp[3];
			ep[1] = cp[1] / cp[3];
			ep[2] = cp[2] / cp[3];

			vtmp = v0;

			for (ii = 1; ii < numvMinusOne; ii++) {

				epind = ii*myint2;
				indi2 = ii*orderV;

				vtmp += deltav;
				spanArray[ii] = FindSpanIncSeq(kcp, vtmp, knotV, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], vtmp, degV, knotV, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdV;
				sdV = spanArray[ii] - degV;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*(sdV + i);
					indj2 = indi2 + i;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numv - 2] = sdV;

			epind = numvMinusOne*myint2;

			indj = fourncp*kcpMinusOne + 3;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

			spanScalar = degU;
			utmp = u0;

			for (jj = 1; jj < numuMinusOne; jj++) {

				utmp += deltau;
				spanScalar = FindSpanIncSeq(ncp, utmp, knotU, spanScalar);
				BasisFuns(spanScalar, utmp, degU, knotU, N, left, right);
				sdU = spanScalar - degU;

				ind = 3 * jj;
				epind = ind;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (sdU + j);
					ep[epind] += N[j] * cp[indj];
					ep[epind + 1] += N[j] * cp[indj + 1];
					ep[epind + 2] += N[j] * cp[indj + 2];
					wgh += N[j] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

				for (ii = 1; ii < numvMinusOne; ii++) {

					epind = myint2*ii + ind;
					indi2 = ii*orderV;
					myint = sdU + spanArray[ii] * ncp;

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						indj2 = indi2 + i;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							ep[epind] += Npoly[indj2] * N[j] * cp[indj];
							ep[epind + 1] += Npoly[indj2] * N[j] * cp[indj + 1];
							ep[epind + 2] += Npoly[indj2] * N[j] * cp[indj + 2];
							wgh += Npoly[indj2] * N[j] * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

				epind = myint2*numvMinusOne + ind;

				myint = sdU + kcpMinusOne*ncp;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (j + myint);
					ep[epind] += N[j] * cp[indj];
					ep[epind + 1] += N[j] * cp[indj + 1];
					ep[epind + 2] += N[j] * cp[indj + 2];
					wgh += N[j] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

			ind = 3 * numuMinusOne;
			epind = ind;
			indj = fourncp - 1;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

			// 上面是沿着两个方向计算基函数和控制点位置
			// 下面是求加权和
			for (ii = 1; ii < numvMinusOne; ii++) {

				epind = myint2*ii + ind;
				indi2 = ii*orderV;

				wgh = 0.0;
				indi = fourncp*spanArray[ii] + fourncpMinusFour;
				for (i = 0; i <= degV; i++) {
					indj = fourncp*i + indi;
					indj2 = indi2 + i;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

			epind = 3 * (numu*numv - 1);
			indj = fourncp*kcp - 1;
			ep[epind] = cp[indj - 3] / cp[indj];
			ep[epind + 1] = cp[indj - 2] / cp[indj];
			ep[epind + 2] = cp[indj - 1] / cp[indj];

		}
		else if (vBounds) {

			spanArray[0] = degV;
			sdV = 0;

			spanScalar = FindSpan(ncp, degU, u0, knotU);
			BasisFuns(spanScalar, u0, degU, knotU, N, left, right);
			sdU = spanScalar - degU;

			wgh = 0.0;
			for (j = 0; j <= degU; j++) {
				indj = 4 * (sdU + j);
				ep[0] += N[j] * cp[indj];
				ep[1] += N[j] * cp[indj + 1];
				ep[2] += N[j] * cp[indj + 2];
				wgh += N[j] * cp[indj + 3];
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			vtmp = v0;

			for (ii = 1; ii < numvMinusOne; ii++) {

				epind = ii*myint2;
				indi2 = ii*orderV;

				vtmp += deltav;
				spanArray[ii] = FindSpanIncSeq(kcp, vtmp, knotV, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], vtmp, degV, knotV, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdV;
				sdV = spanArray[ii] - degV;

				myint = sdV*ncp + sdU;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indi = ncp*i + myint;
					indj2 = indi2 + i;
					for (j = 0; j <= degU; j++) {
						indj = 4 * (indi + j);
						NN = Npoly[indj2] * N[j];
						ep[epind] += NN * cp[indj];
						ep[epind + 1] += NN * cp[indj + 1];
						ep[epind + 2] += NN * cp[indj + 2];
						wgh += NN * cp[indj + 3];
					}
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numv - 2] = sdV;

			epind = numvMinusOne*myint2;
			indi = ncp*kcpMinusOne + sdU;

			wgh = 0.0;
			for (j = 0; j <= degU; j++) {
				indj = 4 * (indi + j);
				ep[epind] += N[j] * cp[indj];
				ep[epind + 1] += N[j] * cp[indj + 1];
				ep[epind + 2] += N[j] * cp[indj + 2];
				wgh += N[j] * cp[indj + 3];
			}
			ep[epind] = ep[epind] / wgh;
			ep[epind + 1] = ep[epind + 1] / wgh;
			ep[epind + 2] = ep[epind + 2] / wgh;

			utmp = u0;

			for (jj = 1; jj < numu; jj++) {

				utmp += deltau;
				spanScalar = FindSpanIncSeq(ncp, utmp, knotU, spanScalar);
				BasisFuns(spanScalar, utmp, degU, knotU, N, left, right);
				sdU = spanScalar - degU;

				ind = 3 * jj;

				epind = ind;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (sdU + j);
					ep[epind] += N[j] * cp[indj];
					ep[epind + 1] += N[j] * cp[indj + 1];
					ep[epind + 2] += N[j] * cp[indj + 2];
					wgh += N[j] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

				for (ii = 1; ii < numvMinusOne; ii++) {

					epind = myint2*ii + ind;
					indi2 = ii*orderV;
					myint = sdU + spanArray[ii] * ncp;

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						indj2 = indi2 + i;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = Npoly[indj2] * N[j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

				epind = myint2*numvMinusOne + ind;
				indi = ncp*kcpMinusOne + sdU;

				wgh = 0.0;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (indi + j);
					ep[epind] += N[j] * cp[indj];
					ep[epind + 1] += N[j] * cp[indj + 1];
					ep[epind + 2] += N[j] * cp[indj + 2];
					wgh += N[j] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

		}
		else if (uBounds) {

			spanArray[0] = FindSpan(kcp, degV, v0, knotV);
			BasisFuns(spanArray[0], v0, degV, knotV, Npoly, left, right);
			sdV = spanArray[0] - degV;

			wgh = 0.0;
			for (i = 0; i <= degV; i++) {
				indj = fourncp*(sdV + i);
				ep[0] += Npoly[i] * cp[indj];
				ep[1] += Npoly[i] * cp[indj + 1];
				ep[2] += Npoly[i] * cp[indj + 2];
				wgh += Npoly[i] * cp[indj + 3];
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			vtmp = v0;

			for (ii = 1; ii < numv; ii++) {

				epind = ii*myint2;
				indi2 = ii*orderV;

				vtmp += deltav;
				spanArray[ii] = FindSpanIncSeq(kcp, vtmp, knotV, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], vtmp, degV, knotV, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdV;
				sdV = spanArray[ii] - degV;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj2 = indi2 + i;
					indj = fourncp*(sdV + i);
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numvMinusOne] = sdV;

			spanScalar = degU;
			utmp = u0;

			for (jj = 1; jj < numuMinusOne; jj++) {

				utmp += deltau;
				spanScalar = FindSpanIncSeq(ncp, utmp, knotU, spanScalar);
				BasisFuns(spanScalar, utmp, degU, knotU, N, left, right);
				sdU = spanScalar - degU;

				ind = 3 * jj;

				for (ii = 0; ii < numv; ii++) {

					epind = myint2*ii + ind;
					indi2 = ii*orderV;
					myint = sdU + spanArray[ii] * ncp;

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						indj2 = indi2 + i;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = Npoly[indj2] * N[j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

			}

			ind = 3 * numuMinusOne;

			for (ii = 0; ii < numv; ii++) {

				epind = myint2*ii + ind;
				indi2 = ii*orderV;

				indi = fourncp*spanArray[ii] + fourncpMinusFour;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indj2 = indi2 + i;
					indj = fourncp*i + indi;
					ep[epind] += Npoly[indj2] * cp[indj];
					ep[epind + 1] += Npoly[indj2] * cp[indj + 1];
					ep[epind + 2] += Npoly[indj2] * cp[indj + 2];
					wgh += Npoly[indj2] * cp[indj + 3];
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}

		}
		else {

			spanArray[0] = FindSpan(kcp, degV, v0, knotV);
			BasisFuns(spanArray[0], v0, degV, knotV, Npoly, left, right);
			sdV = spanArray[0] - degV;

			spanScalar = FindSpan(ncp, degU, u0, knotU);
			BasisFuns(spanScalar, u0, degU, knotU, N, left, right);
			sdU = spanScalar - degU;

			myint = sdV*ncp + sdU;

			wgh = 0.0;
			for (i = 0; i <= degV; i++) {
				indi = ncp*i + myint;
				for (j = 0; j <= degU; j++) {
					indj = 4 * (indi + j);
					NN = Npoly[i] * N[j];
					ep[0] += NN * cp[indj];
					ep[1] += NN * cp[indj + 1];
					ep[2] += NN * cp[indj + 2];
					wgh += NN * cp[indj + 3];
				}
			}
			ep[0] = ep[0] / wgh;
			ep[1] = ep[1] / wgh;
			ep[2] = ep[2] / wgh;

			vtmp = v0;

			for (ii = 1; ii < numv; ii++) {

				epind = ii*myint2;
				indi2 = ii*orderV;

				vtmp += deltav;
				spanArray[ii] = FindSpanIncSeq(kcp, vtmp, knotV, spanArray[ii - 1]);
				BasisFuns(spanArray[ii], vtmp, degV, knotV, &Npoly[indi2], left, right);

				spanArray[ii - 1] = sdV;
				sdV = spanArray[ii] - degV;

				myint = sdV*ncp + sdU;

				wgh = 0.0;
				for (i = 0; i <= degV; i++) {
					indi = ncp*i + myint;
					indj2 = indi2 + i;
					for (j = 0; j <= degU; j++) {
						indj = 4 * (indi + j);
						NN = Npoly[indj2] * N[j];
						ep[epind] += NN * cp[indj];
						ep[epind + 1] += NN * cp[indj + 1];
						ep[epind + 2] += NN * cp[indj + 2];
						wgh += NN * cp[indj + 3];
					}
				}
				ep[epind] = ep[epind] / wgh;
				ep[epind + 1] = ep[epind + 1] / wgh;
				ep[epind + 2] = ep[epind + 2] / wgh;

			}
			spanArray[numvMinusOne] = sdV;

			utmp = u0;

			for (jj = 1; jj < numu; jj++) {

				utmp += deltau;
				spanScalar = FindSpanIncSeq(ncp, utmp, knotU, spanScalar);
				BasisFuns(spanScalar, utmp, degU, knotU, N, left, right);
				sdU = spanScalar - degU;

				ind = 3 * jj;

				for (ii = 0; ii < numv; ii++) {

					epind = myint2*ii + ind;
					indi2 = ii*orderV;
					myint = sdU + spanArray[ii] * ncp;

					wgh = 0.0;
					for (i = 0; i <= degV; i++) {
						indi = ncp*i + myint;
						indj2 = indi2 + i;
						for (j = 0; j <= degU; j++) {
							indj = 4 * (indi + j);
							NN = Npoly[indj2] * N[j];
							ep[epind] += NN * cp[indj];
							ep[epind + 1] += NN * cp[indj + 1];
							ep[epind + 2] += NN * cp[indj + 2];
							wgh += NN * cp[indj + 3];
						}
					}
					ep[epind] = ep[epind] / wgh;
					ep[epind + 1] = ep[epind + 1] / wgh;
					ep[epind + 2] = ep[epind + 2] / wgh;

				}

			}

		}

	}

	free(spanArray);//释放内存，与malloc对应
	free(Npoly);

	free(left);
	free(right);
	free(N);

}