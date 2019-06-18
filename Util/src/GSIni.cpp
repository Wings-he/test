/********************************************************************************
* @copyright 2015, 高新兴科技集团股份有限公司 Co.Ltd
* @file		gsini.cpp
* @author	LiQiaohui(2702)
* @version	1.0
* @date		2015-3-10
* @brief	ini配置类源文件
* @details  ini配置类接口实现
********************************************************************************/

#include "GSIni.h"

#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <locale.h>

#define _mapSectionKeyValue (*_pMapSectionKeyValue)

GSIni::GSIni(char const* filename)
{
	_pMapSectionKeyValue = new std::map< std::string, std::map< std::string, std::string > >();

	GSAbort(_pMapSectionKeyValue);

	if (NULL != filename)
		_fileName = filename;
	else
		_fileName = "";
	
	_fileContent = "";
}

GSIni::~GSIni()
{
	if (_fStream.is_open())
		_fStream.close();

	delete _pMapSectionKeyValue;

}

ErrorCode GSIni::Parse()
{
	if ("" == _fileName)
		return FILE_NOT_FOUND;

	
	// 解决vs2008下无法打开包含中文路径文件的问题
	std::locale oldLoc = std::locale::global(std::locale(""));

	_fStream.open(_fileName.c_str(), std::ios::in);
	std::locale::global(std::locale(oldLoc));

	if (!_fStream.is_open())
	{
		std::locale::global(std::locale(oldLoc));
		return FILE_NOT_FOUND;
	}
	std::locale::global(std::locale(oldLoc));

	while (!_fStream.eof())
	{
		std::string buff;
		getline(_fStream, buff);

		buff = trimLeft(buff);


		if( buff.length()<1)
		{
			continue;
		}

		std::string bak = buff;

		std::size_t notePos = getNotePosition(buff);   //注释开始位置

		if (0 == notePos)		//整行注释
		{
			_fileContent += (bak + "\n");
			continue;
		}
			
		if (0 < notePos)
			buff = buff.substr(0, notePos - 1);
		
		std::size_t lPos = buff.find("[");
		std::size_t rPos = buff.find("]");
		if (lPos == std::string::npos || rPos == std::string::npos || rPos <= lPos)
			return UNKNOWN;
		std::string sectionName = buff.substr(lPos + 1, rPos - lPos - 1);
		sectionName = trim(sectionName);
		std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(sectionName);
		if (it != _mapSectionKeyValue.end())
		{
			return SECTION_EXIST;
		}

		_fileContent += (bak + "\n");

		/*解析key-value*/
		ErrorCode err = parseKeyValue(sectionName);  //递归
		if (SUCCESS != err)
		{
			return err;
		}
	}

	_fileContent += "<$$$>";

	_fStream.close();

	return SUCCESS;
}

ErrorCode GSIni::parseKeyValue(const std::string& section)
{
	while (!_fStream.eof())
	{
		std::string buff;
		getline(_fStream, buff);
		std::string bak = buff;
		std::size_t lineSize = buff.size();

		std::size_t notePos = getNotePosition(buff);   //注释开始位置

		if (0 == notePos)				//整行注释
		{
			_fileContent += (bak + "\n");
			continue;
		}

		if (0 < notePos)
			buff = buff.substr(0, notePos - 1);

		std::size_t lPos = buff.find("[");
		std::size_t rPos = buff.find("]");
		if (lPos != std::string::npos && rPos != std::string::npos && lPos < rPos)
		{
			_fileContent += ("<$$$>" + bak + "\n");  //插入一个“<$$$>”，标示section结束

			std::string sectionName = buff.substr(lPos + 1, rPos - 1);
			sectionName = trim(sectionName);
			return parseKeyValue(sectionName);
		}

		_fileContent += (bak + "\n");

		std::size_t ePos = buff.find("=");

		std::string key, value;
		if (std::string::npos == ePos)    //存在只有key没有value的情况
		{
			key = trim(buff);
			if (std::string("") == key)  //空行
				continue;
			value = std::string("");
		}
		else
		{
			key = trim(buff.substr(0, ePos));
			value = trim(buff.substr(ePos + 1, buff.size()));
		}

		/*去除不可见字符*/
		std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(section);
		if (it != _mapSectionKeyValue.end())
		{
			std::map< std::string, std::string >::iterator iter = it->second.find(key);
			if (iter != it->second.end())
			{
				return KEY_EXIST;
			}
			else
			{
				it->second.insert(make_pair(key, value));
			}
		}
		else
		{
			_mapSectionKeyValue[section] = std::map< std::string, std::string >();
			_mapSectionKeyValue[section].insert(make_pair(key, value));
		}
	}

	return SUCCESS;
}

