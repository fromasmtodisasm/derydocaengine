#pragma once
#include <map>
#include "Color.h"
#include "glm\glm.hpp"
#include <gl\glew.h>

class Shader;
class Texture;
class RenderTexture;

class Material
{
public:
	Material();
	~Material();

	inline void setShader(Shader* shader) { m_shader = shader; }
	inline Shader* getShader() const { return m_shader; }
	void setBool(std::string const& name, bool const& value);
	void setInt(std::string const& name, int const& value);
	void setTexture(std::string const& name, Texture* const& texture);
	void setTextureSlot(int const& slot, Texture* const& texture);
	void setFloat(std::string const& name, float const& value);
	void setVec3(std::string const& name, glm::vec3 const& value);
	void setVec4(std::string const& name, glm::vec4 const& value);
	void setColorRGB(std::string const& name, Color const& value);
	void setColorRGBA(std::string const& name, Color const& value);
	void setMat3(std::string const& name, glm::mat3 const& value);
	void setMat4(std::string const& name, glm::mat4 const& value);
	void setSubroutine(GLuint const& program, GLuint const& value);
	void bind();
	void unbind();
private:
	Shader* m_shader;
	// TODO: Replace this with a BST for multiple textures
	Texture* m_texture;
	std::map<std::string, bool> m_boolValues;
	std::map<std::string, int> m_intValues;
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, float> m_floatValues;
	std::map<std::string, glm::vec3> m_vec3Values;
	std::map<std::string, glm::vec4> m_vec4Values;
	std::map<std::string, glm::mat3> m_mat3Values;
	std::map<std::string, glm::mat4> m_mat4Values;
	std::map<GLuint, GLuint> m_subroutineValues;
};

