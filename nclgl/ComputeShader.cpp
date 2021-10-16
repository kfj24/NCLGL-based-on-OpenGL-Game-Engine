#include "ComputeShader.h"
#include "Shader.h"
#include <iostream>
#include <sstream>

using std::string;
using std::ifstream;
using std::cout;

ComputeShader::ComputeShader(const std::string& filename) {
	ifstream	file(SHADERDIR + filename);

	cout << "Loading compute shader text from " << filename << "\n\n";

	if (!file.is_open()) {
		cout << "ERROR ERROR ERROR ERROR: File does not exist!\n";
		return;
	}
	std::ostringstream stream;
	stream << file.rdbuf();
	string contents = stream.str();

	programID	= glCreateProgram();
	shaderID	= glCreateShader(GL_COMPUTE_SHADER);

	const char* stringData = contents.c_str();
	int			stringLength = (int)contents.length();
	glShaderSource(shaderID, 1, &stringData, &stringLength);
	glCompileShader(shaderID);
	glAttachShader(programID, shaderID);

	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &programValid);

	if (!programValid) {
		Shader::PrintCompileLog(shaderID);
		Shader::PrintLinkLog(programID);
	}

	glGetProgramiv(programID, GL_COMPUTE_WORK_GROUP_SIZE, threadsInGroup);
}

ComputeShader::~ComputeShader(void) {
	glDetachShader(programID, shaderID);
	glDeleteShader(shaderID);
	glDeleteProgram(programID);
}

void ComputeShader::Dispatch(unsigned int x, unsigned int y, unsigned int z) const {
	glDispatchCompute(x, y, z);
}

void ComputeShader::Bind()		const {
	glUseProgram(programID);
}

void ComputeShader::Unbind()	const {
	glUseProgram(0);
}