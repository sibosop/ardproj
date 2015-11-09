
#include <iostream>
#include <assert.h>
#include <iomanip>
#include "BITFILE___"

int
main(int argc, char *argv[])
{
#if 0
  std::cout <<
  "#include <stdint.h>\n"
  "typedef uint8_t byte;\n"
  "#include \"../../sketches/libraries/LedMatrix/BitMap.h\"\n";
#endif
  assert(argc==2);
  
  std::cout << "const BitMap " << argv[1] << " = {\n"; 
#if 0
  for (int i = 0; i < gimp_image.height; ++i)
  {
    for ( int j = 0; j < (gimp_image.width); ++j )
    {
      std::cout << std::hex 
        << "0x" 
        << (int)gimp_image.pixel_data[j * 3]
        << ",0x"
        << (int)gimp_image.pixel_data[(j * 3)+1];
        if ( j != (gimp_image.width - 1) )
            std::cout << ","
        ;
    }
    std::cout << std::endl;
  }
#else
  int count = 1;
  int rowc = 0;
  for ( int i = 0; rowc < 128; ++i )
  {
      if (count % 3)
      {
        std::cout << "0x";
        std::cout 
          << std::hex 
          << std::setw(2) 
          << std::setfill('0') 
          << (int)(gimp_image.pixel_data[i]/8);
        ++rowc;
        if ( rowc != 128 )
          std::cout << ",";
        if (!(rowc % 16))
          std::cout << std::endl;
      }            
      ++count;
  }
#endif
  std::cout << "};\n";
}