int GSIni::GetInt(char const* section, char const* key, int notfound) const
{
	if (NULL == section || NULL == key)
		return notfound;

	std::string t_section(section);
	std::string t_key(key);
	std::map< std::string, std::map< std::string, std::string > >::const_iterator it = _mapSectionKeyValue.find(t_section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map< std::string, std::string >::const_iterator iter = it->second.find(t_key);
		if (iter != it->second.end())
		{
			int intValue;
			std::istringstream(iter->second) >> intValue;
			return intValue;
		}
	}

	return notfound;
}

bool GSIni::GetBool(char const* section, char const* key, bool notfound) const
{
	if (NULL == section || NULL == key)
		return notfound;

	std::string t_section(section);
	std::string t_key(key);
	std::map< std::string, std::map< std::string, std::string > >::const_iterator it = _mapSectionKeyValue.find(t_section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map<std::string, std::string>::const_iterator iter = it->second.find(t_key);
		if (iter != it->second.end())
		{
			bool boolValue;
			if (iter->second.length() > 1)  // true/false 形式
				std::istringstream(iter->second) >> std::boolalpha >> boolValue;
			else  // 0/1 形式
				std::istringstream(iter->second) >> boolValue;
			return boolValue;
		}
	}

	return notfound;
}

double GSIni::GetDouble(char const* section, char const* key, double notfound) const
{
	if (NULL == section || NULL == key)
		return notfound;

	std::string t_section(section);
	std::string t_key(key);
	std::map< std::string, std::map< std::string, std::string > >::const_iterator it = _mapSectionKeyValue.find(t_section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map< std::string, std::string >::const_iterator iter = it->second.find(t_key);
		if (iter != it->second.end())
		{
			double doubleValue;
			std::istringstream(iter->second) >> doubleValue;
			return doubleValue;
		}
	}

	return notfound;
}

char const* GSIni::GetString(char const* section, char const* key, char const* notfound) const
{
	if (NULL == section || NULL == key)
		return notfound;

	std::string t_section(section);
	std::string t_key(key);
	std::map< std::string, std::map< std::string, std::string > >::const_iterator it = _mapSectionKeyValue.find(t_section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map<std::string, std::string>::const_iterator iter = it->second.find(t_key);
		if (iter != it->second.end())
		{
			return iter->second.c_str();
		}
	}

	return notfound;
}


ErrorCode GSIni::GetSection( const char * section, FunPtrGetKeyValueCallBack funcb, void * pUserData ) const
{
	GS_ASSERT_RET_VAL( section != NULL, INVALID_ARG )
	GS_ASSERT_RET_VAL( funcb != NULL, INVALID_ARG )

	std::map< std::string, std::map< std::string, std::string > >::const_iterator it = _mapSectionKeyValue.find( std::string( section ) );
	if( it == _mapSectionKeyValue.end() )
		return SECTION_NOT_FOUND;
	
	std::map< std::string, std::string >::const_iterator itKeyValue = ( it->second ).begin();
	std::map< std::string, std::string >::const_iterator itKeyValueEnd = ( it->second ).end();
	for( ; itKeyValue != itKeyValueEnd; itKeyValue++ )
		funcb( itKeyValue->first.c_str(), itKeyValue->second.c_str(), pUserData );

	return SUCCESS;
}

