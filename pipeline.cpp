#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace v_engine
{
    Pipeline::Pipeline(const std::string &vertFilePath, const std::string &fragFilePath)
    {
        createGraphicsPipeline(vertFilePath, fragFilePath);
    }

    std::vector<char> Pipeline::readFile(const std::string &filePath)
    {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        if(!file.is_open()){
            throw std::runtime_error("ERROR: Failed to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg()); // Because of ate we are already at the end of the file and are able to read position and therefore know the size.
        std::vector<char> buffer(fileSize);

        file.seekg(0); // Return to the starting location of the file.
        file.read(buffer.data(), fileSize); // Start reading file and loading it into the buffer.

        file.close(); // Free resource
        return buffer; // return buffer
    }

    void Pipeline::createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath)
    {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        std::cout << "Vertex shader fileSize: " << vertCode.size() << std::endl;
        std::cout << "Vertex fragment filesize: " << fragCode.size() << std::endl;
    }
}