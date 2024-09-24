#pragma once

#include "components/software_rendering_scene.h"
#include "core/gpu/frame_buffer.h"

#include "triangle_rasterizer.h"

namespace lab
{
    class Lab01 : public gfxc::SoftwareRenderScene
    {
     public:
        Lab01();
        ~Lab01();

        void Initialize() override;

     private:
        void Rasterize(
            const std::vector<VertexFormat> &vertices,
            const std::vector<unsigned int> &indices);
    };
}   // namespace lab
