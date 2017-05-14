//
//  main.cpp
//
//  Created by Manfred Morgner on 25.04.15.
//  Copyright (c) 2015 Manfred Morgner. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "base64.h"

const static char B64_MODE_ENCODE = 0;
const static char B64_MODE_DECODE = 1;

void Encode()
  {
  char c;
  std::vector<char> v(3);

  while ( std::cin.get(c) )
    {
    v.push_back(c);
    if ( v.size() == 3 )
      {
      std::cout << base64Encode(v);
      v.clear();
      }
    }
  if ( v.size() > 0) std::cout << base64Encode(v);
  } // void Encode()

void Decode()
  {
  char c;
  std::vector<char> v(4);

  while ( std::cin.get(c) )
    {
    v.push_back(c);
    if ( v.size() == 4 )
      {
      for (auto o : base64Decode(v)) std::cout << o;
      v.clear();
      }
    }
  } // void Decode()

int main(int argc, const char * argv[])
  {
  char cMode = B64_MODE_ENCODE;
  for ( int a=0; a<argc; ++a) if ( argv[a] == (std::string)"-D" ) cMode = B64_MODE_DECODE;

  if (cMode == B64_MODE_ENCODE)
    Encode();
  else
    Decode();

  return 0;
  }
