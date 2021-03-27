#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

const int BUFFERSIZE = 65536;

void char2short(unsigned char* pchar, unsigned short* pshort)
{
  *pshort = (pchar[0] << 8) | pchar[1];
}


int main(int argc, char **argv) {

    std::string sidfname;

    if (argc > 1)
        {
        //strcpy(sidfname, argv[1]);
        sidfname = argv[1];
        }
    else
        {
            std::cout << "Convertsid v1.0" <<std::endl;
            std::cout << "Usage: Convertsid [filename]" <<std::endl;
            exit(0);
        }
   
    std::ifstream sidfile;
    sidfile.open( sidfname, std::ifstream::in | std::ifstream::binary );

    if (!sidfile) {
        std::cout << "Error: could not open file " << sidfname << std::endl;
        return -1;
        }

    // copies all data into buffer
    std::vector<unsigned char> siddata(std::istreambuf_iterator<char>(sidfile), {});
    sidfile.close();

    int bytes = siddata.size();
    std::cout << "Bytes read: " << bytes <<std::endl;

    char header[4];
    memcpy(header, siddata.data(), sizeof header);
    header[4]=0;
    std::cout << "Header: " << header <<std::endl;

    short int version = ((siddata[4] << 8) | siddata[5]);
    std::cout << "version: " << version <<std::endl;

    short int dataOffset = ((siddata[6] << 8) | siddata[7]);
    std::cout << "dataOffset: " << dataOffset <<std::endl;

    short int loadAddress = ((siddata[8] << 8) | siddata[9]);
    if (loadAddress == 0)
    {
        std::cout << "LoadAddress from real c64 data." <<std::endl;
        loadAddress = ((siddata[dataOffset+1] << 8) | siddata[dataOffset]);
    }
    std::cout << "loadAddress: $" << std::hex << loadAddress <<std::endl;

    short int initAddress = ((siddata[10] << 8) | siddata[11]);
    std::cout << "initAddress: $" << std::hex << initAddress <<std::endl;

    short int playAddress = ((siddata[12] << 8) | siddata[13]);
    std::cout << "playAddress: $" << std::hex << playAddress <<std::endl;

    int datasize = bytes - dataOffset;
    std::cout << "datasize: " << std::dec << datasize << std::endl;


    int pointloc = sidfname.find('.');
    sidfname.replace(pointloc+1,3,"dat");
    std::cout << "outfile:" << sidfname << std::endl;

    std::ofstream outfile;
    outfile.open( sidfname, std::ifstream::out | std::ifstream::binary );
    if (!outfile) {
        std::cout << "Error: could not write to file: " << sidfname << std::endl;
        return -1;
        }
    outfile.write((char*)&siddata[dataOffset], datasize);
    outfile.close();

    return 0;
}
