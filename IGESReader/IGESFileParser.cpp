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
		std::cerr << "Error open file" << filename << std::endl;// 无法打开文件
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
		std::cerr << "Error open file" << filename << std::endl;// 无法打开文件
		exit(EXIT_FAILURE);
	}
}

void IGESFileParser::infoByteParse()
{
	char ch;
	while (IGESFileIn.get(ch)) {// 若未到达文件尾 执行循环
		if(ch != '\n')
			infoByteStorage += ch;
		else;// 吃掉换行 
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
	// G段字符串划分用变量
	std::string GInfoString;
	int GFirstLineInd = lineType.find('G');
	int GLastLineInd = lineType.rfind('G');

	// 分隔符处理略
	GStringStorage[0] = ',';
	GStringStorage[1] = ';';
	short k = 2;
	const short SepDefSeg = 8;

	// 首先将字符串按照分隔符分开
	for (int i = GFirstLineInd; i <= GLastLineInd; i++)
		for (short j = i * lineCharCount; j < i * lineCharCount + lineInfoCount; j++)
		{
			if (j == GFirstLineInd * lineCharCount) j += SepDefSeg; // 跳过分隔符设置
			else;//这句貌似不加亦可

			if (infoByteStorage[j] != ',' && j < GLastLineInd * lineCharCount + lineInfoCount - 1
					&& infoByteStorage[j] != ';')
				GInfoString.push_back(infoByteStorage[j]);
			else if(infoByteStorage[j] != ';'){
				GStringStorage[k++] = GInfoString;
				GInfoString.clear();
			}
			else;//;表示G段结束
		}

	// G段信息两部分分别整合
	const short GStringWithSpaceH[] = { 2, 3, 4, 5, 11, 14, 17, 20, 21, 22, 24 };
	const short GStringToNum[] = { 6, 7, 8, 9, 10, 12, 13, 15, 16, 18, 19, 23 };

	size_t GPartInfoStart(0), GPartInfoEnd(0);// 《C++ Primer Plus》6th Edition Pg42
	std::stringstream inOutStrStream;
	float stringToNum(0);

	// 找到H字符和H后第一个空格字符
	for (short i : GStringWithSpaceH)
	{
		GPartInfoStart = GStringStorage[i].find('H') + 1;
		GPartInfoEnd = GStringStorage[i].find(' ', GPartInfoStart);//空格索引是最后一个有效字符后一个位置
		GStringStorage[i] = GStringStorage[i].substr(GPartInfoStart, GPartInfoEnd - GPartInfoStart);
	}

	for (short i : GStringToNum)
	{
		if (GStringStorage[i] == std::string(GStringStorage[i].size(), ' '))// 检查字符串非空
		{
			GNumStorage.insert(std::pair<int, float>(i, 0));
			continue;
		}
		inOutStrStream << GStringStorage[i];	// 内核格式化转换成数字
		inOutStrStream >> stringToNum ;
		inOutStrStream.clear();					
		GNumStorage.insert(std::pair<int, float>(i, stringToNum));
	}
		
}


// 处理D段信息 参考GB/T 14213-2008 Pg29
void IGESFileParser::DInfoSegProcess()
{
	std::vector<std::string> DStringStorage;
	int DFirstLineInd = lineType.find('D');
	int DLastLineInd = lineType.rfind('D');

	std::stringstream inOutStream;
	int j(0), k(0);
	
	// 提取D段原始信息 并整合到结构体中
	for (int i = DFirstLineInd; i < DLastLineInd; i += 2)// DLastLineInd不可能是奇数
	{
		for (j = 0; j < 20; j++)
			DStringStorage.push_back(infoByteStorage.substr(i * lineCharCount + j * 8, 8));

		DInfoStorage.push_back(DInfoStorageType());
		j = 0;

		// 实体类型号
		inOutStream << DStringStorage[j];
		inOutStream >> DInfoStorage[k].entityTypeNum;
		inOutStream.clear();

		// 参数数据
		inOutStream << DStringStorage[j + 1];
		inOutStream >> DInfoStorage[k].paramStartLineInd;
		inOutStream.clear();

		// 变换矩阵
		inOutStream << DStringStorage[j + 6];
		inOutStream >> DInfoStorage[k].transformMatPtr;
		inOutStream.clear();

		// 颜色号
		inOutStream << DStringStorage[j + 12];
		inOutStream >> DInfoStorage[k].colorNumOrPtr;
		inOutStream.clear();

		// 格式号
		inOutStream << DStringStorage[j + 14];
		inOutStream >> DInfoStorage[k++].formNum;
		inOutStream.clear();

		DStringStorage.clear();
	}
		
}


// Pg109
void IGESFileParser::PInfoSegProcess()
{
	std::string PStringStorage;		// 每个实体P段字符串
	int PFirstLineInd, PLastLineInd;
	const int linePInfoCount = 64;
	int entityType;

	// 用于将分号分隔符替换成逗号分隔
	std::string::size_type pos;
	int PStrSepPosInd(0);
	std::vector<int> PStrSepPosIndVec;
	std::vector<double> PInfoStorageVec;

	// 用于找到逗号索引并划分字符串转化成数值
	int PInfoStrFirstInd, PInfoStrLastInd;
	std::stringstream inOutStrStream;
	double PInfoStorage;

	// 对每个实体执行下面的循环
	for (int i = 0; i < DInfoStorage.size(); i++)
	{
		PFirstLineInd = DInfoStorage[i].paramStartLineInd + lineType.rfind('D');
		if (i < DInfoStorage.size() - 1)
			PLastLineInd = DInfoStorage[i + 1].paramStartLineInd - 1 + lineType.rfind('D');
		else
			PLastLineInd = lineType.rfind('P');

		// 连续取出同一实体每行前64个字符串
		for (int j = PFirstLineInd; j < PLastLineInd + 1; j++)
		{
			PStringStorage.append(infoByteStorage.substr(j * lineCharCount, linePInfoCount));
		}

		// 暂时不考虑分隔符是分号或自定义情况，先都替换为逗号
		pos = PStringStorage.find(';');
		while (pos != std::string::npos)
		{
			PStringStorage.replace(pos, 1, ",");
			pos = PStringStorage.find(';');
		}

		// 找到逗号位置构成向量
		PStrSepPosInd = PStringStorage.find(',');
		while (PStrSepPosInd != std::string::npos)
		{
			PStrSepPosIndVec.push_back(PStrSepPosInd);
			PStrSepPosInd = PStringStorage.find(',', PStrSepPosInd + 1);
		}

		// 分隔字符串并转换成数字
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

		// 根据类型创建对象
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
		
		PInfoStorageClass[i]->parseIGESStringInfo(PInfoStorageVec);// 分词并提取有效参数信息
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



