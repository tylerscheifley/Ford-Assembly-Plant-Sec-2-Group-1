#include "plant.h"
#include "plantFiles.h"
#include "GUILog.h"

Log readFileLogs;

extern void ShowReadLogs(bool* p_open)
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

	//Print all log files to the GUI log - with proper logs
	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin("Log", p_open, Log_flags);
	if (ImGui::SmallButton("Refresh Log"))
	{
		//the categories that the user can filter by
		const char* categories[4] = { "BodyMachineInvLog", "ChassisMachineInvLog", "InteriorMachineInvLog", "PlantLog" };

		//load the body machine logs from the body machine log file into the gui log
		ifstream file("BodyMachineInventoryLog.txt");
		string line;
		if (file.is_open()) {
			const char* category = categories[0];
			while (!file.eof()) {
				getline(file, line);
				readFileLogs.AddLog("[%s] %s\n", category, line.c_str());
			}
		}
		else {
			cout << "file cannot be opened!\n" << "Filename : BodyMachineInventoryLog.txt" << endl;
		}
		file.close();

		//load the chassis machine logs from the chassis machine log file into the gui log
		ifstream file1("ChassisMachineInventoryLog.txt");
		string line1;
		if (file1.is_open()) {
			const char* category1 = categories[1];
			while (!file1.eof()) {
				getline(file1, line1);
				readFileLogs.AddLog("[%s] %s\n", category1, line1.c_str());
			}
		}
		else {
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}
		file1.close();

		//load the interior machine logs from the interior machine log file into the gui log
		ifstream file2("InteriorMachineInventoryLog.txt");
		string line2;
		if (file2.is_open()) {
			const char* category2 = categories[2];
			while (!file2.eof()) {
				getline(file2, line2);
				readFileLogs.AddLog("[%s] %s\n", category2, line2.c_str());
			}
		}
		else {
			cout << "file cannot be opened!\n" << "Filename : ChassisMachineInventoryLog.txt" << endl;
		}
		file2.close();

		//load the plant logs from the plant log file into the gui log
		ifstream file3("plantLog.txt");
		string line3;
		if (file3.is_open()) {
			const char* category3 = categories[3];
			while (!file3.eof()) {
				getline(file3, line3);
				readFileLogs.AddLog("[%s] %s\n", category3, line3.c_str());
			}
		}
		else {
			cout << "file cannot be opened!\n" << "Filename : plantLog.txt" << endl;
		}
		file3.close();
	}
	ImGui::End();

	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	readFileLogs.Draw("Log", p_open);
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

//This converts the name of the body panel in the object to the correct format for the file name
string convertBodyPanelSetToFileName(string bodyPanelSet, Order order)
{
	string convertedBodyPanelSetName;
	if (order.getYear() == "2022")
	{
		if (bodyPanelSet == "Regular")
		{
			convertedBodyPanelSetName = " Regular Cab ";
		}
		else if (bodyPanelSet == "SuperCrew")
		{
			convertedBodyPanelSetName = " Super Crew ";
		}
		else if (bodyPanelSet == "SuperCab")
		{
			convertedBodyPanelSetName = " Super Cab ";
		}
	}
	else if (order.getYear() == "2023")
	{
		if (bodyPanelSet == "REG")
		{
			convertedBodyPanelSetName = " ";
		}
		else if (bodyPanelSet == "MAX")
		{
			convertedBodyPanelSetName = " MAX ";
		}
	}
	return convertedBodyPanelSetName;
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

	return startOfPath + order.getYear() + " " + order.getModel() + " " + order.getTrim() + convertBodyPanelSetToFileName(order.getBodyPanelSet(), order) + order.getColour() + ".jpg";
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
				fileName = "2023 High Expedition.jpg";
			}
			else if (order.getInteriorLevel() == "base")
			{
				fileName = "2023 Base Expedition.jpg";
			}
			else if (order.getInteriorLevel() == "mid")
			{
				fileName = "2023 Mid Expedition.jpg";
			}
		}
	}
	return startOfPath + fileName;
}

//This converts the name of the body panel in the object to the correct format for the file name
string convertBodyPanelSetToPaintFileName(string bodyPanelSet, Order order)
{
	string convertedBodyPanelSetName;
	if (order.getYear() == "2022")
	{
		if (bodyPanelSet == "Regular")
		{
			convertedBodyPanelSetName = "Regular";
		}
		else if (bodyPanelSet == "SuperCrew")
		{
			convertedBodyPanelSetName = "SuperCrew";
		}
		else if (bodyPanelSet == "SuperCab")
		{
			convertedBodyPanelSetName = "SuperCab";
		}
	}
	else if (order.getYear() == "2023")
	{
		if (bodyPanelSet == "REG")
		{
			convertedBodyPanelSetName = "Regular";
		}
		else if (bodyPanelSet == "MAX")
		{
			convertedBodyPanelSetName = "Max";
		}
	}
	return convertedBodyPanelSetName;
}

//This loads the correct image for the paint machine using the order
string loadPaintMachineImage(Order order)
{
	string startOfPath = "Images/GUI/Paint Machine/";

	return startOfPath + order.getYear() + " " + convertBodyPanelSetToPaintFileName(order.getBodyPanelSet(), order) + " " + order.getModel() + " " + order.getColour() + ".jpg";
}

