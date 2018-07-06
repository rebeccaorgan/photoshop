#include "image.h"
#include <string>
#include <stdlib.h> // malloc
#include <stdio.h> // fprintf

// Triple index array. The respective indices are for the specific RGBA channel,
// row and column. These bitmaps are all 32 bit RGBA, and will thus have 4 
// channels. The canvas size of the image will dictate the exact size of the 
// other two dimensions

int rgba_vals = 4; // TODO: Okay to have global?
int rgb_vals = 3;  

// Header
struct HeaderStruct{
  uint16_t identifier; // Magic identifier for BMP and DIB file
  uint32_t data_size;  // Int containing size of data
  uint16_t reserved1; // Value depends on app creating image
  uint16_t reserved2; // Value depends on app creating image
  uint32_t data_offset;  // In bytes; Where image data starts
};
typedef HeaderStruct Header;

// Info Header
struct InfoHeaderStruct{
  uint32_t header_size; // In bytes
  int32_t width;  // Int for image width
  int32_t height;  // Int for image height
  uint16_t planes; // Number of color planes
  uint16_t datum_size; // bits per pixel
  uint32_t compression; // Compression type
  uint32_t image_size; // In bytes
  int32_t x_res;
  int32_t y_res;
  uint32_t n_colors; // Number of colors
  uint32_t important_colors;
};
typedef InfoHeaderStruct InfoHeader;


class Bitmap{
  public:
    Bitmap(uint32_t w, uint32_t h);
    Bitmap(const Bitmap& other);
    Bitmap& operator=(const Bitmap& other);
    ~Bitmap();

    uint8_t*** data;
    uint32_t width;
    uint32_t height;


  private:
};