ErrorCode GSIni::Add(char const* section, char const* key, char const* value)
{
	if (NULL == section || NULL == key || NULL == value)
		return INVALID_ARG;

	//std::size_t sectionStartPos = _fileContent.find(section);
	std::size_t sectionStartPos = getSectionPosition(section);
	if (sectionStartPos != std::string::npos)
	{
		std::size_t sectionEndPos = _fileContent.find("<$$$>", sectionStartPos);
		//std::string temp = _fileContent.substr(sectionStartPos, sectionEndPos);
		//std::size_t keyPos = temp.find(key);
		std::size_t keyPos = getKeyPosition(key, sectionStartPos, sectionEndPos);
		if (keyPos != std::string::npos)
		{
			return KEY_EXIST;
		}
		_fileContent.insert(sectionEndPos, std::string(key) + " = " +std::string(value) + "\r\n");
	}
	else
	{
		_fileContent = _fileContent + "[" + std::string(section) + "]\r\n";
		_fileContent += (std::string(key) + " = " + std::string(value) + "\r\n");
		_fileContent += "<$$$>";
	}

	return addToMap(std::string(section), std::string(key), std::string(value));
}

ErrorCode GSIni::Add(char const* section, char const* key)
{
	if (NULL == section || NULL == key)
		return INVALID_ARG;

	//std::size_t sectionStartPos = _fileContent.find(section);
	std::size_t sectionStartPos = getSectionPosition(section);
	if (sectionStartPos != std::string::npos)
	{
		std::size_t sectionEndPos = _fileContent.find("<$$$>", sectionStartPos);
		//std::string temp = _fileContent.substr(sectionStartPos, sectionEndPos);
		//std::size_t keyPos = temp.find(key);
		std::size_t keyPos = getKeyPosition(key, sectionStartPos, sectionEndPos);
		if (keyPos != std::string::npos)
		{
			return KEY_EXIST;
		}
		_fileContent.insert(sectionEndPos, std::string(key) + "\r\n");
	}
	else
	{
		_fileContent = _fileContent + "[" + std::string(section) + "]\r\n";
		_fileContent += (std::string(key) + "=\r\n");
		_fileContent += "<$$$>";
	}

	return addToMap(std::string(section), std::string(key), "");
}

ErrorCode GSIni::Set(char const* section, char const* key, char const* value)
{
	if (NULL == section || NULL == key || NULL == value)
		return INVALID_ARG;

	//std::size_t sectionStartPos = _fileContent.find(section);
	std::size_t sectionStartPos = getSectionPosition(section);
	if (sectionStartPos != std::string::npos)
	{
		std::size_t sectionEndPos = _fileContent.find("<$$$>", sectionStartPos);
		//std::size_t keyPos = _fileContent.find(key, sectionStartPos);
		std::size_t keyPos = getKeyPosition(key, sectionStartPos, sectionEndPos);
		if (keyPos < sectionEndPos)
		{
			std::size_t valuePos = _fileContent.find("=", keyPos);
			std::size_t lineEnding = _fileContent.find_first_of("\r\n", valuePos);
			bool endFileFlag = false;
			if (lineEnding == std::string::npos)
			{
				lineEnding = _fileContent.size();
			}
			std::size_t valueEndPos = getNotePosition(_fileContent, valuePos);
			if (valueEndPos > lineEnding)
				valueEndPos = lineEnding;

			_fileContent.replace(valuePos + 1, (valueEndPos - 1) - valuePos, " " + std::string(value));

			if (endFileFlag)
				_fileContent += "<$$$>"; //上一步把标记擦除了，重新添加
		}
		else
			return KEY_NOT_FOUND;
	}
	else
	{
		return SECTION_NOT_FOUND;
	}

	return setToMap(std::string(section), std::string(key), std::string(value));
}

