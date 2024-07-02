#include <glfw_init.h>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "shader.h"

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow*, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    const char* glsl_version = "#version 450"; // GL 4.0 + GLSL 450
    GLFWwindow* window = initialize_window_glsl_450();
    if (window == nullptr)
    {
        return 1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/consola.ttf", 16.0f);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    // Enable Keyboard Controls and Gamepad Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init(glsl_version);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << '\n';
    std::cout << "OpenGL version supported: " << version << '\n';

    Shader our_shader("shaders/test.vs.glsl", "shaders/test.fs.glsl");
    glUseProgram(our_shader.ID);

    // set up vertex data
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    // Create Vertex Array Object
    glCreateVertexArrays(1, &VAO);

    // Create Vertex Buffer Object
    glCreateBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create Element Buffer Object
    glCreateBuffers(1, &EBO);
    glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

    // Specify vertex attributes for VAO
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 1, 0);
    glEnableVertexArrayAttrib(VAO, 1);

    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
    glVertexArrayAttribBinding(VAO, 2, 0);
    glEnableVertexArrayAttrib(VAO, 2);

    // Link buffer objects to the VAO
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 8 * sizeof(float));
    glVertexArrayElementBuffer(VAO, EBO);

    // load and create a texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data;
    // -------------------------
    unsigned int texture1;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture1);

    // set the texture wrapping parameters
    glTextureParameteri(texture1, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture1, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTextureParameteri(texture1, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTextureStorage2D(texture1, 3, GL_RGB8, width, height);
        glTextureSubImage2D(texture1, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(texture1);
    }
    else
    {
        std::cout << "Failed to load texture1" << '\n';
    }
    stbi_image_free(data);
    // -------------------------
    unsigned int texture2;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture2);

    // set the texture wrapping parameters
    glTextureParameteri(texture2, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture2, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTextureParameteri(texture2, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTextureStorage2D(texture2, 3, GL_RGB8, width, height);
        glTextureSubImage2D(texture2, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(texture2);
    }
    else
    {
        std::cout << "Failed to load 2" << '\n';
    }
    stbi_image_free(data);


    GLint texture1Location = glGetUniformLocation(our_shader.ID, "texture1");
    GLint texture2Location = glGetUniformLocation(our_shader.ID, "texture2");

    glUniform1i(texture1Location, 0);
    glUniform1i(texture2Location, 1);
    glBindTextureUnit(0, texture1);
    glBindTextureUnit(1, texture2);


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float x_offset = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Editor");
            ImGui::ColorEdit3("Color", reinterpret_cast<float*>(&clear_color));
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::Separator();

            ImGui::SliderFloat("Position offset", &x_offset, -1.0f, 1.0f);

            ImGui::End();
        }
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
            clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        int pos_offset_location = glGetUniformLocation(our_shader.ID, "pos_offset");
        glUniform1f(pos_offset_location, x_offset);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
