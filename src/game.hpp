#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "line_renderer.hpp"
#include "triangle_renderer.hpp"

#include "voronoi.hpp"
#include "line_object.hpp"
#include <atomic>
#include <memory>
#include <vector>

namespace ldwb
{

//Represents the current state of the game
enum class GameState : int {
    ACTIVE = 1,
    MENU   = 2
};

//Size of the cursor
constexpr unsigned int CURSOR_SIZE = 8;

/*
    This class takes care of rendering and managing the general state of the "game"
*/
class Game
{
public:
    Game(unsigned int width, unsigned int height);
    ~Game();

    //initialize game state and load all shaders
    void Init();

    //function for processing input from the user
    void ProcessInput(float dt);
    //function for rendering to the screen
    void Render(float dt);
    //handle key presses
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    //handle button presses
    void MouseCallback(GLFWwindow* window, int button, int action, int mode);
    //handle window resize
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    //game loop
    void RunLoop();
protected:
    GLFWwindow* window_;

    //Renderers
    std::unique_ptr<LineRenderer> line_renderer_;
    std::unique_ptr<TriangleRenderer> triangle_renderer_;

    //voronoi graph object
    ldwb::Voronoi voronoi_;

    //members for game state
    GameState               game_state_;
    
    //members that express user input, window state, etc.
    bool                    keys_[1024];
    unsigned int            width_, height_;

    glm::vec2 screen_mouse_coord_;
    glm::vec2 screen_click_coord_;

    glm::vec2 frame_pos_;
    std::atomic<bool> should_end_;

    unsigned int voronoi_reset_presses_;
    std::atomic<bool> draw_voronoi_color_;
    std::atomic<bool> draw_voronoi_border_;
    std::atomic<bool> draw_voronoi_point_;

};

} //namespace ldwb

#endif