ErrorCode GSIni::DeleteKey(char const* section, char const* key)
{
	if (NULL == section || NULL == key)
		return INVALID_ARG;

	//std::size_t sectionStartPos = _fileContent.find(section);
	std::size_t sectionStartPos = getSectionPosition(section);
	if (sectionStartPos != std::string::npos)
	{
		std::size_t sectionEndPos = _fileContent.find("<$$$>", sectionStartPos);
		//std::size_t keyPos = _fileContent.find(key, sectionStartPos);
		std::size_t keyPos = getKeyPosition(key, sectionStartPos, sectionEndPos);
		if (keyPos < sectionEndPos)
		{
			std::size_t lineEnding = _fileContent.find("\n", keyPos);
			if (lineEnding != std::string::npos)
			{
				std::string lineStr = _fileContent.substr(keyPos, lineEnding - keyPos);
				std::size_t pairEnding = getNotePosition(lineStr);
				if (pairEnding == std::string::npos)
					pairEnding = lineEnding;

				_fileContent.erase(keyPos, pairEnding - keyPos);
			}
			else  //文件的最后一行可能没有'\n'
			{
				_fileContent.erase(keyPos, _fileContent.size());
				_fileContent += "<$$$>";  //上一步把标记擦除了，重新添加
			}
		}
		else
			return KEY_NOT_FOUND;
	}
	else
	{
		return SECTION_NOT_FOUND;
	}

	return deleteToMap(std::string(section), std::string(key));
}

ErrorCode GSIni::DeleteSection(char const* section)
{
	if (NULL == section)
		return INVALID_ARG;

	//std::size_t sectionStartPos = _fileContent.find(section);
	std::size_t sectionStartPos = getSectionPosition(section);
	if (sectionStartPos != std::string::npos)
	{
		std::size_t sectionEndPos = _fileContent.find("<$$$>", sectionStartPos);
		_fileContent.erase(sectionStartPos, (sectionEndPos + 5) - sectionStartPos); // +4是"<$$$>"的末尾位置
	}
	else
	{
		return SECTION_NOT_FOUND;
	}

	return deleteToMap(std::string(section));
}

ErrorCode GSIni::SaveToFile(char const* filename)
{
	// 解决vs2008下无法打开包含中文路径文件的问题
	std::fstream stream;

	std::locale oldLoc = std::locale::global(std::locale(""));

	std::string targetFile = NULL == filename ? _fileName : std::string(filename);

	stream.open(targetFile.c_str(), std::ios::trunc | std::ios::out);

	if (!stream.is_open())
	{
		std::locale::global(std::locale(oldLoc));
		return FILE_NOT_FOUND;
	}

	std::locale::global(std::locale(oldLoc));

	std::size_t sectionStartPos = 0;
	std::size_t sectionEndPos;
	std::string sectionStr;
	while ((sectionEndPos = _fileContent.find("<$$$>", sectionStartPos)) != std::string::npos)
	{
		sectionStr = _fileContent.substr(sectionStartPos, sectionEndPos - sectionStartPos);
		sectionStr = trimRight(sectionStr) +"\r\n";
		stream.write(sectionStr.data(), sectionStr.size());
		sectionStartPos = sectionEndPos + 5;
	}
	stream.flush();
	stream.close();

	return SUCCESS;
}

std::size_t GSIni::getNotePosition(const std::string& str, std::size_t pos)
{
	std::size_t semicolonPos = str.find(";", pos);   // ";"分号注释
	std::size_t poundPos = str.find("#", pos);		 // "#"井号注释
#if 1
	if( poundPos>0 && poundPos!=std::string::npos )
	{
		char c =  str[poundPos-1];
		
		if(strchr("\r\n \t;", c ) ||  c == '\0')
		{
			//是注释
		}
		else
		{
			for(pos = poundPos+1;;pos++)
			{
				c =  str[pos];
				if(strchr("\r\n \t;", c ) ||  c == '\0')
				{
					break;
				}
			}
			if(c=='\0')
			{
				poundPos = std::string::npos;
			}
			else
			{
				poundPos = str.find("#", pos);	
			}			
		}
	}
#endif

	return semicolonPos < poundPos ? semicolonPos : poundPos; //注释开始位置
}

std::size_t GSIni::getSectionPosition(const std::string& section, std::size_t pos)
{
	std::string mySection = "["+section+"]";    // modify by jsx   20160226

	std::size_t sectionPos = _fileContent.find(mySection, pos);
	if (std::string::npos == sectionPos)
		return std::string::npos;

	std::size_t lineStartPos = _fileContent.rfind("\n", sectionPos); // 找到上一行的结尾
	if (std::string::npos == lineStartPos)
		return sectionPos;

	std::string lineStr = _fileContent.substr(lineStartPos + 1, sectionPos - (lineStartPos + 1)); //行头到段名之间的字符串
	std::size_t notePos = getNotePosition(lineStr);
	if (std::string::npos == notePos)
		return sectionPos;

	return getSectionPosition(section, sectionPos + 1);
}

