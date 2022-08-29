#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

ifstream timetable;
ifstream programs;
ofstream f_hex;

void hex_open(char * file);
void hex_close(void);
void hex_write(int address, unsigned int a, unsigned int b, unsigned int c);

int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        cout << "You're doing it wrong!" << endl;
        cout << "Should: " << argv[0] << "  timetable.txt program.txt out.hex" << endl;
        return 1;
    }

    timetable.open(argv[1], ifstream::in);
    programs.open(argv[2], ifstream::in);
    hex_open(argv[3]);

    if(!timetable)
    {
        cout << "Failed to open timetable file (code: 2)" << endl;
        return 2;
    }
    if(!programs)
    {
        cout << "Failed to open programs file (code: 3)" << endl;
        return 3;
    }
    if(!hex)
    {
        cout << "Failed to create or rewrite hex output file (code: 4)" << endl;
        return 4;
    }

    cout << "Hello!\n";

    unsigned int a = 0, b = 0, c = 0, d = 0; // temp vars for reading
    unsigned int i = 0, g = 0, x = 0; // index of current
    int hex_address = 0;
    char buffer[128]; // buffer of current string
    for(x=0; x<128; x++)
        buffer[x] = 0;

    //  Stage 1.
    //  Reading the timetable file

    while(!timetable.eof() && g < 6) // while the end of file is not reached
    {
        timetable.getline(buffer, 128); // read the current line from the file

        if(sscanf(buffer, "%u:%u:%u;%u", &a, &b, &c, &d) == 4) // if current line is not a delimiter
        {
            if(i++ != 24) // if the end of block is not reached
            {
                c |= d << 6; // join the bytes (system requirement)
                hex_write(hex_address, a, b, c); // write three bytes in HEX
                hex_address += 3;
            }
            else // if the end of block is reached
            {
                do
                {
                    timetable.getline(buffer, 128); // read the line...
                }
                while(buffer[0] != '-' || !timetable.eof()); // and check it for end of block
                i = 0;
                g++;
            }
        }
        else // if current line is delimiter
        {
            while(i++ != 24) {
                hex_write(hex_address, 255, 255, 255); // fill current section of EEPROM with ones (clear memory)
                hex_address += 3;
            }
            i = 0;
            g++;
        }
    }

    while (hex_address != 462) {
      hex_write(hex_address, 255, 255, 255);
      hex_address += 3;
    }

    // Stage 2.
    // Reading programs file

    g = 0;
    a = 0;
    b = 0;
    i = 0;
    c = 0;
    d = 0;
    hex_address = 462;
    unsigned int buf[12];


    while(g < 4) // while the end of file is not reached
    {
        d = 0;
        c = 0;
        for(x=0; x<128; x++)
            buffer[x] = 0; // clear string buffer

        programs.getline(buffer, 128); // read the line from file
        for(x=0; x<12; x++)
            buf[x] = 0; // clear prog buffer

        while(sscanf(buffer+d, "%u:%u;%n", &a, &b, &c) == 2 && i < 12) // while there is one more command in program
        {
            buf[i++] = b | (a ? (1<<7) : 0); // join bytes and write byte to prog buffer
            d+=c;
        }

        for(x=0; x<12; x+=3) {
            hex_write(hex_address, buf[x], buf[x+1], buf[x+2]); // flush the buffer to HEX
            hex_address += 3;
        }

        g++;
        i = 0;
    }


    // Stage 3.
    // Close hex file
    hex_close();

    return 0;
}

void hex_open(char * file)
{
    f_hex.open(file);
}

void hex_close(void)
{
    f_hex << ":00000001FF" << endl;
    f_hex.close();
}

void hex_write(int address, unsigned int a, unsigned int b, unsigned int c)
{
    f_hex << ":03";
    f_hex.width(4);
    f_hex.setf(ios::hex, ios::basefield);
    f_hex.fill('0');
    f_hex << right << address;
    f_hex << "00";

    f_hex.width(2);
    f_hex << (int) a;
    f_hex.width(2);
    f_hex << (int) b;
    f_hex.width(2);
    f_hex << (int) c;

    unsigned char sum = a + b + c + 3 + ((address >> 8) & 0x00FF) + (address & 0x00FF);

    f_hex.width(2);
    f_hex << ((~sum + 1) & 0xFF) << endl;
}
