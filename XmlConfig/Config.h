/******************************************************************************
	  COPYRIGHT: 2008 Edan Instruments,	Inc. All rights	reserved
	PROJECTNAME: Mx
		VERSION: 1.0
	   FILENAME: Config/Config.h
		CREATED: 2009.03.04
		 AUTHOR: Yue Hong
	DESCRIPTION:
******************************************************************************/
#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>
#include <vector>
#include <typeinfo>

#include "SyncDisk.h"
#include "XmlEditor.h"
#include "Misc.h"


/******************************************************************************
AUTHOR: ZhangHu, 2008.11.10
DESCRIPTION:
******************************************************************************/
class Config : public SyncDiskIFace
{
	public:
		// 数字类型的值
		template <typename T>
		bool get_numerical_value(const std::string &index_str, T &result);
		template <typename T>
		bool set_numerical_value(const std::string &index_str, const T &value);
		template <typename T>
		void set_numerical_value(const std::vector<std::string> &path,
				const std::vector<T> &value);

		// 字符串类型的值
		bool get_string_value(const std::string &index_str, std::string &result);
		bool set_string_value(const std::string &index_str,
				const std::string &value);
		bool set_string_value(const std::vector<std::string> &path,
				const std::vector<std::string> &value);

		// 以“CurrentOption”标识的配置
		bool get_current_value(const std::string &index_str, std::string &result);
		template <typename T>
		bool get_current_numerical_value(const std::string &index_str, T &result);
		bool get_current_index(const std::string &index_str, int &index);
		bool set_current_index(const std::string &index_str, int value);
		bool get_current_list(const std::string &index_str,
				std::vector<std::string> &result);

		// 属性（attribute）相关
		bool get_string_attribute(const std::string &index_str,
				const std::string &attribute, std::string &result);
		template <typename T>
		/******************************************************************************
		AUTHOR: zhoucheng
		DESCRIPTION:
		设置当前索引对应的值
		PARAMETERS:

		RETURN:
			是设置取成功
		******************************************************************************/
		bool get_numerical_attribute(const std::string &index_str,
				const std::string &attribute, T &result);
		template <typename T>
		bool set_numerical_attribute(const std::string &index_str,
				const std::string &attribute, const T &value);

		bool load(const std::string &config_path);
		int save_file(void);

		XmlEditor *get_xml_editor(void) {return _xml_editor;};

		void locked_save(bool value)
		{
			IF(_xml_editor == NULL)
			{
				return;
			}

			_xml_editor->locked_save(value);
		}
		bool set_current_index_value(const string &index_str, const string &value,int ind);
		Config(const std::string &config_path);
		virtual ~Config();

	protected:
		XmlEditor *_xml_editor;

	private:
		std::string name;
		static SyncDisk *_sync_disk;
};

/******************************************************************************
AUTHOR: ZhangHu, 2008.11.10
DESCRIPTION:
PARAMETERS:
	index_str: 索引字符串
	result: 用来存放返回值
RETURN:
******************************************************************************/
template <typename T>
inline bool Config::get_numerical_value(const std::string &index_str, T &result)
{
	std::string temp;
	if(!_xml_editor->get_val(index_str, temp))
	{
//		debug("%s\t%s : error! \n", name.c_str(), index_str.c_str());
		return false;
	}

	if(typeid(result) == typeid(float))
	{
		result = (T)atof(temp.c_str());
	}
	else
	{
		result = (T)atoi(temp.c_str());

	}

	// 这段代码检测获取的字串能不能转化成数字，如果不能则打印出提示信息。
	if(result == T(0))
	{
		int size = static_cast<int>(temp.size());
		while(size--)
		{
			if((temp[size] >= '0') && (temp[size] <= '9'))
			{
				continue;
			}
			else //temp中包含了非0~9的字符，因此不能成功转换成数字。
			{
				debug("The value of %s is not a valid numeric.\n",
						index_str.c_str());
				result = (T)(0);
				return false;
			}
		}
	}
	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.11.09
DESCRIPTION:
	读取数值型的Current配置信息
PARAMETERS:
	index_str: 索引字符串
	result: 用来存放返回值
RETURN:
	是否读取成功
******************************************************************************/
template <typename T>
inline bool Config::get_current_numerical_value(const std::string &index_str,
		T &result)
{
	int current_opt = 0;
	std::string index("0");

	// 读取当前选项
	if(!get_numerical_attribute(index_str, "CurrentOption", current_opt))
	{
		debug("%s: read error 1! \n", index_str.c_str());
		return false;
	}

	to_string(current_opt, index);

	// 按照当前选项来读取实际值
	if(!get_numerical_value(index_str+"|Opt"+index, result))
	{
		warning("%s: read error 2! \n", index_str.c_str());
		return false;
	}

	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.06.29
DESCRIPTION:
	读取指定xml元素的属性值
PARAMETERS:
	index_str：xml元素位置字符串
	attribute：xml元素属性名
	result：返回属性值
RETURN:
******************************************************************************/
template <typename T>
inline bool Config::get_numerical_attribute(const std::string &index_str,
		const std::string &attribute, T &result)
{
	std::string temp;
	if(!_xml_editor->get_attribute(index_str, attribute, temp))
	{
		//debug("%s\t%s : error! \n", name.c_str(), index_str.c_str());
		return false;
	}

	if(typeid(result) == typeid(float))
	{
		result = (T)atof(temp.c_str());
	}
	else
	{
		result = (T)atoi(temp.c_str());
	}

	return true;
}

/******************************************************************************
AUTHOR: Wu Yin, 2009.09.10
DESCRIPTION:
	设置数值型的属性值
PARAMETERS:
	index_str：xml元素位置字符串
	attribute：xml元素属性名
	value：待设置的值
RETURN:
	true: 设置成功，并保存文件
	false: 设置失败，没有保存配置文件
******************************************************************************/
template <typename T>
inline bool Config::set_numerical_attribute(const std::string &index_str,
		const std::string &attribute, const T &value)
{
	// 如果设置的值与配置文件中的值一样，则返回
	T value_in_file = value;
	get_numerical_attribute(index_str, attribute, value_in_file);
	if(value_in_file == value)
	{
		return false;
	}

	std::string str;
	::to_string(value, str);
	_xml_editor->set_attribute(index_str, attribute, str);

	return true;
}

/******************************************************************************
AUTHOR: ZhangHu, 2008.11.10
DESCRIPTION:
	保存数值型配置，如果设置的值与配置文件中的值一样，则返回
PARAMETERS:
	index_str: 索引字符串
	value: 用来存放返回值
RETURN:
	true: 设置成功，并保存文件
	false: 设置失败，没有保存配置文件
******************************************************************************/
template <typename T>
inline bool Config::set_numerical_value(const std::string &index_str,
		const T &value)
{
	std::string str;
	::to_string(value, str);
	return _xml_editor->set_val(index_str, str);
}

/******************************************************************************
AUTHOR: ZhangHu, 2009.11.30
DESCRIPTION:
PARAMETERS:
RETURN:
******************************************************************************/
template <typename T>
void Config::set_numerical_value(const std::vector<std::string> &path,
		const std::vector<T> &value)
{
	std::vector<std::string> v;
	std::string str;

	for(unsigned i=0; i<value.size(); i++)
	{
		::to_string(value[i], str);
		v.push_back(str);
	}

	_xml_editor->set_val(path, v);
}


#endif

