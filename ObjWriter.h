#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

class ObjWriter final
{
public:
	explicit ObjWriter(const std::string& inputFile, const std::string& outputFile);
	void WriteObj();

private:
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::unordered_map<char, std::string> m_TypeMap;
	void DecodeBinaryHeader();
	void VerifyBinaryHeader();
};

