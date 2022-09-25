// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ComplexPlane.h"
#include "HSL.hpp"

// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;

int main() {

    double screenWidth = 1920;//VideoMode::getDesktopMode().width;
    double screenHeight = 1080;// VideoMode::getDesktopMode().height;
    double aspectRatio = (double)screenHeight / screenWidth;

    cout << screenHeight << "\t" << screenWidth << "\t" << aspectRatio;
    
    // Create a video mode object
    VideoMode vm(screenWidth, screenHeight);



    ComplexPlane plane(aspectRatio);

    Font font;
    font.loadFromFile("fonts/editundo.ttf");

    Text text;
    VertexArray vArr;

    RectangleShape textBg;
    textBg.setSize(Vector2f(300,110));
    textBg.setFillColor(Color(0, 0, 0, 100));

	text.setFont(font);
	text.setCharacterSize(screenHeight * screenWidth / 100000);
	text.setStyle(Text::Bold);
    text.setPosition(3, 3);

    vArr.setPrimitiveType(Points);
    vArr.resize(screenWidth * screenHeight);

    enum class State{CALCULATING, DISPLAYING};
    State state = State::CALCULATING;

    // Create and open a window for the game
    RenderWindow window(vm, "Mandelbrot", Style::Titlebar | Style::Close);

   
    // Frame-updating loop
    while (window.isOpen()) {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                Vector2f clickCoords = window.mapPixelToCoords(Mouse::getPosition(window), plane.getView());
                if (event.mouseButton.button == Mouse::Left) {
                    cout << "\nLeft click:\t" << clickCoords.x << "\t" << clickCoords.y;
                    plane.zoomIn();
                    plane.setCenter(clickCoords);
                }
                if (event.mouseButton.button == Mouse::Right) {
                    cout << "\nRight click:\t" << clickCoords.x << "\t" << clickCoords.y;
                    plane.zoomOut();
                    plane.setCenter(clickCoords);
                }
                state = State::CALCULATING;
            }
            if (event.type == Event::MouseMoved) {
                Vector2f mouseCoords = window.mapPixelToCoords(Mouse::getPosition(window), plane.getView());
                plane.setMouseLocation(mouseCoords);
            }
        }

        // Close the window
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        /*
        ****************************************
        Update the scene
        ****************************************
        */

        if (state == State::CALCULATING)
            for (int i = 0; i < screenHeight; i++) {
                for (int j = 0; j < screenWidth; j++) {
                    vArr[j + i * screenWidth].position = { (float)j, (float)i };
                    Vector2f pixelCoords = window.mapPixelToCoords(Vector2i(j, i), plane.getView());
                    int iter = plane.countIterations(pixelCoords);
                    Uint8 r, g, b;
                    plane.iterationsToRGB(iter, r, g, b);
                    vArr[j + i * screenWidth].color = { r, g, b };
                }
            }
        state = State::DISPLAYING;

        plane.loadText(text);

        /*
        ****************************************
        Draw the scene
        ****************************************
        */

        // Clear the frame
        window.clear();

        window.draw(vArr);
        window.draw(textBg);
        window.draw(text);

        // Show everything we just drew
        window.display();
    }

    return 0;
}