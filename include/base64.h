#ifndef __mm__base64__
#define __mm__base64__

//
//  base64.h
//
//  Created by Manfred Morgner on 25.04.15.
//  Copyright (c) 2015 Manfred Morgner. All rights reserved.
//
//  http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B_2

#include <string>


// Lookup table for encoding
const static char padCharacter   = '=';

template<typename T> std::string base64Encode(const T& inputBuffer)
{
    const static char encodeLookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encodedString("");
    encodedString.reserve(((inputBuffer.size()/3) + (inputBuffer.size() % 3 > 0)) * 4);
    int32_t temp;
    typename T::const_iterator cursor = inputBuffer.begin();
    for(size_t idx = 0; idx < inputBuffer.size()/3; idx++)
    {
        // Convert to big endian
        temp  = *cursor << 16; ++cursor;
        temp |= *cursor <<  8; ++cursor;
        temp |= *cursor;       ++cursor;
        // encode
        encodedString.append(1,encodeLookup[(temp & 0x00fc0000) >> 18]);
        encodedString.append(1,encodeLookup[(temp & 0x0003f000) >> 12]);
        encodedString.append(1,encodeLookup[(temp & 0x00000fc0) >>  6]);
        encodedString.append(1,encodeLookup[(temp & 0x0000003f)      ]);
    } // for(size_t idx = 0; idx < inputBuffer.size()/3; idx++)
    
    switch(inputBuffer.size() % 3)
    {
        case 1:
            // Convert to big endian
            temp  = *cursor << 16; ++cursor;
            // encode
            encodedString.append(1,encodeLookup[(temp & 0x00fc0000) >> 18]);
            encodedString.append(1,encodeLookup[(temp & 0x0003f000) >> 12]);
            encodedString.append(2,padCharacter);
            break;
        case 2:
            // Convert to big endian
            temp  = *cursor << 16; ++cursor;
            temp |= *cursor <<  8; ++cursor;
            // encode
            encodedString.append(1,encodeLookup[(temp & 0x00fc0000) >> 18]);
            encodedString.append(1,encodeLookup[(temp & 0x0003f000) >> 12]);
            encodedString.append(1,encodeLookup[(temp & 0x00000fc0) >>  6]);
            encodedString.append(1,padCharacter);
            break;
    } // switch(inputBuffer.size() % 3)
    return encodedString;
} // std::string base64Encode(...)


template<typename T> std::vector<char> base64Decode(const T& input)
{
    if (input.size() % 4) // Sanity check
        throw std::runtime_error("Non-Valid base64!");
    
    size_t padding = 0;
    if (input.size())
    {
        if (input[input.size()-1] == padCharacter) padding++;
        if (input[input.size()-2] == padCharacter) padding++;
    } // if (input.size())
    
    // Setup a vector to hold the result
    std::vector<char> decodedBytes(((input.size()/4)*3)-padding);
    int32_t temp=0; // Holds decoded quanta
    typename T::const_iterator cursor = input.begin();
    
    while (cursor != input.end())
    {
        for (size_t quantelPos = 0; quantelPos < 4; ++quantelPos)
        {
            // This area will need tweaking if you are using an alternate alphabet
            temp <<= 6;
            if      (*cursor >= 0x41 && *cursor <= 0x5A) temp |= *cursor - 0x41;
            else if (*cursor >= 0x61 && *cursor <= 0x7A) temp |= *cursor - 0x47;
            else if (*cursor >= 0x30 && *cursor <= 0x39) temp |= *cursor + 0x04;
            else if (*cursor == 0x2B)                    temp |= 0x3E; // change to 0x2D for URL alphabet
            else if (*cursor == 0x2F)                    temp |= 0x3F; // change to 0x5F for URL alphabet
            else if (*cursor == padCharacter) // pad
            {
                switch( input.end() - cursor )
                {
                    case 1: // One pad character
                        decodedBytes.push_back((temp >> 16) & 0x000000FF);
                        decodedBytes.push_back((temp >>  8) & 0x000000FF);
                        return decodedBytes;
                    case 2: // Two pad characters
                        decodedBytes.push_back((temp >> 10) & 0x000000FF);
                        return decodedBytes;
                    default:
                        throw std::runtime_error("Invalid Padding in Base 64!");
                } // switch( input.end() - cursor )
            }
            else
                throw std::runtime_error("Non-Valid Character in Base 64!");
            cursor++;
        } // for (size_t quantelPos = 0; quantelPos < 4; quantelPos++)
        
        decodedBytes.push_back((temp >> 16) & 0x000000FF);
        decodedBytes.push_back((temp >>  8) & 0x000000FF);
        decodedBytes.push_back((temp      ) & 0x000000FF);
    } //  // while (cursor < input.end())
    
    return decodedBytes;
} // std::vector<char> base64Decode(const std::string& input)

/* defined(__mm__base64__) */
#endif

