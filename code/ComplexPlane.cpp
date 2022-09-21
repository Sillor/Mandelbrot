#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include "ComplexPlane.h"

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
	m_view.setSize(x, y);
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
	if (count >= MAX_ITER) {
		r = g = b = 0;
	}
	else if (count <= MAX_ITER / 5) {
		r = 128;
		g = 0;
		b = 128;
	}
	else if (count <= MAX_ITER / 5 * 2) {
		r = 64;
		g = 224;
		b = 208;
	}
	else if (count <= MAX_ITER / 5 * 3) {
		r = 0;
		g = 128;
		b = 0;
	}
	else if (count <= MAX_ITER / 5 * 4) {
		r = 255;
		g = 255;
		b = 0;
	}
	else {
		r = 255;
		g = 0;
		b = 0;
	}
}