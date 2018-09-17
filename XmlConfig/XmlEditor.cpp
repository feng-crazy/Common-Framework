/******************************************************************************
版权所有：	深圳市理邦精密仪器有限公司
项目名称：	Mx
版本号：		1.0
文件名：		XmlEditor.h
生成日期：	2009-03-18
作者：		岳红
功能说明：
******************************************************************************/
#include "XmlEditor.h"
#include "Utility/DebugCtrl.h"
#include "Utility/Misc.h"
#include "XmlFileManager.h"
#include <cstdlib>

// TODO: 以下的依赖文件用于优化！
#include "ComFuncs/FunctionEntries.h"


std::string XmlEditor::_SEPARATOR = ":";


/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	保存配置文件，该方法在数据保存线程中调用，一般程序不要轻易调用！
参数说明:
返回值:
	// 调用该函数时返回值为0，则说明不需要再进行保存，因此这里对xml文件没有或者文件保存成功两种情形进行区分。
	0:没有xml文件或保存成功文件；
	1:没有新数据需要更新，不需要保存文件；
	2:锁住，不保存文件直接返回；
	3:保存文件失败。
******************************************************************************/
int XmlEditor::save_file(void)
{
	if(NULL == _xml_doc)
	{
		return SAVE_SUCCESS;
	}

	_mutex.lock();

	// 没有新数据需要更新。
	if(_write_pool.empty() && _write_attr_pool.empty())
	{
		_mutex.unlock();
		return SAVE_NO_NEED;
	}

	// 锁住，不保存文件直接返回。
	if(_locked_save == true)
	{
		_mutex.unlock();
		return SAVE_NO_NEED_LOCK;
	}

	// TODO:先解锁，再加锁，保存配置文件费时。
	_mutex.unlock();

	// 保存之前先备份该文件。
	string backup_fname;
	XmlFileManager &xfm = *XmlFileManagerSingleton::instance();
	backup_fname = _file_name + ".temp";
	xfm.copy_file(backup_fname, _file_name);

	// TODO:重新加锁
	_mutex.lock();
	// 锁住，不保存文件直接返回。
	if(_locked_save == true)
	{
		_mutex.unlock();
		return SAVE_NO_NEED_LOCK;
	}

	std::string path;
	std::string attribute;
	bool re = false;

	// 将_write_pool中的节点值写进配置文件。
	XmlEditorMap::iterator it = _write_pool.begin();
	for(; it!=_write_pool.end(); ++it)
	{
		re = _write_val((*it).first, (*it).second);
		if(re == false)
		{
			debug("Save %s:%s failed!\n",
					(*it).first.c_str(), (*it).second.c_str());
		}
	}
	_write_pool.clear();

	// 将_write_attr_pool中的节点属性值写进配置文件。
	it = _write_attr_pool.begin();
	for(; it!=_write_attr_pool.end(); ++it)
	{
		 ::distill_string((*it).first, path, _SEPARATOR, 0);
		 ::distill_string((*it).first, attribute, _SEPARATOR, 1);
		re = _write_attr(path, attribute, (*it).second);

		if(re == false)
		{
			debug("Save %s %s:%s failed!\n",
					path.c_str(), attribute.c_str(), (*it).second.c_str());
		}
	}
	_write_attr_pool.clear();
	_mutex.unlock();


	// TODO: 优化！暂停网络部件。在保存配置文件的时候，由于XML需要很多CPU资源，主
	// 线程产生的数据减少，导致网络部件需要发往中央站的数据减少，经过插值以后使得波
	// 形拉伸、变形。为此，在保存配置时暂停网络部件。
	pause_network();

	// 开始将配置文件写入磁盘。
	re = _xml_doc->SaveFile(_file_name.c_str());
	if(re == false)
	{
		debug("Save %s failed\n", _file_name.c_str());
		return SAVE_FAILED;
	}

	// 保存成功删除部分的配置文件。
	xfm.delete_file(backup_fname);
	return SAVE_SUCCESS;
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	插入根节点
参数说明:
	根节点名称
返回值:
	成功返回true
******************************************************************************/
bool XmlEditor::set_rootelm(const std::string &root_str)
{
	TiXmlElement RootElement(root_str.c_str());

	IF(NULL == _xml_doc)
	{
		return false;
	}

	_mutex.lock();

	//把根节点插入当前文档
	_xml_doc->InsertEndChild(RootElement);
	bool re = _xml_doc->SaveFile(_file_name.c_str());

	_mutex.unlock();
	return re;
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	读取参数所表示的节点的值
参数说明:
	index_str:节点字符串
返回值:
	成功返回节点值，失败返回空字符
******************************************************************************/
bool XmlEditor::get_val(const std::string &index_str, std::string &result)
{
	IF(NULL == _xml_doc)
	{
		return false;
	}

	TiXmlElement *pElement = NULL; // 一个指向Element的指针

	_mutex.lock();
	XmlEditorMap::iterator it = _read_pool.find(index_str);

	if(it != _read_pool.end())
	{
		result = it->second;// use iterator to reduce the "find" operation.

		_mutex.unlock();
		return true;
	}

	pElement = _xml_doc->RootElement();	//获得根元素
	IF(NULL == pElement)
	{
		debug("%s: get_val(): error! \n", index_str.c_str());

		_mutex.unlock();
		return false;
	}

	std::string dest;
	for(unsigned index=0; ; index++)
	{
		if(::distill_string(index_str, dest, "|", index) == true)
		{
			pElement = pElement->FirstChildElement(dest.c_str());

			if(pElement == NULL)
			{
				_mutex.unlock();
				return false;
			}
		}
		else
		{
			// 吴垠, 2009-09-24
			// 如果待取得的元素是个根节点，比如AppConfig.xml中的DarkFactor，则会
			// 直接用字符串"DarkFactor"来取索引值，不会再加上一个分割字符串
			// "DarkFactor|"，此时需要执行下面这段代码。
			if(0 == index)
			{
				pElement = pElement->FirstChildElement(index_str.c_str());

				if(pElement == NULL)
				{
					debug("pElement->FirstChildElement is failed! %s\n",
							index_str.c_str());
					_mutex.unlock();
					return false;
				}
			}
			break;
		}
	}


	// Wu Yin, 2009-06-23
	// TinyXML有个致命的缺点：
	// 如果xml文件中某元素的值为空的话，其读取的时候程序就会崩溃。
	// 这里增加一层保护，如果xml元素为空的话则返回一个警告字符串。
	if(pElement->NoChildren() == true)
	{
		debug("%s:%s is Empty!! \n", _file_name.c_str(), index_str.c_str());

		_mutex.unlock();
		return false;
	}

	const char *str = NULL;
	// 获取节点值
	str = pElement->GetText();
	if(str == NULL)
	{
		_mutex.unlock();
		return false;
	}
	result = str;
	_read_pool.insert(XmlEditorPair(index_str, result));

	_mutex.unlock();
	return true;
}



/******************************************************************************
作者: 张虎, 2009.11.24
功能描述:
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::_set_val(const std::string &index_str, const std::string &value)
{
	XmlEditorMap::iterator it = _read_pool.find(index_str);

	if(it != _read_pool.end())
	{
		if((*it).second == value)
		{
			return false;
		}
		else
		{
			(*it).second = value;
		}
	}
	else
	{
		// 如果在_read_pool中没有读到这个数据,则在将数据写入磁盘之前需要同时将这个
		// 数据插入_read_pool。如果出现还没有将数据写入磁盘，而又马上发生读操作，此
		// 时读出的数据会是上一次磁盘中的数据了，这是错误的了。 官辉.2009.12.9
		_read_pool.insert(XmlEditorPair(index_str, value));
	}

	if(_read_pool.size() > 1030)
	{
		debug("Too many date in _read_pool! %d\n", _read_pool.size());
	}

	if((it = _write_pool.find(index_str)) != _write_pool.end())
	{
		(*it).second = value;
	}
	else
	{
		_write_pool.insert(XmlEditorPair(index_str, value));
	}

	return true;
}



/******************************************************************************
作者: 张虎, 2010.01.20
功能描述:
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::set_val(const std::string &index_str, const std::string &value)
{
	_mutex.lock();
	bool ret = _set_val(index_str, value);
	_mutex.unlock();
	return ret;
}



/******************************************************************************
作者: 张虎, 2009.11.24
功能描述:
参数说明:
返回值:
******************************************************************************/
void XmlEditor::set_val(const std::vector<std::string> &path,
		const std::vector<std::string> &value)
{
	// 要求一一配对，如果不等则不处理。
	if(path.size() != value.size())
	{
		debug("Invalid!\n");
		return;
	}

	_mutex.lock();

	// Refresh read pool.
	XmlEditorMap::iterator it;

	for(unsigned index=0; index<path.size(); index++)
	{
		//_write_pool.insert(XmlEditorPair(path[index], value[index]));
		_write_pool[path[index]] = value[index];

		it = _read_pool.find(path[index]);
		if(it != _read_pool.end())
		{
			(*it).second = value[index];
		}
	}

	_mutex.unlock();
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	修改或插入参数所表示的节点的值
参数说明:
	index_str:节点字符串
返回值:
******************************************************************************/
bool XmlEditor::get_attribute(const std::string &index_str,
		const std::string &attribute, std::string &result)
{
	IF(NULL == _xml_doc)
	{
		return false;
	}

	// 加锁保护。
	_mutex.lock();

	// 首先查看_read_attr_pool中是否有该属性值了，如果存在则直接返回。
	// 注意：使用了路径加属性作为键值。
	XmlEditorMap::iterator it;
	it = _read_attr_pool.find(index_str + _SEPARATOR + attribute);
	if(it != _read_attr_pool.end())
	{
		result = it->second;

		_mutex.unlock();
		return true;
	}

	// 在_read_attr_pool中没有需要选项则从tinyXML中解析出来。
	int str_len = index_str.length();
	char strx[str_len + 1];
	memset(strx, '\0', str_len + 1);
	index_str.copy(strx, str_len);      // 将字符串常量复制为一个字符数组。

	TiXmlElement *pElement = NULL;      // 一个指向Element的指针。
	char *pch = NULL;

	pElement = _xml_doc->RootElement();	//获得根节点。
	IF(NULL == pElement)
	{
		debug("%s : get_attribute(): error! \n", index_str.c_str());

		_mutex.unlock();
		return false;
	}

	// 用"｜"把字符串分离开来
	pch = strtok(strx, "|");
	while (pch != NULL)
	{
		// 自根节点以下找到每一个下级节点
		pElement = pElement->FirstChildElement(pch);
		// 如果该节点不存在，打印出错信息
		if(pElement == NULL)
		{
		//	debug("%s : Parameter invalid! \n", index_str.c_str());

			_mutex.unlock();
			return false;
		}
		pch = strtok(NULL, "|");
	}

	// 获取节点的指定属性值
	// 但是如果没有找到指定的属性值，此时直接赋值给 result，会有段错误。
	// 用 *str 中转一下，并做判断。
	const char *str = pElement->Attribute(attribute.c_str());
	if(NULL == str)
	{
		//debug("%s->%s : get_attribute() error:  \n",
//				index_str.c_str(), attribute.c_str());

		_mutex.unlock();
		return false;
	}

	result = str;
	_read_attr_pool.insert(XmlEditorPair(index_str + _SEPARATOR + attribute,
			result));

	_mutex.unlock();
	return true;
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	修改或插入参数所表示的节点的值
参数说明:
	index_str:节点字符串
返回值:
	成功返回true,失败返回false
******************************************************************************/
bool XmlEditor::set_attribute(const std::string &index_str,
		const std::string &attribute, const std::string &value)
{
	_mutex.lock();
	bool ret = _set_attr(index_str, attribute, value);
	_mutex.unlock();
	return ret;
}



/******************************************************************************
作者: Wu Yin, 2009.04.09
功能描述:
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::get_next(std::string &element_str, std::string &element_val)
{
	_mutex.lock();

	_elm = _elm->NextSiblingElement();
	if(NULL == _elm)
	{
		_mutex.unlock();
		return false;
	}

	element_str = _elm->Value();
	// Wu Yin, 2010-11-11
	// TinyXML有个致命的缺点：
	// 如果xml文件中某元素的值为空的话，其读取的时候程序就会崩溃。
	// 这里增加一层保护，如果xml元素为空的话则返回一个警告字符串。
	if(_elm->NoChildren())
	{
		warning("%s:%s is Empty!! \n", _file_name.c_str(), element_str.c_str());
		_mutex.unlock();
		return false;
	}
	element_val = _elm->FirstChild()->Value();

	_mutex.unlock();
	return true;
}



/******************************************************************************
作者: Wu Yin, 2009.04.09
功能描述:
	取得指定索引处的第一个“元素”和“元素值”
参数说明:
	index_str: 索引字符串
	element_str: 返回元素名
	element_val: 返回元素值
返回值:
	是否读取成功
******************************************************************************/
bool XmlEditor::get_first(const std::string &index_str,
		std::string &element_str, std::string &element_val)
{
	IF(NULL == _xml_doc)
	{
		return false;
	}

	int str_len = index_str.length();
	char strx[str_len + 1];
	memset(strx, '\0', str_len + 1);

	char *pch = NULL;
	TiXmlElement *pElement = NULL; // 一个指向Element的指针

	_mutex.lock();

	//将字符串常量复制为一个字符数组
	index_str.copy(strx, str_len);

	pElement = _xml_doc->RootElement();	//获得根元素
	IF(NULL == pElement)
	{
		debug("%s : get_first(): error! \n", index_str.c_str());

		_mutex.unlock();
		return false;
	}

	//用“｜”把字符串分离开来
	pch = strtok(strx, "|");
	while(pch != NULL)
	{
		//自根节点以下找到每一个下级节点
		pElement = pElement->FirstChildElement(pch);
		//如果该节点不存在，打印出错信息
		if (pElement == NULL)
		{
			_mutex.unlock();
			return false;
		}
		pch = strtok(NULL, "|");
	}
	_elm = pElement->FirstChildElement();
	if(NULL == _elm)
	{
		debug("%s : get_first(): error! \n", index_str.c_str());

		_mutex.unlock();
		return false;
	}

	element_str = _elm->Value();
	element_val = _elm->FirstChild()->Value();

	_mutex.unlock();
	return true;
}



/******************************************************************************
作者: 官辉, 2009.12.15
功能描述:
	检测指定的文件是否为XML格式的文件。
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::_is_xml_file(const std::string &fname)
{
	if(access(fname.c_str(), F_OK) == -1)
	{
		return false;
	}

	TiXmlDocument *temp_xml_doc = new TiXmlDocument(fname.c_str());

	if (temp_xml_doc == NULL)
	{
		debug("new xml file failed: %s\n", fname.c_str());
		return false;
	}

	bool is_xmlfile = temp_xml_doc->LoadFile();

	delete temp_xml_doc;

	return is_xmlfile;
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	修改或插入参数所表示的节点的值，该方法在调用方法中已经被锁住，这里不再加锁。
参数说明:
	index_str:节点字符串
返回值:
	成功返回true,失败返回false
******************************************************************************/
bool XmlEditor::_write_val(const std::string &index_str, const std::string &value)
{
	IF(NULL == _xml_doc)
	{
		debug("No XML_DOC!");
		return false;
	}

	TiXmlElement *pElement = NULL;
	std::string dest;

	pElement = _xml_doc->RootElement();	//获得根元素
	IF(pElement == NULL)
	{
		debug("%s: get_val(): error! \n", index_str.c_str());
		return false;
	}

	for(unsigned index=0; ; index++)
	{
		if(::distill_string(index_str, dest, "|", index) == true)
		{
			pElement = pElement->FirstChildElement(dest.c_str());

			if(pElement == NULL)
			{
				debug("%s has no child.\n", dest.c_str());
				return false;
			}
		}
		else
		{
			if(0 == index)
			{
				pElement = pElement->FirstChildElement(index_str.c_str());

				if(pElement == NULL)
				{
					debug("pElement->FirstChildElement is failed! %s\n",
							index_str.c_str());
					return false;
				}
			}

			break;
		}
	}

	TiXmlNode *pNode = pElement->FirstChild();
	if(pNode == NULL)
	{
		debug("%s has no Node!\n", pElement->Value());
		return false;
	}

	pNode->SetValue(value);
	return true;
}



/******************************************************************************
作者: 张虎, 2010.05.05
功能描述:
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::_set_attr(const std::string &index_str,
		const std::string &attribute, const std::string &value)
{
	XmlEditorMap::iterator it;
	it = _read_attr_pool.find(index_str + _SEPARATOR + attribute);

	if(it != _read_attr_pool.end())
	{
		if((*it).second == value)
		{
			return false;
		}
		else
		{
			(*it).second = value;
		}
	}
	else
	{
		// 如果在_read_attr_pool中没有读到这个数据,则在将数据写入磁盘之前需要同时
		// 将这个数据插入_read_pool.因为不能排除还没有将数据写入磁盘,而又很快的发
		// 生读操作，此时读出的数据会是上一次磁盘中的数据了，这是错误的了。
		_read_attr_pool.insert(XmlEditorPair(index_str +
				_SEPARATOR + attribute, value));
	}

	if(_read_attr_pool.size() > 1030)
	{
		debug("Too many date in _read_pool! %d\n", _read_attr_pool.size());
	}

	if((it = _write_attr_pool.find(index_str + _SEPARATOR + attribute)) !=
			_write_attr_pool.end())
	{
		(*it).second = value;
	}
	else
	{
		_write_attr_pool.insert(XmlEditorPair(index_str +
				_SEPARATOR + attribute, value));
	}

	return true;
}



/******************************************************************************
作者: 张虎, 2010.05.05
功能描述:
参数说明:
返回值:
******************************************************************************/
bool XmlEditor::_write_attr(const std::string &index_str,
		const std::string &attribute, const std::string &value)
{
	IF(NULL == _xml_doc)
	{
		return false;
	}

	int str_len = index_str.length();
	char strx[str_len + 1];
	memset(strx, '\0', str_len + 1);
	index_str.copy(strx, str_len);     // 将字符串常量复制为一个字符数组

	char *pch = NULL;
	TiXmlElement *pElement = NULL;

	pElement = _xml_doc->RootElement(); // 获得根元素。
	IF(NULL == pElement)
	{
		debug("%s : set_attribute(): error! \n", index_str.c_str());
		return false;
	}

	pch = strtok(strx, "|");
	while (pch != NULL)
	{
		// 自根节点以下找到每一个下级节点如果对应名称的子节点不存在，则创建该节点。
		if((pElement->FirstChildElement(pch)) == NULL)
		{
			TiXmlElement pElm(pch);
			pElement->InsertEndChild(pElm);// 在当前节点下插入子节点。
		}
		pElement = pElement->FirstChildElement(pch);
		IF(NULL == pElement)
		{
			debug("%s : set_attribute(): error! \n", index_str.c_str());
			return false;
		}
		pch = strtok(NULL, "|");
	}

	pElement->SetAttribute(attribute.c_str(), value.c_str());
	return true;
}



/******************************************************************************
作者: 张虎, 2010.04.28
功能描述:
	将pchild的所有父对象的Value按照"xx|xx"的形式组织起来，并有path带回。
参数说明:
返回值:
******************************************************************************/
void XmlEditor::_replace_attr(const TiXmlNode &root, const TiXmlElement &pelement)
{
	std::string path;
	const TiXmlAttribute *pattr = pelement.FirstAttribute();
	if(pattr != NULL)
	{
		path = pelement.Value();
		_build_prefix(root, pelement, path);
		path += _SEPARATOR;

		_attr_pool.insert(XmlEditorPair(path + pattr->Name(),
				pattr->Value()));

		while((pattr = pattr->Next()) != NULL)
		{
			_attr_pool.insert(XmlEditorPair(path + pattr->Name(),
					pattr->Value()));
		}
	}
}



/******************************************************************************
作者: 张虎, 2010.04.28
功能描述:
	将pchild的所有父对象的Value按照"xx|xx"的形式组织起来，并有path带回。
参数说明:
返回值:
******************************************************************************/
void XmlEditor::_build_prefix(const TiXmlNode &root,
		const TiXmlNode &pchild, std::string &path)
{
	const TiXmlNode *parent = &pchild;
	while((parent->Parent()) && (parent->Parent() != &root))
	{
		parent = parent->Parent();

		if(path.empty())
		{
			path += parent->Value();
		}
		else
		{
			path.insert(0, "|");
			path.insert(0, parent->Value());
		}
	}
}


/******************************************************************************
作者: 张虎, 2010.01.10
功能描述:
参数说明:
返回值:
******************************************************************************/
void XmlEditor::_traverse(const TiXmlDocument &doc, const TiXmlNode &sector)
{
	const TiXmlNode *pchild = sector.FirstChild();
	const TiXmlNode *root = doc.RootElement();
	if((root == NULL) || (pchild == NULL))
	{
		return;
	}

	const TiXmlElement *pelement = NULL;
	while(pchild)
	{
		int type = pchild->Type();
		if(type == TiXmlNode::ELEMENT)
		{
			pelement = dynamic_cast<const TiXmlElement*>(pchild);
			_replace_attr(*root, *pelement);
			_traverse(doc, *pchild); // 递归调用。
		}
		else if(type == TiXmlNode::TEXT)
		{
			std::string path;

			_build_prefix(*root, *pchild, path);
			_temporary_pool.insert(XmlEditorPair(path, pchild->Value()));
		}

		pchild = pchild->NextSibling();
	}
}



/******************************************************************************
作者: 张虎, 2010.01.10
功能描述:
参数说明:
返回值:
******************************************************************************/
void XmlEditor::replace_sector(XmlEditor &src, const std::string &sector)
{
	IF(this == &src)
	{
		debug("Repalced with self!\n");
		return;
	}

	const TiXmlDocument *doc = src.get_xmldoc();
	IF(doc == NULL)
	{
		return;
	}

	// 以下开始遍历出正确的节点。
	const TiXmlElement *pElement = doc->RootElement();	// 获得根元素
	IF(NULL == pElement)
	{
		debug("%s: get_val(): error! \n", sector.c_str());
		return;
	}

	// 如果传入的不是根元素。
	if(sector != pElement->Value())
	{
		std::string dest;
		for(unsigned index=0; ; index++)
		{
			if(::distill_string(sector, dest, "|", index) == true)
			{
				pElement = pElement->FirstChildElement(dest.c_str());

				if(pElement == NULL)
				{
					debug("pElement->FirstChildElement is failed! %s\n",
							sector.c_str());
					return;
				}
			}
			else
			{
				if(0 == index)
				{
					pElement = pElement->FirstChildElement(sector.c_str());

					if(pElement == NULL)
					{
						debug("pElement->FirstChildElement is failed! %s\n",
								sector.c_str());
						return;
					}
				}
				break;
			}
		}
	}

	// _traverse将指定节点的所有子节点的值和属性分别遍历到_temporary_pool和
	// _attr_pool中。
	_attr_pool.clear();
	_temporary_pool.clear();
	_replace_attr(*doc->RootElement(), *pElement); // 替换根节点的属性。
	_traverse(*doc, *pElement);

	// 将新值从_temporary_pool中赋值到本对象，注意_set_val只是把新值放到读写缓冲区
	// 中，而没有直接写到tinyXML中。
	_mutex.lock();
	XmlEditorMap::iterator it = _temporary_pool.begin();
	for(; it!=_temporary_pool.end(); ++it)
	{
		_set_val((*it).first, (*it).second);
	}

	// 将新的属性数据也更新，_set_attr也是将新属性写到读写缓冲区中。
	std::string path, attr;
	it = _attr_pool.begin();
	for(; it!=_attr_pool.end(); ++it)
	{
		::distill_string((*it).first, path, _SEPARATOR, 0);
		::distill_string((*it).first, attr, _SEPARATOR, 1);
		_set_attr(path,	attr, (*it).second);
	}
	_mutex.unlock();
}



/******************************************************************************
作者: 张虎, 2010.01.21
功能描述:
参数说明:
返回值:
******************************************************************************/
void XmlEditor::locked_save(bool value)
{
	_mutex.lock();
	_locked_save = value;
	_mutex.unlock();
}



/******************************************************************************
作者: 岳红, 2009.03.24
功能描述:
	构造函数
参数说明:
	fname: xml文件绝对路径名。
	flag: 作为返回值表示是否存在配置文件。
返回值:
	失败返回NULL。
MODIFICATION: 官辉.2009.12.1.
	如果文件不存在，从/etc/edan/目录下拷贝一个默认目录过来。
	官辉. 2009.12.15.增加.temp文件的恢复配置功能。
******************************************************************************/
XmlEditor::XmlEditor(const std::string &fname, const std::string &root_str)
{
	bool res;
	_file_name = fname;
	_locked_save = false;
	XmlFileManager &xfm = *XmlFileManagerSingleton::instance();

	bool file_ok = false;

	if(access(fname.c_str(), F_OK) == 0)
	{
		file_ok = true;
	}
	else
	{
		debug("File not exist!: %s \n", fname.c_str());
		// 如果文件不存在,从/etc/edan/拷贝一个厂家默认的过来
		file_ok = xfm.copy_from_backup_file(fname);
	}

	string backup_fname = fname+".temp";
	if(access(backup_fname.c_str(), F_OK) == 0)
	{
		// 若存在有效的.temp备份文件则尝试恢复配置.
		if(_is_xml_file(backup_fname))
		{
			debug("copy from OK backup file:%s ----> %s\n",
					backup_fname.c_str(), fname.c_str());
			file_ok = xfm.copy_file(fname, backup_fname);
			xfm.delete_file(backup_fname);
		}
		else // 损坏的.temp备份文件则删除之
		{
			debug("exist backup file, but broken,delete it!:%s\n",
					backup_fname.c_str());
			xfm.delete_file(backup_fname);
		}
	}

	if(file_ok)
	{
		_xml_doc = new TiXmlDocument(fname.c_str());

		// 加载配置文件
		res = _xml_doc->LoadFile();
		if(!res)
		{
			debug("The file \"%s\" : is not XML format!!\n", fname.c_str());
			xfm.copy_from_backup_file(fname);// 拷贝文件.
			_xml_doc->LoadFile();
		}
	}
	else
	{
		debug("%s : File not exist! and can't be copied from backup file!\n",
				fname.c_str());
		_xml_doc = NULL;
	}
}



/******************************************************************************
作者: 岳红, 2009.03.18
功能描述:
参数说明:
返回值:
******************************************************************************/
XmlEditor::~XmlEditor()
{
	delete _xml_doc;
}

