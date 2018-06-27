#include "image.h"
#include <string>
#include <stdlib.h> // malloc
#include <stdio.h> // fprintf

// Triple index array. The respective indices are for the specific RGBA channel,
// row and column. These bitmaps are all 32 bit RGBA, and will thus have 4 
// channels. The canvas size of the image will dictate the exact size of the 
// other two dimensions

//uint8_t*** data; data[rgba][x][y]

class bitmap{
  public:
    bitmap(uint32_t w, uint32_t h);
    bitmap(const bitmap& other);
    bitmap& operator=(const bitmap& other);
    ~bitmap();

    uint8_t*** data;
    uint32_t width;
    uint32_t height;

  private:
    
};

// (1)  default constructor, creates a 1x1 image which is completely black and 
// opaque. Note that black and opaque implies an RGBA value of (0,0,0,0)
 bitmap::bitmap(uint32_t w, uint32_t h) {
  int n = 1; // Image size set to 1
  //uint32_t width = 1;
  //uint32_t height = 1;
  int rgba_vals = 4; // r, g, b, a

  // Saving dimensions as data members in the class
  width = w; 
  height = h;

  // Malloc each of the nested channels and save as rgba for xy
  data  = (uint8_t***) malloc(sizeof(uint8_t***) * 4);
  for(int rgba = 0; rgba < rgba_vals; rgba++) {

    data[rgba] = (uint8_t**) malloc(sizeof(uint8_t**) * width ); // TODO: Draw and confirm
    for(int x = 0; x < width; x++) {
      data[rgba][x] = (uint8_t*) malloc(sizeof(uint8_t*) * height);
      for(int y = 0; y < height; y++) { 
        //data[rgba][x][y] = (uint8_t) malloc(sizeof(uint8_t));
        data[rgba][x][y] = 0;  // Assign RGBA values to zero
      }
    }
  } 
  
 }


// (2) copy constructor.
 bitmap::bitmap(const bitmap& other) { }

// (3) assignment operator
 bitmap& bitmap::operator=(const bitmap& other) { }

// (4) destructor
 bitmap::~bitmap() { }
//TODO: This and others?

// (5) creates an empty (ie, completely black and opaque) image. Note that black 
//and opaque implies an RGBA value of (0,0,0,0). Returns true if the bitmap was 
//created and false if the creation fails (eg, due to memory allocation failure).
bitmap create(uint32_t width, uint32_t height) {
    bitmap bmp(width, height); // Call constructor
    int rgba_vals = 4; // r, g, b, a


  // TODO: Bool based on memory allocation failure and stuff
  return bmp;

 }

// (6) loads a bitmap into the object. Returns true if the bitmap was
//  successfully loaded and false otherwise.
bitmap load(const std::string& filename) {  //TODO: string filename
  int rgba_vals = 4; // r, g, b, a

  FILE *fp;
  fp = fopen(filename.c_str(), "r+"); // .c_str() turns std::str into c style char*

  fclose(fp);

  //TODO: return bitmap. Also change function signature
  bitmap my_bmp = create(5, 3);
  return my_bmp;

}


// (7) saves the bitmap into a bitmap file. Returns true if the bitmap was
//  successfully written to disk and false otherwise.
bool save(bitmap &bmp, const std::string& filename) {  // TODO: Why signature like this?
  int rgba_vals = 4; // r, g, b, a
  //printf("%d", bmp.data[0][0][0]); // TODO: WHYNOT

  FILE *fp;
  fp = fopen(filename.c_str(), "w+"); // .c_str() turns std::str into c style char*


  // Write all values 
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < bmp.width; x++) {
      for(int y = 0; y < bmp.height; y++) {
        fprintf(fp, "%d ", bmp.data[rgba][x][y]);
      }
    fprintf(fp, "\n"); // TODO:  New line at end of row
    }
  } 
  fclose(fp);
  return true; // TODO: Have this actually reflect success
}


// (8) clears every pixel in the bitmap image to the given color.
 bitmap clear(bitmap &bmp, uint8_t r , uint8_t g, uint8_t b, uint8_t a) {

  // Store the rgba parameters in an array in same order as that in bitmap data
  uint8_t rgba_array[4];
  rgba_array[0] = r;
  rgba_array[1] = g;
  rgba_array[2] = b;
  rgba_array[3] = a;

  
  // Assign all values 
  for(int rgba = 0; rgba < sizeof(rgba_array); rgba++) {
    for(int x = 0; x < bmp.width; x++) {
      for(int y = 0; y < bmp.height; y++) {
        bmp.data[rgba][x][y] = rgba_array[rgba];
      }
    }
  } 
  return bmp;
 }


// (9) flips the image horizontally, identical to the corresponding Photoshop 
// operation.
bitmap horizontal_flip(bitmap &bmp) { 
  uint8_t temp; // rgba value
  int rgba_vals = 4;
  uint32_t w = bmp.width;
  uint32_t h = bmp.height;

  // Assign all values 
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w/2; x++) {
      for(int y = 0; y < h; y++) {
        bmp.data[rgba][x][y] = temp; 
        bmp.data[rgba][x][y] = bmp.data[rgba][x-w][y];  // TODO: x-w or w-x
        bmp.data[rgba][w-x][y] = temp;
      }
    }
  } 
  return bmp;
}

// (10) flips the image vertically, identical to the corresponding Photoshop 
// operation.
bitmap vertical_flip(bitmap &bmp) {
  uint8_t temp; // rgba value
  int rgba_vals = 4;
  uint32_t h = bmp.height;
  uint32_t w = bmp.width;


  // Assign all values 
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) {
      for(int y = 0; y < h/2; y++) {
        bmp.data[rgba][x][y] = temp; 
        bmp.data[rgba][x][y] = bmp.data[rgba][x][y-h];  // TODO: y-h or h-w
        bmp.data[rgba][x][h-y] = temp;
      }
    }
  } 
  return bmp;

}


// (11)  performs a blur on the image. For each pixel p in the original image,
//  its new RGBA value should be the average of the 9 pixels centered on p (ie,
// average of the top left, top middle, top right, center left, p, center right,
//  bottom left, bottom middle and bottom right). You probably cannot operate 
//in-situ on the current image and you'll need to create a new temporary image 
//to write to.
// void blur() { }

// (12; bonus) blends a given image with the current image at the specified 
// transparency level. For simplicity, you can assume the two input images will
//  have the same height and width. You'll need to perform some calculations to
//  determine the final color at each pixel position.
// void blend(const bitmap& other, uint8_t transparency) { }


// executive entrypoint
int main(int argc, char** argv) {

  // instantiate a copy of the image object type
  image img;
  bitmap my_bmp = create(5, 3);
  ////my_bmp = horizontal_flip(my_bmp);
  //my_bmp = vertical_flip(my_bmp);
  bool is_saved = save(my_bmp, "saved_file.txt"); 
  bitmap loaded_bmp = load("saved_file.txt");
  loaded_bmp = clear(loaded_bmp, 30, 64, 20, 18);
  bool is_saved2 = save(loaded_bmp, "saved_file2.txt"); 


  return 0;
}


//TODO: Why won't it print line 88 WHYNOT
//TODO: Seg faults for horizontal and vertical flips
//TODO: Get it to save values in save:w

