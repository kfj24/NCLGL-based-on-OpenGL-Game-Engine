#include "Shader.h"
#include "Mesh.h"
#include <iostream>

using std::string;
using std::cout;
using std::ifstream;

vector<Shader*> Shader::allShaders;

GLuint shaderTypes[SHADER_MAX] = {
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER
};

string ShaderNames[SHADER_MAX] = {
	"Vertex",
	"Fragment",
	"Geometry",
	"Tess. Control",
	"Tess. Eval"
};

Shader::Shader(const string& vertex, const string& fragment, const string& geometry, const string& domain, const string& hull)	{
	shaderFiles[SHADER_VERTEX]		= vertex;
	shaderFiles[SHADER_FRAGMENT]	= fragment;
	shaderFiles[SHADER_GEOMETRY]	= geometry;
	shaderFiles[SHADER_DOMAIN]		= domain;
	shaderFiles[SHADER_HULL]		= hull;

	Reload(false);
	allShaders.emplace_back(this);
}

Shader::~Shader(void)	{
	DeleteIDs();
}

void	Shader::Reload(bool deleteOld) {
	if(deleteOld) {
		DeleteIDs();
	}

	programID		= glCreateProgram();

	for (int i = 0; i < SHADER_MAX; ++i) {
		if (!shaderFiles[i].empty()) {
			GenerateShaderObject(i);
		}
		else {
			objectIDs[i]	= 0;
			shaderValid[i]	= 0;
		}
	}
	SetDefaultAttributes();
	LinkProgram();
	PrintLinkLog(programID);
}

bool	Shader::LoadShaderFile(const string& filename, string &into)	{
	ifstream	file(SHADERDIR + filename);
	string		textLine;

	cout << "Loading shader text from " << filename << "\n\n";

	if(!file.is_open()){
		cout << "ERROR ERROR ERROR ERROR: File does not exist!\n";
		return false;
	}
	int lineNum = 1; 
	while(!file.eof()){
		getline(file,textLine);
		textLine += "\n";
		into += textLine;
		cout << "(" << lineNum << ") :" << textLine;
		++lineNum;
	}
	cout << "\nLoaded shader text!\n\n";
	return true;
}

void	Shader::GenerateShaderObject(unsigned int i)	{
	cout << "Compiling Shader...\n";

	string shaderText;
	if(!LoadShaderFile(shaderFiles[i],shaderText)) {
		cout << "Loading failed!\n";
		shaderValid[i] = false;
		return;
	}

	objectIDs[i] = glCreateShader(shaderTypes[i]);

	const char *chars	= shaderText.c_str();
	int textLength		= (int)shaderText.length();
	glShaderSource(objectIDs[i], 1, &chars, &textLength);
	glCompileShader(objectIDs[i]);

	glGetShaderiv(objectIDs[i], GL_COMPILE_STATUS, &shaderValid[i]);

	if (!shaderValid[i]) {
		cout << "Compiling failed!\n";
		PrintCompileLog(objectIDs[i]);
	}
	else {
		cout << "Compiling success!\n\n";
	}

	glObjectLabel(GL_SHADER, objectIDs[i], -1, shaderFiles[i].c_str());
	glAttachShader(programID, objectIDs[i]);
}

void Shader::LinkProgram()	{
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &programValid);
}

void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(programID, VERTEX_BUFFER,  "position");
	glBindAttribLocation(programID, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(programID, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(programID, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(programID, TEXTURE_BUFFER, "texCoord");

	glBindAttribLocation(programID, WEIGHTVALUE_BUFFER, "jointWeights");
	glBindAttribLocation(programID, WEIGHTINDEX_BUFFER, "jointIndices");
}

void	Shader::DeleteIDs() {
	if (!programID) {
		return;
	}
	for (int i = 0; i < SHADER_MAX; ++i) {
		if (objectIDs[i]) {
			glDetachShader(programID, objectIDs[i]);
			glDeleteShader(objectIDs[i]);
		}
	}
	glDeleteProgram(programID);
	programID = 0;
}

void	Shader::PrintCompileLog(GLuint object) {
	int logLength = 0;
	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength) {
		char* tempData = new char[logLength];
		glGetShaderInfoLog(object, logLength, NULL, tempData);
		std::cout << "Compile Log:\n" << tempData << std::endl;
		delete[] tempData;
	}
}

void	Shader::PrintLinkLog(GLuint program) {
	int logLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength) {
		char* tempData = new char[logLength];
		glGetProgramInfoLog(program, logLength, NULL, tempData);
		std::cout << "Link Log:\n" << tempData << std::endl;
		delete[] tempData;
	}
}

void Shader::ReloadAllShaders() {
	for (auto& i : allShaders) {
		i->Reload();
	}
}