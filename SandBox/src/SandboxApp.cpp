#include <imgui/imgui.h>

#include <GooE.h>

class TestLayer : public GooE::Layer {
public:
    TestLayer() : Layer("Test") {}

    void OnUpdate() override {
        if (GooE::Input::IsKeyPressed(GOOE_KEY_TAB))
            GOOE_TRACE("Tab is pressed!");
    }

    virtual void OnImGuiRender() override {
        ImGui::Begin("test");
        ImGui::Text("Here I am!");
        ImGui::End();
    }
};

class Sandbox : public GooE::Application {
public:
    Sandbox() {
        PushLayer(new TestLayer());
    }

    ~Sandbox() {}

};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}
