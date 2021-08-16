#pragma once

#include "device.hpp"

#include <string>
#include <vector>

namespace v_engine
{
  struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

    class Pipeline
    {
        public:
            Pipeline(Device &device, const PipelineConfigInfo &configInfo, const std::string &vertFilePath, const std::string &fragFilePath);
            ~Pipeline();

            Pipeline(const Pipeline&) = delete;
            void operator=(const Pipeline&) = delete;

            void bind(VkCommandBuffer commandBuffer);
            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
            static std::vector<char> readFile(const std::string &filePath);

            void createGraphicsPipeline(const PipelineConfigInfo &configInfo, const std::string &vertFilePath, const std::string &fragFilePath);
            void createShaderModule(const std::vector<char> &code, VkShaderModule *VkShaderModule); 

            Device &device; // Composite relation, pipeline cannot exists without device, so its save to use reference member var here.
            VkPipeline  graphicsPipeline;
            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;
    };
}