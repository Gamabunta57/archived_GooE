#include <GooE.h>

class TestLayer : public GooE::Layer
{
public:
	TestLayer() : Layer("Test") {}

	void OnUpdate() override {
		GOOE_TRACE("TestLayer::OnUpdate");
	}

	void OnEvent(GooE::Event& e) override {
		GOOE_TRACE("TestLayer::OnEvent {0}", e);
	}
};



class Sandbox : public GooE::Application {
public:
    Sandbox() {
        PushLayer(new TestLayer());
    }

    ~Sandbox() {

    }

};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}