std::size_t GSIni::getKeyPosition(const std::string& key, std::size_t sPos, std::size_t ePos)
{
	if (sPos >= ePos)
		return std::string::npos;
	
	std::size_t keyPos = _fileContent.find(key, sPos);
	if (keyPos >= ePos)
		return std::string::npos;

	std::size_t lineStartPos = _fileContent.rfind("\n", keyPos); // 找到上一行的结尾
	if (std::string::npos == lineStartPos)
		return lineStartPos;

	std::string lineStr = _fileContent.substr(lineStartPos + 1, keyPos - (lineStartPos + 1)); //行头到键名之间的字符串
	std::size_t notePos = getNotePosition(lineStr);
	if (std::string::npos == notePos)
		return keyPos;

	return getKeyPosition(key, keyPos+1, ePos);
}

ErrorCode GSIni::addToMap(const std::string& section, const std::string& key, const std::string& value)
{
	std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map< std::string, std::string >::iterator iter = it->second.find(key);
		if (iter != it->second.end())
			return KEY_EXIST;
	}
	else
	{
		_mapSectionKeyValue[section] = std::map<std::string, std::string>();
	}

	_mapSectionKeyValue[section].insert(make_pair(key, value));

	return SUCCESS;
}

ErrorCode GSIni::setToMap(const std::string& section, const std::string& key, const std::string& value)
{
	std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map< std::string, std::string >::iterator iter = it->second.find(key);
		if (iter != it->second.end())
		{
			iter->second = value;
		}
		else
			return KEY_NOT_FOUND;
	}
	else
		return SECTION_NOT_FOUND;

	return SUCCESS;
}

ErrorCode GSIni::deleteToMap(const std::string& section, const std::string& key)
{
	std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(section);
	if (it != _mapSectionKeyValue.end())
	{
		std::map< std::string, std::string >::iterator iter = it->second.find(key);
		if (iter != it->second.end())
		{
			it->second.erase(key);
		}
		else
			return KEY_NOT_FOUND;
	}
	else
		return SECTION_NOT_FOUND;

	return SUCCESS;
}

ErrorCode GSIni::deleteToMap(const std::string& section)
{
	std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.find(section);
	if (it != _mapSectionKeyValue.end())
	{
		_mapSectionKeyValue.erase(section);
	}
	else
		return SECTION_NOT_FOUND;

	return SUCCESS;
}

std::string GSIni::trim(const std::string& str)
{
	return trimRight(trimLeft(str));
}

std::string GSIni::trimLeft(const std::string& str)
{
	std::size_t pos;
	std::string newStr = str;
	pos = newStr.find_first_not_of(" \t\n\r");  //空格、制表、换行、回车
	if (pos != std::string::npos)
		newStr.erase(0, pos);
	else
	    newStr = std::string("");

	return newStr;
}

std::string GSIni::trimRight(const std::string& str)
{
	std::size_t pos;
	std::string newStr = str;
	pos = newStr.find_last_not_of(" \t\n\r");   //空格、制表、换行、回车
	if (pos != std::string::npos)
		newStr.erase(pos + 1, newStr.size() - pos);
	else
	    newStr = std::string("");

	return newStr;
}

//void GSIni::PrintTest()
//{
//	std::map< std::string, std::map< std::string, std::string > >::iterator it = _mapSectionKeyValue.begin();
//	for (; it != _mapSectionKeyValue.end(); ++it)
//	{
//		std::cout << "[" << it->first << "]" << std::endl;
//		std::map< std::string, std::string > ::iterator iter = it->second.begin();
//		for (; iter != it->second.end(); ++iter)
//		{
//			std::cout << iter->first << " = " << iter->second << std::endl;
//		}
//		std::cout << std::endl;
//	}
//}