//This loads the correct image for displaying the order using the order
string loadOrderImage(Order order)
{
	string startOfPath = "Images/GUI/Orders/";

	return startOfPath + order.getYear() + " " + order.getModel() + " " + order.getTrim() + convertBodyPanelSetToFileName(order.getBodyPanelSet(), order) + order.getColour() + ".jpg";
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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "ImGui + GLFW", NULL, NULL);
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
	glViewport(0, 0, 1920, 1080);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Variables to be changed in the ImGUI window

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
	int DryChamberHMD = 0;
	double globalTemp = 0;
	int GlobalAir = 0;
	static int b;
	static int c;
	int VeComp = 0;
	int i = 0;
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();


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

	bool isRendered = true;
	bool isRenderedBay2 = true;
	readLog(&plant);
	int VeQuota = plant.getVehicleQuota();
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		bool changePaintValues = true;
		bool changeBodyValues = true;
		bool changeChassisValues = true;
		bool changeInteriorValues = true;

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

		VeComp = VeComp++;

		while (bodyMachine) {



			glfwPollEvents();

			//changing values from order to body machine.
			if (glfwWindowShouldClose(window)) {
				bodyMachine = false;
				paintMachine = false;
				chassisMachine = false;
				interiorMachine = false;
			}

				if (changeBodyValues) {

					plant.bodyMachine.RunBodyMachine(plant.order, &plant.vehicle);
					plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
					changeBodyValues = false;
				}

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
				if (isRendered) {
					bool body = LoadTextureFromFile(loadBodyMachineImage(plant.order).c_str(), &body_image_texture, &body_image_width, &body_image_height);
					IM_ASSERT(body);

					bool toBeMade = LoadTextureFromFile(loadOrderImage(plant.order).c_str(), &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
					cout << loadOrderImage(plant.order) << endl;
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
				}

				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (plant.bodyMachine.bayOne.bayThisLineInUse()) {
					if (plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount() == 49 ||
						plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount() == 49 ||
						plant.bodyMachine.bayOne.GetRegularF150InventoryAmount() == 49 ||
						plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount() == 49 ||
						plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount() == 49) {

						check = false;
						if (isRendered) {
							ImGui::OpenPopup("Warning Bay 1");
						}

					}
					else if (plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount() < 1 ||
						plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount() < 1 ||
						plant.bodyMachine.bayOne.GetRegularF150InventoryAmount() < 1 ||
						plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount() < 1 ||
						plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount() < 1) {

						check = false;

						ImGui::OpenPopup("Critical Warning Bay 1");
					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Warning Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Regular Expedition = %d", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
						ImGui::Text("Max Expedition = %d", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
						ImGui::Text("Regular Cab = %d", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
						ImGui::Text("Super Cab = %d", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
						ImGui::Text("SuperCrew = %d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());


						ImGui::SetItemDefaultFocus();
						
						if (ImGui::Button("Dismiss", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
						ImGui::PopStyleColor();
					//Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Critical Warning Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Regular Expedition = %d", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
						ImGui::Text("Max Expedition = %d", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
						ImGui::Text("Regular Cab = %d", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
						ImGui::Text("Super Cab = %d", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
						ImGui::Text("SuperCrew = %d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());


						ImGui::SetItemDefaultFocus();
						
						if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
							e = 1;
							plant.bodyMachine.SwitchVehiclePanelsBays("BayTwo");
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(500, "BayOne");


							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(500, "BayOne");


							plant.bodyMachine.UpdateRegularF150InventoryAmount(500, "BayOne");


							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(500, "BayOne");


							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(500, "BayOne");

						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

				}
				else if (plant.bodyMachine.bayTwo.bayThisLineInUse()) {
						
						if (plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount() == 49 ||
							plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount() == 49 ||
							plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount() == 49 ||
							plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount() == 49 ||
							plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount() == 49) {

							check = false;
							if (isRenderedBay2) {
								ImGui::OpenPopup("Warning Bay 2");
								isRenderedBay2 = false;
							}
						}else if (plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount() < 1 ||
							plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount() < 1 ||
							plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount() < 1 ||
							plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount() < 1 ||
							plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount() < 1) {

							check = false;

							ImGui::OpenPopup("Critical Warning Bay 2");
							}

							// Always center this window when appearing
							center = ImGui::GetMainViewport()->GetCenter();
							ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
							if (ImGui::BeginPopupModal("Warning Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::Text("Regular Expedition = %d", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
								ImGui::Text("Max Expedition = %d", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
								ImGui::Text("Regular Cab = %d", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
								ImGui::Text("Super Cab = %d", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
								ImGui::Text("SuperCrew = %d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());


								ImGui::SetItemDefaultFocus();
								
								if (ImGui::Button("Dismiss", ImVec2(120, 0))) {
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
							ImGui::PopStyleColor();
					
							// Always center this window when appearing
							center = ImGui::GetMainViewport()->GetCenter();
							ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
							if (ImGui::BeginPopupModal("Critical Warning Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
							{
								ImGui::Text("Regular Expedition = %d", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
								ImGui::Text("Max Expedition = %d", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
								ImGui::Text("Regular Cab = %d", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
								ImGui::Text("Super Cab = %d", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
								ImGui::Text("SuperCrew = %d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());


								ImGui::SetItemDefaultFocus();
								
								if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
									ImGui::CloseCurrentPopup();
									e = 0;
									plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
									plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(500, "BayTwo");


									plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(500, "BayTwo");


									plant.bodyMachine.UpdateRegularF150InventoryAmount(500, "BayTwo");


									plant.bodyMachine.UpdateSuperCabF150InventoryAmount(500, "BayTwo");


									plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(500, "BayTwo");

								}
								ImGui::EndPopup();
							}
							ImGui::PopStyleColor();
					}





				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



					//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Body Machine Manager", NULL, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);

				if (e == 0) {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
				}
				else {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayTwo");
				}

				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");

					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						check = false;
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayOne");
						}

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
						check = false;
						ImGui::CloseCurrentPopup();
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayTwo");
						}




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
				ImGui::Begin("Control Menu", NULL, controlMenuFlags);
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

				
				ImGui::SetNextWindowSize(ImVec2(50, 300));
				ImGui::Begin("bay1InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
			
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>



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
				if (!ImGui::Begin("Body Machine Image", NULL, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				////ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2((float)body_image_width, (float)body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================





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
				ImGui::Begin("Chassis Machine Manager", NULL, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Line Selection:");
				//radio buttons

				ImGui::RadioButton("Line 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Line 2", &b, 1);
				if (b == 0) {
					plant.chassisMachine.SwitchVehicleChassisLines("LineOne");
				}
				else {
					plant.chassisMachine.SwitchVehicleChassisLines("LineTwo");
				}

				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (b == 0) {

						ImGui::OpenPopup("Restock Line 2");
					}
					else {
						ImGui::OpenPopup("Restock Line 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineOne");
						}
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
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineTwo");
						}
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
				ImGui::Text("%d\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Chassis Machine Image", NULL, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}




				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2((float)chassis_image_width, (float)chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Interior Machine Manager", NULL, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons

				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayOne");
						}



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
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayTwo");
						}

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
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Interior Machine Image", NULL, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}




				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2((float)Interior_image_width, (float)Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE



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

				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2((float)RedPaintVat_image_width, (float)RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat



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

				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2((float)BluePaintVat_image_width, (float)BluePaintVat_image_height));
				ImGui::End();

				//Green Vat


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

				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2((float)GreenPaintVat_image_width, (float)GreenPaintVat_image_height));
				ImGui::End();

				//DipTank



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

				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);

				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2((float)DipTank_image_width, (float)DipTank_image_height));

				ImGui::End();




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

				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);

				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2((float)PaintChamber_image_width, (float)PaintChamber_image_height));

				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Paint Machine Image", NULL, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2((float)Paint_image_width, (float)Paint_image_height));


				ImGui::End();



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
				ImGui::Begin("Paint Machine Manager", NULL, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					check = false;
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
							plant.paintingMachine.resupplyRGBpaintVat("RED", 500, "RGBPaintVats.txt");

							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 1) {
							plant.paintingMachine.resupplyRGBpaintVat("GREEN", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 2) {
							plant.paintingMachine.resupplyRGBpaintVat("BLUE", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();


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
				if (!ImGui::Begin("Global Temp", NULL, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Global Air", NULL, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();





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
				if (!ImGui::Begin("Vehicle Quota", NULL, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Vehicle Completed", NULL, VeComp_flags))
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
				ShowReadLogs(NULL);
				//END OF LOG STUFF


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

				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", NULL, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				//// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				//// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				////ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				//// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				//



				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2((float)my_image_width, (float)my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>


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

				//// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				//// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				////ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				//// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
				//



				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2((float)toBeMade_image_width, (float)toBeMade_image_height));


				ImGui::End();



				ImGui::EndFrame();




				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events


				if (plant.bodyMachine.bayOne.bayThisLineInUse()) {
					isRendered = false;
				}
				
				
				
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
				isRendered = true;
			}

			while (paintMachine) {
				glfwPollEvents();



				//changing values from order to body machine.
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
					plant.vehicle.setColour(plant.paintingMachine.getcolour());
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
				if (isRendered) {
					//Set to blank
					bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
					IM_ASSERT(body);

					bool toBeMade = LoadTextureFromFile(loadOrderImage(plant.order).c_str(), &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
					IM_ASSERT(toBeMade);


					bool chassis = LoadTextureFromFile("Images/Blank.png", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
					IM_ASSERT(chassis);


					//set to blank
					bool Paint = LoadTextureFromFile(loadPaintMachineImage(plant.order).c_str(), &Paint_image_texture, &Paint_image_width, &Paint_image_height);
					cout << loadPaintMachineImage(plant.order).c_str() << endl;
					IM_ASSERT(Paint);

					//set to blank
					bool Interior = LoadTextureFromFile("Images/Blank.png", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
					IM_ASSERT(Interior);
				}

				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				
					if ((DipTankFluidLevel < 100) || (DipTankTemp < 19.0) || (DipTankTemp > 24.0) ||
						(PaintChamberTemp < 19.0) || (PaintChamberTemp > 24.0) || 
						(PaintChamberHMD < 40) || (PaintChamberHMD > 50) || 
						(DryChamberTemp < 19.0) || (DryChamberTemp > 24.0) || 
						(DryChamberHMD < 40) || (DryChamberHMD > 50) || (paintGreenVatVol <= 5) || 
						(paintBlueVatVol <= 5) || (paintRedVatVol <= 5))
					{

						check = false;
						ImGui::OpenPopup("Adjust Everything 1");
					
					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Adjust Everything 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Dip Tank Fluid Level = %d", plant.dipTank.getfluidLevel());
						ImGui::Text("Dip Tank Temperature = %f", plant.dipTank.getTemperature());
						ImGui::Text("Paint Chamber Temperature = % f", plant.paintChamber.getTemperature());
						ImGui::Text("Paint Chamber Humidity = %d", plant.paintChamber.getHumidity());
						ImGui::Text("Green Volume = %d", plant.paintingMachine.getpaintVolumeGREEN());
						ImGui::Text("Red Volume = %d", plant.paintingMachine.getpaintVolumeRED());
						ImGui::Text("Blue Volume = %d", plant.paintingMachine.getpaintVolumeBLUE());


						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Regulate Values", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); 

						
							plant.paintingMachine.resupplyRGBpaintVat("RED", 500, "RGBPaintVats.txt");

							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

					
							plant.paintingMachine.resupplyRGBpaintVat("GREEN", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

					
							plant.paintingMachine.resupplyRGBpaintVat("BLUE", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();



					
						DipTankFluidLevel = 1000;
							
							
						DipTankTemp = 20.0;
							
							
						PaintChamberTemp = 20.0;
							
							
						PaintChamberHMD = 45;
							
							
						DryChamberTemp = 20.0;
							
							
						DryChamberHMD = 45;
							
						
						
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();
				
				

			



				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/





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
				ImGui::Begin("Body Machine Manager", NULL, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);

				if (e == 0) {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
				}
				else {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayTwo");
				}
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}



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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayOne");
						}

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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayTwo");
						}




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
				ImGui::Begin("Control Menu", NULL, controlMenuFlags);
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
				ImGui::Begin("bay1InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Body Machine Image", NULL, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2((float)body_image_width, (float)body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================





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
				ImGui::Begin("Chassis Machine Manager", NULL, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Line Selection:");
				//radio buttons

				ImGui::RadioButton("Line 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Line 2", &b, 1);
				if (b == 0) {
					plant.chassisMachine.SwitchVehicleChassisLines("LineOne");
				}
				else {
					plant.chassisMachine.SwitchVehicleChassisLines("LineTwo");
				}

				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (b == 0) {
						ImGui::OpenPopup("Restock Line 2");
					}
					else {
						ImGui::OpenPopup("Restock Line 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineOne");
						}
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
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineTwo");
						}
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
				ImGui::Text("%d\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





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
				if (!ImGui::Begin("Chassis Machine Image", NULL, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2((float)chassis_image_width, (float)chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Interior Machine Manager", NULL, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons

				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayOne");
						}



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
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayTwo");
						}

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
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Interior Machine Image", NULL, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2((float)Interior_image_width, (float)Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE



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

				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2((float)RedPaintVat_image_width, (float)RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat

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

				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2((float)BluePaintVat_image_width, (float)BluePaintVat_image_height));
				ImGui::End();

				//Green Vat



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

				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2((float)GreenPaintVat_image_width, (float)GreenPaintVat_image_height));
				ImGui::End();

				//DipTank


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

				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);
				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2((float)DipTank_image_width, (float)DipTank_image_height));
				ImGui::End();


				//PaintChamber



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

				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);

				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2((float)PaintChamber_image_width, (float)PaintChamber_image_height));

				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Paint Machine Image", NULL, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2((float)Paint_image_width, (float)Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Paint Machine Manager", NULL, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					check = false;
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}



				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
							plant.paintingMachine.resupplyRGBpaintVat("RED", 500, "RGBPaintVats.txt");

							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 1) {
							plant.paintingMachine.resupplyRGBpaintVat("GREEN", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 2) {
							plant.paintingMachine.resupplyRGBpaintVat("BLUE", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();


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
				if (!ImGui::Begin("Global Temp", NULL, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Global Air", NULL, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();



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
				if (!ImGui::Begin("Vehicle Quota", NULL, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();



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
				if (!ImGui::Begin("Vehicle Completed", NULL, VeComp_flags))
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
				ShowReadLogs(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!


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
				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", NULL, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2((float)my_image_width, (float)my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>


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




				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2((float)toBeMade_image_width, (float)toBeMade_image_height));


				ImGui::End();


				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events

				if (plant.bodyMachine.bayOne.bayThisLineInUse()) {
					isRendered = false;
				}
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
				isRendered = true;
			}

			while (chassisMachine) {
				glfwPollEvents();
				if (glfwWindowShouldClose(window)) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;
				}

				if (changeChassisValues) {

					plant.chassisMachine.RunChassisMachine(plant.order, &plant.vehicle);
					plant.chassisMachine.SwitchVehicleChassisLines("LineOne");
					changeChassisValues = false;
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
				if (isRendered) {
					bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
					IM_ASSERT(body);

					bool toBeMade = LoadTextureFromFile(loadOrderImage(plant.order).c_str(), &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
					IM_ASSERT(toBeMade);


					bool chassis = LoadTextureFromFile(loadChassisMachineImage(plant.order).c_str(), &chassis_image_texture, &chassis_image_width, &chassis_image_height);
					IM_ASSERT(chassis);


					//set to blank
					bool Paint = LoadTextureFromFile("Images/Blank.png", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
					IM_ASSERT(Paint);

					//set to blank
					bool Interior = LoadTextureFromFile("Images/Blank.png", &Interior_image_texture, &Interior_image_width, &Interior_image_height);
					IM_ASSERT(Interior);
				}

				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (plant.chassisMachine.lineOne.isThisLineInUse()) {
					if (plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount() == 49 ||
						plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount() == 49 ||
						plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount() == 49
						|| plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount() == 49) {

						check = false;
						if (isRendered) {
							ImGui::OpenPopup("Warning Line 1");
						}

					}
					else if (plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount() < 1 ||
						plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount() < 1 ||
						plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount() < 1
						|| plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount() < 1) {

						check = false;

						ImGui::OpenPopup("Critical Warning Line 1");
					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Warning Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("35LV6C = %d", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
						ImGui::Text("35LV6HOC = %d", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
						ImGui::Text("27LV6C = %d", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
						ImGui::Text("33LV6C = %d", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
						ImGui::Text("35LV6EcoC = %d", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
						ImGui::Text("35LV6PwrBstC = %d", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());


						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Dismiss", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();
					//Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Critical Warning Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("35LV6C = %d", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
						ImGui::Text("35LV6HOC = %d", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
						ImGui::Text("27LV6C = %d", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
						ImGui::Text("33LV6C = %d", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
						ImGui::Text("35LV6EcoC = %d", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
						ImGui::Text("35LV6PwrBstC = %d", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());


						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
							b = 1;
							plant.chassisMachine.SwitchVehicleChassisLines("LineTwo");

							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(500, "LineOne");
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(500, "LineOne");
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

				}
				else if (plant.chassisMachine.lineTwo.isThisLineInUse()) {

					if (plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount() == 49 ||
						plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount() == 49 ||
						plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount() == 49 ||
						plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount() == 49
						|| plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount() == 49) {

						check = false;
						if (isRenderedBay2) {
							ImGui::OpenPopup("Warning Line 2");
							isRenderedBay2 = false;
						}
					}
					else if (plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount() < 1 ||
						plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount() < 1 ||
						plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount() < 1 ||
						plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount() < 1
						|| plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount() < 1) {

						check = false;

						ImGui::OpenPopup("Critical Warning Line 2");


					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Warning Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("35LV6C = %d", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
						ImGui::Text("35LV6HOC = %d", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
						ImGui::Text("27LV6C = %d", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
						ImGui::Text("33LV6C = %d", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
						ImGui::Text("35LV6EcoC = %d", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
						ImGui::Text("35LV6PwrBstC = %d", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());

						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Dismiss", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Critical Warning Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("35LV6C = %d", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
						ImGui::Text("35LV6HOC = %d", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
						ImGui::Text("27LV6C = %d", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
						ImGui::Text("33LV6C = %d", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
						ImGui::Text("35LV6EcoC = %d", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
						ImGui::Text("35LV6PwrBstC = %d", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());

						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
							b = 0;
							plant.chassisMachine.SwitchVehicleChassisLines("LineOne");

							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(500, "LineTwo");
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(500, "LineTwo");

						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();
				}





				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/





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
				ImGui::Begin("Body Machine Manager", NULL, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);

				if (e == 0) {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
				}
				else {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayTwo");
				}
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayOne");
						}

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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayTwo");
						}




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
				ImGui::Begin("Control Menu", NULL, controlMenuFlags);
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
				ImGui::Begin("bay1InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>

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
				if (!ImGui::Begin("Body Machine Image", NULL, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2((float)body_image_width, (float)body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Chassis Machine Manager", NULL, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Line Selection:");
				//radio buttons

				ImGui::RadioButton("Line 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Line 2", &b, 1);
				if (b == 0) {
					plant.chassisMachine.SwitchVehicleChassisLines("LineOne");
				}
				else {
					plant.chassisMachine.SwitchVehicleChassisLines("LineTwo");
				}

				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (b == 0) {
						ImGui::OpenPopup("Restock Line 2");
					}
					else {
						ImGui::OpenPopup("Restock Line 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineOne");
						}
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
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineTwo");
						}
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
				ImGui::Text("%d\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>

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
				if (!ImGui::Begin("Chassis Machine Image", NULL, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2((float)chassis_image_width, (float)chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Interior Machine Manager", NULL, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons

				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayOne");
						}



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
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayTwo");
						}

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
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>

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
				if (!ImGui::Begin("Interior Machine Image", NULL, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2((float)Interior_image_width, (float)Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE




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

				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2((float)RedPaintVat_image_width, (float)RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat


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

				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2((float)BluePaintVat_image_width, (float)BluePaintVat_image_height));
				ImGui::End();

				//Green Vat


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

				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2((float)GreenPaintVat_image_width, (float)GreenPaintVat_image_height));
				ImGui::End();

				//DipTank


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

				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);

				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2((float)DipTank_image_width, (float)DipTank_image_height));

				ImGui::End();


				//PaintChamber



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

				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);

				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2((float)PaintChamber_image_width, (float)PaintChamber_image_height));

				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Paint Machine Image", NULL, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2((float)Paint_image_width, (float)Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Paint Machine Manager", NULL, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					check = false;
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
							plant.paintingMachine.resupplyRGBpaintVat("RED", 500, "RGBPaintVats.txt");

							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 1) {
							plant.paintingMachine.resupplyRGBpaintVat("GREEN", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 2) {
							plant.paintingMachine.resupplyRGBpaintVat("BLUE", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();


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
				if (!ImGui::Begin("Global Temp", NULL, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Global Air", NULL, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();





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
				if (!ImGui::Begin("Vehicle Quota", NULL, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Vehicle Completed", NULL, VeComp_flags))
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
				ShowReadLogs(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!


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

				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", NULL, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2((float)my_image_width, (float)my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>


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




				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2((float)toBeMade_image_width, (float)toBeMade_image_height));


				ImGui::End();

				// Renders the ImGUI elements
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Swap the back buffer with the front buffer
				glfwSwapBuffers(window);
				// Take care of all GLFW events

				isRendered = false;
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
				isRendered = true;

			}

			while (interiorMachine) {

				glfwPollEvents();
				if (glfwWindowShouldClose(window)) {
					bodyMachine = false;
					paintMachine = false;
					chassisMachine = false;
					interiorMachine = false;
				}

				if (changeInteriorValues) {
					plant.interiorMachine.RunInteriorMachine(plant.order, &plant.vehicle);
					plant.interiorMachine.SwitchVehicleInteriorsBays("BayOne");
					changeInteriorValues = false;
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
				if (isRendered) {
					//Set to blank
					bool body = LoadTextureFromFile("Images/Blank.png", &body_image_texture, &body_image_width, &body_image_height);
					IM_ASSERT(body);

					bool toBeMade = LoadTextureFromFile(loadOrderImage(plant.order).c_str(), &toBeMade_image_texture, &toBeMade_image_width, &toBeMade_image_height);
					IM_ASSERT(toBeMade);


					bool chassis = LoadTextureFromFile("Images/Blank.png", &chassis_image_texture, &chassis_image_width, &chassis_image_height);
					IM_ASSERT(chassis);


					//set to blank
					bool Paint = LoadTextureFromFile("Images/Blank.png", &Paint_image_texture, &Paint_image_width, &Paint_image_height);
					IM_ASSERT(Paint);

					//set to blank
					bool Interior = LoadTextureFromFile(loadInteriorMachineImage(plant.order).c_str(), &Interior_image_texture, &Interior_image_width, &Interior_image_height);
					IM_ASSERT(Interior);

				}
				// Specify the color of the background
				glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
				// Clean the back buffer and assign the new color to it
				glClear(GL_COLOR_BUFFER_BIT);

				// Tell OpenGL a new frame is about to begin
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();
				
				if (plant.interiorMachine.bayOne.bayThisLineInUse()) {
					if (plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount() == 49){

						check = false;
						if (isRendered) {
							ImGui::OpenPopup("Warning Bay 1");
						}

					}
					else if (plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount() < 1) {

						check = false;

						ImGui::OpenPopup("Critical Warning Bay 1");
					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Warning Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Base Expedition = %d", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
						ImGui::Text("Mid Expedition = %d", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
						ImGui::Text("High Expedition = %d", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
						ImGui::Text("Base F150 = %d", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
						ImGui::Text("Mid F150 = %d", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
						ImGui::Text("High F150 = %d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());


						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Dismiss", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();
					//Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Critical Warning Bay 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Base Expedition = %d", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
						ImGui::Text("Mid Expedition = %d", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
						ImGui::Text("High Expedition = %d", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
						ImGui::Text("Base F150 = %d", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
						ImGui::Text("Mid F150 = %d", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
						ImGui::Text("High F150 = %d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());


						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
							c = 1;
							plant.interiorMachine.SwitchVehicleInteriorsBays("BayTwo");
							
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(500, "BayOne");
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(500, "BayOne");
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(500, "BayOne");
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(500, "BayOne");
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(500, "BayOne");
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(500, "BayOne");
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

				}
				else if (plant.interiorMachine.bayTwo.bayThisLineInUse()) {

					if (plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount() == 49 ||
						plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount() == 49) {

						check = false;
						if (isRenderedBay2) {
							ImGui::OpenPopup("Warning Bay 2");
							isRenderedBay2 = false;
						}
					}
					else if (plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount() < 1 ||
						plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount() < 1) {
						check = false;

						ImGui::OpenPopup("Critical Warning Bay 2");
					
					
					}

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Warning Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Base Expedition = %d", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
						ImGui::Text("Mid Expedition = %d", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
						ImGui::Text("High Expedition = %d", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
						ImGui::Text("Base F150 = %d", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
						ImGui::Text("Mid F150 = %d", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
						ImGui::Text("High F150 = %d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());

						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Dismiss", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();

					// Always center this window when appearing
					center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					if (ImGui::BeginPopupModal("Critical Warning Bay 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Base Expedition = %d", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
						ImGui::Text("Mid Expedition = %d", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
						ImGui::Text("High Expedition = %d", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
						ImGui::Text("Base F150 = %d", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
						ImGui::Text("Mid F150 = %d", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
						ImGui::Text("High F150 = %d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());

						ImGui::SetItemDefaultFocus();

						if (ImGui::Button("Switch and Restock", ImVec2(120, 0))) {
							ImGui::CloseCurrentPopup();
							c = 0;
							plant.interiorMachine.SwitchVehicleInteriorsBays("BayOne");

							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(500, "BayTwo");
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(500, "BayTwo");
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(500, "BayTwo");
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(500, "BayTwo");
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(500, "BayTwo");
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(500, "BayTwo");
						}
						ImGui::EndPopup();
					}
					ImGui::PopStyleColor();
				}






				//BODY MACHINE================================================================================================================================================
					/*
					* CHECK TO SEE THE VALUES ARE IN THE BAY Currently in use
					Inventory Values(received from the body machine bays) INTS
					Button to restock(go to pop up window)
					bay selectors
					Images boxes to show the body being worked on
					*/



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
				ImGui::Begin("Body Machine Manager", NULL, bodyButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons
				ImGui::RadioButton("Bay 1", &e, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &e, 1);

				if (e == 0) {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayOne");
				}
				else {
					plant.bodyMachine.SwitchVehiclePanelsBays("BayTwo");
				}
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (e == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayOne");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayOne");
						}

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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (bodyitem_current == 0) {
							plant.bodyMachine.UpdateMaxExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 1) {
							plant.bodyMachine.UpdateRegularExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 2) {
							plant.bodyMachine.UpdateRegularF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 3) {
							plant.bodyMachine.UpdateSuperCabF150InventoryAmount(i0, "BayTwo");
						}
						else if (bodyitem_current == 4) {
							plant.bodyMachine.UpdateSuperCrewF150InventoryAmount(i0, "BayTwo");
						}




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
				ImGui::Begin("Control Menu", NULL, controlMenuFlags);
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
				ImGui::Begin("bay1InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayOne.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayOne.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayOne.GetSuperCrewF150InventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();



				ImGui::SetNextWindowSize(ImVec2(50, 300));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InventoryValues", NULL, inventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetMaxExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularExpeditionInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetRegularF150InventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.bodyMachine.bayTwo.GetSuperCabF150InventoryAmount());
				ImGui::Text("%d", plant.bodyMachine.bayTwo.GetSuperCrewF150InventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// BODY IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>

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
				if (!ImGui::Begin("Body Machine Image", NULL, bodyImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)body_image_texture, ImVec2((float)body_image_width, (float)body_image_height));


				ImGui::End();


				//END OF BODY

			//	//CHASSIS MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Chassis Machine Manager", NULL, ChassisButtonflags);
				// Text that appears in the window
				ImGui::Text("Line Selection:");
				//radio buttons

				ImGui::RadioButton("Line 1", &b, 0); ImGui::SameLine();
				ImGui::RadioButton("Line 2", &b, 1);
				if (b == 0) {
					plant.chassisMachine.SwitchVehicleChassisLines("LineOne");
				}
				else {
					plant.chassisMachine.SwitchVehicleChassisLines("LineTwo");
				}

				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (b == 0) {
						ImGui::OpenPopup("Restock Line 2");
					}
					else {
						ImGui::OpenPopup("Restock Line 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 1", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineOne");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineOne");
						}
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
				center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopupModal("Restock Line 2", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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

						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}
						if (chassisitem_current == 0) {
							plant.chassisMachine.UpdateExpedition35LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 1) {
							plant.chassisMachine.UpdateExpedition35LV6HOCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 2) {
							plant.chassisMachine.UpdateF15027LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 3) {
							plant.chassisMachine.UpdateF15033LV6CInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 4) {
							plant.chassisMachine.UpdateF15035LV6EcoCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 5) {
							plant.chassisMachine.UpdateF15035LV6PwrBstCInventoryAmount(i0, "LineTwo");
						}
						else if (chassisitem_current == 6) {
							plant.chassisMachine.UpdateF15050LV8CInventoryAmount(i0, "LineTwo");
						}
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
				ImGui::Text("%d\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineOne.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineOne.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineOne.GetF15050LV8CInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2ChassisInventoryValues", NULL, ChassisinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetExpedition35LV6HOCInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15027LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.chassisMachine.lineTwo.GetF15033LV6CInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6EcoCInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.chassisMachine.lineTwo.GetF15035LV6PwrBstCInventoryAmount());
				ImGui::Text("%d", plant.chassisMachine.lineTwo.GetF15050LV8CInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();



				/// <summary>
				/// Chassis IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Chassis Machine Image", NULL, ChassisImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)chassis_image_texture, ImVec2((float)chassis_image_width, (float)chassis_image_height));


				ImGui::End();

				//END OF CHASSIS

						//	//Interior MACHINE======================================================================



				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Interior Machine Manager", NULL, InteriorButtonflags);
				// Text that appears in the window
				ImGui::Text("Bay Selection:");
				//radio buttons

				ImGui::RadioButton("Bay 1", &c, 0); ImGui::SameLine();
				ImGui::RadioButton("Bay 2", &c, 1);
				if (c == 0) {
					plant.interiorMachine.SwitchVehicleInteriorsBays("BayOne");
				}
				else {
					plant.interiorMachine.SwitchVehicleInteriorsBays("BayTwo");
				}
				ImGui::Separator();
				if (ImGui::Button("Restock")) {
					check = false;
					if (c == 0) {
						ImGui::OpenPopup("Restock Bay 2");
					}
					else {
						ImGui::OpenPopup("Restock Bay 1");
					}
				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayOne");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayOne");
						}



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
					const char* items[] = { "BaseExp", "MidExp", "HighExp", "BaseF150", "MidF150", "HighF150" };

					ImGui::Combo("Interior Type", &Interioritem_current, items, IM_ARRAYSIZE(items));

					static int i0 = 123;
					ImGui::InputInt("input int", &i0);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

					ImGui::PopStyleVar();

					if (ImGui::Button("OK", ImVec2(120, 0))) {
						ImGui::CloseCurrentPopup();
						if (i0 > 500) {
							i0 = 500;
						}
						else if (i0 < 0) {
							i0 = 0;
						}

						if (Interioritem_current == 0) {
							plant.interiorMachine.UpdateBaseInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 1) {
							plant.interiorMachine.UpdateMidInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 2) {
							plant.interiorMachine.UpdateHighInteriorExpeditionInventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 3) {
							plant.interiorMachine.UpdateBaseInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 4) {
							plant.interiorMachine.UpdateMidInteriorF150InventoryAmount(i0, "BayTwo");
						}
						else if (Interioritem_current == 5) {
							plant.interiorMachine.UpdateHighInteriorF150InventoryAmount(i0, "BayTwo");
						}

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
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayOne.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayOne.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayOne.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayOne.GetHighF150InteriorInventoryAmount());
				/*	ImGui::PopStyleColor();*/
				ImGui::End();


				ImGui::SetNextWindowSize(ImVec2(50, 365));
				//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.788f, 0.788f, 0.788f, 1.0f)); // Set window background to grey
				ImGui::Begin("bay2InteriorInventoryValues", NULL, InteriorinventoryFlags);
				/*ImGui::SetWindowFocus();*/
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetMidExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n", plant.interiorMachine.bayTwo.GetHighExpeditionInteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n", plant.interiorMachine.bayTwo.GetBaseF150InteriorInventoryAmount());
				ImGui::Text("%d\n\n\n\n\n", plant.interiorMachine.bayTwo.GetMidF150InteriorInventoryAmount());
				ImGui::Text("%d", plant.interiorMachine.bayTwo.GetHighF150InteriorInventoryAmount());
				/*ImGui::PopStyleColor();*/
				ImGui::End();





				/// <summary>
				/// Interior IMAGE STRUCTURE
				/// </summary>

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
				if (!ImGui::Begin("Interior Machine Image", NULL, InteriorImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Interior_image_texture, ImVec2((float)Interior_image_width, (float)Interior_image_height));


				ImGui::End();

				////END OF Interior


				//START OF PAINT MACHINE



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

				ImGui::Text("Volume: %d L", paintRedVatVol);
				ImGui::Image((void*)(intptr_t)RedPaintVat_image_texture, ImVec2((float)RedPaintVat_image_width, (float)RedPaintVat_image_height));
				ImGui::End();

				//Blue Vat



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

				ImGui::Text("Volume: %d L", paintBlueVatVol);
				ImGui::Image((void*)(intptr_t)BluePaintVat_image_texture, ImVec2((float)BluePaintVat_image_width, (float)BluePaintVat_image_height));
				ImGui::End();

				//Green Vat


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

				ImGui::Text("Volume: %d L", paintGreenVatVol);
				ImGui::Image((void*)(intptr_t)GreenPaintVat_image_texture, ImVec2((float)GreenPaintVat_image_width, (float)GreenPaintVat_image_height));
				ImGui::End();

				//DipTank



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

				ImGui::Text("Fluid Level: %d L", DipTankFluidLevel);
				ImGui::Text("Temperature: %.2f`C", DipTankTemp);

				ImGui::Image((void*)(intptr_t)DipTank_image_texture, ImVec2((float)DipTank_image_width, (float)DipTank_image_height));

				ImGui::End();


				//PaintChamber


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

				ImGui::Text("Paint Humidity: %d", PaintChamberHMD);
				ImGui::Text("Paint Temperature: %.2f`C", PaintChamberTemp);
				ImGui::Text("Dry Humidity: %d", DryChamberHMD);
				ImGui::Text("Dry Temperature: %.2f`C", DryChamberTemp);

				ImGui::Image((void*)(intptr_t)PaintChamber_image_texture, ImVec2((float)PaintChamber_image_width, (float)PaintChamber_image_height));

				ImGui::End();


				/// <summary>
				/// Paint IMAGE STRUCTURE
				/// </summary>
				/// <returns></returns>


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
				if (!ImGui::Begin("Paint Machine Image", NULL, PaintImage_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)Paint_image_texture, ImVec2((float)Paint_image_width, (float)Paint_image_height));


				ImGui::End();

				//BAY SELECTION TOGGLE and restock button


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
				ImGui::Begin("Paint Machine Manager", NULL, PaintButtonflags);
				//radio buttons
				if (ImGui::Button("Adjust Values")) {
					check = false;
					ImGui::OpenPopup("Paint Machine Values Adjuster");

				}


				// Always center this window when appearing
				center = ImGui::GetMainViewport()->GetCenter();
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
							plant.paintingMachine.resupplyRGBpaintVat("RED", 500, "RGBPaintVats.txt");

							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 1) {
							plant.paintingMachine.resupplyRGBpaintVat("GREEN", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();

						}
						else if (PaintVatitem_current == 2) {
							plant.paintingMachine.resupplyRGBpaintVat("BLUE", 500, "RGBPaintVats.txt");
							plant.paintingMachine.readRGBpaintVat("RGBPaintVats.txt");

							paintRedVatVol = plant.paintingMachine.getpaintVolumeRED();
							paintGreenVatVol = plant.paintingMachine.getpaintVolumeGREEN();
							paintBlueVatVol = plant.paintingMachine.getpaintVolumeBLUE();


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
				if (!ImGui::Begin("Global Temp", NULL, GlobalTemp_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", globalTemp);
				ImGui::PopStyleColor();
				ImGui::End();




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
				if (!ImGui::Begin("Global Air", NULL, GlobalAir_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", GlobalAir);
				ImGui::PopStyleColor();
				ImGui::End();





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
				if (!ImGui::Begin("Vehicle Quota", NULL, VeQuota_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				ImGui::SetWindowFontScale(4);
				ImGui::Text("%d", VeQuota);
				ImGui::PopStyleColor();
				ImGui::End();





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
				if (!ImGui::Begin("Vehicle Completed", NULL, VeComp_flags))
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
				ShowReadLogs(NULL);
				//END OF LOG STUFF
				// Demonstrate the various window flags. Typically you would just use the default!

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

				ImGui::SetNextWindowSize(ImVec2(1280, 1024), ImGuiCond_FirstUseEver);

				// Main body of the Demo window starts here.
				if (!ImGui::Begin("Ford Assembly Plant", NULL, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return 0;
				}

				// Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
				// e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
				//ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
				// e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.




				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2((float)my_image_width, (float)my_image_height));


				ImGui::End();


				/// <summary>
				/// To be made image STRUCTURE
				/// </summary>
				/// <returns></returns>


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




				ImGui::Image((void*)(intptr_t)toBeMade_image_texture, ImVec2((float)toBeMade_image_width, (float)toBeMade_image_height));


				ImGui::End();


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
				

				if (plant.bodyMachine.bayOne.bayThisLineInUse()) {
					isRendered = false;
				}


			}
			isRendered = true;
			isRenderedBay2 = true;


			if (plant.vehicle.checkQAQC()) {

				plant.vehicle.setMake(plant.order.getMake());
				plant.vehicle.setModel(plant.order.getModel());
				plant.vehicle.setYear(plant.order.getYear());
				plant.vehicle.setTrim(plant.order.getTrim());
				plant.vehicle.setOrder(plant.order);

				plant.vehicle.generateCount("completedVehicles.txt");

				time_t now = time(0);
				string dateAndTime = ctime(&now);
				dateAndTime.erase(std::remove(dateAndTime.begin(), dateAndTime.end(), '\n'), dateAndTime.cend());
				plant.vehicle.setVIN(plant.vehicle.GenerateVIN());
				plant.vehicle.setDate(dateAndTime);

				plant.vehicle.LogCompletedVehicle("completedVehicles.txt");

				/*plant.setNumVehicleToday(VeComp++);*/
			}



			updateLog(&plant);
			cout << "plant log  and of loop" << endl;
		}


		// Deletes all ImGUI instances
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();


		// Delete window before ending the program
		glfwDestroyWindow(window);

		// Terminate GLFW before ending the program
		glfwTerminate();

		return 0;
}