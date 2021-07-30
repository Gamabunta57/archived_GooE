#include <GooE/Core/EntryPoint.h>

#include "TestLayer.h"
#include "Sandbox2D.h"

class Sandbox : public GooE::Application {
public:
    Sandbox() : Application("Sandbox") {
		//PushLayer(new TestLayer());		
		PushLayer(new Sandbox2D());
    }

    ~Sandbox() {}
};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}
