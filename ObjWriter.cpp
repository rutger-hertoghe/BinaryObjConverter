#include "ObjWriter.h"

#include <iostream>
#include <string>

#include "Structs.h"
#include <sstream>

//TODO: cleanup code, add comments

ObjWriter::ObjWriter(const std::string& inputFile, const std::string& outputFile)
	: m_InputFile(inputFile, std::ios::binary)
	, m_OutputFile(outputFile)
{
    DecodeBinaryHeader();
    VerifyBinaryHeader();
}

void ObjWriter::WriteObj()
{
    //std::vector<Vertex> vertexBlock;
    
    char charType{ 1 };
    while (charType != 0)
    {
	    // Read type
        m_InputFile.read(&charType, sizeof(charType));

        // Determine how many bytes have to be read
        size_t blockSize;
        m_InputFile.read((char*) &blockSize, sizeof(blockSize));

        if(m_TypeMap[charType] == "f")
        {
	        for(size_t i{0}; i < blockSize; ++i)
	        {
                Face readFace;
                std::stringstream line;
                m_InputFile.read((char*)&readFace, sizeof(readFace));
                line << "f " << readFace.v0 << " " << readFace.v1 << " " << readFace.v2 << "\n";

                m_OutputFile << line.str();
	        }
        }

        if(m_TypeMap[charType] == "v")
        {
            for(size_t i {0}; i < blockSize; ++i)
            {
                Vertex readVertex;
                std::stringstream line;
                std::cout << sizeof(readVertex);
                m_InputFile.read((char*)&readVertex, sizeof(readVertex));
                //vertexBlock.push_back(readVertex);
                line << "v " << readVertex.x << " " << readVertex.y << " " << readVertex.z << '\n';
                std::cout << line.str() << "\n";
                m_OutputFile << line.str();
            }
        }
        char terminator;
        m_InputFile.read(&terminator, sizeof(terminator));
        if (terminator != 0)
        {
            std::cout << "Block did not end with null terminator! \n";
        }
    }


    //// Create vertices for each block type
    //std::vector<Vertex> readVertexBlock;
    //std::vector<Face> readFaceBlock;

    //char charType{ 1 }; // Initialize as 1 to start reading of bobj
    //while (charType != 0)
    //{
    //    // Read actual type
    //    m_InputFile.read(&charType, sizeof(charType));

    //    // Stop reading if terminator is reached
    //    if (charType == 0) break;

    //    // Read blocksize
    //    size_t blockSize;
    //    m_InputFile.read((char*)&blockSize, sizeof(blockSize));

    //    // Get actual type from map
    //    std::string strType{ m_TypeMap[charType] };

    //    if (strType == "v")
    //    {
    //        //readVertexBlock.reserve(blockSize);
    //        for (size_t i{ 0 }; i < blockSize; ++i)
    //        {
    //            std::stringstream outputLine;
    //            Vertex readVertex{};
    //            m_InputFile.read((char*)&readVertex, sizeof(Vertex));
    //            outputLine << "v " << readVertex.x << ' ' << readVertex.y << ' ' << readVertex.z << '\n';
    //            m_OutputFile << outputLine.str();
    //        }
    //    }
    //    if (strType == "f")
    //    {
    //        for (size_t i{ 0 }; i < blockSize; ++i)
    //        {
    //            std::stringstream outputLine;
    //            Face readFace;
    //            m_InputFile.read((char*)&readFace, sizeof(readFace));
    //            outputLine << "f" << readFace.v0 << ' ' << readFace.v1 << ' ' << readFace.v2 << '\n';
    //            m_OutputFile << outputLine.str();
    //        }
    //    }
    //}
}

void ObjWriter::DecodeBinaryHeader()
{
    bool isReading{ true };
    char typeInt{ 0 };
    while (isReading)
    {
        // Decode type char
        m_InputFile.read((char*)&typeInt, sizeof(typeInt));
        // If 0, end of header has been reached
        if (typeInt == 0)
        {
            isReading = false;
            break;
        }
        // Decode length of type string
        char stringLen;
        m_InputFile.read((char*)&stringLen, sizeof(stringLen));

        // Initialize destination string of retrieved length;
        std::string typeString;
        typeString.resize(stringLen);

        // Decode string
        m_InputFile.read(&typeString[0], static_cast<int>(stringLen));

        // Add string to type map for further references
    	m_TypeMap[typeInt] = typeString;
    }
}

void ObjWriter::VerifyBinaryHeader()
{
    for(auto pair : m_TypeMap)
    {
        std::cout << static_cast<int>(pair.first) << " : " << pair.second << "\n";
    }
}
