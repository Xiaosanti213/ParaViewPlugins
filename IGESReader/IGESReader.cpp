// IGESReader.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "IGESFileParser.h"
#include "sstream"
#include "NURBSParam.h"
#include "pvIGESReader.h"
#include "vtkSmartPointer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkPolyDataWriter.h"


using namespace std;

int main(int argc, char** argv)
{
	/*
	using namespace std;

	
	IGESFileParser example2("example2.igs"); 
	example2.infoByteParse();
	example2.GInfoSegProcess();
	example2.DInfoSegProcess();
	example2.PInfoSegProcess();
	*/
	


	// 参考《VTK图形图像开发进阶》Pg40
	vtkSmartPointer<pvIGESReader> IGESFileReaderInstance =
		vtkSmartPointer<pvIGESReader>::New();

	IGESFileReaderInstance->SetFileName("C:\\Users\\lenovo\\Desktop\\doc\\model\\UAV\\UAV_asm.igs");

	//IGESFileReaderInstance->DebugOn();
	IGESFileReaderInstance->Update();

	vtkSmartPointer<vtkPolyData> NURBSPolyData =
		vtkSmartPointer<vtkPolyData>::New();
	NURBSPolyData = IGESFileReaderInstance->GetOutput();

	// 参考 Pg53
	vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName("UAVPolyDataFile.vtk");
	writer->SetInputData(NURBSPolyData);
	writer->Write();


	
	/*
	vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	polyDataMapper->SetInputConnection(IGESFileReaderInstance->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();

	actor->SetMapper(polyDataMapper);

	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();

	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renWin->AddRenderer(renderer);
	interactor->SetRenderWindow(renWin);
	renderer->AddActor(actor);
	renderer->Render();


	interactor->Initialize();
	interactor->Start();*/

	
	// 创建一个NRUBS Curve
	/*
	std::vector<std::vector<double>> coefsPos = {
		{0.5, 3, 0, 1}, {1.5, 5.5, 0, 1}, {4.5, 5.5, 0, 1},
		{3.0, 1.5, 0, 1}, {7.5, 1.5, 0, 1}, {6.0, 4, 0, 1},
		{8.5, 4.5, 0, 1}
	};

	std::vector<std::vector<double>> knotsPos = {
		{ 0, 0, 0, 0.1, 0.5, 0.75, 0.75, 1, 1, 1 }	};


	int uintToIntCurveOrder = knotsPos[0].size() - coefsPos.size();// 赋值缩窄转换没有初始化列表转换严格
	std::vector<int> curveOrder = {
		uintToIntCurveOrder
	};

	NURBSParam* createNURBSParam = new 
		NURBSParam("B-NURBS Curve", 4, {5, 1}, coefsPos, knotsPos, curveOrder);


	createNURBSParam->getEvaluatePoints({ 100 });
	*/
	


	


	// 创建一个NURBS Surface
	/*
	std::vector<std::vector<double>> coefsPos = {
		{0, 0, 8, 1}, {3, 0, 2, 1}, {5, 0, 7, 1}, {8, 0, 7, 1},
		{10, 0, 8, 1},
		{0, 3, 0, 1}, {3, 3, 0, 1}, {5, 3, 5, 1}, {8, 3, 5, 1},
		{10, 3, 7, 1},
		{0, 5, 0, 1}, {3, 5, 0, 1}, {5, 5, 5, 1}, {8, 5, 5, 1},
		{10, 5, 7, 1},
		{0, 8, 5, 1}, {3, 8, 5, 1}, {5, 8, 8, 1}, {8, 8, 8, 1},
		{10, 8, 10, 1},
		{0, 10, 5, 1},{3, 10, 5, 1},{5, 10, 8, 1},{8, 10, 8, 1},
		{10, 10, 10, 1}
	};

	

	std::vector<std::vector<double>> knotsPos = {
		{0, 0, 0, 1.0/3, 2.0/3, 1, 1, 1},
		{0, 0, 0, 1.0/3, 2.0/3, 1, 1, 1}
	};


	NURBSParam* testNURBSParamSurf = new NURBSParam(
		"B-NURBS", 4, { 5, 5 }, coefsPos, knotsPos, {3, 3}
	);

	testNURBSParamSurf->getTransposeCoefsMat();
	testNURBSParamSurf->showNURBSParamInfo();
	testNURBSParamSurf->getEvaluatePoints({ 20, 30 });
	
	*/

	//// 用于调试
	//int a;
	//cin >> a;
	//return 0;

}

