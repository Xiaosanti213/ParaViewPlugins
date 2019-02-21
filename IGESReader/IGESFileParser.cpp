#include "stdafx.h"
#include "IGESFileParser.h"
#include <iostream>
#include <cstdlib> // exit(EXIT_FAILURE)
#include <algorithm>
#include <vector>
#include <sstream>



IGESFileParser::IGESFileParser(std::string filename):
	infoByteStorage(),infoLength(0),IGESFileIn(filename.c_str())
{
	if (!IGESFileIn.is_open())
	{
		std::cerr << "Error open file" << filename << std::endl;// �޷����ļ�
		exit(EXIT_FAILURE);
	}
	
}

IGESFileParser::~IGESFileParser()
{
}

void IGESFileParser::setIGESFileName(char * filename)
{
	IGESFileIn.open(filename);
	if (!IGESFileIn.is_open())
	{
		std::cerr << "Error open file" << filename << std::endl;// �޷����ļ�
		exit(EXIT_FAILURE);
	}
}

void IGESFileParser::infoByteParse()
{
	char ch;
	while (IGESFileIn.get(ch)) {// ��δ�����ļ�β ִ��ѭ��
		if(ch != '\n')
			infoByteStorage += ch;
		else;// �Ե����� 
	}
	// std::cout << infoByteStorage.size() << std::endl;


	for (int i = 0; i < infoByteStorage.size() / lineCharCount; i++)
	{
		lineType.push_back(infoByteStorage[i * lineCharCount + lineInfoCount]);
		if (lineType[i] == 'S')
			std::cout << infoByteStorage.substr(i * lineCharCount, lineInfoCount) << std::endl;
	}
	
}


void IGESFileParser::GInfoSegProcess()
{
	// G���ַ��������ñ���
	std::string GInfoString;
	int GFirstLineInd = lineType.find('G');
	int GLastLineInd = lineType.rfind('G');

	// �ָ���������
	GStringStorage[0] = ',';
	GStringStorage[1] = ';';
	short k = 2;
	const short SepDefSeg = 8;

	// ���Ƚ��ַ������շָ����ֿ�
	for (int i = GFirstLineInd; i <= GLastLineInd; i++)
		for (short j = i * lineCharCount; j < i * lineCharCount + lineInfoCount; j++)
		{
			if (j == GFirstLineInd * lineCharCount) j += SepDefSeg; // �����ָ�������
			else;//���ò�Ʋ������

			if (infoByteStorage[j] != ',' && j < GLastLineInd * lineCharCount + lineInfoCount - 1
					&& infoByteStorage[j] != ';')
				GInfoString.push_back(infoByteStorage[j]);
			else if(infoByteStorage[j] != ';'){
				GStringStorage[k++] = GInfoString;
				GInfoString.clear();
			}
			else;//;��ʾG�ν���
		}

	// G����Ϣ�����ֱַ�����
	const short GStringWithSpaceH[] = { 2, 3, 4, 5, 11, 14, 17, 20, 21, 22, 24 };
	const short GStringToNum[] = { 6, 7, 8, 9, 10, 12, 13, 15, 16, 18, 19, 23 };

	size_t GPartInfoStart(0), GPartInfoEnd(0);// ��C++ Primer Plus��6th Edition Pg42
	std::stringstream inOutStrStream;
	float stringToNum(0);

	// �ҵ�H�ַ���H���һ���ո��ַ�
	for (short i : GStringWithSpaceH)
	{
		GPartInfoStart = GStringStorage[i].find('H') + 1;
		GPartInfoEnd = GStringStorage[i].find(' ', GPartInfoStart);//�ո����������һ����Ч�ַ���һ��λ��
		GStringStorage[i] = GStringStorage[i].substr(GPartInfoStart, GPartInfoEnd - GPartInfoStart);
	}

	for (short i : GStringToNum)
	{
		if (GStringStorage[i] == std::string(GStringStorage[i].size(), ' '))// ����ַ����ǿ�
		{
			GNumStorage.insert(std::pair<int, float>(i, 0));
			continue;
		}
		inOutStrStream << GStringStorage[i];	// �ں˸�ʽ��ת��������
		inOutStrStream >> stringToNum ;
		inOutStrStream.clear();					
		GNumStorage.insert(std::pair<int, float>(i, stringToNum));
	}
		
}


// ����D����Ϣ �ο�GB/T 14213-2008 Pg29
void IGESFileParser::DInfoSegProcess()
{
	std::vector<std::string> DStringStorage;
	int DFirstLineInd = lineType.find('D');
	int DLastLineInd = lineType.rfind('D');

	std::stringstream inOutStream;
	int j(0), k(0);
	
	// ��ȡD��ԭʼ��Ϣ �����ϵ��ṹ����
	for (int i = DFirstLineInd; i < DLastLineInd; i += 2)// DLastLineInd������������
	{
		for (j = 0; j < 20; j++)
			DStringStorage.push_back(infoByteStorage.substr(i * lineCharCount + j * 8, 8));

		DInfoStorage.push_back(DInfoStorageType());
		j = 0;

		// ʵ�����ͺ�
		inOutStream << DStringStorage[j];
		inOutStream >> DInfoStorage[k].entityTypeNum;
		inOutStream.clear();

		// ��������
		inOutStream << DStringStorage[j + 1];
		inOutStream >> DInfoStorage[k].paramStartLineInd;
		inOutStream.clear();

		// �任����
		inOutStream << DStringStorage[j + 6];
		inOutStream >> DInfoStorage[k].transformMatPtr;
		inOutStream.clear();

		// ��ɫ��
		inOutStream << DStringStorage[j + 12];
		inOutStream >> DInfoStorage[k].colorNumOrPtr;
		inOutStream.clear();

		// ��ʽ��
		inOutStream << DStringStorage[j + 14];
		inOutStream >> DInfoStorage[k++].formNum;
		inOutStream.clear();

		DStringStorage.clear();
	}
		
}


