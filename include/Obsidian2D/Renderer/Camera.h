#ifndef OBSIDIAN2D_CORE_CAMERA_H
#define OBSIDIAN2D_CORE_CAMERA_H

#include "Layers.h"

typedef struct _view_camera {
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
} ViewCamera;

namespace Obsidian2D
{
	namespace Renderer
	{
		class Camera : public Layers
		{
		private:

			ViewCamera _view_camera;

			glm::mat4 _projection;
			glm::mat4 _view;
			glm::mat4 _model;

			glm::mat4 _mvp; // Model View Projection

			const std::array<int, 3> _default_eye		= {-5, 3, -10};
			const std::array<int, 3> _default_center	= {0, 0, 0};
			const std::array<int, 3> _default_up 		= {0, -1, 0};

			void updateMVP()
			{
				this->_mvp = this->_projection * this->_view * this->_model;
			}

		public:
			VkMemoryRequirements mem_reqs;

			struct {
				VkBuffer 							buf;
				VkDeviceMemory 						mem;
				VkDescriptorBufferInfo 				buffer_info;
			} uniform_data;

			void initCamera()
			{
				float fov = glm::radians(45.0f);
				if (width > height) {
					fov *= static_cast<float>(height) / static_cast<float>(width);
				}
				this->_projection = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

				this->_view_camera.eye 	  =  glm::vec3(_default_eye[0], _default_eye[1], _default_eye[2]);
				this->_view_camera.center =  glm::vec3(_default_center[0], _default_center[1], _default_center[2]);
				this->_view_camera.up     =  glm::vec3(_default_up[0], _default_up[1], _default_up[2]);

				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );

				this->_model = glm::mat4(1.0f);

				this->updateMVP();
			}

			void setCameraViewEye(glm::vec3 eye)
			{
				this->_view_camera.eye = eye;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			void setCameraViewCenter(glm::vec3 center)
			{
				this->_view_camera.center = center;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			void setCameraViewUp(glm::vec3 up)
			{
				this->_view_camera.up = up;
				this->_view = glm::lookAt( this->_view_camera.eye, this->_view_camera.center, this->_view_camera.up );
				this->updateMVP();
			}

			glm::mat4 getMVP()
			{
				return this->_mvp;
			}

			std::array<int, 3> getCameraDefaultEye()
			{
				return _default_eye;
			};

			std::array<int, 3> getCameraDefaultCenter()
			{
				return _default_center;
			};

			std::array<int, 3> getCameraDefaultUp()
			{
				return _default_up;
			};

			void updateCamera(VkDevice device)
			{
				VkResult res;
				void* _buffer_address = nullptr;

				res = vkMapMemory(device, uniform_data.mem, 0, sizeof(this->_mvp), 0, &_buffer_address);
				assert(res == VK_SUCCESS);
				memcpy(_buffer_address, &this->_mvp, sizeof(this->_mvp));
				vkUnmapMemory(device, uniform_data.mem);
			}

		};
	}
}


#endif //OBSIDIAN2D_CORE_CAMERA_H