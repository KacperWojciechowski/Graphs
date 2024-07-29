// this
#include <Graphs/Pixel_map.hpp>

// libraries
#include <fstream>
#include <iostream>

/*
        Function generating a randomized map of pixels in given
        file path.

        Params:
        file_path - path to the output file
        length - length of the map in pixels
        width - width of the map in pixels

        Return:
        None
*/
void Data::randomize_map(std::string file_path, uint32_t length,
                         uint32_t width) {
  std::fstream file;
  file.open(file_path, std::ios::out);
  srand(static_cast<unsigned int>(time(NULL)));

  uint32_t sector;

  // generate subsequent pixels in each row
  for (uint32_t i = 0; i < length; i++) {
    for (uint32_t j = 0; j < width; j++) {
      sector = rand() % 2;
      file << sector;
      if (j < width - 1) {
        file << ' ';
      }
    }
    // at the end of the line insert new line character
    if (i < length - 1) {
      file << std::endl;
    }
  }
  file.close();
}

/*
        Constructor reading provided file to load the map of pixels
        in a matrix form. In case of passing a path to missing file,
        returns a default empty object.

        Params:
        file_path - path to the map file

        Return:
        None
*/
Data::Pixel_map::Pixel_map(std::string file_path)
    : area_map(nullptr), rows(0), columns(0) {
  std::ifstream file(file_path, std::ios::in);
  if (file.good()) {
    std::string line;

    std::getline(file, line);
    size_t offset = 0;
    size_t pos = 0;

    // parse the line to calculate the columns count
    while (true) {
      // search for space character that's inbetween
      // each pixel
      pos = line.find(' ', offset);
      if (pos != std::string::npos) {
        this->columns++;
        offset = pos + 1;
      }
      // when no more spaces are found, add the last column
      // to the column count
      else {
        this->columns++;
        break;
      }
    }
    // count the row that was just searched
    this->rows++;

    // count subsequent rows
    while (!file.eof() && line != "") {
      this->rows++;
      std::getline(file, line);
    }

    // create sufficiently big matrix
    this->area_map = new uint8_t *[this->rows];

    for (uint32_t i = 0; i < this->rows; i++) {
      this->area_map[i] = new uint8_t[this->columns];
    }

    // move to the beginning of the file
    file.seekg(std::ios_base::beg);

    uint32_t temp;

    // load the matrix from file
    for (uint32_t i = 0; i < this->rows; i++) {
      for (uint32_t j = 0; j < this->columns; j++) {
        file >> temp;
        this->area_map[i][j] = static_cast<uint8_t>(temp);
      }
    }
  }
  // if given file could not be opened correctly
  else {
    std::cout << "File missing" << std::endl;
  }
}

/*
        Function finding the vertex index of given matrix coordinates for
   creation of graph based on the pixel map. In case of not finding the pixel
   with given coordinates, 0xFFFFFFFF is returned as an indication.

        Params:
        nodes	- reference to the vector of coordinates of each non-wall (0)
   field in the pixel map x		- X coordinate of the pixel in the
   matrix y		- Y coordinate of the pixel in the matrix
*/
uint32_t Data::find_index(const std::vector<Data::coord> &nodes, uint32_t x,
                          uint32_t y) {
  uint32_t size = static_cast<uint32_t>(nodes.size());

  // default value for the return
  uint32_t ret = 0xFFFFFFFF;

  // search though each of the pixel coordinates
  for (uint32_t i = 0; i < size; i++) {
    if (nodes[i].x == x && nodes[i].y == y) {
      ret = i;
      break;
    }
  }
  return ret;
}

/*
        Function displaying the map on the screen.

        Params:
        None

        Return:
        None
*/
void Data::Pixel_map::print_area_map() {
  std::cout << "[" << std::endl;
  for (uint32_t i = 0; i < this->rows; i++) {
    for (uint32_t j = 0; j < this->columns; j++) {
      std::cout << static_cast<uint32_t>(this->area_map[i][j]);
      if (j < this->columns - 1) {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << "]" << std::endl;
}

/*
        Rows count getter

        Params:
        None

        Return:
        rows count
*/
uint32_t Data::Pixel_map::get_rows() const { return this->rows; }

/*
        Columns count getter

        Params:
        None

        Return:
        columns count
*/
uint32_t Data::Pixel_map::get_columns() const { return this->columns; }

/*
        Getter for the pixel value of the map.

        Params:
        x - X coordinate of the matrix
        y - Y coordinate of the matrix

        Return:
        value of the pixel
*/
uint32_t Data::Pixel_map::get_field(uint32_t x, uint32_t y) const {
  return static_cast<uint32_t>(this->area_map[x][y]);
}

/*
        Destructor cleaning up the memory allocation
*/
Data::Pixel_map::~Pixel_map() {
  for (uint32_t i = 0; i < this->rows; i++) {
    delete[] this->area_map[i];
  }
  delete[] this->area_map;
}
