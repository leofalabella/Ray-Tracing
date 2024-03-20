#include <SFML/Graphics.hpp>
#include "general.h"
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "timer.h"
#include <thread>
#include <mutex>


int main()
{

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Create an enum to represent the currently selected input box
    enum class SelectedInput
    {
        None,
        Width,
        Height,
        SamplesPerPixel,
        MaxDepth,
        LookFromX,
        LookFromY,
        LookFromZ,
        LookAtX,
        LookAtY,
        LookAtZ,
        VUpX,
        VUpY,
        VUpZ
    };

    // Initialize the currently selected input box to None
    SelectedInput selectedInput = SelectedInput::None;

    // Inicialização da janela SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ray Tracing App");

    // Fonte para textos
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf"))
    {
        return EXIT_FAILURE;
    }

    // Create a rectangle shape to represent the input width
    sf::RectangleShape inputBox_Width(sf::Vector2f(200, 50));
    inputBox_Width.setFillColor(sf::Color::White);
    inputBox_Width.setPosition(100, 100);

    // Create a text object to represent the input width
    sf::Text inputText_Width;
    inputText_Width.setFont(font);
    inputText_Width.setCharacterSize(24);
    inputText_Width.setFillColor(sf::Color::Black);
    inputText_Width.setPosition(105, 105);

    sf::Text Label_Width;
    Label_Width.setFont(font);
    Label_Width.setString("Width");
    Label_Width.setCharacterSize(24);
    Label_Width.setFillColor(sf::Color::Red);
    Label_Width.setPosition(100, 70);

    // Create a rectangle shape to represent the input height
    sf::RectangleShape inputBox_Height(sf::Vector2f(200, 50));
    inputBox_Height.setFillColor(sf::Color::White);
    inputBox_Height.setPosition(100, 200);

    // Create a text object to represent the input height
    sf::Text inputText_Height;
    inputText_Height.setFont(font);
    inputText_Height.setCharacterSize(24);
    inputText_Height.setFillColor(sf::Color::Black);
    inputText_Height.setPosition(105, 205);

    sf::Text Label_Height;
    Label_Height.setFont(font);
    Label_Height.setString("Height");
    Label_Height.setCharacterSize(24);
    Label_Height.setFillColor(sf::Color::Red);
    Label_Height.setPosition(100, 170);

    // Create a rectangle shape to represent the input samples per pixel
    sf::RectangleShape inputBox_SamplesPerPixel(sf::Vector2f(200, 50));
    inputBox_SamplesPerPixel.setFillColor(sf::Color::White);
    inputBox_SamplesPerPixel.setPosition(100, 300);

    // Create a text object to represent the input samples per pixel
    sf::Text inputText_SamplesPerPixel;
    inputText_SamplesPerPixel.setFont(font);
    inputText_SamplesPerPixel.setCharacterSize(24);
    inputText_SamplesPerPixel.setFillColor(sf::Color::Black);
    inputText_SamplesPerPixel.setPosition(105, 305);

    sf::Text Label_SamplesPerPixel;
    Label_SamplesPerPixel.setFont(font);
    Label_SamplesPerPixel.setString("Samples Per Pixel");
    Label_SamplesPerPixel.setCharacterSize(24);
    Label_SamplesPerPixel.setFillColor(sf::Color::Red);
    Label_SamplesPerPixel.setPosition(100, 270);

    // Create a rectangle shape to represent the input max depth
    sf::RectangleShape inputBox_MaxDepth(sf::Vector2f(200, 50));
    inputBox_MaxDepth.setFillColor(sf::Color::White);
    inputBox_MaxDepth.setPosition(100, 400);

    // Create a text object to represent the input max depth
    sf::Text inputText_MaxDepth;
    inputText_MaxDepth.setFont(font);
    inputText_MaxDepth.setCharacterSize(24);
    inputText_MaxDepth.setFillColor(sf::Color::Black);
    inputText_MaxDepth.setPosition(105, 405);

    sf::Text Label_MaxDepth;
    Label_MaxDepth.setFont(font);
    Label_MaxDepth.setString("Max Depth");
    Label_MaxDepth.setCharacterSize(24);
    Label_MaxDepth.setFillColor(sf::Color::Red);
    Label_MaxDepth.setPosition(100, 370);

    // Create a rectangle shape to represent the input look from x
    sf::RectangleShape inputBox_LookFromX(sf::Vector2f(50, 50));
    inputBox_LookFromX.setFillColor(sf::Color::White);
    inputBox_LookFromX.setPosition(500, 100);
    
    // Create a text object to represent the input look from x
    sf::Text inputText_LookFromX;
    inputText_LookFromX.setFont(font);
    inputText_LookFromX.setCharacterSize(24);
    inputText_LookFromX.setFillColor(sf::Color::Black);
    inputText_LookFromX.setPosition(505, 105);

    // Create a rectangle shape to represent the input look from y
    sf::RectangleShape inputBox_LookFromY(sf::Vector2f(50, 50));
    inputBox_LookFromY.setFillColor(sf::Color::White);
    inputBox_LookFromY.setPosition(560, 100);
    
    // Create a text object to represent the input look from y
    sf::Text inputText_LookFromY;
    inputText_LookFromY.setFont(font);
    inputText_LookFromY.setCharacterSize(24);
    inputText_LookFromY.setFillColor(sf::Color::Black);
    inputText_LookFromY.setPosition(565, 105);

    // Create a rectangle shape to represent the input look from z
    sf::RectangleShape inputBox_LookFromZ(sf::Vector2f(50, 50));
    inputBox_LookFromZ.setFillColor(sf::Color::White);
    inputBox_LookFromZ.setPosition(620, 100);
    
    // Create a text object to represent the input look from z
    sf::Text inputText_LookFromZ;
    inputText_LookFromZ.setFont(font);
    inputText_LookFromZ.setCharacterSize(24);
    inputText_LookFromZ.setFillColor(sf::Color::Black);
    inputText_LookFromZ.setPosition(625, 105);

    sf::Text Label_LookFrom;
    Label_LookFrom.setFont(font);
    Label_LookFrom.setString("Look From");
    Label_LookFrom.setCharacterSize(24);
    Label_LookFrom.setFillColor(sf::Color::Red);
    Label_LookFrom.setPosition(525, 70);

    // Create a rectangle shape to represent the input look at x
    sf::RectangleShape inputBox_LookAtX(sf::Vector2f(50, 50));
    inputBox_LookAtX.setFillColor(sf::Color::White);
    inputBox_LookAtX.setPosition(500, 200);

    // Create a text object to represent the input look at x
    sf::Text inputText_LookAtX;
    inputText_LookAtX.setFont(font);
    inputText_LookAtX.setCharacterSize(24);
    inputText_LookAtX.setFillColor(sf::Color::Black);
    inputText_LookAtX.setPosition(505, 205);

    // Create a rectangle shape to represent the input look at y
    sf::RectangleShape inputBox_LookAtY(sf::Vector2f(50, 50));
    inputBox_LookAtY.setFillColor(sf::Color::White);
    inputBox_LookAtY.setPosition(560, 200);

    // Create a text object to represent the input look at y
    sf::Text inputText_LookAtY;
    inputText_LookAtY.setFont(font);
    inputText_LookAtY.setCharacterSize(24);
    inputText_LookAtY.setFillColor(sf::Color::Black);
    inputText_LookAtY.setPosition(565, 205);

    // Create a rectangle shape to represent the input look at z
    sf::RectangleShape inputBox_LookAtZ(sf::Vector2f(50, 50));
    inputBox_LookAtZ.setFillColor(sf::Color::White);
    inputBox_LookAtZ.setPosition(620, 200);
    
    // Create a text object to represent the input look at z
    sf::Text inputText_LookAtZ;
    inputText_LookAtZ.setFont(font);
    inputText_LookAtZ.setCharacterSize(24);
    inputText_LookAtZ.setFillColor(sf::Color::Black);
    inputText_LookAtZ.setPosition(625, 205);

    sf::Text Label_LookAt;
    Label_LookAt.setFont(font);
    Label_LookAt.setString("Look At");
    Label_LookAt.setCharacterSize(24);
    Label_LookAt.setFillColor(sf::Color::Red);
    Label_LookAt.setPosition(525, 170);

    // Create a rectangle shape to represent the button
    sf::RectangleShape button(sf::Vector2f(100, 50));
    button.setFillColor(sf::Color::Green);
    button.setPosition(525, 300);

    // Create a text object to represent the button label
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("Render");
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(530, 305);

    std::string width;
    std::string height;
    std::string samplesPerPixel;
    std::string maxDepth;
    std::string lookFromX;
    std::string lookFromY;
    std::string lookFromZ;
    std::string lookAtX;
    std::string lookAtY;
    std::string lookAtZ;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // Handle mouse click events
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Check if the mouse click is within the bounds of the input boxes
                    if (inputBox_Width.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::Width;
                    
                    else if (inputBox_Height.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::Height;
                    
                    else if (inputBox_SamplesPerPixel.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::SamplesPerPixel;
                    
                    else if (inputBox_MaxDepth.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::MaxDepth;

                    else if (inputBox_LookFromX.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromX;

                    else if (inputBox_LookFromY.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromY;

                    else if (inputBox_LookFromZ.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookFromZ;
                    
                    else if (inputBox_LookAtX.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtX;

                    else if (inputBox_LookAtY.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtY;

                    else if (inputBox_LookAtZ.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        selectedInput = SelectedInput::LookAtZ;

                    else if (button.getGlobalBounds().contains(mousePos.x, mousePos.y))
                        window.close();

                    else
                        selectedInput = SelectedInput::None;
                }
            }

            // Handle text input events
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    // Update the text for the currently selected input box
                    switch (selectedInput)
                    {
                    case SelectedInput::Width:
                        width += static_cast<char>(event.text.unicode);
                        inputText_Width.setString(width);
                        break;
                    case SelectedInput::Height:
                        height += static_cast<char>(event.text.unicode);
                        inputText_Height.setString(height);
                        break;
                    case SelectedInput::SamplesPerPixel:
                        samplesPerPixel += static_cast<char>(event.text.unicode);
                        inputText_SamplesPerPixel.setString(samplesPerPixel);
                        break;
                    case SelectedInput::MaxDepth:
                        maxDepth += static_cast<char>(event.text.unicode);
                        inputText_MaxDepth.setString(maxDepth);
                        break;
                    case SelectedInput::LookFromX:
                        lookFromX += static_cast<char>(event.text.unicode);
                        inputText_LookFromX.setString(lookFromX);
                        break;
                    case SelectedInput::LookFromY:
                        lookFromY += static_cast<char>(event.text.unicode);
                        inputText_LookFromY.setString(lookFromY);
                        break;
                    case SelectedInput::LookFromZ:
                        lookFromZ += static_cast<char>(event.text.unicode);
                        inputText_LookFromZ.setString(lookFromZ);
                        break;
                    case SelectedInput::LookAtX:
                        lookAtX += static_cast<char>(event.text.unicode);
                        inputText_LookAtX.setString(lookAtX);
                        break;
                    case SelectedInput::LookAtY:
                        lookAtY += static_cast<char>(event.text.unicode);
                        inputText_LookAtY.setString(lookAtY);
                        break;
                    case SelectedInput::LookAtZ:
                        lookAtZ += static_cast<char>(event.text.unicode);
                        inputText_LookAtZ.setString(lookAtZ);
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        window.clear();

        window.draw(inputBox_Width);
        window.draw(inputText_Width);
        window.draw(Label_Width);

        window.draw(inputBox_Height);
        window.draw(inputText_Height);
        window.draw(Label_Height);

        window.draw(inputBox_SamplesPerPixel);
        window.draw(inputText_SamplesPerPixel);
        window.draw(Label_SamplesPerPixel);

        window.draw(inputBox_MaxDepth);
        window.draw(inputText_MaxDepth);
        window.draw(Label_MaxDepth);

        window.draw(button);
        window.draw(buttonText);

        window.draw(inputBox_LookFromX);
        window.draw(inputText_LookFromX);
        window.draw(inputBox_LookFromY);
        window.draw(inputText_LookFromY);
        window.draw(inputBox_LookFromZ);
        window.draw(inputText_LookFromZ);
        window.draw(Label_LookFrom);

        window.draw(inputBox_LookAtX);
        window.draw(inputText_LookAtX);
        window.draw(inputBox_LookAtY);
        window.draw(inputText_LookAtY);
        window.draw(inputBox_LookAtZ);
        window.draw(inputText_LookAtZ);
        window.draw(Label_LookAt);

        window.display();
    }

    // Camera
    camera cam;

    unsigned int WIDTH = std::stoi(width);
    unsigned int HEIGHT = std::stoi(height);

    cam.image_width = WIDTH;
    cam.image_height = HEIGHT; 
    cam.samples_per_pixel = std::stoi(samplesPerPixel);
    cam.max_depth = std::stoi(maxDepth);

    cam.vfov = 20;

    cam.lookfrom = point3(std::stoi(lookFromX), std::stoi(lookFromY), std::stoi(lookFromZ));
    cam.lookat = point3(std::stoi(lookAtX), std::stoi(lookAtY), std::stoi(lookAtZ));
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    cam.render(world);
}

camera cam;

constexpr unsigned N = 16;

unsigned W_CNT = (cam.image_width + N - 1) / N;
unsigned H_CNT = (cam.image_height + N - 1) / N;

struct Pixels {

    Pixels(unsigned w, unsigned h)
        : width{w},
        height{h},
        data{new float[width * height * 5]()},       // RGBA + sample count
        pixels{new sf::Uint8[width * height * 4]()}  // RGBA
    {}

    ~Pixels() {
    delete[] data;
    delete[] pixels;
    }

    sf::Uint8 *get_pixels() {
    // convert accumulated pixels values so we can display them
    for (int i = 0; i < cam.image_height; i++)
        for (int j = 0; j < cam.image_width; j++) {
        const unsigned data_pos = (i * width + j) * 5;
        const unsigned pix_pos = ((cam.image_height - i - 1) * width + j) << 2;
        const float ns = data[data_pos + 4];  // number of accumulated samples
        pixels[pix_pos + 0] =
            sf::Uint8(255.99f * (sqrtf(data[data_pos + 0] / ns)));
        pixels[pix_pos + 1] =
            sf::Uint8(255.99f * (sqrtf(data[data_pos + 1] / ns)));
        pixels[pix_pos + 2] =
            sf::Uint8(255.99f * (sqrtf(data[data_pos + 2] / ns)));
        pixels[pix_pos + 3] = 255u;
        }

    return pixels;
    }

    inline void accumulate(unsigned x, unsigned y, float r, float g, float b) {
        const unsigned pos = (y * width + x) * 5;
        data[pos + 0] += r;
        data[pos + 1] += g;
        data[pos + 2] += b;
        data[pos + 3] += 255.f;  // opaque
        data[pos + 4] += 1.f;    // number of samples
    }

    inline void accumulate(unsigned x, unsigned y, const vec3 &col) {
        accumulate(x, y, col.x, col.y, col.z);
    }

    unsigned width;
    unsigned height;
    float *data;  // RGBA + sample count

    private:
    // use get_pixels()
    sf::Uint8 *pixels;  // RGBA
} pixels{cam.image_width,cam.image_height};



struct Task {
  Task() : my_id{id++} {}

  Task(int x, int y) : sx{x}, sy{y}, my_id{id++} {}

  void move_in_pattern(int &rx, int &ry) {
    // snake pattern implementation
    static int x = -1, y = H_CNT - 1;
    static int dir = 0;

    x = dir ? x - 1 : x + 1;
    if (x == W_CNT || x == -1) {
      x = y & 1 ? W_CNT - 1 : 0;
      y--;
      dir = !dir;
    }
    rx = x;
    ry = y;
  }

  bool get_next_task() {
    static bool taken[H_CNT][W_CNT] = {};
    static std::mutex m;

    std::lock_guard<std::mutex> guard{m};

    bool found = false;
    int x, y;
    while (!found) {
      move_in_pattern(x, y);
      if (x < 0 || x > W_CNT || y < 0 || y > H_CNT) break;

      if (!taken[y][x]) {
        sx = x * N;
        sy = y * N;
        taken[y][x] = true;
        found = true;
      }
    }

    return found;
  }

  void operator()() {
    bool done = false;
    do {
      if (!get_next_task()) {
        done = true;
        continue;
      }

      for (unsigned s = 0; s < cam.samples_per_pixel; s++)
        for (unsigned y = sy; y < sy + N; y++)
          for (unsigned x = sx; x < sx + N; x++) {
            if (x < 0 || y < 0 || x >= cam.image_width || y >= cam.image_height) continue;

            const float u = float(x + drand48()) / float(cam.image_width);
            const float v = float(y + drand48()) / float(cam.image_height);
            ray r = cam.get_ray(u, v);
            const vec3 col = color(r, world, 0);

            pixels.accumulate(x, y, col);
          }
    } while (!done);

    done_count++;

    std::cout << "Thread " << my_id << " is done!" << std::endl;
  }

  int sx = -1, sy = -1;
  int my_id;
  static int id;
};