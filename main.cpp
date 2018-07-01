#include "image.h"
#include <string>
#include <stdlib.h> // malloc
#include <stdio.h> // fprintf

// Triple index array. The respective indices are for the specific RGBA channel,
// row and column. These bitmaps are all 32 bit RGBA, and will thus have 4 
// channels. The canvas size of the image will dictate the exact size of the 
// other two dimensions

int rgba_vals = 4; // TODO: Okay to have global?

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
  printf("Inside default constructor\n");

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
// Deep copy. 
// Want "this" to be identical to other
bitmap::bitmap(const bitmap& other) {  // No dimensions because other is already created 
  printf("Inside copy constructor\n");

  width = other.width;
  height = other.height;

  // Malloc each of the nested channels and save as rgba for xy
  data  = (uint8_t***) malloc(sizeof(uint8_t***) * 4);
  
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    data[rgba] = (uint8_t**) malloc(sizeof(uint8_t**) * width ); // TODO: Draw and confirm

    for(int x = 0; x < width; x++) {
      data[rgba][x] = (uint8_t*) malloc(sizeof(uint8_t*) * height);
    
      for(int y = 0; y < height; y++) { 
        //data[rgba][x][y] = (uint8_t) malloc(sizeof(uint8_t));
        data[rgba][x][y] = other.data[rgba][x][y];  // Assign RGBA values to zero
      }
    }
  }
}

// (3) assignment operator
bitmap& bitmap::operator=(const bitmap& other) { 
  printf("Inside assignment operator\n");

  //TODO: Check if is same size. Implement destructor and copy constructor?

  width = other.width;
  height = other.height;

  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < width; x++) {
      for(int y = 0; y < height; y++) { 
        data[rgba][x][y] = other.data[rgba][x][y];  // Assign RGBA values to zero
      } 
    }
  }
}


// (4) destructor
bitmap::~bitmap() { 
  printf("Inside destructor\n");
  
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    
    for(int x = 0; x < width; x++) {
      free(data[rgba][x]);    
    }

    free(data[rgba]);
  }
  
  //TODO
  free(data);
  //free(width);
  //free(height);
}

// (5) creates an empty (ie, completely black and opaque) image. Note that black 
//and opaque implies an RGBA value of (0,0,0,0). Returns true if the bitmap was 
//created and false if the creation fails (eg, due to memory allocation failure).
bitmap create(uint32_t width, uint32_t height) {
  printf("Inside create\n");
  bitmap bmp(width, height); // Call constructor

  // TODO: Bool based on memory allocation failure and stuff
  return bmp;
 }

// (6) loads a bitmap into the object. Returns true if the bitmap was
//  successfully loaded and false otherwise.
bitmap load(const std::string& filename) { 
  printf("Inside load()\n");

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
      }
    }
  fclose(fp);
  return bmp;

}

// (7) saves the bitmap into a bitmap file. Returns true if the bitmap was
//  successfully written to disk and false otherwise.
bool save(bitmap &bmp, const std::string& filename) {  // TODO: Why signature like this?
  printf("Inside save\n");
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
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
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
bitmap blur(bitmap &bmp) { 
  printf("Inside blur\n");

  bitmap temp_bmp = bmp;

  uint32_t h = bmp.height;
  uint32_t w = bmp.width;
  uint8_t neighbor_pixel;
  //TODO: Just have temp_bmp and have new values go over to that one. Deep copy
  // it over at end to bmp that gets returned
  // TODO: Borders

  // All but borders
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 1; x < w - 1; x++) { // TODO: confirm -1 
      for(int y = 1; y < h - 1; y++) {

        neighbor_pixel = bmp.data[rgba][x-1][y+1] + bmp.data[rgba][x][y+1] +
               bmp.data[rgba][x+1][y+1] + bmp.data[rgba][x-1][y] +
               bmp.data[rgba][x][y] + bmp.data[rgba][x][y+1] + 
               bmp.data[rgba][x-1][y-1] + bmp.data[rgba][x][y-1] +
               bmp.data[rgba][x-1][y-1];
        temp_bmp.data[rgba][x][y] = neighbor_pixel / 9;
      }
    }
  }
  //printf("%d", temp_bmp.data[0][3][3]);

  printf("Marker\n");
  bmp = temp_bmp; // Deep copy using copy constructor
  return bmp; 
}

// (12; bonus) blends a given image with the current image at the specified 
// transparency level. For simplicity, you can assume the two input images will
//  have the same height and width. You'll need to perform some calculations to
//  determine the final color at each pixel position.
bitmap blend(const bitmap &orig_bmp, const bitmap& other_bmp, uint8_t transparency) { 
  printf("Inside blend\n");

  //TODO: Check the two are same dimensions
  uint32_t h = orig_bmp.height;
  uint32_t w = orig_bmp.width;
  float percentage = transparency / (float) 256;

  bitmap blend_bmp = create(w, h);  

  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) {
      for(int y = 0; y < h; y++) {
        blend_bmp.data[rgba][x][y] = (int) (orig_bmp.data[rgba][x][y]*percentage) + 
                                (other_bmp.data[rgba][x][y]*(1-percentage));
      }
    }
  }
  
  return blend_bmp;
}

// (13) prints out the matrix values of an image
void print(bitmap &bmp) { 
  printf("Inside print\n");
  
  uint32_t h = bmp.height;
  uint32_t w = bmp.width;

  printf("\n");
  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) { 
      for(int y = 0; y < h; y++) {
        printf("%d ", bmp.data[rgba][x][y]);
      }
      printf("\n"); // Print new line at end of row
    }
    printf("\n"); // Print extra space between matrices
  }
}


// executive entrypoint
int main(int argc, char** argv) {

  // instantiate a copy of the image object type
  //image img;
  bitmap zeros_bmp = create(5, 6);
  bitmap ones_bmp = clear(zeros_bmp, 1, 1, 1, 1);
  bitmap deciles_bmp = clear(zeros_bmp, 10, 20, 30, 40);
  bitmap edge_deciles_bmp = load("edges.txt");

  //my_bmp = vertical_flip(loaded_bmp);
  //my_bmp = horizontal_flip(loaded_bmp);

  //bitmap blur_bmp = blur(edge_deciles_bmp);
  bitmap blended_bmp = blend(ones_bmp, deciles_bmp, 128);
  print(blended_bmp);
  //save(deciles_bmp, "deciles.txt"); 

  return 0;
}

// TODO: all todos

// TODO: Use real images
// TODO: Have load get dimensions from actual images

// TODO: What other functions?
// TODO: Have a GUI?

