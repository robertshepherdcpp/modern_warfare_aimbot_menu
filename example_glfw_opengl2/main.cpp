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

            ImGui::Separator();

            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (ImGui::BeginTabItem("Aimbot"))
                {
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

                    ImGui::Separator();

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

                    ImGui::Separator();

                    ImGui::Checkbox("auto shoot", &autoShoot);
                    ImGui::Checkbox("triggerbot", &triggerBot);

                    ImGui::Separator();

                    ImGui::Text("Screenshots Cleared: %d", num_screenshots_cleared);

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("ESP"))
                {
                    ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Chams"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Visuals"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Removals"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Misc"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Settings"))
                {
                    ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
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
