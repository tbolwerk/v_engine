#pragma once

#include "device.hpp"

#include <string>
#include <vector>

namespace v_engine
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo(const PipelineConfigInfo &) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
    public:
        Pipeline(Device &device, const PipelineConfigInfo &configInfo, const std::string &vertFilePath, const std::string &fragFilePath);
        ~Pipeline();

        Pipeline(const Pipeline &) = delete;
        Pipeline &operator=(const Pipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char> readFile(const std::string &filePath);

        void createGraphicsPipeline(const PipelineConfigInfo &configInfo, const std::string &vertFilePath, const std::string &fragFilePath);
        void createShaderModule(const std::vector<char> &code, VkShaderModule *VkShaderModule);

        Device &device; // Composite relation, pipeline cannot exists without device, so its save to use reference member var here.
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}