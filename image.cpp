// an implementation of the image class

#include "image.h"

// constructor
image::image() {

}

// copy constructor
image::image(const image& other) {

}

// assignment operator
image& image::operator=(const image& other) {
  if (&other == this) return *this;  // prevent self assignment

  return *this;
}

// destructor
image::~image() {

}
