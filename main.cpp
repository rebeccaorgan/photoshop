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
    Bitmap(uint32_t w, uint32_t h, Header head, InfoHeader i_head);
    Bitmap(const Bitmap& other);
    Bitmap& operator=(const Bitmap& other);
    ~Bitmap();

    uint8_t*** data;
    uint32_t width;
    uint32_t height;
    Header header;
    InfoHeader info_header;


  private:
};

// (1)  default constructor, creates a 1x1 image which is completely black and 
// opaque. Note that black and opaque implies an RGBA value of (0,0,0,0)
Bitmap::Bitmap(uint32_t w, uint32_t h, Header my_header, InfoHeader my_info_header) {
  printf("Inside default constructor.\n");

  // Saving dimensions as data members in the class
  // Copy over other variables besides data
  width = w; 
  height = h;
  header = my_header; // TODO: Overload constructor to be able to assign these
  info_header = my_info_header;

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
  header = other.header;
  info_header = other.info_header;

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
  header = other.header;
  info_header = other.info_header;

  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < width; x++) {
      for(int y = 0; y < height; y++) { 
        data[rgba][x][y] = other.data[rgba][x][y];  
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
Bitmap create(uint32_t width, uint32_t height, Header header, InfoHeader info_header) {
  printf("Inside create\n");
  Bitmap bmp(width, height, header, info_header); // Call constructor

  // TODO: Bool based on memory allocation failure and stuff
  return bmp;
 }

// (6) Lodad bmp
Bitmap load_bmp(const std::string& filename) { 
  printf("Inside load_bmp\n");

  // Instantiate an object of types both Header and InfoHeader
  Header header;
  InfoHeader info_header;

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

  // TODO: Why are width and height so big when I loaded the picture, saved it, then loaded my saved picture?
  printf("Data_size: %u, Data_offset: %u\n", header.data_size, header.data_offset);
  printf("Width: %u, Height: %u\n", info_header.width, info_header.height);
  
  // Make Bitmap image based on these dimensions
  Bitmap new_bmp = create(info_header.width, info_header.height, header, info_header);


  // Assign Header and InfoHeader
  
  // TODO: Switch statement. Get error for redefining datum.
  // Now you know bits per pixel, can create individual pixel value (datum) to 
  // be extracted from image and fed into matrix piecemeal. According to 
  // wikipedia. Typical values are 1, 4, 8, 16, 24, and 32. Also, 
  // rgba_cap determines how many channels to look for based on size of pixel
  // datum (how many bits per pixel)
  int rgba_cap;
  if(info_header.datum_size == 8) { // Assuming 8 bit indexed color, not grayscale
  // 3 bits for red, 3 for green, 2 for blue
    uint8_t datum;
    rgba_cap = 1;
  }
  else if(info_header.datum_size == 24) {
    uint32_t datum;
    rgba_cap = 3;
  }
  else if(info_header.datum_size == 32) {
    uint32_t datum;
    rgba_cap = 4;
  }

  // TODO: Is this redundant from above?
  // Single data piece to be fed into matrix piecewise
  uint8_t datum;  

  // Go to start of data and one by one store to matrix
  fseek(fp, header.data_offset, SEEK_SET);  // fseek returns the current offset 
  int row_count = new_bmp.height;
  int col_count = new_bmp.width;
  // Scanning left to right, top to bottom (raster scanning)
  for(int y = 0; y < row_count; y++) {
    for(int x = 0; x < col_count; x++) {
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

// (7) saves the Bitmap into a bitmap file. Returns true if the bitmap was
//  successfully written to disk and false otherwise.
bool save_bmp(Bitmap &bmp, const std::string& filename) {  // TODO: Why signature like this?
  printf("Inside save_bmp\n");

  // Change data_offset to 54 because we're going to store data after headers
  bmp.header.data_offset = 54;

  Header header = bmp.header;
  InfoHeader info_header = bmp.info_header;


  FILE *fp;
  fp = fopen(filename.c_str(), "w+"); // .c_str() turns std::str into c style char*

  

  // Write Header
  fwrite(&header.identifier, sizeof(header.identifier), 1, fp);
  fwrite(&header.data_size, sizeof(header.data_size), 1, fp);
  fwrite(&header.reserved1, sizeof(header.reserved1), 1, fp);
  fwrite(&header.reserved2, sizeof(header.reserved2), 1, fp);
  fwrite(&header.data_offset, sizeof(header.data_offset), 1, fp);
  // Write HeaderInfo
  fwrite(&info_header.header_size, sizeof(info_header.header_size), 1, fp);
  fwrite(&info_header.width, sizeof(info_header.width), 1, fp);
  fwrite(&info_header.height, sizeof(info_header.height), 1, fp);
  fwrite(&info_header.planes, sizeof(info_header.planes), 1, fp);
  fwrite(&info_header.datum_size, sizeof(info_header.datum_size), 1, fp);
  fwrite(&info_header.compression, sizeof(info_header.compression), 1, fp);
  fwrite(&info_header.image_size, sizeof(info_header.image_size), 1, fp);
  fwrite(&info_header.x_res, sizeof(info_header.x_res), 1, fp);
  fwrite(&info_header.y_res, sizeof(info_header.y_res), 1, fp);
  fwrite(&info_header.n_colors, sizeof(info_header.n_colors), 1, fp);
  fwrite(&info_header.important_colors, sizeof(info_header.important_colors), 1, fp);

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

  // TODO: Is this redundant from above?
  // Single data piece to be fed into matrix piecewise
  uint8_t datum;  

  // Go to start of data and one by one store from matrix
  fseek(fp, header.data_offset, SEEK_SET);  // Should already be here
  int row_count = info_header.height;
  int col_count = info_header.width;
  // Order is raster scan, left to right, top to bottom
  for(int y = 0; y < row_count; y++) {
    for(int x = 0; x < col_count; x++) {
      for(int rgba = 0; rgba < rgba_cap; rgba++) {  // alpha value is 0 here
        fwrite(&bmp.data[rgba][x][y], sizeof(bmp.data[rgba][x][y]), 1, fp);
        // fprintf(fp, "%d", bmp.data[rgba][x][y]);
      }
    }
  }
  fclose(fp);
}

// (8) clears every pixel in the Bitmap image to the given color.
Bitmap clear(Bitmap &bmp, uint8_t r , uint8_t g, uint8_t b, uint8_t a) {
  printf("Inside clear\n");

  // Store the rgba parameters in an array in same order as that in Bitmap data
  // Note: blue and red order is inverted to match bitmap file type
  uint8_t rgba_array[4];
  rgba_array[0] = b;
  rgba_array[1] = g;
  rgba_array[2] = r;
  rgba_array[3] = a;

  // Assign all values 
  for(int y = 0; y < bmp.height; y++) {
    for(int x = 0; x < bmp.width; x++) {
      for(int rgba = 0; rgba < rgba_vals; rgba++) {
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
  int x_mid; 
  if(w % 2) { // height is odd
    x_mid = (w + 1) / 2;
  }
  else { // height is even
    x_mid = w / 2;
  }

  // Assign all values 
  for(int y = 0; y < h; y++) {
    for(int x = 0; x <= x_mid; x++) {
      for(int rgba = 0; rgba < rgba_vals; rgba++) {
        temp = bmp.data[rgba][x][y];
        bmp.data[rgba][x][y] = bmp.data[rgba][w-1-x][y];
        bmp.data[rgba][w-1-x][y] = temp;
//        printf("Temp: %d, x: %d, oppoxite x: %d, y: %d\n", temp, x, w-1-x, y); 
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
  uint32_t h = bmp.height;
  int y_mid; // halfway mark; stopping point for why in inner for loop
  if(h % 2) { // height is odd
    y_mid = (h + 1) / 2;
  }
  else { // height is even
    y_mid = h / 2;
  }

  // Assign all values 
  for(int y = 0; y <= y_mid; y++) {
    for(int x = 0; x < w; x++) {
      for(int rgba = 0; rgba < rgba_vals; rgba++) {
        temp = bmp.data[rgba][x][y];
        bmp.data[rgba][x][y] = bmp.data[rgba][x][h-1-y]; 
        bmp.data[rgba][x][h-1-y] = temp;
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
  uint32_t neighbor_pixel_sum;
  //TODO: Just have temp_bmp and have new values go over to that one. Deep copy
  // it over at end to bmp that gets returned
  // TODO: Borders

  // All but borders
  for(int y = 1; y < h - 1; y++) {
    for(int x = 1; x < w - 1; x++) { // TODO: confirm -1 
      for(int rgba = 0; rgba < rgba_vals; rgba++) {

        neighbor_pixel_sum = bmp.data[rgba][x-1][y+1] + bmp.data[rgba][x][y+1] +
               bmp.data[rgba][x+1][y+1] + bmp.data[rgba][x-1][y] +
               bmp.data[rgba][x][y] + bmp.data[rgba][x][y+1] + 
               bmp.data[rgba][x-1][y-1] + bmp.data[rgba][x][y-1] +
               bmp.data[rgba][x+1][y-1];
        temp_bmp.data[rgba][x][y] = neighbor_pixel_sum / 9;
      }
    }
  }

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
  Header header = orig_bmp.header;
  InfoHeader info_header = orig_bmp.info_header;
  float percentage = transparency / (float) 256;

  Bitmap blend_bmp = create(w, h, header, info_header);  

  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) {
      for(int y = 0; y < h; y++) {
        blend_bmp.data[rgba][x][y] = (int) (orig_bmp.data[rgba][x][y]*(1-percentage)) + 
                                (other_bmp.data[rgba][x][y]*percentage);
      }
    }
  }
  
  return blend_bmp;
}

Bitmap scale(const Bitmap &bmp, float magnitude) {

  Bitmap new_bmp = create(bmp.width * magnitude, (bmp.height * magnitude), bmp.header, bmp.info_header);
  
  uint32_t h = bmp.height * magnitude;
  uint32_t w = bmp.width * magnitude;
  // Update header file
  new_bmp.height = h;
  new_bmp.width = w;
  new_bmp.info_header.height = h;
  new_bmp.info_header.width = w;

  for(int rgba = 0; rgba < rgba_vals; rgba++) {
    for(int x = 0; x < w; x++) { 
      for(int y = 0; y < h; y++) {
        new_bmp.data[rgba][x][y] = bmp.data[rgba][int(x/magnitude)][int(y/magnitude)];
      }
    }
  }

  return new_bmp;
}

// (13) prints out the matrix values of an image
void print_data(Bitmap &bmp) { 
  printf("Inside print data\n");
  
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

// (14) prints out the header values of an image
void print_header(Bitmap &bmp) { 
  printf("Inside print header\n");

  Header header = bmp.header;
  InfoHeader info_header = bmp.info_header;

  // Header
  printf("identifier: %u\n"
          "data_size: %u\n"
          "reserved1: %u\n"
          "reserved2: %u\n"
          "data_offset: %u\n", 
          header.identifier, header.data_size, header.reserved1, 
          header.reserved2, header.data_offset);
  // InfoHeader
  printf("header_size: %u\n"
         "width: %d\n"
         "height: %d\n"
         "planes: %u\n"
         "datum_size: %u\n"
         "compression: %u\n"
         "image_size: %u\n"
         "x_res: %d\n"
         "y_res: %d\n"
         "n_colors: %u\n"
         "important_colors: %u\n",
         info_header.header_size, info_header.width, info_header.height, 
         info_header.planes, info_header.datum_size, info_header.compression,
         info_header.image_size, info_header.x_res, info_header.y_res,
         info_header.n_colors, info_header.important_colors);
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

  Bitmap loaded_bmp = load_bmp("picture3.bmp");
//  Bitmap loaded_bmp = load_bmp("picture3.bmp");
//  Bitmap loaded_bmp2 = load_bmp("picture.bmp");
//  print_header(loaded_bmp);
//  loaded_bmp = clear(loaded_bmp, 128, 0, 0, 0);
//  loaded_bmp = vertical_flip(loaded_bmp);
//  loaded_bmp = horizontal_flip(loaded_bmp);
//  loaded_bmp = blur(loaded_bmp);
//  loaded_bmp = blend(loaded_bmp1, loaded_bmp2, 128);
    Bitmap scaled_bmp = scale(loaded_bmp, 10);
  save_bmp(scaled_bmp, "saved_picture.bmp");

  //Bitmap blur_bmp = blur(edge_deciles_bmp);
//  Bitmap blended_bmp = blend(ones_bmp, deciles_bmp, 128);
//  print_data(blended_bmp);
  //save(deciles_bmp, "deciles.txt"); 

  return 0;
}

// TODO: all todos

// TODO: What other functions? set_opacity_percentage()
// TODO: Have blend function blend the boundaries / edges
// TODO: histogram
// TODO: edge detection

// TODO: Have a GUI?

// TODO: Load picture2.bmp and work with other bitsperpixel

