#include "stdafx.h"
#include "NURBSParam.h"
#include "EntityParam.h"

typedef std::vector<std::vector<double>> vec_vec_dbl;


NURBSParam::NURBSParam(std::string NURBSForm, int NURBSDim, 
	std::vector<int> NURBSNumber, 
		std::vector<std::vector<double>> NURBSCoefs, 
			std::vector<std::vector<double>> NURBSKnots,
				std::vector<int> NURBSOrder):
form(NURBSForm), dim(NURBSDim), number(NURBSNumber),
coefs(NURBSCoefs), knots(NURBSKnots), order(NURBSOrder)
{
	// TODO: ���ά�������ȶ�Ӧ��ϵ�Ƿ���ȷ
}

NURBSParam::~NURBSParam()
{
}

void NURBSParam::showNURBSParamInfo()
{
	using namespace std;

	cout.width(8);
	cout << right << "form: " << left << "B-NURBS\n";

	cout.width(8);
	cout << right << "dim: " << left << dim << endl;

	cout.width(8);
	cout << right << "number: " << left << "[ ";

	for (int i = 0; i < number.capacity(); i++)
	{
		cout << number[i] << " ";
		if (i == number.capacity() - 1) cout << "]\n";
	}

	cout.width(8);
	cout << right << "coefs:\n";
	
	for (int i = 0; i < (knots.size() == 2 ? number[1] : 1); i++)
	{
		for (int j = 0; j < number[0]; j++)
			for (int k = 0; k < dim; k++)
			{
				if (k == 0) cout << " [ ";
				cout << coefs[i * (knots.size() == 2 ? number[0] : 0) + j][k] << " ";
				if (k == dim - 1)
				{
					cout << "]";
					if (j % 2 == 1) cout << endl;
					else if (j == number[0] - 1) cout << "\n";
				}
			}
	}

	cout.width(8);
	cout << right << "knots:\n";

	for (int i = 0; i < knots.size(); i++)
		for (int j =0; j < knots[i].size(); j++)
		{
			if (j == 0) cout << " [ ";
			cout << knots[i][j] << " ";
			if (j == knots[i].size() - 1)
				cout << "]\n";
		}

	cout.width(8);
	cout << right << "order: " << left << "[ ";

	for (int i = 0; i < order.capacity(); i++)
	{
		cout << order[i] << " ";
		if (i == order.capacity() - 1) cout << "]\n";
	}

}



// һ�����ú���
void NURBSParam::setDim(int NURBSDim)
{
	dim = NURBSDim;
}

void NURBSParam::setNumber(std::vector<int> NURBSNumber)
{
	number = NURBSNumber;
}

void NURBSParam::setCoefs(std::vector<std::vector<double>>NURBSCoefs)
{
	coefs = NURBSCoefs;
}

void NURBSParam::setWeight(std::vector<double> NURBSCoefsWeight)
{
	for (int i = 0; i < NURBSCoefsWeight.capacity(); i++)
		coefs[i].push_back(NURBSCoefsWeight[i]);
}

void NURBSParam::setKnots(std::vector<std::vector<double>>NURBSKnots)
{
	knots = NURBSKnots;
}

void NURBSParam::setOrder(std::vector <int> NURBSOrder)
{
	order = NURBSOrder;
}





// ����ת�� ������ת���Ƶ����� ����˳��
void NURBSParam::getTransposeCoefsMat()
{
	std::vector<std::vector<double>> transCoefs(coefs);
	for (int i = 0; i < number[1]; i++)
		for (int j = 0; j < number[0]; j++)
			for (int k = 0; k < dim; k++)
				transCoefs[j * number[1] + i][k] = 
								coefs[i * number[0] + j][k];
	coefs = transCoefs;
}





// ������ӳ�䵽������
void NURBSParam::getEvaluatePoints(std::vector<int> numOfEvalPnts)
{
	std::vector<std::vector<int>> evalPntsInd;
	std::vector<vec_vec_dbl> basisFuncsVal;
	int w = 0;  

	// ����evalPntsEnds 
	for (int i = 0; i < knots.size(); i++)
	{
		// �ӽڵ��ȡevaluate�˵�
		evalPntsEnds.push_back({ *(knots[i].begin() + order[i] - 1),
			*(knots[i].end() - order[i]) });

		evaluatePnts.push_back(evalPntsEnds[i]);
		for (int j = 0; j < numOfEvalPnts[i] - 1; j++)
		{
			// �ڳ�β֮ǰ������ȷֲ���
			evaluatePnts[i].insert(evaluatePnts[i].end() - 1,						
				*evalPntsEnds[i].begin() + (j + 1) * (*(evalPntsEnds[i].end() - 1) 
						- *evalPntsEnds[i].begin()) / numOfEvalPnts[i]);
		}

		// �������ڽڵ㷶Χ����
		evalPntsInd.push_back(findSpan(evaluatePnts[i], knots[i]));

		// ���������
		basisFuncsVal.push_back(basisFuncs(evalPntsInd[i], evaluatePnts[i], order[i] - 1, knots[i]));	

	}

	// ����evaluate Points������
	finalEvalPnts = calculateEvalPnts(evalPntsInd, basisFuncsVal);
}


