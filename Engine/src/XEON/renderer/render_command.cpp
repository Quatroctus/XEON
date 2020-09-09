#include "xeonpch.h"

#include "XEON/renderer/render_command.h"
#include "XEON/platform/opengl/opengl_renderer_api.h"

namespace XEON {

	Scope<RendererAPI> RenderCommand::rendererAPI = CreateScope<OpenGLRendererAPI>();

}
