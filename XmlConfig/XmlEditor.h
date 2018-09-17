/******************************************************************************
版权所有：	深圳市理邦精密仪器有限公司
项目名称：	Mx
版本号：		1.0
文件名：		XmlEditor.h
生成日期：	2009-03-18
作者：		岳红
功能说明：	XML文件编辑器，用来读取和写入XML的键值。应用程序最好不直接调用该类，
			而是通过XxConfig类来调用。
-------------------------------------------------------------------------------
修改：
	版本号：		V 1.0.1
	修改作者: 	张虎
	日期：		2009-11-15
	修改内容：	增加对XML节点的优化，该优化的方案是采用单独一组缓冲区来加快对XML节
				点的读、写及存储操作。优化的主要措施是在XmlEditor内部分别增加一个
				读、写缓冲区。当读取的时候先从读缓冲区中获取，如果获取失败，再从
				tinyXML中解析，并将结果放到读缓冲区中。当需要写一个值的时候，将数据
				写入读缓冲区和写缓冲区。后台线程运行的时候检查写缓冲区是否有数据，
				如果有数据则将数据写入tinyXML中。
-------------------------------------------------------------------------------
	版本号：		V 1.0.2
	修改作者: 	张虎
	日期：		2010-05-05
	修改内容：	增加对XML节点属性的优化，该优化的方案是采用单独一组缓冲区来加快对
				XML节点属性的读、写及存储操作。该方案与1.0.1版的方式类似，也是增加
				读和写缓冲区。
******************************************************************************/
#ifndef _XML_EDITOR_H
#define _XML_EDITOR_H

#include <string>
#include <vector>
#include <map>


#include "tinyxml.h"

enum save_files_result
{
	SAVE_SUCCESS = 0,
	SAVE_NO_NEED = 1,
	SAVE_NO_NEED_LOCK = 2,
	SAVE_FAILED = 3,
};

class XmlEditor
{
	public:
		// ----------------------------- 公有方法 -----------------------------
		// 插入根节点。
		bool set_rootelm(const std::string &root_str);

		// 保存文件到磁盘，由后台保存线程调用，应用程序不要轻易调用！它将调用
		// TinyXML提供的接口将内存中的XML映像写回到磁盘中。
		//返回值为0时，表示保存文件成功；非0时不需要保存文件或保存文件失败。
		int save_file(void);

		// 锁住保存，当value为true的时候，后台线程不会保存配置文件。由于后台线程保
		// 存文件很耗CPU资源，在需要一次保存多个选项的时候可以先锁住保存，等所有选
		// 项都更新的时候再打开锁保存。这样就只需保存一次。
		void locked_save(bool value);

		// 获取第一个节点。
		bool get_first(const std::string &index_str,
				std::string &element_str, std::string &element_val);

		// 获取与element_val邻接的下一个节点。
		bool get_next(std::string &element_str, std::string &element_val);

		// 导出TiXmlDocumen结构。
		const TiXmlDocument *get_xmldoc(void){return _xml_doc;};

		// 该方法可以递归地替换某个字段，以及该字段的子字段的所有值。该方法在参数
		// 载入默认配置中使用。
		void replace_sector(XmlEditor &with_this, const std::string &sector);


		// ---------------------------- 节点值操作 ----------------------------
		// 获某个配置项的值。
		bool get_val(const std::string &index_str, std::string &result);

		// 设置某个配置项的值。
		bool set_val(const std::string &index_str, const std::string &value);

		// 设置一组配置项的值。如果遇到多个设置项的时候可以调用这个方法将需要设置的
		// 项一次性写入。在很多情况下它可以替代save_file和locked_save的组合。该方
		// 法的局限性在于能够一次性地设定所有的项，而save_file和locked_save可以控
		// 制涉及多个源文件中写配置项的操作，比如更改病人类型操作需要大范围更新配置
		// 项，使用本方法无能为力，而使用save_file和locked_save的组合能达到很好的
		// 效果。
		void set_val(const std::vector<std::string> &path,
					 const std::vector<std::string> &value);


		// --------------------------- 节点属性操作 ---------------------------
		// 获取属性。
		bool get_attribute(const std::string &index_str,
				const std::string &attribute, std::string &result);

		// 设置属性。
		bool set_attribute(const std::string &index_str,
				const std::string &attribute, const std::string &value);


		// ---------------------------- 构造与析构 ----------------------------
		XmlEditor(const std::string &fname,
				const std::string &root_str = "xmleditor");
		~XmlEditor();


	private:
		// ----------------------- 内部使用的私有成员方法 ----------------------
		// 判断fname指定的文件是否为XML文件。
		bool _is_xml_file(const std::string &fname);

		// 该方法被_traverse调用，用于构造从root开始的所有pchild的父节点的值。
		void _build_prefix(const TiXmlNode &root,
				const TiXmlNode &pchild, std::string &path);

		// 该方法从sector开始将所有的子节点的值和属性都检索出来，其中值放到
		// _temporary_pool中，属性放到_attr_pool中。
		void _traverse(const TiXmlDocument &doc, const TiXmlNode &sector);

		// 该方法只是将节点值写到缓冲区中，而不是真正写tinyXML。
		bool _set_val(const std::string &index_str, const std::string &value);

		// 该方法将节点值写到tinyXML结构中。
		bool _write_val(const std::string &index_str,
				const std::string &value);

		// 该方法只是将节点属性写到缓冲区中，而不是真正写tinyXML。
		bool _set_attr(const std::string &index_str,
				const std::string &attribute, const std::string &value);

		// 该方法只是将节点属性写到tinyXML中。
		bool _write_attr(const std::string &index_str,
				const std::string &attribute, const std::string &value);

		// 将pelement指向节点的属性值都写到_attr_pool中，_attr_pool的格式为：
		// <路径:属性, 值>，其中路径和属性在同一个字串，使用冒号隔开。
		void _replace_attr(const TiXmlNode &root, const TiXmlElement &pelement);


		// ----------------------------- 控制成员 -----------------------------
		Mutex _mutex;                  // XmlEditor会多线程访问，因此声明保护锁。
		TiXmlDocument *_xml_doc;
		std::string _file_name;
		TiXmlElement *_elm;            // 遍历XML时的临时变量,
		                               // 在get_first()和get_next()中使用。

		// 定义两个map类型，这样方便使用。
		typedef std::map<std::string, std::string> XmlEditorMap;
		typedef std::pair<std::string, std::string> XmlEditorPair;

		// 下面三个成员用于XML节点值的处理。
		XmlEditorMap _write_pool;      // 写缓冲区
		XmlEditorMap _read_pool;       // 读缓冲区
		XmlEditorMap _temporary_pool;  // 保存值的临时缓冲区。

		// 下面三个成员用于XML节点属性的处理。它们内部的组织结构为：
		// <路径:属性, 属性值>，将路径和属性放到键中以冒号分隔。
		static std::string _SEPARATOR;
		XmlEditorMap _write_attr_pool; // 保存属性的写缓冲区。
		XmlEditorMap _read_attr_pool;  // 保存属性的读缓冲区。
		XmlEditorMap _attr_pool;       // 保存属性的临时缓冲区。

		bool _has_changed;             // 标识文件已经被修改过。
		bool _locked_save;             // 该成员为true时，文件不被保存到磁盘。
};


#endif
