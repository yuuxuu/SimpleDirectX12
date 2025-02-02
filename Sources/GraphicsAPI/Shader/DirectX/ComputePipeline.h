/**
 * @file ComputePipeline.h
 * @brief
 * @author Yu Kimura
 * @date 2020/05/18
 */

#pragma once

#ifndef _COMPUTE_PIPELINE_H_
#define _COMPUTE_PIPELINE_H_

namespace Graphics 
{
namespace Shader
{ 
    class ComputePipeline
    {
    private:

    private:
        ComputePipeline(const ComputePipeline&) = delete;
        ComputePipeline operator=(const ComputePipeline&) = delete;

    public:
        ComputePipeline();
        ~ComputePipeline();
    };
} // namespace ComputePipeline
} // namespace Graphics

#endif // _COMPUTE_PIPELINE_H_
