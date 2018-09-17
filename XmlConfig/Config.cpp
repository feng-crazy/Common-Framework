/******************************************************************************
	  COPYRIGHT: 2008 Edan Instruments,	Inc. All rights	reserved
	PROJECTNAME: Mx
		VERSION: 1.0
	   FILENAME: Config.cpp
		CREATED: 2009.03.04
		 AUTHOR: Yue Hong
	DESCRIPTION:
******************************************************************************/
#include "Config.h"



SyncDisk *Config::_sync_disk = NULL;


/******************************************************************************
AUTHOR: ZhangHu, 2008.11.10
DESCRIPTION:
PARAMETERS:
	index_str: 索引字符串
	result: 用来存放返回值
RETURN:
******************************************************************************/
bool Config::get_string_value(const string &index_str, string &result)
{
	IF(_xml_editor == NULL)
	{
		return false;
	}

	string temp;
	if(!_xml_editor->get_val(index_str, temp))
	{
//		debug("%s\t%s : error! \n", name.c_str(), index_str.c_str());
		return false;
	}
	result = temp;
	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.09.01
DESCRIPTION:
	读取类似下面这种格式的配置信息：
	-----------------------------------------------------
		<DateFormat CurrentOption="2" OptionCount="3">
			<Opt0>%Y-%m-%d %H:%M:%S</Opt0>
			<Opt1>%m-%d-%Y %H:%M:%S</Opt1>
			<Opt2>%d-%m-%Y %H:%M:%S</Opt2>
		</DateFormat>
	-----------------------------------------------------
	TODO: 目前只能读取字符串型的值。但是目前来看也只有这一种用途，数值型的不会用
	这种方式来保存。
PARAMETERS:
	index_str: 索引字符串
	result: 用来存放返回值
RETURN:
	是否读取成功
******************************************************************************/
bool Config::get_current_value(const string &index_str, string &result)
{
	int current_opt = 0;
	string index("0");

	// 读取当前选项
	if(!get_numerical_attribute(index_str, "CurrentOption", current_opt))
	{
		//warning("%s: read error 1! \n", index_str.c_str());
		return false;
	}

	to_string(current_opt, index);

	// 按照当前选项来读取实际值
	if(!get_string_value(index_str+"|Opt"+index, result))
	{
		warning("%s: read error 2! \n", index_str.c_str());
		return false;
	}

	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.11.09
DESCRIPTION:
	读取Current型的配置信息，读取当前选中的索引值
PARAMETERS:
	index_str: 索引字符串
	index: 用来返回索引值
RETURN:
	是否读取成功
******************************************************************************/
bool Config::get_current_index(const string &index_str, int &index)
{
	int current_opt = 0;

	// 读取当前选项
	if(!get_numerical_attribute(index_str, "CurrentOption", current_opt))
	{
		warning("%s: read error 1! \n", index_str.c_str());
		return false;
	}
	index = current_opt;

	return true;
}

/******************************************************************************
AUTHOR: zhoucheng
DESCRIPTION:
设置当前索引对应的值
PARAMETERS:

RETURN:
	是设置取成功
******************************************************************************/
bool Config::set_current_index_value(const string &index_str, const string &value,int ind)
{
	int count = 0;
	string ret, index;

	// 确认有多少个待读取的字段
	if(!get_numerical_attribute(index_str, "OptionCount", count))
	{
		warning("%s: read error! \n", index_str.c_str());
		return false;
	}
	if(ind<count)
	{
		to_string(ind, index);
		set_string_value(index_str+"|Opt"+index,value);
	}
	return true;
}

bool Config::get_current_list(const string &index_str, vector<string> &result)
{
	int count = 0;
	string ret, index;

	// 确认有多少个待读取的字段
	if(!get_numerical_attribute(index_str, "OptionCount", count))
	{
		warning("%s: read error! \n", index_str.c_str());
		return false;
	}

	// 循环读取所有字段
	for(int i = 0; i < count; i++)
	{
		to_string(i, index);
		if(!get_string_value(index_str+"|Opt"+index, ret))
		{
			warning("%s: read error! \n", index_str.c_str());
			return false;
		}
		result.push_back(ret);
	}

	return true;
}
/******************************************************************************
AUTHOR: Wu Yin, 2009.09.10
DESCRIPTION:
	设置以“CurrentOption”标识的配置信息
PARAMETERS:
	index_str：xml元素位置字符串
	value：待设置的值
RETURN:
	true: 设置成功，并保存文件
	false: 设置失败，没有保存配置文件
******************************************************************************/
bool Config::set_current_index(const string &index_str, int value)
{
	// 如果设置的值与配置文件中的值一样，则返回
	int value_in_file = 0;
	get_numerical_attribute(index_str, "CurrentOption", value_in_file);
	if(value_in_file == value)
	{
		return false;
	}

	return set_numerical_attribute(index_str, "CurrentOption", value);
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.06.27
DESCRIPTION:
	读取指定xml元素的属性值
PARAMETERS:
	index_str：xml元素位置字符串
	attribute：xml元素属性名
	result：返回属性值
RETURN:
******************************************************************************/
bool Config::get_string_attribute(const string &index_str,
		const string &attribute, string &result)
{
	IF(_xml_editor == NULL)
	{
		return false;
	}

	string temp;
	if(!_xml_editor->get_attribute(index_str, attribute, temp))
	{
		debug("%s : error! \n", index_str.c_str());
		return false;
	}
	result = temp;
	return true;
}

/******************************************************************************
AUTHOR: ZhangHu, 2008.11.10
DESCRIPTION:
PARAMETERS:
	index_str: 索引字符串
	value: 用来存放返回值
RETURN:
	true: 设置成功，并保存文件
	false: 设置失败，没有保存配置文件
******************************************************************************/
bool Config::set_string_value(const string &index_str, const string &value)
{
	IF(_xml_editor == NULL)
	{
		return false;
	}

	return _xml_editor->set_val(index_str, value);
}

/******************************************************************************
AUTHOR: ZhangHu, 2009.11.30
DESCRIPTION:
PARAMETERS:
RETURN:
******************************************************************************/
bool Config::set_string_value(const vector<string> &path,
		const vector<string> &value)
{
	IF(_xml_editor == NULL)
	{
		return false;
	}

	_xml_editor->set_val(path, value);
	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.08.14
DESCRIPTION:
	重新加载配置文件。
	主要是用来动态切换病人类型时用的，因为此时需要动态切换配置文件。
	TODO：目前还没实际使用和测试过(2009-08-15)
PARAMETERS:
	config_path：需要重新打开的配置文件。
RETURN:
	是否重新加载成功
******************************************************************************/
bool Config::load(const string &config_path)
{
	delete _xml_editor;
	_xml_editor = new XmlEditor(config_path);
	return true;
}

/******************************************************************************
作者: 张虎, 2009.11.17
功能描述:
	将配置写入文件中.
参数说明:
返回值:
******************************************************************************/
int Config::save_file(void)
{
	IF(_xml_editor == NULL)
	{
		return 0;
	}

	return _xml_editor->save_file();
}

/******************************************************************************
作者: 张虎, 2009.08.05
功能描述:
参数说明:
返回值:
******************************************************************************/
Config::Config(const string &config_path) : SyncDiskIFace()
{
	_xml_editor = new XmlEditor(config_path);
	name = config_path.substr(config_path.find_last_of("/") + 1);

	// 只需要一个后台线程保存数据到磁盘即可。
	if(_sync_disk == NULL)
	{
		_sync_disk = new SyncDisk();
	}

	_sync_disk->register_sync_iface(*this);
}

/******************************************************************************
作者: 张虎, 2009.08.05
功能描述:
参数说明:
返回值:
******************************************************************************/
Config::~Config()
{
	delete _xml_editor;

	if(_sync_disk != NULL)
	{
		delete _sync_disk;
		_sync_disk = NULL;
	}
}