// (1)  default constructor, creates a 1x1 image which is completely black and 
// opaque. Note that black and opaque implies an RGBA value of (0,0,0,0)
Bitmap::Bitmap(uint32_t w, uint32_t h) {
  printf("Inside default constructor\n");

  // Saving dimensions as data members in the class
  // Copy over other variables besides data
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
Bitmap::Bitmap(const Bitmap& other) {  // No dimensions because other is already created 
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
Bitmap& Bitmap::operator=(const Bitmap& other) { 
  printf("Inside assignment operator\n");

  //TODO: Check if is same size. Implement destructor and copy constructor?

  // Copy over other variables besides data
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
Bitmap::~Bitmap() { 
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
//and opaque implies an RGBA value of (0,0,0,0). Returns true if the Bitmap was 
//created and false if the creation fails (eg, due to memory allocation failure).
Bitmap create(uint32_t width, uint32_t height) {
  printf("Inside create\n");
  Bitmap bmp(width, height); // Call constructor

  // TODO: Bool based on memory allocation failure and stuff
  return bmp;
 }

// (6) loads a Bitmap into the object. Returns true if the bitmap was
//  successfully loaded and false otherwise.
Bitmap load(const std::string& filename) { 
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
  int col_count = 0;
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
  Bitmap bmp = create(h, w);

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

// (7) saves the Bitmap into a bitmap file. Returns true if the bitmap was
//  successfully written to disk and false otherwise.
bool save(Bitmap &bmp, const std::string& filename) {  // TODO: Why signature like this?
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


// (8) clears every pixel in the Bitmap image to the given color.
Bitmap clear(Bitmap &bmp, uint8_t r , uint8_t g, uint8_t b, uint8_t a) {
  printf("Inside clear\n");

  // Store the rgba parameters in an array in same order as that in Bitmap data
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
Bitmap horizontal_flip(Bitmap &bmp) { 
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
Bitmap vertical_flip(Bitmap &bmp) {
  printf("Inside vertical_flip()\n");

  uint8_t temp; // rgba value
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
Bitmap blur(Bitmap &bmp) { 
  printf("Inside blur\n");

  Bitmap temp_bmp = bmp;

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
Bitmap blend(const Bitmap &orig_bmp, const Bitmap& other_bmp, uint8_t transparency) { 
  printf("Inside blend\n");

  //TODO: Check the two are same dimensions
  uint32_t h = orig_bmp.height;
  uint32_t w = orig_bmp.width;
  float percentage = transparency / (float) 256;

  Bitmap blend_bmp = create(w, h);  

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
void print(Bitmap &bmp) { 
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

// Then assess them in a meaningful order
Bitmap load_bmp(const std::string& filename) { 
  printf("Inside load_bmp\n");

  // Instantiate an object of types both Header and InfoHeader
  Header header;
  InfoHeader info_header;
  
  // Other
  uint8_t datum;  // Single data piece to be fed into matrix piecewise

  // Open the file
  FILE *fp;
  fp = fopen(filename.c_str(), "r"); // .c_str() turns std::str into c style char*

  // Read in all header values 
  fread(&header.identifier, sizeof(header.identifier), 1, fp);
  fread(&header.data_size, sizeof(header.data_size), 1, fp);
  fread(&header.reserved1, sizeof(header.reserved1), 1, fp);
  fread(&header.reserved2, sizeof(header.reserved2), 1, fp);
  fread(&header.data_offset, sizeof(header.data_offset), 1, fp);
  fread(&info_header.header_size, sizeof(info_header.header_size), 1, fp);
  fread(&info_header.width,sizeof(info_header.width), 1, fp);
  fread(&info_header.height, sizeof(info_header.height), 1, fp);
  fread(&info_header.planes, sizeof(info_header.planes), 1, fp);
  fread(&info_header.datum_size, sizeof(info_header.datum_size), 1, fp);
  fread(&info_header.compression, sizeof(info_header.compression), 1, fp);
  fread(&info_header.image_size, sizeof(info_header.image_size), 1, fp);
  fread(&info_header.x_res, sizeof(info_header.x_res), 1, fp);
  fread(&info_header.y_res, sizeof(info_header.y_res), 1, fp);
  fread(&info_header.n_colors, sizeof(info_header.n_colors), 1, fp);
  fread(&info_header.important_colors, sizeof(info_header.important_colors), 1, fp);

  // Make Bitmap image based on these dimensions
  Bitmap new_bmp = create(info_header.width, info_header.height);
  
  // TODO: Switch statement. Get error for redefining datum.
  // Now you know bits per pixel, can create individual pixel value (datum) to 
  // be extracted from image and fed into matrix piecemeal. According to 
  // wikipedia. Typical values are 1, 4, 8, 16, 24, and 32. Also, 
  // rgba_cap determines how many channels to look for based on size of pixel
  // datum (how many bits per pixel)
  int rgba_cap;
  if(info_header.datum_size == 8) { // Assuming 8 bit indexed color, not grayscale
    uint8_t datum;
    rgba_cap = 3;
  }
  else if(info_header.datum_size == 24) {
    uint32_t datum;
    rgba_cap = 3;
  }
  else if(info_header.datum_size == 32) {
    uint32_t datum;
    rgba_cap = 4;
  }

  // Go to start of data and one by one store to matrix
  fseek(fp, header.data_offset, SEEK_SET);  // fseek returns the current offset 
  int row_count = new_bmp.height;
  int col_count = new_bmp.width;
  for(int x = 0; x < row_count; x++) {
    for(int y = 0; y < col_count; y++) {
      for(int rgba = 0; rgba < rgba_cap; rgba++) {  // alpha value is 0 here
//        printf("Location within pointer: %ld\n", ftell(fp));
        fread(&datum, 1, 1, fp);  // div by 8 to convert to bytes, by 4 for rgba component
//        printf("%d\n", datum);
        new_bmp.data[rgba][x][y] = (float) datum;
      }
    } 
  }
  
  // Print some relevant stats
  printf("----Bits per pixel (datum_size): %d\n", info_header.datum_size);
  printf("----Measured data size: %d\n", ftell(fp)-header.data_offset);
  printf("----Width: %d\n", info_header.width);
  printf("----Height: %d\n", info_header.height);
  fclose(fp);

  return new_bmp;
}

// executive entrypoint
int main(int argc, char** argv) {

  // instantiate a copy of the image object type
  //image img;
//  Bitmap zeros_bmp = create(5, 6);
//  Bitmap ones_bmp = clear(zeros_bmp, 1, 1, 1, 1);
//  Bitmap deciles_bmp = clear(zeros_bmp, 10, 20, 30, 40);
//  Bitmap edge_deciles_bmp = load("edges.txt");

  //my_bmp = vertical_flip(loaded_bmp);
  //my_bmp = horizontal_flip(loaded_bmp);

  Bitmap loaded_bmp = load_bmp("picture.bmp");
//  print(loaded_bmp);
//  save(loaded_bmp, "saved_picture.bmp");

  //Bitmap blur_bmp = blur(edge_deciles_bmp);
//  Bitmap blended_bmp = blend(ones_bmp, deciles_bmp, 128);
//  print(blended_bmp);
  //save(deciles_bmp, "deciles.txt"); 

  return 0;
}

// TODO: all todos

// TODO: Use real images
// TODO: Have load get dimensions from actual images

// TODO: What other functions? set_opacity_percentage()
// TODO: Have a GUI?

