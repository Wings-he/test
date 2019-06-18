/*******************************************************************************
  Copyright (C), 2014-2015, GOSUNCN 
  File name   : CommonXml.h      
  Author      : Banson      
  Version     : 2.0.0
  DateTime    : 2015/3/5
  Description : 通用xml配置与数据结构转换
*******************************************************************************/
#ifndef _COMMON_XML_H_
#define _COMMON_XML_H_

#include "xmlParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include "string"
#include "vector"
#include "map"
#include "set"
#include "list"
#include <fstream>
// #include "memory.h"
// 
// #ifdef _LINUX
// #include "tr1/memory"
// #include "tr1/shared_ptr.h"
// #endif
 using namespace std;
// using namespace tr1;
// using std::tr1::shared_ptr;


	/* StruNode的用法示例：

			StruNode	nodeReq("request");
			nodeReq["A"] = "a";
			nodeReq["A"]("B") = "b";
			nodeReq("A",1) = 1;
			nodeReq("A",2) = 0.5f;
			nodeReq["C"]["D"] = "cd";

			CCommonXmlConfig<> myParser;
			myParser.m_nodeList.push_back(nodeReq);
			strXml = myParser.ToStr();

	<==>	<?xml version="1.0" encoding="utf-8" ?> 
			<request>
				<A b="b">a</A>
				<A>1</A>
				<A>0.5</A>
				<C>
					<D>cd</D>
				</C>
			<request>

			CCommonXmlConfig<> myParser;
			Int32 nRet =  myParser.Parse(strXml);
			StruNode &nodeReq = myParser.m_nodeList.front();
	
			string a = nodeReq["A"].ToStr();
			string b = nodeReq["A"]("B").ToStr();
			int a1 = nodeReq("A",1,READONLY)->ToInt();
			float a2 = nodeReq("A",2,READONLY)->ToFloat();
			string cd = nodeReq["C"]["D"].ToStr();
	*/

namespace COMMON_XML
{
	#define		READONLY		999

	template<class valueT>
	class MyList:public vector<valueT>
	{
	public:
		//write & read by key
		valueT& operator[](const string& _Keyval)
		{
			for(typename vector<valueT>::iterator it = this->begin();it != this->end();it++)
			{
				if(it->name.compare(_Keyval) == 0)
					return *it;
			}
			valueT	tmpObj;
			tmpObj.name = _Keyval;
			this->push_back(tmpObj);
			return this->back();
		}
		//read by key
		valueT* operator()(const string& _Keyval)
		{
			for(typename vector<valueT>::iterator it = this->begin();it != this->end();it++)
			{
				if(it->name.compare(_Keyval) == 0)
					return &(*it);
			}
			return NULL;
		}
		//write & read by index
		valueT& operator[](unsigned int nIndex)
		{
			try
			{
				return this->at(nIndex);
			}
			catch (std::exception)
			{}

			while(this->size() < (nIndex + 1))
			{
				valueT	tmpObj;
				this->push_back(tmpObj);
			}
			return this->back();
		}

		//read by index
		valueT* operator()(unsigned int nIndex)
		{
			try
			{
				return &(this->at(nIndex));
			}
			catch (std::exception)
			{}
			return NULL;
		}

		//write & read by index start from the key
		valueT& operator()(const string& _Keyval, unsigned int nIndex)
		{
			unsigned int nOffset = 0;
			for(typename vector<valueT>::iterator it = this->begin();it != this->end();it++,nOffset++)
			{
				if(it->name.compare(_Keyval) == 0)
				{
					try
					{
						return this->at(nOffset + nIndex);
					}
					catch (std::exception)
					{
						break;
					}
				}
			}
			while(this->size() < (nOffset + nIndex + 1))
			{
				valueT	tmpObj;
				tmpObj.name = _Keyval;
				this->push_back(tmpObj);
			}
			return this->back();
		}

		//read only by index start from the key
		valueT* operator()(const string& _Keyval, unsigned int nIndex, int nIsRead)
		{
			unsigned int nOffset = 0;
			for(typename vector<valueT>::iterator it = this->begin();it != this->end();it++,nOffset++)
			{
				if(it->name.compare(_Keyval) == 0)
				{
					try
					{
						return &(this->at(nOffset + nIndex));
					}
					catch (std::exception)
					{
						break;
					}
				}
			}
			return NULL;
		}
	};

	struct		StruNode;
	typedef		MyList<StruNode>				MY_NODE_LIST;

	struct StruNode 
	{
		string					name;
		string					value;
		MY_NODE_LIST			child;
		MY_NODE_LIST			attributes;

		StruNode(){};
		StruNode(const string& _Keyval):name(_Keyval){};

