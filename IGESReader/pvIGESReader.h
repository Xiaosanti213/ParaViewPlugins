#ifndef _PVIGESREADER_H
#define _PVIGESREADER_H

#include "vtkPolyDataAlgorithm.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "IGESFileParser.h"
#include "vtkSetGet.h"


class pvIGESReader : public vtkPolyDataAlgorithm
{
public:

	static pvIGESReader *New();
	vtkTypeMacro(pvIGESReader, vtkPolyDataAlgorithm);

	vtkSetStringMacro(FileName);
	vtkGetStringMacro(FileName);

protected:
	pvIGESReader();
	~pvIGESReader();
	int FillInputPortInformation(int port, vtkInformation * info);
	int FillOutputPortInformation(int port, vtkInformation* info);
	
	int ProcessRequest(vtkInformation*, vtkInformationVector**,
		vtkInformationVector*);
	int RequestDataObject(vtkInformation *, vtkInformationVector **, vtkInformationVector * outputVector);
	int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

	char* FileName;

private:
	pvIGESReader(const pvIGESReader&);
	void operator=(const pvIGESReader&);

private:
	IGESFileParser varIGESFileParser;

};




#endif // !_PVIGESREADER_H