// ������㻮�鵽�ڵ���
std::vector<int> NURBSParam::findSpan(std::vector<double> evalPnts, std::vector<double> knotsVec)
{
	// FIXME : this implementation has linear, rather than log complexity
	std::vector<int> evalPntsInd;
	std::vector<double> ::iterator knotsVecIterator = knotsVec.begin();

	for (int i = 0; i < evalPnts.size(); i++)
	{
		if (evalPnts[i] > *(knotsVec.end() - 1) || evalPnts[i] < *(knotsVec.begin()))
		{
			std::cerr << "Value: " << evalPnts[i] << " is outside the knot span\n"; break;
		}
		else if (evalPnts[i] == *(knotsVec.end() - 1));
		else
			while (evalPnts[i] >= *knotsVecIterator) knotsVecIterator++;//�ҵ�����ҿ�������������ΪԪ��;
		evalPntsInd.push_back(knotsVecIterator - knotsVec.begin() - 1);
	}	

	return evalPntsInd;
}



// ������������ֵ
std::vector<std::vector<double>> NURBSParam::basisFuncs(std::vector<int> evalPntsInd,
				std::vector<double> evalPnts, int degree, std::vector<double> knotsVec)
{
	int r;
	double saved, temp;

	std::vector<std::vector<double>> basicFuncVal;
	std::vector<double> left = { 0 };
	std::vector<double> right = { 0 };

	for(int i = 0; i < evalPnts.size(); i++)
	{
		basicFuncVal.push_back({ 1 });
		for (int j = 1; j <= degree; j++)
		{
			left.push_back(evalPnts[i] - knotsVec[evalPntsInd[i] + 1 - j]);
			right.push_back(knotsVec[evalPntsInd[i] + j] - evalPnts[i]);
			saved = 0.0;
			for (r = 0; r < j; r++)
			{
				temp = basicFuncVal[i][r] / (right[r + 1] + left[j - r]);
				basicFuncVal[i][r] = saved + right[r + 1] * temp;
				saved = left[j - r] * temp;
			}
			basicFuncVal[i].push_back(saved);
		}

		left.erase(left.begin() + 1, left.end());
		right.erase(right.begin() + 1, right.end());
	}
	return basicFuncVal;
}


// ��Ȩ����������������װ
vec_vec_dbl NURBSParam::calculateEvalPnts(std::vector<std::vector<int>> evalPntsInd,
	std::vector<vec_vec_dbl> basisFuncVal)
{
	std::vector<std::vector<double>> finalWeightedPnts;
	switch (evalPntsInd.size())
	{
		case 1:// NURBS Curve
			for (int i = 0; i < evalPntsInd[0].size(); i++)
				finalWeightedPnts.push_back(findCtrlPntsMat(evalPntsInd[0][i] , basisFuncVal[0][i]));
			break;
		case 2:// NURBS Surface
			for (int i = 0; i < evalPntsInd[1].size(); i++)
				for (int j = 0; j < evalPntsInd[0].size(); j++)
					finalWeightedPnts.push_back(findCtrlPntsMat(evalPntsInd[0][j], evalPntsInd[1][i],
						basisFuncVal[0][j], basisFuncVal[1][i]));
			break;
	}
	return finalWeightedPnts;

}



// ���߼����Ȩ���������˷�
std::vector<double> NURBSParam::findCtrlPntsMat(int index, std::vector<double> basisFuncValOnePnt)
{
	std::vector<double> weightedPntsRet = {0, 0, 0, 0};
	for (int i = 0; i < order[0]; i++)
		weightedPntsRet +=
			calPntsVecMultiply(coefs[index - (order[0] - 1) + i], basisFuncValOnePnt[i]);
	return weightedPntsRet;
}




// ��������Ȩ���������˷�
std::vector<double> NURBSParam::findCtrlPntsMat(int indexU, int indexV, std::vector<double> basisFunU, 
	std::vector<double> basisFuncV)
{
	std::vector<double> tempSelectedCtrlPnts = {0, 0, 0, 0};
	std::vector<double> weightedPntsRet = {0, 0, 0, 0};

	// ���������������Ƶ�����
	for (int i = 0; i < order[0]; i++)
		for (int j = 0; j < order[1]; j++)
		{
			tempSelectedCtrlPnts +=	calPntsVecMultiply(coefs[number[1] *
				(i + indexU - (order[0] - 1)) + j + indexV - (order[1] - 1)], basisFuncV[j]);
			if (j == order[1] - 1)
			{
				weightedPntsRet += calPntsVecMultiply(tempSelectedCtrlPnts,
					basisFunU[i]);
				tempSelectedCtrlPnts = { 0, 0, 0, 0 };
			}
		}

	return weightedPntsRet;
}



// ������������������д�ɷ�������
std::vector<double> NURBSParam::calPntsVecMultiply(std::vector<double> pntsToCal, double basisFunValForPnts)
{
	std::vector<double> retMultiWeightPnts;
	for (int i = 0; i < dim; i++)
		retMultiWeightPnts.push_back(pntsToCal[i] * basisFunValForPnts);

	return retMultiWeightPnts;
}



// ���Բ�����������Ԫ
void operator+=(std::vector<double>& xDimCoordFirst, 
		std::vector<double> xDimCoordSecond)
{
	for (int i = 0; i < xDimCoordFirst.size(); i++)
	{
		xDimCoordFirst[i] += xDimCoordSecond[i];
	}
	return;
}