		StruNode& operator=(const string& _Keyval)
		{
			value = _Keyval;
			return *this;
		}
		StruNode& operator=(int _Keyval)
		{
			stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator=(unsigned int _Keyval)
		{
			stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator=(float _Keyval)
		{
			std::stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator=(UInt64 _Keyval)
		{
			std::stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator=(Int64 _Keyval)
		{
			std::stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator=(double _Keyval)
		{
			std::stringstream strStream;    
			strStream << _Keyval;   
			value = strStream.str();
			return *this;
		}
		StruNode& operator[](const string& _name)
		{
			return child[_name];
		}
		StruNode& operator()(const string& _name)
		{
			return attributes[_name];
		}
		StruNode& operator()(const string& _Keyval, unsigned int nIndex)
		{
			return child(_Keyval,nIndex);
		}

		StruNode* operator()(const string& _Keyval, unsigned int nIndex, int nForRead)
		{
			return child(_Keyval,nIndex,nForRead);
		}

		StruNode* operator()(const string& _Keyval, unsigned int nIndex, int nForRead,int nForRead2)
		{
			return attributes(_Keyval,nIndex,nForRead);
		}
		string ToStr()
		{
			return value;
		}
		int ToInt()
		{
			int valueInt = 0;
			if(!value.empty())
			{
				std::stringstream strStream;
				strStream << value;
				strStream >> valueInt;   
			}
			return valueInt;
		}
		unsigned int ToUInt()
		{
			unsigned int valueInt = 0;
			if(!value.empty())
			{
				std::stringstream strStream;
				strStream << value;
				strStream >> valueInt;   
			}
			return valueInt;
		}
		UInt64 ToUInt64()
		{
			UInt64 valueInt = 0;
			if(!value.empty())
			{
				std::stringstream strStream;
				strStream << value;
				strStream >> valueInt;   
			}
			return valueInt;
		}
		Int64 ToInt64()
		{
			Int64 valueInt = 0;
			if(!value.empty())
			{
				std::stringstream strStream;
				strStream << value;
				strStream >> valueInt;   
			}
			return valueInt;
		}
		float ToFloat()
		{
			float valueFloat = 0;
			if(!value.empty())
			{
				std::stringstream strStream; 
				strStream << value;
				strStream >> valueFloat;   
			}
			return valueFloat;
		}

		double ToDouble()
		{
			double valueDouble = 0.0;
			if(!value.empty())
			{
				std::stringstream strStream; 
				strStream << value;
				strStream >> valueDouble;   
			}
			return valueDouble;
		}
	};

	/////////////////////////////////////////////////////////

	template<class valueT = char>
	class CCommonXmlConfig
	{
	public:
		CCommonXmlConfig(){};
		virtual ~CCommonXmlConfig(){};

		virtual		int				Parse(const std::string & content);
		virtual		std::string		ToStr(const std::string & fileName="",const std::string & strEncoding="utf-8",BOOL bNeedHeader = TRUE);

		virtual		int				Load(const std::string & configFile);//一定要绝对路径
		virtual		void			Save(const std::string & configFile);
		
		MY_NODE_LIST	m_nodeList;

	private:
		int							ParseInner(XMLNode &xmlNode, MY_NODE_LIST& node_list);
		void						ToStrInner(XMLNode &xmlNode, MY_NODE_LIST& node_list);
		bool						IsExistHeaderLine(const std::string strXml); 
		XMLNode::XMLCharEncoding    GetXMLEncoding(const string& strEncode);

	};

	template<class valueT /*= char*/>
	XMLNode::XMLCharEncoding COMMON_XML::CCommonXmlConfig<valueT>::GetXMLEncoding(const string& strEncode)
	{
		std::string t = strEncode;
		std::transform(t.begin(), t.end(), t.begin(), ::tolower);
		
		if (t == "gb2312")
		{
			return XMLNode::char_encoding_GB2312;
		}
		else if (t == "utf-8")
		{
			return XMLNode::char_encoding_UTF8;
		}
		else if (t == "gbk")
		{
			return XMLNode::char_encoding_GBK;
		}
		else
		{
			return XMLNode::char_encoding_UTF8;
		}
	}

	template<class valueT>
	bool CCommonXmlConfig<valueT>::IsExistHeaderLine(const std::string strXml) 
	{
		size_t pos = strXml.find_first_of("\r\n");
		if( pos	!= std::string::npos )
		{
			std::string strFirstLine = strXml.substr(0,pos);
			pos = strFirstLine.find("version");
			if( pos == std::string::npos )
				return false;
			pos = strFirstLine.find("encoding");
			if( pos == std::string::npos )
				return false;
		}
		else
		{
			return false;
		}

		return true;
	}


	template<class valueT>
	int CCommonXmlConfig<valueT>::Parse(const std::string & content)
	{
		XMLResults results;
		XMLNode top = XMLNode::parseString(content.c_str(),NULL,&results);

		if (results.error != eXMLErrorNone)
		{
			return -1;
		}

		bool bExistHead = IsExistHeaderLine(content);
		if( bExistHead )
		{
			top = top.getChildNode();
		}

		StruNode templateNodeMap;
		templateNodeMap.name = top.getName();
		const char* pValue = top.getText();
		templateNodeMap.value = pValue== NULL?"":pValue;

		ParseInner(top, templateNodeMap.child);

		int i = 0;
		while(1)
		{
			XMLAttribute attr = top.getAttribute(i++);
			if (attr.lpszName == NULL)
			{
				break;
			}
			StruNode tempAttr;
			tempAttr.name = attr.lpszName;
			tempAttr.value = attr.lpszValue;
			//if(tempAttr.value.empty())	tempAttr.value = "0";
			templateNodeMap.attributes.push_back(tempAttr);//填充属性
		}

		m_nodeList.push_back(templateNodeMap);

		return 0;
	}

	template<class valueT>
	int	CCommonXmlConfig<valueT>::ParseInner(XMLNode &xmlNode, MY_NODE_LIST& node_list)
	{
		if(xmlNode.isEmpty())
			return -1;
		//遍历
		int Index = 0;
		while(1)
		{
			XMLNode xmlNodeChild = xmlNode.getChildNode(Index++);//顶层根节点 
			if (xmlNodeChild.isEmpty())
			{
				//末节点
				break;
			}
			StruNode templateNodeMap;
			templateNodeMap.name = xmlNodeChild.getName();
			const char* pValue = xmlNodeChild.getText();
			templateNodeMap.value = pValue== NULL?"":pValue;

			int i = 0;
			while(1)
			{
				XMLAttribute attr = xmlNodeChild.getAttribute(i++);
				if (attr.lpszName == NULL)
				{
					break;
				}
				StruNode tempAttr;
				tempAttr.name = attr.lpszName;
				tempAttr.value = attr.lpszValue;
				//if(tempAttr.value.empty())	tempAttr.value = "0";
				templateNodeMap.attributes.push_back(tempAttr);//填充属性
			}

			//非末节点
			//前序遍历递归
			ParseInner(xmlNodeChild, templateNodeMap.child);

			node_list.push_back(templateNodeMap);
		}

		return 0;
	}

	template<class valueT>
	std::string  CCommonXmlConfig<valueT>::ToStr(const std::string & fileName/*=""*/, const std::string & strEncoding/*="utf-8"*/,BOOL bNeedHeader)
	{
		string xmlHeader("");
		
		if (bNeedHeader)
		{
			xmlHeader = "<?xml version='1.0' encoding='";
		xmlHeader = xmlHeader + strEncoding + "'?>\r\n";
		}
		
		XMLNode top;

		top.setGlobalOptions(GetXMLEncoding(strEncoding));

		ToStrInner(top,m_nodeList);

		char* pXml = top.createXMLString();

		xmlHeader += pXml;

		freeXMLString(pXml);

		if(!fileName.empty())
		{
			setlocale(LC_ALL,"Chinese-simplified");// 设置中文环境
			ofstream out(fileName.c_str());  
			out << xmlHeader; 
			setlocale(LC_ALL,"C");// 还原 
		}
		return xmlHeader;
	}

	template<class valueT>
	void CCommonXmlConfig<valueT>::ToStrInner(XMLNode &xmlNode, MY_NODE_LIST& node_list)
	{
		for(MY_NODE_LIST::iterator basenode = node_list.begin();basenode!= node_list.end();basenode++)
		{
			StruNode & nodeData = *basenode;
			if(nodeData.child.empty())//末节点
			{
				XMLNode childNode;
				if (xmlNode.isEmpty())//根结点 
				{
					xmlNode = XMLNode::createXMLTopNode(nodeData.name.c_str());
					xmlNode.addText(nodeData.value.c_str());
					childNode = xmlNode;
				}
				else
				{
					childNode = xmlNode.addChild(nodeData.name.c_str());
					childNode.addText(nodeData.value.c_str());
				}

				for (MY_NODE_LIST::iterator templeAttri = nodeData.attributes.begin();templeAttri!= nodeData.attributes.end();templeAttri++)
				{
					childNode.addAttribute(templeAttri->name.c_str(),templeAttri->value.c_str());
				}
			}
			else//非末节点
			{
				XMLNode childNode;
				if (xmlNode.isEmpty()) //根结点 
				{
					xmlNode = XMLNode::createXMLTopNode(nodeData.name.c_str());
					childNode = xmlNode;
				}
				else  //子结点
				{
					childNode = xmlNode.addChild(nodeData.name.c_str());
				}	

				for (MY_NODE_LIST::iterator templeAttri = nodeData.attributes.begin();templeAttri!= nodeData.attributes.end();templeAttri++)
				{
					childNode.addAttribute(templeAttri->name.c_str(),templeAttri->value.c_str());
				}

				ToStrInner(childNode,nodeData.child);//递归
			}

		}
	}

	template<class valueT>
	int	CCommonXmlConfig<valueT>::Load(const std::string & configFile)
	{
		std::string strContent;

		setlocale(LC_ALL, "Chinese-simplified"); // 设置中文环境
#ifdef _UNICODE
		USES_CONVERSION;
		XMLNode top =  XMLNode::parseFile(W2A(configFile.c_str()));
#else
		XMLNode top =  XMLNode::parseFile(configFile.c_str());
#endif
		setlocale(LC_ALL, "C"); // 还原
		if (top.isEmpty())
		{
			return -1;
		}
		return ParseInner(top,m_nodeList);
	}

	template<class valueT>
	void CCommonXmlConfig<valueT>::Save(const std::string & configFile)
	{
		ToStr(configFile);
	}
}
#endif