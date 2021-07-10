#include "gepch.h"

#include <Platform/OpenGL/OpenGLRendererApi.h>

#include "RenderCommand.h"

namespace GooE {
	RendererApi* RenderCommand::rendererApi = new OpenGLRendererApi;
}
