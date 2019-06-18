/********************************************************************************
* @copyright 2015, 高新兴科技集团股份有限公司 Co.Ltd
* @file		gsini.h
* @author	LiQiaohui(2702)
* @version	1.0
* @date		2015-3-10
* @brief	ini配置类头文件
* @details  ini配置类接口定义
********************************************************************************/

#ifndef __GSINI_H__
#define __GSINI_H__

#include "GSUtilDefs.h"

#include <map>
#include <fstream>

/********************
* 错误码定义
********************/
enum ErrorCode {
	SUCCESS = 0,          // 成功
	SECTION_EXIST,		  // 段重复
	SECTION_NOT_FOUND,    // 未找到段
	KEY_EXIST,            // 项重复
	KEY_NOT_FOUND,        // 未找到项
	FILE_NOT_FOUND,       // 未找到文件
	INVALID_ARG,          // 无效参数
	UNKNOWN
};


/********************
* ini文件读取
********************/
class GSUTIL_CLASS GSIni
{
public:
	GSIni(char const* filename);
	~GSIni();

	// 解析ini文件
	// return: 错误码 (SUCCESS, FILE_NOT_FOUND, UNKNOWN)
	ErrorCode Parse();

	// 获取int类型项
	// section: 段名称  key: 项名称 notfound: 未找到时的默认值
	// return: 获取到的值
	int GetInt(char const* section, char const* key, int notfound) const;
	bool GetBool(char const* section, char const* key, bool notfound) const;
	double GetDouble(char const* section, char const* key, double notfound) const;
	char const* GetString(char const* section, char const* key, char const* notfound) const;

	// 获取配置段的所有项
	// section：段名称，funcb：键值对返回回调函数，pUserData：回调函数中使用的用户透传参数
	// return：错误码（SUCCESS, INVALID_ARG， SECTION_NOT_FOUND ）
	// note：若传进来的section 或者 funcb 为 NULL ，则返回INVALID_ARG
	typedef void ( * FunPtrGetKeyValueCallBack)( const char * key, const char * value, void * pUserData );
	ErrorCode GetSection( const char * section, FunPtrGetKeyValueCallBack funcb, void * pUserData ) const;

	// 添加一项
	// section: 段名称  key: 项名称 value: 值
	// return: 错误码（SUCCESS, INVALID_ARG，KEY_EXIST）
	ErrorCode Add(char const* section, char const* key, char const* value);
	ErrorCode Add(char const* section, char const* key);
	// 修改一项的值
	// section: 段名称  key: 项名称 value: 值
	// return: 错误码（SUCCESS, INVALID_ARG， SECTION_NOT_FOUND, KEY_NOT_FOUND）
	ErrorCode Set(char const* section, char const* key, char const* value);
	// 删除一项
	// section: 段名称  key: 项名称 value: 值
	// return: 错误码（SUCCESS, INVALID_ARG， SECTION_NOT_FOUND, KEY_NOT_FOUND）
	ErrorCode DeleteKey(char const* section, char const* key);
	// 删除段及段下的所有项
	// section: 段名称
	// return: 错误码（SUCCESS, INVALID_ARG， SECTION_NOT_FOUND）
	ErrorCode DeleteSection(char const* section);

	// 保存配置到文件
	// filename: 文件名
	//           为NULL时保存到原始的文件，不为NULL时保存到指定的位置
	// return: 错误码 (SUCCESS, FILE_NOT_FOUND, UNKNOWN)
	ErrorCode SaveToFile(char const* filename = (char const*)0);

	//void PrintTest();

private:
	ErrorCode	parseKeyValue(const std::string& section);
	std::size_t getNotePosition(const std::string& str, std::size_t pos = 0);   //寻找行注释(';','#')的开始位置
	std::size_t getSectionPosition(const std::string& section, std::size_t pos = 0);
	std::size_t getKeyPosition(const std::string& key, std::size_t sPos, std::size_t ePos);

	ErrorCode	addToMap(const std::string& section, const std::string& key, const std::string& value);
	ErrorCode	setToMap(const std::string& section, const std::string& key, const std::string& value);
	ErrorCode	deleteToMap(const std::string& section, const std::string& key);
	ErrorCode	deleteToMap(const std::string& section);

	/*去除不可见字符*/
	std::string trim(const std::string& str);
	std::string trimLeft(const std::string& str);
	std::string trimRight(const std::string& str);

private:
	std::string _fileName;
	std::string _fileContent;
	std::fstream _fStream;
	std::map< std::string, std::map< std::string, std::string > > *_pMapSectionKeyValue; // section-key-value 映射
	//std::map< std::string, std::map< std::string, std::string > > _mapSectionKeyValue; // section-key-value 映射
};

#endif