// Pg109
void IGESFileParser::PInfoSegProcess()
{
	std::string PStringStorage;		// ÿ��ʵ��P���ַ���
	int PFirstLineInd, PLastLineInd;
	const int linePInfoCount = 64;
	int entityType;

	// ���ڽ��ֺŷָ����滻�ɶ��ŷָ�
	std::string::size_type pos;
	int PStrSepPosInd(0);
	std::vector<int> PStrSepPosIndVec;
	std::vector<double> PInfoStorageVec;

	// �����ҵ����������������ַ���ת������ֵ
	int PInfoStrFirstInd, PInfoStrLastInd;
	std::stringstream inOutStrStream;
	double PInfoStorage;

	// ��ÿ��ʵ��ִ�������ѭ��
	for (int i = 0; i < DInfoStorage.size(); i++)
	{
		PFirstLineInd = DInfoStorage[i].paramStartLineInd + lineType.rfind('D');
		if (i < DInfoStorage.size() - 1)
			PLastLineInd = DInfoStorage[i + 1].paramStartLineInd - 1 + lineType.rfind('D');
		else
			PLastLineInd = lineType.rfind('P');

		// ����ȡ��ͬһʵ��ÿ��ǰ64���ַ���
		for (int j = PFirstLineInd; j < PLastLineInd + 1; j++)
		{
			PStringStorage.append(infoByteStorage.substr(j * lineCharCount, linePInfoCount));
		}

		// ��ʱ�����Ƿָ����ǷֺŻ��Զ���������ȶ��滻Ϊ����
		pos = PStringStorage.find(';');
		while (pos != std::string::npos)
		{
			PStringStorage.replace(pos, 1, ",");
			pos = PStringStorage.find(';');
		}

		// �ҵ�����λ�ù�������
		PStrSepPosInd = PStringStorage.find(',');
		while (PStrSepPosInd != std::string::npos)
		{
			PStrSepPosIndVec.push_back(PStrSepPosInd);
			PStrSepPosInd = PStringStorage.find(',', PStrSepPosInd + 1);
		}

		// �ָ��ַ�����ת��������
		for (int k = 0; k < PStrSepPosIndVec.size(); k++)
		{
			if (k == 0) PInfoStrFirstInd = 0;
			else  PInfoStrFirstInd = PStrSepPosIndVec[k - 1] + 1;

			PInfoStrLastInd = PStrSepPosIndVec[k] - 1;
			inOutStrStream << PStringStorage.substr(PInfoStrFirstInd,
									PInfoStrLastInd - PInfoStrFirstInd + 1);
			inOutStrStream >> PInfoStorage;
			inOutStrStream.clear();
			PInfoStorageVec.push_back(PInfoStorage);
		}

		// �������ʹ�������
		EntityTransformMat* entityTransMatPtr;
		entityType = DInfoStorage[i].entityTypeNum;
		switch (entityType)									
		{
			case 102:
				PInfoStorageClass.push_back(new EntityCompositeCurve()); break;
			case 124:
				PInfoStorageClass.push_back(new EntityTransformMat()); break;
			case 126:
				PInfoStorageClass.push_back(new EntityBSplineCurve()); break;
			case 128:
				entityTransMatPtr = dynamic_cast<EntityTransformMat*>(*(PInfoStorageClass.end()-1));
				PInfoStorageClass.push_back(new EntityBSplineSurf(entityTransMatPtr, 200, 200)); break;
			case 142:
				PInfoStorageClass.push_back(new EntityCurveOnParamSurf()); break;
			case 144:
				PInfoStorageClass.push_back(new EntityClipParamSurf()); break;
			case 404:
				PInfoStorageClass.push_back(new EntityDrawing()); break;
			case 406:
				PInfoStorageClass.push_back(new EntityProperty()); break;
			case 410:
				PInfoStorageClass.push_back(new EntityView()); break;
			default:;
		}
		
		PInfoStorageClass[i]->parseIGESStringInfo(PInfoStorageVec);// �ִʲ���ȡ��Ч������Ϣ
		PStringStorage.clear();
		PInfoStorageVec.clear();
		PStrSepPosIndVec.clear();
	}

}

EntityParam* IGESFileParser::getEachEntityClass(int num)
{
	if (num < getNumOfEntity())
		return PInfoStorageClass[num];
	else
		std::cerr << "index out of range of Number of Entity\n";
}



int IGESFileParser::paramStartLineIndFindPClassInd(int indToFind)
{
	for (int ind = 0; ind < DInfoStorage.size(); ind++)
		if (DInfoStorage[ind].paramStartLineInd == indToFind) return ind;
	return -1;
}



