#include "BinaryWriter.h"

#include <iostream>

#include "Structs.h"
#include <sstream>

// TODO: cleanup code

BinaryWriter::BinaryWriter(const std::string& inputFile, const std::string& outputFile)
	: m_InputFile(inputFile)
	, m_OutputFile(outputFile)
{
    // TODO: work with getline instead of vector
    std::string readLine;
    while (std::getline(m_InputFile, readLine))
    {
        m_ReadLines.push_back(readLine);
    }

    CreateTypeMap();
    VerifyBinaryHeader();
}

void BinaryWriter::WriteBObj()
{
    WriteBinaryHeader();
    WriteDataBlocks();
    m_OutputFile.close();
}

void BinaryWriter::CreateTypeMap()
{
    char typeIdx{ 1 };

    for (const auto& line : m_ReadLines)
    {
        std::stringstream lineSS{ line };
        std::string type;
        lineSS >> type;

        auto it = m_TypeMap.find(type);
        if (it == m_TypeMap.end())
        {
            m_TypeMap[type] = typeIdx++;
        }
    }
}

void BinaryWriter::WriteBinaryHeader()
{
    for (const auto& typePair : m_TypeMap)
    {
        const auto& typeStr{ typePair.first };
        const auto& typeChar{ typePair.second };

        // encode type char
        m_OutputFile.write((const char*)&typeChar, sizeof(typeChar));

        // encode size of string the char should translate to
        char stringLength{ static_cast<char>(typeStr.size())};
        m_OutputFile.write((const char*)&stringLength, sizeof(stringLength));

        // encode type string itself
        m_OutputFile.write(&typeStr[0], stringLength);
    }

    // Add null terminator to delimit end of header
    WriteTerminator();
}

char BinaryWriter::GetFirstBlockType()
{
    std::string typeString;
    std::stringstream firstLine{ m_ReadLines[0] };
    firstLine >> typeString;
    return m_TypeMap[typeString];
}

void BinaryWriter::WriteDataBlocks()
{
    char currentType{ GetFirstBlockType() };

    std::vector<Vertex> vertexBlock;
    std::vector<Face> faceBlock;
    std::vector<Vertex> normalBlock;

    for(const auto& line : m_ReadLines)
    {
        std::stringstream readLine{ line };
        std::string lineTypeString;
        readLine >> lineTypeString;
        char lineType{ m_TypeMap[lineTypeString] };

        if(currentType != lineType)
        {
            if(currentType == m_TypeMap["v"])
            {
                WriteBlock(currentType, vertexBlock);
            }
            currentType = lineType;
        }

        if (currentType == m_TypeMap["v"])
        {
            Vertex vertex;
            readLine >> vertex.x;
            readLine >> vertex.y;
            readLine >> vertex.z;
            vertexBlock.push_back(vertex);
        }
    }

    if (currentType == m_TypeMap["v"])
    {
        WriteBlock(currentType, vertexBlock);
    }
	
    WriteTerminator();
}

/*else if (currentType == m_TypeMap["f"])
    {
        WriteBlock(currentType, faceBlock);
    }
    else if (currentType == m_TypeMap["vn"])
    {
        WriteBlock(currentType, normalBlock);
    }*/
/*else if(currentType == m_TypeMap["f"])
        {
            Face face;
            readLine >> face.v0;
            readLine >> face.v1;
            readLine >> face.v2;
            faceBlock.push_back(face);
        }
        else if(currentType == m_TypeMap["vn"])
        {
            Vertex normal;
            readLine >> normal.x;
            readLine >> normal.y;
            readLine >> normal.z;
            normalBlock.push_back(normal);
        }*/

void BinaryWriter::VerifyBinaryHeader()
{
    for(const auto& pair : m_TypeMap)
    {
        std::cout << pair.first << ": " << static_cast<int>(pair.second) << "\n";
    }
}

void BinaryWriter::WriteTerminator()
{
    const char zeroTerminator{ 0 };
    m_OutputFile.write(&zeroTerminator, sizeof(char));
}
