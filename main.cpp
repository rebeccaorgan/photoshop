#include "image.h"
#include <string>
#include <stdlib.h> // malloc
#include <stdio.h> // fprintf

// Triple index array. The respective indices are for the specific RGBA channel,
// row and column. These bitmaps are all 32 bit RGBA, and will thus have 4 
// channels. The canvas size of the image will dictate the exact size of the 
// other two dimensions


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
  printf("Inside create\n");
  bitmap bmp(width, height); // Call constructor
  int rgba_vals = 4; // r, g, b, a


  // TODO: Bool based on memory allocation failure and stuff
  return bmp;

 }

// (6) loads a bitmap into the object. Returns true if the bitmap was
//  successfully loaded and false otherwise.
bitmap load(const std::string& filename) { 
  printf("Inside load()\n");
  int rgba_vals = 4; // r, g, b, a

  FILE *fp;
  fp = fopen(filename.c_str(), "r+"); // .c_str() turns std::str into c style char*

  // Get dimensions to know how big to create image
  // TODO: pull dimensions from metadata instead
  // First get number of rows
  int row_count = 0;
  int c; // To store a character read from the file. Has to be int for EOF
  while((c = fgetc(fp)) != EOF) {
    if(c == '\n') {  // Check if new line
      row_count++;
    }
  }
  printf("Finished with rows\n");

  // Now get number of columns
  int col_count;
  rewind(fp); //Reset fp to beginning
  while((c = fgetc(fp)) != '\n') {
    if(c == ' ') {
      col_count++;
    }
  }
  printf("Finished with cols\n");
  uint32_t h = row_count / rgba_vals;
  uint32_t w = col_count;

  // Create bitmap
  bitmap bmp = create(h, w);

  // Test read the buffer
/*  rewind(fp);
  char buffer[512];
  fread(buffer, 512, 1, fp);
  printf("%s", buffer);
*/
  // Populate bitmap
  rewind(fp);
  char str[65536]; //TODO: Will need eed megabytes / gigabytes, right? 


    for(int rgba = 0; rgba < rgba_vals; rgba++) {
      for(int x = 0; x < h; x++) { // TODO: Why is height on outside, width on inside
        for(int y = 0; y < w; y++) {

          fscanf(fp, "%s", str);
          float val = atof(str); //Convert char* to float
          bmp.data[rgba][x][y] = val;  // Assign RGBA values 

        }
        printf("\n");
      }
    }
  fclose(fp);

  return bmp;

}


// (7) saves the bitmap into a bitmap file. Returns true if the bitmap was
//  successfully written to disk and false otherwise.
bool save(bitmap &bmp, const std::string& filename) {  // TODO: Why signature like this?
  printf("Inside save\n");
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
  printf("Inside clear\n");

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
  printf("Inside horizontal_flip()\n");

  uint8_t temp; // rgba value
  int rgba_vals = 4;
  uint32_t w = bmp.width;
  uint32_t h = bmp.height;
  int y_cap; // halfway mark; stopping point for why in inner for loop
  if(h % 2) { // height is odd
    y_cap = (h + 1) / 2;
  }
  else { // height is even
    y_cap = h / 2;
  }

  // Assign all values 
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) {
      for(int y = 0; y < y_cap; y++) {
        temp = bmp.data[rgba][x][y];
        bmp.data[rgba][x][y] = bmp.data[rgba][x][h-1-y]; // h-1 because  offset 
        bmp.data[rgba][x][h-1-y] = temp;
      }
    }
  } 
  return bmp;
}

// (10) flips the image vertically, identical to the corresponding Photoshop 
// operation.
bitmap vertical_flip(bitmap &bmp) {
  printf("Inside vertical_flip()\n");

  uint8_t temp; // rgba value
  int rgba_vals = 4;
  uint32_t h = bmp.height;
  uint32_t w = bmp.width;
  int x_cap; // halfway mark; stopping point for why in inner for loop
  if(w % 2) { // height is odd
    x_cap = (w + 1) / 2;
  }
  else { // height is even
    x_cap = w / 2;
  }

  // Assign all values 
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < x_cap; x++) {
      for(int y = 0; y < w; y++) {
        temp = bmp.data[rgba][x][y];
        bmp.data[rgba][x][y] = bmp.data[rgba][w-1-x][y]; // w-1 
        bmp.data[rgba][w-1-x][y] = temp;
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
  //bitmap my_bmp = create(5, 3);
  //my_bmp = horizontal_flip(my_bmp);
  bitmap loaded_bmp = load("to_load.txt");
  loaded_bmp = vertical_flip(loaded_bmp);
  //loaded_bmp = horizontal_flip(loaded_bmp);
  //my_bmp = clear(my_bmp, 30, 64, 20, 18);
  //bitmap loaded_bmp = vertical_flip(my_bmp);
  save(loaded_bmp, "saved_file.txt"); 

  return 0;
}


//TODO: Why won't it print line 88 WHYNOT
//TODO: Seg faults for horizontal and vertical flips
//TODO: Get it to save values in save

