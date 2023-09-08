/*******************************************************************************
* C++ Name: syx2standard.cpp
*
* Description: This program opens the input syx file, parses it and strips the 
*              midi messages that not conform to the Sample Dump Standard, which
*              in this case are just an header message and data packet.
*              The stripped file is saved using the output file name passed as
*              second argument. 
*              Compile with: g++ -o syx2standard.out -std=c++2a syx2standard.cpp
*
* Author: Andrew Bloom
* License: MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* For more details, please refer to the standard MIT LICENSE description online.
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

class Manip {
public:
    friend std::ostream &operator<<(std::ostream & os, Manip manip) { os << std::right << std::setfill('0') << std::setw(2) << std::hex; return os; };
};

constexpr uint16_t BUFSIZE = 256;
constexpr uint8_t endByte = 0xf7;
constexpr uint8_t sysExByte = 0xF0;
constexpr uint8_t ID_RealTime_Byte = 0x7F;
constexpr uint8_t ID_Non_RealTime_Byte = 0x7E;
constexpr uint8_t Generic_Byte = 0x00;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    const std::string inputFileName = argv[1];
    const std::string outputFileName = argv[2];

    std::ifstream input(inputFileName, std::ios::binary);

    if (!input.is_open()) {
        std::cerr << "Error opening input file: " << inputFileName << std::endl;
        return 1;
    }

    std::ofstream output(outputFileName);

    if (!output.is_open()) {
        std::cerr << "Error opening output file: " << outputFileName << std::endl;
        return 1;
    }

	uint8_t buf[BUFSIZE];
	uint8_t format;
	uint8_t nbytes;
	
	while (!input.eof()) {
		// search first sysex byte
	    while (input.read(reinterpret_cast<char*>(buf), sizeof(uint8_t))) if (buf[0]==sysExByte) break;
		// id number
		input.read(reinterpret_cast<char*>(buf+1), sizeof(uint8_t));
		switch (buf[1]) {
				case ID_RealTime_Byte:
						std::cout << "SysEx Message Realtime found" << std::endl;
	    				while (input.read(reinterpret_cast<char*>(buf), sizeof(uint8_t))) if (buf[2] == endByte) break;
						break;
				case ID_Non_RealTime_Byte:
						input.read(reinterpret_cast<char*>(buf+2), sizeof(uint8_t)); // device_id
						input.read(reinterpret_cast<char*>(buf+3), sizeof(uint8_t)); // command
						switch (buf[3]) {
							case 0x01:
							// DUMP HEADER
							// std::cout << "SysEx Message Sample Dump Header found: " << std::endl;
							input.read(reinterpret_cast<char*>(buf+4), sizeof(uint8_t)*2); // Sample Number
							input.read(reinterpret_cast<char*>(buf+6), sizeof(uint8_t)); // Sample Format
							input.read(reinterpret_cast<char*>(buf+7), sizeof(uint8_t)*3); // Sample Period
							input.read(reinterpret_cast<char*>(buf+10), sizeof(uint8_t)*3); // Sample Length in words
							input.read(reinterpret_cast<char*>(buf+13), sizeof(uint8_t)*3); // Sustain loop start point word number
							input.read(reinterpret_cast<char*>(buf+16), sizeof(uint8_t)*3); // Sustain loop end point word number
							input.read(reinterpret_cast<char*>(buf+19), sizeof(uint8_t)); // Sample Length in words
																							
	    					input.read(reinterpret_cast<char*>(buf+20), sizeof(uint8_t));
							if (buf[20] == endByte) {
								output.write(reinterpret_cast<char*>(buf), 21);
								break;
							} else {
								std::cout << "Error, endByte not found. " << Manip() << buf[20] << " != " << Manip() << endByte << std::endl;
							}
							break;
							case 0x02:
							// DATA PACKET
							input.read(reinterpret_cast<char*>(buf+4), sizeof(uint8_t)); // packet count
							// std::cout << "SysEx Message Sample Dump Data Paket found, packet count: " << Manip() << (int) buf[4] << std::endl;
							input.read(reinterpret_cast<char*>(buf+5), sizeof(uint8_t)*120); // 120 bytes payload
							input.read(reinterpret_cast<char*>(buf+125), sizeof(uint8_t)); // checksum
							input.read(reinterpret_cast<char*>(buf+126), sizeof(uint8_t)); // endbyte
							if (buf[126] != endByte) std::cout << "Error! this should be an endbyte F7! Byte found: " << Manip() << (int) buf[126] << std::endl;
							output.write(reinterpret_cast<char*>(buf),127);
							break;
						};
						break;
				case Generic_Byte:
						std::cout << "SysEx Message id number (00, generic) found: " << std::endl;
	    				while (input.read(reinterpret_cast<char*>(buf), sizeof(uint8_t))) if (buf[0] == endByte) break;
						break;
				default:
					std::cout << "SysEx Message id number unknown: " << Manip() << (int) buf[1] << std::endl;
					break;
		}
	
	
	};

    input.close();
    output.close();

	std::cout << "Done." << std::endl;
	
};

