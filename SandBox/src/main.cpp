namespace GooE {
    __declspec(dllimport) void print(const char* message);
}

int main() {
    GooE::print("It works!\n");
}