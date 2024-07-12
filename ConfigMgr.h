#pragma once
#include <fstream>  //文件读写
#include <boost/property_tree/ptree.hpp>  //文件解析
#include <boost/property_tree/ini_parser.hpp>  
#include <boost/filesystem.hpp>    //系统文件操作
#include <map>
#include <iostream>

struct SectionInfo {
	SectionInfo(){}
	~SectionInfo(){
		_section_datas.clear();
	}
	
	SectionInfo(const SectionInfo& src) {
		_section_datas = src._section_datas;
	}
	
	SectionInfo& operator = (const SectionInfo& src) {
		if (&src == this) {
			return *this;	//使用同一对象
		}

		this->_section_datas = src._section_datas;	//复制对象参数
		return *this;
	}

	std::map<std::string, std::string> _section_datas;
	std::string  operator[](const std::string  &key) {
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";
		}
		// 这里可以添加一些边界检查  
		return _section_datas[key];
	}

	std::string GetValue(const std::string & key) {
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";
		}
		// 这里可以添加一些边界检查  
		return _section_datas[key];
	}
};

class ConfigMgr
{
public:
	~ConfigMgr() {
		_config_map.clear();
	}
	SectionInfo operator[](const std::string& section) {	//全局重写操作符ConfigMgr[]，并返回section键对
		if (_config_map.find(section) == _config_map.end()) {
			return SectionInfo();
		}
		return _config_map[section];
	}


	ConfigMgr& operator=(const ConfigMgr& src) {	//重写赋值运算符
		if (&src == this) {
			return *this;
		}

		this->_config_map = src._config_map;
	};

	ConfigMgr(const ConfigMgr& src) {
		this->_config_map = src._config_map;
	}

	static ConfigMgr& Inst() {
		static ConfigMgr cfg_mgr;	//静态声明保证全局单例对象
		return cfg_mgr;
	}

	std::string GetValue(const std::string& section, const std::string & key);
private:
	ConfigMgr();
	// 存储section和key-value对的map  
	std::map<std::string, SectionInfo> _config_map;
};

