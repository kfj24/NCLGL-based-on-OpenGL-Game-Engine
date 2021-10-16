/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	 <richard-gordon.davison@newcastle.ac.uk>
Description:VERY simple class to encapsulate GLSL shader loading, linking,
and binding. Useful additions to this class would be overloaded functions to
replace the glUniformxx functions in external code, and possibly a map to store
uniform names and their resulting bindings. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "OGLRenderer.h"

enum ShaderStage {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_DOMAIN,
	SHADER_HULL,
	SHADER_MAX
};

class Shader	{
public:
	Shader(const std::string& vertex, const std::string& fragment, const std::string& geometry = "", const std::string& domain = "", const std::string& hull = "");
	~Shader(void);

	GLuint  GetProgram() { return programID;}
	
	void	Reload(bool deleteOld = true);

	bool	LoadSuccess() {
		return shaderValid[0] == GL_TRUE && programValid == GL_TRUE;
	}

	static void ReloadAllShaders();
	static void	PrintCompileLog(GLuint object);
	static void	PrintLinkLog(GLuint program);

protected:
	void	DeleteIDs();

	bool	LoadShaderFile(const  std::string& from, std::string &into);
	void	GenerateShaderObject(unsigned int i);
	void	SetDefaultAttributes();
	void	LinkProgram();

	GLuint	programID;
	GLuint	objectIDs[SHADER_MAX];
	GLint	programValid;
	GLint	shaderValid[SHADER_MAX];

	std::string  shaderFiles[SHADER_MAX];

	static std::vector<Shader*> allShaders;
};

