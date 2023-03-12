#pragma once
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

class BinaryWriter final
{
public:
	explicit BinaryWriter(const std::string& inputFile, const std::string& outputFile);

	void WriteBObj();

private:
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::vector<std::string> m_ReadLines;
	std::unordered_map<std::string, char> m_TypeMap;
	void CreateTypeMap();

	void WriteBinaryHeader();

	char GetFirstBlockType();
	void WriteDataBlocks();

	void VerifyBinaryHeader();

	template<typename T>
	void WriteBlock(char typeId, std::vector<T>& block);

	void WriteTerminator();
};

template<typename T>
void BinaryWriter::WriteBlock(char typeId, std::vector<T>& block)
{
	// Write block type
	m_OutputFile.write(&typeId, sizeof(typeId));

	// Write number of elements in block
	size_t blockSize{ block.size() };
	m_OutputFile.write((const char*)&blockSize, sizeof(blockSize));

	// Write block elements
	for (const auto& elem : block)
	{
		m_OutputFile.write((const char*)&elem, sizeof(elem));
	}

	// Flush block
	block.clear();

	// Write terminator for block safety
	WriteTerminator();
}

