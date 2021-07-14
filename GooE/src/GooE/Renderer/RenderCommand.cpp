#include "gepch.h"

#include <Platform/OpenGL/OpenGLRendererApi.h>

#include "RenderCommand.h"

namespace GooE {
	Scope<RendererApi> RenderCommand::rendererApi = CreateScope<OpenGLRendererApi>();
}
