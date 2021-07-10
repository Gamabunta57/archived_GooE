#include "gepch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"

namespace GooE {
	RendererApi* RenderCommand::rendererApi = new OpenGLRendererApi;
}