#include "../nclGL/window.h"
#include "Renderer.h"
#include <string>
using std::string;
int main() {	
	Window w("Scissors and Stencils!", 1280, 720,false);	//This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);	//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	bool scissor = false;
	bool stencil = false;

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.ToggleScissor();
			scissor = !scissor;
		}
		if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.ToggleStencil();
			stencil = !stencil;
		}

		string title;

		if (scissor) {
			title += "Scissor Enabled. ";
		}
		else {
			title += "Scissor Disabled. ";
		}

		if (stencil) {
			title += "Stencil Enabled. ";
		}
		else {
			title += "Stencil Disabled. ";
		}
		w.SetTitle(title);

		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
	}
	return 0;
}