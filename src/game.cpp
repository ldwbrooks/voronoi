#include "game.hpp"
#include "resource_manager.hpp"

#include <iostream>
#include <thread>
#include <chrono>

namespace ldwb
{

Game::Game(unsigned int width, unsigned int height) 
    : game_state_(GameState::ACTIVE),
      voronoi_(glm::vec2(0.0f), glm::vec2(static_cast<float>(width), static_cast<float>(height))), 
      keys_(), width_(width), height_(height),
      frame_pos_(0.0f,0.0f), voronoi_reset_presses_(0),
      draw_voronoi_color_(true), draw_voronoi_border_(true), draw_voronoi_point_(true)
{ 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, true);

    window_ = glfwCreateWindow(width_, height_, "TheGame", nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    unsigned char pixels[CURSOR_SIZE * CURSOR_SIZE * 4];
    memset(pixels, 0xff, sizeof(pixels));

    //set unique cursor for the window
    GLFWimage image;
    image.width = CURSOR_SIZE;
    image.height = CURSOR_SIZE;
    image.pixels = pixels;

    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    glfwSetCursor(window_, cursor);
    //load all OpenGL function pointers through glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Initialization of GLAD failed." << std::endl;
        return;
    }

    glfwSetWindowUserPointer( window_, this );
    
    auto keyCallback = []( GLFWwindow* window, int key, int scancode, int action, int mods ) {
        auto me = (Game*)glfwGetWindowUserPointer( window );
        me->KeyCallback( window, key, scancode, action, mods );
    };
    glfwSetKeyCallback( window_, keyCallback );

    auto framebufferSizeCallback = []( GLFWwindow* window, int width, int height ) {
        auto me = (Game*)glfwGetWindowUserPointer(window );
        me->FramebufferSizeCallback( window, width, height );
    };
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    //OpenGL settings
    glViewport(0, 0, width_, height_);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Game::~Game()
{
    delete line_renderer_.get();
    delete triangle_renderer_.get();
}

void Game::Init()
{
    //load all shaders
    ResourceHandler::LoadShader("shaders/line.vs", "shaders/line.frag", "", "line");
    ResourceHandler::LoadShader("shaders/triangle.vs", "shaders/triangle.frag", "", "triangle");
    //configure all shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width_), 
        static_cast<float>(height_), 0.0f, -1.0f, 1.0f);
    ResourceHandler::GetShader("line").Use().SetInteger("image", 0);
    ResourceHandler::GetShader("line").SetMatrix4("projection", projection);
    ResourceHandler::GetShader("triangle").Use().SetInteger("image", 0);
    ResourceHandler::GetShader("triangle").SetMatrix4("projection", projection);
    //create renderers
    line_renderer_ = std::make_unique<LineRenderer>(ResourceHandler::GetShader("line"));
    triangle_renderer_ = std::make_unique<TriangleRenderer>(ResourceHandler::GetShader("triangle"));
}

void Game::RunLoop()
{
    //deltaTime variables
    float deltaTime = 0.0f;
    float lastFrame = glfwGetTime();
    constexpr float DESIRED_FRAMERATE = 1.0f/60.0f;
    while (!glfwWindowShouldClose(window_))
    {
        //calculate difference in time from the previous frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        //handle user input
        ProcessInput(deltaTime);

        //render everything to the screen
        glClearColor(0.0f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Render(deltaTime);

        glfwSwapBuffers(window_);

        //wait until desired rate is reached
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        constexpr float DESIRED_FRAMERATE = 1.0f / 120.0f;
        float time_diff = (DESIRED_FRAMERATE - deltaTime);
        std::this_thread::sleep_for(std::chrono::duration<float, std::ratio<1LL, 1LL>>(time_diff));
    }

    //delete all resources loaded by the resource handler
    ResourceHandler::Clear();

    glfwTerminate();
}

void Game::ProcessInput(float dt)
{
    if (game_state_ == GameState::ACTIVE)
    {
        if (keys_[GLFW_KEY_UP])
        {
            frame_pos_.y += 10;
        }
        if (keys_[GLFW_KEY_DOWN])
        {
            frame_pos_.y -= 10;
        }
        if (keys_[GLFW_KEY_LEFT])
        {
            frame_pos_.x += 10;
        }
        if (keys_[GLFW_KEY_RIGHT])
        {
            frame_pos_.x -= 10;
        }
        if (keys_[GLFW_KEY_G])
        {
            voronoi_reset_presses_ = (voronoi_reset_presses_ + 1) % 3;
            if (voronoi_reset_presses_ == 0)
                voronoi_.RelaxDiagram();
        }
        if (keys_[GLFW_KEY_R])
        {
            voronoi_.ResetDiagram(50);
        }
        if (keys_[GLFW_KEY_T])
        {
            voronoi_.ResetDiagram(100);
        }
        if (keys_[GLFW_KEY_V])
            draw_voronoi_color_ = true;
        if (keys_[GLFW_KEY_B])
            draw_voronoi_color_ = false;
        if (keys_[GLFW_KEY_H])
            draw_voronoi_border_ = true;
        if (keys_[GLFW_KEY_J])
            draw_voronoi_border_ = false;
        if (keys_[GLFW_KEY_Y])
            draw_voronoi_point_ = true;
        if (keys_[GLFW_KEY_U])
            draw_voronoi_point_ = false;
        // //get mouse position
        // {
        //     //get cursor position
        //     double xpos, ypos;
        //     glfwGetCursorPos(window_, &xpos, &ypos);

        //     //adjust for cursor size
        //     xpos += CURSOR_SIZE / 2.0f;
        //     ypos += CURSOR_SIZE / 2.0f;
        //     xpos -= frame_pos_.x;
        //     ypos -= frame_pos_.y;
        //     //convert window coord to screen coord
        //     int curr_width, curr_height;
        //     glfwGetWindowSize(window_, &curr_width, &curr_height);
        //     double width_ratio = static_cast<double>(width_) / static_cast<double>(curr_width);
        //     xpos = xpos * width_ratio;
        //     double height_ratio = static_cast<double>(height_) / static_cast<double>(curr_height);
        //     ypos = ypos * height_ratio;
        //     screen_mouse_coord_ = glm::vec2(xpos, ypos);

        //     //TODO: make use of the mouse cursor
        // }
    }
}

void Game::Render(float dt)
{
    line_renderer_->SetFramePosition(frame_pos_);
    triangle_renderer_->SetFramePosition(frame_pos_);
    if (game_state_ == GameState::ACTIVE)
    {        
        if (draw_voronoi_color_)
        {
            voronoi_.DrawSiteColor(*triangle_renderer_);
        }
        if (draw_voronoi_border_)
        {
            voronoi_.DrawSiteBorder(*line_renderer_);
        }
        if (draw_voronoi_point_)
        {
            voronoi_.DrawSitePoint(*line_renderer_);
        }

        //handle text
        // std::string fps = "FPS: " + std::to_string(1.0f / dt);
        // std::string mouse = "x: " + std::to_string(screen_mouse_coord_.x) + "\ny: " + std::to_string(screen_mouse_coord_.y);
    }
    else
    {

    }
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //set WindowShouldClose to true (closing the application), when a user presses the escape key 
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys_[key] = true;
        else if (action == GLFW_RELEASE)
            keys_[key] = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //update the viewport to match the new window dimensions
    glViewport(0, 0, width, height);
}

} //namespace ldwb