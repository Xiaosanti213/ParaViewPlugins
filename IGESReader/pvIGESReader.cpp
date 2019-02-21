#include "pvIGESReader.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkTriangle.h"
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
	
	// �������ļ��ж�����
	varIGESFileParser.setIGESFileName(FileName);
	varIGESFileParser.infoByteParse();
	varIGESFileParser.GInfoSegProcess();
	varIGESFileParser.DInfoSegProcess();
	varIGESFileParser.PInfoSegProcess();


	// ����PolyData����
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
	vtkIdType pid[1];//Point Id

	// ��NURBS����������ȡ����
	EntityBSplineSurf* BSplineSurfPtr;
	std::vector<std::vector<double>> NURBSEntityEvalPnts;
	unsigned int* NURBSEntityTriangleInd;
	int numParamField[2];
	int numTriGrid;
	double finalPnts[3];
	static int transInd = 0;

	for (int i = 0; i < varIGESFileParser.getNumOfEntity(); i++)
	{
		switch (varIGESFileParser.getEachEntityClass(i)->getEntityType())
		{
		case 128://NURBS����
			if (BSplineSurfPtr = dynamic_cast<EntityBSplineSurf*>(varIGESFileParser.getEachEntityClass(i)))
			{
				BSplineSurfPtr->getnumUnumV(numParamField);
				numTriGrid = 2 * (numParamField[0]-1) * (numParamField[1] - 1);
				NURBSEntityEvalPnts = *(BSplineSurfPtr->getNURBSSurf()->getFinalEvalPnts());//const�÷�
				NURBSEntityTriangleInd = new unsigned int[3* numTriGrid];
				memcpy(NURBSEntityTriangleInd, BSplineSurfPtr->getTriangleIndex(), 
								numTriGrid * sizeof(unsigned int) * 3);
				for (int j = 0; j < NURBSEntityEvalPnts.size(); j++)
				{
					pid[0] = points->InsertNextPoint(//Ȩ�ض�1
									NURBSEntityEvalPnts[j][0],
									NURBSEntityEvalPnts[j][1],
									NURBSEntityEvalPnts[j][2]);
					//vertices->InsertNextCell(1, pid);
				}

				for (int k = 0; k < numTriGrid; k++)
				{
					triangle->GetPointIds()->SetId(0, NURBSEntityTriangleInd[k] - 1 + transInd);
					triangle->GetPointIds()->SetId(1, NURBSEntityTriangleInd[k + numTriGrid] - 1 + transInd);
					triangle->GetPointIds()->SetId(2, NURBSEntityTriangleInd[k + 2 * numTriGrid] - 1 + transInd);
					triangles->InsertNextCell(triangle);
				}
				outData->SetPoints(points);
				outData->SetStrips(triangles);

				// ��������ƽ�Ʊ���
				transInd += numParamField[0] * numParamField[1];
			}
			break;
		default:
			break;
		}
	}

	return 1;
}







