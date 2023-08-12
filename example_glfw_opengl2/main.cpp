// Dear ImGui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>
#include <fstream>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

bool AimbotIsOn = true;

const char* types[] = { "Normal" };
static int types_item_current_idx = 0;

const char* filter[] = { "Visible Check" };
static int filter_item_current_idx = 0;

const char* priority[] = { "Distance" };
static int priority_item_current_idx = 0;

bool AimRequired = true;

static float min_amin_amount = 0.5f;

bool stickToTarget = true;
bool ignoreDowned = true;
bool FOVcheck = true;

int FOVradius = 320;
int smoothAmount = 70;
int smoothAcceleration = 90;

const char* target_bone[] = { "Helmet" };
static int target_bone_item_current_idx = 0;

bool bodyScan = true;
bool boneBodyScanFirst = false;

bool autoShoot = false;
bool triggerBot = false;

bool box = true;

const char* box_type[] = { "2D" };
static int box_type_item_current_idx = 0;

const char* team[] = { "All" };
static int team_item_current_idx = 0;

bool name = true;
bool distance = true;
bool skeleton = true;
bool weaponName = true;
bool snapline = true;
bool healthBar = true;

bool drawItems = true;
bool drawExplosives = true;
bool drawVehicles = true;
bool Box = true;
bool Name = true;
bool Distance = true;

bool drawLoot = true;
bool Box_ = true;
bool Name_ = true;

bool playerAndAI = true;
bool entities = true;
bool outlineOnly = false;
int outlineWidth = 2;

const char* Type[] = { "x" };
static int Type_item_current_idx = 0;

int Size = 10;

bool showOnAimbotTarget = true;

bool compass = true;

int compass_size = 200;

bool HealthBar = true;
bool ServerInfo = true;
bool BulletTracers = true;
bool ColouredGameUI = true;

bool noRecoil = true;
bool noShellShock = true;
bool noFog = true;

bool fastReload = true;
bool UAV = true;
bool nightVision = true;

int FOV = 110;

bool infinite_slide = true;
bool air_stuck = false;

bool tempUnlockAll = false;

int PlayerAndAI_ = 400;
int Vehicles_ = 800;
int Explosives_ = 400;
int Loot = 20;
int Items = 100;

static ImVec4 EnemyPlayer =           ImVec4(255.0f / 255.0f, 64.0f  / 255.0f, 0.f   / 255.0f, 200.0f / 255.0f);
static ImVec4 VisibleEnemyPlayer =    ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 0.f   / 255.0f, 200.0f / 255.0f);
static ImVec4 FriendlyPlayer =        ImVec4(0.f    / 255.0f, 64.0f  / 255.0f, 255.f / 255.0f, 200.0f / 255.0f);
static ImVec4 VisibleFriendlyPlayer = ImVec4(0.f    / 255.0f, 255.0f / 255.0f, 0.f   / 255.0f, 200.0f / 255.0f);
static ImVec4 EnemyAI =               ImVec4(128.0f / 255.0f, 0.0f   / 255.0f, 128.f / 255.0f, 200.0f / 255.0f);
static ImVec4 VisibleEnemyAI =        ImVec4(255.f  / 255.0f, 191.f  / 255.0f, 204.f / 255.0f, 200.0f / 255.0f);
static ImVec4 FreindlyAI =            ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 204.f / 255.0f, 200.0f / 255.0f);
static ImVec4 VisibleFriendlyAI =     ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 153.f / 255.0f, 200.0f / 255.0f);
static ImVec4 AimbotTarget =          ImVec4(0.0f   / 255.0f, 255.0f / 255.0f, 255.f / 255.0f, 200.0f / 255.0f);
static ImVec4 EnemyEntity =           ImVec4(255.0f / 255.0f, 0.0f   / 255.0f, 0.f   / 255.0f, 200.0f / 255.0f);
static ImVec4 FriendlyEntity =        ImVec4(0.0f   / 255.0f, 255.0f / 255.0f, 0.f   / 255.f, 200.0f  / 255.0f);
static ImVec4 LootColour =            ImVec4(255.0f / 255.0f, 255.f  / 255.0f, 255.f / 200.0f, 200.0f / 255.0f);
static ImVec4 SpecialLoot =           ImVec4(255.0f / 255.0f, 191.0f / 255.0f, 26.f  / 200.0f, 200.0f / 255.0f);

