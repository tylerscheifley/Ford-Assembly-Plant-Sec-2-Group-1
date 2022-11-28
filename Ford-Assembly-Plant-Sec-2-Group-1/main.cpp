

#include "plant.h"
#include "plantFiles.h"

using namespace std;



#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float size;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(size * aPos.x, size * aPos.y, size * aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct ExampleAppLog
{
	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.

	ExampleAppLog()
	{
		AutoScroll = true;
		Clear();
	}

	void    Clear()
	{
		Buf.clear();
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		int old_size = Buf.size();
		va_list args;
		va_start(args, fmt);
		Buf.appendfv(fmt, args);
		va_end(args);
		for (int new_size = Buf.size(); old_size < new_size; old_size++)
			if (Buf[old_size] == '\n')
				LineOffsets.push_back(old_size + 1);
	}

	void    Draw(const char* title, bool* p_open = NULL)
	{
		if (!ImGui::Begin(title, p_open))
		{
			ImGui::End();
			return;
		}

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		// Main window
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");
		ImGui::SameLine();
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		Filter.Draw("Filter", -100.0f);

		ImGui::Separator();

		if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			if (clear)
				Clear();
			if (copy)
				ImGui::LogToClipboard();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			const char* buf = Buf.begin();
			const char* buf_end = Buf.end();
			if (Filter.IsActive())
			{
				// In this example we don't use the clipper when Filter is enabled.
				// This is because we don't have random access to the result of our filter.
				// A real application processing logs with ten of thousands of entries may want to store the result of
				// search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
				for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
				{
					const char* line_start = buf + LineOffsets[line_no];
					const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
					if (Filter.PassFilter(line_start, line_end))
						ImGui::TextUnformatted(line_start, line_end);
				}
			}
			else
			{
				// The simplest and easy way to display the entire buffer:
				//   ImGui::TextUnformatted(buf_begin, buf_end);
				// And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
				// to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
				// within the visible area.
				// If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
				// on your side is recommended. Using ImGuiListClipper requires
				// - A) random access into your data
				// - B) items all being the  same height,
				// both of which we can handle since we have an array pointing to the beginning of each line of text.
				// When using the filter (in the block of code above) we don't have random access into the data to display
				// anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
				// it possible (and would be recommended if you want to search through tens of thousands of entries).
				ImGuiListClipper clipper;
				clipper.Begin(LineOffsets.Size);
				while (clipper.Step())
				{
					for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
					{
						const char* line_start = buf + LineOffsets[line_no];
						const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
						ImGui::TextUnformatted(line_start, line_end);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();

			// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
			// Using a scrollbar or mouse-wheel will take away from the bottom edge.
			if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
		}
		ImGui::EndChild();
		ImGui::End();
	}
};
ExampleAppLog assemblyLogs;
static void ShowExampleAppLog(bool* p_open)
{

	static bool Log_no_titlebar = true;
	static bool Log_no_scrollbar = true;
	static bool Log_no_menu = true;
	static bool Log_no_move = true;
	static bool Log_no_resize = true;
	static bool Log_no_collapse = false;
	static bool Log_no_close = false;
	static bool Log_no_nav = true;
	static bool Log_no_background = false;
	static bool Log_no_bring_to_front = false;
	static bool Log_unsaved_document = false;

	ImGuiWindowFlags Log_flags = 1;
	if (Log_no_titlebar)        Log_flags |= ImGuiWindowFlags_NoTitleBar;
	if (Log_no_scrollbar)       Log_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!Log_no_menu)           Log_flags |= ImGuiWindowFlags_MenuBar;
	if (Log_no_move)            Log_flags |= ImGuiWindowFlags_NoMove;
	if (Log_no_resize)          Log_flags |= ImGuiWindowFlags_NoResize;
	if (Log_no_collapse)        Log_flags |= ImGuiWindowFlags_NoCollapse;
	if (Log_no_nav)             Log_flags |= ImGuiWindowFlags_NoNav;
	if (Log_no_background)      Log_flags |= ImGuiWindowFlags_NoBackground;
	if (Log_no_bring_to_front)  Log_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (Log_unsaved_document)   Log_flags |= ImGuiWindowFlags_UnsavedDocument;

	// For the demo: add a debug button _BEFORE_ the normal log window contents
	// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
	// Most of the contents of the window will be added by the log.Draw() call.
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Log", p_open, Log_flags);
	if (ImGui::SmallButton("[Debug] Add 5 entries"))
	{
		static int counter = 0;
		const char* categories[3] = { "info", "warn", "error" };
		const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
		for (int n = 0; n < 5; n++)
		{
			const char* category = categories[counter % IM_ARRAYSIZE(categories)];
			const char* word = words[counter % IM_ARRAYSIZE(words)];
			assemblyLogs.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
				ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
			counter++;
		}
	}
	ImGui::End();

	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	assemblyLogs.Draw("Log", p_open);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}
//This loads the correct image for the body machine using the order
string loadBodyMachineImage(Order order)
{
	string startOfPath = "Images/GUI/Body Machine/";
	string fileName = "NoFileNameFound";

	if (order.getYear() == "2022")
	{
		if (order.getModel() == "F150")
		{
			if (order.getBodyPanelSet() == "Regular")
			{
				fileName = "RegF150.jpg";
			}
			else if (order.getBodyPanelSet() == "SuperCab")
			{
				fileName = "SprCabF150.jpg";
			}
			else if (order.getBodyPanelSet() == "SuperCrew")
			{
				fileName = "SprCrwF150.jpg";
			}
		}
	}
	else if (order.getYear() == "2023")
	{
		if (order.getModel() == "Expedition")
		{
			if (order.getBodyPanelSet() == "REG")
			{
				fileName = "RegExp.jpg";
			}
			else if (order.getBodyPanelSet() == "MAX")
			{
				fileName = "MaxExp.jpg";
			}
		}
	}
	return startOfPath + fileName;
}

//This loads the correct image for the chassis machine using the order
string loadChassisMachineImage(Order order)
{
	string startOfPath = "Images/GUI/Chassis Machine/";

	return startOfPath + order.getYear() + " " + order.getModel() + " " + order.getTrim() + " " + order.getColour() + ".jpg";
}

//This loads the correct image for the interior machine using the order
string loadInteriorMachineImage(Order order)
{
	string startOfPath = "Images/GUI/Interior Machine/";
	string fileName = "NoFileNameFound";

	if (order.getYear() == "2022")
	{
		if (order.getModel() == "F150")
		{
			if (order.getInteriorLevel() == "high")
			{
				fileName = "2022 High F150.jpg";
			}
			else if (order.getInteriorLevel() == "base")
			{
				fileName = "2022 Base F150.jpg";
			}
			else if (order.getInteriorLevel() == "mid")
			{
				fileName = "2022 Mid F150.jpg";
			}
		}
	}
	else if (order.getYear() == "2023")
	{
		if (order.getModel() == "Expedition")
		{
			if (order.getInteriorLevel() == "high")
			{
				fileName = "2022 High Expedition.jpg";
			}
			else if (order.getInteriorLevel() == "base")
			{
				fileName = "2022 Base Expedition.jpg";
			}
			else if (order.getInteriorLevel() == "mid")
			{
				fileName = "2022 Mid Expedition.jpg";
			}
		}
	}
	return startOfPath + fileName;
}

//This loads the correct image for the paint machine using the order
string loadPaintMachineImage(Order order)
{
	string startOfPath = "Images/GUI/Paint Machine/";

	return startOfPath + order.getYear() + " " + order.getBodyPanelSet() + " " + order.getModel() + " " + order.getColour() + ".jpg";
}

//This loads the correct image for displaying the order using the order
string loadOrderImage(Order order)
{
	string startOfPath = "Images/GUI/Orders/";

	return startOfPath + order.getYear() + " " + order.getModel() + " " + order.getTrim() + " " + order.getColour() + ".jpg";
}



int main()
{
	Plant plant;
	sampleDataCreator();
	
	//
	

	//PAINT MACHINE DATA FLOW
	/*plant.paintingMachine.identifyRGBvalues("PlantColours.txt");*/ // each machine loop





	static int e = 0;
	static bool check = true;

	const bool controlMenu_no_titlebar = false;
	const bool controlMenu_no_scrollbar = true;
	const bool controlMenu_no_menu = false;
	const bool controlMenu_no_move = false;
	const bool controlMenu_no_resize = false;
	const bool controlMenu_no_collapse = false;
	const bool controlMenu_no_close = false;
	const bool controlMenu_no_nav = false;
	static bool controlMenu_no_bring_to_front = false;
	const bool controlMenu_no_background = false;
	const bool controlMenu_unsaved_document = false;

	const bool InvBody1_no_titlebar = true;
	const bool InvBody1_no_scrollbar = true;
	const bool InvBody1_no_menu = true;
	const bool InvBody1_no_move = true;
	const bool InvBody1_no_resize = true;
	const bool InvBody1_no_collapse = false;
	const bool InvBody1_no_close = false;
	const bool InvBody1_no_nav = true;
	static bool InvBody1_no_bring_to_front = false;
	const bool InvBody1_no_background = false;
	const bool InvBody1_unsaved_document = false;


	const bool InvChassis_no_titlebar = true;
	const bool InvChassis_no_scrollbar = true;
	const bool InvChassis_no_menu = true;
	const bool InvChassis_no_move = true;
	const bool InvChassis_no_resize = true;
	const bool InvChassis_no_collapse = false;
	const bool InvChassis_no_close = false;
	const bool InvChassis_no_nav = false;
	static bool InvChassis_no_bring_to_front = false;
	const bool InvChassis_no_background = false;
	const bool InvChassis_unsaved_document = false;

	const bool InvInterior_no_titlebar = true;
	const bool InvInterior_no_scrollbar = true;
	const bool InvInterior_no_menu = true;
	const bool InvInterior_no_move = true;
	const bool InvInterior_no_resize = true;
	const bool InvInterior_no_collapse = false;
	const bool InvInterior_no_close = false;
	const bool InvInterior_no_nav = false;
	static bool InvInterior_no_bring_to_front = false;
	const bool InvInterior_no_background = false;
	const bool InvInterior_unsaved_document = false;

	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "ImGui + GLFW", glfwGetPrimaryMonitor(), NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 1280, 1024);



	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Value boxes for each inventory
//BAY 1
	int bay1MaxExp = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay1RegExp = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay1RegCab = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay1SupCab = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay1SupCrew = 5; // INVENTORY LEVELS for bay 1 super crew f150
	//Value boxes for each inventory
			//BAY 2
	int bay2MaxExp = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay2RegExp = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay2RegCab = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay2SupCab = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay2SupCrew = 5; // INVENTORY LEVELS for bay 1 super crew f150


	//Value boxes for each inventory
//BAY 1
	int bay135LV6C = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay135LV6HOC = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay127LV6C = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay133LV6C = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay135LV6EcoC = 5; // INVENTORY LEVELS for bay 1 super crew f150
	int bay135LV6PwrBstC = 6; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay150LV8C = 7; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	//Value boxes for each inventory
			//BAY 2
	int bay235LV6C = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay235LV6HOC = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay227LV6C = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay233LV6C = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay235LV6EcoC = 5; // INVENTORY LEVELS for bay 1 super crew f150
	int bay235LV6PwrBstC = 6; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay250LV8C = 7; // INVENTORY LEVELS FOR bay 1 Reg Expedition


	//Value boxes for each inventory
//BAY 1
	int bay1BaseExp = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay1MidExp = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay1HighExp = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay1BaseF150 = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay1MidF150 = 5; // INVENTORY LEVELS for bay 1 super crew f150
	int bay1HighF150 = 6; // INVENTORY LEVELS FOR Bay 1 Max Expedition

	//Value boxes for each inventory
			//BAY 2
	int bay2BaseExp = 1; // INVENTORY LEVELS FOR Bay 1 Max Expedition
	int bay2MidExp = 2; // INVENTORY LEVELS FOR bay 1 Reg Expedition
	int bay2HighExp = 3; // INVENTORY LEVELS FOR bay 1 Reg Cab f150
	int bay2BaseF150 = 4; // INVENTORY LEVELS For bay 1 Super Cab f150
	int bay2MidF150 = 5; // INVENTORY LEVELS for bay 1 super crew f150
	int bay2HighF150 = 6; // INVENTORY LEVELS FOR Bay 1 Max Expedition


	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO


	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	 //Variables to be changed in the ImGUI window
	bool drawTriangle = true;
	float size = 1.0f;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	static int bodyitem_current = 0;
	static int chassisitem_current = 0;
	static int Interioritem_current = 0;
	static int PaintVatitem_current = 0;
	static int PaintMachineitem_current = 0;
	int paintRedVatVol = 0;
	int paintGreenVatVol = 0;
	int paintBlueVatVol = 0;
	double DipTankTemp = 0.0;
	int DipTankFluidLevel = 0;
	double PaintChamberTemp = 0.0;
	int PaintChamberHMD = 0;
	double DryChamberTemp = 0.0;
	int DryChamberHMD =0;
	int globalTemp =0;
	int GlobalAir = 0;
	int VeQuota = 0;
	int VeComp = 0;
	int i = 0;


	// Exporting variables to shaders
	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
	glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);


	int my_image_width = 1280;
	int my_image_height = 1024;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile("Images/Ford_Assembly_Line_GUI_Background.png", &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool p = true;
	bool* open = &p;

	int toBeMade_image_width = 240;
	int toBeMade_image_height = 121;
	GLuint toBeMade_image_texture = 0;
	//bool toBeMade = LoadTextureFromFile("Images/GUI/Orders/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
	//IM_ASSERT(toBeMade);

	int body_image_width = 350;
	int body_image_height = 179;
	GLuint body_image_texture = 0;
	//bool body = LoadTextureFromFile("Images/GUI/Body Machine/MaxExp.jpg", &body_image_texture, &body_image_width, &body_image_height);
	//IM_ASSERT(body);

	int chassis_image_width = 350;
	int chassis_image_height = 179;
	GLuint chassis_image_texture = 0;
	//bool chassis = LoadTextureFromFile("Images/GUI/Chassis Machine/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
	//IM_ASSERT(chassis);

	int Interior_image_width = 350;
	int Interior_image_height = 179;
	GLuint Interior_image_texture = 0;
	//bool Interior = LoadTextureFromFile("Images/GUI/Interior Machine/2022 Base F150.jpg", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
	//IM_ASSERT(Interior);

	int Paint_image_width = 350;
	int Paint_image_height = 179;
	GLuint Paint_image_texture = 0;
	//bool Paint = LoadTextureFromFile("Images/GUI/Paint Machine/2022 Regular F150 Antimatter Blue Metallic.jpg", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
	//IM_ASSERT(Paint);


	bool RedPaintVat = false;
	int RedPaintVat_image_width = 90;
	int RedPaintVat_image_height = 113;
	GLuint RedPaintVat_image_texture = 0;
	if (paintRedVatVol >= 250) {
		RedPaintVat = LoadTextureFromFile("Images/Redpaintfull.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
	}
	else if (paintRedVatVol >= 100 && paintRedVatVol <= 249) {
		RedPaintVat = LoadTextureFromFile("Images/Redpainthalf.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
	}
	else if (paintRedVatVol <= 99 && paintRedVatVol >= 0) {
		RedPaintVat = LoadTextureFromFile("Images/redpaintlow.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
	}

	IM_ASSERT(RedPaintVat);

	bool GreenPaintVat;
	int GreenPaintVat_image_width = 90;
	int GreenPaintVat_image_height = 113;
	GLuint GreenPaintVat_image_texture = 0;
	if (paintGreenVatVol >= 250) {
		GreenPaintVat = LoadTextureFromFile("Images/greenpaintfull.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
	}
	else if (paintGreenVatVol >= 100 && paintGreenVatVol <= 249) {
		GreenPaintVat = LoadTextureFromFile("Images/greenpainthalf.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
	}
	else if (paintGreenVatVol <= 99 && paintGreenVatVol >= 0) {
		GreenPaintVat = LoadTextureFromFile("Images/greenpaintlow.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
	}

	IM_ASSERT(GreenPaintVat);

	bool BluePaintVat = false;
	int BluePaintVat_image_width = 90;
	int BluePaintVat_image_height = 113;
	GLuint BluePaintVat_image_texture = 0;
	if (paintBlueVatVol >= 250) {
		BluePaintVat = LoadTextureFromFile("Images/bluepaintfull.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
	}
	else if (paintBlueVatVol >= 100 && paintBlueVatVol <= 249) {
		BluePaintVat = LoadTextureFromFile("Images/bluepainthalf.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
	}
	else if (paintBlueVatVol <= 99 && paintBlueVatVol >= 0) {
		BluePaintVat = LoadTextureFromFile("Images/bluepaintlow.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
	}

	IM_ASSERT(BluePaintVat);

	int DipTank_image_width = 350;
	int DipTank_image_height = 179;
	GLuint DipTank_image_texture = 0;
	bool DipTank = LoadTextureFromFile("Images/Diptank.png", &DipTank_image_texture, &DipTank_image_width, &DipTank_image_height);
	IM_ASSERT(DipTank);


	int PaintChamber_image_width = 350;
	int PaintChamber_image_height = 179;
	GLuint PaintChamber_image_texture = 0;
	bool PaintChamber = LoadTextureFromFile("Images/DryingchamberIcon.png", &PaintChamber_image_texture, &PaintChamber_image_width, &PaintChamber_image_height);
	IM_ASSERT(PaintChamber);


	bool bodyMachine = true;
	bool paintMachine = true;
	bool chassisMachine = true;
	bool interiorMachine = true;
	bool ClosedHMI = false;
	
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		bool changePaintValues = true;

		plant.order.loadOrder("Order.txt");
		
		readLog(&plant);

		plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");
		plant.dipTank.readTemperature("DipTankTemperature.txt");
		plant.dipTank.readfluidLevel("DipTankFluidLevel.txt");
		plant.paintChamber.readTemperature("PaintChamberTemperature.txt");
		plant.paintChamber.readHumidity("PaintChamberHumidity.txt");
		plant.dryingChamber.readTemperature("DryingChamberTemperature.txt");
		plant.dryingChamber.readHumidity("DryingChamberHumidity.txt");

		paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
		paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
		paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();
		DipTankTemp = plant.dipTank.getTemperature();
		DipTankFluidLevel = plant.dipTank.getfluidLevel();
		PaintChamberTemp = plant.paintChamber.getTemperature();
		PaintChamberHMD = plant.paintChamber.getHumidity();
		DryChamberTemp = plant.dryingChamber.getTemperature();
		DryChamberHMD = plant.dryingChamber.getHumidity();
		globalTemp = plant.getGlobalTemp();
		GlobalAir = plant.getGlobalAirQuality();
		VeQuota = plant.getVehicleQuota();
		VeComp = plant.getNumVehicleToday();
		
		//Value boxes for each inventory
//BAY 1
		int bay1MaxExp = plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay1RegExp = plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay1RegCab = plant.bodyMachine.bayOne.GetRegularF150InventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay1SupCab = plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay1SupCrew = plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150
		//Value boxes for each inventory
				//BAY 2
		int bay2MaxExp = plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay2RegExp = plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay2RegCab = plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay2SupCab = plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay2SupCrew = plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150


		//Value boxes for each inventory
	//BAY 1
		int bay135LV6C = plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay135LV6HOC = plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay127LV6C = plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay133LV6C = plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay135LV6EcoC = plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150
		int bay135LV6PwrBstC = plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay150LV8C = plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		//Value boxes for each inventory
				//BAY 2
		int bay235LV6C = plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay235LV6HOC = plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay227LV6C = plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay233LV6C = plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay235LV6EcoC = plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150
		int bay235LV6PwrBstC = plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay250LV8C = plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition


		//Value boxes for each inventory
	//BAY 1
		int bay1BaseExp = plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay1MidExp = plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay1HighExp = plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay1BaseF150 = plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay1MidF150 = plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150
		int bay1HighF150 = plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition

		//Value boxes for each inventory
				//BAY 2
		int bay2BaseExp = plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition
		int bay2MidExp = plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Expedition
		int bay2HighExp = plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount(); // INVENTORY LEVELS FOR bay 1 Reg Cab f150
		int bay2BaseF150 = plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount(); // INVENTORY LEVELS For bay 1 Super Cab f150
		int bay2MidF150 = plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount(); // INVENTORY LEVELS for bay 1 super crew f150
		int bay2HighF150 = plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount(); // INVENTORY LEVELS FOR Bay 1 Max Expedition


			while (bodyMachine) {

				glfwPollEvents();

				//changing values from order to body machine.



				//end of changing values.
			
				
				if (paintRedVatVol >= 250) {
					RedPaintVat = LoadTextureFromFile("Images/Redpaintfull.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol >= 100 && paintRedVatVol <= 249) {
					RedPaintVat = LoadTextureFromFile("Images/Redpainthalf.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol <= 99 && paintRedVatVol >= 0) {
					RedPaintVat = LoadTextureFromFile("Images/redpaintlow.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}

				if (paintGreenVatVol >= 250) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintfull.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol >= 100 && paintGreenVatVol <= 249) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpainthalf.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol <= 99 && paintGreenVatVol >= 0) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintlow.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				if (paintBlueVatVol >= 250) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintfull.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol >= 100 && paintBlueVatVol <= 249) {
					BluePaintVat = LoadTextureFromFile("Images/bluepainthalf.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol <= 99 && paintBlueVatVol >= 0) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintlow.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}

				bool body = LoadTextureFromFile("Images/GUI/Body Machine/MaxExp.jpg", &body_image_texture, &body_image_width, &body_image_height);
				IM_ASSERT(body);

				bool toBeMade = LoadTextureFromFile("Images/GUI/Orders/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
				IM_ASSERT(toBeMade);

				//Set to blank
				bool chassis = LoadTextureFromFile("Images/Blank.png", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
				IM_ASSERT(chassis);


				//set to blank
				bool Paint = LoadTextureFromFile("Images/Blank.png", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
				IM_ASSERT(Paint);

				//set to blank
				bool Interior = LoadTextureFromFile("Images/Blank.png", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
				IM_ASSERT(Interior);


				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();



				// Tell OpenGL which Shader Program we want to use
				glUseProgram(shaderProgram);
				// Bind the VAO so OpenGL knows to use it
				glBindVertexArray(VAO);


				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



					//BAY SELECTION TOGGLE and restock button
				const bool InvBodyButtons_no_titlebar = true;
				const bool InvBodyButtons_no_scrollbar = true;
				const bool InvBodyButtons_no_menu = true;
				const bool InvBodyButtons_no_move = true;
				const bool InvBodyButtons_no_resize = true;
				const bool InvBodyButtons_no_collapse = false;
				const bool InvBodyButtons_no_close = false;
				const bool InvBodyButtons_no_nav = true;
				static bool InvBodyButtons_no_bring_to_front = false;
				const bool InvBodyButtons_no_background = false;
				const bool InvBodyButtons_unsaved_document = false;

				ImGuiWindowFlags bodyButtonflags = 1;
				if (InvBodyButtons_no_titlebar)        bodyButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBodyButtons_no_scrollbar)       bodyButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBodyButtons_no_menu)           bodyButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvBodyButtons_no_move)            bodyButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvBodyButtons_no_resize)          bodyButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvBodyButtons_no_collapse)        bodyButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvBodyButtons_no_nav)             bodyButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvBodyButtons_no_background)      bodyButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvBodyButtons_no_bring_to_front)  bodyButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBodyButtons_unsaved_document)   bodyButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBodyButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Body Machine Manager", open, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						
							/*
							* Bay 1 if bay 2 is selected you can only restock bay 1
							Set inventory levels for that body type
							using the methods from body inventory bay

							use : bodyitem_current to get the body type selected
							i0 to get the value for body type if greater than 500 set = 500
							if < 0 = 0
							*/
						

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						
							/*
							* BAY 2 selected
							Set inventory levels for that body type
							using the methods from body inventory bay

							use : bodyitem_current to get the body type selected
							i0 to get the value for body type if greater than 500 set = 500
							if < 0 = 0
							*/
					

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();

				ImGuiWindowFlags controlMenuFlags = 1;
				if (controlMenu_no_titlebar)        controlMenuFlags |= ImGuiWindowFlags_NoTitleBar;
				if (controlMenu_no_scrollbar)       controlMenuFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!controlMenu_no_menu)           controlMenuFlags |= ImGuiWindowFlags_MenuBar;
				if (controlMenu_no_move)            controlMenuFlags |= ImGuiWindowFlags_NoMove;
				if (controlMenu_no_resize)          controlMenuFlags |= ImGuiWindowFlags_NoResize;
				if (controlMenu_no_collapse)        controlMenuFlags |= ImGuiWindowFlags_NoCollapse;
				if (controlMenu_no_nav)             controlMenuFlags |= ImGuiWindowFlags_NoNav;
				if (controlMenu_no_background)      controlMenuFlags |= ImGuiWindowFlags_NoBackground;
				if (controlMenu_no_bring_to_front)  controlMenuFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (controlMenu_unsaved_document)   controlMenuFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (controlMenu_no_close)           open = NULL; // Don't pass our bool* to Begin
				// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
			// Most functions would normally just crash if the context is missing.
				IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

				//CONTROL MENU CREATION
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.13f, 0.17f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(300, 100));
				ImGui::Begin("Control Menu", open, controlMenuFlags);
				if (ImGui::Button("Next Machine")) {
					
					bodyMachine = false;
					paintMachine = true;
				}
				if (ImGui::Button("Close HMI")) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;

					// Deletes all ImGUI instances
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();

					glfwDestroyWindow(window);

					glfwTerminate();
					ClosedHMI = true;
					return 0;
				}
				ImGui::Separator();

				if (ImGui::Checkbox("Start Line", &check)) {};


				ImGui::PopStyleColor();
				ImGui::End();


				ImGuiWindowFlags inventoryFlags = 1;
				if (InvBody1_no_titlebar)        inventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBody1_no_scrollbar)       inventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBody1_no_menu)           inventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvBody1_no_move)            inventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvBody1_no_resize)          inventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvBody1_no_collapse)        inventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvBody1_no_nav)             inventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvBody1_no_background)      inventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvBody1_no_bring_to_front)  inventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBody1_unsaved_document)   inventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBody1_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(50, 300));
				ImGui::Begin("bay1InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay1RegExp);
				ImGui::Text("%d\n\n\n\n", bay1RegCab);
				ImGui::Text("%d\n\n\n\n", bay1SupCab);
				ImGui::Text("%d", bay1SupCrew);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay2RegExp);
				ImGui::Text("%d\n\n\n\n", bay2RegCab);
				ImGui::Text("%d\n\n\n\n", bay2SupCab);
				ImGui::Text("%d", bay2SupCrew);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool BodyImage_no_titlebar = true;
				static bool BodyImage_no_scrollbar = true;
				static bool BodyImage_no_menu = true;
				static bool BodyImage_no_move = true;
				static bool BodyImage_no_resize = true;
				static bool BodyImage_no_collapse = false;
				static bool BodyImage_no_close = false;
				static bool BodyImage_no_nav = true;
				static bool BodyImage_no_background = false;
				static bool BodyImage_no_bring_to_front = false;
				static bool BodyImage_unsaved_document = false;

				ImGuiWindowFlags bodyImage_flags = 1;
				if (BodyImage_no_titlebar)        bodyImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BodyImage_no_scrollbar)       bodyImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BodyImage_no_menu)           bodyImage_flags |= ImGuiWindowFlags_MenuBar;
				if (BodyImage_no_move)            bodyImage_flags |= ImGuiWindowFlags_NoMove;
				if (BodyImage_no_resize)          bodyImage_flags |= ImGuiWindowFlags_NoResize;
				if (BodyImage_no_collapse)        bodyImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (BodyImage_no_nav)             bodyImage_flags |= ImGuiWindowFlags_NoNav;
				if (BodyImage_no_background)      bodyImage_flags |= ImGuiWindowFlags_NoBackground;
				if (BodyImage_no_bring_to_front)  bodyImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BodyImage_unsaved_document)   bodyImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BodyImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Body Machine Image", open, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2(body_image_width, body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvChassisButtons_no_titlebar = true;
				const bool InvChassisButtons_no_scrollbar = true;
				const bool InvChassisButtons_no_menu = true;
				const bool InvChassisButtons_no_move = true;
				const bool InvChassisButtons_no_resize = true;
				const bool InvChassisButtons_no_collapse = false;
				const bool InvChassisButtons_no_close = false;
				const bool InvChassisButtons_no_nav = false;
				static bool InvChassisButtons_no_bring_to_front = false;
				const bool InvChassisButtons_no_background = false;
				const bool InvChassisButtons_unsaved_document = false;

				ImGuiWindowFlags ChassisButtonflags = 1;
				if (InvChassisButtons_no_titlebar)        ChassisButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassisButtons_no_scrollbar)       ChassisButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassisButtons_no_menu)           ChassisButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvChassisButtons_no_move)            ChassisButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvChassisButtons_no_resize)          ChassisButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvChassisButtons_no_collapse)        ChassisButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassisButtons_no_nav)             ChassisButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvChassisButtons_no_background)      ChassisButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvChassisButtons_no_bring_to_front)  ChassisButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassisButtons_unsaved_document)   ChassisButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassisButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Chassis Machine Manager", open, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int b;
				ImGui::RadioButton("Bay 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &b, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (b == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 2 if bay 1 is selected you can only restock bay 2
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags ChassisinventoryFlags = 0;
				if (InvChassis_no_titlebar)        ChassisinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassis_no_scrollbar)       ChassisinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassis_no_menu)           ChassisinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvChassis_no_move)            ChassisinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvChassis_no_resize)          ChassisinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvChassis_no_collapse)        ChassisinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassis_no_nav)             ChassisinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvChassis_no_background)      ChassisinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvChassis_no_bring_to_front)  ChassisinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassis_unsaved_document)   ChassisinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassis_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay135LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6HOC);
				ImGui::Text("%d\n\n\n", bay127LV6C);
				ImGui::Text("%d\n\n\n", bay133LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay135LV6PwrBstC);
				ImGui::Text("%d", bay150LV8C);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay235LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6HOC);
				ImGui::Text("%d\n\n\n", bay227LV6C);
				ImGui::Text("%d\n\n\n", bay233LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay235LV6PwrBstC);
				ImGui::Text("%d", bay250LV8C);
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool ChassisImage_no_titlebar = true;
				static bool ChassisImage_no_scrollbar = true;
				static bool ChassisImage_no_menu = true;
				static bool ChassisImage_no_move = true;
				static bool ChassisImage_no_resize = true;
				static bool ChassisImage_no_collapse = false;
				static bool ChassisImage_no_close = false;
				static bool ChassisImage_no_nav = true;
				static bool ChassisImage_no_background = false;
				static bool ChassisImage_no_bring_to_front = false;
				static bool ChassisImage_unsaved_document = false;

				ImGuiWindowFlags ChassisImage_flags = 1;
				if (ChassisImage_no_titlebar)        ChassisImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (ChassisImage_no_scrollbar)       ChassisImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!ChassisImage_no_menu)           ChassisImage_flags |= ImGuiWindowFlags_MenuBar;
				if (ChassisImage_no_move)            ChassisImage_flags |= ImGuiWindowFlags_NoMove;
				if (ChassisImage_no_resize)          ChassisImage_flags |= ImGuiWindowFlags_NoResize;
				if (ChassisImage_no_collapse)        ChassisImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (ChassisImage_no_nav)             ChassisImage_flags |= ImGuiWindowFlags_NoNav;
				if (ChassisImage_no_background)      ChassisImage_flags |= ImGuiWindowFlags_NoBackground;
				if (ChassisImage_no_bring_to_front)  ChassisImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (ChassisImage_unsaved_document)   ChassisImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (ChassisImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Chassis Machine Image", open, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2(chassis_image_width, chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvInteriorButtons_no_titlebar = true;
				const bool InvInteriorButtons_no_scrollbar = true;
				const bool InvInteriorButtons_no_menu = true;
				const bool InvInteriorButtons_no_move = true;
				const bool InvInteriorButtons_no_resize = true;
				const bool InvInteriorButtons_no_collapse = false;
				const bool InvInteriorButtons_no_close = false;
				const bool InvInteriorButtons_no_nav = false;
				static bool InvInteriorButtons_no_bring_to_front = false;
				const bool InvInteriorButtons_no_background = false;
				const bool InvInteriorButtons_unsaved_document = false;

				ImGuiWindowFlags InteriorButtonflags = 1;
				if (InvInteriorButtons_no_titlebar)        InteriorButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInteriorButtons_no_scrollbar)       InteriorButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInteriorButtons_no_menu)           InteriorButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvInteriorButtons_no_move)            InteriorButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvInteriorButtons_no_resize)          InteriorButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvInteriorButtons_no_collapse)        InteriorButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvInteriorButtons_no_nav)             InteriorButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvInteriorButtons_no_background)      InteriorButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvInteriorButtons_no_bring_to_front)  InteriorButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInteriorButtons_unsaved_document)   InteriorButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInteriorButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Interior Machine Manager", open, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int c;
				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from Interior inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags InteriorinventoryFlags = 1;
				if (InvInterior_no_titlebar)        InteriorinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInterior_no_scrollbar)       InteriorinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInterior_no_menu)           InteriorinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvInterior_no_move)            InteriorinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvInterior_no_resize)          InteriorinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvInterior_no_collapse)        InteriorinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvInterior_no_nav)             InteriorinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvInterior_no_background)      InteriorinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvInterior_no_bring_to_front)  InteriorinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInterior_unsaved_document)   InteriorinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInterior_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay1BaseExp);
				ImGui::Text("%d\n\n\n\n", bay1MidExp);
				ImGui::Text("%d\n\n\n", bay1HighExp);
				ImGui::Text("%d\n\n\n\n", bay1BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay1MidF150);
				ImGui::Text("%d", bay1HighF150);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay2BaseExp);
				ImGui::Text("%d\n\n\n\n", bay2MidExp);
				ImGui::Text("%d\n\n\n", bay2HighExp);
				ImGui::Text("%d\n\n\n\n", bay2BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay2MidF150);
				ImGui::Text("%d", bay2HighF150);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool InteriorImage_no_titlebar = true;
				static bool InteriorImage_no_scrollbar = true;
				static bool InteriorImage_no_menu = true;
				static bool InteriorImage_no_move = true;
				static bool InteriorImage_no_resize = false;
				static bool InteriorImage_no_collapse = false;
				static bool InteriorImage_no_close = false;
				static bool InteriorImage_no_nav = true;
				static bool InteriorImage_no_background = false;
				static bool InteriorImage_no_bring_to_front = false;
				static bool InteriorImage_unsaved_document = false;

				ImGuiWindowFlags InteriorImage_flags = 1;
				if (InteriorImage_no_titlebar)        InteriorImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (InteriorImage_no_scrollbar)       InteriorImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!InteriorImage_no_menu)           InteriorImage_flags |= ImGuiWindowFlags_MenuBar;
				if (InteriorImage_no_move)            InteriorImage_flags |= ImGuiWindowFlags_NoMove;
				if (InteriorImage_no_resize)          InteriorImage_flags |= ImGuiWindowFlags_NoResize;
				if (InteriorImage_no_collapse)        InteriorImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (InteriorImage_no_nav)             InteriorImage_flags |= ImGuiWindowFlags_NoNav;
				if (InteriorImage_no_background)      InteriorImage_flags |= ImGuiWindowFlags_NoBackground;
				if (InteriorImage_no_bring_to_front)  InteriorImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InteriorImage_unsaved_document)   InteriorImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (InteriorImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Interior Machine Image", open, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2(Interior_image_width, Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE


				static bool RedPaintVat_no_titlebar = true;
				static bool RedPaintVat_no_scrollbar = true;
				static bool RedPaintVat_no_menu = true;
				static bool RedPaintVat_no_move = true;
				static bool RedPaintVat_no_resize = true;
				static bool RedPaintVat_no_collapse = false;
				static bool RedPaintVat_no_close = false;
				static bool RedPaintVat_no_nav = true;
				static bool RedPaintVat_no_background = false;
				static bool RedPaintVat_no_bring_to_front = false;
				static bool RedPaintVat_unsaved_document = false;

				ImGuiWindowFlags RedPaintVat_flags = 1;
				if (RedPaintVat_no_titlebar)        RedPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (RedPaintVat_no_scrollbar)       RedPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!RedPaintVat_no_menu)           RedPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (RedPaintVat_no_move)            RedPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (RedPaintVat_no_resize)          RedPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (RedPaintVat_no_collapse)        RedPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (RedPaintVat_no_nav)             RedPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (RedPaintVat_no_background)      RedPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (RedPaintVat_no_bring_to_front)  RedPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (RedPaintVat_unsaved_document)   RedPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (RedPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("RedPaintVat Image", NULL, RedPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2(RedPaintVat_image_width, RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat

				static bool BluePaintVat_no_titlebar = true;
				static bool BluePaintVat_no_scrollbar = true;
				static bool BluePaintVat_no_menu = true;
				static bool BluePaintVat_no_move = true;
				static bool BluePaintVat_no_resize = true;
				static bool BluePaintVat_no_collapse = false;
				static bool BluePaintVat_no_close = false;
				static bool BluePaintVat_no_nav = true;
				static bool BluePaintVat_no_background = false;
				static bool BluePaintVat_no_bring_to_front = false;
				static bool BluePaintVat_unsaved_document = false;

				ImGuiWindowFlags BluePaintVat_flags = 1;
				if (BluePaintVat_no_titlebar)        BluePaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BluePaintVat_no_scrollbar)       BluePaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BluePaintVat_no_menu)           BluePaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (BluePaintVat_no_move)            BluePaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (BluePaintVat_no_resize)          BluePaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (BluePaintVat_no_collapse)        BluePaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (BluePaintVat_no_nav)             BluePaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (BluePaintVat_no_background)      BluePaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (BluePaintVat_no_bring_to_front)  BluePaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BluePaintVat_unsaved_document)   BluePaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BluePaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("BluePaintVat Image", NULL, BluePaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2(BluePaintVat_image_width, BluePaintVat_image_height));
				ImGui::End();

				//Green Vat

				static bool GreenPaintVat_no_titlebar = true;
				static bool GreenPaintVat_no_scrollbar = true;
				static bool GreenPaintVat_no_menu = true;
				static bool GreenPaintVat_no_move = true;
				static bool GreenPaintVat_no_resize = true;
				static bool GreenPaintVat_no_collapse = false;
				static bool GreenPaintVat_no_close = false;
				static bool GreenPaintVat_no_nav = true;
				static bool GreenPaintVat_no_background = false;
				static bool GreenPaintVat_no_bring_to_front = false;
				static bool GreenPaintVat_unsaved_document = false;

				ImGuiWindowFlags GreenPaintVat_flags = 1;
				if (GreenPaintVat_no_titlebar)        GreenPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GreenPaintVat_no_scrollbar)       GreenPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GreenPaintVat_no_menu)           GreenPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (GreenPaintVat_no_move)            GreenPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (GreenPaintVat_no_resize)          GreenPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (GreenPaintVat_no_collapse)        GreenPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (GreenPaintVat_no_nav)             GreenPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (GreenPaintVat_no_background)      GreenPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (GreenPaintVat_no_bring_to_front)  GreenPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GreenPaintVat_unsaved_document)   GreenPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GreenPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("GreenPaintVat Image", NULL, GreenPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2(GreenPaintVat_image_width, GreenPaintVat_image_height));
				ImGui::End();

				//DipTank

				static bool DipTank_no_titlebar = true;
				static bool DipTank_no_scrollbar = true;
				static bool DipTank_no_menu = true;
				static bool DipTank_no_move = true;
				static bool DipTank_no_resize = true;
				static bool DipTank_no_collapse = false;
				static bool DipTank_no_close = false;
				static bool DipTank_no_nav = true;
				static bool DipTank_no_background = false;
				static bool DipTank_no_bring_to_front = false;
				static bool DipTank_unsaved_document = false;

				ImGuiWindowFlags DipTank_flags = 1;
				if (DipTank_no_titlebar)        DipTank_flags |= ImGuiWindowFlags_NoTitleBar;
				if (DipTank_no_scrollbar)       DipTank_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!DipTank_no_menu)           DipTank_flags |= ImGuiWindowFlags_MenuBar;
				if (DipTank_no_move)            DipTank_flags |= ImGuiWindowFlags_NoMove;
				if (DipTank_no_resize)          DipTank_flags |= ImGuiWindowFlags_NoResize;
				if (DipTank_no_collapse)        DipTank_flags |= ImGuiWindowFlags_NoCollapse;
				if (DipTank_no_nav)             DipTank_flags |= ImGuiWindowFlags_NoNav;
				if (DipTank_no_background)      DipTank_flags |= ImGuiWindowFlags_NoBackground;
				if (DipTank_no_bring_to_front)  DipTank_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (DipTank_unsaved_document)   DipTank_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (DipTank_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("DipTank Image", NULL, DipTank_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				/*

				char ch=248;
			printf("Today's temperature was 23%cC",ch);
						*/
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);
				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2(DipTank_image_width, DipTank_image_height));
				ImGui::End();


				//PaintChamber

				static bool PaintChamber_no_titlebar = true;
				static bool PaintChamber_no_scrollbar = true;
				static bool PaintChamber_no_menu = true;
				static bool PaintChamber_no_move = true;
				static bool PaintChamber_no_resize = true;
				static bool PaintChamber_no_collapse = false;
				static bool PaintChamber_no_close = false;
				static bool PaintChamber_no_nav = true;
				static bool PaintChamber_no_background = false;
				static bool PaintChamber_no_bring_to_front = false;
				static bool PaintChamber_unsaved_document = false;

				ImGuiWindowFlags PaintChamber_flags = 1;
				if (PaintChamber_no_titlebar)        PaintChamber_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintChamber_no_scrollbar)       PaintChamber_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintChamber_no_menu)           PaintChamber_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintChamber_no_move)            PaintChamber_flags |= ImGuiWindowFlags_NoMove;
				if (PaintChamber_no_resize)          PaintChamber_flags |= ImGuiWindowFlags_NoResize;
				if (PaintChamber_no_collapse)        PaintChamber_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintChamber_no_nav)             PaintChamber_flags |= ImGuiWindowFlags_NoNav;
				if (PaintChamber_no_background)      PaintChamber_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintChamber_no_bring_to_front)  PaintChamber_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintChamber_unsaved_document)   PaintChamber_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintChamber_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("PaintChamber Image", NULL, PaintChamber_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);
				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2(PaintChamber_image_width, PaintChamber_image_height));
				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool PaintImage_no_titlebar = true;
				static bool PaintImage_no_scrollbar = true;
				static bool PaintImage_no_menu = true;
				static bool PaintImage_no_move = true;
				static bool PaintImage_no_resize = true;
				static bool PaintImage_no_collapse = false;
				static bool PaintImage_no_close = false;
				static bool PaintImage_no_nav = true;
				static bool PaintImage_no_background = false;
				static bool PaintImage_no_bring_to_front = false;
				static bool PaintImage_unsaved_document = false;

				ImGuiWindowFlags PaintImage_flags = 1;
				if (PaintImage_no_titlebar)        PaintImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintImage_no_scrollbar)       PaintImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintImage_no_menu)           PaintImage_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintImage_no_move)            PaintImage_flags |= ImGuiWindowFlags_NoMove;
				if (PaintImage_no_resize)          PaintImage_flags |= ImGuiWindowFlags_NoResize;
				if (PaintImage_no_collapse)        PaintImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintImage_no_nav)             PaintImage_flags |= ImGuiWindowFlags_NoNav;
				if (PaintImage_no_background)      PaintImage_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintImage_no_bring_to_front)  PaintImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintImage_unsaved_document)   PaintImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Paint Machine Image", open, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2(Paint_image_width, Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button
				const bool InvPaintButtons_no_titlebar = true;
				const bool InvPaintButtons_no_scrollbar = true;
				const bool InvPaintButtons_no_menu = true;
				const bool InvPaintButtons_no_move = true;
				const bool InvPaintButtons_no_resize = true;
				const bool InvPaintButtons_no_collapse = false;
				const bool InvPaintButtons_no_close = false;
				const bool InvPaintButtons_no_nav = false;
				static bool InvPaintButtons_no_bring_to_front = false;
				const bool InvPaintButtons_no_background = false;
				const bool InvPaintButtons_unsaved_document = false;

				ImGuiWindowFlags PaintButtonflags = 1;
				if (InvPaintButtons_no_titlebar)        PaintButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvPaintButtons_no_scrollbar)       PaintButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvPaintButtons_no_menu)           PaintButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvPaintButtons_no_move)            PaintButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvPaintButtons_no_resize)          PaintButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvPaintButtons_no_collapse)        PaintButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvPaintButtons_no_nav)             PaintButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvPaintButtons_no_background)      PaintButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvPaintButtons_no_bring_to_front)  PaintButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvPaintButtons_unsaved_document)   PaintButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvPaintButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Paint Machine Manager", open, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Paint Machine Values Adjuster", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Paint Vat to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Red Vat", "Green Vat", "Blue Vat" };
					ImGui::Combo("Paint Vats", &PaintVatitem_current, items, IM_ARRAYSIZE(items));
					if (ImGui::Button("Restock Button", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintVatitem_current == 0) {
							paintRedVatVol = 500;


						}
						else if (PaintVatitem_current == 1) {
							paintGreenVatVol = 500;
						}
						else if (PaintVatitem_current == 2) {
							paintBlueVatVol = 500;

						}
					}

					ImGui::Text("Please Select Which to Regulate\n\n");
					ImGui::Separator();
					const char* mach[] = { "Dip Tank Level", "Dip Tank Temperature", "Paint Chamber Temperature", "Paint Chamber Humidity", "Drying Chamber Temperature", "Drying Chamber Humidity" };
					ImGui::Combo("Values", &PaintMachineitem_current, mach, IM_ARRAYSIZE(mach));
					if (ImGui::Button("Regulate", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintMachineitem_current == 0) {
							DipTankFluidLevel = 1000;
						}
						else if (PaintMachineitem_current == 1) {
							DipTankTemp = 20.0;
						}
						else if (PaintMachineitem_current == 2) {
							PaintChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 3) {
							PaintChamberHMD = 45;
						}
						else if (PaintMachineitem_current == 4) {
							DryChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 5) {
							DryChamberHMD = 45;
						}
					}



					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}


				// Ends the window
				ImGui::End();

				// END OF PAINT MACHINE


				//GLOBAL PLANT STATS
					/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool GlobalTemp_no_titlebar = true;
				static bool GlobalTemp_no_scrollbar = true;
				static bool GlobalTemp_no_menu = true;
				static bool GlobalTemp_no_move = true;
				static bool GlobalTemp_no_resize = true;
				static bool GlobalTemp_no_collapse = false;
				static bool GlobalTemp_no_close = false;
				static bool GlobalTemp_no_nav = true;
				static bool GlobalTemp_no_background = false;
				static bool GlobalTemp_no_bring_to_front = false;
				static bool GlobalTemp_unsaved_document = false;

				ImGuiWindowFlags GlobalTemp_flags = 1;
				if (GlobalTemp_no_titlebar)        GlobalTemp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalTemp_no_scrollbar)       GlobalTemp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalTemp_no_menu)           GlobalTemp_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalTemp_no_move)            GlobalTemp_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalTemp_no_resize)          GlobalTemp_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalTemp_no_collapse)        GlobalTemp_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalTemp_no_nav)             GlobalTemp_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalTemp_no_background)      GlobalTemp_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalTemp_no_bring_to_front)  GlobalTemp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalTemp_unsaved_document)   GlobalTemp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalTemp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Temp", open, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();


				static bool GlobalAir_no_titlebar = true;
				static bool GlobalAir_no_scrollbar = true;
				static bool GlobalAir_no_menu = true;
				static bool GlobalAir_no_move = true;
				static bool GlobalAir_no_resize = true;
				static bool GlobalAir_no_collapse = false;
				static bool GlobalAir_no_close = false;
				static bool GlobalAir_no_nav = true;
				static bool GlobalAir_no_background = false;
				static bool GlobalAir_no_bring_to_front = false;
				static bool GlobalAir_unsaved_document = false;

				ImGuiWindowFlags GlobalAir_flags = 1;
				if (GlobalAir_no_titlebar)        GlobalAir_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalAir_no_scrollbar)       GlobalAir_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalAir_no_menu)           GlobalAir_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalAir_no_move)            GlobalAir_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalAir_no_resize)          GlobalAir_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalAir_no_collapse)        GlobalAir_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalAir_no_nav)             GlobalAir_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalAir_no_background)      GlobalAir_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalAir_no_bring_to_front)  GlobalAir_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalAir_unsaved_document)   GlobalAir_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalAir_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Air", open, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeQuota_no_titlebar = true;
				static bool VeQuota_no_scrollbar = true;
				static bool VeQuota_no_menu = true;
				static bool VeQuota_no_move = true;
				static bool VeQuota_no_resize = true;
				static bool VeQuota_no_collapse = false;
				static bool VeQuota_no_close = false;
				static bool VeQuota_no_nav = true;
				static bool VeQuota_no_background = false;
				static bool VeQuota_no_bring_to_front = false;
				static bool VeQuota_unsaved_document = false;

				ImGuiWindowFlags VeQuota_flags = 1;
				if (VeQuota_no_titlebar)        VeQuota_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeQuota_no_scrollbar)       VeQuota_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeQuota_no_menu)           VeQuota_flags |= ImGuiWindowFlags_MenuBar;
				if (VeQuota_no_move)            VeQuota_flags |= ImGuiWindowFlags_NoMove;
				if (VeQuota_no_resize)          VeQuota_flags |= ImGuiWindowFlags_NoResize;
				if (VeQuota_no_collapse)        VeQuota_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeQuota_no_nav)             VeQuota_flags |= ImGuiWindowFlags_NoNav;
				if (VeQuota_no_background)      VeQuota_flags |= ImGuiWindowFlags_NoBackground;
				if (VeQuota_no_bring_to_front)  VeQuota_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeQuota_unsaved_document)   VeQuota_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeQuota_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Quota", open, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeComp_no_titlebar = true;
				static bool VeComp_no_scrollbar = true;
				static bool VeComp_no_menu = true;
				static bool VeComp_no_move = true;
				static bool VeComp_no_resize = true;
				static bool VeComp_no_collapse = false;
				static bool VeComp_no_close = false;
				static bool VeComp_no_nav = true;
				static bool VeComp_no_background = false;
				static bool VeComp_no_bring_to_front = false;
				static bool VeComp_unsaved_document = false;

				ImGuiWindowFlags VeComp_flags = 1;
				if (VeComp_no_titlebar)        VeComp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeComp_no_scrollbar)       VeComp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeComp_no_menu)           VeComp_flags |= ImGuiWindowFlags_MenuBar;
				if (VeComp_no_move)            VeComp_flags |= ImGuiWindowFlags_NoMove;
				if (VeComp_no_resize)          VeComp_flags |= ImGuiWindowFlags_NoResize;
				if (VeComp_no_collapse)        VeComp_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeComp_no_nav)             VeComp_flags |= ImGuiWindowFlags_NoNav;
				if (VeComp_no_background)      VeComp_flags |= ImGuiWindowFlags_NoBackground;
				if (VeComp_no_bring_to_front)  VeComp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeComp_unsaved_document)   VeComp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeComp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Completed", open, VeComp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::TextColored({ 0, 255, 0, 1 }, "%d", VeComp);
				ImGui::PopStyleColor();
				ImGui::End();
				//END OF GLOBAL PLANT STATS


				//LOG STUFF
				// 
				ShowExampleAppLog(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!
				static bool  no_titlebar = true;
				static bool  no_scrollbar = true;
				static bool  no_menu = false;
				static bool  no_move = true;
				static bool  no_resize = true;
				static bool  no_collapse = false;
				static bool  no_close = false;
				static bool  no_nav = true;
				static bool  no_background = false;
				static bool  no_bring_to_front = true;
				static bool unsaved_document = false;

				ImGuiWindowFlags window_flags = 1;
				if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
				if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
				if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
				if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
				if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
				if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
				if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
				if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool toBeMade_no_titlebar = true;
				static bool toBeMade_no_scrollbar = true;
				static bool toBeMade_no_menu = true;
				static bool toBeMade_no_move = true;
				static bool toBeMade_no_resize = true;
				static bool toBeMade_no_collapse = false;
				static bool toBeMade_no_close = false;
				static bool toBeMade_no_nav = true;
				static bool toBeMade_no_background = false;
				static bool toBeMade_no_bring_to_front = false;
				static bool toBeMade_unsaved_document = false;

				ImGuiWindowFlags toBeMade_flags = 1;
				if (toBeMade_no_titlebar)        toBeMade_flags |= ImGuiWindowFlags_NoTitleBar;
				if (toBeMade_no_scrollbar)       toBeMade_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!toBeMade_no_menu)           toBeMade_flags |= ImGuiWindowFlags_MenuBar;
				if (toBeMade_no_move)            toBeMade_flags |= ImGuiWindowFlags_NoMove;
				if (toBeMade_no_resize)          toBeMade_flags |= ImGuiWindowFlags_NoResize;
				if (toBeMade_no_collapse)        toBeMade_flags |= ImGuiWindowFlags_NoCollapse;
				if (toBeMade_no_nav)             toBeMade_flags |= ImGuiWindowFlags_NoNav;
				if (toBeMade_no_background)      toBeMade_flags |= ImGuiWindowFlags_NoBackground;
				if (toBeMade_no_bring_to_front)  toBeMade_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (toBeMade_unsaved_document)   toBeMade_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (toBeMade_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(240, 121));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("ToBeMade Image", NULL, toBeMade_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2(toBeMade_image_width, toBeMade_image_height));


				ImGui::End();

				// Export variables to shader
				glUseProgram(shaderProgram);
				glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
				glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events
				
			

				i++;
				if (check == true) {
					if (i > 300) {
						bodyMachine = false;
						paintMachine = true;
						i = 0;
					}
				}
				
			}

			if (!bodyMachine) {

			}

			while (paintMachine) {
				glfwPollEvents();


				if (glfwWindowShouldClose(window)) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;
				}

				//PAINT CHANGING STUFF.

				if (changePaintValues) {

					plant.paintingMachine.setcolour(plant.order.getColour());
					plant.paintingMachine.identifyRGBvalues("PlantColours.txt");
					plant.paintingMachine.updateRGBpaintVat("RGBPaintVats.txt");

					plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");
					paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
					paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
					paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

					

					changePaintValues = false;
				}

				if (paintRedVatVol >= 250) {
					RedPaintVat = LoadTextureFromFile("Images/Redpaintfull.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol >= 100 && paintRedVatVol <= 249) {
					RedPaintVat = LoadTextureFromFile("Images/Redpainthalf.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol <= 99 && paintRedVatVol >= 0) {
					RedPaintVat = LoadTextureFromFile("Images/redpaintlow.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}

				if (paintGreenVatVol >= 250) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintfull.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol >= 100 && paintGreenVatVol <= 249) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpainthalf.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol <= 99 && paintGreenVatVol >= 0) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintlow.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				if (paintBlueVatVol >= 250) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintfull.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol >= 100 && paintBlueVatVol <= 249) {
					BluePaintVat = LoadTextureFromFile("Images/bluepainthalf.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol <= 99 && paintBlueVatVol >= 0) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintlow.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}

				//Set to blank
				bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
				IM_ASSERT(body);

				bool toBeMade = LoadTextureFromFile("Images/GUI/Orders/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
				IM_ASSERT(toBeMade);

				
				bool chassis = LoadTextureFromFile("Images/Blank.png", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
				IM_ASSERT(chassis);


				//set to blank
				bool Paint = LoadTextureFromFile("Images/GUI/Paint Machine/2022 Regular F150 Antimatter Blue Metallic.jpg", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
				IM_ASSERT(Paint);

				//set to blank
				bool Interior = LoadTextureFromFile("Images/Blank.png", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
				IM_ASSERT(Interior);


				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();



				// Tell OpenGL which Shader Program we want to use
				glUseProgram(shaderProgram);
				// Bind the VAO so OpenGL knows to use it
				glBindVertexArray(VAO);


				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



					//BAY SELECTION TOGGLE and restock button
				const bool InvBodyButtons_no_titlebar = true;
				const bool InvBodyButtons_no_scrollbar = true;
				const bool InvBodyButtons_no_menu = true;
				const bool InvBodyButtons_no_move = true;
				const bool InvBodyButtons_no_resize = true;
				const bool InvBodyButtons_no_collapse = false;
				const bool InvBodyButtons_no_close = false;
				const bool InvBodyButtons_no_nav = true;
				static bool InvBodyButtons_no_bring_to_front = false;
				const bool InvBodyButtons_no_background = false;
				const bool InvBodyButtons_unsaved_document = false;

				ImGuiWindowFlags bodyButtonflags = 1;
				if (InvBodyButtons_no_titlebar)        bodyButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBodyButtons_no_scrollbar)       bodyButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBodyButtons_no_menu)           bodyButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvBodyButtons_no_move)            bodyButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvBodyButtons_no_resize)          bodyButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvBodyButtons_no_collapse)        bodyButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvBodyButtons_no_nav)             bodyButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvBodyButtons_no_background)      bodyButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvBodyButtons_no_bring_to_front)  bodyButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBodyButtons_unsaved_document)   bodyButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBodyButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Body Machine Manager", open, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* BAY 2 selected
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();

				ImGuiWindowFlags controlMenuFlags = 1;
				if (controlMenu_no_titlebar)        controlMenuFlags |= ImGuiWindowFlags_NoTitleBar;
				if (controlMenu_no_scrollbar)       controlMenuFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!controlMenu_no_menu)           controlMenuFlags |= ImGuiWindowFlags_MenuBar;
				if (controlMenu_no_move)            controlMenuFlags |= ImGuiWindowFlags_NoMove;
				if (controlMenu_no_resize)          controlMenuFlags |= ImGuiWindowFlags_NoResize;
				if (controlMenu_no_collapse)        controlMenuFlags |= ImGuiWindowFlags_NoCollapse;
				if (controlMenu_no_nav)             controlMenuFlags |= ImGuiWindowFlags_NoNav;
				if (controlMenu_no_background)      controlMenuFlags |= ImGuiWindowFlags_NoBackground;
				if (controlMenu_no_bring_to_front)  controlMenuFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (controlMenu_unsaved_document)   controlMenuFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (controlMenu_no_close)           open = NULL; // Don't pass our bool* to Begin
				// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
			// Most functions would normally just crash if the context is missing.
				IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

				//CONTROL MENU CREATION
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.13f, 0.17f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(300, 100));
				ImGui::Begin("Control Menu", open, controlMenuFlags);
				if (ImGui::Button("Next Machine")) {
					paintMachine = false;
					chassisMachine = true;
				}
				if (ImGui::Button("Close HMI")) {

					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;

					// Deletes all ImGUI instances
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();

					glfwDestroyWindow(window);

					glfwTerminate();
					ClosedHMI = true;
					return 0;
				}
				ImGui::Separator();

				if (ImGui::Checkbox("Start Line", &check)) {};


				ImGui::PopStyleColor();
				ImGui::End();


				ImGuiWindowFlags inventoryFlags = 1;
				if (InvBody1_no_titlebar)        inventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBody1_no_scrollbar)       inventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBody1_no_menu)           inventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvBody1_no_move)            inventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvBody1_no_resize)          inventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvBody1_no_collapse)        inventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvBody1_no_nav)             inventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvBody1_no_background)      inventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvBody1_no_bring_to_front)  inventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBody1_unsaved_document)   inventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBody1_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(50, 300));
				ImGui::Begin("bay1InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay1RegExp);
				ImGui::Text("%d\n\n\n\n", bay1RegCab);
				ImGui::Text("%d\n\n\n\n", bay1SupCab);
				ImGui::Text("%d", bay1SupCrew);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay2RegExp);
				ImGui::Text("%d\n\n\n\n", bay2RegCab);
				ImGui::Text("%d\n\n\n\n", bay2SupCab);
				ImGui::Text("%d", bay2SupCrew);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool BodyImage_no_titlebar = true;
				static bool BodyImage_no_scrollbar = true;
				static bool BodyImage_no_menu = true;
				static bool BodyImage_no_move = true;
				static bool BodyImage_no_resize = true;
				static bool BodyImage_no_collapse = false;
				static bool BodyImage_no_close = false;
				static bool BodyImage_no_nav = true;
				static bool BodyImage_no_background = false;
				static bool BodyImage_no_bring_to_front = false;
				static bool BodyImage_unsaved_document = false;

				ImGuiWindowFlags bodyImage_flags = 1;
				if (BodyImage_no_titlebar)        bodyImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BodyImage_no_scrollbar)       bodyImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BodyImage_no_menu)           bodyImage_flags |= ImGuiWindowFlags_MenuBar;
				if (BodyImage_no_move)            bodyImage_flags |= ImGuiWindowFlags_NoMove;
				if (BodyImage_no_resize)          bodyImage_flags |= ImGuiWindowFlags_NoResize;
				if (BodyImage_no_collapse)        bodyImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (BodyImage_no_nav)             bodyImage_flags |= ImGuiWindowFlags_NoNav;
				if (BodyImage_no_background)      bodyImage_flags |= ImGuiWindowFlags_NoBackground;
				if (BodyImage_no_bring_to_front)  bodyImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BodyImage_unsaved_document)   bodyImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BodyImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Body Machine Image", open, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2(body_image_width, body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvChassisButtons_no_titlebar = true;
				const bool InvChassisButtons_no_scrollbar = true;
				const bool InvChassisButtons_no_menu = true;
				const bool InvChassisButtons_no_move = true;
				const bool InvChassisButtons_no_resize = true;
				const bool InvChassisButtons_no_collapse = false;
				const bool InvChassisButtons_no_close = false;
				const bool InvChassisButtons_no_nav = false;
				static bool InvChassisButtons_no_bring_to_front = false;
				const bool InvChassisButtons_no_background = false;
				const bool InvChassisButtons_unsaved_document = false;

				ImGuiWindowFlags ChassisButtonflags = 1;
				if (InvChassisButtons_no_titlebar)        ChassisButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassisButtons_no_scrollbar)       ChassisButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassisButtons_no_menu)           ChassisButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvChassisButtons_no_move)            ChassisButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvChassisButtons_no_resize)          ChassisButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvChassisButtons_no_collapse)        ChassisButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassisButtons_no_nav)             ChassisButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvChassisButtons_no_background)      ChassisButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvChassisButtons_no_bring_to_front)  ChassisButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassisButtons_unsaved_document)   ChassisButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassisButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Chassis Machine Manager", open, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int b;
				ImGui::RadioButton("Bay 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &b, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (b == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 2 if bay 1 is selected you can only restock bay 2
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags ChassisinventoryFlags = 0;
				if (InvChassis_no_titlebar)        ChassisinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassis_no_scrollbar)       ChassisinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassis_no_menu)           ChassisinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvChassis_no_move)            ChassisinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvChassis_no_resize)          ChassisinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvChassis_no_collapse)        ChassisinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassis_no_nav)             ChassisinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvChassis_no_background)      ChassisinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvChassis_no_bring_to_front)  ChassisinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassis_unsaved_document)   ChassisinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassis_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay135LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6HOC);
				ImGui::Text("%d\n\n\n", bay127LV6C);
				ImGui::Text("%d\n\n\n", bay133LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay135LV6PwrBstC);
				ImGui::Text("%d", bay150LV8C);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay235LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6HOC);
				ImGui::Text("%d\n\n\n", bay227LV6C);
				ImGui::Text("%d\n\n\n", bay233LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay235LV6PwrBstC);
				ImGui::Text("%d", bay250LV8C);
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool ChassisImage_no_titlebar = true;
				static bool ChassisImage_no_scrollbar = true;
				static bool ChassisImage_no_menu = true;
				static bool ChassisImage_no_move = true;
				static bool ChassisImage_no_resize = true;
				static bool ChassisImage_no_collapse = false;
				static bool ChassisImage_no_close = false;
				static bool ChassisImage_no_nav = true;
				static bool ChassisImage_no_background = false;
				static bool ChassisImage_no_bring_to_front = false;
				static bool ChassisImage_unsaved_document = false;

				ImGuiWindowFlags ChassisImage_flags = 1;
				if (ChassisImage_no_titlebar)        ChassisImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (ChassisImage_no_scrollbar)       ChassisImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!ChassisImage_no_menu)           ChassisImage_flags |= ImGuiWindowFlags_MenuBar;
				if (ChassisImage_no_move)            ChassisImage_flags |= ImGuiWindowFlags_NoMove;
				if (ChassisImage_no_resize)          ChassisImage_flags |= ImGuiWindowFlags_NoResize;
				if (ChassisImage_no_collapse)        ChassisImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (ChassisImage_no_nav)             ChassisImage_flags |= ImGuiWindowFlags_NoNav;
				if (ChassisImage_no_background)      ChassisImage_flags |= ImGuiWindowFlags_NoBackground;
				if (ChassisImage_no_bring_to_front)  ChassisImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (ChassisImage_unsaved_document)   ChassisImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (ChassisImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Chassis Machine Image", open, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2(chassis_image_width, chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvInteriorButtons_no_titlebar = true;
				const bool InvInteriorButtons_no_scrollbar = true;
				const bool InvInteriorButtons_no_menu = true;
				const bool InvInteriorButtons_no_move = true;
				const bool InvInteriorButtons_no_resize = true;
				const bool InvInteriorButtons_no_collapse = false;
				const bool InvInteriorButtons_no_close = false;
				const bool InvInteriorButtons_no_nav = false;
				static bool InvInteriorButtons_no_bring_to_front = false;
				const bool InvInteriorButtons_no_background = false;
				const bool InvInteriorButtons_unsaved_document = false;

				ImGuiWindowFlags InteriorButtonflags = 1;
				if (InvInteriorButtons_no_titlebar)        InteriorButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInteriorButtons_no_scrollbar)       InteriorButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInteriorButtons_no_menu)           InteriorButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvInteriorButtons_no_move)            InteriorButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvInteriorButtons_no_resize)          InteriorButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvInteriorButtons_no_collapse)        InteriorButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvInteriorButtons_no_nav)             InteriorButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvInteriorButtons_no_background)      InteriorButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvInteriorButtons_no_bring_to_front)  InteriorButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInteriorButtons_unsaved_document)   InteriorButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInteriorButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Interior Machine Manager", open, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int c;
				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from Interior inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags InteriorinventoryFlags = 1;
				if (InvInterior_no_titlebar)        InteriorinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInterior_no_scrollbar)       InteriorinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInterior_no_menu)           InteriorinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvInterior_no_move)            InteriorinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvInterior_no_resize)          InteriorinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvInterior_no_collapse)        InteriorinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvInterior_no_nav)             InteriorinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvInterior_no_background)      InteriorinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvInterior_no_bring_to_front)  InteriorinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInterior_unsaved_document)   InteriorinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInterior_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay1BaseExp);
				ImGui::Text("%d\n\n\n\n", bay1MidExp);
				ImGui::Text("%d\n\n\n", bay1HighExp);
				ImGui::Text("%d\n\n\n\n", bay1BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay1MidF150);
				ImGui::Text("%d", bay1HighF150);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay2BaseExp);
				ImGui::Text("%d\n\n\n\n", bay2MidExp);
				ImGui::Text("%d\n\n\n", bay2HighExp);
				ImGui::Text("%d\n\n\n\n", bay2BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay2MidF150);
				ImGui::Text("%d", bay2HighF150);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool InteriorImage_no_titlebar = true;
				static bool InteriorImage_no_scrollbar = true;
				static bool InteriorImage_no_menu = true;
				static bool InteriorImage_no_move = true;
				static bool InteriorImage_no_resize = false;
				static bool InteriorImage_no_collapse = false;
				static bool InteriorImage_no_close = false;
				static bool InteriorImage_no_nav = true;
				static bool InteriorImage_no_background = false;
				static bool InteriorImage_no_bring_to_front = false;
				static bool InteriorImage_unsaved_document = false;

				ImGuiWindowFlags InteriorImage_flags = 1;
				if (InteriorImage_no_titlebar)        InteriorImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (InteriorImage_no_scrollbar)       InteriorImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!InteriorImage_no_menu)           InteriorImage_flags |= ImGuiWindowFlags_MenuBar;
				if (InteriorImage_no_move)            InteriorImage_flags |= ImGuiWindowFlags_NoMove;
				if (InteriorImage_no_resize)          InteriorImage_flags |= ImGuiWindowFlags_NoResize;
				if (InteriorImage_no_collapse)        InteriorImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (InteriorImage_no_nav)             InteriorImage_flags |= ImGuiWindowFlags_NoNav;
				if (InteriorImage_no_background)      InteriorImage_flags |= ImGuiWindowFlags_NoBackground;
				if (InteriorImage_no_bring_to_front)  InteriorImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InteriorImage_unsaved_document)   InteriorImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (InteriorImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Interior Machine Image", open, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2(Interior_image_width, Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE


				static bool RedPaintVat_no_titlebar = true;
				static bool RedPaintVat_no_scrollbar = true;
				static bool RedPaintVat_no_menu = true;
				static bool RedPaintVat_no_move = true;
				static bool RedPaintVat_no_resize = true;
				static bool RedPaintVat_no_collapse = false;
				static bool RedPaintVat_no_close = false;
				static bool RedPaintVat_no_nav = true;
				static bool RedPaintVat_no_background = false;
				static bool RedPaintVat_no_bring_to_front = false;
				static bool RedPaintVat_unsaved_document = false;

				ImGuiWindowFlags RedPaintVat_flags = 1;
				if (RedPaintVat_no_titlebar)        RedPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (RedPaintVat_no_scrollbar)       RedPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!RedPaintVat_no_menu)           RedPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (RedPaintVat_no_move)            RedPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (RedPaintVat_no_resize)          RedPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (RedPaintVat_no_collapse)        RedPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (RedPaintVat_no_nav)             RedPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (RedPaintVat_no_background)      RedPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (RedPaintVat_no_bring_to_front)  RedPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (RedPaintVat_unsaved_document)   RedPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (RedPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("RedPaintVat Image", NULL, RedPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2(RedPaintVat_image_width, RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat

				static bool BluePaintVat_no_titlebar = true;
				static bool BluePaintVat_no_scrollbar = true;
				static bool BluePaintVat_no_menu = true;
				static bool BluePaintVat_no_move = true;
				static bool BluePaintVat_no_resize = true;
				static bool BluePaintVat_no_collapse = false;
				static bool BluePaintVat_no_close = false;
				static bool BluePaintVat_no_nav = true;
				static bool BluePaintVat_no_background = false;
				static bool BluePaintVat_no_bring_to_front = false;
				static bool BluePaintVat_unsaved_document = false;

				ImGuiWindowFlags BluePaintVat_flags = 1;
				if (BluePaintVat_no_titlebar)        BluePaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BluePaintVat_no_scrollbar)       BluePaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BluePaintVat_no_menu)           BluePaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (BluePaintVat_no_move)            BluePaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (BluePaintVat_no_resize)          BluePaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (BluePaintVat_no_collapse)        BluePaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (BluePaintVat_no_nav)             BluePaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (BluePaintVat_no_background)      BluePaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (BluePaintVat_no_bring_to_front)  BluePaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BluePaintVat_unsaved_document)   BluePaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BluePaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("BluePaintVat Image", NULL, BluePaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2(BluePaintVat_image_width, BluePaintVat_image_height));
				ImGui::End();

				//Green Vat

				static bool GreenPaintVat_no_titlebar = true;
				static bool GreenPaintVat_no_scrollbar = true;
				static bool GreenPaintVat_no_menu = true;
				static bool GreenPaintVat_no_move = true;
				static bool GreenPaintVat_no_resize = true;
				static bool GreenPaintVat_no_collapse = false;
				static bool GreenPaintVat_no_close = false;
				static bool GreenPaintVat_no_nav = true;
				static bool GreenPaintVat_no_background = false;
				static bool GreenPaintVat_no_bring_to_front = false;
				static bool GreenPaintVat_unsaved_document = false;

				ImGuiWindowFlags GreenPaintVat_flags = 1;
				if (GreenPaintVat_no_titlebar)        GreenPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GreenPaintVat_no_scrollbar)       GreenPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GreenPaintVat_no_menu)           GreenPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (GreenPaintVat_no_move)            GreenPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (GreenPaintVat_no_resize)          GreenPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (GreenPaintVat_no_collapse)        GreenPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (GreenPaintVat_no_nav)             GreenPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (GreenPaintVat_no_background)      GreenPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (GreenPaintVat_no_bring_to_front)  GreenPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GreenPaintVat_unsaved_document)   GreenPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GreenPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("GreenPaintVat Image", NULL, GreenPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2(GreenPaintVat_image_width, GreenPaintVat_image_height));
				ImGui::End();

				//DipTank

				static bool DipTank_no_titlebar = true;
				static bool DipTank_no_scrollbar = true;
				static bool DipTank_no_menu = true;
				static bool DipTank_no_move = true;
				static bool DipTank_no_resize = true;
				static bool DipTank_no_collapse = false;
				static bool DipTank_no_close = false;
				static bool DipTank_no_nav = true;
				static bool DipTank_no_background = false;
				static bool DipTank_no_bring_to_front = false;
				static bool DipTank_unsaved_document = false;

				ImGuiWindowFlags DipTank_flags = 1;
				if (DipTank_no_titlebar)        DipTank_flags |= ImGuiWindowFlags_NoTitleBar;
				if (DipTank_no_scrollbar)       DipTank_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!DipTank_no_menu)           DipTank_flags |= ImGuiWindowFlags_MenuBar;
				if (DipTank_no_move)            DipTank_flags |= ImGuiWindowFlags_NoMove;
				if (DipTank_no_resize)          DipTank_flags |= ImGuiWindowFlags_NoResize;
				if (DipTank_no_collapse)        DipTank_flags |= ImGuiWindowFlags_NoCollapse;
				if (DipTank_no_nav)             DipTank_flags |= ImGuiWindowFlags_NoNav;
				if (DipTank_no_background)      DipTank_flags |= ImGuiWindowFlags_NoBackground;
				if (DipTank_no_bring_to_front)  DipTank_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (DipTank_unsaved_document)   DipTank_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (DipTank_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("DipTank Image", NULL, DipTank_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				/*

				char ch=248;
			printf("Today's temperature was 23%cC",ch);
						*/
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);
				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2(DipTank_image_width, DipTank_image_height));
				ImGui::End();


				//PaintChamber

				static bool PaintChamber_no_titlebar = true;
				static bool PaintChamber_no_scrollbar = true;
				static bool PaintChamber_no_menu = true;
				static bool PaintChamber_no_move = true;
				static bool PaintChamber_no_resize = true;
				static bool PaintChamber_no_collapse = false;
				static bool PaintChamber_no_close = false;
				static bool PaintChamber_no_nav = true;
				static bool PaintChamber_no_background = false;
				static bool PaintChamber_no_bring_to_front = false;
				static bool PaintChamber_unsaved_document = false;

				ImGuiWindowFlags PaintChamber_flags = 1;
				if (PaintChamber_no_titlebar)        PaintChamber_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintChamber_no_scrollbar)       PaintChamber_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintChamber_no_menu)           PaintChamber_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintChamber_no_move)            PaintChamber_flags |= ImGuiWindowFlags_NoMove;
				if (PaintChamber_no_resize)          PaintChamber_flags |= ImGuiWindowFlags_NoResize;
				if (PaintChamber_no_collapse)        PaintChamber_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintChamber_no_nav)             PaintChamber_flags |= ImGuiWindowFlags_NoNav;
				if (PaintChamber_no_background)      PaintChamber_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintChamber_no_bring_to_front)  PaintChamber_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintChamber_unsaved_document)   PaintChamber_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintChamber_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("PaintChamber Image", NULL, PaintChamber_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);
				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2(PaintChamber_image_width, PaintChamber_image_height));
				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool PaintImage_no_titlebar = true;
				static bool PaintImage_no_scrollbar = true;
				static bool PaintImage_no_menu = true;
				static bool PaintImage_no_move = true;
				static bool PaintImage_no_resize = true;
				static bool PaintImage_no_collapse = false;
				static bool PaintImage_no_close = false;
				static bool PaintImage_no_nav = true;
				static bool PaintImage_no_background = false;
				static bool PaintImage_no_bring_to_front = false;
				static bool PaintImage_unsaved_document = false;

				ImGuiWindowFlags PaintImage_flags = 1;
				if (PaintImage_no_titlebar)        PaintImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintImage_no_scrollbar)       PaintImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintImage_no_menu)           PaintImage_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintImage_no_move)            PaintImage_flags |= ImGuiWindowFlags_NoMove;
				if (PaintImage_no_resize)          PaintImage_flags |= ImGuiWindowFlags_NoResize;
				if (PaintImage_no_collapse)        PaintImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintImage_no_nav)             PaintImage_flags |= ImGuiWindowFlags_NoNav;
				if (PaintImage_no_background)      PaintImage_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintImage_no_bring_to_front)  PaintImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintImage_unsaved_document)   PaintImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Paint Machine Image", open, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2(Paint_image_width, Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button
				const bool InvPaintButtons_no_titlebar = true;
				const bool InvPaintButtons_no_scrollbar = true;
				const bool InvPaintButtons_no_menu = true;
				const bool InvPaintButtons_no_move = true;
				const bool InvPaintButtons_no_resize = true;
				const bool InvPaintButtons_no_collapse = false;
				const bool InvPaintButtons_no_close = false;
				const bool InvPaintButtons_no_nav = false;
				static bool InvPaintButtons_no_bring_to_front = false;
				const bool InvPaintButtons_no_background = false;
				const bool InvPaintButtons_unsaved_document = false;

				ImGuiWindowFlags PaintButtonflags = 1;
				if (InvPaintButtons_no_titlebar)        PaintButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvPaintButtons_no_scrollbar)       PaintButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvPaintButtons_no_menu)           PaintButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvPaintButtons_no_move)            PaintButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvPaintButtons_no_resize)          PaintButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvPaintButtons_no_collapse)        PaintButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvPaintButtons_no_nav)             PaintButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvPaintButtons_no_background)      PaintButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvPaintButtons_no_bring_to_front)  PaintButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvPaintButtons_unsaved_document)   PaintButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvPaintButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Paint Machine Manager", open, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Paint Machine Values Adjuster", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Paint Vat to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Red Vat", "Green Vat", "Blue Vat" };
					ImGui::Combo("Paint Vats", &PaintVatitem_current, items, IM_ARRAYSIZE(items));
					if (ImGui::Button("Restock Button", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintVatitem_current == 0) {
							paintRedVatVol = 500;


						}
						else if (PaintVatitem_current == 1) {
							paintGreenVatVol = 500;
						}
						else if (PaintVatitem_current == 2) {
							paintBlueVatVol = 500;

						}
					}

					ImGui::Text("Please Select Which to Regulate\n\n");
					ImGui::Separator();
					const char* mach[] = { "Dip Tank Level", "Dip Tank Temperature", "Paint Chamber Temperature", "Paint Chamber Humidity", "Drying Chamber Temperature", "Drying Chamber Humidity" };
					ImGui::Combo("Values", &PaintMachineitem_current, mach, IM_ARRAYSIZE(mach));
					if (ImGui::Button("Regulate", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintMachineitem_current == 0) {
							DipTankFluidLevel = 1000;
						}
						else if (PaintMachineitem_current == 1) {
							DipTankTemp = 20.0;
						}
						else if (PaintMachineitem_current == 2) {
							PaintChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 3) {
							PaintChamberHMD = 45;
						}
						else if (PaintMachineitem_current == 4) {
							DryChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 5) {
							DryChamberHMD = 45;
						}
					}



					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}


				// Ends the window
				ImGui::End();

				// END OF PAINT MACHINE


				//GLOBAL PLANT STATS
					/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool GlobalTemp_no_titlebar = true;
				static bool GlobalTemp_no_scrollbar = true;
				static bool GlobalTemp_no_menu = true;
				static bool GlobalTemp_no_move = true;
				static bool GlobalTemp_no_resize = true;
				static bool GlobalTemp_no_collapse = false;
				static bool GlobalTemp_no_close = false;
				static bool GlobalTemp_no_nav = true;
				static bool GlobalTemp_no_background = false;
				static bool GlobalTemp_no_bring_to_front = false;
				static bool GlobalTemp_unsaved_document = false;

				ImGuiWindowFlags GlobalTemp_flags = 1;
				if (GlobalTemp_no_titlebar)        GlobalTemp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalTemp_no_scrollbar)       GlobalTemp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalTemp_no_menu)           GlobalTemp_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalTemp_no_move)            GlobalTemp_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalTemp_no_resize)          GlobalTemp_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalTemp_no_collapse)        GlobalTemp_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalTemp_no_nav)             GlobalTemp_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalTemp_no_background)      GlobalTemp_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalTemp_no_bring_to_front)  GlobalTemp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalTemp_unsaved_document)   GlobalTemp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalTemp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Temp", open, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();


				static bool GlobalAir_no_titlebar = true;
				static bool GlobalAir_no_scrollbar = true;
				static bool GlobalAir_no_menu = true;
				static bool GlobalAir_no_move = true;
				static bool GlobalAir_no_resize = true;
				static bool GlobalAir_no_collapse = false;
				static bool GlobalAir_no_close = false;
				static bool GlobalAir_no_nav = true;
				static bool GlobalAir_no_background = false;
				static bool GlobalAir_no_bring_to_front = false;
				static bool GlobalAir_unsaved_document = false;

				ImGuiWindowFlags GlobalAir_flags = 1;
				if (GlobalAir_no_titlebar)        GlobalAir_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalAir_no_scrollbar)       GlobalAir_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalAir_no_menu)           GlobalAir_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalAir_no_move)            GlobalAir_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalAir_no_resize)          GlobalAir_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalAir_no_collapse)        GlobalAir_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalAir_no_nav)             GlobalAir_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalAir_no_background)      GlobalAir_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalAir_no_bring_to_front)  GlobalAir_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalAir_unsaved_document)   GlobalAir_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalAir_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Air", open, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeQuota_no_titlebar = true;
				static bool VeQuota_no_scrollbar = true;
				static bool VeQuota_no_menu = true;
				static bool VeQuota_no_move = true;
				static bool VeQuota_no_resize = true;
				static bool VeQuota_no_collapse = false;
				static bool VeQuota_no_close = false;
				static bool VeQuota_no_nav = true;
				static bool VeQuota_no_background = false;
				static bool VeQuota_no_bring_to_front = false;
				static bool VeQuota_unsaved_document = false;

				ImGuiWindowFlags VeQuota_flags = 1;
				if (VeQuota_no_titlebar)        VeQuota_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeQuota_no_scrollbar)       VeQuota_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeQuota_no_menu)           VeQuota_flags |= ImGuiWindowFlags_MenuBar;
				if (VeQuota_no_move)            VeQuota_flags |= ImGuiWindowFlags_NoMove;
				if (VeQuota_no_resize)          VeQuota_flags |= ImGuiWindowFlags_NoResize;
				if (VeQuota_no_collapse)        VeQuota_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeQuota_no_nav)             VeQuota_flags |= ImGuiWindowFlags_NoNav;
				if (VeQuota_no_background)      VeQuota_flags |= ImGuiWindowFlags_NoBackground;
				if (VeQuota_no_bring_to_front)  VeQuota_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeQuota_unsaved_document)   VeQuota_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeQuota_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Quota", open, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeComp_no_titlebar = true;
				static bool VeComp_no_scrollbar = true;
				static bool VeComp_no_menu = true;
				static bool VeComp_no_move = true;
				static bool VeComp_no_resize = true;
				static bool VeComp_no_collapse = false;
				static bool VeComp_no_close = false;
				static bool VeComp_no_nav = true;
				static bool VeComp_no_background = false;
				static bool VeComp_no_bring_to_front = false;
				static bool VeComp_unsaved_document = false;

				ImGuiWindowFlags VeComp_flags = 1;
				if (VeComp_no_titlebar)        VeComp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeComp_no_scrollbar)       VeComp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeComp_no_menu)           VeComp_flags |= ImGuiWindowFlags_MenuBar;
				if (VeComp_no_move)            VeComp_flags |= ImGuiWindowFlags_NoMove;
				if (VeComp_no_resize)          VeComp_flags |= ImGuiWindowFlags_NoResize;
				if (VeComp_no_collapse)        VeComp_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeComp_no_nav)             VeComp_flags |= ImGuiWindowFlags_NoNav;
				if (VeComp_no_background)      VeComp_flags |= ImGuiWindowFlags_NoBackground;
				if (VeComp_no_bring_to_front)  VeComp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeComp_unsaved_document)   VeComp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeComp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Completed", open, VeComp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::TextColored({ 0, 255, 0, 1 }, "%d", VeComp);
				ImGui::PopStyleColor();
				ImGui::End();
				//END OF GLOBAL PLANT STATS


				//LOG STUFF
				// 
				ShowExampleAppLog(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!
				static bool  no_titlebar = true;
				static bool  no_scrollbar = true;
				static bool  no_menu = false;
				static bool  no_move = true;
				static bool  no_resize = true;
				static bool  no_collapse = false;
				static bool  no_close = false;
				static bool  no_nav = true;
				static bool  no_background = false;
				static bool  no_bring_to_front = true;
				static bool unsaved_document = false;

				ImGuiWindowFlags window_flags = 1;
				if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
				if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
				if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
				if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
				if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
				if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
				if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
				if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool toBeMade_no_titlebar = true;
				static bool toBeMade_no_scrollbar = true;
				static bool toBeMade_no_menu = true;
				static bool toBeMade_no_move = true;
				static bool toBeMade_no_resize = true;
				static bool toBeMade_no_collapse = false;
				static bool toBeMade_no_close = false;
				static bool toBeMade_no_nav = true;
				static bool toBeMade_no_background = false;
				static bool toBeMade_no_bring_to_front = false;
				static bool toBeMade_unsaved_document = false;

				ImGuiWindowFlags toBeMade_flags = 1;
				if (toBeMade_no_titlebar)        toBeMade_flags |= ImGuiWindowFlags_NoTitleBar;
				if (toBeMade_no_scrollbar)       toBeMade_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!toBeMade_no_menu)           toBeMade_flags |= ImGuiWindowFlags_MenuBar;
				if (toBeMade_no_move)            toBeMade_flags |= ImGuiWindowFlags_NoMove;
				if (toBeMade_no_resize)          toBeMade_flags |= ImGuiWindowFlags_NoResize;
				if (toBeMade_no_collapse)        toBeMade_flags |= ImGuiWindowFlags_NoCollapse;
				if (toBeMade_no_nav)             toBeMade_flags |= ImGuiWindowFlags_NoNav;
				if (toBeMade_no_background)      toBeMade_flags |= ImGuiWindowFlags_NoBackground;
				if (toBeMade_no_bring_to_front)  toBeMade_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (toBeMade_unsaved_document)   toBeMade_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (toBeMade_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(240, 121));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("ToBeMade Image", NULL, toBeMade_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2(toBeMade_image_width, toBeMade_image_height));


				ImGui::End();

				// Export variables to shader
				glUseProgram(shaderProgram);
				glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
				glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events
				
			
				i++;
				if (check == true) {
					if (i > 300) {
						paintMachine = false;
						chassisMachine = true;
						i = 0;
					}
				}
			}
			if (!paintMachine) {

		
			}

			while (chassisMachine) {
				glfwPollEvents();
				if (glfwWindowShouldClose(window)) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;
				}

				if (paintRedVatVol >= 250) {
					RedPaintVat = LoadTextureFromFile("Images/Redpaintfull.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol >= 100 && paintRedVatVol <= 249) {
					RedPaintVat = LoadTextureFromFile("Images/Redpainthalf.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol <= 99 && paintRedVatVol >= 0) {
					RedPaintVat = LoadTextureFromFile("Images/redpaintlow.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}

				if (paintGreenVatVol >= 250) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintfull.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol >= 100 && paintGreenVatVol <= 249) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpainthalf.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol <= 99 && paintGreenVatVol >= 0) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintlow.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				if (paintBlueVatVol >= 250) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintfull.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol >= 100 && paintBlueVatVol <= 249) {
					BluePaintVat = LoadTextureFromFile("Images/bluepainthalf.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol <= 99 && paintBlueVatVol >= 0) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintlow.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				//Set to blank
				bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
				IM_ASSERT(body);

				bool toBeMade = LoadTextureFromFile("Images/GUI/Orders/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
				IM_ASSERT(toBeMade);

				
				bool chassis = LoadTextureFromFile("Images/GUI/Chassis Machine/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
				IM_ASSERT(chassis);


				//set to blank
				bool Paint = LoadTextureFromFile("Images/Blank.png", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
				IM_ASSERT(Paint);

				//set to blank
				bool Interior = LoadTextureFromFile("Images/Blank.png", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
				IM_ASSERT(Interior);


				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();



				// Tell OpenGL which Shader Program we want to use
				glUseProgram(shaderProgram);
				// Bind the VAO so OpenGL knows to use it
				glBindVertexArray(VAO);


				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



					//BAY SELECTION TOGGLE and restock button
				const bool InvBodyButtons_no_titlebar = true;
				const bool InvBodyButtons_no_scrollbar = true;
				const bool InvBodyButtons_no_menu = true;
				const bool InvBodyButtons_no_move = true;
				const bool InvBodyButtons_no_resize = true;
				const bool InvBodyButtons_no_collapse = false;
				const bool InvBodyButtons_no_close = false;
				const bool InvBodyButtons_no_nav = true;
				static bool InvBodyButtons_no_bring_to_front = false;
				const bool InvBodyButtons_no_background = false;
				const bool InvBodyButtons_unsaved_document = false;

				ImGuiWindowFlags bodyButtonflags = 1;
				if (InvBodyButtons_no_titlebar)        bodyButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBodyButtons_no_scrollbar)       bodyButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBodyButtons_no_menu)           bodyButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvBodyButtons_no_move)            bodyButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvBodyButtons_no_resize)          bodyButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvBodyButtons_no_collapse)        bodyButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvBodyButtons_no_nav)             bodyButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvBodyButtons_no_background)      bodyButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvBodyButtons_no_bring_to_front)  bodyButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBodyButtons_unsaved_document)   bodyButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBodyButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Body Machine Manager", open, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* BAY 2 selected
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();

				ImGuiWindowFlags controlMenuFlags = 1;
				if (controlMenu_no_titlebar)        controlMenuFlags |= ImGuiWindowFlags_NoTitleBar;
				if (controlMenu_no_scrollbar)       controlMenuFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!controlMenu_no_menu)           controlMenuFlags |= ImGuiWindowFlags_MenuBar;
				if (controlMenu_no_move)            controlMenuFlags |= ImGuiWindowFlags_NoMove;
				if (controlMenu_no_resize)          controlMenuFlags |= ImGuiWindowFlags_NoResize;
				if (controlMenu_no_collapse)        controlMenuFlags |= ImGuiWindowFlags_NoCollapse;
				if (controlMenu_no_nav)             controlMenuFlags |= ImGuiWindowFlags_NoNav;
				if (controlMenu_no_background)      controlMenuFlags |= ImGuiWindowFlags_NoBackground;
				if (controlMenu_no_bring_to_front)  controlMenuFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (controlMenu_unsaved_document)   controlMenuFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (controlMenu_no_close)           open = NULL; // Don't pass our bool* to Begin
				// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
			// Most functions would normally just crash if the context is missing.
				IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

				//CONTROL MENU CREATION
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.13f, 0.17f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(300, 100));
				ImGui::Begin("Control Menu", open, controlMenuFlags);
				if (ImGui::Button("Next Machine")) {

					chassisMachine = false;
					interiorMachine = true;
				}
				if (ImGui::Button("Close HMI")) {

					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;

					// Deletes all ImGUI instances
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();

					glfwDestroyWindow(window);

					glfwTerminate();
					ClosedHMI = true;
					return 0;
				}
				ImGui::Separator();

				if (ImGui::Checkbox("Start Line", &check)) {};


				ImGui::PopStyleColor();
				ImGui::End();


				ImGuiWindowFlags inventoryFlags = 1;
				if (InvBody1_no_titlebar)        inventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBody1_no_scrollbar)       inventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBody1_no_menu)           inventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvBody1_no_move)            inventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvBody1_no_resize)          inventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvBody1_no_collapse)        inventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvBody1_no_nav)             inventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvBody1_no_background)      inventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvBody1_no_bring_to_front)  inventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBody1_unsaved_document)   inventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBody1_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(50, 300));
				ImGui::Begin("bay1InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay1RegExp);
				ImGui::Text("%d\n\n\n\n", bay1RegCab);
				ImGui::Text("%d\n\n\n\n", bay1SupCab);
				ImGui::Text("%d", bay1SupCrew);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay2RegExp);
				ImGui::Text("%d\n\n\n\n", bay2RegCab);
				ImGui::Text("%d\n\n\n\n", bay2SupCab);
				ImGui::Text("%d", bay2SupCrew);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool BodyImage_no_titlebar = true;
				static bool BodyImage_no_scrollbar = true;
				static bool BodyImage_no_menu = true;
				static bool BodyImage_no_move = true;
				static bool BodyImage_no_resize = true;
				static bool BodyImage_no_collapse = false;
				static bool BodyImage_no_close = false;
				static bool BodyImage_no_nav = true;
				static bool BodyImage_no_background = false;
				static bool BodyImage_no_bring_to_front = false;
				static bool BodyImage_unsaved_document = false;

				ImGuiWindowFlags bodyImage_flags = 1;
				if (BodyImage_no_titlebar)        bodyImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BodyImage_no_scrollbar)       bodyImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BodyImage_no_menu)           bodyImage_flags |= ImGuiWindowFlags_MenuBar;
				if (BodyImage_no_move)            bodyImage_flags |= ImGuiWindowFlags_NoMove;
				if (BodyImage_no_resize)          bodyImage_flags |= ImGuiWindowFlags_NoResize;
				if (BodyImage_no_collapse)        bodyImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (BodyImage_no_nav)             bodyImage_flags |= ImGuiWindowFlags_NoNav;
				if (BodyImage_no_background)      bodyImage_flags |= ImGuiWindowFlags_NoBackground;
				if (BodyImage_no_bring_to_front)  bodyImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BodyImage_unsaved_document)   bodyImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BodyImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Body Machine Image", open, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2(body_image_width, body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvChassisButtons_no_titlebar = true;
				const bool InvChassisButtons_no_scrollbar = true;
				const bool InvChassisButtons_no_menu = true;
				const bool InvChassisButtons_no_move = true;
				const bool InvChassisButtons_no_resize = true;
				const bool InvChassisButtons_no_collapse = false;
				const bool InvChassisButtons_no_close = false;
				const bool InvChassisButtons_no_nav = false;
				static bool InvChassisButtons_no_bring_to_front = false;
				const bool InvChassisButtons_no_background = false;
				const bool InvChassisButtons_unsaved_document = false;

				ImGuiWindowFlags ChassisButtonflags = 1;
				if (InvChassisButtons_no_titlebar)        ChassisButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassisButtons_no_scrollbar)       ChassisButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassisButtons_no_menu)           ChassisButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvChassisButtons_no_move)            ChassisButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvChassisButtons_no_resize)          ChassisButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvChassisButtons_no_collapse)        ChassisButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassisButtons_no_nav)             ChassisButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvChassisButtons_no_background)      ChassisButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvChassisButtons_no_bring_to_front)  ChassisButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassisButtons_unsaved_document)   ChassisButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassisButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Chassis Machine Manager", open, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int b;
				ImGui::RadioButton("Bay 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &b, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (b == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 2 if bay 1 is selected you can only restock bay 2
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags ChassisinventoryFlags = 0;
				if (InvChassis_no_titlebar)        ChassisinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassis_no_scrollbar)       ChassisinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassis_no_menu)           ChassisinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvChassis_no_move)            ChassisinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvChassis_no_resize)          ChassisinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvChassis_no_collapse)        ChassisinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassis_no_nav)             ChassisinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvChassis_no_background)      ChassisinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvChassis_no_bring_to_front)  ChassisinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassis_unsaved_document)   ChassisinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassis_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay135LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6HOC);
				ImGui::Text("%d\n\n\n", bay127LV6C);
				ImGui::Text("%d\n\n\n", bay133LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay135LV6PwrBstC);
				ImGui::Text("%d", bay150LV8C);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay235LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6HOC);
				ImGui::Text("%d\n\n\n", bay227LV6C);
				ImGui::Text("%d\n\n\n", bay233LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay235LV6PwrBstC);
				ImGui::Text("%d", bay250LV8C);
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool ChassisImage_no_titlebar = true;
				static bool ChassisImage_no_scrollbar = true;
				static bool ChassisImage_no_menu = true;
				static bool ChassisImage_no_move = true;
				static bool ChassisImage_no_resize = true;
				static bool ChassisImage_no_collapse = false;
				static bool ChassisImage_no_close = false;
				static bool ChassisImage_no_nav = true;
				static bool ChassisImage_no_background = false;
				static bool ChassisImage_no_bring_to_front = false;
				static bool ChassisImage_unsaved_document = false;

				ImGuiWindowFlags ChassisImage_flags = 1;
				if (ChassisImage_no_titlebar)        ChassisImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (ChassisImage_no_scrollbar)       ChassisImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!ChassisImage_no_menu)           ChassisImage_flags |= ImGuiWindowFlags_MenuBar;
				if (ChassisImage_no_move)            ChassisImage_flags |= ImGuiWindowFlags_NoMove;
				if (ChassisImage_no_resize)          ChassisImage_flags |= ImGuiWindowFlags_NoResize;
				if (ChassisImage_no_collapse)        ChassisImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (ChassisImage_no_nav)             ChassisImage_flags |= ImGuiWindowFlags_NoNav;
				if (ChassisImage_no_background)      ChassisImage_flags |= ImGuiWindowFlags_NoBackground;
				if (ChassisImage_no_bring_to_front)  ChassisImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (ChassisImage_unsaved_document)   ChassisImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (ChassisImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Chassis Machine Image", open, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2(chassis_image_width, chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvInteriorButtons_no_titlebar = true;
				const bool InvInteriorButtons_no_scrollbar = true;
				const bool InvInteriorButtons_no_menu = true;
				const bool InvInteriorButtons_no_move = true;
				const bool InvInteriorButtons_no_resize = true;
				const bool InvInteriorButtons_no_collapse = false;
				const bool InvInteriorButtons_no_close = false;
				const bool InvInteriorButtons_no_nav = false;
				static bool InvInteriorButtons_no_bring_to_front = false;
				const bool InvInteriorButtons_no_background = false;
				const bool InvInteriorButtons_unsaved_document = false;

				ImGuiWindowFlags InteriorButtonflags = 1;
				if (InvInteriorButtons_no_titlebar)        InteriorButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInteriorButtons_no_scrollbar)       InteriorButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInteriorButtons_no_menu)           InteriorButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvInteriorButtons_no_move)            InteriorButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvInteriorButtons_no_resize)          InteriorButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvInteriorButtons_no_collapse)        InteriorButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvInteriorButtons_no_nav)             InteriorButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvInteriorButtons_no_background)      InteriorButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvInteriorButtons_no_bring_to_front)  InteriorButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInteriorButtons_unsaved_document)   InteriorButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInteriorButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Interior Machine Manager", open, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int c;
				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from Interior inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags InteriorinventoryFlags = 1;
				if (InvInterior_no_titlebar)        InteriorinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInterior_no_scrollbar)       InteriorinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInterior_no_menu)           InteriorinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvInterior_no_move)            InteriorinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvInterior_no_resize)          InteriorinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvInterior_no_collapse)        InteriorinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvInterior_no_nav)             InteriorinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvInterior_no_background)      InteriorinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvInterior_no_bring_to_front)  InteriorinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInterior_unsaved_document)   InteriorinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInterior_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay1BaseExp);
				ImGui::Text("%d\n\n\n\n", bay1MidExp);
				ImGui::Text("%d\n\n\n", bay1HighExp);
				ImGui::Text("%d\n\n\n\n", bay1BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay1MidF150);
				ImGui::Text("%d", bay1HighF150);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay2BaseExp);
				ImGui::Text("%d\n\n\n\n", bay2MidExp);
				ImGui::Text("%d\n\n\n", bay2HighExp);
				ImGui::Text("%d\n\n\n\n", bay2BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay2MidF150);
				ImGui::Text("%d", bay2HighF150);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool InteriorImage_no_titlebar = true;
				static bool InteriorImage_no_scrollbar = true;
				static bool InteriorImage_no_menu = true;
				static bool InteriorImage_no_move = true;
				static bool InteriorImage_no_resize = false;
				static bool InteriorImage_no_collapse = false;
				static bool InteriorImage_no_close = false;
				static bool InteriorImage_no_nav = true;
				static bool InteriorImage_no_background = false;
				static bool InteriorImage_no_bring_to_front = false;
				static bool InteriorImage_unsaved_document = false;

				ImGuiWindowFlags InteriorImage_flags = 1;
				if (InteriorImage_no_titlebar)        InteriorImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (InteriorImage_no_scrollbar)       InteriorImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!InteriorImage_no_menu)           InteriorImage_flags |= ImGuiWindowFlags_MenuBar;
				if (InteriorImage_no_move)            InteriorImage_flags |= ImGuiWindowFlags_NoMove;
				if (InteriorImage_no_resize)          InteriorImage_flags |= ImGuiWindowFlags_NoResize;
				if (InteriorImage_no_collapse)        InteriorImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (InteriorImage_no_nav)             InteriorImage_flags |= ImGuiWindowFlags_NoNav;
				if (InteriorImage_no_background)      InteriorImage_flags |= ImGuiWindowFlags_NoBackground;
				if (InteriorImage_no_bring_to_front)  InteriorImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InteriorImage_unsaved_document)   InteriorImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (InteriorImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Interior Machine Image", open, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2(Interior_image_width, Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE


				static bool RedPaintVat_no_titlebar = true;
				static bool RedPaintVat_no_scrollbar = true;
				static bool RedPaintVat_no_menu = true;
				static bool RedPaintVat_no_move = true;
				static bool RedPaintVat_no_resize = true;
				static bool RedPaintVat_no_collapse = false;
				static bool RedPaintVat_no_close = false;
				static bool RedPaintVat_no_nav = true;
				static bool RedPaintVat_no_background = false;
				static bool RedPaintVat_no_bring_to_front = false;
				static bool RedPaintVat_unsaved_document = false;

				ImGuiWindowFlags RedPaintVat_flags = 1;
				if (RedPaintVat_no_titlebar)        RedPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (RedPaintVat_no_scrollbar)       RedPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!RedPaintVat_no_menu)           RedPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (RedPaintVat_no_move)            RedPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (RedPaintVat_no_resize)          RedPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (RedPaintVat_no_collapse)        RedPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (RedPaintVat_no_nav)             RedPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (RedPaintVat_no_background)      RedPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (RedPaintVat_no_bring_to_front)  RedPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (RedPaintVat_unsaved_document)   RedPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (RedPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("RedPaintVat Image", NULL, RedPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2(RedPaintVat_image_width, RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat

				static bool BluePaintVat_no_titlebar = true;
				static bool BluePaintVat_no_scrollbar = true;
				static bool BluePaintVat_no_menu = true;
				static bool BluePaintVat_no_move = true;
				static bool BluePaintVat_no_resize = true;
				static bool BluePaintVat_no_collapse = false;
				static bool BluePaintVat_no_close = false;
				static bool BluePaintVat_no_nav = true;
				static bool BluePaintVat_no_background = false;
				static bool BluePaintVat_no_bring_to_front = false;
				static bool BluePaintVat_unsaved_document = false;

				ImGuiWindowFlags BluePaintVat_flags = 1;
				if (BluePaintVat_no_titlebar)        BluePaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BluePaintVat_no_scrollbar)       BluePaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BluePaintVat_no_menu)           BluePaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (BluePaintVat_no_move)            BluePaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (BluePaintVat_no_resize)          BluePaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (BluePaintVat_no_collapse)        BluePaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (BluePaintVat_no_nav)             BluePaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (BluePaintVat_no_background)      BluePaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (BluePaintVat_no_bring_to_front)  BluePaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BluePaintVat_unsaved_document)   BluePaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BluePaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("BluePaintVat Image", NULL, BluePaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2(BluePaintVat_image_width, BluePaintVat_image_height));
				ImGui::End();

				//Green Vat

				static bool GreenPaintVat_no_titlebar = true;
				static bool GreenPaintVat_no_scrollbar = true;
				static bool GreenPaintVat_no_menu = true;
				static bool GreenPaintVat_no_move = true;
				static bool GreenPaintVat_no_resize = true;
				static bool GreenPaintVat_no_collapse = false;
				static bool GreenPaintVat_no_close = false;
				static bool GreenPaintVat_no_nav = true;
				static bool GreenPaintVat_no_background = false;
				static bool GreenPaintVat_no_bring_to_front = false;
				static bool GreenPaintVat_unsaved_document = false;

				ImGuiWindowFlags GreenPaintVat_flags = 1;
				if (GreenPaintVat_no_titlebar)        GreenPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GreenPaintVat_no_scrollbar)       GreenPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GreenPaintVat_no_menu)           GreenPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (GreenPaintVat_no_move)            GreenPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (GreenPaintVat_no_resize)          GreenPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (GreenPaintVat_no_collapse)        GreenPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (GreenPaintVat_no_nav)             GreenPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (GreenPaintVat_no_background)      GreenPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (GreenPaintVat_no_bring_to_front)  GreenPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GreenPaintVat_unsaved_document)   GreenPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GreenPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("GreenPaintVat Image", NULL, GreenPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2(GreenPaintVat_image_width, GreenPaintVat_image_height));
				ImGui::End();

				//DipTank

				static bool DipTank_no_titlebar = true;
				static bool DipTank_no_scrollbar = true;
				static bool DipTank_no_menu = true;
				static bool DipTank_no_move = true;
				static bool DipTank_no_resize = true;
				static bool DipTank_no_collapse = false;
				static bool DipTank_no_close = false;
				static bool DipTank_no_nav = true;
				static bool DipTank_no_background = false;
				static bool DipTank_no_bring_to_front = false;
				static bool DipTank_unsaved_document = false;

				ImGuiWindowFlags DipTank_flags = 1;
				if (DipTank_no_titlebar)        DipTank_flags |= ImGuiWindowFlags_NoTitleBar;
				if (DipTank_no_scrollbar)       DipTank_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!DipTank_no_menu)           DipTank_flags |= ImGuiWindowFlags_MenuBar;
				if (DipTank_no_move)            DipTank_flags |= ImGuiWindowFlags_NoMove;
				if (DipTank_no_resize)          DipTank_flags |= ImGuiWindowFlags_NoResize;
				if (DipTank_no_collapse)        DipTank_flags |= ImGuiWindowFlags_NoCollapse;
				if (DipTank_no_nav)             DipTank_flags |= ImGuiWindowFlags_NoNav;
				if (DipTank_no_background)      DipTank_flags |= ImGuiWindowFlags_NoBackground;
				if (DipTank_no_bring_to_front)  DipTank_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (DipTank_unsaved_document)   DipTank_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (DipTank_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("DipTank Image", NULL, DipTank_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				/*

				char ch=248;
			printf("Today's temperature was 23%cC",ch);
						*/
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);
				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2(DipTank_image_width, DipTank_image_height));
				ImGui::End();


				//PaintChamber

				static bool PaintChamber_no_titlebar = true;
				static bool PaintChamber_no_scrollbar = true;
				static bool PaintChamber_no_menu = true;
				static bool PaintChamber_no_move = true;
				static bool PaintChamber_no_resize = true;
				static bool PaintChamber_no_collapse = false;
				static bool PaintChamber_no_close = false;
				static bool PaintChamber_no_nav = true;
				static bool PaintChamber_no_background = false;
				static bool PaintChamber_no_bring_to_front = false;
				static bool PaintChamber_unsaved_document = false;

				ImGuiWindowFlags PaintChamber_flags = 1;
				if (PaintChamber_no_titlebar)        PaintChamber_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintChamber_no_scrollbar)       PaintChamber_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintChamber_no_menu)           PaintChamber_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintChamber_no_move)            PaintChamber_flags |= ImGuiWindowFlags_NoMove;
				if (PaintChamber_no_resize)          PaintChamber_flags |= ImGuiWindowFlags_NoResize;
				if (PaintChamber_no_collapse)        PaintChamber_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintChamber_no_nav)             PaintChamber_flags |= ImGuiWindowFlags_NoNav;
				if (PaintChamber_no_background)      PaintChamber_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintChamber_no_bring_to_front)  PaintChamber_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintChamber_unsaved_document)   PaintChamber_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintChamber_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("PaintChamber Image", NULL, PaintChamber_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);
				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2(PaintChamber_image_width, PaintChamber_image_height));
				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool PaintImage_no_titlebar = true;
				static bool PaintImage_no_scrollbar = true;
				static bool PaintImage_no_menu = true;
				static bool PaintImage_no_move = true;
				static bool PaintImage_no_resize = true;
				static bool PaintImage_no_collapse = false;
				static bool PaintImage_no_close = false;
				static bool PaintImage_no_nav = true;
				static bool PaintImage_no_background = false;
				static bool PaintImage_no_bring_to_front = false;
				static bool PaintImage_unsaved_document = false;

				ImGuiWindowFlags PaintImage_flags = 1;
				if (PaintImage_no_titlebar)        PaintImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintImage_no_scrollbar)       PaintImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintImage_no_menu)           PaintImage_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintImage_no_move)            PaintImage_flags |= ImGuiWindowFlags_NoMove;
				if (PaintImage_no_resize)          PaintImage_flags |= ImGuiWindowFlags_NoResize;
				if (PaintImage_no_collapse)        PaintImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintImage_no_nav)             PaintImage_flags |= ImGuiWindowFlags_NoNav;
				if (PaintImage_no_background)      PaintImage_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintImage_no_bring_to_front)  PaintImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintImage_unsaved_document)   PaintImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Paint Machine Image", open, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2(Paint_image_width, Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button
				const bool InvPaintButtons_no_titlebar = true;
				const bool InvPaintButtons_no_scrollbar = true;
				const bool InvPaintButtons_no_menu = true;
				const bool InvPaintButtons_no_move = true;
				const bool InvPaintButtons_no_resize = true;
				const bool InvPaintButtons_no_collapse = false;
				const bool InvPaintButtons_no_close = false;
				const bool InvPaintButtons_no_nav = false;
				static bool InvPaintButtons_no_bring_to_front = false;
				const bool InvPaintButtons_no_background = false;
				const bool InvPaintButtons_unsaved_document = false;

				ImGuiWindowFlags PaintButtonflags = 1;
				if (InvPaintButtons_no_titlebar)        PaintButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvPaintButtons_no_scrollbar)       PaintButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvPaintButtons_no_menu)           PaintButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvPaintButtons_no_move)            PaintButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvPaintButtons_no_resize)          PaintButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvPaintButtons_no_collapse)        PaintButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvPaintButtons_no_nav)             PaintButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvPaintButtons_no_background)      PaintButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvPaintButtons_no_bring_to_front)  PaintButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvPaintButtons_unsaved_document)   PaintButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvPaintButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Paint Machine Manager", open, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Paint Machine Values Adjuster", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Paint Vat to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Red Vat", "Green Vat", "Blue Vat" };
					ImGui::Combo("Paint Vats", &PaintVatitem_current, items, IM_ARRAYSIZE(items));
					if (ImGui::Button("Restock Button", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintVatitem_current == 0) {
							paintRedVatVol = 500;


						}
						else if (PaintVatitem_current == 1) {
							paintGreenVatVol = 500;
						}
						else if (PaintVatitem_current == 2) {
							paintBlueVatVol = 500;

						}
					}

					ImGui::Text("Please Select Which to Regulate\n\n");
					ImGui::Separator();
					const char* mach[] = { "Dip Tank Level", "Dip Tank Temperature", "Paint Chamber Temperature", "Paint Chamber Humidity", "Drying Chamber Temperature", "Drying Chamber Humidity" };
					ImGui::Combo("Values", &PaintMachineitem_current, mach, IM_ARRAYSIZE(mach));
					if (ImGui::Button("Regulate", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintMachineitem_current == 0) {
							DipTankFluidLevel = 1000;
						}
						else if (PaintMachineitem_current == 1) {
							DipTankTemp = 20.0;
						}
						else if (PaintMachineitem_current == 2) {
							PaintChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 3) {
							PaintChamberHMD = 45;
						}
						else if (PaintMachineitem_current == 4) {
							DryChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 5) {
							DryChamberHMD = 45;
						}
					}



					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}


				// Ends the window
				ImGui::End();

				// END OF PAINT MACHINE


				//GLOBAL PLANT STATS
					/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool GlobalTemp_no_titlebar = true;
				static bool GlobalTemp_no_scrollbar = true;
				static bool GlobalTemp_no_menu = true;
				static bool GlobalTemp_no_move = true;
				static bool GlobalTemp_no_resize = true;
				static bool GlobalTemp_no_collapse = false;
				static bool GlobalTemp_no_close = false;
				static bool GlobalTemp_no_nav = true;
				static bool GlobalTemp_no_background = false;
				static bool GlobalTemp_no_bring_to_front = false;
				static bool GlobalTemp_unsaved_document = false;

				ImGuiWindowFlags GlobalTemp_flags = 1;
				if (GlobalTemp_no_titlebar)        GlobalTemp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalTemp_no_scrollbar)       GlobalTemp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalTemp_no_menu)           GlobalTemp_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalTemp_no_move)            GlobalTemp_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalTemp_no_resize)          GlobalTemp_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalTemp_no_collapse)        GlobalTemp_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalTemp_no_nav)             GlobalTemp_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalTemp_no_background)      GlobalTemp_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalTemp_no_bring_to_front)  GlobalTemp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalTemp_unsaved_document)   GlobalTemp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalTemp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Temp", open, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();


				static bool GlobalAir_no_titlebar = true;
				static bool GlobalAir_no_scrollbar = true;
				static bool GlobalAir_no_menu = true;
				static bool GlobalAir_no_move = true;
				static bool GlobalAir_no_resize = true;
				static bool GlobalAir_no_collapse = false;
				static bool GlobalAir_no_close = false;
				static bool GlobalAir_no_nav = true;
				static bool GlobalAir_no_background = false;
				static bool GlobalAir_no_bring_to_front = false;
				static bool GlobalAir_unsaved_document = false;

				ImGuiWindowFlags GlobalAir_flags = 1;
				if (GlobalAir_no_titlebar)        GlobalAir_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalAir_no_scrollbar)       GlobalAir_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalAir_no_menu)           GlobalAir_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalAir_no_move)            GlobalAir_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalAir_no_resize)          GlobalAir_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalAir_no_collapse)        GlobalAir_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalAir_no_nav)             GlobalAir_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalAir_no_background)      GlobalAir_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalAir_no_bring_to_front)  GlobalAir_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalAir_unsaved_document)   GlobalAir_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalAir_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Air", open, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeQuota_no_titlebar = true;
				static bool VeQuota_no_scrollbar = true;
				static bool VeQuota_no_menu = true;
				static bool VeQuota_no_move = true;
				static bool VeQuota_no_resize = true;
				static bool VeQuota_no_collapse = false;
				static bool VeQuota_no_close = false;
				static bool VeQuota_no_nav = true;
				static bool VeQuota_no_background = false;
				static bool VeQuota_no_bring_to_front = false;
				static bool VeQuota_unsaved_document = false;

				ImGuiWindowFlags VeQuota_flags = 1;
				if (VeQuota_no_titlebar)        VeQuota_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeQuota_no_scrollbar)       VeQuota_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeQuota_no_menu)           VeQuota_flags |= ImGuiWindowFlags_MenuBar;
				if (VeQuota_no_move)            VeQuota_flags |= ImGuiWindowFlags_NoMove;
				if (VeQuota_no_resize)          VeQuota_flags |= ImGuiWindowFlags_NoResize;
				if (VeQuota_no_collapse)        VeQuota_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeQuota_no_nav)             VeQuota_flags |= ImGuiWindowFlags_NoNav;
				if (VeQuota_no_background)      VeQuota_flags |= ImGuiWindowFlags_NoBackground;
				if (VeQuota_no_bring_to_front)  VeQuota_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeQuota_unsaved_document)   VeQuota_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeQuota_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Quota", open, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeComp_no_titlebar = true;
				static bool VeComp_no_scrollbar = true;
				static bool VeComp_no_menu = true;
				static bool VeComp_no_move = true;
				static bool VeComp_no_resize = true;
				static bool VeComp_no_collapse = false;
				static bool VeComp_no_close = false;
				static bool VeComp_no_nav = true;
				static bool VeComp_no_background = false;
				static bool VeComp_no_bring_to_front = false;
				static bool VeComp_unsaved_document = false;

				ImGuiWindowFlags VeComp_flags = 1;
				if (VeComp_no_titlebar)        VeComp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeComp_no_scrollbar)       VeComp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeComp_no_menu)           VeComp_flags |= ImGuiWindowFlags_MenuBar;
				if (VeComp_no_move)            VeComp_flags |= ImGuiWindowFlags_NoMove;
				if (VeComp_no_resize)          VeComp_flags |= ImGuiWindowFlags_NoResize;
				if (VeComp_no_collapse)        VeComp_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeComp_no_nav)             VeComp_flags |= ImGuiWindowFlags_NoNav;
				if (VeComp_no_background)      VeComp_flags |= ImGuiWindowFlags_NoBackground;
				if (VeComp_no_bring_to_front)  VeComp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeComp_unsaved_document)   VeComp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeComp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Completed", open, VeComp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::TextColored({ 0, 255, 0, 1 }, "%d", VeComp);
				ImGui::PopStyleColor();
				ImGui::End();
				//END OF GLOBAL PLANT STATS


				//LOG STUFF
				// 
				ShowExampleAppLog(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!
				static bool  no_titlebar = true;
				static bool  no_scrollbar = true;
				static bool  no_menu = false;
				static bool  no_move = true;
				static bool  no_resize = true;
				static bool  no_collapse = false;
				static bool  no_close = false;
				static bool  no_nav = true;
				static bool  no_background = false;
				static bool  no_bring_to_front = true;
				static bool unsaved_document = false;

				ImGuiWindowFlags window_flags = 1;
				if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
				if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
				if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
				if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
				if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
				if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
				if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
				if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool toBeMade_no_titlebar = true;
				static bool toBeMade_no_scrollbar = true;
				static bool toBeMade_no_menu = true;
				static bool toBeMade_no_move = true;
				static bool toBeMade_no_resize = true;
				static bool toBeMade_no_collapse = false;
				static bool toBeMade_no_close = false;
				static bool toBeMade_no_nav = true;
				static bool toBeMade_no_background = false;
				static bool toBeMade_no_bring_to_front = false;
				static bool toBeMade_unsaved_document = false;

				ImGuiWindowFlags toBeMade_flags = 1;
				if (toBeMade_no_titlebar)        toBeMade_flags |= ImGuiWindowFlags_NoTitleBar;
				if (toBeMade_no_scrollbar)       toBeMade_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!toBeMade_no_menu)           toBeMade_flags |= ImGuiWindowFlags_MenuBar;
				if (toBeMade_no_move)            toBeMade_flags |= ImGuiWindowFlags_NoMove;
				if (toBeMade_no_resize)          toBeMade_flags |= ImGuiWindowFlags_NoResize;
				if (toBeMade_no_collapse)        toBeMade_flags |= ImGuiWindowFlags_NoCollapse;
				if (toBeMade_no_nav)             toBeMade_flags |= ImGuiWindowFlags_NoNav;
				if (toBeMade_no_background)      toBeMade_flags |= ImGuiWindowFlags_NoBackground;
				if (toBeMade_no_bring_to_front)  toBeMade_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (toBeMade_unsaved_document)   toBeMade_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (toBeMade_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(240, 121));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("ToBeMade Image", NULL, toBeMade_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2(toBeMade_image_width, toBeMade_image_height));


				ImGui::End();

				// Export variables to shader
				glUseProgram(shaderProgram);
				glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
				glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events

				i++;
				if (check == true) {
					if (i > 300) {

						chassisMachine = false;
						interiorMachine = true;
						i = 0;
					}
				}

			}
			if (!chassisMachine) {


			}

			while (interiorMachine) {

				glfwPollEvents();
				if (glfwWindowShouldClose(window)) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;
				}

				if (paintRedVatVol >= 250) {
					RedPaintVat = LoadTextureFromFile("Images/Redpaintfull.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol >= 100 && paintRedVatVol <= 249) {
					RedPaintVat = LoadTextureFromFile("Images/Redpainthalf.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}
				else if (paintRedVatVol <= 99 && paintRedVatVol >= 0) {
					RedPaintVat = LoadTextureFromFile("Images/redpaintlow.png", &RedPaintVat_image_texture, &RedPaintVat_image_width, &RedPaintVat_image_height);
				}

				if (paintGreenVatVol >= 250) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintfull.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol >= 100 && paintGreenVatVol <= 249) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpainthalf.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				else if (paintGreenVatVol <= 99 && paintGreenVatVol >= 0) {
					GreenPaintVat = LoadTextureFromFile("Images/greenpaintlow.png", &GreenPaintVat_image_texture, &GreenPaintVat_image_width, &GreenPaintVat_image_height);
				}
				if (paintBlueVatVol >= 250) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintfull.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol >= 100 && paintBlueVatVol <= 249) {
					BluePaintVat = LoadTextureFromFile("Images/bluepainthalf.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				else if (paintBlueVatVol <= 99 && paintBlueVatVol >= 0) {
					BluePaintVat = LoadTextureFromFile("Images/bluepaintlow.png", &BluePaintVat_image_texture, &BluePaintVat_image_width, &BluePaintVat_image_height);
				}
				//Set to blank
				bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
				IM_ASSERT(body);

				bool toBeMade = LoadTextureFromFile("Images/GUI/Orders/2022 F150 KING RANCH Super Crew Agate Black Metallic.jpg", &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
				IM_ASSERT(toBeMade);


				bool chassis = LoadTextureFromFile("Images/Blank.png", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
				IM_ASSERT(chassis);


				//set to blank
				bool Paint = LoadTextureFromFile("Images/Blank.png", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
				IM_ASSERT(Paint);

				//set to blank
				bool Interior = LoadTextureFromFile("Images/GUI/Interior Machine/2022 Base F150.jpg", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
				IM_ASSERT(Interior);


				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();



				// Tell OpenGL which Shader Program we want to use
				glUseProgram(shaderProgram);
				// Bind the VAO so OpenGL knows to use it
				glBindVertexArray(VAO);


				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



					//BAY SELECTION TOGGLE and restock button
				const bool InvBodyButtons_no_titlebar = true;
				const bool InvBodyButtons_no_scrollbar = true;
				const bool InvBodyButtons_no_menu = true;
				const bool InvBodyButtons_no_move = true;
				const bool InvBodyButtons_no_resize = true;
				const bool InvBodyButtons_no_collapse = false;
				const bool InvBodyButtons_no_close = false;
				const bool InvBodyButtons_no_nav = true;
				static bool InvBodyButtons_no_bring_to_front = false;
				const bool InvBodyButtons_no_background = false;
				const bool InvBodyButtons_unsaved_document = false;

				ImGuiWindowFlags bodyButtonflags = 1;
				if (InvBodyButtons_no_titlebar)        bodyButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBodyButtons_no_scrollbar)       bodyButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBodyButtons_no_menu)           bodyButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvBodyButtons_no_move)            bodyButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvBodyButtons_no_resize)          bodyButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvBodyButtons_no_collapse)        bodyButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvBodyButtons_no_nav)             bodyButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvBodyButtons_no_background)      bodyButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvBodyButtons_no_bring_to_front)  bodyButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBodyButtons_unsaved_document)   bodyButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBodyButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Body Machine Manager", open, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Max Expedition Body", "Regular Expediton Body", "Regular Cab F150 Body", "SuperCab F150 Body", "SuperCrew F150 Body" };

					ImGui::Combo("Body Type", &bodyitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();

						/*
						* BAY 2 selected
						Set inventory levels for that body type
						using the methods from body inventory bay

						use : bodyitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/


					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();

				ImGuiWindowFlags controlMenuFlags = 1;
				if (controlMenu_no_titlebar)        controlMenuFlags |= ImGuiWindowFlags_NoTitleBar;
				if (controlMenu_no_scrollbar)       controlMenuFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!controlMenu_no_menu)           controlMenuFlags |= ImGuiWindowFlags_MenuBar;
				if (controlMenu_no_move)            controlMenuFlags |= ImGuiWindowFlags_NoMove;
				if (controlMenu_no_resize)          controlMenuFlags |= ImGuiWindowFlags_NoResize;
				if (controlMenu_no_collapse)        controlMenuFlags |= ImGuiWindowFlags_NoCollapse;
				if (controlMenu_no_nav)             controlMenuFlags |= ImGuiWindowFlags_NoNav;
				if (controlMenu_no_background)      controlMenuFlags |= ImGuiWindowFlags_NoBackground;
				if (controlMenu_no_bring_to_front)  controlMenuFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (controlMenu_unsaved_document)   controlMenuFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (controlMenu_no_close)           open = NULL; // Don't pass our bool* to Begin
				// Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
			// Most functions would normally just crash if the context is missing.
				IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!");

				//CONTROL MENU CREATION
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.07f, 0.13f, 0.17f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(300, 100));
				ImGui::Begin("Control Menu", open, controlMenuFlags);
				if (ImGui::Button("Next Machine")) {

					interiorMachine = false;
					bodyMachine = true;
				}
				if (ImGui::Button("Close HMI")) {

					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;

					// Deletes all ImGUI instances
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();

					glfwDestroyWindow(window);

					glfwTerminate();
					ClosedHMI = true;
					return 0;

				}
				ImGui::Separator();

				if (ImGui::Checkbox("Start Line", &check)) {};


				ImGui::PopStyleColor();
				ImGui::End();


				ImGuiWindowFlags inventoryFlags = 1;
				if (InvBody1_no_titlebar)        inventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvBody1_no_scrollbar)       inventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvBody1_no_menu)           inventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvBody1_no_move)            inventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvBody1_no_resize)          inventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvBody1_no_collapse)        inventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvBody1_no_nav)             inventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvBody1_no_background)      inventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvBody1_no_bring_to_front)  inventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvBody1_unsaved_document)   inventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvBody1_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::SetNextWindowSize(ImVec2(50, 300));
				ImGui::Begin("bay1InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay1RegExp);
				ImGui::Text("%d\n\n\n\n", bay1RegCab);
				ImGui::Text("%d\n\n\n\n", bay1SupCab);
				ImGui::Text("%d", bay1SupCrew);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", open, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", bay1MaxExp);
				ImGui::Text("%d\n\n\n\n\n", bay2RegExp);
				ImGui::Text("%d\n\n\n\n", bay2RegCab);
				ImGui::Text("%d\n\n\n\n", bay2SupCab);
				ImGui::Text("%d", bay2SupCrew);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool BodyImage_no_titlebar = true;
				static bool BodyImage_no_scrollbar = true;
				static bool BodyImage_no_menu = true;
				static bool BodyImage_no_move = true;
				static bool BodyImage_no_resize = true;
				static bool BodyImage_no_collapse = false;
				static bool BodyImage_no_close = false;
				static bool BodyImage_no_nav = true;
				static bool BodyImage_no_background = false;
				static bool BodyImage_no_bring_to_front = false;
				static bool BodyImage_unsaved_document = false;

				ImGuiWindowFlags bodyImage_flags = 1;
				if (BodyImage_no_titlebar)        bodyImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BodyImage_no_scrollbar)       bodyImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BodyImage_no_menu)           bodyImage_flags |= ImGuiWindowFlags_MenuBar;
				if (BodyImage_no_move)            bodyImage_flags |= ImGuiWindowFlags_NoMove;
				if (BodyImage_no_resize)          bodyImage_flags |= ImGuiWindowFlags_NoResize;
				if (BodyImage_no_collapse)        bodyImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (BodyImage_no_nav)             bodyImage_flags |= ImGuiWindowFlags_NoNav;
				if (BodyImage_no_background)      bodyImage_flags |= ImGuiWindowFlags_NoBackground;
				if (BodyImage_no_bring_to_front)  bodyImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BodyImage_unsaved_document)   bodyImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BodyImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Body Machine Image", open, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2(body_image_width, body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvChassisButtons_no_titlebar = true;
				const bool InvChassisButtons_no_scrollbar = true;
				const bool InvChassisButtons_no_menu = true;
				const bool InvChassisButtons_no_move = true;
				const bool InvChassisButtons_no_resize = true;
				const bool InvChassisButtons_no_collapse = false;
				const bool InvChassisButtons_no_close = false;
				const bool InvChassisButtons_no_nav = false;
				static bool InvChassisButtons_no_bring_to_front = false;
				const bool InvChassisButtons_no_background = false;
				const bool InvChassisButtons_unsaved_document = false;

				ImGuiWindowFlags ChassisButtonflags = 1;
				if (InvChassisButtons_no_titlebar)        ChassisButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassisButtons_no_scrollbar)       ChassisButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassisButtons_no_menu)           ChassisButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvChassisButtons_no_move)            ChassisButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvChassisButtons_no_resize)          ChassisButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvChassisButtons_no_collapse)        ChassisButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassisButtons_no_nav)             ChassisButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvChassisButtons_no_background)      ChassisButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvChassisButtons_no_bring_to_front)  ChassisButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassisButtons_unsaved_document)   ChassisButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassisButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Chassis Machine Manager", open, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int b;
				ImGui::RadioButton("Bay 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &b, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (b == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "35LV6C", "35LV6HOC", "27LV6C", "33LV6C", "35LV6EcoC", "35LV6PwrBstC", "50LV8C" };

					ImGui::Combo("Engine Type", &chassisitem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 2 if bay 1 is selected you can only restock bay 2
						Set inventory levels for that engine type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags ChassisinventoryFlags = 0;
				if (InvChassis_no_titlebar)        ChassisinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvChassis_no_scrollbar)       ChassisinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvChassis_no_menu)           ChassisinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvChassis_no_move)            ChassisinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvChassis_no_resize)          ChassisinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvChassis_no_collapse)        ChassisinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvChassis_no_nav)             ChassisinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvChassis_no_background)      ChassisinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvChassis_no_bring_to_front)  ChassisinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvChassis_unsaved_document)   ChassisinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvChassis_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay135LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6HOC);
				ImGui::Text("%d\n\n\n", bay127LV6C);
				ImGui::Text("%d\n\n\n", bay133LV6C);
				ImGui::Text("%d\n\n\n\n", bay135LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay135LV6PwrBstC);
				ImGui::Text("%d", bay150LV8C);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", bay235LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6HOC);
				ImGui::Text("%d\n\n\n", bay227LV6C);
				ImGui::Text("%d\n\n\n", bay233LV6C);
				ImGui::Text("%d\n\n\n\n", bay235LV6EcoC);
				ImGui::Text("%d\n\n\n\n", bay235LV6PwrBstC);
				ImGui::Text("%d", bay250LV8C);
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool ChassisImage_no_titlebar = true;
				static bool ChassisImage_no_scrollbar = true;
				static bool ChassisImage_no_menu = true;
				static bool ChassisImage_no_move = true;
				static bool ChassisImage_no_resize = true;
				static bool ChassisImage_no_collapse = false;
				static bool ChassisImage_no_close = false;
				static bool ChassisImage_no_nav = true;
				static bool ChassisImage_no_background = false;
				static bool ChassisImage_no_bring_to_front = false;
				static bool ChassisImage_unsaved_document = false;

				ImGuiWindowFlags ChassisImage_flags = 1;
				if (ChassisImage_no_titlebar)        ChassisImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (ChassisImage_no_scrollbar)       ChassisImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!ChassisImage_no_menu)           ChassisImage_flags |= ImGuiWindowFlags_MenuBar;
				if (ChassisImage_no_move)            ChassisImage_flags |= ImGuiWindowFlags_NoMove;
				if (ChassisImage_no_resize)          ChassisImage_flags |= ImGuiWindowFlags_NoResize;
				if (ChassisImage_no_collapse)        ChassisImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (ChassisImage_no_nav)             ChassisImage_flags |= ImGuiWindowFlags_NoNav;
				if (ChassisImage_no_background)      ChassisImage_flags |= ImGuiWindowFlags_NoBackground;
				if (ChassisImage_no_bring_to_front)  ChassisImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (ChassisImage_unsaved_document)   ChassisImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (ChassisImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Chassis Machine Image", open, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2(chassis_image_width, chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button
				const bool InvInteriorButtons_no_titlebar = true;
				const bool InvInteriorButtons_no_scrollbar = true;
				const bool InvInteriorButtons_no_menu = true;
				const bool InvInteriorButtons_no_move = true;
				const bool InvInteriorButtons_no_resize = true;
				const bool InvInteriorButtons_no_collapse = false;
				const bool InvInteriorButtons_no_close = false;
				const bool InvInteriorButtons_no_nav = false;
				static bool InvInteriorButtons_no_bring_to_front = false;
				const bool InvInteriorButtons_no_background = false;
				const bool InvInteriorButtons_unsaved_document = false;

				ImGuiWindowFlags InteriorButtonflags = 1;
				if (InvInteriorButtons_no_titlebar)        InteriorButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInteriorButtons_no_scrollbar)       InteriorButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInteriorButtons_no_menu)           InteriorButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvInteriorButtons_no_move)            InteriorButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvInteriorButtons_no_resize)          InteriorButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvInteriorButtons_no_collapse)        InteriorButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvInteriorButtons_no_nav)             InteriorButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvInteriorButtons_no_background)      InteriorButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvInteriorButtons_no_bring_to_front)  InteriorButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInteriorButtons_unsaved_document)   InteriorButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInteriorButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Interior Machine Manager", open, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				static int c;
				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from engine inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}

				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Inventory You Would like to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						/*
						* Bay 1 if bay 2 is selected you can only restock bay 1
						Set inventory levels for that Interior type
						using the methods from Interior inventory bay

						use : engineitem_current to get the body type selected
						i0 to get the value for body type if greater than 500 set = 500
						if < 0 = 0
						*/

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
				// Ends the window
				ImGui::End();


				ImGuiWindowFlags InteriorinventoryFlags = 1;
				if (InvInterior_no_titlebar)        InteriorinventoryFlags |= ImGuiWindowFlags_NoTitleBar;
				if (InvInterior_no_scrollbar)       InteriorinventoryFlags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvInterior_no_menu)           InteriorinventoryFlags |= ImGuiWindowFlags_MenuBar;
				if (InvInterior_no_move)            InteriorinventoryFlags |= ImGuiWindowFlags_NoMove;
				if (InvInterior_no_resize)          InteriorinventoryFlags |= ImGuiWindowFlags_NoResize;
				if (InvInterior_no_collapse)        InteriorinventoryFlags |= ImGuiWindowFlags_NoCollapse;
				if (InvInterior_no_nav)             InteriorinventoryFlags |= ImGuiWindowFlags_NoNav;
				if (InvInterior_no_background)      InteriorinventoryFlags |= ImGuiWindowFlags_NoBackground;
				if (InvInterior_no_bring_to_front)  InteriorinventoryFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvInterior_unsaved_document)   InteriorinventoryFlags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvInterior_no_close)           open = NULL; // Don't pass our bool* to Begin

				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey

				ImGui::SetNextWindowSize(ImVec2(50, 365));
				ImGui::Begin("bay1InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay1BaseExp);
				ImGui::Text("%d\n\n\n\n", bay1MidExp);
				ImGui::Text("%d\n\n\n", bay1HighExp);
				ImGui::Text("%d\n\n\n\n", bay1BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay1MidF150);
				ImGui::Text("%d", bay1HighF150);
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", bay2BaseExp);
				ImGui::Text("%d\n\n\n\n", bay2MidExp);
				ImGui::Text("%d\n\n\n", bay2HighExp);
				ImGui::Text("%d\n\n\n\n", bay2BaseF150);
				ImGui::Text("%d\n\n\n\n\n", bay2MidF150);
				ImGui::Text("%d", bay2HighF150);
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool InteriorImage_no_titlebar = true;
				static bool InteriorImage_no_scrollbar = true;
				static bool InteriorImage_no_menu = true;
				static bool InteriorImage_no_move = true;
				static bool InteriorImage_no_resize = false;
				static bool InteriorImage_no_collapse = false;
				static bool InteriorImage_no_close = false;
				static bool InteriorImage_no_nav = true;
				static bool InteriorImage_no_background = false;
				static bool InteriorImage_no_bring_to_front = false;
				static bool InteriorImage_unsaved_document = false;

				ImGuiWindowFlags InteriorImage_flags = 1;
				if (InteriorImage_no_titlebar)        InteriorImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (InteriorImage_no_scrollbar)       InteriorImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!InteriorImage_no_menu)           InteriorImage_flags |= ImGuiWindowFlags_MenuBar;
				if (InteriorImage_no_move)            InteriorImage_flags |= ImGuiWindowFlags_NoMove;
				if (InteriorImage_no_resize)          InteriorImage_flags |= ImGuiWindowFlags_NoResize;
				if (InteriorImage_no_collapse)        InteriorImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (InteriorImage_no_nav)             InteriorImage_flags |= ImGuiWindowFlags_NoNav;
				if (InteriorImage_no_background)      InteriorImage_flags |= ImGuiWindowFlags_NoBackground;
				if (InteriorImage_no_bring_to_front)  InteriorImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InteriorImage_unsaved_document)   InteriorImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (InteriorImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Interior Machine Image", open, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2(Interior_image_width, Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE


				static bool RedPaintVat_no_titlebar = true;
				static bool RedPaintVat_no_scrollbar = true;
				static bool RedPaintVat_no_menu = true;
				static bool RedPaintVat_no_move = true;
				static bool RedPaintVat_no_resize = true;
				static bool RedPaintVat_no_collapse = false;
				static bool RedPaintVat_no_close = false;
				static bool RedPaintVat_no_nav = true;
				static bool RedPaintVat_no_background = false;
				static bool RedPaintVat_no_bring_to_front = false;
				static bool RedPaintVat_unsaved_document = false;

				ImGuiWindowFlags RedPaintVat_flags = 1;
				if (RedPaintVat_no_titlebar)        RedPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (RedPaintVat_no_scrollbar)       RedPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!RedPaintVat_no_menu)           RedPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (RedPaintVat_no_move)            RedPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (RedPaintVat_no_resize)          RedPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (RedPaintVat_no_collapse)        RedPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (RedPaintVat_no_nav)             RedPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (RedPaintVat_no_background)      RedPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (RedPaintVat_no_bring_to_front)  RedPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (RedPaintVat_unsaved_document)   RedPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (RedPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("RedPaintVat Image", NULL, RedPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2(RedPaintVat_image_width, RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat

				static bool BluePaintVat_no_titlebar = true;
				static bool BluePaintVat_no_scrollbar = true;
				static bool BluePaintVat_no_menu = true;
				static bool BluePaintVat_no_move = true;
				static bool BluePaintVat_no_resize = true;
				static bool BluePaintVat_no_collapse = false;
				static bool BluePaintVat_no_close = false;
				static bool BluePaintVat_no_nav = true;
				static bool BluePaintVat_no_background = false;
				static bool BluePaintVat_no_bring_to_front = false;
				static bool BluePaintVat_unsaved_document = false;

				ImGuiWindowFlags BluePaintVat_flags = 1;
				if (BluePaintVat_no_titlebar)        BluePaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (BluePaintVat_no_scrollbar)       BluePaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!BluePaintVat_no_menu)           BluePaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (BluePaintVat_no_move)            BluePaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (BluePaintVat_no_resize)          BluePaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (BluePaintVat_no_collapse)        BluePaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (BluePaintVat_no_nav)             BluePaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (BluePaintVat_no_background)      BluePaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (BluePaintVat_no_bring_to_front)  BluePaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (BluePaintVat_unsaved_document)   BluePaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (BluePaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("BluePaintVat Image", NULL, BluePaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2(BluePaintVat_image_width, BluePaintVat_image_height));
				ImGui::End();

				//Green Vat

				static bool GreenPaintVat_no_titlebar = true;
				static bool GreenPaintVat_no_scrollbar = true;
				static bool GreenPaintVat_no_menu = true;
				static bool GreenPaintVat_no_move = true;
				static bool GreenPaintVat_no_resize = true;
				static bool GreenPaintVat_no_collapse = false;
				static bool GreenPaintVat_no_close = false;
				static bool GreenPaintVat_no_nav = true;
				static bool GreenPaintVat_no_background = false;
				static bool GreenPaintVat_no_bring_to_front = false;
				static bool GreenPaintVat_unsaved_document = false;

				ImGuiWindowFlags GreenPaintVat_flags = 1;
				if (GreenPaintVat_no_titlebar)        GreenPaintVat_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GreenPaintVat_no_scrollbar)       GreenPaintVat_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GreenPaintVat_no_menu)           GreenPaintVat_flags |= ImGuiWindowFlags_MenuBar;
				if (GreenPaintVat_no_move)            GreenPaintVat_flags |= ImGuiWindowFlags_NoMove;
				if (GreenPaintVat_no_resize)          GreenPaintVat_flags |= ImGuiWindowFlags_NoResize;
				if (GreenPaintVat_no_collapse)        GreenPaintVat_flags |= ImGuiWindowFlags_NoCollapse;
				if (GreenPaintVat_no_nav)             GreenPaintVat_flags |= ImGuiWindowFlags_NoNav;
				if (GreenPaintVat_no_background)      GreenPaintVat_flags |= ImGuiWindowFlags_NoBackground;
				if (GreenPaintVat_no_bring_to_front)  GreenPaintVat_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GreenPaintVat_unsaved_document)   GreenPaintVat_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GreenPaintVat_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(110, 140));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("GreenPaintVat Image", NULL, GreenPaintVat_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2(GreenPaintVat_image_width, GreenPaintVat_image_height));
				ImGui::End();

				//DipTank

				static bool DipTank_no_titlebar = true;
				static bool DipTank_no_scrollbar = true;
				static bool DipTank_no_menu = true;
				static bool DipTank_no_move = true;
				static bool DipTank_no_resize = true;
				static bool DipTank_no_collapse = false;
				static bool DipTank_no_close = false;
				static bool DipTank_no_nav = true;
				static bool DipTank_no_background = false;
				static bool DipTank_no_bring_to_front = false;
				static bool DipTank_unsaved_document = false;

				ImGuiWindowFlags DipTank_flags = 1;
				if (DipTank_no_titlebar)        DipTank_flags |= ImGuiWindowFlags_NoTitleBar;
				if (DipTank_no_scrollbar)       DipTank_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!DipTank_no_menu)           DipTank_flags |= ImGuiWindowFlags_MenuBar;
				if (DipTank_no_move)            DipTank_flags |= ImGuiWindowFlags_NoMove;
				if (DipTank_no_resize)          DipTank_flags |= ImGuiWindowFlags_NoResize;
				if (DipTank_no_collapse)        DipTank_flags |= ImGuiWindowFlags_NoCollapse;
				if (DipTank_no_nav)             DipTank_flags |= ImGuiWindowFlags_NoNav;
				if (DipTank_no_background)      DipTank_flags |= ImGuiWindowFlags_NoBackground;
				if (DipTank_no_bring_to_front)  DipTank_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (DipTank_unsaved_document)   DipTank_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (DipTank_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("DipTank Image", NULL, DipTank_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				/*

				char ch=248;
			printf("Today's temperature was 23%cC",ch);
						*/
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);
				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2(DipTank_image_width, DipTank_image_height));
				ImGui::End();


				//PaintChamber

				static bool PaintChamber_no_titlebar = true;
				static bool PaintChamber_no_scrollbar = true;
				static bool PaintChamber_no_menu = true;
				static bool PaintChamber_no_move = true;
				static bool PaintChamber_no_resize = true;
				static bool PaintChamber_no_collapse = false;
				static bool PaintChamber_no_close = false;
				static bool PaintChamber_no_nav = true;
				static bool PaintChamber_no_background = false;
				static bool PaintChamber_no_bring_to_front = false;
				static bool PaintChamber_unsaved_document = false;

				ImGuiWindowFlags PaintChamber_flags = 1;
				if (PaintChamber_no_titlebar)        PaintChamber_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintChamber_no_scrollbar)       PaintChamber_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintChamber_no_menu)           PaintChamber_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintChamber_no_move)            PaintChamber_flags |= ImGuiWindowFlags_NoMove;
				if (PaintChamber_no_resize)          PaintChamber_flags |= ImGuiWindowFlags_NoResize;
				if (PaintChamber_no_collapse)        PaintChamber_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintChamber_no_nav)             PaintChamber_flags |= ImGuiWindowFlags_NoNav;
				if (PaintChamber_no_background)      PaintChamber_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintChamber_no_bring_to_front)  PaintChamber_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintChamber_unsaved_document)   PaintChamber_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintChamber_no_close)           open = NULL; // Don't pass our bool* to Begin

				ImGui::SetNextWindowSize(ImVec2(200, 200));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("PaintChamber Image", NULL, PaintChamber_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);
				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2(PaintChamber_image_width, PaintChamber_image_height));
				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool PaintImage_no_titlebar = true;
				static bool PaintImage_no_scrollbar = true;
				static bool PaintImage_no_menu = true;
				static bool PaintImage_no_move = true;
				static bool PaintImage_no_resize = true;
				static bool PaintImage_no_collapse = false;
				static bool PaintImage_no_close = false;
				static bool PaintImage_no_nav = true;
				static bool PaintImage_no_background = false;
				static bool PaintImage_no_bring_to_front = false;
				static bool PaintImage_unsaved_document = false;

				ImGuiWindowFlags PaintImage_flags = 1;
				if (PaintImage_no_titlebar)        PaintImage_flags |= ImGuiWindowFlags_NoTitleBar;
				if (PaintImage_no_scrollbar)       PaintImage_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!PaintImage_no_menu)           PaintImage_flags |= ImGuiWindowFlags_MenuBar;
				if (PaintImage_no_move)            PaintImage_flags |= ImGuiWindowFlags_NoMove;
				if (PaintImage_no_resize)          PaintImage_flags |= ImGuiWindowFlags_NoResize;
				if (PaintImage_no_collapse)        PaintImage_flags |= ImGuiWindowFlags_NoCollapse;
				if (PaintImage_no_nav)             PaintImage_flags |= ImGuiWindowFlags_NoNav;
				if (PaintImage_no_background)      PaintImage_flags |= ImGuiWindowFlags_NoBackground;
				if (PaintImage_no_bring_to_front)  PaintImage_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (PaintImage_unsaved_document)   PaintImage_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (PaintImage_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(350, 179));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Paint Machine Image", open, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2(Paint_image_width, Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button
				const bool InvPaintButtons_no_titlebar = true;
				const bool InvPaintButtons_no_scrollbar = true;
				const bool InvPaintButtons_no_menu = true;
				const bool InvPaintButtons_no_move = true;
				const bool InvPaintButtons_no_resize = true;
				const bool InvPaintButtons_no_collapse = false;
				const bool InvPaintButtons_no_close = false;
				const bool InvPaintButtons_no_nav = false;
				static bool InvPaintButtons_no_bring_to_front = false;
				const bool InvPaintButtons_no_background = false;
				const bool InvPaintButtons_unsaved_document = false;

				ImGuiWindowFlags PaintButtonflags = 1;
				if (InvPaintButtons_no_titlebar)        PaintButtonflags |= ImGuiWindowFlags_NoTitleBar;
				if (InvPaintButtons_no_scrollbar)       PaintButtonflags |= ImGuiWindowFlags_NoScrollbar;
				if (!InvPaintButtons_no_menu)           PaintButtonflags |= ImGuiWindowFlags_MenuBar;
				if (InvPaintButtons_no_move)            PaintButtonflags |= ImGuiWindowFlags_NoMove;
				if (InvPaintButtons_no_resize)          PaintButtonflags |= ImGuiWindowFlags_NoResize;
				if (InvPaintButtons_no_collapse)        PaintButtonflags |= ImGuiWindowFlags_NoCollapse;
				if (InvPaintButtons_no_nav)             PaintButtonflags |= ImGuiWindowFlags_NoNav;
				if (InvPaintButtons_no_background)      PaintButtonflags |= ImGuiWindowFlags_NoBackground;
				if (InvPaintButtons_no_bring_to_front)  PaintButtonflags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (InvPaintButtons_unsaved_document)   PaintButtonflags |= ImGuiWindowFlags_UnsavedDocument;
				if (InvPaintButtons_no_close)           open = NULL; // Don't pass our bool* to Begin

				// ImGUI window creation
				ImGui::Begin("Paint Machine Manager", open, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing

				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Paint Machine Values Adjuster", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Please Select Which Paint Vat to Restock\n\n");
					ImGui::Separator();
					const char* items[] = { "Red Vat", "Green Vat", "Blue Vat" };
					ImGui::Combo("Paint Vats", &PaintVatitem_current, items, IM_ARRAYSIZE(items));
					if (ImGui::Button("Restock Button", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintVatitem_current == 0) {
							paintRedVatVol = 500;


						}
						else if (PaintVatitem_current == 1) {
							paintGreenVatVol = 500;
						}
						else if (PaintVatitem_current == 2) {
							paintBlueVatVol = 500;

						}
					}

					ImGui::Text("Please Select Which to Regulate\n\n");
					ImGui::Separator();
					const char* mach[] = { "Dip Tank Level", "Dip Tank Temperature", "Paint Chamber Temperature", "Paint Chamber Humidity", "Drying Chamber Temperature", "Drying Chamber Humidity" };
					ImGui::Combo("Values", &PaintMachineitem_current, mach, IM_ARRAYSIZE(mach));
					if (ImGui::Button("Regulate", ImVec2(120, 0))) {
						//RESTOCKING PAINT VALUE

						if (PaintMachineitem_current == 0) {
							DipTankFluidLevel = 1000;
						}
						else if (PaintMachineitem_current == 1) {
							DipTankTemp = 20.0;
						}
						else if (PaintMachineitem_current == 2) {
							PaintChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 3) {
							PaintChamberHMD = 45;
						}
						else if (PaintMachineitem_current == 4) {
							DryChamberTemp = 20.0;
						}
						else if (PaintMachineitem_current == 5) {
							DryChamberHMD = 45;
						}
					}



					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}


				// Ends the window
				ImGui::End();

				// END OF PAINT MACHINE


				//GLOBAL PLANT STATS
					/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool GlobalTemp_no_titlebar = true;
				static bool GlobalTemp_no_scrollbar = true;
				static bool GlobalTemp_no_menu = true;
				static bool GlobalTemp_no_move = true;
				static bool GlobalTemp_no_resize = true;
				static bool GlobalTemp_no_collapse = false;
				static bool GlobalTemp_no_close = false;
				static bool GlobalTemp_no_nav = true;
				static bool GlobalTemp_no_background = false;
				static bool GlobalTemp_no_bring_to_front = false;
				static bool GlobalTemp_unsaved_document = false;

				ImGuiWindowFlags GlobalTemp_flags = 1;
				if (GlobalTemp_no_titlebar)        GlobalTemp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalTemp_no_scrollbar)       GlobalTemp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalTemp_no_menu)           GlobalTemp_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalTemp_no_move)            GlobalTemp_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalTemp_no_resize)          GlobalTemp_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalTemp_no_collapse)        GlobalTemp_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalTemp_no_nav)             GlobalTemp_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalTemp_no_background)      GlobalTemp_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalTemp_no_bring_to_front)  GlobalTemp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalTemp_unsaved_document)   GlobalTemp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalTemp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Temp", open, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();


				static bool GlobalAir_no_titlebar = true;
				static bool GlobalAir_no_scrollbar = true;
				static bool GlobalAir_no_menu = true;
				static bool GlobalAir_no_move = true;
				static bool GlobalAir_no_resize = true;
				static bool GlobalAir_no_collapse = false;
				static bool GlobalAir_no_close = false;
				static bool GlobalAir_no_nav = true;
				static bool GlobalAir_no_background = false;
				static bool GlobalAir_no_bring_to_front = false;
				static bool GlobalAir_unsaved_document = false;

				ImGuiWindowFlags GlobalAir_flags = 1;
				if (GlobalAir_no_titlebar)        GlobalAir_flags |= ImGuiWindowFlags_NoTitleBar;
				if (GlobalAir_no_scrollbar)       GlobalAir_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!GlobalAir_no_menu)           GlobalAir_flags |= ImGuiWindowFlags_MenuBar;
				if (GlobalAir_no_move)            GlobalAir_flags |= ImGuiWindowFlags_NoMove;
				if (GlobalAir_no_resize)          GlobalAir_flags |= ImGuiWindowFlags_NoResize;
				if (GlobalAir_no_collapse)        GlobalAir_flags |= ImGuiWindowFlags_NoCollapse;
				if (GlobalAir_no_nav)             GlobalAir_flags |= ImGuiWindowFlags_NoNav;
				if (GlobalAir_no_background)      GlobalAir_flags |= ImGuiWindowFlags_NoBackground;
				if (GlobalAir_no_bring_to_front)  GlobalAir_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (GlobalAir_unsaved_document)   GlobalAir_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (GlobalAir_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Global Air", open, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeQuota_no_titlebar = true;
				static bool VeQuota_no_scrollbar = true;
				static bool VeQuota_no_menu = true;
				static bool VeQuota_no_move = true;
				static bool VeQuota_no_resize = true;
				static bool VeQuota_no_collapse = false;
				static bool VeQuota_no_close = false;
				static bool VeQuota_no_nav = true;
				static bool VeQuota_no_background = false;
				static bool VeQuota_no_bring_to_front = false;
				static bool VeQuota_unsaved_document = false;

				ImGuiWindowFlags VeQuota_flags = 1;
				if (VeQuota_no_titlebar)        VeQuota_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeQuota_no_scrollbar)       VeQuota_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeQuota_no_menu)           VeQuota_flags |= ImGuiWindowFlags_MenuBar;
				if (VeQuota_no_move)            VeQuota_flags |= ImGuiWindowFlags_NoMove;
				if (VeQuota_no_resize)          VeQuota_flags |= ImGuiWindowFlags_NoResize;
				if (VeQuota_no_collapse)        VeQuota_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeQuota_no_nav)             VeQuota_flags |= ImGuiWindowFlags_NoNav;
				if (VeQuota_no_background)      VeQuota_flags |= ImGuiWindowFlags_NoBackground;
				if (VeQuota_no_bring_to_front)  VeQuota_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeQuota_unsaved_document)   VeQuota_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeQuota_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Quota", open, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();



				static bool VeComp_no_titlebar = true;
				static bool VeComp_no_scrollbar = true;
				static bool VeComp_no_menu = true;
				static bool VeComp_no_move = true;
				static bool VeComp_no_resize = true;
				static bool VeComp_no_collapse = false;
				static bool VeComp_no_close = false;
				static bool VeComp_no_nav = true;
				static bool VeComp_no_background = false;
				static bool VeComp_no_bring_to_front = false;
				static bool VeComp_unsaved_document = false;

				ImGuiWindowFlags VeComp_flags = 1;
				if (VeComp_no_titlebar)        VeComp_flags |= ImGuiWindowFlags_NoTitleBar;
				if (VeComp_no_scrollbar)       VeComp_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!VeComp_no_menu)           VeComp_flags |= ImGuiWindowFlags_MenuBar;
				if (VeComp_no_move)            VeComp_flags |= ImGuiWindowFlags_NoMove;
				if (VeComp_no_resize)          VeComp_flags |= ImGuiWindowFlags_NoResize;
				if (VeComp_no_collapse)        VeComp_flags |= ImGuiWindowFlags_NoCollapse;
				if (VeComp_no_nav)             VeComp_flags |= ImGuiWindowFlags_NoNav;
				if (VeComp_no_background)      VeComp_flags |= ImGuiWindowFlags_NoBackground;
				if (VeComp_no_bring_to_front)  VeComp_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (VeComp_unsaved_document)   VeComp_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (VeComp_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(150, 65));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 0.0f)); // Set window background to grey


				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Vehicle Completed", open, VeComp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::TextColored({ 0, 255, 0, 1 }, "%d", VeComp);
				ImGui::PopStyleColor();
				ImGui::End();
				//END OF GLOBAL PLANT STATS


				//LOG STUFF
				// 
				ShowExampleAppLog(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!
				static bool  no_titlebar = true;
				static bool  no_scrollbar = true;
				static bool  no_menu = false;
				static bool  no_move = true;
				static bool  no_resize = true;
				static bool  no_collapse = false;
				static bool  no_close = false;
				static bool  no_nav = true;
				static bool  no_background = false;
				static bool  no_bring_to_front = true;
				static bool unsaved_document = false;

				ImGuiWindowFlags window_flags = 1;
				if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
				if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
				if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
				if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
				if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
				if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
				if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
				if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>
				static bool toBeMade_no_titlebar = true;
				static bool toBeMade_no_scrollbar = true;
				static bool toBeMade_no_menu = true;
				static bool toBeMade_no_move = true;
				static bool toBeMade_no_resize = true;
				static bool toBeMade_no_collapse = false;
				static bool toBeMade_no_close = false;
				static bool toBeMade_no_nav = true;
				static bool toBeMade_no_background = false;
				static bool toBeMade_no_bring_to_front = false;
				static bool toBeMade_unsaved_document = false;

				ImGuiWindowFlags toBeMade_flags = 1;
				if (toBeMade_no_titlebar)        toBeMade_flags |= ImGuiWindowFlags_NoTitleBar;
				if (toBeMade_no_scrollbar)       toBeMade_flags |= ImGuiWindowFlags_NoScrollbar;
				if (!toBeMade_no_menu)           toBeMade_flags |= ImGuiWindowFlags_MenuBar;
				if (toBeMade_no_move)            toBeMade_flags |= ImGuiWindowFlags_NoMove;
				if (toBeMade_no_resize)          toBeMade_flags |= ImGuiWindowFlags_NoResize;
				if (toBeMade_no_collapse)        toBeMade_flags |= ImGuiWindowFlags_NoCollapse;
				if (toBeMade_no_nav)             toBeMade_flags |= ImGuiWindowFlags_NoNav;
				if (toBeMade_no_background)      toBeMade_flags |= ImGuiWindowFlags_NoBackground;
				if (toBeMade_no_bring_to_front)  toBeMade_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
				if (toBeMade_unsaved_document)   toBeMade_flags |= ImGuiWindowFlags_UnsavedDocument;
				if (toBeMade_no_close)           open = NULL; // Don't pass our bool* to Begin

				// We specify a default position/size in case there's no data in the .ini file.
				// We only do it to make the demo applications a little more welcoming, but typically this isn't required.

				ImGui::SetNextWindowSize(ImVec2(240, 121));

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("ToBeMade Image", NULL, toBeMade_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				ImGui::PushItemWidth(ImGui::GetFontSize() * -12);



				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2(toBeMade_image_width, toBeMade_image_height));


				ImGui::End();

				// Export variables to shader
				glUseProgram(shaderProgram);
				glUniform1f(glGetUniformLocation(shaderProgram, "size"), size);
				glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);

				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events
			
				i++;
				if (check == true) {
					if (i > 300) {
						interiorMachine = false;
						bodyMachine = true;
						i = 0;
					}
				}
			}

			if (!interiorMachine) {
				if (!ClosedHMI) {
					// Deletes all ImGUI instances
					ImGui_ImplOpenGL3_Shutdown();
					ImGui_ImplGlfw_Shutdown();
					ImGui::DestroyContext();

					glfwDestroyWindow(window);

					glfwTerminate();

					// Initialize GLFW
					glfwInit();

					// Tell GLFW what version of OpenGL we are using 
					// In this case we are using OpenGL 3.3
					glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
					glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
					// Tell GLFW we are using the CORE profile
					// So that means we only have the modern functions
					glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

					// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
					window = glfwCreateWindow(1920, 1080, "ImGui + GLFW", glfwGetPrimaryMonitor(), NULL);
					// Error check if the window fails to create
					if (window == NULL)
					{
						std::cout << "Failed to create GLFW window" << std::endl;
						glfwTerminate();
						return -1;
					}
					// Introduce the window into the current context
					glfwMakeContextCurrent(window);

					//Load GLAD so it configures OpenGL
					gladLoadGL();
					// Specify the viewport of OpenGL in the Window
					// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
					glViewport(0, 0, 1280, 1024);

					IMGUI_CHECKVERSION();
					ImGui::CreateContext();
					ImGuiIO& io = ImGui::GetIO(); (void)io;
					ImGui::StyleColorsDark();
					ImGui_ImplGlfw_InitForOpenGL(window, true);
					ImGui_ImplOpenGL3_Init("#version 330");


					bool ret = LoadTextureFromFile("Images/Ford_Assembly_Line_GUI_Background.png", &my_image_texture, &my_image_width, &my_image_height);
					IM_ASSERT(ret);

					bool DipTank = LoadTextureFromFile("Images/Diptank.png", &DipTank_image_texture, &DipTank_image_width, &DipTank_image_height);
					IM_ASSERT(DipTank);

					bool PaintChamber = LoadTextureFromFile("Images/DryingchamberIcon.png", &PaintChamber_image_texture, &PaintChamber_image_width, &PaintChamber_image_height);
					IM_ASSERT(PaintChamber);
				}

			}




	}
		
	
	
	if (!ClosedHMI) {
		// Deletes all ImGUI instances
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();


		// Delete window before ending the program
		glfwDestroyWindow(window);
	}
		// Terminate GLFW before ending the program
		glfwTerminate();
	
	return 0;
}



