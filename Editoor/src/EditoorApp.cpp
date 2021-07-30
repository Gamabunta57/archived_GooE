#include <GooE/Core/EntryPoint.h>

#include "EditoorLayer.h"


class Editoor : public GooE::Application {
public:
    Editoor() : Application("Editoor") {
		PushLayer(new GooE::EditoorLayer());
    }

    ~Editoor() {}
};

GooE::Application* GooE::CreateApplication() {
    return new Editoor();
}
