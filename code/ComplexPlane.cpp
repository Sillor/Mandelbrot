#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include "ComplexPlane.h"
#include "HSL.cpp"
#include <sstream>

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(float aspectRatio) {
	m_aspectRatio = aspectRatio;
	m_view.setSize(BASE_WIDTH, -BASE_HEIGHT * m_aspectRatio);
	m_view.setCenter(0.0, 0.0);
	m_zoomCount = 0;
}

void ComplexPlane::zoomIn() {
	double x, y;
	m_zoomCount++;
	x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_view.setSize(x, -y);
}

void ComplexPlane::zoomOut() {
	double x, y;
	m_zoomCount--;
	x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_view.setSize(x, y);
}

void ComplexPlane::setCenter(Vector2f coord) {
	m_view.setCenter(coord);
}

View ComplexPlane::getView() {
	return m_view;
}

void ComplexPlane::setMouseLocation(Vector2f coord) {
	m_mouseLocation = coord;
}

void ComplexPlane::loadText(Text& text) {
	string l1 = "Mandelbrot Set\n";
	string l2 = "Center: ";
	string l3 = "Cursor: ";
	string l4 = "Left-click to Zoom in\nRight-click to Zoom out";

	string newString;

	string center = "(" + to_string(m_view.getCenter().x) + ", " + to_string(m_view.getCenter().y) + ")\n";
	string mouseCoords = "(" + to_string(m_mouseLocation.x) + ", " + to_string(m_mouseLocation.y) + ")\n";

	newString = l1 + l2 + center + l3 + mouseCoords + l4;

	text.setString(newString);
}

size_t ComplexPlane::countIterations(Vector2f coord) {
	size_t count = 0;
	complex<double> z (0, 0);
	complex<double> c (coord.x, coord.y);

	for (int i = 1; i <= MAX_ITER; i++) {
		z = z * z + c;
		if (abs(z) <= 2.0) {
			count++;
		}
	}
	return count;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
	HSL hueCol;

	if (count >= MAX_ITER) {
		r = g = b = 0;
	}
	else {
		hueCol.Hue = 360.0 / 10.0 * count;
		hueCol.Saturation = 100;
		hueCol.Luminance = 50;

		Color col = hueCol.TurnToRGB();
		r = col.r;
		g = col.g;
		b = col.b;
	}
}