auto save_to_file() -> void
{
    std::ofstream file("aimbot_menu.txt");
    file << "aimbot:               " << AimbotIsOn << "\n";
    file << "types:                " << types_item_current_idx << "\n";
    file << "filter:               " << filter_item_current_idx << "\n";
    file << "priority:             " << priority_item_current_idx << "\n";
    file << "aim required:         " << AimRequired << "\n";
    file << "min amin amount:      " << min_amin_amount << "\n";
    file << "stick to target:      " << stickToTarget << "\n";
    file << "ignore downed:        " << ignoreDowned << "\n";
    file << "FOV check:            " << FOVcheck << "\n";
    file << "FOV radius:           " << FOVradius << "\n";
    file << "smooth amount:        " << smoothAmount << "\n";
    file << "smooth acceleration:  " << smoothAcceleration << "\n";
    file << "target_bone:          " << target_bone_item_current_idx << "\n";
    file << "body scan:            " << bodyScan << "\n";
    file << "bone body scan first: " << boneBodyScanFirst << "\n";
    file << "auto shoot:           " << autoShoot << "\n";
    file << "triggerbot:           " << triggerBot << "\n";
    file << "box:                  " << box << "\n";
    file << "box type:             " << box_type_item_current_idx << "\n";
    file << "team:                 " << team_item_current_idx << "\n";
    file << "name:                 " << name << "\n";
    file << "distance:             " << distance << "\n";
    file << "skeleton:             " << skeleton << "\n";
    file << "weapon name:          " << weaponName << "\n";
    file << "snapline:             " << snapline << "\n";
    file << "health bar:           " << healthBar << "\n";
    file << "draw items:           " << drawItems << "\n";
    file << "draw explosives:      " << drawExplosives << "\n";
    file << "draw vehicles:        " << drawVehicles << "\n";
    file << "Box:                  " << Box << "\n";
    file << "Name:                 " << Name << "\n";
    file << "Distance:             " << Distance << "\n";
    file << "draw loot:            " << drawLoot << "\n";
    file << "Box_:                 " << Box_ << "\n";
    file << "Name_:                " << Name_ << "\n";
    file << "Player and AI:        " << playerAndAI << "\n";
    file << "entities:             " << entities << "\n";
    file << "outline only:         " << outlineOnly << "\n";
    file << "outline width:        " << outlineWidth << "\n";
    file << "type:                 " << Type_item_current_idx << "\n";
    file << "Size:                 " << Size << "\n";
    file << "Show on aimbot target:" << showOnAimbotTarget << "\n";
    file << "compass:              " << compass << "\n";
    file << "compass size:         " << compass_size << "\n";
    file << "Health Bar:           " << HealthBar << "\n";
    file << "Server Info:          " << ServerInfo << "\n";
    file << "Bullet Tracers:       " << BulletTracers << "\n";
    file << "Coloured Game UI:     " << ColouredGameUI << "\n";
    file << "No Recoil:            " << noRecoil << "\n";
    file << "No Shell Shock:       " << noShellShock << "\n";
    file << "No Fog:               " << noFog << "\n";
    file << "Fast reload:          " << fastReload << "\n";
    file << "UAV:                  " << UAV << "\n";
    file << "Night vision:         " << nightVision << "\n";
    file << "FOV:                  " << FOV << "\n";
    file << "Infinite slide:       " << infinite_slide << "\n";
    file << "Air Stuck:            " << air_stuck << "\n";
    file << "Temp Unlock All:      " << tempUnlockAll << "\n";
    file << "PlayerAndAI:          " << PlayerAndAI_ << "\n";
    file << "Vehicles:             " << Vehicles_ << "\n";
    file << "Explosives:           " << Explosives_ << "\n";
    file << "Loot:                 " << Loot << "\n";
    file << "Items:                " << Items << "\n";
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL2 example", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Our state
    int num_screenshots_cleared = 0;
    bool showAimbotMenu = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // This is where we begin the Aimbot menu.
        {
            ImGui::Begin("PUNCHWARE MW2 CHAIR", &showAimbotMenu);

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Aimbot"))
                {
                    ImGui::SeparatorText("Aimbot Settings");

                    ImGui::Checkbox("Aimbot", &AimbotIsOn);

                    // the dropdown box for Type
                    const char* type_combo_preview_value = types[types_item_current_idx];
                    if (ImGui::BeginCombo("Type", type_combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(types); n++)
                        {
                            const bool is_selected = (types_item_current_idx == n);
                            if (ImGui::Selectable(types[n], is_selected))
                                types_item_current_idx = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    const char* filter_combo_preview_value = filter[filter_item_current_idx];
                    if (ImGui::BeginCombo("Filter", filter_combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(filter); n++)
                        {
                            const bool is_selected = (filter_item_current_idx == n);
                            if (ImGui::Selectable(filter[n], is_selected))
                                filter_item_current_idx = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    const char* priority_combo_preview_value = priority[priority_item_current_idx];
                    if (ImGui::BeginCombo("Priority", priority_combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(priority); n++)
                        {
                            const bool is_selected = (priority_item_current_idx == n);
                            if (ImGui::Selectable(priority[n], is_selected))
                                priority_item_current_idx = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Checkbox("Aim Required", &AimRequired);

                    ImGui::SliderFloat("Min Amin Amount", &min_amin_amount, 0.0f, 1.0f, "%.3f");

                    ImGui::Checkbox("Stick To Target", &stickToTarget);
                    ImGui::Checkbox("Ignore Downed", &ignoreDowned);
                    ImGui::Checkbox("FOV Check", &FOVcheck);

                    ImGui::DragInt("FOV radius", &FOVradius, 0.5f, 0, 360, "%d");
                    ImGui::DragInt("Smooth Amount", &smoothAmount, 0.5f, 0, 100, "%d");
                    ImGui::DragInt("Smooth Acceleration", &smoothAcceleration, 0.5f, 0, 100, "%d");

                    ImGui::SeparatorText("Bone Settings");

                    const char* target_bone_combo_preview_value = target_bone[target_bone_item_current_idx];
                    if (ImGui::BeginCombo("Target Bone", target_bone_combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(target_bone); n++)
                        {
                            const bool is_selected = (target_bone_item_current_idx == n);
                            if (ImGui::Selectable(target_bone[n], is_selected))
                                target_bone_item_current_idx = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Checkbox("Body scan", &bodyScan);
                    ImGui::Checkbox("Bone body scan first", &boneBodyScanFirst);

                    ImGui::SeparatorText("Automations");

                    ImGui::Checkbox("auto shoot", &autoShoot);
                    ImGui::Checkbox("triggerbot", &triggerBot);

                    ImGui::Separator();

                    ImGui::Text("Screenshots Cleared: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("ESP"))
                {
                    {
                        ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
                        ImGui::BeginChild("Player & AI ESP Settings", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);

                        ImGui::Text("Players & AI ESP Settings");

                        ImGui::Checkbox("Box", &box);

                        const char* box_type_combo_preview_value = box_type[box_type_item_current_idx];
                        if (ImGui::BeginCombo("Box Type", box_type_combo_preview_value))
                        {
                            for (int n = 0; n < IM_ARRAYSIZE(box_type); n++)
                            {
                                const bool is_selected = (box_type_item_current_idx == n);
                                if (ImGui::Selectable(box_type[n], is_selected))
                                    box_type_item_current_idx = n;

                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }

                        const char* team_combo_preview_value = team[team_item_current_idx];
                        if (ImGui::BeginCombo("team", team_combo_preview_value))
                        {
                            for (int n = 0; n < IM_ARRAYSIZE(team); n++)
                            {
                                const bool is_selected = (team_item_current_idx == n);
                                if (ImGui::Selectable(team[n], is_selected))
                                    team_item_current_idx = n;

                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }

                        ImGui::Checkbox("Name", &name);
                        ImGui::Checkbox("Distance", &distance);
                        ImGui::Checkbox("Skeleton", &skeleton);
                        ImGui::Checkbox("Weapon name", &weaponName);
                        ImGui::Checkbox("Snapline", &snapline);
                        ImGui::Checkbox("Health bar", &healthBar);

                        ImGui::EndChild();
                    }

                    {
                        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
                        ImGui::BeginChild("Entities ESP settings", ImVec2(0, 260), true, window_flags);

                        ImGui::Text("Entities ESP settings");
                        ImGui::Checkbox("Draw Items", &drawItems);
                        ImGui::Checkbox("Draw Explosives", &drawExplosives);
                        ImGui::Checkbox("Draw Vehicles", &drawVehicles);
                        ImGui::Checkbox("Box", &Box);
                        ImGui::Checkbox("Name", &Name);
                        ImGui::Checkbox("distance", &Distance);

                        ImGui::EndChild();
                    }

                    ImGui::SeparatorText("Loot ESP Settings");

                    ImGui::Checkbox("Draw Loot", &drawLoot);
                    ImGui::Checkbox("Box", &Box_);
                    ImGui::Checkbox("Name", &Name_);

                    ImGui::Text("Screenshots cleared: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Chams"))
                {
                    ImGui::SeparatorText("Chams");
                    ImGui::Checkbox("Player & AI", &playerAndAI);
                    ImGui::Checkbox("Entities", &entities);
                    ImGui::Checkbox("Outline only", &outlineOnly);
                    ImGui::SliderInt("Outline width", &outlineWidth, 0, 10, "%d");

                    ImGui::Text("Screenshots cleaned: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Visuals"))
                {
                    ImGui::SeparatorText("Crosshair settings");

                    const char* Type_combo_preview_value = Type[Type_item_current_idx];
                    if (ImGui::BeginCombo("Type", Type_combo_preview_value))
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(Type); n++)
                        {
                            const bool is_selected = (Type_item_current_idx == n);
                            if (ImGui::Selectable(Type[n], is_selected))
                                Type_item_current_idx = n;

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    ImGui::DragInt("Size", &Size, 1.0f, 0, 10);
                    ImGui::Checkbox("Show on aimbot target", &showOnAimbotTarget);

                    ImGui::SeparatorText("Compass Settings");
                    ImGui::Checkbox("Compass", &compass);
                    ImGui::DragInt("Compass Size", &compass_size, 1.0f, 0, 1000);

                    ImGui::Separator();
                    ImGui::Checkbox("Health Bar", &HealthBar);
                    ImGui::Checkbox("Server Info", &ServerInfo);
                    ImGui::Checkbox("Bullter Tracers", &BulletTracers);
                    ImGui::Checkbox("Coloured Game UI", &ColouredGameUI);

                    ImGui::Separator();

                    ImGui::Text("Screenshots cleared: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Removals"))
                {
                    ImGui::Checkbox("No recoil", &noRecoil);
                    ImGui::Checkbox("No shellshock", &noShellShock);
                    ImGui::Checkbox("No fog", &noFog);

                    ImGui::Separator();

                    ImGui::Text("Screenshots cleared: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Misc"))
                {
                    ImGui::SeparatorText("Game Settings");
                    ImGui::Checkbox("Fast Reload", &fastReload);
                    ImGui::Checkbox("UAV", &UAV);
                    ImGui::Checkbox("Night Vision", &nightVision);
                    ImGui::DragInt("FOV", &FOV, 1.0f, 0, 360);

                    ImGui::SeparatorText("Fun Settings");
                    ImGui::Checkbox("Infinite slide", &infinite_slide);
                    ImGui::Checkbox("Air Stuck", &air_stuck);

                    ImGui::Separator();
                    ImGui::Checkbox("Temp unlock all", &tempUnlockAll);
                    if (ImGui::Button("Disconnect"))
                    {
                        // Implement the disconnect.
                    }
                    static char buf1[64] = ""; ImGui::InputText("", buf1, 64);
                    ImGui::SameLine();
                    if (ImGui::Button("Send Command"))
                    {
                        // send the command.
                    }
                    if (ImGui::Button("Set PM Game 1"))
                    {
                        // Set PM Game 1
                    }
                    if (ImGui::Button("Set PM Game 2"))
                    {
                        // Set PM Game 2
                    }
                    if (ImGui::Button("Set PM Game"))
                    {
                        // Set PM Game
                    }
                    if (ImGui::Button("Game BIO"))
                    {
                        // Game BIO
                    }

                    ImGui::Separator();

                    ImGui::Text("Screenshots cleaned: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Settings"))
                {
                    if (ImGui::Button("Factory Reset"))
                    {
                        // Factory Reset
                    }

                    ImGui::SeparatorText("Max Distances (FSP & Aimbot)");

                    ImGui::DragInt("Players & AI", &PlayerAndAI_, 1.0f, 0, 1000);
                    ImGui::DragInt("Vehicles", &Vehicles_, 1.0f, 0, 1000);
                    ImGui::DragInt("Explosives", &Explosives_, 1.0f, 0, 1000);
                    ImGui::DragInt("Loot", &Loot, 1.0f, 0, 1000);
                    ImGui::DragInt("Items (MP)", &Items, 1.0f, 0, 1000);

                    ImGui::SeparatorText("ESP & Chams Colours");

                    ImGui::ColorEdit3("Enemy Player##1", (float*)&EnemyPlayer);
                    ImGui::ColorEdit3("Visible Enemy Player##1", (float*)&VisibleEnemyPlayer);
                    ImGui::ColorEdit3("Friendly Player##1", (float*)&FriendlyPlayer);
                    ImGui::ColorEdit3("Visible Friendly Player##1", (float*)&VisibleFriendlyPlayer);
                    ImGui::ColorEdit3("Enemy AI##1", (float*)&EnemyAI);
                    ImGui::ColorEdit3("Visible Enemy AI##1", (float*)&VisibleEnemyAI);
                    ImGui::ColorEdit3("Friendly AI##1", (float*)&FreindlyAI);
                    ImGui::ColorEdit3("Visible Friendly AI##1", (float*)&VisibleFriendlyAI);
                    ImGui::ColorEdit3("Aimbot Target##1", (float*)&AimbotTarget);
                    ImGui::ColorEdit3("Enemy Entity##1", (float*)&EnemyEntity);
                    ImGui::ColorEdit3("Friendly Entity##1", (float*)&FriendlyEntity);
                    ImGui::ColorEdit3("Loot##1", (float*)&LootColour);
                    ImGui::ColorEdit3("Special Loot##1", (float*)&SpecialLoot);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Configs"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
