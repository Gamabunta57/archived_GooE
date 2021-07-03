#include <GooE.h>

class Sandbox : public GooE::Application {
public:
    Sandbox() {

    }

    ~Sandbox() {

    }

};

GooE::Application* GooE::CreateApplication() {
    return new Sandbox();
}