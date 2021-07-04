#include <GooE.h>

class Sandbox : public GooE::Application {
public:
    Sandbox() {
        PushOverlay(new GooE::ImGuiLayer());
    }

    ~Sandbox() {

    }

};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}