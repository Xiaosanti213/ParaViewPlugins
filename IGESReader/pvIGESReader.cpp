#include "pvIGESReader.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkDemandDrivenPipeline.h"
#include "vtkStreamingDemandDrivenPipeline.h"


vtkStandardNewMacro(pvIGESReader);

pvIGESReader::pvIGESReader() : FileName(nullptr)
{
	this->SetNumberOfInputPorts(0);
	this->SetNumberOfOutputPorts(1);
}

pvIGESReader::~pvIGESReader()
{
	SetFileName(0);
}


int pvIGESReader::FillInputPortInformation(int port, vtkInformation* info)
{
	info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
	return 1;
}


int pvIGESReader::FillOutputPortInformation(int port, vtkInformation* info)
{
	if (port == 0)
	{
		info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkPolyData");
		info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
		return 1;
	}
	vtkErrorMacro("This filter does not have more than 1 output port!");
}


int pvIGESReader::ProcessRequest(vtkInformation * request, 
		vtkInformationVector ** inputVector, vtkInformationVector* outputVector)
{
	if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA_OBJECT()))
	{
		return this->RequestDataObject(request, inputVector, outputVector);
	}

	if (request->Has(vtkDemandDrivenPipeline::REQUEST_DATA()))
	{
		return this->RequestData(request, inputVector, outputVector);
	}

	if (request->Has(vtkStreamingDemandDrivenPipeline::REQUEST_UPDATE_EXTENT()))
	{
		return this->RequestUpdateExtent(request, inputVector, outputVector);
	}

	if (request->Has(vtkDemandDrivenPipeline::REQUEST_INFORMATION()))
	{
		return this->RequestInformation(request, inputVector, outputVector);
	}
	return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}


int pvIGESReader::RequestDataObject(vtkInformation*, vtkInformationVector**,
	vtkInformationVector* outputVector)
{
	vtkInformation* outInfo = outputVector->GetInformationObject(0);
	vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->
		Get(vtkPolyData::DATA_OBJECT()));

	if (!output)
	{
		output = vtkPolyData::New();
		outInfo->Set(vtkPolyData::DATA_OBJECT(), output);
		output->FastDelete();
		//output->SetPipelineInformation(outInfo);

		this->GetOutputPortInformation(0)->Set(
			vtkDataObject::DATA_EXTENT_TYPE(), output->GetExtentType());
	}

	return 1;
}




int pvIGESReader::RequestData(vtkInformation* vtkNotUsed(request),
	vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkPolyData* outData = vtkPolyData::SafeDownCast(outInfo->Get
											(vtkDataObject::DATA_OBJECT()));
	
	// 从输入文件中读数据
	varIGESFileParser.setIGESFileName(FileName);
	varIGESFileParser.infoByteParse();
	varIGESFileParser.GInfoSegProcess();
	varIGESFileParser.DInfoSegProcess();
	varIGESFileParser.PInfoSegProcess();


	// 创建PolyData数据
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	vtkIdType pid[1];//Point Id

	// 将NURBS曲面数据提取整合
	EntityBSplineSurf* BSplineSurfPtr;
	std::vector<std::vector<double>> NURBSEntityEvalPnts;
	double finalPnts[3];

	for (int i = 0; i < varIGESFileParser.getNumOfEntity(); i++)
	{
		switch (varIGESFileParser.getEachEntityClass(i)->getEntityType())
		{
		case 128://NURBS曲面
			if (BSplineSurfPtr = dynamic_cast<EntityBSplineSurf*>(varIGESFileParser.getEachEntityClass(i)))
			{
				NURBSEntityEvalPnts = *(BSplineSurfPtr->getNURBSSurf()->getFinalEvalPnts());//const用法
				for (int j = 0; j < NURBSEntityEvalPnts.size(); j++)
				{
					pid[0] = points->InsertNextPoint(//权重都1
									NURBSEntityEvalPnts[j][0],
									NURBSEntityEvalPnts[j][1],
									NURBSEntityEvalPnts[j][2]);
					vertices->InsertNextCell(1, pid);
				}
				outData->SetPoints(points);
				outData->SetVerts(vertices);
			}
			break;
		default:
			break;
		}
	}

	return 1;
}







