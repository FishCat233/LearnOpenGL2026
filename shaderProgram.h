#pragma once

#include <string>

class ShaderProgram {
public:
	unsigned int ID;

	ShaderProgram(const char* vertexPath, const char* fragmentPath);

	// 禁用拷贝
	ShaderProgram(const ShaderProgram&) = delete;

	// 允许移动
	ShaderProgram(ShaderProgram&&);
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;

	~ShaderProgram();

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};
