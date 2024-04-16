#include <iostream>

#include <Unkeny/Unkeny.h>
#include <string>
#include <Ntsm/Vector/Vector2.h>
#include <Logger/Formatter.h>

using namespace nkentseu;

class UnkenyEditor : public nkentseu::Application {
public:
	UnkenyEditor() : Application() {
		Log.Error("Unkeny Editor");
	}
};

namespace nkentseu {
	Memory::Shared<Application> MainApplication(const nkentseu::ARGV& argv) {
		Memory::Shared<UnkenyEditor> editor = Memory::Alloc<UnkenyEditor>();
		return editor;
	}
}