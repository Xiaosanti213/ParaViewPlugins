#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>

#include "EntityParam.h"



class IGESFileParser
{
public:
	IGESFileParser(std::string);
	IGESFileParser() {};
	~IGESFileParser();

	void setIGESFileName(char* fileName);

	void infoByteParse();							 // 提取IGES文件信息
	void GInfoSegProcess();
	void DInfoSegProcess();
	void PInfoSegProcess();

	int getNumOfEntity() { return PInfoStorageClass.size(); };
	EntityParam* getEachEntityClass(int num = 0);

private:
	int infoLength;									// IGES文件字符数组长度
	std::ifstream IGESFileIn;						// 用文件名初始化输入流，建立联系
	std::string lineType;							// 可能用string类型更好一些
	const short lineCharCount = 80;
	const short lineInfoCount = 72;
	std::string infoByteStorage;					// IGES文件字符ASCII码数组

	std::array<std::string, 25> GStringStorage;		// 定长使用array比vector效率高
	std::map<short, float> GNumStorage;				// 存储GStringStorage索引对应的浮点数

	struct DInfoStorageType {
		int entityTypeNum;							// 实体类型号
		int paramStartLineInd;						// 参数数据
		int transformMatPtr;						// 变换矩阵
		int colorNumOrPtr;							// 颜色号
		int formNum;
	};
	std::vector<DInfoStorageType> DInfoStorage;

	std::vector< EntityParam* > PInfoStorageClass;	// 多态存储实体子类信息 但是不能用引用
};

