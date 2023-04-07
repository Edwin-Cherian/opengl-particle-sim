#include <string>
#include <iostream>
#include <random>
#include <ctime>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Solver.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    float WINDOW_WIDTH = 1200.0f;
    float WINDOW_HEIGHT = 700.0f;

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //sets the swap interval for the current OpenGL or OpenGL ES context, 
    //i.e. the number of screen updates to wait from the time glfwSwapBuffers 
    //was called before swapping the buffers and returning.  AKA vsync
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
        std::cout << "ERROR: could not initialise glew" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    
    // <------ define constants ------>
    srand(time(NULL));
    const int pCount = 10000;
    float* positions = new float[pCount * 16];
    float* velocities = new float[pCount * 2];
    unsigned int* indices = new unsigned int[pCount * 6];
    
    int vertex_acount = 4;
    int quad_vacount = 4 * vertex_acount;
    int quad_icount = 6;

    
    const float p_size = 2.0f;
    const float spacing = p_size + 1.0f;


    for (int i = 0; i < pCount; i++)
    {   
        const float x_offset = (float)(rand() % (1100 - 100 + 1) + 100);
        const float y_offset = (float)(rand() % (600 - 100 + 1) + 100);
        // top left
        positions[i * 4 * 4 + 0] = x_offset;
        positions[i * 4 * 4 + 1] = y_offset;
        positions[i * 4 * 4 + 2] = 0.0f;
        positions[i * 4 * 4 + 3] = 0.0f;
        // top right
        positions[i * 4 * 4 + 4] = x_offset + p_size;
        positions[i * 4 * 4 + 5] = y_offset;
        positions[i * 4 * 4 + 6] = 1.0f;
        positions[i * 4 * 4 + 7] = 0.0f;
        // bottom right
        positions[i * 4 * 4 + 8] = x_offset + p_size;
        positions[i * 4 * 4 + 9] = y_offset + p_size;
        positions[i * 4 * 4 + 10] = 0.0f;
        positions[i * 4 * 4 + 11] = 1.0f;
        // bottom left
        positions[i * 4 * 4 + 12] = x_offset;
        positions[i * 4 * 4 + 13] = y_offset + p_size;
        positions[i * 4 * 4 + 14] = 1.0f;
        positions[i * 4 * 4 + 15] = 1.0f;

    }

    for (int i = 0; i < pCount; i++)
    {
        velocities[i * 2 + 0] = (float)(rand() % 20 - 10) / 10;
        velocities[i * 2 + 1] = (float)(rand() % 20 - 10) / 10;
    }

    for (int i = 0; i < pCount; i++)
    {
        // join order: ( TL, TR, BR )
        indices[i * quad_icount + 0] = i * 4;
        indices[i * quad_icount + 1] = i * 4 + 1;
        indices[i * quad_icount + 2] = i * 4 + 2;
        // join order: ( BR, BL, TL )
        indices[i * quad_icount + 3] = i * 4 + 2;
        indices[i * quad_icount + 4] = i * 4 + 3;
        indices[i * quad_icount + 5] = i * 4;
    }

    // brackets to create a scope and its just because opengl is annoying
    // and won't "properly" terminate otherwise
    {
        GLCall(glEnable(GL_BLEND))
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // vao = vertex array object
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions,  pCount * quad_vacount * 4); // pcount * vertices attributes per quad * 4 for size of 1 attribute
        VertexBufferLayout layout;
        layout.Push<float>(2); // 2 floats for position
        layout.Push<float>(2); // 2 floats for tex coords 

        va.AddBuffer(vb, layout);
        
        // ibo = index buffer object, links buffer with vao
        IndexBuffer ib(indices, pCount * 6); // pcount = # quads and each quad = 6 indices

        // projection matrix
        glm::vec3 translation(0, 0, 0);
        glm::mat4 proj = glm::ortho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;

        float dx = 0;

        // setting up shaders
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.9f, 0.4f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/enemy2.png");
        texture.Bind(); // bind is 0 by default
        shader.SetUniform1i("u_Texture", 0); // # needs to match bind # from above

        /*va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();*/

        Renderer renderer;
        Solver solver(WINDOW_WIDTH, WINDOW_HEIGHT);

        // initialise gui
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // initialise r value for rect colour
        float r = 0.1f;
        float increment = 0.01f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            for (int i = 0; i < pCount; i++)
            {
                solver.updatePosition(&positions[16 * i], &velocities[2 * i], p_size);
                solver.wallCollision(&positions[16 * i], &velocities[2 * i], p_size);
               /* positions[i * 16 + 0]  += velocities[2 * i + 0];
                positions[i * 16 + 1]  += velocities[2 * i + 1];
                positions[i * 16 + 4]  += velocities[2 * i + 0];
                positions[i * 16 + 5]  += velocities[2 * i + 1];
                positions[i * 16 + 8]  += velocities[2 * i + 0];
                positions[i * 16 + 9]  += velocities[2 * i + 1];
                positions[i * 16 + 12] += velocities[2 * i + 0];
                positions[i * 16 + 13] += velocities[2 * i + 1];*/
            }

            vb.UpdateBuffer(positions, pCount * quad_vacount * 4);

            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.2f, 0.3f, 1.0f);
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
            

            if (r > 1.0f || r < 0.0f)
                increment *= -1;

            r += increment;

            /* Render gui */
            {
                ImGui::Text("Settings");
                ImGui::SliderFloat3("Translation", &translation.x, -200.0f, 200.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            model = glm::translate(glm::mat4(1.0f), translation);
            mvp = proj * view * model;

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    delete[] positions;
    delete[] indices;
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

//using namespace std;
//
//int main() {
//    // define program settings
//    sf::RenderWindow window(sf::VideoMode(1280, 720), "My sim");
//    //window.setVerticalSyncEnabled(true);
//    //window.setFramerateLimit(144);
//    const int numParticles = 5000;
//
//    // create random seed based on time
//    srand(time(NULL));
//
//    // define particle object
//    class Particle {
//        sf::CircleShape point;
//        sf::Vector2f position;
//        sf::Vector2f velocity;
//    public:
//        Particle() {
//            point.setRadius(1);
//            point.setFillColor(sf::Color::Green);
//            position.x = (float)(rand() % (1000 - 200 + 1) + 200);
//            position.y = (float)(rand() % (600 - 100 + 1) + 100);
//            velocity.x = (float)(rand() % 20 - 10) / 10;
//            velocity.y = (float)(rand() % 20 - 10) / 10;
//
//        }
//        void updatePos() {
//            position.x += velocity.x;
//            position.y += velocity.y;
//            resolveCollisions();
//            point.setPosition(position);
//        }
//
//        void resolveCollisions() {
//            if (position.x > 1280 - point.getRadius()) {
//                position.x = 1280 - point.getRadius();
//                velocity.x *= -1;
//            }
//            else if (position.x < 0) {
//                position.x = 0;
//                velocity.x *= -1;
//            }
//            if (position.y > 720 - point.getRadius()) {
//                position.y = 720 - point.getRadius();
//                velocity.y *= -1;
//            }
//            else if (position.y < 0) {
//                position.y = 0;
//                velocity.y *= -1;
//            }
//        }
//
//        void render(sf::RenderWindow& window) {
//            window.draw(point);
//        }
//
//        sf::Vector2f getPos() {
//            return position;
//        }
//
//        sf::CircleShape getShape() {
//            return point;
//        }
//    };
//
//    // define array for accessing particles
//    //array<Particle, 3500> particles;
//    Particle* particles = new Particle[numParticles];
//
//    // define clock and text for performance metrics
//    sf::Clock clock;
//    sf::Font sans;
//    sans.loadFromFile("C:/Users/edwin/Documents/C++ projects/C++ fonts/LDFComicSansLight.ttf");
//    sf::Text fps("", sans, 20);
//    fps.setPosition(0, 0);
//    fps.setFillColor(sf::Color::Red);
//
//
//    while (window.isOpen()) {
//        // <------ clear screen ------>
//        window.clear();
//
//        // <------ display render time ------> 
//        sf::Time elapsed = clock.restart();
//        fps.setString("render time: " + to_string(elapsed.asMilliseconds()) + " ms");
//
//
//        // <------ define close window events ------>
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed)
//                window.close();
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
//                window.close();
//        }
//
//
//        // <------ run physics ------>
//        for (int i = 0; i < numParticles; i++) {
//            //cout << particles[i].getPos().x << " ";
//            particles[i].updatePos();
//            particles[i].render(window);
//        }
//
//        // <------ render objects ------>
//        window.draw(fps);
//        window.display();
//
//    }
//
//
//    return 0;
//}