#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int Main() {
    ofstream outFile("trace.txt");
    srand(time(0));

    int baseAddresses[] = { 0x1000, 0x2000, 0x3000, 0x4000, 0x5000 };

    for (int i = 0; i < 100000; i++) {
        int address;
        // 80% of the time, reuse the same 3 pages (Locality!)
        if (rand() % 100 < 80) {
            address = baseAddresses[rand() % 3];
        }
        // 20% of the time, access a random page
        else {
            address = baseAddresses[rand() % 5];
        }

        char op = (rand() % 100 < 70) ? 'R' : 'W'; // 70% reads, 30% writes

        outFile << "0x" << hex << address << " " << op << "\n";
    }

    outFile.close();
    cout << "Success! New trace.txt generated with Locality of Reference.\n";
    return 0;
}