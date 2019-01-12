#pragma once
#include <string>
#include <vector>


typedef std::vector<std::vector<double>> vec_vec_dbl;



class NURBSParam
{
public:
	NURBSParam(std::string NURBSform = "B-NURBS", int NURBSDim = 4, 
		 std::vector<int> NURBSNumber = { 1 }, 
			std::vector<std::vector<double>> NURBSCoefs = { {0,0,0,1} },
				std::vector<std::vector<double>> NURBSKnots = { {0, 1} },
					std::vector<int> NURBSOrder = { 1 });
	~NURBSParam();

	void showNURBSParamInfo();

	void setDim(int);
	void setNumber(std::vector<int>);
	void setCoefs(std::vector <std::vector <double>>);
	void setWeight(std::vector<double>);
	void setKnots(std::vector <std::vector <double>>);
	void setOrder(std::vector <int>);

	const vec_vec_dbl* getFinalEvalPnts() const { return &finalEvalPnts; };
	
	void getTransposeCoefsMat();
	void getEvaluatePoints(std::vector<int>); // 分别指定两个方向上点的个数
	
private:
	std::vector<int> findSpan(std::vector<double>, std::vector<double>);
	std::vector<std::vector<double>> basisFuncs(std::vector<int>, std::vector<double>, int, std::vector<double>);

	vec_vec_dbl calculateEvalPnts(std::vector<std::vector<int>>, std::vector<vec_vec_dbl>);
	std::vector<double> findCtrlPntsMat(int, std::vector<double>);
	std::vector<double> findCtrlPntsMat(int, int, std::vector<double>, std::vector<double>);
	std::vector<double> calPntsVecMultiply(std::vector<double>, double);


friend	void operator+=(std::vector<double>&, std::vector<double>);


private:
	std::string form;
	int dim;											// 维数
	std::vector<int> number;							// 控制点个数
	std::vector<std::vector<double>> coefs;				// 控制点坐标(个数由dim和number变量决定)
	std::vector< std::vector<double> > knots;			// 节点坐标
	std::vector<int> order;								// 阶数 order = deg + 1;

	std::vector<std::vector<double>> evalPntsEnds;		// 待估计点起点终点							
	std::vector<std::vector<double>> evaluatePnts;
	vec_vec_dbl finalEvalPnts;//用于存储结果


};

