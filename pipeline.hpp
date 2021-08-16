#pragma once

#include <string>
#include <vector>

namespace v_engine
{
    class Pipeline
    {
        public:
            Pipeline(const std::string &vertFilePath, const std::string &fragFilePath);
        private:
            static std::vector<char> readFile(const std::string &filePath);

            void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath);
    };
}