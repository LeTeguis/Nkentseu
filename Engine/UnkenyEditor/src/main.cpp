#include <iostream>

#include <Unkeny/Unkeny.h>
#include <string>
#include <Ntsm/Vector/Vector2.h>
#include <Logger/Formatter.h>

using namespace nkentseu;

class Pop {
public:
	float32 x = 1;
	float32 y = 0;

	friend std::string ToString(const Pop& p) {
		return FORMATTER.Format("{0}-{1}-{0}", p.x, p.y);
	}
};

class Editor : public nkentseu::Application {
public:
	Editor() : Application() {
		Pop v;
		//std::cout << "Bonjour-de std " << v << "\n";
		Log.Error("Bonjour de log {0}", v);
	}
};

namespace nkentseu {
	Memory::Shared<Application> MainApplication(const nkentseu::ARGV& argv) {
		Memory::Shared<Editor> editor = Memory::Alloc<Editor>();
		return editor;
	}
}